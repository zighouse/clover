/*
 *  Copyright 2011 Zhigang Xie
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  Created on: 2011-4-12
 *      Author: Zhigang Xie
 */

#ifndef CLOVERTYPES_H_
#define CLOVERTYPES_H_

namespace clover {

template < typename _T >
class type_parameter
{
public:
    typedef _T identifier;
};

#if defined(_MSC_VER)
#define FRIEND_TYPE(type) friend type
#else
#define FRIEND_TYPE(type) friend class type_parameter<type>::identifier
#endif

} // namespace clover

#endif /* CLOVERTYPES_H_ */

