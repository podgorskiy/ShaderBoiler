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
			v.originalsrc = v.src;
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
		T& out(const std::string& name)
		{
			T* result = new T(name);
			result->src->optype = detail::node::storage_output;
			Register(*result);
			garbageVars.push_back(detail::varPtr(result));
			return *result;
		}

		template<typename T>
		T& varying(const std::string& name)
		{
			T* result = new T(name);
			result->src->optype = detail::node::storage_varying;
			Register(*result);
			garbageVars.push_back(detail::varPtr(result));
			return *result;
		}

		template<typename T>
		T attribute(const std::string& name)
		{
			T v(name);
			v.src->optype = detail::node::storage_attribute;
			return v;
		}

		template<typename T>
		T& buffer(const std::string& name)
		{
			T* result = new T(name);
			result->src->optype = detail::node::storage_buffer;
			Register(*result);
			garbageVars.push_back(detail::varPtr(result));
			return *result;
		}

		template<typename T>
		T& shared(const std::string& name)
		{
			T* result = new T(name);
			result->src->optype = detail::node::storage_shared;
			Register(*result);
			garbageVars.push_back(detail::varPtr(result));
			return *result;
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
			targetList.push_back(x.src);
			x.ptrToSrcPtr.push_back(&targetList.back());
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

		std::string Accept(detail::nodePtr n, detail::node::OpType parentOp = detail::node::uninitialised, bool parentOpModifies = false);
		void Emit(const std::string& str, const std::string& comment = "");

		std::string codeblock;

		detail::token_generator tokenGen;
		std::list<detail::nodePtr> targetList;
		std::list<detail::nodePtr> ioVariablesList;
		std::list<std::list<detail::nodePtr> > listOffunctionNodesList;
		std::list<detail::nodePtr> mainBlockList;
		std::set<detail::nodePtr> visitedNodes;
		std::list<detail::varPtr> garbageVars; // destroyed when context is destoyed
		int id;
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
		for (std::list<nodePtr>::iterator it = targetList.begin(); it != targetList.end(); ++it)
		{
			VisitNode(*it, mainBlockList);
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
		for (std::list<nodePtr>::iterator it = targetList.begin(); it != targetList.end(); ++it)
		{
			(*it)->pointersTo = 2;
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
	
	struct IOSortingFunctor
	{
		bool operator()(const detail::nodePtr& a, const detail::nodePtr& b) const
		{
			return (int)a->optype < (int)b->optype;
		}
	};

	inline std::string context::GenerateCode()
	{
		using namespace detail;

		visitedNodes.clear();
		codeblock.clear();
		
		id = 0;

		std::stringstream ss;

		ioVariablesList.sort(IOSortingFunctor());

		detail::node::OpType curentType = ioVariablesList.size() > 0 ? ioVariablesList.begin()->get()->optype : detail::node::storage_bit;

		for (std::list<nodePtr>::iterator it = ioVariablesList.begin(); it != ioVariablesList.end(); ++it)
		{
			visitedNodes.insert(*it);
			(*it)->InitWithIdId(id);

			std::stringstream ss;

			if ((*it)->optype != curentType)
			{
				curentType = (*it)->optype;
				ss << "\n";
			}

			ss << tokenGen.GetStorageQualifier((*it)->optype) << tokenGen.GetType(*it) << " " << (*it)->GetId();

			for (std::vector<int>::iterator size = (*it)->arraySize.begin(); size != (*it)->arraySize.end(); ++size)
			{
				ss << "[" << (*size) << "]";
			}

			Emit(ss.str());
		}

		ss << codeblock;
		codeblock.clear();

		ss << "\nvoid main(void)\n{\n";
		{
			IndentGuard ig(this);

			for (std::list<nodePtr>::iterator it = targetList.begin(); it != targetList.end(); ++it)
			{
				Accept(*it);

				ss << codeblock;
				codeblock.clear();
			}
		}
		ss << "}\n";

		targetList.clear();
		return ss.str();
	}

	inline int GetPrecedence(detail::node::OpType op)
	{
		using namespace detail;
		switch (op)
		{
		case node::arrayLookup:
		case node::postunary_postdec:
		case node::postunary_postinc:
		case node::memberAccess:
			return 2;
		case node::preunary_neg:
		case node::preunary_dec:
		case node::preunary_inc:
			return 3;
		case node::binary_multiplication:
		case node::binary_division:
		case node::binary_mod:
			return 4;
		case node::binary_addition:
		case node::binary_substruction:
			return 5;
		case node::binary_rshift:
		case node::binary_lshift:
			return 6;
		/*case node::binary_less:
		case node::binary_greater:
		case node::binary_eqless:
		case node::binary_eqgreater:
			return 7;*/
		case node::binary_equal:
		//case node::binary_nequal:
			return 8;
		case node::binary_and:
			return 9;
		case node::binary_xor:
			return 10;
		case node::binary_or:
			return 11;
		case node::binary_land:
			return 12;
		case node::binary_lxor:
			return 13;
		case node::binary_lor:
			return 14;
		//case node::binary_selection:
		//	return 15;
		}
		return 0;
	}

	inline std::string Parenthesize(const std::string& str, detail::node::OpType currentOp, detail::node::OpType parentOp)
	{
		using namespace detail;

		if ((node::assign_bit & parentOp) ||
			(node::storage_bit & currentOp) ||
			(node::constliteral_bit & currentOp) ||
			(node::functionCall == currentOp) ||
			(node::functionCall == parentOp) ||
			(node::builtin_variable == currentOp) ||
			(node::uninitialised == parentOp) ||
			(node::array_declaration == currentOp) ||
			(node::uninitialised == currentOp) ||
			(node::dependency == currentOp) ||
			(node::cast == currentOp) ||
			(node::cast == parentOp))
		{
			return str;
		}

		int p1 = GetPrecedence(currentOp);
		int p2 = GetPrecedence(parentOp);

		if (p2 < p1 || p1 == 0)
		{
			return "(" + str + ")";
		}
		else
		{
			return str;
		}
	}

	inline std::string context::Accept(detail::nodePtr n, detail::node::OpType parentOp, bool parentOpModifies)
	{
		using namespace detail;

		// Node was already visited. That means, it has to edges going to it, thus it was instantiated.
		if (n->Initialised())
		{
			return n->GetId();
		}

		if (node::assign == n->optype &&
			node::uninitialised == n->childs[0]->optype &&
			n->childs[1]->Initialised() &&
			!parentOpModifies)
		{
			return Accept(n->childs[1], parentOp);
		}

		// Assign chain contraction, while preserving assigned name.
		if (node::assign == n->optype && 
			node::uninitialised == n->childs[0]->optype &&
			node::assign == n->childs[1]->optype &&
			node::uninitialised == n->childs[1]->childs[0]->optype &&
			n->childs[1]->childs[0]->pointersTo == 1)
		{
			std::string name = n->childs[1]->childs[0]->name;			
			if (n->childs[0]->name.size() > 0)
			{
				name = n->childs[0]->name;
			}
			n->childs[1]->childs[0] = n->childs[0];
			n->childs[1]->childs[0]->name = name;
			return Accept(n->childs[1], parentOp);
		}

		std::string expression;

		if (node::assign_bit & n->optype)
		{
			// If parent node does to modify current node
			// If there is only one edge going to current node
			// If current node is assign operation
			// If parent node was not a dependecy node (all nodes that are dependencies to other nodes, must be instantiated)
			// If no name assigned to current node
			// Then current node can be converted to a temporary object, thus no instantiation happens
			if (!parentOpModifies && n->pointersTo == 1 && node::assign == n->optype && parentOp != node::dependency && n->name.size() == 0)
			{
				return Accept(n->childs[1], parentOp);
			}

			// Otherwise assignement is instantiated to:

			// A new variable, if child to which assignment happens is uninitialised
			if (n->optype == node::assign && n->childs[0]->optype == node::uninitialised)
			{
				n->childs[0]->name = n->name;
				n->childs[0]->InitWithIdId(id);
				expression = tokenGen.GetType(n->childs[0]) + " " + n->childs[0]->GetId();
				Emit(expression + tokenGen.GetAssignOperator(n->optype) + Accept(n->childs[1], n->optype));
				n->CopyIdFrom(n->childs[0]); // assignment node is different from it's first argument node, but we want to preserve the same name
				return Accept(n->childs[0]);
			}
			// Or assignment happens to already existing variable, thas a modification happens, note argument 'true' in 'Accept' call.
			else
			{
				Emit(Accept(n->childs[0], n->optype, true) + tokenGen.GetAssignOperator(n->optype) + Accept(n->childs[1], n->optype));
				n->CopyIdFrom(n->childs[0]); // assignment node is different from it's first argument node, but we want to preserve the same name
				return Accept(n->childs[0]);
			}
		}
		else if (node::binary_bit & n->optype)
		{
			expression = Accept(n->childs[0], n->optype) + tokenGen.GetBinaryOperator(n->optype) + Accept(n->childs[1], n->optype);
		}
		else if (node::preunary_bit & n->optype)
		{
			expression = tokenGen.GetPreUnaryOperator(n->optype) + Accept(n->childs[0], n->optype);
		}
		else if (node::postunary_bit & n->optype)
		{
			expression = Accept(n->childs[0], n->optype) + tokenGen.GetPreUnaryOperator(n->optype);
		}
		else if ((node::storage_bit & n->optype) || (node::builtin_variable == n->optype))
		{
			expression = n->name;
		}
		else if (node::constliteral_bit & n->optype)
		{
			expression = tokenGen.GetConstantLiteral(n);
		}
		else if (node::arrayLookup == n->optype)
		{
			expression = Accept(n->childs[0], n->optype) + "[" + Accept(n->childs[1]) + "]";
		}
		else if (node::cast == n->optype)
		{
			std::stringstream ss;
			ss << tokenGen.GetType(n) << "(";
			ss << Accept(n->childs[0], n->optype);
			for (int i = 1; i < int(n->childs.size()); ++i)
			{
				ss << ", " << Accept(n->childs[i], n->optype);
			}
			ss << ")";
			expression = ss.str();
		}
		else if (node::functionCall == n->optype)
		{
			std::stringstream ss;
			ss << n->fname << "(";
			ss << Accept(n->childs[0], n->optype);
			for (int i = 1; i < int(n->childs.size()); ++i)
			{
				ss << ", " << Accept(n->childs[i], n->optype);
			}
			ss << ")";
			expression = ss.str();
		}
		else if (node::memberAccess == n->optype)
		{
			expression = Accept(n->childs[0], n->optype, parentOpModifies) + "." + n->fname;
		}
		else if (node::uninitialised == n->optype)
		{
			n->InitWithIdId(id);
			expression = tokenGen.GetType(n) + " " + n->GetId();
			Emit(expression, "Warning, variable is used but might be unset.");
			expression = n->GetId();
		}
		else if (node::array_declaration == n->optype)
		{
			std::stringstream ss;
			n->InitWithIdId(id);
			ss << tokenGen.GetType(n) << " " << n->GetId();
			
			for (std::vector<int>::iterator it = n->arraySize.begin(); it != n->arraySize.end(); ++it)
			{
				ss << "[" << (*it) << "]";
			}

			expression = ss.str();
			Emit(expression);
			return n->GetId();
		}
		else if (node::dependency == n->optype)
		{
			Accept(n->childs[0], n->optype);
			n->childs[0]->MarkInitialised(); // needed if lhs is not named
			expression = Accept(n->childs[1]);
			n->CopyIdFrom(n->childs[1]); // assignment node is different from it's first argument node, but we want to preserve the same name
		}

		bool purge = false;

		if (n->name != "")
		{
			purge = true;
		}

		if (n->pointersTo > 1)
		{
			purge = true;
		}

		if (parentOpModifies && (node::constliteral_bit & n->optype) != 0)
		{
			purge = true;
		}

		if ((node::storage_bit & n->optype) || (node::builtin_variable == n->optype) || (node::dependency == n->optype))
		{
			purge = false;
		}

		if (purge && !n->Initialised())
		{
			n->InitWithIdId(id);
			expression = tokenGen.GetType(n) + " " + n->GetId() + " = " + expression;
			Emit(expression);
			expression = n->GetId();
		}

		return Parenthesize(expression, n->optype, parentOp);
	}

	void context::Emit(const std::string& str, const std::string& comment)
	{
		std::stringstream ss;

		for (int i = 0; i < indent; ++i)
		{
			ss << "\t";
		}

		ss << str << ";";
		
		if (comment.size() != 0)
		{
			ss << "\t\\\\ " << comment;
		}
		
		ss << "\n";
		codeblock += ss.str();
	}
}
