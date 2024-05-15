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


#ifndef __BP_IMAGE_READER_INTERFACE__
#define __BP_IMAGE_READER_INTERFACE__


#include "ImarisReader/interface/bpReaderTypes.h"


class bpImageReaderBaseInterface
{
public:
  struct cHistogram
  {
    bpFloat mMin = 0;
    bpFloat mMax = 255;
    std::vector<bpUInt64> mBins;
  };

  struct cThumbnail
  {
    bpSize mSizeX = 0;
    bpSize mSizeY = 0;
    std::vector<bpUInt8> mInterleavedRGBA;
  };

  virtual ~bpImageReaderBaseInterface() = default;

  virtual void ReadMetadata(
    std::vector<bpConverterTypes::tSize5D>& aImageSizePerResolution,
    std::vector<bpConverterTypes::tSize5D>& aFileBlockSizePerResolution,
    bpConverterTypes::cImageExtent& aImageExtent,
    bpConverterTypes::tTimeInfoVector& aTimeInfoPerTimePoint,
    bpConverterTypes::tColorInfoVector& aColorInfoPerChannel,
    bpConverterTypes::tCompressionAlgorithmType& aCompressionAlgorithmType) = 0;

  virtual void ReadParameters(bpConverterTypes::tParameters& aParameters) = 0;

  virtual cHistogram ReadHistogram(const bpVec3& aIndexTCR) = 0;

  virtual cThumbnail ReadThumbnail() = 0;
};


template<typename TDataType>
class bpImageReaderInterface : public bpImageReaderBaseInterface
{
public:
  virtual void ReadData(const bpConverterTypes::tIndex5D& aBegin, const bpConverterTypes::tIndex5D& aEnd, bpSize aResolutionIndex, TDataType* aData) = 0;
};


#endif // __BP_IMAGE_READER_INTERFACE__
