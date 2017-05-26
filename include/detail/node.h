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
#include <cassert>

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
				mat_bit = 0x10,
				mat,
				dmat,
			};

			enum OpType
			{
				bit_table = 0x1000,
				table_mask = ~(bit_table - 1),

				uninitialised = 0,

				cast,
				functionCall,
				arrayLookup,
				builtin_variable,
				array_declaration,
				dependency,
				memberAccess,

				binary_bit = bit_table << 0,
				binary_addition,
				binary_substruction,
				binary_multiplication,
				binary_division,
				binary_lshift,
				binary_rshift,
				binary_or,
				binary_xor,
				binary_and,
				binary_lor,
				binary_lxor,
				binary_land,
				binary_equal,
				binary_mod,

				preunary_bit = bit_table << 1,
				preunary_neg,
				preunary_inc,
				preunary_dec,

				postunary_bit = bit_table << 2,
				postunary_postinc,
				postunary_postdec,

				constliteral_bit = bit_table << 3,
				floatConstant,
				integerConstant,
				unsignedIntegerConstant,
				booleanConstant,

				ovec = floatConstant,
				oivec = integerConstant,
				ouvec = unsignedIntegerConstant,
				obvec = booleanConstant,

				assign_bit = bit_table << 4,
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

				storage_bit = bit_table << 5,
				storage_uniform,
				storage_attribute,
				storage_varying,
				storage_input,
				storage_output,
				storage_buffer,
				storage_shared,
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
				return initialised;
			}

			std::string GetId() const
			{
				if (name.size() == 0)
				{
					std::string prefix = "sb_";

					int base = 'z' - 'a' + 1;

					int n = id;

					if (n == 0)
					{
						return prefix + "a";
					}

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

			void InitWithIdId(int& id)
			{
				if (!initialised)
				{
					if (name.size() == 0)
					{
						this->id = id++;
					}
					initialised = true;
				}
				else
				{
					//assert(false);
				}
			}

			void MarkInitialised()
			{
				initialised = true;
			}

			void CopyIdFrom(nodePtr other)
			{
				id = other->id;
				name = other->name;
				initialised = true;
			}

			OpType optype = uninitialised;
			std::vector<nodePtr> childs;
			std::string name;
			std::string fname;
			Data data[16];
			DataType datatype;
			DataSize datasize;
			DataSize datasize_secondary;
			std::vector<int> arraySize;
			int pointersTo = 0;

		private:
			bool initialised = false;
			int id = -1;
		};
	}
}
