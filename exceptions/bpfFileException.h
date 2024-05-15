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


#ifndef __BP_FILE_EXCEPTION__
#define __BP_FILE_EXCEPTION__

/*****************************************************************************
 *
 * Base class for all exception that occur in the fileIO classes.
 *
 * Base classes          : bpfException
 * Known derived classes : bpfFileIOException, bpf FileFormatException
 *
 *
 ****************************************************************************/
#include "ImarisReader/exceptions/bpfException.h"

class bpfFileException : public bpfException
{
public:
  /**************************************************************************
   *
   * Construct an exception with no detail message.
   *
   **************************************************************************/
  bpfFileException()
    :bpfException("bpfFileException","") {}

  /**************************************************************************
   *
   * Construct an exception with the given detail message.
   *
   **************************************************************************/
  bpfFileException(const bpfString& detailMessage)
    :bpfException("bpfFileException",detailMessage) {}

protected:
  /**************************************************************************
   *
   * Construct an exception with the given name and detail message. Used
   * (only) for subclasses.
   *
   *************************************************************************/
  bpfFileException(const bpfString& exceptionName, const bpfString& detailMessage)
    :bpfException(exceptionName,detailMessage) {}
};

#endif
