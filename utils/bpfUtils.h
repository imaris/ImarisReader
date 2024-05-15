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


#ifndef __BPFUTILS__
#define __BPFUTILS__

#include "ImarisReader/types/bpfTypes.h"
#include "ImarisReader/types/bpfTimeInfo.h"

#include <vector>

class bpfTimeInfo;

int bpf_rfind(const bpfString&, char);

bpfString bpfTrim(const bpfString& s);
bpfString bpfTrim(const bpfString& s, const bpfString& aTrimCharacters);
std::vector<bpfString> bpfSplit(const bpfString& aString, const bpfString& aSplitString);
// If aSkipEmptyParts is enabled, multiple consecutive separators are treatened as one separator, thus not creating empty vector elements
std::vector<bpfString> bpfSplit(const bpfString& aString, const bpfString& aSeparatorString, const bool aSkipEmptyParts);
// If aSkipEmptyBoundParts is enabled the strings before the first separator and after the last separator will be skipped if they are empty
std::vector<bpfString> bpfSplit(const bpfString& aString, const bpfString& aSeparatorString, const bool aSkipEmptyParts, const bool aSkipEmptyBoundParts);
bpfString bpfJoin(const std::vector<bpfString>& aStrings, const bpfString& aDelimitor);
bpfString bpfReplace(const bpfString& aString, const bpfString& aSubStringOld, const bpfString& aSubStringNew);
bool bpfStartsWith(const bpfString& aString, const bpfString& aPrefix);
bool bpfEndsWith(const bpfString& aString, const bpfString& aPostfix);

bpfString bpfToUpper(const bpfString& aString);
bpfString bpfToLower(const bpfString& aString);

int bpf_filenpos(const bpfString&);

extern char bpfMonths[12][4];
char*  bpfMonthStr(int month);
char* bpfWDayStr(int year, int month, int day);
int bpf_isnumber(char);
///checks if aString is "um" or a variant
bool bpfIsMicro(const bpfString& aString);

bpfString bpfToUtf8(const std::wstring& aString);
std::wstring bpfFromUtf8(const bpfString& aString);
bpfString bpfFromUtf16(const bpfUInt16* aString, bpfSize aNumberOfUnicodeCharacters);

bpfString bpfLatin1FromUtf16(const std::wstring& aString);
bpfString bpfLatin1FromUtf8(const bpfString& aString);

/**
 * Converts a bpString which is encoded in the
 * currently active OS encoding to a UTF-8 encoded bpString
 * @param aString String to be converted
 * @return the converted UTF-8 string
 */
bpfString bpfUtf8FromSystemEncoding(const bpfString& aString);

template <typename TIterator, typename TValue>
TValue bpfJoin(TIterator aBegin, TIterator aEnd, const TValue& aDelimitor)
{
  TValue vResult;
  while (aBegin != aEnd) {
    vResult += *aBegin++;
    if (aBegin != aEnd) vResult += aDelimitor;
  }
  return vResult;
}

bpfString bpfIntToString(bpfUInt16 aValue);

bpfString bpfGetTemporaryPath();

bpfUInt32 bpfGetProcessId();

/**
 * 0.0 is equivalent to 30th december 1899 midnight
 * integer part is days since 30th december 1899
 * fractional part is time since midnight 0.0 => 0:00, 0.25 => 06:00
 * @param aOffSetFrom1900 Must be positive
 */
bpfTimeInfo bpfTimeInfoFromDate1900(bpfDouble aOffSetFrom1900);
bool IsTimeInfoValid(const bpfTimeInfo& aTimeInfo);

/**
 * 0.0 is equivalent to aJulianDay
 * integer part is days since aJulianDay
 * fractional part is time
 * since midnight 0.0 => 0:00, 0.25 => 06:00
 * @param aOffSetFrom1900 Must be positive
 */
bpfTimeInfo bpfTimeInfoFromDate(bpfDouble aOffSetFromJulianDay, bpfInt32 aJulianDay);

bool bpfTimeInfoFromMetamorphFormat(const bpfString& aTime, bpfTimeInfo& aTimeInfo);

/**
* Tries to fill up a bpTimeInfo by parsing certain types of date formats
* Method to extend
* Return false if could not find a known pattern
*/
bool bpfTimeInfoFromAnyFormat(const bpfString& aTime, bpfTimeInfo& aTimeInfo);


void FixPadding(bpfChar* aData, const bpfUInt64 (&aSrcDim)[3], const bpfUInt64 (&aDestDim)[3], bpfUInt32 aSizeOfType);

//same values used to check timepoints
//from base/application/bpData.cxx
//const bpfTimeInfo mValidTimeInfoMin("1750-01-01");
//const bpfTimeInfo mValidTimeInfoMax("2300-12-31");

#endif
