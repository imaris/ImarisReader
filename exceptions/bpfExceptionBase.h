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


#ifndef __BPFEXCEPTIONBASE__
#define __BPFEXCEPTIONBASE__


#ifndef __BP_TYPES__
#include <string>
using bpfString = std::string;
#endif

class bpfExceptionBase
{
public:
  virtual ~bpfExceptionBase() {}

  virtual const bpfString& GetName() const = 0;
  virtual const bpfString& GetDescription() const = 0;
};

#endif
