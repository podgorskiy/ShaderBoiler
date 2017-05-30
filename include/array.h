// Copyright 2017 Stanislav Pidhorskyi. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once
#include <string>
#include <list>

#include "datatypes.h"

namespace sb
{
	namespace detail
	{
		template<typename T>
		inline void PropagatePointerToParent(T& var, detail::nodePtr* ptr)
		{
		}

		// Needed to propagate shell pointer through multidimentional arrays.
		template<typename T, int S>
		inline void PropagatePointerToParent(array<T, S>& var, detail::nodePtr* ptr)
		{
			var.ptrToParentSrc = ptr;
		}

		template<typename T>
		inline void PushArraySize(const T*, std::vector<int>& sizes)
		{
		}

		template<typename T, int S>
		inline void PushArraySize(const array<T, S>*, std::vector<int>& sizes)
		{
			sizes.push_back(S);
			PushArraySize((T*)(nullptr), sizes);
		}
	}

	template<typename T, int S>
	class array : public detail::typed_variable<static_cast<detail::node::DataType>(T::type), static_cast<detail::node::DataSize>(T::sizeM), static_cast<detail::node::DataSize>(T::sizeN)>
	{
		template<typename TPropagate, int SPropagate> friend void detail::PropagatePointerToParent(array<TPropagate, SPropagate>& var, detail::nodePtr* ptr);
		template<typename TPropagate> friend void detail::PropagatePointerToParent(TPropagate& var, detail::nodePtr* ptr);
	public:
		using detail::variable::src;
		using detail::variable::originalsrc;

		array(): ptrToParentSrc(&src)
		{
			src->optype = detail::node::array_declaration;
			src->arraySize.push_back(S);
			PushArraySize((T*)(nullptr), src->arraySize);
		}
		array(const std::string name) : ptrToParentSrc(&src)
		{
			src->name = name;
			src->optype = detail::node::array_declaration;
			src->arraySize.push_back(S);
			PushArraySize((T*)(nullptr), src->arraySize);
		}
		ivec1 length()
		{
			return S;
		}
		T& operator[](ivec1 i)
		{
			T* result = new T();

			PropagatePointerToParent(*result, ptrToParentSrc);

			result->src->optype = detail::node::arrayLookup;

			if (!originalsrc || src == originalsrc)
			{
				result->src->childs.push_back(src);
				originalsrc = src;
			}
			else
			{
				detail::nodePtr junction(new detail::node());

				junction->optype = detail::node::dependency;
				junction->childs.push_back(src);
				junction->childs.push_back(originalsrc);

				result->src->childs.push_back(junction);
			}

			result->src->childs.push_back(i.src);
			
			result->ptrToSrcPtr.push_back(ptrToParentSrc);

			for (std::vector<detail::nodePtr*>::iterator it = this->ptrToSrcPtr.begin(); it != this->ptrToSrcPtr.end(); ++it)
			{
				result->ptrToSrcPtr.push_back(*it);
			}

			this->garbageVars.push_back(detail::varPtr(result));
			return *result;
		}

		array<T, S>& SetName(const std::string& name)
		{
			if (originalsrc)
			{
				originalsrc->name = name;
			}
			else
			{
				src->name = name;
			}
			return *this;
		}

	private:
		detail::nodePtr* ptrToParentSrc;
	};
}
