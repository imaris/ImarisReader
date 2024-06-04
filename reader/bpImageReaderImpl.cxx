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

#if defined(_WIN32)
  #define NOMINMAX
#endif

#include "ImarisReader/reader/bpImageReaderImpl.h"

#include "ImarisReader/utils/bpfFileTools.h"
#include "ImarisReader/utils/bpfUtils.h"
#include "ImarisReader/utils/bpfH5LZ4.h"

#include <iostream>

using namespace bpConverterTypes;

const bpfString mDataSetDirectoryName = "DataSet";
const bpfString mDataSetInfoDirectoryName = "DataSetInfo";
const bpfString mThumbnailDirectoryName = "Thumbnail";

template<typename TDataType>
bpImageReaderImpl<TDataType>::bpImageReaderImpl(const bpString& aInputFile, bpSize aImageIndex, const bpReaderTypes::cReadOptions& aOptions)
  : mFileName(aInputFile),
  mFileID(0),
  mSWMR(aOptions.mSWMR),
  mNumberOfChannels(0),
  mNumberOfTimePoints(0),
  mType(bpfNoType),
  mHDFType(0),
  mNumberOfDataSets(1),
  mActiveDataSetIndex(aImageIndex)
{
  H5Zregister_lz4();
  if (!IsFormat()) {
    std::cerr << "Imaris Reader: false file format!" << std::endl;
  }
  ReadProperties();
}

template<typename TDataType>
bpImageReaderImpl<TDataType>::~bpImageReaderImpl()
{}

  
template<typename TDataType>
void bpImageReaderImpl<TDataType>::ReadMetadata(
    std::vector<tSize5D>& aImageSizePerResolution,
    std::vector<tSize5D>& aFileBlockSizePerResolution,
    cImageExtent& aImageExtent,
    tTimeInfoVector& aTimeInfoPerTimePoint,
    tColorInfoVector& aColorInfoPerChannel,
    tCompressionAlgorithmType& aCompressionAlgorithmType)
{
  bpfString vDirectoryName = GetDirectoryName(mDataSetDirectoryName);
  hid_t vDatasetId = H5Gopen(mFileID, vDirectoryName.c_str(), H5P_DEFAULT);

  // get the number of resolution levels, timepoints and channels
  hsize_t vNumResolutionLevels;
  H5Gget_num_objs(vDatasetId, &vNumResolutionLevels);
  hid_t vResolutionZeroId = H5Gopen(vDatasetId, "ResolutionLevel 0", H5P_DEFAULT);
  hsize_t vNumTimepoints;
  H5Gget_num_objs(vResolutionZeroId, &vNumTimepoints);
  hid_t vTimePointZeroResZeroId = H5Gopen(vResolutionZeroId, "TimePoint 0", H5P_DEFAULT);
  hsize_t vNumChannels;
  H5Gget_num_objs(vTimePointZeroResZeroId, &vNumChannels);

  // read image and block sizes
  for (bpSize vResolutionLevel = 0; vResolutionLevel < vNumResolutionLevels; vResolutionLevel++) {
    // open resolution level group
    hid_t vResolutionLevelId = H5Gopen(vDatasetId, (bpfString("ResolutionLevel ") + bpfToString(vResolutionLevel)).c_str(), H5P_DEFAULT);
    // open Timepoint 0 and Channel 0
    hid_t vTimePointId = H5Gopen(vResolutionLevelId, "TimePoint 0", H5P_DEFAULT);
    hid_t vChannelId = H5Gopen(vTimePointId, "Channel 0", H5P_DEFAULT);

    // read image size from Channel attributes
    std::vector<bpfString> vImageSizeStrings{"", "", ""};
    std::vector<bpSize> vImageSizes(3);
    ReadAttributeString("ImageSizeX", vImageSizeStrings[0], vChannelId);
    ReadAttributeString("ImageSizeY", vImageSizeStrings[1], vChannelId);
    ReadAttributeString("ImageSizeZ", vImageSizeStrings[2], vChannelId);
    for (bpSize vI = 0; vI < vImageSizes.size(); vI++) {
      bpfFromString(vImageSizeStrings[vI], vImageSizes[vI]);
    }
    aImageSizePerResolution.push_back(tSize5D{ { X, vImageSizes[0] }, { Y, vImageSizes[1] }, { Z, vImageSizes[2] },
                                                                 { T, (bpSize)vNumTimepoints }, { C, (bpSize)vNumChannels } });

    // read image block size from dataset Chunk Shape
    hid_t vDataId = H5Dopen2(vChannelId, "Data", H5P_DEFAULT);
    hid_t vDataspace = H5Dget_space(vDataId);
    bpSize vRank = H5Sget_simple_extent_ndims(vDataspace);
    std::vector<hsize_t> vBlockSizes(vRank);
    H5Sget_simple_extent_dims(vDataspace, vBlockSizes.data(), NULL);
    hid_t vPlist = H5Dget_create_plist(vDataId);
    H5Pget_chunk(vPlist, vRank, vBlockSizes.data());
    aFileBlockSizePerResolution.push_back(tSize5D{ { X, (bpSize)vBlockSizes[2] }, { Y, (bpSize)vBlockSizes[1] }, { Z, (bpSize)vBlockSizes[0] },
                                                                     { T, 1 }, { C, 1 } });

    H5Pclose(vPlist);
    H5Sclose(vDataspace);
    H5Dclose(vDataId);

    H5Gclose(vChannelId);
    H5Gclose(vTimePointId);
    H5Gclose(vResolutionLevelId);
  }

  // read image extents
  bpfString vInfoDirectoryName = GetDirectoryName(mDataSetInfoDirectoryName);
  hid_t vDatasetInfoId = H5Gopen(mFileID, vInfoDirectoryName.c_str(), H5P_DEFAULT);
  hid_t vImageId = H5Gopen(vDatasetInfoId, "Image", H5P_DEFAULT);
  std::vector<bpfString> vExtentsMinStrings{"", "", ""};
  std::vector<bpfString> vExtentsMaxStrings{"", "", ""};
  std::vector<bpSize> vExtentMins(3);
  std::vector<bpSize> vExtentMaxs(3);
  ReadAttributeString("ExtMin0", vExtentsMinStrings[0], vImageId);
  ReadAttributeString("ExtMin1", vExtentsMinStrings[1], vImageId);
  ReadAttributeString("ExtMin2", vExtentsMinStrings[2], vImageId);
  ReadAttributeString("ExtMax0", vExtentsMaxStrings[0], vImageId);
  ReadAttributeString("ExtMax1", vExtentsMaxStrings[1], vImageId);
  ReadAttributeString("ExtMax2", vExtentsMaxStrings[2], vImageId);
  bpfFromString(vExtentsMinStrings[0], aImageExtent.mExtentMinX);
  bpfFromString(vExtentsMinStrings[1], aImageExtent.mExtentMinY);
  bpfFromString(vExtentsMinStrings[2], aImageExtent.mExtentMinZ);
  bpfFromString(vExtentsMaxStrings[0], aImageExtent.mExtentMaxX);
  bpfFromString(vExtentsMaxStrings[1], aImageExtent.mExtentMaxY);
  bpfFromString(vExtentsMaxStrings[2], aImageExtent.mExtentMaxZ);

  // read time info
  aTimeInfoPerTimePoint.resize(vNumTimepoints);
  hid_t vTimeInfoId = H5Gopen(vDatasetInfoId, "TimeInfo", H5P_DEFAULT);
  for (bpSize vT = 0; vT < vNumTimepoints; vT++) {
    bpfString vTimeString = "";
    ReadAttributeString((bpfString("TimePoint") + bpfToString(vT+1)).c_str(), vTimeString, vTimeInfoId);
    bpfTimeInfo vTime(vTimeString);
    aTimeInfoPerTimePoint[vT].mJulianDay = vTime.GetJulianDay();
    aTimeInfoPerTimePoint[vT].mNanosecondsOfDay = vTime.GetNanoseconds();
  }

  // read color info
  aColorInfoPerChannel.resize(vNumChannels);
  for (bpSize vC = 0; vC < vNumChannels; vC++) {
    hid_t vChannelId = H5Gopen(vDatasetInfoId, (bpfString("Channel ") + bpfToString(vC)).c_str(), H5P_DEFAULT);

    bpfString vColorMode = "";
    ReadAttributeString("ColorMode", vColorMode, vChannelId);
    if (vColorMode == "BaseColor") {
      bpfString vColorString = "";
      ReadAttributeString("Color", vColorString, vChannelId);
      std::vector<bpfString> vColors = bpfSplit(vColorString, " ", false, false);
      bpfFromString(vColors[0], aColorInfoPerChannel[vC].mBaseColor.mRed);
      bpfFromString(vColors[1], aColorInfoPerChannel[vC].mBaseColor.mGreen);
      bpfFromString(vColors[2], aColorInfoPerChannel[vC].mBaseColor.mBlue);
    }
    else {
      aColorInfoPerChannel[vC].mIsBaseColorMode = false;
      hid_t vColorTableId = H5Dopen2(vChannelId, "ColorTable", H5P_DEFAULT);
      hid_t vDataspace = H5Dget_space(vColorTableId);
      hsize_t vDim;
      H5Sget_simple_extent_dims(vDataspace, &vDim, NULL);
      bpfString vColorTable;
      vColorTable.resize(vDim);
      H5Dread(vColorTableId, H5T_C_S1, H5S_ALL, H5S_ALL, H5P_DEFAULT, &vColorTable[0]);
      std::vector<bpfString> vColors = bpfSplit(vColorTable, " ", false, true);
      for (bpSize vI = 0; vI < vColors.size()/3; vI++) {
        cColor vColor;
        bpfFromString(vColors[3*vI], vColor.mRed);
        bpfFromString(vColors[3*vI+1], vColor.mGreen);
        bpfFromString(vColors[3*vI+2], vColor.mBlue);
        aColorInfoPerChannel[vC].mColorTable.push_back(vColor);
      }
      H5Sclose(vDataspace);
      H5Dclose(vColorTableId);
    }

    // read opacity
    bpfString vOpacity = "";
    ReadAttributeString("ColorOpacity", vOpacity, vChannelId);
    bpfFromString(vOpacity, aColorInfoPerChannel[vC].mOpacity);

    // read range
    bpfString vRange = "";
    ReadAttributeString("ColorRange", vRange, vChannelId);
    std::vector<bpfString> vMinAndMaxRange = bpfSplit(vRange, " ", false, false);
    bpfFromString(vMinAndMaxRange[0], aColorInfoPerChannel[vC].mRangeMin);
    bpfFromString(vMinAndMaxRange[1], aColorInfoPerChannel[vC].mRangeMax);

    // read gamma correction
    bpfString vGamma = "";
    ReadAttributeString("GammaCorrection", vGamma, vChannelId);
    bpfFromString(vGamma, aColorInfoPerChannel[vC].mGammaCorrection);

    H5Gclose(vChannelId);
  }

  // read compression algorithm
  std::vector<tCompressionAlgorithmType> vAlgorithmTypes = {eCompressionAlgorithmNone,
                                                            eCompressionAlgorithmGzipLevel1, eCompressionAlgorithmGzipLevel2, eCompressionAlgorithmGzipLevel3,
                                                            eCompressionAlgorithmGzipLevel4, eCompressionAlgorithmGzipLevel5, eCompressionAlgorithmGzipLevel6,
                                                            eCompressionAlgorithmGzipLevel7, eCompressionAlgorithmGzipLevel8, eCompressionAlgorithmGzipLevel9,
                                                            eCompressionAlgorithmShuffleGzipLevel1, eCompressionAlgorithmShuffleGzipLevel2, eCompressionAlgorithmShuffleGzipLevel3,
                                                            eCompressionAlgorithmShuffleGzipLevel4, eCompressionAlgorithmShuffleGzipLevel5, eCompressionAlgorithmShuffleGzipLevel6,
                                                            eCompressionAlgorithmShuffleGzipLevel7, eCompressionAlgorithmShuffleGzipLevel8, eCompressionAlgorithmShuffleGzipLevel9,
                                                            eCompressionAlgorithmLZ4, eCompressionAlgorithmShuffleLZ4};
  hid_t vChannelId = H5Gopen(vTimePointZeroResZeroId, "Channel 0", H5P_DEFAULT);
  hid_t vDataId = H5Dopen2(vChannelId, "Data", H5P_DEFAULT);
  hid_t vPlist = H5Dget_create_plist(vDataId);
  std::vector<bpInt32> vFilters;
  bpInt32 vNumFilters = H5Pget_nfilters(vPlist);
  if (0 == vNumFilters) {
    aCompressionAlgorithmType = vAlgorithmTypes[0];
  }
  else {
    for (bpInt32 vF = 0; vF < vNumFilters; vF++) {
      vFilters.push_back(H5Pget_filter2(vPlist, vF, nullptr, nullptr, nullptr, 0, nullptr, nullptr));
    }
    bool vIsShuffle = false;
    if (vFilters.front() == H5Z_FILTER_SHUFFLE) {
      vIsShuffle = true;
    }
    bpSize vNumElements = 1;
    bpUInt32 vCompressionLevel[1];
    bpSize vAlgoIndex = 0;
    if (vFilters.back() == H5Z_FILTER_DEFLATE) {
      H5Pget_filter_by_id(vPlist, H5Z_FILTER_DEFLATE, nullptr, &vNumElements, vCompressionLevel, 0, nullptr, nullptr);
      vAlgoIndex += (bpSize)vCompressionLevel[0];
      if (vIsShuffle) {
        vAlgoIndex += 9;
      }
      aCompressionAlgorithmType = vAlgorithmTypes[vAlgoIndex];
    }
    else if (vFilters.back() == H5Z_FILTER_LZ4) {
      vAlgoIndex = (vIsShuffle) ? 20 : 19;
      aCompressionAlgorithmType = vAlgorithmTypes[vAlgoIndex];
    }
  }
  H5Pclose(vPlist);
  H5Dclose(vDataId);
  H5Gclose(vChannelId);


  H5Gclose(vTimeInfoId);
  H5Gclose(vImageId);
  H5Gclose(vDatasetInfoId);
  H5Gclose(vTimePointZeroResZeroId);
  H5Gclose(vResolutionZeroId);
  H5Gclose(vDatasetId);
}


template<typename TDataType>
void bpImageReaderImpl<TDataType>::ReadParameters(tParameters& aParameters)
{
  bpfString vDirectoryName = GetDirectoryName(mDataSetInfoDirectoryName);
  hid_t vDataSetInfoId = H5Gopen(mFileID, vDirectoryName.c_str(), H5P_DEFAULT);

  hsize_t vNumberOfSections = 0;
  H5Gget_num_objs(vDataSetInfoId, &vNumberOfSections);
  ssize_t vSectionNameSize = 0;
  bpfChar* vSectionName = nullptr;
  hid_t vSectionId;

  // loop over sections
  for (hsize_t vSectionIndex = 0; vSectionIndex < vNumberOfSections; ++vSectionIndex) {
    if (H5Gget_objtype_by_idx(vDataSetInfoId, vSectionIndex) == H5G_GROUP) {
      // get new section name
      vSectionNameSize = H5Gget_objname_by_idx(vDataSetInfoId, vSectionIndex, nullptr, 0) + 1;
      vSectionName = new bpfChar[vSectionNameSize];
      bpfParameterSection* vParameterSection = new bpfParameterSection;
      H5Gget_objname_by_idx(vDataSetInfoId, vSectionIndex, vSectionName, static_cast<bpfSize>(vSectionNameSize));

      // open the group and read attributes
      vSectionId = H5Gopen(vDataSetInfoId, vSectionName, H5P_DEFAULT);
      ReadSection(vSectionId, vParameterSection, true);
      H5Gclose(vSectionId);

      auto vSectionIter = vParameterSection->Begin();
      while (vSectionIter != vParameterSection->End()) {
        aParameters[vSectionName].emplace(vSectionIter->first, vSectionIter->second);
        vSectionIter++;
      }

      delete vParameterSection;
      delete[] vSectionName;
    }
  }

  H5Gclose(vDataSetInfoId);
}


template<typename TDataType>
void bpImageReaderImpl<TDataType>::ReadData(const tIndex5D& aBegin, const tIndex5D& aEnd, bpSize aResolutionIndex, TDataType* aData)
{
  bpSize vEndT = std::min(aEnd[T], GetSizeT(aResolutionIndex));
  bpSize vEndC = std::min(aEnd[C], GetSizeC(aResolutionIndex));
  hsize_t vStart[] = { aBegin[Z], aBegin[Y], aBegin[X]};
  hsize_t vReadSizeDim[] = { aEnd[Z] - aBegin[Z], aEnd[Y] - aBegin[Y], aEnd[X] - aBegin[X] };

  bpfString vDirectoryName = GetDirectoryName(mDataSetDirectoryName);
//  std::cerr << "Trying to open " << mFileID << " directory name: " << vDirectoryName << std::endl;
  hid_t vDataSetID = H5Gopen(mFileID, vDirectoryName.c_str(), H5P_DEFAULT);

  bpfString vResolutionLevel = "ResolutionLevel " + bpfToString(aResolutionIndex);
  hid_t vLevelId = H5Gopen(vDataSetID, vResolutionLevel.c_str(), H5P_DEFAULT);
  bpSize vSizeXYZ = vReadSizeDim[0] * vReadSizeDim[1] * vReadSizeDim[2];
  bpSize vSizeXYZC = vSizeXYZ * (vEndC - aBegin[C]);

  for (bpSize vIndexT = aBegin[T]; vIndexT < vEndT; ++vIndexT) {
    bpSize vOffsetT = vSizeXYZC * (vIndexT - aBegin[T]);

    bpfString vTimePointStr = "TimePoint " + bpfToString(vIndexT);
    hid_t vTimePointId = H5Gopen(vLevelId, vTimePointStr.c_str(), H5P_DEFAULT);

    for (bpSize vIndexC = aBegin[C]; vIndexC < vEndC; ++vIndexC) {
      bpSize vOffsetC = vOffsetT + vSizeXYZ * (vIndexC - aBegin[C]);

      bpfString vChannelStr = "Channel " + bpfToString(vIndexC);
      hid_t vChannelId = H5Gopen(vTimePointId, vChannelStr.c_str(), H5P_DEFAULT);

//      std::cerr << "Try open " << vTimePointStr << " " << vChannelStr << std::endl;

      hid_t vDataId = H5Dopen(vChannelId, "Data", H5P_DEFAULT);
      if (mSWMR) {
        H5Drefresh(vDataId);
      }
      H5Gclose(vChannelId);

      hid_t vDataSetSpaceID = H5Dget_space(vDataId);
      hsize_t vMemDim[3] = { vReadSizeDim[0], vReadSizeDim[1], vReadSizeDim[2] };

      hsize_t vFileDim[] = { GetSizeZ(aResolutionIndex), GetSizeY(aResolutionIndex), GetSizeX(aResolutionIndex) };
      H5Sget_simple_extent_dims(vDataSetSpaceID, vFileDim, nullptr);

      for (bpfSize vIndex = 0; vIndex < 3; vIndex++) {
        if (vStart[vIndex] + vMemDim[vIndex] > vFileDim[vIndex]) {
          vMemDim[vIndex] = vFileDim[vIndex] - vStart[vIndex];
        }
      }

      H5Sselect_hyperslab(vDataSetSpaceID, H5S_SELECT_SET, vStart, nullptr, vMemDim, nullptr);
      hid_t vMemSpaceID = H5Screate_simple(3, vMemDim, nullptr);

      herr_t vError = -1;
      if (H5I_INVALID_HID < vMemSpaceID) {
        vError = H5Dread(vDataId, mHDFType, vMemSpaceID, vDataSetSpaceID, H5P_DEFAULT, aData + vOffsetC);
        H5Sclose(vMemSpaceID);
//        std::cout << "Success! " << vError << std::endl;
      }
      bpSize vCountMem = vMemDim[0] * vMemDim[1] * vMemDim[2];
      if (vError < 0) {
        hid_t vTypeID = H5Dget_type(vDataId);
        if (H5I_INVALID_HID < vTypeID) {
          bpSize vTypeSize = H5Tget_size(vTypeID);
          std::memset(aData + vOffsetC, 0, vCountMem * vTypeSize);
          H5Tclose(vTypeID);
          std::cout << "Fail!" << std::endl;
        }
      }

      H5Sclose(vDataSetSpaceID);
      H5Dclose(vDataId);

      // revert notation for dimension sequence from hdf to bitplane
      hsize_t vMemDim_[3] = { vMemDim[2], vMemDim[1], vMemDim[0] };
      hsize_t vReadSizeDim_[3] = { vReadSizeDim[2], vReadSizeDim[1], vReadSizeDim[0] };
      FixPadding((bpfChar*)(aData + vOffsetC), vMemDim_, vReadSizeDim_, bpfGetSizeOfType(mType));
    }

    H5Gclose(vTimePointId);
  }

  H5Gclose(vLevelId);
  H5Gclose(vDataSetID);
}

template<typename TDataType>
bpImageReaderBaseInterface::cHistogram bpImageReaderImpl<TDataType>::ReadHistogram(const bpVec3& aIndexTCR)
{
  bpImageReaderBaseInterface::cHistogram vHistogram;

  bpSize vTimePoint = aIndexTCR[0];
  bpSize vChannel = aIndexTCR[1];
  bpSize vResolutionLevel = aIndexTCR[2];

  // open dataset group
  bpfString vDirectoryName = GetDirectoryName(mDataSetDirectoryName);
  hid_t vDataSetId = H5Gopen(mFileID, vDirectoryName.c_str(), H5P_DEFAULT);
  // open resolution level group
  hid_t vResolutionLevelId = H5Gopen(vDataSetId, (bpfString("ResolutionLevel ") + bpfToString(vResolutionLevel)).c_str(), H5P_DEFAULT);
  // open timepoint group
  hid_t vTimePointId = H5Gopen(vResolutionLevelId, (bpfString("TimePoint ") + bpfToString(vTimePoint)).c_str(), H5P_DEFAULT);
  // open channel group
  hid_t vChannelId = H5Gopen(vTimePointId, (bpfString("Channel ") + bpfToString(vChannel)).c_str(), H5P_DEFAULT);
  // read HistogramMin and Max and open histogram dataset, using Histogram1024 if it exists
  bpfString vHistMin = "";
  bpfString vHistMax = "";
  bpfString vSuffix = "";
  if (H5Lexists(vChannelId, "Histogram1024", H5P_DEFAULT) > 0) {
    vSuffix = "1024";
  }
  ReadAttributeString((bpfString("HistogramMin") + vSuffix).c_str(), vHistMin, vChannelId);
  ReadAttributeString((bpfString("HistogramMax") + vSuffix).c_str(), vHistMax, vChannelId);
  bpfFromString(vHistMin, vHistogram.mMin);
  bpfFromString(vHistMax, vHistogram.mMax);
  hid_t vHistogramId = H5Dopen(vChannelId, (bpfString("Histogram") + vSuffix).c_str(), H5P_DEFAULT);
  // get the dataspace and number of elements in dataset
  hid_t vHistogramDataspace = H5Dget_space(vHistogramId);
  hsize_t vBinsNumber;
  H5Sget_simple_extent_dims(vHistogramDataspace, &vBinsNumber, NULL);
  // read the histogram
  std::vector<bpUInt64> vData(vBinsNumber);
  H5Dread(vHistogramId, H5T_NATIVE_UINT64, H5S_ALL, H5S_ALL, H5P_DEFAULT, vData.data());

  H5Gclose(vDataSetId);
  H5Gclose(vResolutionLevelId);
  H5Gclose(vTimePointId);
  H5Gclose(vChannelId);
  H5Sclose(vHistogramDataspace);
  H5Dclose(vHistogramId);

  vHistogram.mBins = vData;
  return vHistogram;
}

template<typename TDataType>
bpImageReaderBaseInterface::cThumbnail bpImageReaderImpl<TDataType>::ReadThumbnail()
{
  bpImageReaderBaseInterface::cThumbnail vThumbnail;

  // open thumbnail group
  bpfString vDirectoryName = GetDirectoryName(mThumbnailDirectoryName);
  hid_t vThumbnailId = H5Gopen(mFileID, vDirectoryName.c_str(), H5P_DEFAULT);
  // get the dataset
  hid_t vThumbnailDataId = H5Dopen(vThumbnailId, "Data", H5P_DEFAULT);
  // get the dataspace
  hid_t vThumbnailDataspaceId = H5Dget_space(vThumbnailDataId);
  // get the size of data
  hsize_t vThumbnailDims[2];
  H5Sget_simple_extent_dims(vThumbnailDataspaceId, vThumbnailDims, NULL);
  // read the thumbnail data
  std::vector<bpUInt8> vData((bpSize)(vThumbnailDims[0] * vThumbnailDims[1]));
  H5Dread(vThumbnailDataId, H5T_NATIVE_UCHAR, H5S_ALL, H5S_ALL, H5P_DEFAULT, vData.data());

  // hdf5 stores dimension sizes from slower to faster changing, [sizeY, sizeX] here
  vThumbnail.mSizeY = vThumbnailDims[0];
  vThumbnail.mSizeX = vThumbnailDims[1]/4;
  vThumbnail.mInterleavedRGBA = vData;

  H5Sclose(vThumbnailDataspaceId);
  H5Dclose(vThumbnailDataId);
  H5Gclose(vThumbnailId);

  return vThumbnail;
}

template<typename TDataType>
const bpfString& bpImageReaderImpl<TDataType>::GetFileName() const {
  return mFileName;
}

template<typename TDataType>
bpfSize bpImageReaderImpl<TDataType>::GetActiveDatasetIndex() {
  return mActiveDataSetIndex;
}

template<typename TDataType>
bpfString bpImageReaderImpl<TDataType>::GetDirectoryName(const bpfString& aDirectoryName)
{
  bpfSize vImageIndex = GetActiveDatasetIndex();
  return aDirectoryName + (vImageIndex == 0 ? "" : bpfToString(vImageIndex));
}

template<typename TDataType>
bool bpImageReaderImpl<TDataType>::ReadSection(const hid_t& aSectionId, bpfParameterSection* aParameterSection, bool aReadLongParameters)
{
  bpfInt32 vNumberOfAttributes = H5Aget_num_attrs(aSectionId);

  if (vNumberOfAttributes < 0) {
    // operation failed
    return false;
  }

  H5Aiterate1(aSectionId, nullptr, ReadSectionParameter, aParameterSection);
  if (aReadLongParameters) {
    H5Literate(aSectionId, H5_INDEX_NAME, H5_ITER_INC, nullptr, ReadSectionLongParameter, aParameterSection);
  }
  return true;
}

template<typename TDataType>
herr_t bpImageReaderImpl<TDataType>::ReadSectionParameter(hid_t aSectionId, const char* aAttributeName, void* aParameterSection)
{
  bpfString vAttributeValue = "";

  if (!ReadAttributeString(aAttributeName, vAttributeValue, aSectionId)) {
    // failed to read attribute
    return 1;
  }

  // put attribute name and value in section
  static_cast<bpfParameterSection*>(aParameterSection)->SetParameter(aAttributeName, vAttributeValue);
  return 0;
}


template<typename TDataType>
herr_t bpImageReaderImpl<TDataType>::ReadSectionLongParameter(hid_t aSectionId, const char* aAttributeName, const H5L_info_t* aLinkInfo, void* aParameterSection)
{
  H5G_stat_t vStat;
  H5Gget_objinfo(aSectionId, aAttributeName, 0, &vStat);
  if (vStat.type != H5G_DATASET) {
    return 0;
  }

  hid_t vDataId = H5Dopen(aSectionId, aAttributeName, H5P_DEFAULT);
  if (vDataId <= 0) {
    return 0;
  }
  hid_t vSpaceId = H5Dget_space(vDataId);
  if (vSpaceId <= 0) {
    H5Dclose(vDataId);
    return 0;
  }
  bpfInt32 vDims = H5Sget_simple_extent_ndims(vSpaceId);
  if (vDims != 1) {
    H5Sclose(vSpaceId);
    H5Dclose(vDataId);
    return 0;
  }

  hsize_t vSize = 0;
  H5Sget_simple_extent_dims(vSpaceId, &vSize, nullptr);
  H5Sclose(vSpaceId);
  if (vSize == 0) {
    H5Dclose(vDataId);
    return 0;
  }

  bpfString vAttributeValue(vSize, '\0');
  H5Dread(vDataId, H5T_C_S1, H5S_ALL, H5S_ALL, H5P_DEFAULT, &vAttributeValue[0]);
  H5Dclose(vDataId);

  static_cast<bpfParameterSection*>(aParameterSection)->SetParameter(DecodeName(aAttributeName), vAttributeValue);
  return 0;
}


template<typename TDataType>
bool bpImageReaderImpl<TDataType>::ReadAttributeString(const bpfString aAttributeName, bpfString& aAttributeValue, const hid_t& aAttributeLocation)
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

  bpfChar* vBuffer = new bpfChar[(bpfSize)vAttributeSize + 1];
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


template<typename TDataType>
bpfString bpImageReaderImpl<TDataType>::DecodeName(bpfString aName)
{
  aName = bpfReplace(aName, "%s", "/");
  aName = bpfReplace(aName, "%p;", "%"); // must be last
  return aName;
}


template<typename TDataType>
bool bpImageReaderImpl<TDataType>::ReadProperties()
{
  bpfString vDirectoryName = GetDirectoryName(mDataSetDirectoryName);
  hid_t vDataSetId = H5Gopen(mFileID, vDirectoryName.c_str(), H5P_DEFAULT);

  if (vDataSetId < 0) {
    return false;
  }

  // get the number of resolution levels
  hsize_t vNum;
  H5Gget_num_objs(vDataSetId, &vNum);
  mNumberOfResolutions = static_cast<bpfSize>(vNum);

  for (bpfUInt32 vResolutionIndex = 0; vResolutionIndex < mNumberOfResolutions; ++vResolutionIndex) {
    bpfString vResolutionLevel = "ResolutionLevel " + bpfToString(vResolutionIndex);
    hid_t vLevelId = H5Gopen(vDataSetId, vResolutionLevel.c_str(), H5P_DEFAULT);

    if (vLevelId < 0) {
      BP_DEBUG_MSG("ReadProperties() - Could not open group " + vResolutionLevel);
      H5Gclose(vDataSetId);
      return false;
    }

    // get the number of timepoints
    if (vResolutionIndex == 0) {
      H5Gget_num_objs(vLevelId, &vNum);
      mNumberOfTimePoints = (bpfUInt32)vNum;
    }

    hid_t vTimePointId = H5Gopen(vLevelId, "TimePoint 0", H5P_DEFAULT);
    H5Gclose(vLevelId);

    if (vTimePointId < 0) {
      BP_DEBUG_MSG("ReadProperties() - Could not open group " + vResolutionLevel + "/TimePoint 0");
      H5Gclose(vDataSetId);
      return false;
    }

    // get the number of channels
    if (vResolutionIndex == 0) {
      H5Gget_num_objs(vTimePointId, &vNum);
      mNumberOfChannels = (bpfUInt32)vNum;
    }

    hid_t vChannelId = H5Gopen(vTimePointId, "Channel 0", H5P_DEFAULT);
    H5Gclose(vTimePointId);

    if (vChannelId < 0) {
      BP_DEBUG_MSG("ReadProperties() - Could not open group " + vResolutionLevel + "/TimePoint 0 /Channel 0");
      H5Gclose(vDataSetId);
      return false;
    }

    // find info about data type
    hid_t vDataId = H5Dopen(vChannelId, "Data", H5P_DEFAULT);

    if (vDataId < 0) {
      H5Gclose(vDataSetId);
      return false;
    }

    hid_t vDataTypeId = H5Dget_type(vDataId);
    H5Dclose(vDataId);
    bpfSize vDataTypeSize = H5Tget_size(vDataTypeId);

    switch (vDataTypeSize) {
    case 1:
      mType = bpfUInt8Type;
      mHDFType = H5T_NATIVE_UCHAR;
      break;
    case 2:
      mType = bpfUInt16Type;
      mHDFType = H5T_NATIVE_USHORT;
      break;
    case 4: {
      H5T_class_t vDataTypeClass = H5Tget_class(vDataTypeId);
      if (vDataTypeClass == H5T_FLOAT) {
        mType = bpfFloatType;
        mHDFType = H5T_NATIVE_FLOAT;
      }
      else {
        mType = bpfUInt32Type;
        mHDFType = H5T_NATIVE_UINT32;
      }
      }
      break;
    default:
      H5Dclose(vDataTypeId);
      H5Gclose(vDataSetId);
      return false;
    }
    H5Tclose(vDataTypeId);

    // set size information
    std::vector<bpfString> vImageSizeStrings{"", "", ""};
    ReadAttributeString("ImageSizeX", vImageSizeStrings[0], vChannelId);
    ReadAttributeString("ImageSizeY", vImageSizeStrings[1], vChannelId);
    ReadAttributeString("ImageSizeZ", vImageSizeStrings[2], vChannelId);
    if (vResolutionIndex >= mSizeX.size()) {
      mSizeX.resize(vResolutionIndex + 1);
      mSizeY.resize(vResolutionIndex + 1);
      mSizeZ.resize(vResolutionIndex + 1);
    }
    bpfFromString(vImageSizeStrings[0], mSizeX[vResolutionIndex]);
    bpfFromString(vImageSizeStrings[1], mSizeY[vResolutionIndex]);
    bpfFromString(vImageSizeStrings[2], mSizeZ[vResolutionIndex]);

    H5Gclose(vChannelId);
  }

  H5Gclose(vDataSetId);
  return true;
}


template<typename TDataType>
bool bpImageReaderImpl<TDataType>::IsFormat()
{
  // std::cout << "Fileid1: " << mFileID << std::endl;
  if (mFileID != 0) {
    // file is already open...
    return true;
  }

  if (!bpfFileTools::FileExists(GetFileName())) {
    return false;
  }

  // test if file is a hdf5 format file
  if (H5Fis_hdf5(bpfFileTools::AddExtendedPathPrefix(GetFileName()).c_str()) == 0) {
    BP_DEBUG_MSG("IsFormat() - Not a HDF5 file format!");
    return false;
  }

  // open the file
  if (mSWMR) {
    mFileID = H5Fopen(bpfFileTools::AddExtendedPathPrefix(GetFileName()).c_str(), H5F_ACC_RDONLY | H5F_ACC_SWMR_READ, H5P_DEFAULT);
  }
  else {
    mFileID = H5Fopen(bpfFileTools::AddExtendedPathPrefix(GetFileName()).c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
  }

  if (mFileID < 0) {
    // could not open the file
    BP_DEBUG_MSG("IsFormat() - Could not open the file!");
    return false;
  }

  // open the root group
  hid_t vRootId = H5Gopen(mFileID, "/", H5P_DEFAULT);

  if (vRootId < 0) {
    // could not open the group
    CloseFile();
    BP_DEBUG_MSG("IsFormat() - Could not open root group in file!");
    return false;
  }

  // look for some attributes
  if (H5Aget_num_attrs(vRootId) <= 0) {
    // operation failed
    H5Gclose(vRootId);
    CloseFile();
    BP_DEBUG_MSG("IsFormat() - Could not find any attributes in root group!");
    return false;
  }

  // look for file format version
  bpfString vAttributeValue = "";

  if (!ReadAttributeString("ImarisVersion", vAttributeValue, vRootId)) {
    // operation failed
    H5Gclose(vRootId);
    CloseFile();
    BP_DEBUG_MSG("IsFormat() - Could not read attributes in root group!");
    return false;
  }

  if (vAttributeValue != "5.5.0") {
    // wrong file format
    H5Gclose(vRootId);
    CloseFile();
    BP_DEBUG_MSG("IsFormat() - Could not read attributes in root group!");
    return false;
  }

  if (H5Aexists(vRootId, "NumberOfDataSets")) {
    hid_t vAttr = H5Aopen_name(vRootId, "NumberOfDataSets");
    bpfUInt32 vNumberOfDataSets = 1;
    H5Aread(vAttr, H5T_NATIVE_UINT32, &vNumberOfDataSets);
    mNumberOfDataSets = vNumberOfDataSets;
    H5Aclose(vAttr);
  }

  if (!ReadAttributeString("ImarisDataSet", vAttributeValue, vRootId)) {
    // operation failed
    H5Gclose(vRootId);
    CloseFile();
    BP_DEBUG_MSG("IsFormat() - Could not read attributes in root group!");
    return false;
  }

  if (vAttributeValue != "ImarisDataSet") {
    // it is an encoded file or there is some other problem
    H5Gclose(vRootId);
    CloseFile();
    BP_DEBUG_MSG("IsFormat() - Invalid attribute content!");
    return false;
  }

  H5Gclose(vRootId);

  return true;
}


template<typename TDataType>
void bpImageReaderImpl<TDataType>::CloseFile()
{
  if (mFileID > 0) {
    H5Fclose(mFileID);
  }

  mFileID = 0;
}


template<typename TDataType>
bpfSize bpImageReaderImpl<TDataType>::GetSizeX(bpfSize aResolutionLevel) const
{
  return mSizeX[aResolutionLevel];
}


template<typename TDataType>
bpfSize bpImageReaderImpl<TDataType>::GetSizeY(bpfSize aResolutionLevel) const
{
  return mSizeY[aResolutionLevel];
}


template<typename TDataType>
bpfSize bpImageReaderImpl<TDataType>::GetSizeZ(bpfSize aResolutionLevel) const
{
  return mSizeZ[aResolutionLevel];
}

template<typename TDataType>
bpfSize bpImageReaderImpl<TDataType>::GetSizeT(bpfSize /*aResolutionLevel*/) const
{
  return mNumberOfTimePoints;
}


template<typename TDataType>
bpfSize bpImageReaderImpl<TDataType>::GetSizeC(bpfSize /*aResolutionLevel*/) const
{
  return mNumberOfChannels;
}


template class bpImageReaderImpl<bpUInt8>;
template class bpImageReaderImpl<bpUInt16>;
template class bpImageReaderImpl<bpUInt32>;
template class bpImageReaderImpl<bpFloat>;
