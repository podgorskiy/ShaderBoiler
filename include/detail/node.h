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
#include <vector>

namespace sb
{
	namespace detail
	{
		class node;

		typedef std::shared_ptr<node>nodePtr;

		class node
		{
		public:
			enum DataSize
			{
				size1 = 1,
				size2 = 2,
				size3 = 3,
				size4 = 4,
			};

			enum DataType
			{
				bvec,
				ivec,
				uvec,
				dvec,
				vec,
				mat,
				dmat,
			};

			enum OpType
			{
				uninitialised,
				addition,
				substruction,
				multiplication,
				division,
				lshift,
				rshift,
				or ,
				xor,
				and,
				lor,
				lxor,
				land,
				equal,
				mod,
				neg,
				preinc,
				predec,
				postinc,
				postdec,
				cast,
				functionCall,
				arrayLookup,

				const_bit = 0x1000,
				floatConstant,
				integerConstant,
				unsignedIntegerConstant,
				booleanConstant,

				ovec = floatConstant,
				oivec = integerConstant,
				ouvec = unsignedIntegerConstant,
				obvec = booleanConstant,

				assign_bit = const_bit << 1,
				assign,
				assign_addition,
				assign_substruction,
				assign_multiplication,
				assign_division,
				assign_mod,
				assign_lshift,
				assign_rshift,
				assign_and,
				assign_or,
				assign_xor,

				io_bit = assign_bit << 1,
				io_input,
				io_output,
				io_uniform,
				io_attribute,
				io_varying,

				predefined_bit = io_bit << 1,
				predefined_const,
				predefined_input,
				predefined_output,
			};

			union Data
			{
				float d_vec;
				int d_ivec;
				unsigned int d_uvec;
				bool d_bvec;
			};

			bool Initialised()
			{
				return id != -1;
			}

			std::string GetId() const
			{
				if (name.size() == 0)
				{
					int base = 'z' - 'a' + 1;

					int n = id;

					if (n == 0)
					{
						return "a";
					}

					std::string prefix;
					while (n != 0)
					{
						int a = n % base;
						n /= base;
						prefix += 'a' + char(a);
					}
					return prefix;
				}
				else
				{
					return name;
				}
			}

			void InitId(int id)
			{
				this->id = id;
			}

			void CopyIdFrom(nodePtr other)
			{
				id = other->id;
				name = other->name;
			}

			OpType optype = uninitialised;
			std::vector<nodePtr> childs;
			std::string name;
			std::string fname;
			Data data[16];
			DataType datatype;
			DataSize datasize;
			DataSize datasize_secondary;
			int pointersTo = 0;

		private:
			int id = -1;
		};

		struct nodeshell;

		typedef std::weak_ptr<nodeshell> nodeshellWeakPtr;
		typedef std::shared_ptr<nodeshell> nodeshellPtr;

		struct nodeshell
		{
			nodeshell(nodePtr n) : n(n)
			{}

			nodePtr n;
		};
	}
}
