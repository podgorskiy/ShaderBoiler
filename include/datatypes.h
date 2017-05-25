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
#include "detail/variable.h"
#include "detail/macroses.h"

#define POD_vec  float
#define POD_dec  double
#define POD_ivec int
#define POD_uvec int
#define POD_bvec bool

namespace sb
{
	namespace detail
	{
		template<typename T1, typename T2>
		T1 simple_assign(T1& this_, const T2& x)
		{
			bool io_node = (this_.src->optype & detail::node::storage_bit) != 0 || (this_.src->optype == detail::node::builtin_variable) != 0;
			bool io_assign_node = 
				(this_.src->optype & detail::node::assign_bit) != 0 &&
				(this_.src->childs.size() > 0) && 
				((this_.src->childs[0]->optype & detail::node::storage_bit) != 0 || (this_.src->childs[0]->optype == detail::node::builtin_variable) != 0);
			bool arrayLookUp = (this_.src->optype & detail::node::arrayLookup) != 0;
			if (io_node || io_assign_node || arrayLookUp)
			{
				T1 result; 
				detail::nodePtr oldsrc; 
			
				if (io_assign_node)
				{
					oldsrc = this_.src->childs[0];
				}
				else 
				{
					oldsrc = this_.src;
				}
				
				this_.src = result.src;
				this_.src->optype = detail::node::assign;
				this_.src->childs.push_back(oldsrc);
				this_.src->childs.push_back(x.src);
				if (!this_.shell.expired())
				{
					this_.shell.lock()->n = this_.src;
					/*if (arrayLookUp && (this_.shell.lock()->n->optype & detail::node::arrayLookup) != 0)
					{
						this_.shell.lock()->n.shell.lock()->n = this_.src;
					}
					else
					{
					}
					*/
				} 
				return this_;
			}
			else 
			{
				this_.src = x.src;
				/* shell is ignored */
				return this_;
			}
		}
	}
}

#define simple_assignop(T1, T2, S)\
	/* Assign operator, like T1 a; a = T2(); Not trivial for input-output variables */ \
	/* Inside class definition */ \
	T1##S operator = (const T2##S& x) { \
		return detail::simple_assign<T1##S, T2##S>(*this, x); \
	}

	// Sets collection of typecast operators to T type
#define type_cast(T, S) type_cast_to_##T(S)

#define type_cast_to_vec(S) type_cast_from(vec, uvec, S) type_cast_from(vec, ivec, S) type_cast_from(vec, bvec, S)
#define type_cast_to_uvec(S) type_cast_from(uvec, vec, S) type_cast_from(uvec, ivec, S) type_cast_from(uvec, bvec, S)
#define type_cast_to_ivec(S) type_cast_from(ivec, uvec, S) type_cast_from(ivec, vec, S) type_cast_from(ivec, bvec, S)
#define type_cast_to_bvec(S) type_cast_from(bvec, uvec, S) type_cast_from(bvec, ivec, S) type_cast_from(bvec, vec, S)

#define type_cast_from(T, T_from, S) \
		/* Casts from type T_from to T, element-wise */ \
		explicit T##S(const T_from##S& f) { \
			src->optype = detail::node::cast; \
			src->childs.push_back(detail::GetPtr(f)); \
		}

#define cast_from_scalar(T, S) \
		/* Initializes each component of the vec<S> with the one argument of vec1 type */ \
		explicit T##S(T##1 f) { \
			src->optype = detail::node::cast; \
			src->childs.push_back(f.src); \
		}

#define cast_from_vec_and_vec(T, S, V1, V2) \
		/* Initializes each component of the vec<S> with the one argument of vec1 type */ \
		explicit T##S(T##V1 v1, T##V2 v2) { \
			src->optype = detail::node::cast; \
			src->childs.push_back(v1.src); \
			src->childs.push_back(v2.src); \
		}

#define cast_from_vec_and_vec_and_vec(T, S, V1, V2, V3) \
		/* Initializes each component of the vec<S> with the one argument of vec1 type */ \
		explicit T##S(T##V1 v1, T##V2 v2, T##V3 v3) { \
			src->optype = detail::node::cast; \
			src->childs.push_back(v1.src); \
			src->childs.push_back(v2.src); \
			src->childs.push_back(v3.src); \
		}

#define drop_cast(T, S, V) \
		/* Initializes each component of the vec<S> with the one argument of vec1 type */ \
		explicit T##S(const T##V& v) { \
			src->optype = detail::node::cast; \
			src->childs.push_back(detail::GetPtr(v)); \
		}

#define cast_from_const_literal_scalar(T, S) \
		/* Initializes each component of the vec<S> with the one argument of POD type*/ \
		explicit T##S(POD_##T f) {\
			REPEAT_ASSIGNMENT(src->data[, ].d_##T, f, S); \
			src->optype = detail::node::OpType::o##T; \
		}

#define main_constructor(T, S) \
		/* Initializes each component of the vec<S> with the S arguments of POD type*/ \
		T##S(REPEAT_WITH_ID_AND_COMMA(POD_##T f, S)) \
		{ \
			REPEAT_ASSIGNMENT_WITH_ID(src->data[, ].d_##T, f, S); \
			src->optype = detail::node::OpType::o##T; \
		}

#define default_constructors(T, S) \
		T##S() {}; \
		T##S(const T##S& other) { \
		 src = other.src; \
		 /* shell is ignored */ \
		}; \
		T##S(const std::string& name): typed_variable(name) {}; \
		T##S(const std::string& name, detail::node::OpType t): typed_variable(name, t) {}; \
		T##S& SetName(const std::string& name) { \
			src->name = name; \
			return *this; \
		}

#define class_vec_dec_size(T, S) class T##S

#define class_vec_def_size1(T) \
	/* All vectors of size 1: vec1, ivec1, uvec1, bvec1, dvec1*/ \
	class T##1: public detail::typed_variable<detail::node::DataType::##T, detail::node::DataSize(1)>{ \
	public: \
		/* Default constructor */ \
		default_constructors(T, 1); \
		drop_cast(T, 1, 4); \
		drop_cast(T, 1, 3); \
		drop_cast(T, 1, 2); \
		type_cast(T, 1); \
		simple_assignop(T, T, 1); \
		/* Constructor from one argument of POD type*/ \
		T##1(POD_##T f0) { src->data[0].d_##T = f0; src->optype = detail::node::OpType::o##T; }; \
	}

#define class_vec_def_size2(T) \
	/* All vectors of size 2: vec2, ivec2, uvec2, bvec2, dvec2*/ \
	class T##2: public detail::typed_variable<detail::node::DataType::##T, detail::node::DataSize(2)>{ \
	public: \
		default_constructors(T, 2); \
		cast_from_scalar(T, 2); \
		drop_cast(T, 2, 4); \
		drop_cast(T, 2, 3); \
		main_constructor(T, 2); \
		type_cast(T, 2); \
		simple_assignop(T, T, 2); \
		cast_from_const_literal_scalar(T, 2); \
	}

#define class_vec_def_size3(T) \
	/* All vectors of size 3: vec3, ivec3, uvec3, bvec3, dvec3*/ \
	class T##3: public detail::typed_variable<detail::node::DataType::##T, detail::node::DataSize(3)>{ \
	public: \
		default_constructors(T, 3); \
		cast_from_scalar(T, 3); \
		cast_from_vec_and_vec(T, 3, 2, 1); \
		cast_from_vec_and_vec(T, 3, 1, 2); \
		drop_cast(T, 3, 4); \
		type_cast(T, 3); \
		simple_assignop(T, T, 3); \
		main_constructor(T, 3); \
		cast_from_const_literal_scalar(T, 3); \
	}

#define class_vec_def_size4(T) \
	/* All vectors of size 4: vec4, ivec4, uvec4, bvec4, dvec4*/ \
	class T##4: public detail::typed_variable<detail::node::DataType::##T, detail::node::DataSize(4)>{ \
	public: \
		default_constructors(T, 4); \
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
	}

#define class_mat_def_(T, PT, M, N, MbyN) \
	class T##M##x##N: public detail::typed_variable<detail::node::DataType::##T, detail::node::DataSize(M), detail::node::DataSize(N)>{ \
	public: \
		T##M##x##N() {}; \
		T##M##x##N(POD_##PT f) { REPEAT_ASSIGNMENT(src->data[(N + 1) *, ].d_##PT, f, M); src->optype = detail::node::OpType::o##PT; }; \
		T##M##x##N(REPEAT_WITH_ID_AND_COMMA(POD_##PT f, MbyN)) { REPEAT_ASSIGNMENT_WITH_ID(src->data[, ].d_##PT, f, MbyN); src->optype = detail::node::OpType::o##PT; }; \
	}

#define class_mat_def(T, PT, M, N) class_mat_def_(T, PT, M, N, MULL(M, N))

#define class_vec_dec(T) \
	class_vec_dec_size(T, 1); \
	class_vec_dec_size(T, 2); \
	class_vec_dec_size(T, 3); \
	class_vec_dec_size(T, 4)

#define class_vec_def(T) \
	class_vec_def_size1(T); \
	class_vec_def_size2(T); \
	class_vec_def_size3(T); \
	class_vec_def_size4(T)

namespace sb
{
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

	// float vector
	class_vec_def(vec);
	// int vector
	class_vec_def(ivec);
	// unsigned int vector
	class_vec_def(uvec);
	// bool vector
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
}
