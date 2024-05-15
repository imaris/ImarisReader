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


#ifndef __BP_FILE_FORMAT_EXCEPTION__
#define __BP_FILE_FORMAT_EXCEPTION__

/*****************************************************************************
 *
 * Class for file format Exceptions.
 *
 * \begin{tabular}{ll}
 * Base classes          & bpfFileException \\
 * Known derived classes & should not be used as base class \\
 * \end{tabular}
 *
 *
 ****************************************************************************/
#include "ImarisReader/exceptions/bpfFileException.h"

class bpfFileFormatException : public bpfFileException
{
public:
  /**************************************************************************
   *
   * Construct an exception with no detail message.
   *
   **************************************************************************/
  bpfFileFormatException()
    :bpfFileException("bpfFileFormatException","") {}

  /**************************************************************************
   *
   * Construct an exception with the given detail message.
   *
   **************************************************************************/
  bpfFileFormatException(const bpfString& detailMessage)
    :bpfFileException("bpfFileFormatException",detailMessage) {}
};

#endif
