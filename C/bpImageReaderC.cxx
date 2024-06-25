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


#define EXPORT_IMARISREADER_DLL
#include "../interfaceC/bpImageReaderInterfaceC.h"
#include "../interface/bpImageReader.h"

static bpString Convert(bpReaderTypesC_String aString)
{
  return bpString(aString ? aString : "");
}


static bpReaderTypesC_DataType Convert(bpConverterTypes::tDataType aDataType) {
  bpReaderTypesC_DataType vDataType;
  switch (aDataType) {
    case 2:
      vDataType = bpReaderTypesC_UInt8Type;
      break;
    case 4:
      vDataType = bpReaderTypesC_UInt16Type;
      break;
    case 6:
      vDataType = bpReaderTypesC_UInt32Type;
      break;
    case 7:
      vDataType = bpReaderTypesC_FloatType;
      break;
    default:
      throw "DataType not supported";
  }
  return vDataType;
}


static void Convert(bpReaderTypesC_DataTypeVectorPtr aDataTypeVectorPtrC, std::vector<bpConverterTypes::tDataType> aDataTypeVector) {
  aDataTypeVectorPtrC->mDataTypesSize = aDataTypeVector.size();
  aDataTypeVectorPtrC->mDataTypes = (bpReaderTypesC_DataTypePtr)malloc(aDataTypeVector.size() * sizeof(bpReaderTypesC_DataType));
  for (bpSize vIndex = 0; vIndex < aDataTypeVector.size(); ++vIndex) {
    *(aDataTypeVectorPtrC->mDataTypes + vIndex) = Convert(aDataTypeVector[vIndex]);
  }
}


void bpImageReaderC_FreeDataTypes(bpReaderTypesC_DataTypeVectorPtr aDataTypes) {
  free(aDataTypes->mDataTypes);
}


static bpConverterTypes::tIndex5D Convert(bpReaderTypesC_Index5DPtr aIndex)
{
  if (!aIndex) {
    throw "Index is not optional.";
  }

  bpConverterTypes::tIndex5D vIndex{
    bpConverterTypes::X, aIndex->mValueX,
    bpConverterTypes::Y, aIndex->mValueY,
    bpConverterTypes::Z, aIndex->mValueZ,
    bpConverterTypes::C, aIndex->mValueC,
    bpConverterTypes::T, aIndex->mValueT
  };
  return vIndex;
}


static bpReaderTypes::cReadOptions Convert(bpReaderTypesC_OptionsPtr aOptions)
{
  if (!aOptions) {
    return{};
  }

  bpReaderTypes::cReadOptions vOptions;
  vOptions.mSWMR = aOptions->mSWMR;
  return vOptions;
}


static bpVec3 Convert(bpReaderTypesC_IndexTCRPtr aIndex) {
  bpVec3 vIndex;
  vIndex[0] = aIndex->mT;
  vIndex[1] = aIndex->mC;
  vIndex[2] = aIndex->mR;
  return vIndex;
}


static bpReaderTypesC_HistogramPtr Convert(bpImageReaderBaseInterface::cHistogram aHistogram)
{
  bpReaderTypesC_HistogramPtr vHistogram = new bpReaderTypesC_Histogram;
  vHistogram->mMin = aHistogram.mMin;
  vHistogram->mMax = aHistogram.mMax;
  vHistogram->mBinsSize = aHistogram.mBins.size();
  vHistogram->mBins = aHistogram.mBins.data();
  return vHistogram;
}


static bpReaderTypesC_ThumbnailPtr Convert(bpImageReaderBaseInterface::cThumbnail aThumbnail)
{
  bpReaderTypesC_ThumbnailPtr vThumbnail = new bpReaderTypesC_Thumbnail;
  vThumbnail->mSizeX = aThumbnail.mSizeX;
  vThumbnail->mSizeY = aThumbnail.mSizeY;
  vThumbnail->mInterleavedRGBASize = aThumbnail.mInterleavedRGBA.size();
  vThumbnail->mInterleavedRGBA = aThumbnail.mInterleavedRGBA.data();
  return vThumbnail;
}


static bpReaderTypesC_Size5D Convert(bpConverterTypes::tSize5D aSize)
{
  bpReaderTypesC_Size5D vSize;
  vSize.mValueX = aSize[bpConverterTypes::X];
  vSize.mValueY = aSize[bpConverterTypes::Y];
  vSize.mValueZ = aSize[bpConverterTypes::Z];
  vSize.mValueC = aSize[bpConverterTypes::C];
  vSize.mValueT = aSize[bpConverterTypes::T];
  return vSize;
}

static void Convert(bpReaderTypesC_Size5DVectorPtr aSizesC, const std::vector<bpConverterTypes::tSize5D>& aSizes)
{
  aSizesC->mSizePerResolutionSize = aSizes.size();
  aSizesC->mSizePerResolution = (bpReaderTypesC_Size5DPtr)malloc(aSizes.size() * sizeof(bpReaderTypesC_Size5D));
  for (bpSize vIndex = 0; vIndex < aSizes.size(); ++vIndex) {
    *(aSizesC->mSizePerResolution + vIndex) = Convert(aSizes[vIndex]);
  }
}


static void Convert(bpReaderTypesC_ImageExtentPtr aImageExtentC, const bpConverterTypes::cImageExtent& aImageExtent)
{
  aImageExtentC->mExtentMinX = aImageExtent.mExtentMinX;
  aImageExtentC->mExtentMinY = aImageExtent.mExtentMinY;
  aImageExtentC->mExtentMinZ = aImageExtent.mExtentMinZ;
  aImageExtentC->mExtentMaxX = aImageExtent.mExtentMaxX;
  aImageExtentC->mExtentMaxY = aImageExtent.mExtentMaxY;
  aImageExtentC->mExtentMaxZ = aImageExtent.mExtentMaxZ;
}


static bpReaderTypesC_TimeInfo Convert(const bpConverterTypes::cTimeInfo& aTimeInfo)
{
  bpReaderTypesC_TimeInfo vTimeInfo;
  vTimeInfo.mJulianDay = aTimeInfo.mJulianDay;
  vTimeInfo.mNanosecondsOfDay = aTimeInfo.mNanosecondsOfDay;
  return vTimeInfo;
}

static void Convert(bpReaderTypesC_TimeInfoVectorPtr aTimeInfoPerTimePointC, const bpConverterTypes::tTimeInfoVector& aTimeInfoPerTimePoint)
{
  aTimeInfoPerTimePointC->mValuesSize = aTimeInfoPerTimePoint.size();
  aTimeInfoPerTimePointC->mValues = (bpReaderTypesC_TimeInfoPtr)malloc(aTimeInfoPerTimePoint.size() * sizeof(bpReaderTypesC_TimeInfo));
  for (bpSize vIndex = 0; vIndex < aTimeInfoPerTimePoint.size(); ++vIndex) {
    *(aTimeInfoPerTimePointC->mValues + vIndex) = Convert(aTimeInfoPerTimePoint[vIndex]);
  }
}

static bpReaderTypesC_Color Convert(const bpConverterTypes::cColor& aColor)
{
  bpReaderTypesC_Color vColor;
  vColor.mRed = aColor.mRed;
  vColor.mGreen = aColor.mGreen;
  vColor.mBlue = aColor.mBlue;
  vColor.mAlpha = aColor.mAlpha;
  return vColor;
}

static bpReaderTypesC_ColorInfo Convert(const bpConverterTypes::cColorInfo& aColorInfo)
{
  bpReaderTypesC_ColorInfo vColorInfo;
  vColorInfo.mBaseColor = Convert(aColorInfo.mBaseColor);
  vColorInfo.mColorTableSize = aColorInfo.mColorTable.size();
  vColorInfo.mColorTable = (bpReaderTypesC_ColorPtr)malloc(aColorInfo.mColorTable.size() * sizeof(bpReaderTypesC_Color));
  for (bpSize vIndex = 0; vIndex < aColorInfo.mColorTable.size(); ++vIndex) {
    *(vColorInfo.mColorTable + vIndex) = Convert(aColorInfo.mColorTable[vIndex]);
  }
  vColorInfo.mGammaCorrection = aColorInfo.mGammaCorrection;
  vColorInfo.mIsBaseColorMode = aColorInfo.mIsBaseColorMode;
  vColorInfo.mOpacity = aColorInfo.mOpacity;
  vColorInfo.mRangeMin = aColorInfo.mRangeMin;
  vColorInfo.mRangeMax = aColorInfo.mRangeMax;
  return vColorInfo;
}

static void Convert(bpReaderTypesC_ColorInfoVectorPtr aColorInfoPerChannelC, const bpConverterTypes::tColorInfoVector& aColorInfoPerChannel)
{
  aColorInfoPerChannelC->mValuesSize = aColorInfoPerChannel.size();
  aColorInfoPerChannelC->mValues = (bpReaderTypesC_ColorInfoPtr)malloc(aColorInfoPerChannel.size() * sizeof(bpReaderTypesC_ColorInfo));
  for (bpSize vIndex = 0; vIndex < aColorInfoPerChannel.size(); ++vIndex) {
    *(aColorInfoPerChannelC->mValues + vIndex) = Convert(aColorInfoPerChannel[vIndex]);
  }
}


static bpReaderTypesC_CompressionAlgorithmType Convert(bpConverterTypes::tCompressionAlgorithmType aAlgorithmType) {
  bpReaderTypesC_CompressionAlgorithmType vAlgorithmType;
  switch (aAlgorithmType) {
    case 0:
      vAlgorithmType = eCompressionAlgorithmNone;
      break;
    case 1:
      vAlgorithmType = eCompressionAlgorithmGzipLevel1;
      break;
    case 2:
      vAlgorithmType = eCompressionAlgorithmGzipLevel2;
      break;
    case 3:
      vAlgorithmType = eCompressionAlgorithmGzipLevel3;
      break;
    case 4:
      vAlgorithmType = eCompressionAlgorithmGzipLevel4;
      break;
    case 5:
      vAlgorithmType = eCompressionAlgorithmGzipLevel5;
      break;
    case 6:
      vAlgorithmType = eCompressionAlgorithmGzipLevel6;
      break;
    case 7:
      vAlgorithmType = eCompressionAlgorithmGzipLevel7;
      break;
    case 8:
      vAlgorithmType = eCompressionAlgorithmGzipLevel8;
      break;
    case 9:
      vAlgorithmType = eCompressionAlgorithmGzipLevel9;
      break;
    case 11:
      vAlgorithmType = eCompressionAlgorithmShuffleGzipLevel1;
      break;
    case 12:
      vAlgorithmType = eCompressionAlgorithmShuffleGzipLevel2;
      break;
    case 13:
      vAlgorithmType = eCompressionAlgorithmShuffleGzipLevel3;
      break;
    case 14:
      vAlgorithmType = eCompressionAlgorithmShuffleGzipLevel4;
      break;
    case 15:
      vAlgorithmType = eCompressionAlgorithmShuffleGzipLevel5;
      break;
    case 16:
      vAlgorithmType = eCompressionAlgorithmShuffleGzipLevel6;
      break;
    case 17:
      vAlgorithmType = eCompressionAlgorithmShuffleGzipLevel7;
      break;
    case 18:
      vAlgorithmType = eCompressionAlgorithmShuffleGzipLevel8;
      break;
    case 19:
      vAlgorithmType = eCompressionAlgorithmShuffleGzipLevel9;
      break;
    case 21:
      vAlgorithmType = eCompressionAlgorithmLZ4;
      break;
    case 31:
      vAlgorithmType = eCompressionAlgorithmLShuffleLZ4;
      break;
  }
  return vAlgorithmType;
}


static void Convert(bpReaderTypesC_ParametersPtr aParamsC, const bpConverterTypes::tParameters& aParams)
{
  aParamsC->mValuesCount = aParams.size();
  aParamsC->mValues = (bpReaderTypesC_ParameterSection*)malloc(aParams.size() * sizeof(bpReaderTypesC_ParameterSection));
  auto vSectionIt = aParams.begin();
  for (bpSize vIndexSection = 0; vIndexSection < aParams.size(); ++vIndexSection) {
    (*(aParamsC->mValues + vIndexSection)).mName = strdup(vSectionIt->first.c_str()); // malloc
    (*(aParamsC->mValues + vIndexSection)).mValuesCount = vSectionIt->second.size();
    (*(aParamsC->mValues + vIndexSection)).mValues = (bpReaderTypesC_Parameter*)malloc(vSectionIt->second.size() * sizeof(bpReaderTypesC_Parameter));
    auto vParamIt = vSectionIt->second.begin();
    for (bpSize vIndexParam = 0; vIndexParam < vSectionIt->second.size(); ++vIndexParam) {
      ((aParamsC->mValues + vIndexSection)->mValues + vIndexParam)->mName = strdup(vParamIt->first.c_str()); // malloc
      ((aParamsC->mValues + vIndexSection)->mValues + vIndexParam)->mValue = strdup(vParamIt->second.c_str()); // malloc
      vParamIt++;
    }
    vSectionIt++;
  }
}


bpReaderTypesC_DataTypeVectorPtr bpImageReaderC_GetFileImagesInformation(bpReaderTypesC_String aInputFile, bool aSWMR) {
  bpReaderTypesC_DataTypeVectorPtr vDataTypeVectorPtr = new bpReaderTypesC_DataTypeVector;
  Convert(vDataTypeVectorPtr, GetFileImagesInformation(Convert(aInputFile), aSWMR));
  return vDataTypeVectorPtr;
}


bpImageReaderCPtr bpImageReaderC_CreateUInt8(bpReaderTypesC_String aInputFile, unsigned int aImageIndex, bpReaderTypesC_OptionsPtr aOptions) {
  return reinterpret_cast<bpImageReaderCPtr>(new bpImageReader<bpUInt8>(Convert(aInputFile), aImageIndex, Convert(aOptions)));
}
bpImageReaderCPtr bpImageReaderC_CreateUInt16(bpReaderTypesC_String aInputFile, unsigned int aImageIndex, bpReaderTypesC_OptionsPtr aOptions) {
  return reinterpret_cast<bpImageReaderCPtr>(new bpImageReader<bpUInt16>(Convert(aInputFile), aImageIndex, Convert(aOptions)));
}
bpImageReaderCPtr bpImageReaderC_CreateUInt32(bpReaderTypesC_String aInputFile, unsigned int aImageIndex, bpReaderTypesC_OptionsPtr aOptions) {
  return reinterpret_cast<bpImageReaderCPtr>(new bpImageReader<bpUInt32>(Convert(aInputFile), aImageIndex, Convert(aOptions)));
}
bpImageReaderCPtr bpImageReaderC_CreateFloat(bpReaderTypesC_String aInputFile, unsigned int aImageIndex, bpReaderTypesC_OptionsPtr aOptions) {
  return reinterpret_cast<bpImageReaderCPtr>(new bpImageReader<bpFloat>(Convert(aInputFile), aImageIndex, Convert(aOptions)));
}


void bpImageReaderC_DestroyUInt8(bpImageReaderCPtr aImageReaderC) {
  delete reinterpret_cast<bpImageReader<bpUInt8>*>(aImageReaderC);
}
void bpImageReaderC_DestroyUInt16(bpImageReaderCPtr aImageReaderC) {
  delete reinterpret_cast<bpImageReader<bpUInt16>*>(aImageReaderC);
}
void bpImageReaderC_DestroyUInt32(bpImageReaderCPtr aImageReaderC) {
  delete reinterpret_cast<bpImageReader<bpUInt32>*>(aImageReaderC);
}
void bpImageReaderC_DestroyFloat(bpImageReaderCPtr aImageReaderC) {
  delete reinterpret_cast<bpImageReader<bpFloat>*>(aImageReaderC);
}


void bpImageReaderC_ReadDataUInt8(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_Index5DPtr aBegin, bpReaderTypesC_Index5DPtr aEnd,
                                  unsigned int aResolutionIndex, bpReaderTypesC_UInt8* aData) {
  reinterpret_cast<bpImageReader<bpUInt8>*>(aImageReaderC)->ReadData(Convert(aBegin), Convert(aEnd), aResolutionIndex, aData);
}
void bpImageReaderC_ReadDataUInt16(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_Index5DPtr aBegin, bpReaderTypesC_Index5DPtr aEnd,
                                  unsigned int aResolutionIndex, bpReaderTypesC_UInt16* aData) {
  reinterpret_cast<bpImageReader<bpUInt16>*>(aImageReaderC)->ReadData(Convert(aBegin), Convert(aEnd), aResolutionIndex, aData);
}
void bpImageReaderC_ReadDataUInt32(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_Index5DPtr aBegin, bpReaderTypesC_Index5DPtr aEnd,
                                  unsigned int aResolutionIndex, bpReaderTypesC_UInt32* aData) {
  reinterpret_cast<bpImageReader<bpUInt32>*>(aImageReaderC)->ReadData(Convert(aBegin), Convert(aEnd), aResolutionIndex, aData);
}
void bpImageReaderC_ReadDataFloat(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_Index5DPtr aBegin, bpReaderTypesC_Index5DPtr aEnd,
                                  unsigned int aResolutionIndex, bpReaderTypesC_Float* aData) {
  reinterpret_cast<bpImageReader<bpFloat>*>(aImageReaderC)->ReadData(Convert(aBegin), Convert(aEnd), aResolutionIndex, aData);
}


void bpImageReaderC_ReadMetadataUInt8(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_Size5DVectorPtr aImageSizePerResolution,
                                 bpReaderTypesC_Size5DVectorPtr aBlockSizePerResolution, bpReaderTypesC_ImageExtentPtr aImageExtent,
                                 bpReaderTypesC_TimeInfoVectorPtr aTimeInfoPerTimePoint, bpReaderTypesC_ColorInfoVectorPtr aColorInfoPerChannel,
                                 bpReaderTypesC_CompressionAlgorithmType& aCompressionAlgorithmType) {
  std::vector<bpConverterTypes::tSize5D> vImageSizePerResolution;
  std::vector<bpConverterTypes::tSize5D> vBlockSizePerResolution;
  bpConverterTypes::cImageExtent vImageExtent;
  bpConverterTypes::tTimeInfoVector vTimeInfoPerTimePoint;
  bpConverterTypes::tColorInfoVector vColorInfoPerChannel;
  bpConverterTypes::tCompressionAlgorithmType vCompressionAlgorithmType;
  reinterpret_cast<bpImageReader<bpUInt8>*>(aImageReaderC)->ReadMetadata(vImageSizePerResolution, vBlockSizePerResolution,
                                                                         vImageExtent, vTimeInfoPerTimePoint,
                                                                         vColorInfoPerChannel, vCompressionAlgorithmType);
  Convert(aImageSizePerResolution, vImageSizePerResolution);
  Convert(aBlockSizePerResolution, vBlockSizePerResolution);
  Convert(aImageExtent, vImageExtent);
  Convert(aTimeInfoPerTimePoint, vTimeInfoPerTimePoint);
  Convert(aColorInfoPerChannel, vColorInfoPerChannel);
  aCompressionAlgorithmType = Convert(vCompressionAlgorithmType);
}
void bpImageReaderC_ReadMetadataUInt16(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_Size5DVectorPtr aImageSizePerResolution,
                                 bpReaderTypesC_Size5DVectorPtr aBlockSizePerResolution, bpReaderTypesC_ImageExtentPtr aImageExtent,
                                 bpReaderTypesC_TimeInfoVectorPtr aTimeInfoPerTimePoint, bpReaderTypesC_ColorInfoVectorPtr aColorInfoPerChannel,
                                 bpReaderTypesC_CompressionAlgorithmType& aCompressionAlgorithmType) {
  std::vector<bpConverterTypes::tSize5D> vImageSizePerResolution;
  std::vector<bpConverterTypes::tSize5D> vBlockSizePerResolution;
  bpConverterTypes::cImageExtent vImageExtent;
  bpConverterTypes::tTimeInfoVector vTimeInfoPerTimePoint;
  bpConverterTypes::tColorInfoVector vColorInfoPerChannel;
  bpConverterTypes::tCompressionAlgorithmType vCompressionAlgorithmType;
  reinterpret_cast<bpImageReader<bpUInt16>*>(aImageReaderC)->ReadMetadata(vImageSizePerResolution, vBlockSizePerResolution,
                                                                         vImageExtent, vTimeInfoPerTimePoint,
                                                                         vColorInfoPerChannel, vCompressionAlgorithmType);
  Convert(aImageSizePerResolution, vImageSizePerResolution);
  Convert(aBlockSizePerResolution, vBlockSizePerResolution);
  Convert(aImageExtent, vImageExtent);
  Convert(aTimeInfoPerTimePoint, vTimeInfoPerTimePoint);
  Convert(aColorInfoPerChannel, vColorInfoPerChannel);
  aCompressionAlgorithmType = Convert(vCompressionAlgorithmType);
}
void bpImageReaderC_ReadMetadataUInt32(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_Size5DVectorPtr aImageSizePerResolution,
                                 bpReaderTypesC_Size5DVectorPtr aBlockSizePerResolution, bpReaderTypesC_ImageExtentPtr aImageExtent,
                                 bpReaderTypesC_TimeInfoVectorPtr aTimeInfoPerTimePoint, bpReaderTypesC_ColorInfoVectorPtr aColorInfoPerChannel,
                                 bpReaderTypesC_CompressionAlgorithmType& aCompressionAlgorithmType) {
  std::vector<bpConverterTypes::tSize5D> vImageSizePerResolution;
  std::vector<bpConverterTypes::tSize5D> vBlockSizePerResolution;
  bpConverterTypes::cImageExtent vImageExtent;
  bpConverterTypes::tTimeInfoVector vTimeInfoPerTimePoint;
  bpConverterTypes::tColorInfoVector vColorInfoPerChannel;
  bpConverterTypes::tCompressionAlgorithmType vCompressionAlgorithmType;
  reinterpret_cast<bpImageReader<bpUInt32>*>(aImageReaderC)->ReadMetadata(vImageSizePerResolution, vBlockSizePerResolution,
                                                                         vImageExtent, vTimeInfoPerTimePoint,
                                                                         vColorInfoPerChannel, vCompressionAlgorithmType);
  Convert(aImageSizePerResolution, vImageSizePerResolution);
  Convert(aBlockSizePerResolution, vBlockSizePerResolution);
  Convert(aImageExtent, vImageExtent);
  Convert(aTimeInfoPerTimePoint, vTimeInfoPerTimePoint);
  Convert(aColorInfoPerChannel, vColorInfoPerChannel);
  aCompressionAlgorithmType = Convert(vCompressionAlgorithmType);
}
void bpImageReaderC_ReadMetadataFloat(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_Size5DVectorPtr aImageSizePerResolution,
                                 bpReaderTypesC_Size5DVectorPtr aBlockSizePerResolution, bpReaderTypesC_ImageExtentPtr aImageExtent,
                                 bpReaderTypesC_TimeInfoVectorPtr aTimeInfoPerTimePoint, bpReaderTypesC_ColorInfoVectorPtr aColorInfoPerChannel,
                                 bpReaderTypesC_CompressionAlgorithmType& aCompressionAlgorithmType) {
  std::vector<bpConverterTypes::tSize5D> vImageSizePerResolution;
  std::vector<bpConverterTypes::tSize5D> vBlockSizePerResolution;
  bpConverterTypes::cImageExtent vImageExtent;
  bpConverterTypes::tTimeInfoVector vTimeInfoPerTimePoint;
  bpConverterTypes::tColorInfoVector vColorInfoPerChannel;
  bpConverterTypes::tCompressionAlgorithmType vCompressionAlgorithmType;
  reinterpret_cast<bpImageReader<bpFloat>*>(aImageReaderC)->ReadMetadata(vImageSizePerResolution, vBlockSizePerResolution,
                                                                         vImageExtent, vTimeInfoPerTimePoint,
                                                                         vColorInfoPerChannel, vCompressionAlgorithmType);
  Convert(aImageSizePerResolution, vImageSizePerResolution);
  Convert(aBlockSizePerResolution, vBlockSizePerResolution);
  Convert(aImageExtent, vImageExtent);
  Convert(aTimeInfoPerTimePoint, vTimeInfoPerTimePoint);
  Convert(aColorInfoPerChannel, vColorInfoPerChannel);
  aCompressionAlgorithmType = Convert(vCompressionAlgorithmType);
}

void bpImageReaderC_FreeMetadata(bpReaderTypesC_Size5DVectorPtr aImageSizePerResolution, bpReaderTypesC_Size5DVectorPtr aBlockSizePerResolution,
                                 bpReaderTypesC_TimeInfoVectorPtr aTimeInfoPerTimePoint, bpReaderTypesC_ColorInfoVectorPtr aColorInfoPerChannel) {
  free(aImageSizePerResolution->mSizePerResolution);
  free(aBlockSizePerResolution->mSizePerResolution);
  free(aTimeInfoPerTimePoint->mValues);
  for (unsigned int vIndex = 0; vIndex < aColorInfoPerChannel->mValuesSize; vIndex++) {
    free(aColorInfoPerChannel->mValues[vIndex].mColorTable);
  }
  free(aColorInfoPerChannel->mValues);
}


void bpImageReaderC_ReadParametersUInt8(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_ParametersPtr aParams) {
  bpConverterTypes::tParameters vParams;
  reinterpret_cast<bpImageReader<bpUInt8>*>(aImageReaderC)->ReadParameters(vParams);
  Convert(aParams, vParams);
}
void bpImageReaderC_ReadParametersUInt16(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_ParametersPtr aParams) {
  bpConverterTypes::tParameters vParams;
  reinterpret_cast<bpImageReader<bpUInt16>*>(aImageReaderC)->ReadParameters(vParams);
  Convert(aParams, vParams);
}
void bpImageReaderC_ReadParametersUInt32(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_ParametersPtr aParams) {
  bpConverterTypes::tParameters vParams;
  reinterpret_cast<bpImageReader<bpUInt32>*>(aImageReaderC)->ReadParameters(vParams);
  Convert(aParams, vParams);
}
void bpImageReaderC_ReadParametersFloat(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_ParametersPtr aParams) {
  bpConverterTypes::tParameters vParams;
  reinterpret_cast<bpImageReader<bpFloat>*>(aImageReaderC)->ReadParameters(vParams);
  Convert(aParams, vParams);
}

void bpImageReaderC_FreeParameters(bpReaderTypesC_ParametersPtr aParams) {
  for (unsigned int vI = 0; vI < aParams->mValuesCount; vI++) {
    free(const_cast<char*>(aParams->mValues[vI].mName));
    for (unsigned int vJ = 0; vJ < aParams->mValues[vI].mValuesCount; vJ++) {
      free(const_cast<char*>(aParams->mValues[vI].mValues[vJ].mName));
      free(const_cast<char*>(aParams->mValues[vI].mValues[vJ].mValue));
    }
    free(aParams->mValues[vI].mValues);
  }
  free(aParams->mValues);
}


bpReaderTypesC_HistogramPtr bpImageReaderC_ReadHistogramUInt8(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_IndexTCRPtr aIndex) {
  return Convert(reinterpret_cast<bpImageReader<bpUInt8>*>(aImageReaderC)->ReadHistogram(Convert(aIndex)));
}
bpReaderTypesC_HistogramPtr bpImageReaderC_ReadHistogramUInt16(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_IndexTCRPtr aIndex) {
  return Convert(reinterpret_cast<bpImageReader<bpUInt16>*>(aImageReaderC)->ReadHistogram(Convert(aIndex)));
}
bpReaderTypesC_HistogramPtr bpImageReaderC_ReadHistogramUInt32(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_IndexTCRPtr aIndex) {
  return Convert(reinterpret_cast<bpImageReader<bpUInt32>*>(aImageReaderC)->ReadHistogram(Convert(aIndex)));
}
bpReaderTypesC_HistogramPtr bpImageReaderC_ReadHistogramFloat(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_IndexTCRPtr aIndex) {
  return Convert(reinterpret_cast<bpImageReader<bpFloat>*>(aImageReaderC)->ReadHistogram(Convert(aIndex)));
}


bpReaderTypesC_ThumbnailPtr bpImageReaderC_ReadThumbnailUInt8(bpImageReaderCPtr aImageReaderC) {
  return Convert(reinterpret_cast<bpImageReader<bpUInt8>*>(aImageReaderC)->ReadThumbnail());
}
bpReaderTypesC_ThumbnailPtr bpImageReaderC_ReadThumbnailUInt16(bpImageReaderCPtr aImageReaderC) {
  return Convert(reinterpret_cast<bpImageReader<bpUInt16>*>(aImageReaderC)->ReadThumbnail());
}
bpReaderTypesC_ThumbnailPtr bpImageReaderC_ReadThumbnailUInt32(bpImageReaderCPtr aImageReaderC) {
  return Convert(reinterpret_cast<bpImageReader<bpUInt32>*>(aImageReaderC)->ReadThumbnail());
}
bpReaderTypesC_ThumbnailPtr bpImageReaderC_ReadThumbnailFloat(bpImageReaderCPtr aImageReaderC) {
  return Convert(reinterpret_cast<bpImageReader<bpFloat>*>(aImageReaderC)->ReadThumbnail());
}
