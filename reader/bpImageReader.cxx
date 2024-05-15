/***************************************************************************
 *   Copyright (c) 2024-present Bitplane AG Zuerich                        *
 *                                                                         *
 *   Licensed under the Apache License, Version 2.0 (the "License");       *
 *   you may not use this file except in compliance with the License.      *
 *   You may obtain a copy of the License at                               *
 *                                                                         *
 *       http://www.apache.org/licenses/LICENSE-2.0                        *
 *                                                                         *
 *   Unless required by applicable law or agreed to in writing, software   *
 *   distributed under the License is distributed on an "AS IS" BASIS,     *
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or imp   *
 *   See the License for the specific language governing permissions and   *
 *   limitations under the License.                                        *
 ***************************************************************************/


#include "ImarisReader/interface/bpImageReader.h"
#include "ImarisReader/reader/bpImageReaderImpl.h"

#include "ImarisReader/utils/bpfH5LZ4.h"
#include "ImarisReader/utils/bpfFileTools.h"

#include "hdf5.h"

#include <iostream>
#include <mutex>

const bpfString mDataSetDirectoryName = "DataSet";
const bpfString mDataSetInfoDirectoryName = "DataSetInfo";
const bpfString mThumbnailDirectoryName = "Thumbnail";

using namespace bpConverterTypes;

void CloseFile(hid_t& aFileId)
{
  if (aFileId > 0) {
    H5Fclose(aFileId);
  }

  aFileId = 0;
}


bool ReadAttributeString(const bpfString aAttributeName, bpfString& aAttributeValue, const hid_t& aAttributeLocation)
{
  auto vExists = H5Aexists(aAttributeLocation, aAttributeName.c_str());

  if (vExists <= 0) {
    BP_DEBUG_MSG("ReadAttribute() - Could not find attribute with name: " + aAttributeName);
    return false;
  }

  hid_t vAttributeId = H5Aopen_by_name(aAttributeLocation, ".", aAttributeName.c_str(), H5P_DEFAULT, H5P_DEFAULT);

  if (vAttributeId < 0) {
    BP_DEBUG_MSG("ReadAttribute() - Could not find attribute with name: " + aAttributeName);
    return false;
  }

  hid_t vAttributeTypeId = H5Aget_type(vAttributeId);

  if (vAttributeTypeId < 0) {
    H5Aclose(vAttributeId);
    BP_DEBUG_MSG("ReadAttribute() - Could not get datatype from attribute" + aAttributeName);
    return false;
  }

  hid_t vAttributeSpaceId = H5Aget_space(vAttributeId);

  if (vAttributeSpaceId < 0) {
    H5Tclose(vAttributeTypeId);
    H5Aclose(vAttributeId);
    BP_DEBUG_MSG("ReadAttribute() - Could not get dataspace from attribute" + aAttributeName);
    return false;
  }

  bpfInt32 vAttributeRank = H5Sget_simple_extent_ndims(vAttributeSpaceId);

  if (vAttributeRank != 1) {
    H5Sclose(vAttributeSpaceId);
    H5Tclose(vAttributeTypeId);
    H5Aclose(vAttributeId);
    BP_DEBUG_MSG("ReadAttribute() - Dataspace rank of attribute " + aAttributeName + " is not 1!");
    return false;
  }

  hsize_t vAttributeSize = 0;

  if (H5Sget_simple_extent_dims(vAttributeSpaceId, &vAttributeSize, nullptr) < 0) {
    H5Sclose(vAttributeSpaceId);
    H5Tclose(vAttributeTypeId);
    H5Aclose(vAttributeId);
    BP_DEBUG_MSG("ReadAttribute() - Could not get size of attribute " + aAttributeName);
    return false;
  }

  bpfChar* vBuffer = new bpfChar[(bpfSize)vAttributeSize+1];
  vBuffer[vAttributeSize] = '\0';

  if (H5Aread(vAttributeId, vAttributeTypeId, vBuffer) < 0) {
    delete[] vBuffer;
    H5Sclose(vAttributeSpaceId);
    H5Tclose(vAttributeTypeId);
    H5Aclose(vAttributeId);
    BP_DEBUG_MSG("ReadAttribute() - Could not read attribute " + aAttributeName + "!");
    return false;
  }

  aAttributeValue = vBuffer;

  delete[] vBuffer;
  H5Sclose(vAttributeSpaceId);
  H5Tclose(vAttributeTypeId);
  H5Aclose(vAttributeId);

  return true;
}

bool IsFormat(const bpString& aInputFile, hid_t& aFileId, bool aSWMR)
{
  if (!bpfFileTools::FileExists(aInputFile)) {
    std::cout << "File does not exist: " << aInputFile << std::endl;
    return false;
  }

  std::cout << "File exists: " << aInputFile << std::endl;
  // test if file is a hdf5 format file
  if (H5Fis_hdf5(bpfFileTools::AddExtendedPathPrefix(aInputFile).c_str()) == 0) {
    BP_DEBUG_MSG("IsFormat() - Not a HDF5 file format!");
    return false;
  }

  // open the file
  const auto& vFileName = bpfFileTools::AddExtendedPathPrefix(aInputFile);
  if (aSWMR) {
    aFileId = H5Fopen(vFileName.c_str(), H5F_ACC_RDONLY | H5F_ACC_SWMR_READ, H5P_DEFAULT);
  }
  else {
    aFileId = H5Fopen(vFileName.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
  }

  if (aFileId < 0) {
    // could not open the file
    BP_DEBUG_MSG("IsFormat() - Could not open the file!");
    return false;
  }

  // open the root group
  hid_t vRootId = H5Gopen(aFileId, "/", H5P_DEFAULT);

  if (vRootId < 0) {
    // could not open the group
    CloseFile(aFileId);
    BP_DEBUG_MSG("IsFormat() - Could not open root group in file!");
    return false;
  }

  // look for some attributes
  if (H5Aget_num_attrs(vRootId) <= 0) {
    // operation failed
    H5Gclose(vRootId);
    CloseFile(aFileId);
    BP_DEBUG_MSG("IsFormat() - Could not find any attributes in root group!");
    return false;
  }

  // look for file format version
  bpfString vAttributeValue = "";

  if (!ReadAttributeString("ImarisVersion", vAttributeValue, vRootId)) {
    // operation failed
    H5Gclose(vRootId);
    CloseFile(aFileId);
    BP_DEBUG_MSG("IsFormat() - Could not read attributes in root group!");
    return false;
  }

  if (vAttributeValue != "5.5.0") {
    // wrong file format
    H5Gclose(vRootId);
    CloseFile(aFileId);
    BP_DEBUG_MSG("IsFormat() - Could not read attributes in root group!");
    return false;
  }

  if (H5Aexists(vRootId, "NumberOfDataSets")) {
    hid_t vAttr = H5Aopen_name(vRootId, "NumberOfDataSets");
    bpfUInt32 vNumberOfDataSets = 1;
    H5Aread(vAttr, H5T_NATIVE_UINT32, &vNumberOfDataSets);
    H5Aclose(vAttr);
  }

  if (!ReadAttributeString("ImarisDataSet", vAttributeValue, vRootId)) {
    // operation failed
    H5Gclose(vRootId);
    CloseFile(aFileId);
    BP_DEBUG_MSG("IsFormat() - Could not read attributes in root group!");
    return false;
  }

  if (vAttributeValue != "ImarisDataSet") {
    // it is an encoded file or there is some other problem
    H5Gclose(vRootId);
    CloseFile(aFileId);
    BP_DEBUG_MSG("IsFormat() - Invalid attribute content!");
    return false;
  }

  // close
  H5Gclose(vRootId);

  return true;
}

std::vector<tDataType> GetFileImagesInformation(const bpString& aInputFile, bool aSWMR)
{
  std::vector<tDataType> vResult;
  hid_t vFileId;
  if (!IsFormat(aInputFile, vFileId, aSWMR)) {
    return vResult;
  }

  // read number of datasets
  hid_t vRootId = H5Gopen(vFileId, "/", H5P_DEFAULT);
  bpfUInt32 vNumberOfDataSets = 1;
  if (H5Aexists(vRootId, "NumberOfDataSets")) {
    hid_t vAttrId = H5Aopen_name(vRootId, "NumberOfDataSets");
    H5Aread(vAttrId, H5T_NATIVE_UINT32, &vNumberOfDataSets);
    H5Aclose(vAttrId);
  }

  for (bpSize vD = 0; vD < vNumberOfDataSets; vD++) {
    bpfString vDirectoryName = mDataSetDirectoryName + (vD == 0 ? "" : bpfToString(vD));
    hid_t vDataSetId = H5Gopen(vFileId, vDirectoryName.c_str(), H5P_DEFAULT);

    if (vDataSetId < 0) {
      return vResult;
    }

    hid_t vLevelId = H5Gopen(vDataSetId, "ResolutionLevel 0", H5P_DEFAULT);
    hid_t vTimePointId = H5Gopen(vLevelId, "TimePoint 0", H5P_DEFAULT);
    H5Gclose(vLevelId);
    hid_t vChannelId = H5Gopen(vTimePointId, "Channel 0", H5P_DEFAULT);
    H5Gclose(vTimePointId);

    // find data type info
    hid_t vDataId = H5Dopen(vChannelId, "Data", H5P_DEFAULT);
    H5Gclose(vChannelId);

    if (vDataId < 0) {
      H5Gclose(vDataSetId);
      return vResult;
    }

    hid_t vDataTypeId = H5Dget_type(vDataId);
    H5Dclose(vDataId);
    bpfSize vDataTypeSize = H5Tget_size(vDataTypeId);
    tDataType vType;

    switch (vDataTypeSize) {
    case 1:
      vType = bpUInt8Type;
      break;
    case 2:
      vType = bpUInt16Type;
      break;
    case 4: {
      H5T_class_t vDataTypeClass = H5Tget_class(vDataTypeId);
      if (vDataTypeClass == H5T_FLOAT) {
        vType = bpFloatType;
      }
      else {
        vType = bpUInt32Type;
      }
      break;
    }
    default:
      H5Dclose(vDataTypeId);
      H5Gclose(vDataSetId);
      return vResult;
    }
    H5Tclose(vDataTypeId);
    vResult.push_back(vType);

    H5Gclose(vDataSetId);
  }

  return vResult;
}

template <typename TDataType>
class bpImageReader<TDataType>::cThreadSafeDecorator : public bpImageReaderInterface<TDataType>
{
public:
  explicit cThreadSafeDecorator(bpUniquePtr<bpImageReaderInterface<TDataType>> aImpl)
    : mImpl(std::move(aImpl))
  {
  }

  void ReadMetadata(
    std::vector<bpConverterTypes::tSize5D>& aImageSizePerResolution,
    std::vector<bpConverterTypes::tSize5D>& aFileBlockSizePerResolution,
    bpConverterTypes::cImageExtent& aImageExtent,
    bpConverterTypes::tTimeInfoVector& aTimeInfoPerTimePoint,
    bpConverterTypes::tColorInfoVector& aColorInfoPerChannel,
    bpConverterTypes::tCompressionAlgorithmType& aCompressionAlgorithmType)
  {
    tLock vLock(mMutex);
    return mImpl->ReadMetadata(aImageSizePerResolution, aFileBlockSizePerResolution, aImageExtent, aTimeInfoPerTimePoint, aColorInfoPerChannel, aCompressionAlgorithmType);
  }

  void ReadParameters(bpConverterTypes::tParameters& aParameters)
  {
    tLock vLock(mMutex);
    return mImpl->ReadParameters(aParameters);
  }

  void ReadData(const bpConverterTypes::tIndex5D& aBegin, const bpConverterTypes::tIndex5D& aEnd, bpSize aResolutionIndex, TDataType* aData)
  {
    tLock vLock(mMutex);
    return mImpl->ReadData(aBegin, aEnd, aResolutionIndex, aData);
  }

  bpImageReaderBaseInterface::cHistogram ReadHistogram(const bpVec3& aIndexTCR)
  {
    tLock vLock(mMutex);
    return mImpl->ReadHistogram(aIndexTCR);
  }

  bpImageReaderBaseInterface::cThumbnail ReadThumbnail()
  {
    tLock vLock(mMutex);
    return mImpl->ReadThumbnail();
  }

private:
  using tMutex = std::mutex;
  using tLock = std::lock_guard<tMutex>;

  mutable tMutex mMutex;

  bpUniquePtr<bpImageReaderInterface<TDataType>> mImpl;
};

template <typename TDataType>
bpImageReader<TDataType>::bpImageReader(const bpString& aInputFile, bpSize aImageIndex, const bpReaderTypes::cReadOptions& aOptions)
{
  auto vImpl = std::make_unique<bpImageReaderImpl<TDataType>>(aInputFile, aImageIndex, aOptions);
  mImpl = std::make_unique<cThreadSafeDecorator>(std::move(vImpl));
}

template <typename TDataType>
bpImageReader<TDataType>::~bpImageReader()
{}

template <typename TDataType>
void bpImageReader<TDataType>::ReadMetadata(
  std::vector<tSize5D>& aImageSizePerResolution,
  std::vector<tSize5D>& aFileBlockSizePerResolution,
  cImageExtent& aImageExtent,
  tTimeInfoVector& aTimeInfoPerTimePoint,
  tColorInfoVector& aColorInfoPerChannel,
  tCompressionAlgorithmType& aCompressionAlgorithmType)
{
  mImpl->ReadMetadata(aImageSizePerResolution, aFileBlockSizePerResolution, aImageExtent, aTimeInfoPerTimePoint, aColorInfoPerChannel, aCompressionAlgorithmType);
}

template <typename TDataType>
void bpImageReader<TDataType>::ReadParameters(tParameters& aParameters)
{
  mImpl->ReadParameters(aParameters);
}

template <typename TDataType>
void bpImageReader<TDataType>::ReadData(const tIndex5D& aBegin, const tIndex5D& aEnd, bpSize aResolutionIndex, TDataType* aData)
{
  mImpl->ReadData(aBegin, aEnd, aResolutionIndex, aData);
}


template <typename TDataType>
bpImageReaderBaseInterface::cHistogram bpImageReader<TDataType>::ReadHistogram(const bpVec3& aIndexTCR)
{
  return mImpl->ReadHistogram(aIndexTCR);
}


template <typename TDataType>
bpImageReaderBaseInterface::cThumbnail bpImageReader<TDataType>::ReadThumbnail()
{
  return mImpl->ReadThumbnail();
}

template class bpImageReader<bpUInt8>;
template class bpImageReader<bpUInt16>;
template class bpImageReader<bpUInt32>;
template class bpImageReader<bpFloat>;

