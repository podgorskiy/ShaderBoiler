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
#include <sstream>
#include <memory>
#include <set>
#include <list>
#include <vector>
#include <cassert>
#include <limits>
#include <iomanip>

namespace sb
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
			floatConstant,
			integerConstant,
			unsignedIntegerConstant,
			booleanConstant,
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

			ovec = floatConstant,
			oivec = integerConstant,
			ouvec = unsignedIntegerConstant,
			obvec = booleanConstant,

			assign_bit = 0x1000,
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

			predefined = io_bit << 1,
			predefined_const,
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
		Data data[16];
		DataType datatype;
		DataSize datasize;
		DataSize datasize_secondary;

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

	template<node::DataType T, node::DataSize S, node::DataSize S2 = node::size1>
	class basevar
	{
	public:
		basevar()
		{
			Init();
		}

		basevar(const std::string& name)
		{
			Init();
			src->name = name;
		}

		basevar(const std::string& name, node::OpType t)
		{
			Init();
			src->name = name;
			src->optype = t;
		}

		// Pointer to a node in compute graph
		nodePtr src;

		// This pointer is not null only for output variables. 
		// It is a pointer to the record (which is a pointer to the node) inside the context, which is used to notify context if the variable was modified. 
		nodeshellWeakPtr shell;
	private:
		void Init()
		{
			src = nodePtr(new node());
			src->datatype = T;
			src->datasize = S;
			src->datasize_secondary = S2;
		}
	};

	namespace plane_types
	{
		typedef float vec;
		typedef int ivec;
		typedef unsigned int uvec;
		typedef bool bvec;
	}

// All this could be templates, not macroses. But sadly, class template arguments deduction was introduced only in C++17. To have implicit type conversion, argument deduction is needed.
// Once C++17 is supported by all major compilers, this should be rewritten

#define REPEAT_WITH_ID_AND_COMMA(X, C) REPEAT_IC##C(X)
#define REPEAT_IC1(X) X##0
#define REPEAT_IC2(X) REPEAT_IC1(X), X##1
#define REPEAT_IC3(X) REPEAT_IC2(X), X##2
#define REPEAT_IC4(X) REPEAT_IC3(X), X##3
#define REPEAT_IC5(X) REPEAT_IC4(X), X##4
#define REPEAT_IC6(X) REPEAT_IC5(X), X##5
#define REPEAT_IC7(X) REPEAT_IC6(X), X##6
#define REPEAT_IC8(X) REPEAT_IC7(X), X##7
#define REPEAT_IC9(X) REPEAT_IC8(X), X##8
#define REPEAT_IC10(X) REPEAT_IC9(X), X##9
#define REPEAT_IC11(X) REPEAT_IC10(X), X##10
#define REPEAT_IC12(X) REPEAT_IC11(X), X##11
#define REPEAT_IC13(X) REPEAT_IC12(X), X##12
#define REPEAT_IC14(X) REPEAT_IC13(X), X##13
#define REPEAT_IC15(X) REPEAT_IC14(X), X##14
#define REPEAT_IC16(X) REPEAT_IC15(X), X##15

#define REPEAT_ASSIGNMENT_WITH_ID(X1, X2, X3, C) REPEAT_AWID##C(X1, X2, X3)
#define REPEAT_AWID1(X1, X2, X3) X1##0##X2 = X3##0;
#define REPEAT_AWID2(X1, X2, X3) REPEAT_AWID1(X1, X2, X3)  X1##1##X2 = X3##1;
#define REPEAT_AWID3(X1, X2, X3) REPEAT_AWID2(X1, X2, X3)  X1##2##X2 = X3##2;
#define REPEAT_AWID4(X1, X2, X3) REPEAT_AWID3(X1, X2, X3)  X1##3##X2 = X3##3;
#define REPEAT_AWID5(X1, X2, X3) REPEAT_AWID4(X1, X2, X3)  X1##4##X2 = X3##4;
#define REPEAT_AWID6(X1, X2, X3) REPEAT_AWID5(X1, X2, X3)  X1##5##X2 = X3##5;
#define REPEAT_AWID7(X1, X2, X3) REPEAT_AWID6(X1, X2, X3)  X1##6##X2 = X3##6;
#define REPEAT_AWID8(X1, X2, X3) REPEAT_AWID7(X1, X2, X3)  X1##7##X2 = X3##7;
#define REPEAT_AWID9(X1, X2, X3) REPEAT_AWID8(X1, X2, X3)  X1##8##X2 = X3##8;
#define REPEAT_AWID10(X1, X2, X3) REPEAT_AWID9(X1, X2, X3)  X1##9##X2 = X3##9;
#define REPEAT_AWID11(X1, X2, X3) REPEAT_AWID10(X1, X2, X3)  X1##10##X2 = X3##10;
#define REPEAT_AWID12(X1, X2, X3) REPEAT_AWID11(X1, X2, X3)  X1##11##X2 = X3##11;
#define REPEAT_AWID13(X1, X2, X3) REPEAT_AWID12(X1, X2, X3)  X1##12##X2 = X3##12;
#define REPEAT_AWID14(X1, X2, X3) REPEAT_AWID13(X1, X2, X3)  X1##13##X2 = X3##13;
#define REPEAT_AWID15(X1, X2, X3) REPEAT_AWID14(X1, X2, X3)  X1##14##X2 = X3##14;
#define REPEAT_AWID16(X1, X2, X3) REPEAT_AWID15(X1, X2, X3)  X1##15##X2 = X3##15;

#define REPEAT_ASSIGNMENT(X1, X2, X3, C) REPEAT_A##C(X1, X2, X3)
#define REPEAT_A1(X1, X2, X3) X1##0##X2 = X3;
#define REPEAT_A2(X1, X2, X3) REPEAT_A1(X1, X2, X3)  X1##1##X2 = X3;
#define REPEAT_A3(X1, X2, X3) REPEAT_A2(X1, X2, X3)  X1##2##X2 = X3;
#define REPEAT_A4(X1, X2, X3) REPEAT_A3(X1, X2, X3)  X1##3##X2 = X3;
#define REPEAT_A5(X1, X2, X3) REPEAT_A4(X1, X2, X3)  X1##4##X2 = X3;
#define REPEAT_A6(X1, X2, X3) REPEAT_A5(X1, X2, X3)  X1##5##X2 = X3;
#define REPEAT_A7(X1, X2, X3) REPEAT_A6(X1, X2, X3)  X1##6##X2 = X3;
#define REPEAT_A8(X1, X2, X3) REPEAT_A7(X1, X2, X3)  X1##7##X2 = X3;
#define REPEAT_A9(X1, X2, X3) REPEAT_A8(X1, X2, X3)  X1##8##X2 = X3;
#define REPEAT_A10(X1, X2, X3) REPEAT_A9(X1, X2, X3)  X1##9##X2 = X3;
#define REPEAT_A11(X1, X2, X3) REPEAT_A10(X1, X2, X3)  X1##10##X2 = X3;
#define REPEAT_A12(X1, X2, X3) REPEAT_A11(X1, X2, X3)  X1##11##X2 = X3;
#define REPEAT_A13(X1, X2, X3) REPEAT_A12(X1, X2, X3)  X1##12##X2 = X3;
#define REPEAT_A14(X1, X2, X3) REPEAT_A13(X1, X2, X3)  X1##13##X2 = X3;
#define REPEAT_A15(X1, X2, X3) REPEAT_A14(X1, X2, X3)  X1##14##X2 = X3;
#define REPEAT_A16(X1, X2, X3) REPEAT_A15(X1, X2, X3)  X1##15##X2 = X3;
	
#define MULL(X, Y) MULL##X##Y
#define MULL11 1
#define MULL12 2
#define MULL13 3
#define MULL14 4
#define MULL21 2
#define MULL22 4
#define MULL23 6
#define MULL24 8
#define MULL31 3
#define MULL32 6
#define MULL33 9
#define MULL34 12
#define MULL41 4
#define MULL42 8
#define MULL43 12
#define MULL44 16

	// Free function to access node from pointer to variable object. 
	// Needed to work objects of with declarated, but not defined classes derivated from basevar
	template<typename T>
	inline nodePtr GetPtr(T* x)
	{
		return x->src;
	}

#define binop(T1, T2, T3, X, E)\
	/* Binary operator, like T3 a = T1() * T2(); */ \
	/* Outside class definition */ \
	T3 operator X (const T1& a, const T2& b) { \
		T3 result; \
		result.src->optype = node::##E; \
		result.src->childs.push_back(a.src); \
		result.src->childs.push_back(b.src); \
		return result; \
	}

#define assignop(T1, T2, X, E)\
	/* Assign operator, like T1 a; a += T2(); */ \
	/* Outside class definition */ \
	T1 operator X (T1&a, const T2& b) { \
		T1 result; \
		result.src->optype = node::##E; \
		result.src->childs.push_back(a.src); \
		result.src->childs.push_back(b.src); \
		a.src = result.src; \
		if (!a.shell.expired()) { \
			a.shell.lock()->n = a.src; \
		} \
		return a; \
	}

#define simple_assignop(T1, T2, S)\
	/* Assign operator, like T1 a; a = T2(); Not trivial for input-output variables */ \
	/* Inside class definition */ \
	T1##S operator = (const T2##S& x) { \
		bool io_node = (src->optype & node::io_bit) != 0; \
		bool io_assign_node = (src->childs.size() > 0) && (src->childs[0]->optype & node::io_bit) != 0; \
		if (io_node || io_assign_node) {\
			T1##S result; \
			nodePtr oldsrc; \
			if (io_assign_node) {\
				oldsrc = src->childs[0]; \
			} else {\
				oldsrc = src; \
			} \
			src = result.src; \
			src->optype = node::assign; \
			src->childs.push_back(oldsrc); \
			src->childs.push_back(x.src); \
			if (!shell.expired()) { \
				shell.lock()->n = src; \
			} \
			return *this; \
		} else { \
			src = x.src; \
			return *this; \
		} \
	}

#define op_ariphm(T1, T2, T3)\
	binop(T1, T2, T3, +, addition); \
	binop(T1, T2, T3, -, substruction); \
	binop(T1, T2, T3, *, multiplication); \
	binop(T1, T2, T3, /, division); \

#define assigop_ariphm(T1, T2)\
	assignop(T1, T2, +=, assign_addition); \
	assignop(T1, T2, -=, assign_substruction); \
	assignop(T1, T2, *=, assign_multiplication); \
	assignop(T1, T2, /=, assign_division); \

#define op_mod(T1, T2, T3) \
	binop(T1, T2, T3, %, mod);

#define assigop_mod(T1, T2) \
	assignop(T1, T2, %=, assign_mod); \

#define op_shifts(T1, T2)\
	binop(T1, T2, T1, <<, lshift); \
	binop(T1, T2, T1, >>, rshift); \
	assignop(T1, T2, <<=, assign_lshift); \
	assignop(T1, T2, >>=, assign_rshift); \

#define op_bitop(T1, T2, T3)\
	binop(T1, T2, T3, &, and); \
	binop(T1, T2, T3, |, or); \
	binop(T1, T2, T3, ^, xor);

#define assigop_bitop(T1, T2)\
	assignop(T1, T2, &=, assign_and); \
	assignop(T1, T2, |=, assign_or); \
	assignop(T1, T2, ^=, assign_xor);

// Sets collection of typecast operators to T type
#define type_cast(T, S) type_cast_to_##T(S)

#define type_cast_to_vec(S) type_cast_from(vec, uvec, S) type_cast_from(vec, ivec, S) type_cast_from(vec, bvec, S)
#define type_cast_to_uvec(S) type_cast_from(uvec, vec, S) type_cast_from(uvec, ivec, S) type_cast_from(uvec, bvec, S)
#define type_cast_to_ivec(S) type_cast_from(ivec, uvec, S) type_cast_from(ivec, vec, S) type_cast_from(ivec, bvec, S)
#define type_cast_to_bvec(S) type_cast_from(bvec, uvec, S) type_cast_from(bvec, ivec, S) type_cast_from(bvec, vec, S)

#define type_cast_from(T, T_from, S) \
		/* Casts from type T_from to T, element-wise */ \
		explicit T##S(const T_from##S& f) { \
			src->optype = node::cast; \
			src->childs.push_back(GetPtr(&f)); \
		};

#define cast_from_scalar(T, S) \
		/* Initializes each component of the vec<S> with the one argument of vec1 type */ \
		explicit T##S(T##1 f) { \
			src->optype = node::cast; \
			src->childs.push_back(f.src); \
		};

#define cast_from_vec_and_vec(T, S, V1, V2) \
		/* Initializes each component of the vec<S> with the one argument of vec1 type */ \
		explicit T##S(T##V1 v1, T##V2 v2) { \
			src->optype = node::cast; \
			src->childs.push_back(v1.src); \
			src->childs.push_back(v2.src); \
		};

#define cast_from_vec_and_vec_and_vec(T, S, V1, V2, V3) \
		/* Initializes each component of the vec<S> with the one argument of vec1 type */ \
		explicit T##S(T##V1 v1, T##V2 v2, T##V3 v3) { \
			src->optype = node::cast; \
			src->childs.push_back(v1.src); \
			src->childs.push_back(v2.src); \
			src->childs.push_back(v3.src); \
		};

#define drop_cast(T, S, V) \
		/* Initializes each component of the vec<S> with the one argument of vec1 type */ \
		explicit T##S(const T##V& v) { \
			src->optype = node::cast; \
			src->childs.push_back(GetPtr(&v)); \
		};

#define cast_from_const_literal_scalar(T, S) \
		/* Initializes each component of the vec<S> with the one argument of POD type*/ \
		explicit T##S(plane_types::##T f) {\
			REPEAT_ASSIGNMENT(src->data[, ].d_##T, f, S); \
			src->optype = node::OpType::o##T; \
		};

#define main_constructor(T, S) \
		/* Initializes each component of the vec<S> with the S arguments of POD type*/ \
		T##S(REPEAT_WITH_ID_AND_COMMA(plane_types::##T f, S)) \
		{ \
			REPEAT_ASSIGNMENT_WITH_ID(src->data[, ].d_##T, f, S); \
			src->optype = node::OpType::o##T; \
		};

#define default_constructors(T, S) \
		T##S() {}; \
		T##S(const std::string& name) : basevar(name) {}; \
		T##S(const std::string& name, node::OpType t) : basevar(name, t) {}; \
		T##S& SetName(const std::string& name) { \
			src->name = name; \
			return *this; \
		}

#define class_vec_dec_size(T, S) class T##S;

#define class_vec_def_size1(T) \
	/* All vectors of size 1: vec1, ivec1, uvec1, bvec1, dvec1*/ \
	class T##1: public basevar<node::DataType::##T, node::DataSize(1)>{ \
	public: \
		/* Default constructor */ \
		default_constructors(T, 1) \
		drop_cast(T, 1, 4); \
		drop_cast(T, 1, 3); \
		drop_cast(T, 1, 2); \
		type_cast(T, 1); \
		simple_assignop(T, T, 1); \
		/* Constructor from one argument of POD type*/ \
		T##1(plane_types::##T f0) { src->data[0].d_##T = f0; src->optype = node::OpType::o##T; }; \
	};

#define class_vec_def_size2(T) \
	/* All vectors of size 2: vec2, ivec2, uvec2, bvec2, dvec2*/ \
	class T##2: public basevar<node::DataType::##T, node::DataSize(2)>{ \
	public: \
		default_constructors(T, 2) \
		cast_from_scalar(T, 2); \
		drop_cast(T, 2, 4); \
		drop_cast(T, 2, 3); \
		main_constructor(T, 2); \
		type_cast(T, 2); \
		simple_assignop(T, T, 2); \
		cast_from_const_literal_scalar(T, 2); \
	};

#define class_vec_def_size3(T) \
	/* All vectors of size 3: vec3, ivec3, uvec3, bvec3, dvec3*/ \
	class T##3: public basevar<node::DataType::##T, node::DataSize(3)>{ \
	public: \
		default_constructors(T, 3) \
		cast_from_scalar(T, 3); \
		cast_from_vec_and_vec(T, 3, 2, 1); \
		cast_from_vec_and_vec(T, 3, 1, 2); \
		drop_cast(T, 3, 4); \
		type_cast(T, 3); \
		simple_assignop(T, T, 3); \
		main_constructor(T, 3); \
		cast_from_const_literal_scalar(T, 3); \
	};

#define class_vec_def_size4(T) \
	/* All vectors of size 4: vec4, ivec4, uvec4, bvec4, dvec4*/ \
	class T##4: public basevar<node::DataType::##T, node::DataSize(4)>{ \
	public: \
		default_constructors(T, 4) \
		cast_from_scalar(T, 4); \
		cast_from_vec_and_vec(T, 4, 3, 1); \
		cast_from_vec_and_vec(T, 4, 2, 2); \
		cast_from_vec_and_vec(T, 4, 1, 3); \
		cast_from_vec_and_vec_and_vec(T, 4, 1, 1, 2); \
		cast_from_vec_and_vec_and_vec(T, 4, 1, 2, 1); \
		cast_from_vec_and_vec_and_vec(T, 4, 2, 1, 1); \
		type_cast(T, 4); \
		simple_assignop(T, T, 4); \
		main_constructor(T, 4); \
		cast_from_const_literal_scalar(T, 4); \
	};

#define class_mat_def_(T, PT, M, N, MbyN) \
	class T##M##x##N: public basevar<node::DataType::##T, node::DataSize(M), node::DataSize(N)>{ \
	public: \
		T##M##x##N() {}; \
		T##M##x##N(plane_types::##PT f) { REPEAT_ASSIGNMENT(src->data[(N + 1) *, ].d_##PT, f, M); src->optype = node::OpType::o##PT; }; \
		T##M##x##N(REPEAT_WITH_ID_AND_COMMA(plane_types::##PT f, MbyN)) { REPEAT_ASSIGNMENT_WITH_ID(src->data[, ].d_##PT, f, MbyN); src->optype = node::OpType::o##PT; }; \
	};
#define class_mat_def(T, PT, M, N) class_mat_def_(T, PT, M, N, MULL(M, N))

#define class_vec_dec(T) \
	class_vec_dec_size(T, 1) \
	class_vec_dec_size(T, 2) \
	class_vec_dec_size(T, 3) \
	class_vec_dec_size(T, 4)

#define class_vec_def(T) \
	class_vec_def_size1(T) \
	class_vec_def_size2(T) \
	class_vec_def_size3(T) \
	class_vec_def_size4(T)

/* Class declaration section.
 * Declarations needed for drop cast operators, like vec2(vec4(1.0))
 */
	class_vec_dec(vec);
	class_vec_dec(ivec);
	class_vec_dec(uvec);
	class_vec_dec(bvec);

/* Class definition section.
 * Actual definitions of classes, with all member operator functions are here
 * Memeber operator functions are mostly casts.
 */
	class_vec_def(vec);
	class_vec_def(ivec);
	class_vec_def(uvec);
	class_vec_def(bvec);

	class_mat_def(mat, vec, 2, 2);
	class_mat_def(mat, vec, 3, 3);
	class_mat_def(mat, vec, 4, 4);

	typedef mat2x2 mat2;
	typedef mat3x3 mat3;
	typedef mat4x4 mat4;

	typedef vec1 Float;
	typedef ivec1 Int;
	typedef uvec1 UInt;
	typedef bvec1 Bool;

#define op_ariphm_allvecTypes(S1, S2, S3) \
	op_ariphm(vec##S1, vec##S2, vec##S3) \
	op_ariphm(bvec##S1, bvec##S2, bvec##S3) \
	op_ariphm(ivec##S1, ivec##S2, ivec##S3) \
	op_ariphm(uvec##S1, uvec##S2, uvec##S3)
	
#define assigop_ariphm_allvecTypes(S1, S2)\
	assigop_ariphm(vec##S1, vec##S2) \
	assigop_ariphm(bvec##S1, bvec##S2) \
	assigop_ariphm(ivec##S1, ivec##S2) \
	assigop_ariphm(uvec##S1, uvec##S2)

/* Non-member operator functions section
 * Actual definitions of classes, with all member operator functions are here
 * Non-memeber operator functions are the majority of operators, including binary operators.
 */
	op_ariphm_allvecTypes(1, 1, 1);
	op_ariphm_allvecTypes(2, 2, 2);
	op_ariphm_allvecTypes(3, 3, 3);
	op_ariphm_allvecTypes(4, 4, 4);

	assigop_ariphm_allvecTypes(1, 1);
	assigop_ariphm_allvecTypes(2, 2);
	assigop_ariphm_allvecTypes(3, 3);
	assigop_ariphm_allvecTypes(4, 4);

	// One operand is a scalar, and the other is a vector or matrix. In this case, the scalar operation is 
	// applied independently to each component of the vector or matrix, resulting in the same size vector
	// or matrix
	op_ariphm_allvecTypes(1, 2, 2);
	op_ariphm_allvecTypes(1, 3, 3);
	op_ariphm_allvecTypes(1, 4, 4);

	op_ariphm_allvecTypes(2, 1, 2);
	op_ariphm_allvecTypes(3, 1, 3);
	op_ariphm_allvecTypes(4, 1, 4);

	assigop_ariphm_allvecTypes(2, 1);
	assigop_ariphm_allvecTypes(3, 1);
	assigop_ariphm_allvecTypes(4, 1);

	//op_ariphm(mat2, mat2, mat2);
	//op_ariphm(mat2, mat2, mat2);
	
	binop(bvec1, bvec1, bvec1, &&, land);
	binop(bvec1, bvec1, bvec1, ||, lor);
	//binop(bvec1, bvec1, bvec1, ^^, lxor); // operator not supported by c++

	bvec1 operator<(const vec1& a, const vec1& b)
	{
		return bvec1();
	}

#undef binop
#undef class_def_size1
#undef class_def_size2
#undef class_def_size3
#undef class_def_size4
#undef class_def
#undef op_ariphm
#undef op_ariphm_allTypes
#undef op_bitop
#undef op_shifts

	class context
	{
	public:
		std::string genShader();

		template<typename T>
		T uniform(const std::string& name)
		{
			T v(name);
			v.src->optype = node::io_uniform;
			return v;
		}

		template<typename T>
		T in(const std::string& name)
		{
			T v(name);
			v.src->optype = node::io_input;
			return v;
		}

		template<typename T>
		T out(const std::string& name)
		{
			T v(name);
			v.src->optype = node::io_output;
			nodeshellPtr shell(new nodeshell(v.src));
			v.shell = shell;
			targetList.push_back(shell);
			return v;
		}

		template<typename T>
		T varying(const std::string& name)
		{
			T v(name);
			v.src->optype = node::io_varying;
			nodeshellPtr shell(new nodeshell(v.src));
			v.shell = shell;
			targetList.push_back(shell);
			return v;
		}

		template<typename T>
		T attribute(const std::string& name)
		{
			T v(name);
			v.src->optype = node::io_attribute;
			return v;
		}

	private:
		class IndentGuard
		{
		public:
			IndentGuard(context* c) : p(++c->indent) {};
			~IndentGuard() { --p; };
		private:
			int p;
		};

		std::string GenerateCode();

		void VisitNode(nodePtr n);
		void VisitNodeInternal(nodePtr n, std::list<nodePtr>& defaultList);

		std::string Emit(nodePtr n);

		static std::string GetType(nodePtr n);

		template<typename T>
		static void PrintConstLiteral(std::stringstream& ss, node::Data* d, std::string type, int size, std::string suffix = "");

		template<typename T>
		static T Get(node::Data* d, int i);

		std::list<nodeshellPtr> targetList;
		std::list<nodePtr> ioVariablesList;
		std::list<std::list<nodePtr> > listOffunctionNodesList;
		std::list<nodePtr> mainBlockList;
		std::set<nodePtr> visitedNodes;
		int id = 0;
		int indent = 0;
	};

	inline std::string context::genShader()
	{
		// Visiting the target forest
		for (std::list<nodeshellPtr>::iterator it = targetList.begin(); it != targetList.end(); ++it)
		{
			VisitNode((*it)->n);
		}
		return GenerateCode();
	}

	template<>
	inline float context::Get<float>(node::Data* d, int i)
	{
		return d[i].d_vec;
	}

	template<>
	inline int context::Get<int>(node::Data* d, int i)
	{
		return d[i].d_ivec;
	}

	template<>
	inline unsigned int context::Get<unsigned int>(node::Data* d, int i)
	{
		return d[i].d_uvec;
	}

	template<>
	inline std::string context::Get<std::string>(node::Data* d, int i)
	{
		return d[i].d_bvec ? "true" : "false";
	}

	inline void context::VisitNode(nodePtr n)
	{
		ioVariablesList.clear();
		listOffunctionNodesList.clear();
		mainBlockList.clear();
		visitedNodes.clear();
		VisitNodeInternal(n, mainBlockList);
	}

	inline void context::VisitNodeInternal(nodePtr n, std::list<nodePtr>& defaultList)
	{
		if (visitedNodes.find(n) == visitedNodes.end())
		{
			std::list<nodePtr>* listToUse = &defaultList;

			if (n->optype & node::io_bit)
			{
				listToUse = &ioVariablesList;
			}

			for (auto& child : n->childs)
			{
				VisitNodeInternal(child, *listToUse);
			}
			
			listToUse->push_back(n);
			visitedNodes.insert(n);
		}
	}

	inline std::string context::GenerateCode()
	{
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
	void context::PrintConstLiteral(std::stringstream& ss, node::Data* d, std::string type, int size, std::string suffix)
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

	inline std::string context::Emit(nodePtr n)
	{
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
			case node::rshift:
			{
				ss << n->childs[0]->GetId() << " >> " << n->childs[1]->GetId();
			}
			case node:: or :
			{
				ss << n->childs[0]->GetId() << " | " << n->childs[1]->GetId();
			}
			case node::xor:
			{
				ss << n->childs[0]->GetId() << " ^ " << n->childs[1]->GetId();
			}
			case node::and:
			{
				ss << n->childs[0]->GetId() << " & " << n->childs[1]->GetId();
			}
			case node::lor:
			{
				ss << n->childs[0]->GetId() << " || " << n->childs[1]->GetId();
			}
			case node::lxor:
			{
				ss << n->childs[0]->GetId() << " ^^ " << n->childs[1]->GetId();
			}
			case node::land:
			{
				ss << n->childs[0]->GetId() << " && " << n->childs[1]->GetId();
			}
			case node::equal:
			{
				ss << n->childs[0]->GetId() << " == " << n->childs[1]->GetId();
			}
			case node::mod:
			{
				ss << n->childs[0]->GetId() << " % " << n->childs[1]->GetId();
			}
			case node::neg:
			{
				ss << "-" << n->childs[0]->GetId();
			}
			case node::preinc:
			{
				ss << "++" << n->childs[0]->GetId();
			}
			case node::predec:
			{
				ss << "--" << n->childs[0]->GetId();
			}
			case node::postinc:
			{
				ss << n->childs[0]->GetId() << "++";
			}
			case node::postdec:
			{
				ss << n->childs[0]->GetId() << "--";
			}
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
			}
		}

		ss << ";\n";
		return ss.str();
	}
	
	inline std::string context::GetType(nodePtr n)
	{
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

#define define_constant(T, X) static T X(#X, node::predefined_const)

	define_constant(ivec1, gl_MaxVertexAttribs);
	define_constant(ivec1, gl_MaxVertexOutputComponents);
	define_constant(ivec1, gl_MaxVertexUniformComponents);
	define_constant(ivec1, gl_MaxVertexTextureImageUnits);
	define_constant(ivec1, gl_MaxGeometryInputComponents);
	define_constant(ivec1, gl_MaxGeometryOutputComponents);
	define_constant(ivec1, gl_MaxGeometryUniformComponents);
	define_constant(ivec1, gl_MaxGeometryTextureImageUnits);
	define_constant(ivec1, gl_MaxGeometryOutputVertices);
	define_constant(ivec1, gl_MaxGeometryTotalOutputComponents);
	define_constant(ivec1, gl_MaxGeometryVaryingComponents);
	define_constant(ivec1, gl_MaxFragmentInputComponents);
	define_constant(ivec1, gl_MaxDrawBuffers);
	define_constant(ivec1, gl_MaxFragmentUniformComponents);
	define_constant(ivec1, gl_MaxTextureImageUnits1);
	define_constant(ivec1, gl_MaxClipDistances);
	define_constant(ivec1, gl_MaxCombinedTextureImageUnits);

	// Requires OpenGL 4.0

	define_constant(ivec1, gl_MaxTessControlInputComponents);
	define_constant(ivec1, gl_MaxTessControlOutputComponents);
	define_constant(ivec1, gl_MaxTessControlUniformComponents);
	define_constant(ivec1, gl_MaxTessControlTextureImageUnits);
	define_constant(ivec1, gl_MaxTessControlTotalOutputComponents);
	define_constant(ivec1, gl_MaxTessEvaluationInputComponents);
	define_constant(ivec1, gl_MaxTessEvaluationOutputComponents);
	define_constant(ivec1, gl_MaxTessEvaluationUniformComponents);
	define_constant(ivec1, gl_MaxTessEvaluationTextureImageUnits);
	define_constant(ivec1, gl_MaxTessPatchComponents);
	define_constant(ivec1, gl_MaxPatchVertices);
	define_constant(ivec1, gl_MaxTessGenLevel);

	// Requires OpenGL 4.1

	define_constant(ivec1, gl_MaxViewports);
	define_constant(ivec1, gl_MaxVertexUniformVectors);
	define_constant(ivec1, gl_MaxFragmentUniformVectors);
	define_constant(ivec1, gl_MaxVaryingVectors);

	// Requires OpenGL 4.2

	define_constant(ivec1, gl_MaxVertexImageUniforms);
	define_constant(ivec1, gl_MaxVertexAtomicCounters);
	define_constant(ivec1, gl_MaxVertexAtomicCounterBuffers);
	define_constant(ivec1, gl_MaxTessControlImageUniforms);
	define_constant(ivec1, gl_MaxTessControlAtomicCounters);
	define_constant(ivec1, gl_MaxTessControlAtomicCounterBuffers);
	define_constant(ivec1, gl_MaxTessEvaluationImageUniforms);
	define_constant(ivec1, gl_MaxTessEvaluationAtomicCounters);
	define_constant(ivec1, gl_MaxTessEvaluationAtomicCounterBuffers);
	define_constant(ivec1, gl_MaxGeometryImageUniforms);
	define_constant(ivec1, gl_MaxGeometryAtomicCounters);
	define_constant(ivec1, gl_MaxGeometryAtomicCounterBuffers);
	define_constant(ivec1, gl_MaxFragmentImageUniforms);
	define_constant(ivec1, gl_MaxFragmentAtomicCounters);
	define_constant(ivec1, gl_MaxFragmentAtomicCounterBuffers);
	define_constant(ivec1, gl_MaxCombinedImageUniforms);
	define_constant(ivec1, gl_MaxCombinedAtomicCounters);
	define_constant(ivec1, gl_MaxCombinedAtomicCounterBuffers);
	define_constant(ivec1, gl_MaxImageUnits);
	define_constant(ivec1, gl_MaxCombinedImageUnitsAndFragmentOutputs);
	define_constant(ivec1, gl_MaxImageSamples);
	define_constant(ivec1, gl_MaxAtomicCounterBindings);
	define_constant(ivec1, gl_MaxAtomicCounterBufferSize);
	define_constant(ivec1, gl_MinProgramTexelOffset);
	define_constant(ivec1, gl_MaxProgramTexelOffset);

	// Requires OpenGL 4.3

	define_constant(ivec3, gl_MaxComputeWorkGroupCount);
	define_constant(ivec3, gl_MaxComputeWorkGroupSize);
	define_constant(ivec1, gl_MaxComputeUniformComponents);
	define_constant(ivec1, gl_MaxComputeTextureImageUnits);
	define_constant(ivec1, gl_MaxComputeImageUniforms);
	define_constant(ivec1, gl_MaxComputeAtomicCounters);
	define_constant(ivec1, gl_MaxComputeAtomicCounterBuffers);

	// Requires OpenGL 4.4

	define_constant(ivec1, gl_MaxTransformFeedbackBuffers);
	define_constant(ivec1, gl_MaxTransformFeedbackInterleavedComponents);
}
