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

#include "datatypes.h"
#include "detail/node.h"
#include "detail/output_proxy.h"
#include "detail/token_generator.h"

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
			v.src->optype = detail::node::storage_uniform;
			return v;
		}

		template<typename T>
		T in(const std::string& name)
		{
			T v(name);
			v.src->optype = detail::node::storage_input;
			return v;
		}

		template<typename T>
		T out(const std::string& name)
		{
			T v(name);
			v.src->optype = detail::node::storage_output;
			detail::nodeshellPtr shell(new detail::nodeshell(v.src));
			v.shell = shell;
			targetList.push_back(shell);
			return v;
		}

		template<typename T>
		T varying(const std::string& name)
		{
			T v(name);
			v.src->optype = detail::node::storage_varying;
			detail::nodeshellPtr shell(new detail::nodeshell(v.src));
			v.shell = shell;
			targetList.push_back(shell);
			return v;
		}

		template<typename T>
		T attribute(const std::string& name)
		{
			T v(name);
			v.src->optype = detail::node::storage_attribute;
			return v;
		}

		template<typename T>
		T buffer(const std::string& name)
		{
			T v(name);
			v.src->optype = detail::node::storage_buffer;
			return v;
		}

		template<typename T>
		T shared(const std::string& name)
		{
			T v(name);
			v.src->optype = detail::node::storage_shared;
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

		detail::token_generator tokenGen;
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

	inline void context::VisitNode(detail::nodePtr n, std::list<detail::nodePtr>& defaultList)
	{
		using namespace detail;

		if (visitedNodes.find(n) == visitedNodes.end())
		{
			std::list<nodePtr>* listToUse = &defaultList;

			if (n->optype & node::storage_bit)
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

	inline std::string context::Emit(detail::nodePtr n)
	{
		using namespace detail;

		if (node::builtin_variable == n->optype)
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
			n->CopyIdFrom(n->childs[0]);         // assignment node is different from it's first argument node, but we want to preserve the same name
			ss << n->GetId() << tokenGen.GetAssignOperator(n->optype) << n->childs[1]->GetId();
		}
		else if (node::storage_bit & n->optype)
		{
			n->InitId(id++);
			ss << tokenGen.GetStorageQualifier(n->optype) << tokenGen.GetType(n) << " " << n->GetId();
		}
		else if (n->optype & node::constliteral_bit)
		{
			n->InitId(id++);
			ss << tokenGen.GetType(n) << " " << n->GetId() << " = " << tokenGen.GetConstantLiteral(n);
		}
		else if (n->optype & node::binary_bit)
		{
			n->InitId(id++);
			ss << tokenGen.GetType(n) << " " << n->GetId() << " = " << n->childs[0]->GetId() << tokenGen.GetBinaryOperator(n->optype) << n->childs[1]->GetId();
		}
		else if (n->optype & node::preunary_bit)
		{
			n->InitId(id++);
			ss << tokenGen.GetType(n) << " " << n->GetId() << " = " << tokenGen.GetPreUnaryOperator(n->optype) << n->childs[0]->GetId();
		}
		else if (n->optype & node::postunary_bit)
		{
			n->InitId(id++);
			ss << tokenGen.GetType(n) << " " << n->GetId() << " = " << n->childs[0]->GetId() << tokenGen.GetPostUnaryOperator(n->optype);
		}
		else if (n->optype == node::arrayLookup)
		{
			n->InitId(id++);
			ss << tokenGen.GetType(n) << " " << n->GetId() << " = " << n->childs[1]->GetId() << "[" << n->childs[0]->GetId() << "]";
		}
		else if (n->optype == node::cast)
		{
			n->InitId(id++);
			ss << tokenGen.GetType(n) << " " << n->GetId() << " = " << tokenGen.GetType(n) << "(";
			ss << n->childs[0]->GetId();
			for (int i = 1; i < int(n->childs.size()); ++i)
			{
				ss << ", " << n->childs[i]->GetId();
			}
			ss << ")";
		}
		else if (n->optype == node::functionCall)
		{
			n->InitId(id++);
			ss << tokenGen.GetType(n) << " " << n->GetId() << " = " << n->fname << "(";
			ss << n->childs[0]->GetId();
			for (int i = 1; i < int(n->childs.size()); ++i)
			{
				ss << ", " << n->childs[i]->GetId();
			}
			ss << ")";
		}
		
		ss << ";\n";
		return ss.str();
	}
}
