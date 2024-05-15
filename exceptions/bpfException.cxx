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


#include "ImarisReader/exceptions/bpfException.h"

/*****************************************************************************
 *
 * Default Constructor
 *
 * public
 *
 ****************************************************************************/
bpfException::bpfException()
  :name("bpfException"), message("")
{}

/*****************************************************************************
 *
 * Constructor with argument for detail message.
 *
 * public
 *
 ****************************************************************************/
bpfException::bpfException(const bpfString& detailMessage)
  :name("bpfException"),
   message(detailMessage)
{}

/*****************************************************************************
 *
 * Constructor for subclasses
 *
 * protected
 *
 ****************************************************************************/
bpfException::bpfException(const bpfString& exceptionName,
                           const bpfString& detailMessage)
  :name(exceptionName),
   message(detailMessage)
{}

/*****************************************************************************
 *
 * Destructor
 *
 ****************************************************************************/
bpfException::~bpfException()
{}

/*****************************************************************************
 *
 * Get the name
 *
 ****************************************************************************/
const bpfString&
bpfException::GetName() const
{
  return name;
}

/*****************************************************************************
 *
 * Get the detail message. If no detail message was specified, an empty bpfString
 * is returned.
 *
 ****************************************************************************/
const bpfString&
bpfException::GetDescription() const
{
  return message;
}

