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


#ifndef __BP_IMAGE_READER_INTERFACE_C__
#define __BP_IMAGE_READER_INTERFACE_C__


#ifdef _WIN32
#define BP_IMARISREADER_DLL_EXPORT __declspec( dllexport )
#define BP_IMARISREADER_DLL_IMPORT __declspec( dllimport )
#else
#define BP_IMARISREADER_DLL_EXPORT __attribute__((visibility("default")))
#define BP_IMARISREADER_DLL_IMPORT __attribute__((visibility("default")))
#endif

#ifdef EXPORT_IMARISREADER_DLL
#define BP_IMARISREADER_DLL_API BP_IMARISREADER_DLL_EXPORT
#else
#define BP_IMARISREADER_DLL_API BP_IMARISREADER_DLL_IMPORT
#endif

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "bpReaderTypesC.h"

struct bpImageReaderC;
typedef struct bpImageReaderC* bpImageReaderCPtr;

BP_IMARISREADER_DLL_API bpImageReaderCPtr bpImageReaderC_CreateUInt8(bpReaderTypesC_String aInputFile, unsigned int aImageIndex, bpReaderTypesC_OptionsPtr aOptions);
BP_IMARISREADER_DLL_API bpImageReaderCPtr bpImageReaderC_CreateUInt16(bpReaderTypesC_String aInputFile, unsigned int aImageIndex, bpReaderTypesC_OptionsPtr aOptions);
BP_IMARISREADER_DLL_API bpImageReaderCPtr bpImageReaderC_CreateUInt32(bpReaderTypesC_String aInputFile, unsigned int aImageIndex, bpReaderTypesC_OptionsPtr aOptions);
BP_IMARISREADER_DLL_API bpImageReaderCPtr bpImageReaderC_CreateFloat(bpReaderTypesC_String aInputFile, unsigned int aImageIndex, bpReaderTypesC_OptionsPtr aOptions);

BP_IMARISREADER_DLL_API void bpImageReaderC_DestroyUInt8(bpImageReaderCPtr aImageReaderC);
BP_IMARISREADER_DLL_API void bpImageReaderC_DestroyUInt16(bpImageReaderCPtr aImageReaderC);
BP_IMARISREADER_DLL_API void bpImageReaderC_DestroyUInt32(bpImageReaderCPtr aImageReaderC);
BP_IMARISREADER_DLL_API void bpImageReaderC_DestroyFloat(bpImageReaderCPtr aImageReaderC);

BP_IMARISREADER_DLL_API void bpImageReaderC_ReadDataUInt8(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_Index5DPtr aBegin, bpReaderTypesC_Index5DPtr aEnd,
                                                          unsigned int aResolutionIndex, bpReaderTypesC_UInt8* aData);
BP_IMARISREADER_DLL_API void bpImageReaderC_ReadDataUInt16(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_Index5DPtr aBegin, bpReaderTypesC_Index5DPtr aEnd,
                                                          unsigned int aResolutionIndex, bpReaderTypesC_UInt16* aData);
BP_IMARISREADER_DLL_API void bpImageReaderC_ReadDataUInt32(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_Index5DPtr aBegin, bpReaderTypesC_Index5DPtr aEnd,
                                                          unsigned int aResolutionIndex, bpReaderTypesC_UInt32* aData);
BP_IMARISREADER_DLL_API void bpImageReaderC_ReadDataFloat(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_Index5DPtr aBegin, bpReaderTypesC_Index5DPtr aEnd,
                                                          unsigned int aResolutionIndex, bpReaderTypesC_Float* aData);

BP_IMARISREADER_DLL_API void bpImageReaderC_ReadMetadataUInt8(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_Size5DVectorPtr aImageSizePerResolution,
                                                         bpReaderTypesC_Size5DVectorPtr aBlockSizePerResolution, bpReaderTypesC_ImageExtentPtr aImageExtent,
                                                         bpReaderTypesC_TimeInfoVectorPtr aTimeInfoPerTimePoint, bpReaderTypesC_ColorInfoVectorPtr aColorInfoPerChannel,
                                                         bpReaderTypesC_CompressionAlgorithmType& aCompressionAlgorithmType);
BP_IMARISREADER_DLL_API void bpImageReaderC_ReadMetadataUInt16(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_Size5DVectorPtr aImageSizePerResolution,
                                                         bpReaderTypesC_Size5DVectorPtr aBlockSizePerResolution, bpReaderTypesC_ImageExtentPtr aImageExtent,
                                                         bpReaderTypesC_TimeInfoVectorPtr aTimeInfoPerTimePoint, bpReaderTypesC_ColorInfoVectorPtr aColorInfoPerChannel,
                                                         bpReaderTypesC_CompressionAlgorithmType& aCompressionAlgorithmType);
BP_IMARISREADER_DLL_API void bpImageReaderC_ReadMetadataUInt32(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_Size5DVectorPtr aImageSizePerResolution,
                                                         bpReaderTypesC_Size5DVectorPtr aBlockSizePerResolution, bpReaderTypesC_ImageExtentPtr aImageExtent,
                                                         bpReaderTypesC_TimeInfoVectorPtr aTimeInfoPerTimePoint, bpReaderTypesC_ColorInfoVectorPtr aColorInfoPerChannel,
                                                         bpReaderTypesC_CompressionAlgorithmType& aCompressionAlgorithmType);
BP_IMARISREADER_DLL_API void bpImageReaderC_ReadMetadataFloat(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_Size5DVectorPtr aImageSizePerResolution,
                                                         bpReaderTypesC_Size5DVectorPtr aBlockSizePerResolution, bpReaderTypesC_ImageExtentPtr aImageExtent,
                                                         bpReaderTypesC_TimeInfoVectorPtr aTimeInfoPerTimePoint, bpReaderTypesC_ColorInfoVectorPtr aColorInfoPerChannel,
                                                         bpReaderTypesC_CompressionAlgorithmType& aCompressionAlgorithmType);

BP_IMARISREADER_DLL_API void bpImageReaderC_FreeMetadata(bpReaderTypesC_Size5DVectorPtr aImageSizePerResolution, bpReaderTypesC_Size5DVectorPtr aBlockSizePerResolution,
                                                         bpReaderTypesC_TimeInfoVectorPtr aTimeInfoPerTimePoint, bpReaderTypesC_ColorInfoVectorPtr aColorInfoPerChannel);

BP_IMARISREADER_DLL_API void bpImageReaderC_ReadParametersUInt8(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_ParametersPtr aParams);
BP_IMARISREADER_DLL_API void bpImageReaderC_ReadParametersUInt16(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_ParametersPtr aParams);
BP_IMARISREADER_DLL_API void bpImageReaderC_ReadParametersUInt32(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_ParametersPtr aParams);
BP_IMARISREADER_DLL_API void bpImageReaderC_ReadParametersFloat(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_ParametersPtr aParams);

BP_IMARISREADER_DLL_API void bpImageReaderC_FreeParameters(bpReaderTypesC_ParametersPtr aParams);

BP_IMARISREADER_DLL_API bpReaderTypesC_HistogramPtr bpImageReaderC_ReadHistogramUInt8(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_IndexTCRPtr aIndex);
BP_IMARISREADER_DLL_API bpReaderTypesC_HistogramPtr bpImageReaderC_ReadHistogramUInt16(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_IndexTCRPtr aIndex);
BP_IMARISREADER_DLL_API bpReaderTypesC_HistogramPtr bpImageReaderC_ReadHistogramUInt32(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_IndexTCRPtr aIndex);
BP_IMARISREADER_DLL_API bpReaderTypesC_HistogramPtr bpImageReaderC_ReadHistogramFloat(bpImageReaderCPtr aImageReaderC, bpReaderTypesC_IndexTCRPtr aIndex);

BP_IMARISREADER_DLL_API bpReaderTypesC_ThumbnailPtr bpImageReaderC_ReadThumbnailUInt8(bpImageReaderCPtr aImageReaderC);
BP_IMARISREADER_DLL_API bpReaderTypesC_ThumbnailPtr bpImageReaderC_ReadThumbnailUInt16(bpImageReaderCPtr aImageReaderC);
BP_IMARISREADER_DLL_API bpReaderTypesC_ThumbnailPtr bpImageReaderC_ReadThumbnailUInt32(bpImageReaderCPtr aImageReaderC);
BP_IMARISREADER_DLL_API bpReaderTypesC_ThumbnailPtr bpImageReaderC_ReadThumbnailFloat(bpImageReaderCPtr aImageReaderC);

#ifdef __cplusplus
}
#endif

#endif // __BP_IMAGE_READER_INTERFACE_C__
