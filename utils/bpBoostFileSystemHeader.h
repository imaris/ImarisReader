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


#if (defined(__GNUC__) || defined(__GNUG__)) && !defined(__clang__)
#pragma GCC system_header

#elif defined _MSC_VER
#pragma warning(push, 1)

#elif defined(__clang__)
#pragma clang diagnostic push
#pragma clang system_header

#endif

#ifndef Q_MOC_RUN
#include "boost/filesystem.hpp"
#endif

#if defined _MSC_VER
#pragma warning(pop)
#endif
