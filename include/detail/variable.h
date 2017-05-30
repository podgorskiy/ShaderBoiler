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
#include <memory>
#include <string>
#include <list>

#include "node.h"

namespace sb
{
	template<typename T, int S>
	class array;

	namespace detail
	{
		template<typename T1, typename T2> T1& GenerateSwizzle(T2*, const char*);

		class variable
		{
		public:
			variable() : src(nodePtr(new node()))
			{}

			// Pointer to a node in compute graph
			nodePtr src;

			// Pointer to a node in compute graph, which is not overwritten by references
			nodePtr originalsrc;

			// This container of pointers is not empty only for reference variables (output variable, return of subscript operator on array, etc.). 
			// It is a pointer to the smartpointger to the node in compute graph, which is used to replace the pointer to the node if the reference to the variable was assigned to.
			// Can contain more than one pointer in can of array which is also output variable.
			std::vector<nodePtr*> ptrToSrcPtr;
		};

		typedef std::shared_ptr<variable> varPtr;

		template<node::DataType T, node::DataSize S, node::DataSize S2 = node::size1>
		class typed_variable : public variable
		{
			template<typename ArrayT, int ArrayS> friend class ::sb::array;
			template<typename T1, typename T2> friend T1& ::sb::detail::GenerateSwizzle(T2*, const char*);
		public:
			typed_variable()
			{
				Init();
			}

			typed_variable(const std::string& name)
			{
				Init();
				src->name = name;
			}

			typed_variable(const std::string& name, node::OpType t)
			{
				Init();
				src->name = name;
				src->optype = t;
			}

			enum
			{
				type = T,
				sizeM = S,
				sizeN = S2
			};

		protected:

			void Init()
			{
				src->datatype = T;
				src->datasize = S;
				src->datasize_secondary = S2;
			}

			std::list<detail::varPtr> garbageVars; // destroyed when array is destoyed
		};

		// Free function to access node from pointer to variable object. 
		// Needed to work objects of with declarated, but not defined classes derivated from typed_variable
		template<typename T>
		inline nodePtr GetPtr(T& x)
		{
			return x.src;
		}
	}
}
