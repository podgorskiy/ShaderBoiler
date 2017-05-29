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

#include "datatypes.h"

#define binop(T1, T2, T3, X, E)\
	/* Binary operator, like T3 a = T1() * T2(); */ \
	/* Outside class definition */ \
	T3 operator X (const T1& a, const T2& b) { \
		T3 result; \
		result.src->optype = detail::node::E; \
		result.src->childs.push_back(a.src); \
		result.src->childs.push_back(b.src); \
		return result; \
	}

#define assignop(T1, T2, X, E)\
	/* Assign operator, like T1 a; a += T2(); */ \
	/* Outside class definition */ \
	T1& operator X (T1&a, const T2& b) { \
		return detail::assign_operation<T1, T2>(a, b, detail::node::E); \
	}

#define op_ariphm(T1, T2, T3)\
	binop(T1, T2, T3, +, binary_addition); \
	binop(T1, T2, T3, -, binary_substruction); \
	binop(T1, T2, T3, *, binary_multiplication); \
	binop(T1, T2, T3, /, binary_division); \

#define assigop_ariphm(T1, T2)\
	assignop(T1, T2, +=, assign_addition); \
	assignop(T1, T2, -=, assign_substruction); \
	assignop(T1, T2, *=, assign_multiplication); \
	assignop(T1, T2, /=, assign_division); \

#define op_mod(T1, T2, T3) \
	binop(T1, T2, T3, %, binary_mod);

#define assigop_mod(T1, T2) \
	assignop(T1, T2, %=, assign_mod); \

#define op_shifts(T1, T2)\
	binop(T1, T2, T1, <<, binary_lshift); \
	binop(T1, T2, T1, >>, binary_rshift); \
	assignop(T1, T2, <<=, assign_lshift); \
	assignop(T1, T2, >>=, assign_rshift); \

#define op_bitop(T1, T2, T3)\
	binop(T1, T2, T3, &, binary_and); \
	binop(T1, T2, T3, |, binary_or); \
	binop(T1, T2, T3, ^, binary_xor);

#define assigop_bitop(T1, T2)\
	assignop(T1, T2, &=, assign_and); \
	assignop(T1, T2, |=, assign_or); \
	assignop(T1, T2, ^=, assign_xor);

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

namespace sb
{
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

	binop(bvec1, bvec1, bvec1, &&, binary_land);
	binop(bvec1, bvec1, bvec1, || , binary_lor);
	//binop(bvec1, bvec1, bvec1, ^^, lxor); // operator not supported by c++

	bvec1 operator<(const vec1& a, const vec1& b)
	{
		return bvec1();
	}
}
