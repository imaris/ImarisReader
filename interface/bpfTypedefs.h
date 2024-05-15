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


#ifndef __BPTYPEDEFS__
#define __BPTYPEDEFS__


#if defined(__linux__)
#include <stdint.h>
#include <stdio.h>
#endif

#include <string>

using bpfString = std::string;
using bpfFloat = float;
using bpfDouble = double;
using bpfSize = size_t;

#if defined(_WIN32) || defined(__linux__)
using bpfChar = char;
using bpfInt8 = signed char;
using bpfInt16 = short int;
using bpfInt32 = int;
using bpfInt64 = long long;
using bpfUChar = unsigned char;
using bpfUInt8 = unsigned char;
using bpfUInt16 = unsigned short int;
using bpfUInt32 = unsigned int;
using bpfUInt64 = unsigned long long;
#endif

#if defined(__APPLE__)
using bpfChar = char;
using bpfInt8 = int8_t;
using bpfInt16 = int16_t;
using bpfInt32 = int32_t;
using bpfInt64 = int64_t;
using bpfUChar = unsigned char;
using bpfUInt8 = uint8_t;
using bpfUInt16 = uint16_t;
using bpfUInt32 = uint32_t;
using bpfUInt64 = uint64_t;
#endif

enum bpfNumberType {
  bpfNoType,
  bpfInt8Type,
  bpfUInt8Type,
  bpfInt16Type,
  bpfUInt16Type,
  bpfInt32Type,
  bpfUInt32Type,
  bpfFloatType,
  bpfDoubleType,
  bpfPackedRGBAType,
  bpfInt64Type,
  bpfUInt64Type
};


#endif
