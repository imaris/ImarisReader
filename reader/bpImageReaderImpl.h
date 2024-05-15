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


#ifndef __BP_FILE_READER_IMPL__
#define __BP_FILE_READER_IMPL__


#include "ImarisReader/interface/bpImageReaderInterface.h"
#include "ImarisReader/types/bpfParameterSection.h"

#include "hdf5.h"


template<typename TDataType>
class bpImageReaderImpl : public bpImageReaderInterface<TDataType>
{
public:
  bpImageReaderImpl(const bpString& aInputFile, bpSize aImageIndex, const bpReaderTypes::cReadOptions& aOptions);

  ~bpImageReaderImpl();

  
  void ReadMetadata(
    std::vector<bpConverterTypes::tSize5D>& aImageSizePerResolution,
    std::vector<bpConverterTypes::tSize5D>& aFileBlockSizePerResolution,
    bpConverterTypes::cImageExtent& aImageExtent,
    bpConverterTypes::tTimeInfoVector& aTimeInfoPerTimePoint,
    bpConverterTypes::tColorInfoVector& aColorInfoPerChannel,
    bpConverterTypes::tCompressionAlgorithmType& aCompressionAlgorithmType) override;

  
  void ReadParameters(bpConverterTypes::tParameters& aParameters) override;

  
  void ReadData(const bpConverterTypes::tIndex5D& aBegin, const bpConverterTypes::tIndex5D& aEnd, bpSize aResolutionIndex, TDataType* aData) override;

  
  bpImageReaderBaseInterface::cHistogram ReadHistogram(const bpVec3& aIndexTCR) override;

  
  bpImageReaderBaseInterface::cThumbnail ReadThumbnail() override;

private:

  bool IsFormat();
  void CloseFile();
  const bpfString& GetFileName() const;

  static bool ReadSection(const hid_t& aSectionId, bpfParameterSection* aParameterSection, bool aReadLongParameters);
  static herr_t ReadSectionParameter(hid_t aSectionId, const char* aAttributeName, void* aParameterSection);
  static herr_t ReadSectionLongParameter(hid_t aSectionId, const char* aAttributeName, const H5L_info_t* aLinkInfo, void* aParameterSection);
  static bool ReadAttributeString(const bpfString aAttributeName, bpfString& aAttributeValue, const hid_t& aAttributeLocation);
  static bpfString DecodeName(bpfString aName);

  bool ReadProperties();

  bpfSize GetActiveDatasetIndex();
  bpfString GetDirectoryName(const bpfString& aDirectoryName);

  bpfSize GetSizeX(bpfSize aResolutionLevel) const;
  bpfSize GetSizeY(bpfSize aResolutionLevel) const;
  bpfSize GetSizeZ(bpfSize aResolutionLevel) const;
  bpfSize GetSizeT(bpfSize aResolutionLevel) const;
  bpfSize GetSizeC(bpfSize aResolutionLevel) const;

  bpfString mFileName;
  hid_t mFileID;
  bool mSWMR;

  std::vector<bpfSize> mSizeX;
  std::vector<bpfSize> mSizeY;
  std::vector<bpfSize> mSizeZ;

  bpfSize mNumberOfResolutions;
  bpfSize mNumberOfChannels;
  bpfSize mNumberOfTimePoints;
  bpfNumberType mType;
  hid_t mHDFType;

  bpfSize mNumberOfDataSets;
  bpfSize mActiveDataSetIndex;
};

#endif // __BP_FILE_READER_IMPL__

