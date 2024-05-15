# /***************************************************************************
#  *   Copyright (c) 2024-present Bitplane AG Zuerich                        *
#  *                                                                         *
#  *   Licensed under the Apache License, Version 2.0 (the "License");       *
#  *   you may not use this file except in compliance with the License.      *
#  *   You may obtain a copy of the License at                               *
#  *                                                                         *
#  *       http://www.apache.org/licenses/LICENSE-2.0                        *
#  *                                                                         *
#  *   Unless required by applicable law or agreed to in writing, software   *
#  *   distributed under the License is distributed on an "AS IS" BASIS,     *
#  *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or imp   *
#  *   See the License for the specific language governing permissions and   *
#  *   limitations under the License.                                        *
#  ***************************************************************************/
 
 
from ctypes import *
import platform
from collections import defaultdict


# --- C types --- 
class bpImageReaderCPtr(Structure):
    _fields_ = [('raw', c_void_p)]

bpReaderTypesC_String = c_char_p

class bpReaderTypesC_Options(Structure):
    _fields_ = [('mSWMR', c_bool)]
bpReaderTypesC_OptionsPtr = POINTER(bpReaderTypesC_Options)

class bpReaderTypesC_Index5D(Structure):
    _fields_ = [('mValueX', c_uint),
                ('mValueY', c_uint),
                ('mValueZ', c_uint),
                ('mValueC', c_uint),
                ('mValueT', c_uint)]
bpReaderTypesC_Index5DPtr = POINTER(bpReaderTypesC_Index5D)

bpReaderTypesC_UInt8 = c_ubyte
bpReaderTypesC_UInt16 = c_ushort
bpReaderTypesC_UInt32 = c_uint
bpReaderTypesC_Float = c_float

bpReaderTypesC_Size5D = bpReaderTypesC_Index5D
class bpReaderTypesC_Size5DVector(Structure):
    _fields_ = [('mSizePerResolution', POINTER(bpReaderTypesC_Size5D)),
                ('mSizePerResolutionSize', c_uint)]
bpReaderTypesC_Size5DVectorPtr = POINTER(bpReaderTypesC_Size5DVector)

class bpReaderTypesC_ImageExtent(Structure):
    _fields_ = [('mExtentMinX', c_float),
                ('mExtentMinY', c_float),
                ('mExtentMinZ', c_float),
                ('mExtentMaxX', c_float),
                ('mExtentMaxY', c_float),
                ('mExtentMaxZ', c_float)]
bpReaderTypesC_ImageExtentPtr = POINTER(bpReaderTypesC_ImageExtent)

class bpReaderTypesC_TimeInfo(Structure):
    _fields_ = [('mJulianDay', c_uint),
                ('mNanosecondsOfDay', c_ulonglong)]
bpReaderTypesC_TimeInfoPtr = POINTER(bpReaderTypesC_TimeInfo)
class bpReaderTypesC_TimeInfos(Structure):
    _fields_ = [('mValues', bpReaderTypesC_TimeInfoPtr),
                ('mValuesSize', c_uint)]
bpReaderTypesC_TimeInfosPtr = POINTER(bpReaderTypesC_TimeInfos)

class bpReaderTypesC_Color(Structure):
    _fields_ = [('mRed', c_float),
                ('mGreen', c_float),
                ('mBlue', c_float),
                ('mAlpha', c_float)]
bpReaderTypesC_ColorPtr = POINTER(bpReaderTypesC_Color)
class bpReaderTypesC_ColorInfo(Structure):
    _fields_ = [('mIsBaseColorMode', c_bool),
                ('mBaseColor', bpReaderTypesC_Color),
                ('mColorTable', bpReaderTypesC_ColorPtr),
                ('mColorTableSize', c_uint),
                ('mOpacity', c_float),
                ('mRangeMin', c_float),
                ('mRangeMax', c_float),
                ('mGammaCorrection', c_float)]
bpReaderTypesC_ColorInfoPtr = POINTER(bpReaderTypesC_ColorInfo)
class bpReaderTypesC_ColorInfos(Structure):
    _fields_ = [('mValues', bpReaderTypesC_ColorInfoPtr),
                ('mValuesSize', c_uint)]
bpReaderTypesC_ColorInfosPtr = POINTER(bpReaderTypesC_ColorInfos)

tCompressionAlgorithmType = c_int
tCompressionAlgorithmTypePtr = POINTER(tCompressionAlgorithmType)

class bpReaderTypesC_Parameter(Structure):
    _fields_ = [('mName', bpReaderTypesC_String),
                ('mValue', bpReaderTypesC_String)]
bpReaderTypesC_ParameterPtr = POINTER(bpReaderTypesC_Parameter)
class bpReaderTypesC_ParameterSection(Structure):
    _fields_ = [('mName', bpReaderTypesC_String),
                ('mValues', bpReaderTypesC_ParameterPtr),
                ('mValuesCount', c_uint)]
bpReaderTypesC_ParameterSectionPtr = POINTER(bpReaderTypesC_ParameterSection)
class bpReaderTypesC_Parameters(Structure):
    _fields_ = [('mValues', bpReaderTypesC_ParameterSectionPtr),
                ('mValuesCount', c_uint)]
bpReaderTypesC_ParametersPtr = POINTER(bpReaderTypesC_Parameters)

class bpReaderTypesC_IndexTCR(Structure):
    _fields_ = [('mT', c_uint),
                ('mC', c_uint),
                ('mR', c_uint)]
bpReaderTypesC_IndexTCRPtr = POINTER(bpReaderTypesC_IndexTCR)

class bpReaderTypesC_Histogram(Structure):
    _fields_ = [('mMin', c_uint),
                ('mMax', c_uint),
                ('mBins', POINTER(c_ulonglong)),
                ('mBinsSize', c_uint)]
bpReaderTypesC_HistogramPtr = POINTER(bpReaderTypesC_Histogram)

class bpReaderTypesC_Thumbnail(Structure):
    _fields_ = [('mSizeX', c_uint),
                ('mSizeY', c_uint),
                ('mInterleavedRGBA', POINTER(c_ubyte)),
                ('mInterleavedRGBASize', c_uint)]
bpReaderTypesC_ThumbnailPtr = POINTER(bpReaderTypesC_Thumbnail)


# --- Python types ---
class Options:
    def __init__(self):
        self.mSWMR = False

class Index5D:
    def __init__(self, X, Y, Z, C, T):
        self.mX : int = X
        self.mY : int = Y
        self.mZ : int = Z
        self.mC : int = C
        self.mT : int = T

    def get_c_index5D(self):
        c_index5D = bpReaderTypesC_Index5D(self.mX, self.mY, self.mZ, self.mC, self.mT)
        return bpReaderTypesC_Index5DPtr(c_index5D)

class IndexTCR:
    def __init__(self, T, C, R):
        self.mT : int = T
        self.mC : int = C
        self.mR : int = R

    def get_c_indexTCR(self):
        c_indexTCR = bpReaderTypesC_IndexTCR(self.mT, self.mC, self.mR)
        return bpReaderTypesC_IndexTCRPtr(c_indexTCR)

class Size5D:
    def __init__(self, X, Y, Z, C, T):
        self.mSizeX : int = X
        self.mSizeY : int = Y
        self.mSizeZ : int = Z
        self.mSizeC : int = C
        self.mSizeT : int = T

class ImageExtents:
    def __init__(self, minX, minY, minZ, maxX, maxY, maxZ):
        self.mMinX : float = minX
        self.mMinY : float = minY
        self.mMinZ : float = minZ
        self.mMaxX : float = maxX
        self.mMaxY : float = maxY
        self.mMaxZ : float = maxZ

class TimeInfo:
    def __init__(self, julian_day, ns_of_day):
        self.mJulianDay = julian_day
        self.mNanosecondsOfDay = ns_of_day

class Color:
    def __init__(self, red, green, blue, alpha):
        self.mRed = red
        self.mGreen = green
        self.mBlue = blue
        self.mAlpha = alpha

class ColorInfo:
    def __init__(self, is_base_color_mode, opacity, range_min, range_max, gamma):
        self.mIsBaseColorMode = is_base_color_mode
        self.mOpacity = opacity
        self.mRangeMin = range_min
        self.mRangeMax = range_max
        self.mGammaCorrection = gamma
        self.mBaseColor : Color
        self.mColorTable : List[Color] = []
        self.mColorTableSize : int

class Parameters:
    def __init__(self):
        self.mSections : Dict[str, Dict[str, str]] = defaultdict(dict)
        
    def set_value(self, section, parameter_name, value):
        self.mSections[section][parameter_name] = value


# --- Reader classes (UInt8, UInt16, UInt32, Float) ---
class ImageReaderUInt8:
    def __init__(self,
                 input_filename : str,
                 image_index : int,
                 options : Options):
        self._load_dll()
        self._store_filename(input_filename)
        self._store_image_index(image_index)
        self._store_options(options)
        self._create()

    def _store_filename(self, input_filename):
        self.mInputFilename = self._get_c_char(input_filename)
    
    def _get_c_char(self, string):
        encoded = c_char_p(str(string).encode())
        return encoded

    def _store_image_index(self, image_index):
        self.mImageIndex = image_index

    def _store_options(self, options):
        self.mOptions = bpReaderTypesC_OptionsPtr(bpReaderTypesC_Options(options.mSWMR))

    def _get_lib_filename(self):
        if platform.system() == 'Windows':
            return 'bpImarisReader.dll'
        elif platform.system() == 'Darwin':
            return 'libbpImarisReader.dylib'
        elif platform.system() == 'Linux':
            return 'libbpImarisReader.so'
        else:
            print('Platform not supported: "{}"'.format(platform.system()))
            return None

    def _load_dll(self):
        lib_filename = self._get_lib_filename()
        self.mcdll = CDLL(lib_filename)

    def _create(self):
        self.mcdll.bpImageReaderC_CreateUInt8.argtypes = [bpReaderTypesC_String, c_uint, bpReaderTypesC_OptionsPtr]
        self.mcdll.bpImageReaderC_CreateUInt8.restype = bpImageReaderCPtr
        self.mImageReaderPtr = self.mcdll.bpImageReaderC_CreateUInt8(self.mInputFilename,
                                                                    self.mImageIndex,
                                                                    self.mOptions)

    def ReadData(self, begin : Index5D, end : Index5D, resolution_index : int, buffer):
        self.mcdll.bpImageReaderC_ReadDataUInt8.argtypes = [bpImageReaderCPtr, bpReaderTypesC_Index5DPtr, bpReaderTypesC_Index5DPtr, c_uint, POINTER(bpReaderTypesC_UInt8)]
        self.mcdll.bpImageReaderC_ReadDataUInt8.restype = None
        self.mcdll.bpImageReaderC_ReadDataUInt8(self.mImageReaderPtr, begin.get_c_index5D(), end.get_c_index5D(), resolution_index, buffer)

    def ReadMetadata(self):
        imageSizePerResolution = bpReaderTypesC_Size5DVectorPtr(bpReaderTypesC_Size5DVector())
        blockSizePerResolution = bpReaderTypesC_Size5DVectorPtr(bpReaderTypesC_Size5DVector())
        imageExtents = bpReaderTypesC_ImageExtentPtr(bpReaderTypesC_ImageExtent())
        timeInfos = bpReaderTypesC_TimeInfosPtr(bpReaderTypesC_TimeInfos())
        colorInfos = bpReaderTypesC_ColorInfosPtr(bpReaderTypesC_ColorInfos())
        compression = tCompressionAlgorithmTypePtr(tCompressionAlgorithmType(0))
        self.mcdll.bpImageReaderC_ReadMetadataUInt8.argtypes = [bpImageReaderCPtr, 
                                                                bpReaderTypesC_Size5DVectorPtr, bpReaderTypesC_Size5DVectorPtr, bpReaderTypesC_ImageExtentPtr, 
                                                                bpReaderTypesC_TimeInfosPtr, bpReaderTypesC_ColorInfosPtr, tCompressionAlgorithmTypePtr]
        self.mcdll.bpImageReaderC_ReadMetadataUInt8.restype = None
        self.mcdll.bpImageReaderC_ReadMetadataUInt8(self.mImageReaderPtr, imageSizePerResolution, blockSizePerResolution, imageExtents,
                                                    timeInfos, colorInfos, compression)
        imageSizePerResolution_py = []
        blockSizePerResolution_py = []
        for res in range(imageSizePerResolution.contents.mSizePerResolutionSize):
            image_size = Size5D(imageSizePerResolution.contents.mSizePerResolution[res].mValueX,
                                imageSizePerResolution.contents.mSizePerResolution[res].mValueY,
                                imageSizePerResolution.contents.mSizePerResolution[res].mValueZ,
                                imageSizePerResolution.contents.mSizePerResolution[res].mValueC,
                                imageSizePerResolution.contents.mSizePerResolution[res].mValueT)
            block_size = Size5D(blockSizePerResolution.contents.mSizePerResolution[res].mValueX,
                                blockSizePerResolution.contents.mSizePerResolution[res].mValueY,
                                blockSizePerResolution.contents.mSizePerResolution[res].mValueZ,
                                blockSizePerResolution.contents.mSizePerResolution[res].mValueC,
                                blockSizePerResolution.contents.mSizePerResolution[res].mValueT)
            imageSizePerResolution_py.append(image_size)
            blockSizePerResolution_py.append(block_size)
        imageExtents_py = ImageExtents(imageExtents.contents.mExtentMinX, imageExtents.contents.mExtentMinY, imageExtents.contents.mExtentMinZ,
                                        imageExtents.contents.mExtentMaxX, imageExtents.contents.mExtentMaxY, imageExtents.contents.mExtentMaxZ)
        timeInfos_py = []
        for t in range(timeInfos.contents.mValuesSize):
            timepoint_info = TimeInfo(timeInfos.contents.mValues[t].mJulianDay, timeInfos.contents.mValues[t].mNanosecondsOfDay)
            timeInfos_py.append(timepoint_info)
        colorInfos_py = []
        for ch in range(colorInfos.contents.mValuesSize):
            channel_info = ColorInfo(colorInfos.contents.mValues[ch].mIsBaseColorMode,
                                    colorInfos.contents.mValues[ch].mOpacity,
                                    colorInfos.contents.mValues[ch].mRangeMin,
                                    colorInfos.contents.mValues[ch].mRangeMax,
                                    colorInfos.contents.mValues[ch].mGammaCorrection)
            if (channel_info.mIsBaseColorMode):
                channel_info.mBaseColor = Color(colorInfos.contents.mValues[ch].mBaseColor.mRed,
                                                colorInfos.contents.mValues[ch].mBaseColor.mGreen,
                                                colorInfos.contents.mValues[ch].mBaseColor.mBlue,
                                                colorInfos.contents.mValues[ch].mBaseColor.mAlpha)
            else:
                channel_info.mColorTableSize = colorInfos.contents.mValues[ch].mColorTableSize
                for i in range(colorInfos.contents.mValues[ch].mColorTableSize):
                    color = Color(colorInfos.contents.mValues[ch].mColorTable[i].mRed,
                                    colorInfos.contents.mValues[ch].mColorTable[i].mGreen,
                                    colorInfos.contents.mValues[ch].mColorTable[i].mBlue,
                                    colorInfos.contents.mValues[ch].mColorTable[i].mAlpha)
                    channel_info.mColorTable.append(color)
            colorInfos_py.append(channel_info)
        compression_py = compression.contents.value
        self.FreeMetadata(imageSizePerResolution, blockSizePerResolution, timeInfos, colorInfos)
        return imageSizePerResolution_py, blockSizePerResolution_py, imageExtents_py, timeInfos_py, colorInfos_py, compression_py

    def FreeMetadata(self, imageSizePerResolution : bpReaderTypesC_Size5DVectorPtr, blockSizePerResolution : bpReaderTypesC_Size5DVectorPtr,
                    timeInfos : bpReaderTypesC_TimeInfosPtr, colorInfos : bpReaderTypesC_ColorInfosPtr):
        self.mcdll.bpImageReaderC_FreeMetadata.argtypes = [bpReaderTypesC_Size5DVectorPtr, bpReaderTypesC_Size5DVectorPtr, bpReaderTypesC_TimeInfosPtr, bpReaderTypesC_ColorInfosPtr]
        self.mcdll.bpImageReaderC_FreeMetadata.restype = None
        self.mcdll.bpImageReaderC_FreeMetadata(imageSizePerResolution, blockSizePerResolution, timeInfos, colorInfos)

    def ReadParameters(self):
        parameters = bpReaderTypesC_ParametersPtr(bpReaderTypesC_Parameters())
        self.mcdll.bpImageReaderC_ReadParametersUInt8.argtypes = [bpImageReaderCPtr, bpReaderTypesC_ParametersPtr]
        self.mcdll.bpImageReaderC_ReadParametersUInt8.restype = None
        self.mcdll.bpImageReaderC_ReadParametersUInt8(self.mImageReaderPtr, parameters)
        parameters_py = Parameters()
        for i in range(parameters.contents.mValuesCount):
            for j in range(parameters.contents.mValues[i].mValuesCount):
                parameters_py.set_value(parameters.contents.mValues[i].mName.decode("utf-8"), parameters.contents.mValues[i].mValues[j].mName.decode("utf-8"), parameters.contents.mValues[i].mValues[j].mValue.decode("utf-8"))
        self.FreeParameters(parameters)
        return parameters_py

    def FreeParameters(self, parameters : bpReaderTypesC_ParametersPtr):
        self.mcdll.bpImageReaderC_FreeParameters.argtypes = [bpReaderTypesC_ParametersPtr]
        self.mcdll.bpImageReaderC_FreeParameters.restype = None
        self.mcdll.bpImageReaderC_FreeParameters(parameters)

    def ReadHistogram(self, index : IndexTCR):
        self.mcdll.bpImageReaderC_ReadHistogramUInt8.argtypes = [bpImageReaderCPtr, bpReaderTypesC_IndexTCRPtr]
        self.mcdll.bpImageReaderC_ReadHistogramUInt8.restype = bpReaderTypesC_HistogramPtr
        return self.mcdll.bpImageReaderC_ReadHistogramUInt8(self.mImageReaderPtr, index.get_c_indexTCR())
    
    def ReadThumbnail(self):
        self.mcdll.bpImageReaderC_ReadThumbnailUInt8.argtypes = [bpImageReaderCPtr]
        self.mcdll.bpImageReaderC_ReadThumbnailUInt8.restype = bpReaderTypesC_ThumbnailPtr
        return self.mcdll.bpImageReaderC_ReadThumbnailUInt8(self.mImageReaderPtr)

    def Destroy(self):
        self.mcdll.bpImageReaderC_DestroyUInt8.argtypes = [bpImageReaderCPtr]
        self.mcdll.bpImageReaderC_DestroyUInt8.restype = None
        self.mcdll.bpImageReaderC_DestroyUInt8(self.mImageReaderPtr)

class ImageReaderUInt16:
    def __init__(self,
                 input_filename : str,
                 image_index : int,
                 options : Options):
        self._load_dll()
        self._store_filename(input_filename)
        self._store_image_index(image_index)
        self._store_options(options)
        self._create()

    def _store_filename(self, input_filename):
        self.mInputFilename = self._get_c_char(input_filename)
    
    def _get_c_char(self, string):
        encoded = c_char_p(str(string).encode())
        return encoded

    def _store_image_index(self, image_index):
        self.mImageIndex = image_index

    def _store_options(self, options):
        self.mOptions = bpReaderTypesC_OptionsPtr(bpReaderTypesC_Options(options.mSWMR))

    def _get_lib_filename(self):
        if platform.system() == 'Windows':
            return 'bpImarisReader.dll'
        elif platform.system() == 'Darwin':
            return 'libbpImarisReader.dylib'
        elif platform.system() == 'Linux':
            return 'libbpImarisReader.so'
        else:
            print('Platform not supported: "{}"'.format(platform.system()))
            return None

    def _load_dll(self):
        lib_filename = self._get_lib_filename()
        self.mcdll = CDLL(lib_filename)

    def _create(self):
        self.mcdll.bpImageReaderC_CreateUInt16.argtypes = [bpReaderTypesC_String, c_uint, bpReaderTypesC_OptionsPtr]
        self.mcdll.bpImageReaderC_CreateUInt16.restype = bpImageReaderCPtr
        self.mImageReaderPtr = self.mcdll.bpImageReaderC_CreateUInt16(self.mInputFilename,
                                                                    self.mImageIndex,
                                                                    self.mOptions)

    def ReadData(self, begin : Index5D, end : Index5D, resolution_index : int):
        buffer_size = (end.X - begin.X) * (end.Y - begin.Y) * (end.Z - begin.Z) * (end.C - begin.C) * (end.T - begin.T)
        buffer = (bpReaderTypesC_UInt16 * buffer_size)()
        self.mcdll.bpImageReaderC_ReadDataUInt16.argtypes = [bpImageReaderCPtr, bpReaderTypesC_Index5DPtr, bpReaderTypesC_Index5DPtr, c_uint, POINTER(bpReaderTypesC_UInt16)]
        self.mcdll.bpImageReaderC_ReadDataUInt16.restype = None
        self.mcdll.bpImageReaderC_ReadDataUInt16(self.mImageReaderPtr, begin.get_c_index5D(), end.get_c_index5D(), resolution_index, buffer)
        return buffer

    def ReadMetadata(self):
        imageSizePerResolution = bpReaderTypesC_Size5DVectorPtr(bpReaderTypesC_Size5DVector())
        blockSizePerResolution = bpReaderTypesC_Size5DVectorPtr(bpReaderTypesC_Size5DVector())
        imageExtents = bpReaderTypesC_ImageExtentPtr(bpReaderTypesC_ImageExtent())
        timeInfos = bpReaderTypesC_TimeInfosPtr(bpReaderTypesC_TimeInfos())
        colorInfos = bpReaderTypesC_ColorInfosPtr(bpReaderTypesC_ColorInfos())
        compression = tCompressionAlgorithmTypePtr(tCompressionAlgorithmType(0))
        self.mcdll.bpImageReaderC_ReadMetadataUInt16.argtypes = [bpImageReaderCPtr, 
                                                                bpReaderTypesC_Size5DVectorPtr, bpReaderTypesC_Size5DVectorPtr, bpReaderTypesC_ImageExtentPtr, 
                                                                bpReaderTypesC_TimeInfosPtr, bpReaderTypesC_ColorInfosPtr, tCompressionAlgorithmTypePtr]
        self.mcdll.bpImageReaderC_ReadMetadataUInt16.restype = None
        self.mcdll.bpImageReaderC_ReadMetadataUInt16(self.mImageReaderPtr, imageSizePerResolution, blockSizePerResolution, imageExtents,
                                                    timeInfos, colorInfos, compression)
        imageSizePerResolution_py = []
        blockSizePerResolution_py = []
        for res in range(imageSizePerResolution.contents.mSizePerResolutionSize):
            image_size = Size5D(imageSizePerResolution.contents.mSizePerResolution[res].mValueX,
                                imageSizePerResolution.contents.mSizePerResolution[res].mValueY,
                                imageSizePerResolution.contents.mSizePerResolution[res].mValueZ,
                                imageSizePerResolution.contents.mSizePerResolution[res].mValueC,
                                imageSizePerResolution.contents.mSizePerResolution[res].mValueT)
            block_size = Size5D(blockSizePerResolution.contents.mSizePerResolution[res].mValueX,
                                blockSizePerResolution.contents.mSizePerResolution[res].mValueY,
                                blockSizePerResolution.contents.mSizePerResolution[res].mValueZ,
                                blockSizePerResolution.contents.mSizePerResolution[res].mValueC,
                                blockSizePerResolution.contents.mSizePerResolution[res].mValueT)
            imageSizePerResolution_py.append(image_size)
            blockSizePerResolution_py.append(block_size)
        imageExtents_py = ImageExtents(imageExtents.contents.mExtentMinX, imageExtents.contents.mExtentMinY, imageExtents.contents.mExtentMinZ,
                                        imageExtents.contents.mExtentMaxX, imageExtents.contents.mExtentMaxY, imageExtents.contents.mExtentMaxZ)
        timeInfos_py = []
        for t in range(timeInfos.contents.mValuesSize):
            timepoint_info = TimeInfo(timeInfos.contents.mValues[t].mJulianDay, timeInfos.contents.mValues[t].mNanosecondsOfDay)
            timeInfos_py.append(timepoint_info)
        colorInfos_py = []
        for ch in range(colorInfos.contents.mValuesSize):
            channel_info = ColorInfo(colorInfos.contents.mValues[ch].mIsBaseColorMode,
                                    colorInfos.contents.mValues[ch].mOpacity,
                                    colorInfos.contents.mValues[ch].mRangeMin,
                                    colorInfos.contents.mValues[ch].mRangeMax,
                                    colorInfos.contents.mValues[ch].mGammaCorrection)
            if (channel_info.mIsBaseColorMode):
                channel_info.mBaseColor = Color(colorInfos.contents.mValues[ch].mBaseColor.mRed,
                                                colorInfos.contents.mValues[ch].mBaseColor.mGreen,
                                                colorInfos.contents.mValues[ch].mBaseColor.mBlue,
                                                colorInfos.contents.mValues[ch].mBaseColor.mAlpha)
            else:
                channel_info.mColorTableSize = colorInfos.contents.mValues[ch].mColorTableSize
                for i in range(colorInfos.contents.mValues[ch].mColorTableSize):
                    color = Color(colorInfos.contents.mValues[ch].mColorTable[i].mRed,
                                    colorInfos.contents.mValues[ch].mColorTable[i].mGreen,
                                    colorInfos.contents.mValues[ch].mColorTable[i].mBlue,
                                    colorInfos.contents.mValues[ch].mColorTable[i].mAlpha)
                    channel_info.mColorTable.append(color)
            colorInfos_py.append(channel_info)
        compression_py = compression.contents.value
        self.FreeMetadata(imageSizePerResolution, blockSizePerResolution, timeInfos, colorInfos)
        return imageSizePerResolution_py, blockSizePerResolution_py, imageExtents_py, timeInfos_py, colorInfos_py, compression_py

    def FreeMetadata(self, imageSizePerResolution : bpReaderTypesC_Size5DVectorPtr, blockSizePerResolution : bpReaderTypesC_Size5DVectorPtr,
                    timeInfos : bpReaderTypesC_TimeInfosPtr, colorInfos : bpReaderTypesC_ColorInfosPtr):
        self.mcdll.bpImageReaderC_FreeMetadata.argtypes = [bpReaderTypesC_Size5DVectorPtr, bpReaderTypesC_Size5DVectorPtr, bpReaderTypesC_TimeInfosPtr, bpReaderTypesC_ColorInfosPtr]
        self.mcdll.bpImageReaderC_FreeMetadata.restype = None
        self.mcdll.bpImageReaderC_FreeMetadata(imageSizePerResolution, blockSizePerResolution, timeInfos, colorInfos)

    def ReadParameters(self):
        parameters = bpReaderTypesC_ParametersPtr(bpReaderTypesC_Parameters())
        self.mcdll.bpImageReaderC_ReadParametersUInt16.argtypes = [bpImageReaderCPtr, bpReaderTypesC_ParametersPtr]
        self.mcdll.bpImageReaderC_ReadParametersUInt16.restype = None
        self.mcdll.bpImageReaderC_ReadParametersUInt16(self.mImageReaderPtr, parameters)
        parameters_py = Parameters()
        for i in range(parameters.contents.mValuesCount):
            for j in range(parameters.contents.mValues[i].mValuesCount):
                parameters_py.set_value(parameters.contents.mValues[i].mName.decode("utf-8"), parameters.contents.mValues[i].mValues[j].mName.decode("utf-8"), parameters.contents.mValues[i].mValues[j].mValue.decode("utf-8"))
        self.FreeParameters(parameters)
        return parameters_py

    def FreeParameters(self, parameters : bpReaderTypesC_ParametersPtr):
        self.mcdll.bpImageReaderC_FreeParameters.argtypes = [bpReaderTypesC_ParametersPtr]
        self.mcdll.bpImageReaderC_FreeParameters.restype = None
        self.mcdll.bpImageReaderC_FreeParameters(parameters)

    def ReadHistogram(self, index : IndexTCR):
        self.mcdll.bpImageReaderC_ReadHistogramUInt16.argtypes = [bpImageReaderCPtr, bpReaderTypesC_IndexTCRPtr]
        self.mcdll.bpImageReaderC_ReadHistogramUInt16.restype = bpReaderTypesC_HistogramPtr
        return self.mcdll.bpImageReaderC_ReadHistogramUInt16(self.mImageReaderPtr, index.get_c_indexTCR())
    
    def ReadThumbnail(self):
        self.mcdll.bpImageReaderC_ReadThumbnailUInt16.argtypes = [bpImageReaderCPtr]
        self.mcdll.bpImageReaderC_ReadThumbnailUInt16.restype = bpReaderTypesC_ThumbnailPtr
        return self.mcdll.bpImageReaderC_ReadThumbnailUInt16(self.mImageReaderPtr)

    def Destroy(self):
        self.mcdll.bpImageReaderC_DestroyUInt16.argtypes = [bpImageReaderCPtr]
        self.mcdll.bpImageReaderC_DestroyUInt16.restype = None
        self.mcdll.bpImageReaderC_DestroyUInt16(self.mImageReaderPtr)

class ImageReaderUInt32:
    def __init__(self,
                 input_filename : str,
                 image_index : int,
                 options : Options):
        self._load_dll()
        self._store_filename(input_filename)
        self._store_image_index(image_index)
        self._store_options(options)
        self._create()

    def _store_filename(self, input_filename):
        self.mInputFilename = self._get_c_char(input_filename)
    
    def _get_c_char(self, string):
        encoded = c_char_p(str(string).encode())
        return encoded

    def _store_image_index(self, image_index):
        self.mImageIndex = image_index

    def _store_options(self, options):
        self.mOptions = bpReaderTypesC_OptionsPtr(bpReaderTypesC_Options(options.mSWMR))

    def _get_lib_filename(self):
        if platform.system() == 'Windows':
            return 'bpImarisReader.dll'
        elif platform.system() == 'Darwin':
            return 'libbpImarisReader.dylib'
        elif platform.system() == 'Linux':
            return 'libbpImarisReader.so'
        else:
            print('Platform not supported: "{}"'.format(platform.system()))
            return None

    def _load_dll(self):
        lib_filename = self._get_lib_filename()
        self.mcdll = CDLL(lib_filename)

    def _create(self):
        self.mcdll.bpImageReaderC_CreateUInt32.argtypes = [bpReaderTypesC_String, c_uint, bpReaderTypesC_OptionsPtr]
        self.mcdll.bpImageReaderC_CreateUInt32.restype = bpImageReaderCPtr
        self.mImageReaderPtr = self.mcdll.bpImageReaderC_CreateUInt32(self.mInputFilename,
                                                                    self.mImageIndex,
                                                                    self.mOptions)

    def ReadData(self, begin : Index5D, end : Index5D, resolution_index : int):
        buffer_size = (end.X - begin.X) * (end.Y - begin.Y) * (end.Z - begin.Z) * (end.C - begin.C) * (end.T - begin.T)
        buffer = (bpReaderTypesC_UInt32 * buffer_size)()
        self.mcdll.bpImageReaderC_ReadDataUInt32.argtypes = [bpImageReaderCPtr, bpReaderTypesC_Index5DPtr, bpReaderTypesC_Index5DPtr, c_uint, POINTER(bpReaderTypesC_UInt32)]
        self.mcdll.bpImageReaderC_ReadDataUInt32.restype = None
        self.mcdll.bpImageReaderC_ReadDataUInt32(self.mImageReaderPtr, begin.get_c_index5D(), end.get_c_index5D(), resolution_index, buffer)
        return buffer

    def ReadMetadata(self):
        imageSizePerResolution = bpReaderTypesC_Size5DVectorPtr(bpReaderTypesC_Size5DVector())
        blockSizePerResolution = bpReaderTypesC_Size5DVectorPtr(bpReaderTypesC_Size5DVector())
        imageExtents = bpReaderTypesC_ImageExtentPtr(bpReaderTypesC_ImageExtent())
        timeInfos = bpReaderTypesC_TimeInfosPtr(bpReaderTypesC_TimeInfos())
        colorInfos = bpReaderTypesC_ColorInfosPtr(bpReaderTypesC_ColorInfos())
        compression = tCompressionAlgorithmTypePtr(tCompressionAlgorithmType(0))
        self.mcdll.bpImageReaderC_ReadMetadataUInt32.argtypes = [bpImageReaderCPtr, 
                                                                bpReaderTypesC_Size5DVectorPtr, bpReaderTypesC_Size5DVectorPtr, bpReaderTypesC_ImageExtentPtr, 
                                                                bpReaderTypesC_TimeInfosPtr, bpReaderTypesC_ColorInfosPtr, tCompressionAlgorithmTypePtr]
        self.mcdll.bpImageReaderC_ReadMetadataUInt32.restype = None
        self.mcdll.bpImageReaderC_ReadMetadataUInt32(self.mImageReaderPtr, imageSizePerResolution, blockSizePerResolution, imageExtents,
                                                    timeInfos, colorInfos, compression)
        imageSizePerResolution_py = []
        blockSizePerResolution_py = []
        for res in range(imageSizePerResolution.contents.mSizePerResolutionSize):
            image_size = Size5D(imageSizePerResolution.contents.mSizePerResolution[res].mValueX,
                                imageSizePerResolution.contents.mSizePerResolution[res].mValueY,
                                imageSizePerResolution.contents.mSizePerResolution[res].mValueZ,
                                imageSizePerResolution.contents.mSizePerResolution[res].mValueC,
                                imageSizePerResolution.contents.mSizePerResolution[res].mValueT)
            block_size = Size5D(blockSizePerResolution.contents.mSizePerResolution[res].mValueX,
                                blockSizePerResolution.contents.mSizePerResolution[res].mValueY,
                                blockSizePerResolution.contents.mSizePerResolution[res].mValueZ,
                                blockSizePerResolution.contents.mSizePerResolution[res].mValueC,
                                blockSizePerResolution.contents.mSizePerResolution[res].mValueT)
            imageSizePerResolution_py.append(image_size)
            blockSizePerResolution_py.append(block_size)
        imageExtents_py = ImageExtents(imageExtents.contents.mExtentMinX, imageExtents.contents.mExtentMinY, imageExtents.contents.mExtentMinZ,
                                        imageExtents.contents.mExtentMaxX, imageExtents.contents.mExtentMaxY, imageExtents.contents.mExtentMaxZ)
        timeInfos_py = []
        for t in range(timeInfos.contents.mValuesSize):
            timepoint_info = TimeInfo(timeInfos.contents.mValues[t].mJulianDay, timeInfos.contents.mValues[t].mNanosecondsOfDay)
            timeInfos_py.append(timepoint_info)
        colorInfos_py = []
        for ch in range(colorInfos.contents.mValuesSize):
            channel_info = ColorInfo(colorInfos.contents.mValues[ch].mIsBaseColorMode,
                                    colorInfos.contents.mValues[ch].mOpacity,
                                    colorInfos.contents.mValues[ch].mRangeMin,
                                    colorInfos.contents.mValues[ch].mRangeMax,
                                    colorInfos.contents.mValues[ch].mGammaCorrection)
            if (channel_info.mIsBaseColorMode):
                channel_info.mBaseColor = Color(colorInfos.contents.mValues[ch].mBaseColor.mRed,
                                                colorInfos.contents.mValues[ch].mBaseColor.mGreen,
                                                colorInfos.contents.mValues[ch].mBaseColor.mBlue,
                                                colorInfos.contents.mValues[ch].mBaseColor.mAlpha)
            else:
                channel_info.mColorTableSize = colorInfos.contents.mValues[ch].mColorTableSize
                for i in range(colorInfos.contents.mValues[ch].mColorTableSize):
                    color = Color(colorInfos.contents.mValues[ch].mColorTable[i].mRed,
                                    colorInfos.contents.mValues[ch].mColorTable[i].mGreen,
                                    colorInfos.contents.mValues[ch].mColorTable[i].mBlue,
                                    colorInfos.contents.mValues[ch].mColorTable[i].mAlpha)
                    channel_info.mColorTable.append(color)
            colorInfos_py.append(channel_info)
        compression_py = compression.contents.value
        self.FreeMetadata(imageSizePerResolution, blockSizePerResolution, timeInfos, colorInfos)
        return imageSizePerResolution_py, blockSizePerResolution_py, imageExtents_py, timeInfos_py, colorInfos_py, compression_py

    def FreeMetadata(self, imageSizePerResolution : bpReaderTypesC_Size5DVectorPtr, blockSizePerResolution : bpReaderTypesC_Size5DVectorPtr,
                    timeInfos : bpReaderTypesC_TimeInfosPtr, colorInfos : bpReaderTypesC_ColorInfosPtr):
        self.mcdll.bpImageReaderC_FreeMetadata.argtypes = [bpReaderTypesC_Size5DVectorPtr, bpReaderTypesC_Size5DVectorPtr, bpReaderTypesC_TimeInfosPtr, bpReaderTypesC_ColorInfosPtr]
        self.mcdll.bpImageReaderC_FreeMetadata.restype = None
        self.mcdll.bpImageReaderC_FreeMetadata(imageSizePerResolution, blockSizePerResolution, timeInfos, colorInfos)

    def ReadParameters(self):
        parameters = bpReaderTypesC_ParametersPtr(bpReaderTypesC_Parameters())
        self.mcdll.bpImageReaderC_ReadParametersUInt32.argtypes = [bpImageReaderCPtr, bpReaderTypesC_ParametersPtr]
        self.mcdll.bpImageReaderC_ReadParametersUInt32.restype = None
        self.mcdll.bpImageReaderC_ReadParametersUInt32(self.mImageReaderPtr, parameters)
        parameters_py = Parameters()
        for i in range(parameters.contents.mValuesCount):
            for j in range(parameters.contents.mValues[i].mValuesCount):
                parameters_py.set_value(parameters.contents.mValues[i].mName.decode("utf-8"), parameters.contents.mValues[i].mValues[j].mName.decode("utf-8"), parameters.contents.mValues[i].mValues[j].mValue.decode("utf-8"))
        self.FreeParameters(parameters)
        return parameters_py

    def FreeParameters(self, parameters : bpReaderTypesC_ParametersPtr):
        self.mcdll.bpImageReaderC_FreeParameters.argtypes = [bpReaderTypesC_ParametersPtr]
        self.mcdll.bpImageReaderC_FreeParameters.restype = None
        self.mcdll.bpImageReaderC_FreeParameters(parameters)

    def ReadHistogram(self, index : IndexTCR):
        self.mcdll.bpImageReaderC_ReadHistogramUInt32.argtypes = [bpImageReaderCPtr, bpReaderTypesC_IndexTCRPtr]
        self.mcdll.bpImageReaderC_ReadHistogramUInt32.restype = bpReaderTypesC_HistogramPtr
        return self.mcdll.bpImageReaderC_ReadHistogramUInt32(self.mImageReaderPtr, index.get_c_indexTCR())
    
    def ReadThumbnail(self):
        self.mcdll.bpImageReaderC_ReadThumbnailUInt32.argtypes = [bpImageReaderCPtr]
        self.mcdll.bpImageReaderC_ReadThumbnailUInt32.restype = bpReaderTypesC_ThumbnailPtr
        return self.mcdll.bpImageReaderC_ReadThumbnailUInt32(self.mImageReaderPtr)

    def Destroy(self):
        self.mcdll.bpImageReaderC_DestroyUInt32.argtypes = [bpImageReaderCPtr]
        self.mcdll.bpImageReaderC_DestroyUInt32.restype = None
        self.mcdll.bpImageReaderC_DestroyUInt32(self.mImageReaderPtr)

class ImageReaderFloat:
    def __init__(self,
                 input_filename : str,
                 image_index : int,
                 options : Options):
        self._load_dll()
        self._store_filename(input_filename)
        self._store_image_index(image_index)
        self._store_options(options)
        self._create()

    def _store_filename(self, input_filename):
        self.mInputFilename = self._get_c_char(input_filename)
    
    def _get_c_char(self, string):
        encoded = c_char_p(str(string).encode())
        return encoded

    def _store_image_index(self, image_index):
        self.mImageIndex = image_index

    def _store_options(self, options):
        self.mOptions = bpReaderTypesC_OptionsPtr(bpReaderTypesC_Options(options.mSWMR))

    def _get_lib_filename(self):
        if platform.system() == 'Windows':
            return 'bpImarisReader.dll'
        elif platform.system() == 'Darwin':
            return 'libbpImarisReader.dylib'
        elif platform.system() == 'Linux':
            return 'libbpImarisReader.so'
        else:
            print('Platform not supported: "{}"'.format(platform.system()))
            return None

    def _load_dll(self):
        lib_filename = self._get_lib_filename()
        self.mcdll = CDLL(lib_filename)

    def _create(self):
        self.mcdll.bpImageReaderC_CreateFloat.argtypes = [bpReaderTypesC_String, c_uint, bpReaderTypesC_OptionsPtr]
        self.mcdll.bpImageReaderC_CreateFloat.restype = bpImageReaderCPtr
        self.mImageReaderPtr = self.mcdll.bpImageReaderC_CreateFloat(self.mInputFilename,
                                                                    self.mImageIndex,
                                                                    self.mOptions)

    def ReadData(self, begin : Index5D, end : Index5D, resolution_index : int):
        buffer_size = (end.X - begin.X) * (end.Y - begin.Y) * (end.Z - begin.Z) * (end.C - begin.C) * (end.T - begin.T)
        buffer = (bpReaderTypesC_Float * buffer_size)()
        self.mcdll.bpImageReaderC_ReadDataFloat.argtypes = [bpImageReaderCPtr, bpReaderTypesC_Index5DPtr, bpReaderTypesC_Index5DPtr, c_uint, POINTER(bpReaderTypesC_Float)]
        self.mcdll.bpImageReaderC_ReadDataFloat.restype = None
        self.mcdll.bpImageReaderC_ReadDataFloat(self.mImageReaderPtr, begin.get_c_index5D(), end.get_c_index5D(), resolution_index, buffer)
        return buffer

    def ReadMetadata(self):
        imageSizePerResolution = bpReaderTypesC_Size5DVectorPtr(bpReaderTypesC_Size5DVector())
        blockSizePerResolution = bpReaderTypesC_Size5DVectorPtr(bpReaderTypesC_Size5DVector())
        imageExtents = bpReaderTypesC_ImageExtentPtr(bpReaderTypesC_ImageExtent())
        timeInfos = bpReaderTypesC_TimeInfosPtr(bpReaderTypesC_TimeInfos())
        colorInfos = bpReaderTypesC_ColorInfosPtr(bpReaderTypesC_ColorInfos())
        compression = tCompressionAlgorithmTypePtr(tCompressionAlgorithmType(0))
        self.mcdll.bpImageReaderC_ReadMetadataFloat.argtypes = [bpImageReaderCPtr, 
                                                                bpReaderTypesC_Size5DVectorPtr, bpReaderTypesC_Size5DVectorPtr, bpReaderTypesC_ImageExtentPtr, 
                                                                bpReaderTypesC_TimeInfosPtr, bpReaderTypesC_ColorInfosPtr, tCompressionAlgorithmTypePtr]
        self.mcdll.bpImageReaderC_ReadMetadataFloat.restype = None
        self.mcdll.bpImageReaderC_ReadMetadataFloat(self.mImageReaderPtr, imageSizePerResolution, blockSizePerResolution, imageExtents,
                                                    timeInfos, colorInfos, compression)
        imageSizePerResolution_py = []
        blockSizePerResolution_py = []
        for res in range(imageSizePerResolution.contents.mSizePerResolutionSize):
            image_size = Size5D(imageSizePerResolution.contents.mSizePerResolution[res].mValueX,
                                imageSizePerResolution.contents.mSizePerResolution[res].mValueY,
                                imageSizePerResolution.contents.mSizePerResolution[res].mValueZ,
                                imageSizePerResolution.contents.mSizePerResolution[res].mValueC,
                                imageSizePerResolution.contents.mSizePerResolution[res].mValueT)
            block_size = Size5D(blockSizePerResolution.contents.mSizePerResolution[res].mValueX,
                                blockSizePerResolution.contents.mSizePerResolution[res].mValueY,
                                blockSizePerResolution.contents.mSizePerResolution[res].mValueZ,
                                blockSizePerResolution.contents.mSizePerResolution[res].mValueC,
                                blockSizePerResolution.contents.mSizePerResolution[res].mValueT)
            imageSizePerResolution_py.append(image_size)
            blockSizePerResolution_py.append(block_size)
        imageExtents_py = ImageExtents(imageExtents.contents.mExtentMinX, imageExtents.contents.mExtentMinY, imageExtents.contents.mExtentMinZ,
                                        imageExtents.contents.mExtentMaxX, imageExtents.contents.mExtentMaxY, imageExtents.contents.mExtentMaxZ)
        timeInfos_py = []
        for t in range(timeInfos.contents.mValuesSize):
            timepoint_info = TimeInfo(timeInfos.contents.mValues[t].mJulianDay, timeInfos.contents.mValues[t].mNanosecondsOfDay)
            timeInfos_py.append(timepoint_info)
        colorInfos_py = []
        for ch in range(colorInfos.contents.mValuesSize):
            channel_info = ColorInfo(colorInfos.contents.mValues[ch].mIsBaseColorMode,
                                    colorInfos.contents.mValues[ch].mOpacity,
                                    colorInfos.contents.mValues[ch].mRangeMin,
                                    colorInfos.contents.mValues[ch].mRangeMax,
                                    colorInfos.contents.mValues[ch].mGammaCorrection)
            if (channel_info.mIsBaseColorMode):
                channel_info.mBaseColor = Color(colorInfos.contents.mValues[ch].mBaseColor.mRed,
                                                colorInfos.contents.mValues[ch].mBaseColor.mGreen,
                                                colorInfos.contents.mValues[ch].mBaseColor.mBlue,
                                                colorInfos.contents.mValues[ch].mBaseColor.mAlpha)
            else:
                channel_info.mColorTableSize = colorInfos.contents.mValues[ch].mColorTableSize
                for i in range(colorInfos.contents.mValues[ch].mColorTableSize):
                    color = Color(colorInfos.contents.mValues[ch].mColorTable[i].mRed,
                                    colorInfos.contents.mValues[ch].mColorTable[i].mGreen,
                                    colorInfos.contents.mValues[ch].mColorTable[i].mBlue,
                                    colorInfos.contents.mValues[ch].mColorTable[i].mAlpha)
                    channel_info.mColorTable.append(color)
            colorInfos_py.append(channel_info)
        compression_py = compression.contents.value
        self.FreeMetadata(imageSizePerResolution, blockSizePerResolution, timeInfos, colorInfos)
        return imageSizePerResolution_py, blockSizePerResolution_py, imageExtents_py, timeInfos_py, colorInfos_py, compression_py

    def FreeMetadata(self, imageSizePerResolution : bpReaderTypesC_Size5DVectorPtr, blockSizePerResolution : bpReaderTypesC_Size5DVectorPtr,
                    timeInfos : bpReaderTypesC_TimeInfosPtr, colorInfos : bpReaderTypesC_ColorInfosPtr):
        self.mcdll.bpImageReaderC_FreeMetadata.argtypes = [bpReaderTypesC_Size5DVectorPtr, bpReaderTypesC_Size5DVectorPtr, bpReaderTypesC_TimeInfosPtr, bpReaderTypesC_ColorInfosPtr]
        self.mcdll.bpImageReaderC_FreeMetadata.restype = None
        self.mcdll.bpImageReaderC_FreeMetadata(imageSizePerResolution, blockSizePerResolution, timeInfos, colorInfos)

    def ReadParameters(self):
        parameters = bpReaderTypesC_ParametersPtr(bpReaderTypesC_Parameters())
        self.mcdll.bpImageReaderC_ReadParametersFloat.argtypes = [bpImageReaderCPtr, bpReaderTypesC_ParametersPtr]
        self.mcdll.bpImageReaderC_ReadParametersFloat.restype = None
        self.mcdll.bpImageReaderC_ReadParametersFloat(self.mImageReaderPtr, parameters)
        parameters_py = Parameters()
        for i in range(parameters.contents.mValuesCount):
            for j in range(parameters.contents.mValues[i].mValuesCount):
                parameters_py.set_value(parameters.contents.mValues[i].mName.decode("utf-8"), parameters.contents.mValues[i].mValues[j].mName.decode("utf-8"), parameters.contents.mValues[i].mValues[j].mValue.decode("utf-8"))
        self.FreeParameters(parameters)
        return parameters_py

    def FreeParameters(self, parameters : bpReaderTypesC_ParametersPtr):
        self.mcdll.bpImageReaderC_FreeParameters.argtypes = [bpReaderTypesC_ParametersPtr]
        self.mcdll.bpImageReaderC_FreeParameters.restype = None
        self.mcdll.bpImageReaderC_FreeParameters(parameters)

    def ReadHistogram(self, index : IndexTCR):
        self.mcdll.bpImageReaderC_ReadHistogramFloat.argtypes = [bpImageReaderCPtr, bpReaderTypesC_IndexTCRPtr]
        self.mcdll.bpImageReaderC_ReadHistogramFloat.restype = bpReaderTypesC_HistogramPtr
        return self.mcdll.bpImageReaderC_ReadHistogramFloat(self.mImageReaderPtr, index.get_c_indexTCR())
    
    def ReadThumbnail(self):
        self.mcdll.bpImageReaderC_ReadThumbnailFloat.argtypes = [bpImageReaderCPtr]
        self.mcdll.bpImageReaderC_ReadThumbnailFloat.restype = bpReaderTypesC_ThumbnailPtr
        return self.mcdll.bpImageReaderC_ReadThumbnailFloat(self.mImageReaderPtr)

    def Destroy(self):
        self.mcdll.bpImageReaderC_DestroyFloat.argtypes = [bpImageReaderCPtr]
        self.mcdll.bpImageReaderC_DestroyFloat.restype = None
        self.mcdll.bpImageReaderC_DestroyFloat(self.mImageReaderPtr)
