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
#include <set>
#include <sstream>
#include <cassert>
#include <limits>
#include <iomanip>

#include "datatypes.h"
#include "detail/node.h"
#include "detail/output_proxy.h"

namespace sb
{
	class context
	{
	public:
		std::string genShader();

		template<typename T>
		T uniform(const std::string& name)
		{
			T v(name);
			v.src->optype = detail::node::io_uniform;
			return v;
		}

		template<typename T>
		T in(const std::string& name)
		{
			T v(name);
			v.src->optype = detail::node::io_input;
			return v;
		}

		template<typename T>
		T out(const std::string& name)
		{
			T v(name);
			v.src->optype = detail::node::io_output;
			detail::nodeshellPtr shell(new detail::nodeshell(v.src));
			v.shell = shell;
			targetList.push_back(shell);
			return v;
		}

		template<typename T>
		T varying(const std::string& name)
		{
			T v(name);
			v.src->optype = detail::node::io_varying;
			detail::nodeshellPtr shell(new detail::nodeshell(v.src));
			v.shell = shell;
			targetList.push_back(shell);
			return v;
		}

		template<typename T>
		T attribute(const std::string& name)
		{
			T v(name);
			v.src->optype = detail::node::io_attribute;
			return v;
		}

		template<typename T>
		T& operator [] (const detail::output_proxy<T>& l)
		{
			T* result = new T(l);
			Register(*result);
			garbageVars.push_back(detail::varPtr(result));
			return *result;
		}

	private:
		template<typename T>
		void Register(T& x)
		{
			detail::nodeshellPtr shell(new detail::nodeshell(x.src));
			x.shell = shell;
			targetList.push_back(shell);
		}

		class IndentGuard
		{
		public:
			IndentGuard(context* c) : p(++c->indent) {};
			~IndentGuard() { --p; };
		private:
			int p;
		};

		std::string GenerateCode();

		void VisitNode(detail::nodePtr n, std::list<detail::nodePtr>& defaultList);

		std::string Emit(detail::nodePtr n);

		static std::string GetType(detail::nodePtr n);

		template<typename T>
		static void PrintConstLiteral(std::stringstream& ss, detail::node::Data* d, std::string type, int size, std::string suffix = "");

		template<typename T>
		static T Get(detail::node::Data* d, int i);

		static std::string genConstant(detail::nodePtr n);

		std::list<detail::nodeshellPtr> targetList;
		std::list<detail::nodePtr> ioVariablesList;
		std::list<std::list<detail::nodePtr> > listOffunctionNodesList;
		std::list<detail::nodePtr> mainBlockList;
		std::set<detail::nodePtr> visitedNodes;
		std::list<detail::varPtr> garbageVars; // destroyed when context is destoyed
		int id = 0;
		int indent = 0;
	};

	inline std::string context::genShader()
	{
		using namespace detail;

		ioVariablesList.clear();
		listOffunctionNodesList.clear();
		mainBlockList.clear();
		visitedNodes.clear();

		// Visiting the target forest
		for (std::list<nodeshellPtr>::iterator it = targetList.begin(); it != targetList.end(); ++it)
		{
			VisitNode((*it)->n, mainBlockList);
		}

		for (std::set<nodePtr>::iterator it = visitedNodes.begin(); it != visitedNodes.end(); ++it)
		{
			(*it)->pointersTo = 0;
		}
		for (std::set<nodePtr>::iterator it = visitedNodes.begin(); it != visitedNodes.end(); ++it)
		{
			std::vector<nodePtr>& childs = (*it)->childs;
			for (std::vector<nodePtr>::iterator it = childs.begin(); it != childs.end(); ++it)
			{
				(*it)->pointersTo++;
			}
		}

		return GenerateCode();
	}

	template<>
	inline float context::Get<float>(detail::node::Data* d, int i)
	{
		return d[i].d_vec;
	}

	template<>
	inline int context::Get<int>(detail::node::Data* d, int i)
	{
		return d[i].d_ivec;
	}

	template<>
	inline unsigned int context::Get<unsigned int>(detail::node::Data* d, int i)
	{
		return d[i].d_uvec;
	}

	template<>
	inline std::string context::Get<std::string>(detail::node::Data* d, int i)
	{
		return d[i].d_bvec ? "true" : "false";
	}

	inline void context::VisitNode(detail::nodePtr n, std::list<detail::nodePtr>& defaultList)
	{
		using namespace detail;

		if (visitedNodes.find(n) == visitedNodes.end())
		{
			std::list<nodePtr>* listToUse = &defaultList;

			if (n->optype & node::io_bit)
			{
				listToUse = &ioVariablesList;
			}

			for (auto& child : n->childs)
			{
				VisitNode(child, *listToUse);
			}

			listToUse->push_back(n);
			visitedNodes.insert(n);
		}
	}

	inline std::string context::GenerateCode()
	{
		using namespace detail;
		std::stringstream ss;

		for (nodePtr n : ioVariablesList)
		{
			ss << Emit(n);
		}

		ss << "\nvoid main(void)\n{\n";
		{
			IndentGuard ig(this);

			for (nodePtr n : mainBlockList)
			{
				ss << Emit(n);
			}
		}
		ss << "}\n";

		return ss.str();
	}

	template<typename T>
	void PrepareStream(std::stringstream& ss)
	{}

	template<>
	void PrepareStream<double>(std::stringstream& ss)
	{
		ss << std::setprecision(std::numeric_limits<double>::digits10 + 1) << std::showpoint;
	}

	template<>
	void PrepareStream<float>(std::stringstream& ss)
	{
		ss << std::setprecision(std::numeric_limits<float>::digits10 + 1) << std::showpoint;
	}

	template<typename T>
	void context::PrintConstLiteral(std::stringstream& ss, detail::node::Data* d, std::string type, int size, std::string suffix)
	{
		std::ios_base::fmtflags oldFlags = ss.flags();
		PrepareStream<T>(ss);
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

	inline std::string context::genConstant(detail::nodePtr n)
	{
		using namespace detail;

		std::stringstream ss;
		switch (n->optype)
		{
		case node::floatConstant:
		{
			PrintConstLiteral<float>(ss, n->data, GetType(n), n->datasize);
		}
		break;
		case node::integerConstant:
		{
			PrintConstLiteral<int>(ss, n->data, GetType(n), n->datasize);
		}
		break;
		case node::unsignedIntegerConstant:
		{
			PrintConstLiteral<unsigned int>(ss, n->data, GetType(n), n->datasize, "u");
		}
		break;
		case node::booleanConstant:
		{
			PrintConstLiteral<std::string>(ss, n->data, GetType(n), n->datasize);
		}
		break;
		}
		return ss.str();
	}

	inline std::string context::Emit(detail::nodePtr n)
	{
		using namespace detail;

		if (node::predefined_bit & n->optype)
		{
			n->InitId(id++);
			return "";
		}

		std::stringstream ss;

		for (int i = 0; i < indent; ++i)
		{
			ss << "\t";
		}

		if (node::assign_bit & n->optype)
		{
			assert(n->childs[0]->Initialised()); // If false - using uninitialised variable
			n->CopyIdFrom(n->childs[0]); // assignment node is different from it's first argument node, but we want to preserve the same name

			ss << n->GetId();

			switch (n->optype)
			{
			case node::assign:
			{
				ss << " = " << n->childs[1]->GetId();
			}
			break;
			case node::assign_addition:
			{
				ss << " += " << n->childs[1]->GetId();
			}
			break;
			case node::assign_substruction:
			{
				ss << " -= " << n->childs[1]->GetId();
			}
			break;
			case node::assign_multiplication:
			{
				ss << " *= " << n->childs[1]->GetId();
			}
			break;
			case node::assign_division:
			{
				ss << " /= " << n->childs[1]->GetId();
			}
			break;
			case node::assign_mod:
			{
				ss << " %= " << n->childs[1]->GetId();
			}
			break;
			case node::assign_lshift:
			{
				ss << " <<= " << n->childs[1]->GetId();
			}
			break;
			case node::assign_rshift:
			{
				ss << " >>= " << n->childs[1]->GetId();
			}
			break;
			case node::assign_and:
			{
				ss << " &= " << n->childs[1]->GetId();
			}
			break;
			case node::assign_or:
			{
				ss << " |= " << n->childs[1]->GetId();
			}
			break;
			case node::assign_xor:
			{
				ss << " ^= " << n->childs[1]->GetId();
			}
			break;
			}
		}
		else if (node::io_bit & n->optype)
		{
			n->InitId(id++);

			switch (n->optype)
			{
			case node::io_input:
			{
				ss << "in " << GetType(n) << " " << n->GetId();
			}
			break;
			case node::io_output:
			{
				ss << "out " << GetType(n) << " " << n->GetId();
			}
			break;
			case node::io_uniform:
			{
				ss << "uniform " << GetType(n) << " " << n->GetId();
			}
			break;
			case node::io_attribute:
			{
				ss << "attribute " << GetType(n) << " " << n->GetId();
			}
			break;
			case node::io_varying:
			{
				ss << "varying " << GetType(n) << " " << n->GetId();
			}
			break;
			}
		}
		else
		{
			n->InitId(id++);
			ss << GetType(n) << " " << n->GetId() << " = ";

			if (n->optype & node::const_bit)
			{
				ss << genConstant(n);
			}
			else
			{
				switch (n->optype)
				{
				case node::addition:
				{
					ss << n->childs[0]->GetId() << " + " << n->childs[1]->GetId();
				}
				break;
				case node::substruction:
				{
					ss << n->childs[0]->GetId() << " - " << n->childs[1]->GetId();
				}
				break;
				case node::multiplication:
				{
					ss << n->childs[0]->GetId() << " * " << n->childs[1]->GetId();
				}
				break;
				case node::division:
				{
					ss << n->childs[0]->GetId() << " / " << n->childs[1]->GetId();
				}
				break;
				case node::lshift:
				{
					ss << n->childs[0]->GetId() << " << " << n->childs[1]->GetId();
				}
				break;
				case node::rshift:
				{
					ss << n->childs[0]->GetId() << " >> " << n->childs[1]->GetId();
				}
				break;
				case node:: or :
				{
					ss << n->childs[0]->GetId() << " | " << n->childs[1]->GetId();
				}
				break;
				case node::xor:
				{
					ss << n->childs[0]->GetId() << " ^ " << n->childs[1]->GetId();
				}
				break;
				case node::and:
				{
					ss << n->childs[0]->GetId() << " & " << n->childs[1]->GetId();
				}
				break;
				case node::lor:
				{
					ss << n->childs[0]->GetId() << " || " << n->childs[1]->GetId();
				}
				break;
				case node::lxor:
				{
					ss << n->childs[0]->GetId() << " ^^ " << n->childs[1]->GetId();
				}
				break;
				case node::land:
				{
					ss << n->childs[0]->GetId() << " && " << n->childs[1]->GetId();
				}
				break;
				case node::equal:
				{
					ss << n->childs[0]->GetId() << " == " << n->childs[1]->GetId();
				}
				break;
				case node::mod:
				{
					ss << n->childs[0]->GetId() << " % " << n->childs[1]->GetId();
				}
				break;
				case node::neg:
				{
					ss << "-" << n->childs[0]->GetId();
				}
				break;
				case node::preinc:
				{
					ss << "++" << n->childs[0]->GetId();
				}
				break;
				case node::predec:
				{
					ss << "--" << n->childs[0]->GetId();
				}
				break;
				case node::postinc:
				{
					ss << n->childs[0]->GetId() << "++";
				}
				break;
				case node::postdec:
				{
					ss << n->childs[0]->GetId() << "--";
				}
				break;
				case node::arrayLookup:
				{
					ss << n->childs[1]->GetId() << "[" << n->childs[0]->GetId() << "]";
				}
				break;
				case node::cast:
				{
					ss << GetType(n) << "(";
					ss << n->childs[0]->GetId();
					for (int i = 1; i < int(n->childs.size()); ++i)
					{
						ss << ", " << n->childs[i]->GetId();
					}
					ss << ")";
				}
				break;
				case node::functionCall:
				{
					ss << n->fname << "(";
					ss << n->childs[0]->GetId();
					for (int i = 1; i < int(n->childs.size()); ++i)
					{
						ss << ", " << n->childs[i]->GetId();
					}
					ss << ")";
				}
				break;
				}
			}
		}

		ss << ";\n";
		return ss.str();
	}

	inline std::string context::GetType(detail::nodePtr n)
	{
		using namespace detail;
		std::string size;
		switch (n->datasize)
		{
		case node::DataSize::size1: size = "1"; break;
		case node::DataSize::size2: size = "2"; break;
		case node::DataSize::size3: size = "3"; break;
		case node::DataSize::size4: size = "4"; break;
		}

		if (n->datasize == node::DataSize::size1)
		{
			switch (n->datatype)
			{
			case node::DataType::bvec: return "bool";
			case node::DataType::ivec: return "int";
			case node::DataType::uvec: return "uint";
			case node::DataType::dvec: return "double";
			case node::DataType::vec: return "float";
			}
		}

		std::string size_secondary;
		switch (n->datasize_secondary)
		{
		case node::DataSize::size1: size_secondary = "1"; break;
		case node::DataSize::size2: size_secondary = "2"; break;
		case node::DataSize::size3: size_secondary = "3"; break;
		case node::DataSize::size4: size_secondary = "4"; break;
		}

		switch (n->datatype)
		{
		case node::DataType::bvec: return "bvec" + size;
		case node::DataType::ivec: return "ivec" + size;
		case node::DataType::uvec: return "uvec" + size;
		case node::DataType::dvec: return "dvec" + size;
		case node::DataType::vec: return "vec" + size;

		case node::DataType::mat: return "mat" + size + "x" + size_secondary;
		case node::DataType::dmat: return "dmat" + size + "x" + size_secondary;
		}
		return "void";
	}
}
