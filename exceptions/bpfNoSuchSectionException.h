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


#ifndef __BP_NOSUCHSECTION_EXCEPTION__
#define __BP_NOSUCHSECTION_EXCEPTION__

#include "ImarisReader/exceptions/bpfException.h"
/*****************************************************************************
 *
 * Exception used in bpfSectionContainer.
 *
 ****************************************************************************/
class bpfNoSuchSectionException : public bpfException
{
public:
  ///
  // Construct an exception with no detail message.
  //
  bpfNoSuchSectionException()
    :bpfException("bpfNoSuchSection","") {}

  ///
  // Construct an exception with the given detail message.
  //
  bpfNoSuchSectionException(const bpfString& detailMessage)
    :bpfException("bpfNoSuchSection",detailMessage) {}
};

#endif
