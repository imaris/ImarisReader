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


#include "ImarisReader/types/bpfParameterSection.h"

#include <sstream>


bpfParameterSection::bpfParameterSection()
{
}


bpfParameterSection::bpfParameterSection(tParameterMap aParameterMap)
  : parameterMap(std::move(aParameterMap))
{
}


bpfParameterSection::operator tParameterMap() &&
{
  return std::move(parameterMap);
}


/**
 * Merge this section with another section. Parameters that do not yet exist
 * are added. Parameters that exist are overwritten with new values.
 *
 * @param other The section to merge with.
   */
void bpfParameterSection::Merge(const bpfParameterSection& other)
{
  if (this != &other) {                 // dont merge with ourselves

    tParameterMap::const_iterator pos = other.parameterMap.begin();

    while (pos != other.parameterMap.end()) {
      SetParameter((*pos).first,(*pos).second);
      ++pos;
    }
  }
}

/**************************************************************************
 *
 * Sets a parameter to the specified value. If the parameter does not yet
 * exist, it is created.
 *
 * Parameters :
 *    parameterName : parameter to set, it does not need to exist
 *    value         : value to which the parameter is set
 *
 **************************************************************************/
void
bpfParameterSection::SetParameter(const bpfString& parameterName,
                                  const bpfString& value)
{
  parameterMap[parameterName] = value;
}

void
bpfParameterSection::SetParameter(const bpfString& parameterName,
                                  float value)
{
  std::ostringstream parstr;
  parstr << value;
  parameterMap[parameterName] = parstr.str();
}

/***************************************************************************
 *
 * Get a parameter value. If the parameter does not exist the exception
 * bpfNoSuchParameter is thrown.
 *
 * Parameters:
 *  parameterName : the name of the parameter to query
 *
 * Returns:
 *   the value of the parameter
 *
 * Throws:
 *   bpfNoSuchParameterException if the parameter does not exist
 *
 **************************************************************************/
const bpfString&
bpfParameterSection::GetParameter(const bpfString& parameterName) const

{
  std::map<bpfString,bpfString>::const_iterator pos = parameterMap.find(parameterName);

  if (pos == parameterMap.end()) {
    throw bpfNoSuchParameterException(bpfString("bpfParameterSection::GetParameter: ")
                                      + parameterName);
  }

  return(*pos).second;
}

/**************************************************************************
 *
 * Checks if the parameter exists.
 *
 * Parameters:
 *   parameterName : name of the parameter to query
 *
 * Returns:
 *   1 if the parameter exists, 0 otherwise
 *
 *************************************************************************/
bool
bpfParameterSection::HasParameter(const bpfString& parameterName) const
{
  return parameterMap.find(parameterName) != parameterMap.end();
}

/**************************************************************************
 *
 * Remove a parameter. If it does not exist no error occurs.
 *
 * Parameters:
 *   parameterName : Parameter to remove
 *
 *************************************************************************/
void
bpfParameterSection::RemoveParameter(const bpfString& parameterName)
{
  std::map<bpfString,bpfString>::iterator pos = parameterMap.find(parameterName);
  if (pos != parameterMap.end()) {
    parameterMap.erase(pos);
  }
}


/***************************************************************************
 *
 * To access all parameters, iterators are used. As all iterators Begin()
 * returns an iterator pointing to the first element, End() returns an
 * iterator pointing behind the last element.
 *
 **************************************************************************/
bpfParameterSection::Iterator
bpfParameterSection::Begin()
{
  return parameterMap.begin();
}

bpfParameterSection::Iterator
bpfParameterSection::End()
{
  return parameterMap.end();
}

bpfParameterSection::ConstIterator
bpfParameterSection::Begin() const
{
  return parameterMap.begin();
}

bpfParameterSection::ConstIterator
bpfParameterSection::End() const
{
  return parameterMap.end();
}

/**************************************************************************
 *
 * Write the parameters to a stream in the format:
 *
 * ParameterName = value
 *
 * Parameters:
 *   out : the stream to write to
 *
 * Returns:
 *   the stream (out)
 *
 *************************************************************************/
std::ostream&
bpfParameterSection::Write(std::ostream& out)
{
  Iterator pos = Begin();
  while (pos != End()) {
    out << (*pos).first.c_str() << " = " << (*pos).second.c_str() << std::endl;
//    out << (*pos).first << " = " << (*pos).second << endl;
    ++pos;
  }
  return out;
}


bpfString bpfParameterSection::RemoveSpecialChars(const bpfString& aString) const
{
  bpfString vResult = "";
  for (bpfUInt32 vPos = 0; vPos < aString.length(); vPos++) {
    switch (aString[vPos]) {
    case '\n':
      vResult += "\\n";
      break;
    case '\r':
      vResult += "\\r";
      break;
    case '\t':
      vResult += "\\t";
      break;
    default:
      vResult += aString[vPos];
    }
  }
  return vResult;
}


bpfString bpfParameterSection::ToString() const
{
  bpfString vResult = "";
  tParameterMap::const_iterator vPos = parameterMap.begin();
  while (vPos != parameterMap.end()) {
    vResult += vPos->first + " = " + RemoveSpecialChars(vPos->second) + "\n";
    ++vPos;
  }
  return vResult;
}

