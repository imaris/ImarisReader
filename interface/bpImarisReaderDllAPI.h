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


#ifndef __BP_IMARISREADERDLL_API_H__
#define __BP_IMARISREADERDLL_API_H__

// DLL stuff
#ifdef _WIN32
#ifndef BP_IMARISREADER_DLL_EXPORT
#define BP_IMARISREADER_DLL_EXPORT __declspec( dllexport )
#endif
#ifndef BP_IMARISREADER_DLL_IMPORT
#define BP_IMARISREADER_DLL_IMPORT __declspec( dllimport )
#endif
#else //_WIN32
#ifndef BP_IMARISREADER_DLL_EXPORT
#define BP_IMARISREADER_DLL_EXPORT __attribute__((visibility("default")))
#endif
#ifndef BP_IMARISREADER_DLL_IMPORT
#define BP_IMARISREADER_DLL_IMPORT __attribute__((visibility("default")))
#endif
#endif


#ifdef IMARISREADER_DLL

// For unit tests we build a static lib
#ifdef BP_UNIT_TEST
#define BP_IMARISREADER_DLL_API
#else
#define BP_IMARISREADER_DLL_API BP_IMARISREADER_DLL_IMPORT
#endif

#else // building the dll

// For unit tests we build a static lib
#ifdef BP_UNIT_TEST
#define BP_IMARISREADER_DLL_API
#else
#define BP_IMARISREADER_DLL_API BP_IMARISREADER_DLL_EXPORT
#endif

#endif // IMARISREADER_DLL

#endif // __BP_IMARISREADERDLL_API_H__
