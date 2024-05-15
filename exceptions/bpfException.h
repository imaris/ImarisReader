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


#ifndef __BP_EXCEPTION__
#define __BP_EXCEPTION__

#if defined(_WIN32)
#include <windows.h>
#endif

/*****************************************************************************
 *
 * Base class for all exception.
 *
 * All objects for which exceptions are thrown should be derived from this
 * class. An exception contains its name and may contain a more specific
 * message, which is set when it is constructed.
 *
 * Classes derived from exception should always have two public constructors:\\
 *  - a default constructor, and\\
 *  - a constructor with a bpfString argument \\
 * and a protected contructor (for subclassing) that initializes the name and
 * the message by calling the parents protected constructor. If the class is
 * {\bf not} to be used as a base class, this constructor may be ommitted.
 *
 * The name of all exceptions should be their class name.
 *
 ****************************************************************************/
#include "ImarisReader/exceptions/bpfExceptionBase.h"


class bpfException : public bpfExceptionBase
{
public:
  /**************************************************************************
   *
   * Construct an exception with no detail message.
   *
   **************************************************************************/
  bpfException();



  /**************************************************************************
   *
   * Construct an exception with the given detail message.
   *
   **************************************************************************/
  bpfException(const bpfString& detailMessage);

  /**************************************************************************
   *
   * Destructor
   *
   **************************************************************************/
  virtual ~bpfException();

  /**************************************************************************
   *
   * Get the name and the detail message. If there is no detail message, an
   * empty bpfString is returned.
   *
   *************************************************************************/
  const bpfString& GetName() const;
  const bpfString& GetDescription() const;

protected:
  /**************************************************************************
   *
   * Construct an exception with the given name and detail message. Used
   * (only) for subclasses.
   *
   *************************************************************************/
  bpfException(const bpfString& exceptionName, const bpfString& detailMessage);

private:
  bpfString name;
  bpfString message;
};

#endif
