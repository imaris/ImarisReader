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


#ifndef __BP_READER_TYPES_C__
#define __BP_READER_TYPES_C__

typedef unsigned char bpReaderTypesC_UInt8;
typedef unsigned short int bpReaderTypesC_UInt16;
typedef unsigned int bpReaderTypesC_UInt32;
typedef float bpReaderTypesC_Float;

typedef enum
{
  bpReaderTypesC_UInt8Type,
  bpReaderTypesC_UInt16Type,
  bpReaderTypesC_UInt32Type,
  bpReaderTypesC_FloatType
} bpReaderTypesC_DataType;
typedef bpReaderTypesC_DataType* bpReaderTypesC_DataTypePtr;

typedef struct
{
  bpReaderTypesC_DataTypePtr mDataTypes;
  unsigned int mDataTypesSize;
} bpReaderTypesC_DataTypeVector;
typedef bpReaderTypesC_DataTypeVector* bpReaderTypesC_DataTypeVectorPtr;

typedef const char* bpReaderTypesC_String;

typedef struct
{
  unsigned int mValueX;
  unsigned int mValueY;
  unsigned int mValueZ;
  unsigned int mValueC;
  unsigned int mValueT;
} bpReaderTypesC_Index5D;
typedef bpReaderTypesC_Index5D* bpReaderTypesC_Index5DPtr;

typedef bpReaderTypesC_Index5D bpReaderTypesC_Size5D;
typedef bpReaderTypesC_Index5DPtr bpReaderTypesC_Size5DPtr;
typedef struct
{
  bpReaderTypesC_Size5DPtr mSizePerResolution;
  unsigned int mSizePerResolutionSize;
} bpReaderTypesC_Size5DVector;
typedef bpReaderTypesC_Size5DVector* bpReaderTypesC_Size5DVectorPtr;

typedef struct
{
  bool mSWMR;
} bpReaderTypesC_Options;
typedef bpReaderTypesC_Options* bpReaderTypesC_OptionsPtr;

typedef struct
{
  unsigned int mT;
  unsigned int mC;
  unsigned int mR;
} bpReaderTypesC_IndexTCR;
typedef const bpReaderTypesC_IndexTCR* bpReaderTypesC_IndexTCRPtr;

typedef struct
{
  unsigned int mMin;
  unsigned int mMax;
  unsigned long long* mBins;
  unsigned int mBinsSize;
} bpReaderTypesC_Histogram;
typedef bpReaderTypesC_Histogram* bpReaderTypesC_HistogramPtr;

typedef struct
{
  unsigned int mSizeX;
  unsigned int mSizeY;
  unsigned char* mInterleavedRGBA;
  unsigned int mInterleavedRGBASize;
} bpReaderTypesC_Thumbnail;
typedef bpReaderTypesC_Thumbnail* bpReaderTypesC_ThumbnailPtr;

typedef struct
{
  float mExtentMinX;
  float mExtentMinY;
  float mExtentMinZ;
  float mExtentMaxX;
  float mExtentMaxY;
  float mExtentMaxZ;
} bpReaderTypesC_ImageExtent;
typedef bpReaderTypesC_ImageExtent* bpReaderTypesC_ImageExtentPtr;

// time info
typedef struct {
  unsigned int mJulianDay;
  unsigned long long mNanosecondsOfDay;
} bpReaderTypesC_TimeInfo;

typedef bpReaderTypesC_TimeInfo* bpReaderTypesC_TimeInfoPtr;

typedef struct {
  bpReaderTypesC_TimeInfoPtr mValues;
  unsigned int mValuesSize;
} bpReaderTypesC_TimeInfos;

typedef bpReaderTypesC_TimeInfos* bpReaderTypesC_TimeInfoVectorPtr;

// color info
typedef struct
{
  float mRed;
  float mGreen;
  float mBlue;
  float mAlpha;
} bpReaderTypesC_Color;
typedef bpReaderTypesC_Color* bpReaderTypesC_ColorPtr;

typedef struct {
  bool mIsBaseColorMode;
  bpReaderTypesC_Color mBaseColor;
  bpReaderTypesC_ColorPtr mColorTable;
  unsigned int mColorTableSize;
  float mOpacity;
  float mRangeMin;
  float mRangeMax;
  float mGammaCorrection;
} bpReaderTypesC_ColorInfo;
typedef bpReaderTypesC_ColorInfo* bpReaderTypesC_ColorInfoPtr;

typedef struct {
  bpReaderTypesC_ColorInfoPtr mValues;
  unsigned int mValuesSize;
} bpReaderTypesC_ColorInfos;
typedef bpReaderTypesC_ColorInfos* bpReaderTypesC_ColorInfoVectorPtr;

// compression algorithm
typedef enum {
  eCompressionAlgorithmNone = 0,
  eCompressionAlgorithmGzipLevel1 = 1,
  eCompressionAlgorithmGzipLevel2 = 2,
  eCompressionAlgorithmGzipLevel3 = 3,
  eCompressionAlgorithmGzipLevel4 = 4,
  eCompressionAlgorithmGzipLevel5 = 5,
  eCompressionAlgorithmGzipLevel6 = 6,
  eCompressionAlgorithmGzipLevel7 = 7,
  eCompressionAlgorithmGzipLevel8 = 8,
  eCompressionAlgorithmGzipLevel9 = 9,
  eCompressionAlgorithmShuffleGzipLevel1 = 11,
  eCompressionAlgorithmShuffleGzipLevel2 = 12,
  eCompressionAlgorithmShuffleGzipLevel3 = 13,
  eCompressionAlgorithmShuffleGzipLevel4 = 14,
  eCompressionAlgorithmShuffleGzipLevel5 = 15,
  eCompressionAlgorithmShuffleGzipLevel6 = 16,
  eCompressionAlgorithmShuffleGzipLevel7 = 17,
  eCompressionAlgorithmShuffleGzipLevel8 = 18,
  eCompressionAlgorithmShuffleGzipLevel9 = 19,
  eCompressionAlgorithmLZ4 = 21,
  eCompressionAlgorithmLShuffleLZ4 = 31
} bpReaderTypesC_CompressionAlgorithmType;

// parameters
typedef struct {
  bpReaderTypesC_String mName;
  bpReaderTypesC_String mValue;
} bpReaderTypesC_Parameter;

typedef struct {
  bpReaderTypesC_String mName;
  bpReaderTypesC_Parameter* mValues;
  unsigned int mValuesCount;
} bpReaderTypesC_ParameterSection;

typedef struct {
  bpReaderTypesC_ParameterSection* mValues;
  unsigned int mValuesCount;
} bpReaderTypesC_Parameters;

typedef bpReaderTypesC_Parameters* bpReaderTypesC_ParametersPtr;

#endif // __BP_READER_TYPES_C__
