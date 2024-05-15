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


#include "ImarisReader/types/bpfTypes.h"
#include "ImarisReader/types/bpfPackedRGBA.h"
#include <iomanip>
#include <vector>
#include <cmath>
#include <sstream>
#include <iostream>
#include <limits>
#include <set>

/**
 * Return the number of bytes for the type indicated by the parameter.
 *
 * If the type is bpfNoType, then 0 is returned.
 *
 * @param type   The type.
 * @return The size for the type.
 */


bpfUInt32 bpfGetSizeOfType(bpfNumberType type)
{
  switch (type) {
  case bpfInt8Type:
    return sizeof(bpfInt8);
  case bpfUInt8Type:
    return sizeof(bpfUInt8);
  case bpfInt16Type:
    return sizeof(bpfInt16);
  case bpfUInt16Type:
    return sizeof(bpfUInt16);
  case bpfInt32Type:
    return sizeof(bpfInt32);
  case bpfUInt32Type:
    return sizeof(bpfUInt32);
  case bpfInt64Type:
    return sizeof(bpfInt64);
  case bpfUInt64Type:
    return sizeof(bpfUInt64);
  case bpfFloatType:
    return sizeof(bpfFloat);
  case bpfDoubleType:
    return sizeof(bpfDouble);
  case bpfPackedRGBAType:
    return sizeof(bpfPackedRGBA);
  default:
    return 0;
  }
}

/**
 * Append an integer to a string.
 *
 * @param s The original string.
 * @param i The integer that is appended.
 * @return A string with i appended to s
 */
bpfString operator+(const bpfString& s,bpfInt32 i)
{
  std::ostringstream vStr;
  vStr << i;
  return s + vStr.str();
}

/**
 * Append an integer to a string.
 *
 * @param s The original string.
 * @param i The integer that is appended.
 * @return A string with i appended to s
 */
bpfString operator+(const bpfString& s,bpfUInt32 i)
{
  std::ostringstream vStr;
  vStr << i;
  return s + vStr.str();
}

/**
 * Append a float to a string.
 *
 * @param s The original string.
 * @param f The float that is appended.
 * @return A string with f appended to s
 */
bpfString operator+(const bpfString& s,float f)
{
  std::ostringstream vStr;
  vStr << f;
  return s + vStr.str();
}

//
// No ANSI limits for rationals
//


/**
 * Convert a type to a string. This must be defined for all types used as
 * template argument in bpfImageFields.
 *
 * @param value The value to be converted to a string.
 * @return The converted string.
 */
bpfString bpfToString(bpfUInt8 value)
{
  std::ostringstream vStr;
  vStr << (int)value;
  return vStr.str();
}

bpfString bpfToString(bpfInt8 value)
{
  std::ostringstream vStr;
  vStr << (int)value;
  return vStr.str();
}

bpfString bpfToString(bpfUInt16 value)
{
  std::ostringstream vStr;
  vStr << (int)value;
  return vStr.str();
}

bpfString bpfToString(bpfInt16 value)
{
  std::ostringstream vStr;
  vStr << (int)value;
  return vStr.str();
}

bpfString bpfToString(bpfUInt32 value)
{
  std::ostringstream vStr;
  vStr << value;
  return vStr.str();
}
bpfString bpfToString(bpfInt32 value)
{
  std::ostringstream vStr;
  vStr << value;
  return vStr.str();
}

bpfString bpfToString(bpfUInt64 value)
{
  std::ostringstream vStr;
  vStr << value;
  return vStr.str();
}

bpfString bpfToString(bpfInt64 value)
{
  std::ostringstream vStr;
  vStr << value;
  return vStr.str();
}

#if !defined(_WIN32)
bpfString bpfToString(bpfSize aValue)
{
  return bpfToString((bpfUInt32)aValue);
}
#endif // !__WIN32__

bpfUInt32 bpfPrecisionToDecimals(bpfFloat aValue, bpfUInt32 aPrecision)
{
  if (aValue == 0.0f) {
    return aPrecision;
  }
  bpfUInt32 vDecimals = (bpfUInt32) log10(fabs(aValue));
  vDecimals = fabs(aValue) < 1.0f ? vDecimals - 1 : vDecimals;
  if (vDecimals >= aPrecision - 1) {
    vDecimals = 0;
  }
  else {
    vDecimals = aPrecision - 1 - vDecimals;
  }
  return vDecimals;
}

bpfUInt32 bpfPrecisionToDecimals(bpfDouble aValue, bpfUInt32 aPrecision)
{
  if (aValue == 0.0) {
    return aPrecision;
  }
  bpfUInt32 vDecimals = (bpfUInt32) log10(fabs(aValue));
  vDecimals = fabs(aValue) < 1.0f ? vDecimals - 1 : vDecimals;
  if (vDecimals >= aPrecision - 1) {
    vDecimals = 0;
  }
  else {
    vDecimals = aPrecision - 1 - vDecimals;
  }
  return vDecimals;
}

bpfString bpfToString(bpfFloat aValue, bpfInt16 aFixedPrecision)
{
  std::ostringstream vOStringStream;
  if (aFixedPrecision >= 0) {
    vOStringStream.setf(std::ios_base::fixed);
    vOStringStream << std::setprecision(aFixedPrecision) << aValue;
  }
  else {
    vOStringStream << std::setprecision(std::numeric_limits<bpfFloat>::digits10) << aValue;
  }
  return vOStringStream.str();
}

bpfString bpfToString(bpfDouble aValue, bpfInt16 aFixedPrecision)
{
  std::ostringstream vOStringStream;
  if (aFixedPrecision >= 0) {
    vOStringStream.setf(std::ios_base::fixed);
    vOStringStream << std::setprecision(aFixedPrecision) << aValue;
  }
  else {
    vOStringStream << std::setprecision(std::numeric_limits<bpfDouble>::digits10) << aValue;
  }
  return vOStringStream.str();
}

bpfString bpfToString(bool value)
{
  if (value) {
    return bpfString("true");
  }
  else {
    return bpfString("false");
  }
}

bpfString bpfToString(bpfNumberType aNumberType)
{
  switch (aNumberType) {
  case bpfNoType:
    return "NoType";
  case bpfInt8Type:
    return "Int8";
  case bpfUInt8Type:
    return "UInt8";
  case bpfInt16Type:
    return "Int16";
  case bpfUInt16Type:
    return "UInt16";
  case bpfInt32Type:
    return "Int32";
  case bpfUInt32Type:
    return "UInt32";
  case bpfInt64Type:
    return "Int64";
  case bpfUInt64Type:
    return "UInt64";
  case bpfFloatType:
    return "Float";
  case bpfDoubleType:
    return "Double";
  case bpfPackedRGBAType:
    return "PackedRGBA";
  default:
    return "";
  }
}


template <class Element>
bpfString bpfToString(Element aBeginIterator, Element aEndIterator, bpfString aDelimiter)
{

  Element it = aBeginIterator;

  if (it == aEndIterator) {
    return "";
  }

  bpfString vString = bpfToString(*it);
  it++;

  while (it != aEndIterator) {
    vString += aDelimiter;
    vString += bpfToString(*it);
    it++;
  }

  return vString;
}


template bpfString bpfToString(bpfInt8* aBegin, bpfInt8* aEnd, bpfString aDelimiter);
template bpfString bpfToString(bpfInt16* aBegin, bpfInt16* aEnd, bpfString aDelimiter);
template bpfString bpfToString(bpfInt32* aBegin, bpfInt32* aEnd, bpfString aDelimiter);
template bpfString bpfToString(bpfInt64* aBegin, bpfInt64* aEnd, bpfString aDelimiter);

template bpfString bpfToString(bpfUInt8* aBegin, bpfUInt8* aEnd, bpfString aDelimiter);
template bpfString bpfToString(bpfUInt16* aBegin, bpfUInt16* aEnd, bpfString aDelimiter);
template bpfString bpfToString(bpfUInt32* aBegin, bpfUInt32* aEnd, bpfString aDelimiter);
template bpfString bpfToString(bpfUInt64* aBegin, bpfUInt64* aEnd, bpfString aDelimiter);

template bpfString bpfToString(bpfFloat* aBegin, bpfFloat* aEnd, bpfString aDelimiter);
template bpfString bpfToString(bpfDouble* aBegin, bpfDouble* aEnd, bpfString aDelimiter);
template bpfString bpfToString(bpfVector3Float* aBegin, bpfVector3Float* aEnd, bpfString aDelimiter);


template bpfString bpfToString(std::vector<bpfSize>::iterator aBegin, std::vector<bpfSize>::iterator aEnd, bpfString aDelimiter);
template bpfString bpfToString(std::vector<bpfInt8>::iterator aBegin, std::vector<bpfInt8>::iterator aEnd, bpfString aDelimiter);
template bpfString bpfToString(std::vector<bpfInt16>::iterator aBegin, std::vector<bpfInt16>::iterator aEnd, bpfString aDelimiter);
template bpfString bpfToString(std::vector<bpfInt32>::iterator aBegin, std::vector<bpfInt32>::iterator aEnd, bpfString aDelimiter);
template bpfString bpfToString(std::vector<bpfInt64>::iterator aBegin, std::vector<bpfInt64>::iterator aEnd, bpfString aDelimiter);

template bpfString bpfToString(std::vector<bpfUInt8>::iterator aBegin, std::vector<bpfUInt8>::iterator aEnd, bpfString aDelimiter);
template bpfString bpfToString(std::vector<bpfUInt16>::iterator aBegin, std::vector<bpfUInt16>::iterator aEnd, bpfString aDelimiter);
template bpfString bpfToString(std::vector<bpfUInt32>::iterator aBegin, std::vector<bpfUInt32>::iterator aEnd, bpfString aDelimiter);
template bpfString bpfToString(std::vector<bpfUInt64>::iterator aBegin, std::vector<bpfUInt64>::iterator aEnd, bpfString aDelimiter);

template bpfString bpfToString(std::vector<bpfFloat>::iterator aBegin, std::vector<bpfFloat>::iterator aEnd, bpfString aDelimiter);
template bpfString bpfToString(std::vector<bpfDouble>::iterator aBegin, std::vector<bpfDouble>::iterator aEnd, bpfString aDelimiter);
template bpfString bpfToString(std::vector<bpfVector3Float>::iterator aBegin, std::vector<bpfVector3Float>::iterator aEnd, bpfString aDelimiter);

template bpfString bpfToString(std::vector<bpfString>::iterator aBegin, std::vector<bpfString>::iterator aEnd, bpfString aDelimiter);


template bpfString bpfToString(std::set<bpfInt8>::iterator aBegin, std::set<bpfInt8>::iterator aEnd, bpfString aDelimiter);
template bpfString bpfToString(std::set<bpfInt16>::iterator aBegin, std::set<bpfInt16>::iterator aEnd, bpfString aDelimiter);
template bpfString bpfToString(std::set<bpfInt32>::iterator aBegin, std::set<bpfInt32>::iterator aEnd, bpfString aDelimiter);
template bpfString bpfToString(std::set<bpfInt64>::iterator aBegin, std::set<bpfInt64>::iterator aEnd, bpfString aDelimiter);

template bpfString bpfToString(std::set<bpfUInt8>::iterator aBegin, std::set<bpfUInt8>::iterator aEnd, bpfString aDelimiter);
template bpfString bpfToString(std::set<bpfUInt16>::iterator aBegin, std::set<bpfUInt16>::iterator aEnd, bpfString aDelimiter);
template bpfString bpfToString(std::set<bpfUInt32>::iterator aBegin, std::set<bpfUInt32>::iterator aEnd, bpfString aDelimiter);
template bpfString bpfToString(std::set<bpfUInt64>::iterator aBegin, std::set<bpfUInt64>::iterator aEnd, bpfString aDelimiter);

template bpfString bpfToString(std::set<bpfFloat>::iterator aBegin, std::set<bpfFloat>::iterator aEnd, bpfString aDelimiter);
template bpfString bpfToString(std::set<bpfDouble>::iterator aBegin, std::set<bpfDouble>::iterator aEnd, bpfString aDelimiter);
template bpfString bpfToString(std::set<bpfVector3Float>::iterator aBegin, std::set<bpfVector3Float>::iterator aEnd, bpfString aDelimiter);

template bpfString bpfToString(std::set<bpfString>::iterator aBegin, std::set<bpfString>::iterator aEnd, bpfString aDelimiter);

