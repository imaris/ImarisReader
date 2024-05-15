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


#ifndef __bpfTIME_INFO__
#define __bpfTIME_INFO__

#include "ImarisReader/types/bpfTypes.h"

/**
 * Provides date and time functions.
 *
 * A bpfTimeInfo object contains a calendar date and a clock time. It can read
 * the current datetime from the system clock. It provides functions for
 * comparing datetimes and for manipulating a datetime by adding a number of
 * milliseconds, days or years.
 *
 * Representation of datetime object is always normalized so that day has
 * no more than 24 hours. E.g. if you add 11h to 20h it will be 7h the next
 * day. (Note: this is a change in the interface. It used to be that time was
 * NOT normalized and 31h was a valid time.)
 *
 * Internally uses a Julian calendar for days and milliseconds from midnight
 * for time.
 */
class bpfTimeInfo
{

public:

  /**
   * Constructs a timeinfo starting at -4713-11-24 00:00:00.000
   *
   * @todo mg should be BCE 4713 January 01 12:00:00.0 UT Monday according to
   * Julian day definition
   *
   * Julian day and miliseconds are both set to zero.
   */
  bpfTimeInfo();

  /**
   * Constructs a timeinfo object from the given string.
   *
   * String is of format "2011-11-27 15:42:37.285". It's also possible to only
   * pass the date or time part. In that case the date part will be set to
   * Julian date zero resp. time part to midnight.
   */
  explicit bpfTimeInfo(const bpfString& aTimeInfo);

  bpfTimeInfo(const bpfTimeInfo& aTimeInfo);

  ~bpfTimeInfo();

  /**
   * Adds years to the timeinfo object. Pos. and neg. values possible.
   */
  void AddYears(bpfInt32 aYears);

  /**
   * Adds days to the timeinfo object. Pos. and neg. values possible.
   */
  void AddDays(bpfInt32 aDays);

  /**
   * Adds miliseconds to the timeinfo object. Pos. and neg. values possible.
   */
  void AddMilliseconds(bpfInt64 aMilliseconds);
  void AddMicroseconds(bpfInt64 aMicroseconds);
  void AddNanoseconds(bpfInt64 aNanoseconds);

  /**
   * Set timeinfo object to the given string.
   *
   * String is of format "2011-11-27 15:42:37.285". It's also possible to only
   * pass the date or time part. In that case the date part will be set to
   * Julian date zero resp time part to midnight.
   */
  void FromString(const bpfString& aTimeInfo);

  /**
   * Returns a string representation of the timeinfo object
   * in the form "2011-11-27 15:42:37.285".
   */
  bpfString ToString() const;
  bpfString ToStringNanosecond() const;

  /**
   * Sets date from given string with format "2002-11-27". Does not change the
   * time part of the object.
   */
  void SetDate(const bpfString& aDate);

  /**
   * Sets date to given Julian date. Does not change the
   * time part of the object.
   */
  void SetDate(bpfInt32 aJulianDay);

  /**
   * Sets time from given string with format "15:42:37.285". The object is normalized if
   * given time crosses the 24h boundary.
   */
  void SetTime(const bpfString& aTime);

  /**
   * Sets time to given milliseconds from midnight. The object is normalized if
   * given time crosses the 24h boundary.
   */
  void SetTime(bpfInt64 aTimeMilliSeconds);
  void SetTimeMicroSeconds(bpfInt64 aTimeMicroSeconds);
  void SetTimeNanoSeconds(bpfInt64 aTimeNanoSeconds);

  // These methods have deliberatively been removed because file readers should not
  // use them. In case of undefined timestamps, use
  // the method: bpfFileTools::GetFileCreationDate(GetFileName());
  //void SetToday();
  //void SetTodayAndNow();

  /**
   * Returns a string representation of the date part in the form "2011-11-27".
   */
  const bpfString& GetDate() const;

  /**
   * Returns a string representation of the time part in the form "15:42:37.285".
   */
  const bpfString& GetTime() const;
  const bpfString& GetTimeNanosecond() const;

  /**
   * Returns the day part of the timeinfo object in Julian days.
   */
  bpfInt32 GetJulianDay() const;

  /**
   * Returns the time part of the timeinfo object in miliseconds.
   */
  bpfInt64 GetMilliseconds() const;

  /**
   * Returns the time part of the timeinfo object in microseconds.
   */
  bpfInt64 GetMicroseconds() const;

  /**
   * Returns the time part of the timeinfo object in nanoseconds.
   */
  bpfInt64 GetNanoseconds() const;

  /**
   * Compares two datatime objects and returns true
   * if first object is smaller then second.
   */
  static bool IsSmaller(const bpfTimeInfo& aTimeInfoA,
                        const bpfTimeInfo& aTimeInfoB);

  /**
   * Compares two datatime objects and returns true
   * if first object is bigger then second.
   */
  static bool IsBigger(const bpfTimeInfo& aTimeInfoA,
                       const bpfTimeInfo& aTimeInfoB);

  /**
   * Compares two datatime objects and returns true
   * if objects are equal.
   */
  static bool IsEqual(const bpfTimeInfo& aTimeInfoA,
                      const bpfTimeInfo& aTimeInfoB);

  /**
   * Adds two datatime objects and returns a new normalized date time object.
   */
  static bpfTimeInfo Add(const bpfTimeInfo& aTimeInfoA,
                        const bpfTimeInfo& aTimeInfoB);

  /**
   * Adds milliseconds to a datatime objects and
   * returns a new normalized date time object.
   */
  static bpfTimeInfo Add(const bpfTimeInfo& aTimeInfo, bpfInt64 aMilliseconds);
  static bpfTimeInfo AddMicroseconds(const bpfTimeInfo& aTimeInfo, bpfInt64 aMicroseconds);
  static bpfTimeInfo AddNanoseconds(const bpfTimeInfo& aTimeInfo, bpfInt64 aNanoseconds);

  /**
   * Subtracts second datatime objects from first and
   * returns a new normalized date time object.
   */
  static bpfTimeInfo Sub(const bpfTimeInfo& aTimeInfoA,
                        const bpfTimeInfo& aTimeInfoB);

  /**
   * Subtracts milliseconds from timeinfo object and
   * returns a new normalized date time object.
   */
  static bpfTimeInfo Sub(const bpfTimeInfo& aTimeInfo, bpfInt64 aMilliseconds);
  static bpfTimeInfo SubMicroseconds(const bpfTimeInfo& aTimeInfo, bpfInt64 aMicroseconds);
  static bpfTimeInfo SubNanoseconds(const bpfTimeInfo& aTimeInfo, bpfInt64 aNanoseconds);

  /**
   * Mulitplies Julian days and milliseconds of object aTimeInfo with aFactor. Returned
   * object is normalized.
   */
  static bpfTimeInfo Multiply(bpfDouble aFactor, const bpfTimeInfo& aTimeInfo);

  /**
   * Result is aTimeInfoA - aTimeInfoB all in milliseconds. May return a
   * negative value.
   */
  static bpfInt64 DiffMilliseconds(const bpfTimeInfo& aTimeInfoA,
                                  const bpfTimeInfo& aTimeInfoB);
  static bpfInt64 DiffMicroseconds(const bpfTimeInfo& aTimeInfoA,
                                  const bpfTimeInfo& aTimeInfoB);
  static bpfInt64 DiffNanoseconds(const bpfTimeInfo& aTimeInfoA,
                                 const bpfTimeInfo& aTimeInfoB);

  /**
   * Calculates Julian days from aYear, aMonth and aDay. Days and months
   * start at 1 (January = 1, December = 12).
   */
  static bpfInt32 ToJulianDay(bpfInt32 aYear, bpfInt32 aMonth, bpfInt32 aDay);

  /**
   * Sets aYear, aMonth and aDay from the given aJulian day. Days and months
   * start at 1 (January = 1, December = 12).
   */
  static void FromJulianDay(bpfInt32 aJulianDay,
                            bpfInt32& aYear,
                            bpfInt32& aMonth,
                            bpfInt32& aDay);

private:

  /**
   * Modify representation so that day has no more than 24 hours.
   */
  void Normalize();

  bpfString GetHour() const;
  bpfString GetMinute() const;
  bpfString GetSecond() const;
  bpfString GetMillisecond() const;
  bpfString GetNanosecond() const;

  bpfString ToString(bpfInt64 aValue,
                    bpfUInt16 aDecimals = 2,
                    bool aLeadingZeros = true) const;

  bpfInt32 mJulianDay;
  bpfInt64 mNanosecondsOfDay;

  /// the next members are only kept for performace reasons
  mutable bpfString mTime;
  mutable bpfString mTimeNanoseconds;
  mutable bpfString mDate;
  mutable bpfInt64 mLastGetTime;
  mutable bpfInt64 mLastGetTimeNanoseconds;
  mutable bpfInt32 mLastGetDateJulianDay;
};

/**
 * Global function for adding two bpfTimeInfo objects.
 */
inline bpfTimeInfo operator+ (const bpfTimeInfo& aTimeInfoA,
                             const bpfTimeInfo& aTimeInfoB)
{
  return bpfTimeInfo::Add(aTimeInfoA, aTimeInfoB);
}

inline bpfTimeInfo operator+ (const bpfTimeInfo& aTimeInfo,
                             bpfInt64 aMilliseconds)
{
  return bpfTimeInfo::Add(aTimeInfo, aMilliseconds);
}

inline bpfTimeInfo operator+ (bpfInt64 aMilliseconds,
                             const bpfTimeInfo& aTimeInfo)
{
  return bpfTimeInfo::Add(aTimeInfo, aMilliseconds);
}

/**
 * Global function for subtracting two bpfTimeInfo objects.
 */
inline bpfTimeInfo operator- (const bpfTimeInfo& aTimeInfoA,
                             const bpfTimeInfo& aTimeInfoB)
{
  return bpfTimeInfo::Sub(aTimeInfoA, aTimeInfoB);
}

inline bpfTimeInfo operator- (const bpfTimeInfo& aTimeInfo,
                             bpfInt64 aMilliseconds)
{
  return bpfTimeInfo::Sub(aTimeInfo, aMilliseconds);
}

inline bpfTimeInfo operator- (bpfInt64 aMilliseconds,
                             const bpfTimeInfo& aTimeInfo)
{
  return bpfTimeInfo::Sub(aTimeInfo, aMilliseconds);
}

/**
 * Global function for multiplying two bpfTimeInfo objects.
 */

inline bpfTimeInfo operator* (bpfDouble aFactor,
                             const bpfTimeInfo& aTimeInfo)
{
  return bpfTimeInfo::Multiply(aFactor, aTimeInfo);
}

inline bpfTimeInfo operator* (const bpfTimeInfo& aTimeInfo,
                             bpfDouble aFactor)
{
  return bpfTimeInfo::Multiply(aFactor, aTimeInfo);
}

inline bpfTimeInfo operator/ (const bpfTimeInfo& aTimeInfo,
                             bpfDouble aDivisor)
{
  return bpfTimeInfo::Multiply(1.0 / aDivisor, aTimeInfo);
}

/**
 * Global function comparing two bpfTimeInfo objects.
 */
inline bool operator< (const bpfTimeInfo& aTimeInfoA,
                       const bpfTimeInfo& aTimeInfoB)
{
  return bpfTimeInfo::IsSmaller(aTimeInfoA, aTimeInfoB);
}

inline bool operator> (const bpfTimeInfo& aTimeInfoA,
                       const bpfTimeInfo& aTimeInfoB)
{
  return bpfTimeInfo::IsBigger(aTimeInfoA, aTimeInfoB);
}

inline bool operator== (const bpfTimeInfo& aTimeInfoA,
                        const bpfTimeInfo& aTimeInfoB)
{
  return bpfTimeInfo::IsEqual(aTimeInfoA, aTimeInfoB);
}

inline bool operator!= (const bpfTimeInfo& aTimeInfoA,
                        const bpfTimeInfo& aTimeInfoB)
{
  return !bpfTimeInfo::IsEqual(aTimeInfoA, aTimeInfoB);
}

#endif // __bpfTIME_INFO__

