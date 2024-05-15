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


#ifndef BPFSMART_PTR_HPP
#define BPFSMART_PTR_HPP

#include <memory>

#define bpfDynamicCast std::dynamic_pointer_cast
#define bpfConstCast std::const_pointer_cast
#define bpfStaticCast std::static_pointer_cast

#define bpfSharedPtr std::shared_ptr
#define bpfMakeSharedPtr std::make_shared

#define bpfUniquePtr std::unique_ptr
#define bpfMakeUniquePtr std::make_unique

#define bpfWeakPtr std::weak_ptr

#endif
