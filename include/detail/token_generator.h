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
#include <map>
#include <string>
#include <sstream>
#include <limits>
#include <iomanip>

#include "node.h"

namespace sb
{
	namespace detail
	{
		class token_generator
		{
		public:
			token_generator(): invalidToken("<Invalid token. Please submit an issue.>")
			{
				binary[node::binary_addition] = " + ";
				binary[node::binary_substruction] = " - ";
				binary[node::binary_multiplication] = " * ";
				binary[node::binary_division] = " / ";
				binary[node::binary_lshift] = " << ";
				binary[node::binary_rshift] = " >> ";
				binary[node::binary_or] = " | ";
				binary[node::binary_xor] = " ^ ";
				binary[node::binary_and] = " & ";
				binary[node::binary_lor] = " || ";
				binary[node::binary_lxor] = " ^^ ";
				binary[node::binary_land] = " && ";
				binary[node::binary_equal] = " == ";
				binary[node::binary_mod] = " % ";

				preunary[node::preunary_neg] = " -";
				preunary[node::preunary_inc] = " ++";
				preunary[node::preunary_dec] = " --";

				postunary[node::postunary_postinc] = "++ ";
				postunary[node::postunary_postdec] = "-- ";

				assign[node::assign] = " = ";
				assign[node::assign_addition] = " += ";
				assign[node::assign_substruction] = " -= ";
				assign[node::assign_multiplication] = " *= ";
				assign[node::assign_division] = " /= ";
				assign[node::assign_mod] = " %= ";
				assign[node::assign_lshift] = " <<= ";
				assign[node::assign_rshift] = " >>= ";
				assign[node::assign_and] = " &= ";
				assign[node::assign_or] = " |= ";
				assign[node::assign_xor] = " ^= ";
				
				storageQualifier[node::storage_input] = "in ";
				storageQualifier[node::storage_output] = "out ";
				storageQualifier[node::storage_attribute] = "attribute ";
				storageQualifier[node::storage_uniform] = "uniform ";
				storageQualifier[node::storage_varying] = "varying ";
				storageQualifier[node::storage_buffer] = "buffer ";
				storageQualifier[node::storage_shared] = "shared ";
				
				scalarNames[node::bvec] = "bool";
				scalarNames[node::ivec] = "int";
				scalarNames[node::uvec] = "uint";
				scalarNames[node::dvec] = "double";
				scalarNames[node::vec] = "float";

				vecNames[node::bvec] = "bvec";
				vecNames[node::ivec] = "ivec";
				vecNames[node::uvec] = "uvec";
				vecNames[node::dvec] = "dvec";
				vecNames[node::vec] = "vec";
				vecNames[node::mat] = "mat";
				vecNames[node::dmat] = "dmat";
			}

			std::string GetBinaryOperator(node::OpType optype) const
			{
				return TableLookup(binary, optype);
			}

			std::string GetPreUnaryOperator(node::OpType optype) const
			{
				return TableLookup(preunary, optype);
			}

			std::string GetPostUnaryOperator(node::OpType optype) const
			{
				return TableLookup(postunary, optype);
			}

			std::string GetAssignOperator(node::OpType optype) const
			{
				return TableLookup(assign, optype);
			}

			std::string GetStorageQualifier(node::OpType optype) const
			{
				return TableLookup(storageQualifier, optype);
			}

			std::string GetType(nodePtr n) const;

			std::string GetConstantLiteral(nodePtr n) const;

		private:
			template<typename T>
			static T Get(node::Data* d, int i);

			template<typename T>
			void PrintConstLiteral(node::Data* d, std::string type, node::DataSize size, std::string suffix) const;

			template<typename T>
			void PrepareStream() const
			{};

			template <typename K>
			const std::string& TableLookup(const std::map <K, std::string>& map, K key) const
			{
				typename std::map<K, std::string>::const_iterator it = map.find(key);
				if (it == map.end())
				{
					return invalidToken;
				}
				else {
					return it->second;
				}
			}

			mutable std::stringstream ss;
			const std::string invalidToken;

			std::map<node::OpType, std::string> binary;
			std::map<node::OpType, std::string> preunary;
			std::map<node::OpType, std::string> postunary;
			std::map<node::OpType, std::string> assign;
			std::map<node::OpType, std::string> storageQualifier;
			std::map<node::DataType, std::string> scalarNames;
			std::map<node::DataType, std::string> vecNames;
		};

		template<>
		inline float token_generator::Get<float>(detail::node::Data* d, int i)
		{
			return d[i].d_vec;
		}

		template<>
		inline int token_generator::Get<int>(detail::node::Data* d, int i)
		{
			return d[i].d_ivec;
		}

		template<>
		inline unsigned int token_generator::Get<unsigned int>(detail::node::Data* d, int i)
		{
			return d[i].d_uvec;
		}

		template<>
		inline std::string token_generator::Get<std::string>(detail::node::Data* d, int i)
		{
			return d[i].d_bvec ? "true" : "false";
		}

		template<typename T>
		inline void token_generator::PrintConstLiteral(node::Data* d, std::string type, node::DataSize size, std::string suffix) const
		{
			std::ios_base::fmtflags oldFlags = ss.flags();
			PrepareStream<T>();
			if (size == 1)
			{
				ss << Get<T>(d, 0);
			}
			else
			{
				ss << type << "(";
				ss << Get<T>(d, 0);
				for (int i = 1; i < size; ++i)
				{
					ss << ", " << Get<T>(d, i) << suffix;
				}
				ss << ")";
			}
			ss.flags(oldFlags);
		}

		template<>
		inline void token_generator::PrepareStream<double>() const
		{
			ss << std::setprecision(std::numeric_limits<double>::digits10 + 1) << std::showpoint;
		}

		template<>
		inline void token_generator::PrepareStream<float>() const
		{
			ss << std::setprecision(std::numeric_limits<float>::digits10 + 1) << std::showpoint;
		}

		inline std::string token_generator::GetConstantLiteral(nodePtr n) const
		{
			ss.str(std::string());

			switch (n->optype)
			{
			case node::floatConstant:
			{
				PrintConstLiteral<float>(n->data, GetType(n), n->datasize, "");
			}
			break;
			case node::integerConstant:
			{
				PrintConstLiteral<int>(n->data, GetType(n), n->datasize, "");
			}
			break;
			case node::unsignedIntegerConstant:
			{
				PrintConstLiteral<unsigned int>(n->data, GetType(n), n->datasize, "u");
			}
			break;
			case node::booleanConstant:
			{
				PrintConstLiteral<std::string>(n->data, GetType(n), n->datasize, "");
			}
			break;
			}
			return ss.str();
		}

		inline void operator << (std::string& str, node::DataSize ds)
		{
			str = '0' + ds;
		}

		inline std::string token_generator::GetType(nodePtr n) const
		{
			using namespace detail;
			if (n->datasize == node::DataSize::size1)
			{
				return TableLookup(scalarNames, n->datatype);
			}
			std::string size;
			size << n->datasize;
			std::string size_secondary;
			size_secondary << n->datasize_secondary;

			if (n->datatype & node::mat_bit)
			{
				return TableLookup(vecNames, n->datatype) + size + "x" + size_secondary;
			}
			else
			{
				return TableLookup(vecNames, n->datatype) + size;
			}
		}
	}
}
