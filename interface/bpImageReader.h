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


#ifndef __BP_IMAGE_READER__
#define __BP_IMAGE_READER__

#include "bpImarisReaderDllAPI.h"
#include "bpImageReaderInterface.h"


template<class TDataType>
class bpfImageConverterImpl;


 // image reader template should be created according to the given type for a specific image
BP_IMARISREADER_DLL_API std::vector<bpConverterTypes::tDataType> GetFileImagesInformation(const bpString& aInputFile, bool aSWMR);


template<class TDataType>
class BP_IMARISREADER_DLL_API bpImageReader : public bpImageReaderInterface<TDataType>
{
public:
  bpImageReader(const bpString& aInputFile, bpSize aImageIndex, const bpReaderTypes::cReadOptions& aOptions);
  ~bpImageReader();

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
  class cThreadSafeDecorator;

  bpUniquePtr<bpImageReaderInterface<TDataType>> mImpl;
};


#endif // __BP_IMAGE_READER__
