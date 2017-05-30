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
#define POD_uvec unsigned int
#define POD_bvec bool

namespace sb
{
	namespace detail
	{
		template<typename T1, typename T2>
		inline T1& assign_operation(T1& this_, const T2& x, detail::node::OpType t = detail::node::assign)
		{
			T1 result;

			result.src->optype = t;
			result.src->childs.push_back(this_.src);
			result.src->childs.push_back(x.src);

			this_.src = result.src;

			for (std::vector<nodePtr*>::iterator it = this_.ptrToSrcPtr.begin(); it != this_.ptrToSrcPtr.end(); ++it)
			{
				detail::nodePtr junction(new detail::node());
				junction->optype = detail::node::dependency;
				junction->childs.push_back(this_.src);
				junction->childs.push_back(*(*it));

				*(*it) = junction;
			}

			return this_;
		}

		template<typename T>
		inline T& SetName(const std::string& name, T& var)
		{
			var.src->name = name;
			return var;
		}
	}
}

#define SWIZZLE(C, T, M) T& M();

#define SWIZZLES_XY1(C, T, P, X, Y) \
		SWIZZLE(C, T, P##X); \
		SWIZZLE(C, T, P##Y); \

#define SWIZZLES_XY2(C, T, P, X, Y) \
		SWIZZLES_XY1(C, T, P##X, X, Y); \
		SWIZZLES_XY1(C, T, P##Y, X, Y); \

#define SWIZZLES_XY3(C, T, P, X, Y) \
		SWIZZLES_XY2(C, T, P##X, X, Y); \
		SWIZZLES_XY2(C, T, P##Y, X, Y); \

#define SWIZZLES_XY4(C, T, _, X, Y) \
		SWIZZLES_XY3(C, T, X, X, Y) \
		SWIZZLES_XY3(C, T, Y, X, Y)

#define SWIZZLES_XYZ1(C, T, P, X, Y, Z) \
		SWIZZLE(C, T, P##X); \
		SWIZZLE(C, T, P##Y); \
		SWIZZLE(C, T, P##Z); \

#define SWIZZLES_XYZ2(C, T, P, X, Y, Z) \
		SWIZZLES_XYZ1(C, T, P##X, X, Y, Z); \
		SWIZZLES_XYZ1(C, T, P##Y, X, Y, Z); \
		SWIZZLES_XYZ1(C, T, P##Z, X, Y, Z); \

#define SWIZZLES_XYZ3(C, T, P, X, Y, Z) \
		SWIZZLES_XYZ2(C, T, P##X, X, Y, Z); \
		SWIZZLES_XYZ2(C, T, P##Y, X, Y, Z); \
		SWIZZLES_XYZ2(C, T, P##Z, X, Y, Z); \

#define SWIZZLES_XYZ4(C, T, _, X, Y, Z) \
		SWIZZLES_XYZ3(C, T, X, X, Y, Z) \
		SWIZZLES_XYZ3(C, T, Y, X, Y, Z) \
		SWIZZLES_XYZ3(C, T, Z, X, Y, Z)

#define SWIZZLES_XYZW1(C, T, P, X, Y, Z, W) \
		SWIZZLE(C, T, P##X); \
		SWIZZLE(C, T, P##Y); \
		SWIZZLE(C, T, P##Z); \
		SWIZZLE(C, T, P##W); \

#define SWIZZLES_XYZW2(C, T, P, X, Y, Z, W) \
		SWIZZLES_XYZW1(C, T, P##X, X, Y, Z, W); \
		SWIZZLES_XYZW1(C, T, P##Y, X, Y, Z, W); \
		SWIZZLES_XYZW1(C, T, P##Z, X, Y, Z, W); \
		SWIZZLES_XYZW1(C, T, P##W, X, Y, Z, W); \

#define SWIZZLES_XYZW3(C, T, P, X, Y, Z, W) \
		SWIZZLES_XYZW2(C, T, P##X, X, Y, Z, W); \
		SWIZZLES_XYZW2(C, T, P##Y, X, Y, Z, W); \
		SWIZZLES_XYZW2(C, T, P##Z, X, Y, Z, W); \
		SWIZZLES_XYZW2(C, T, P##W, X, Y, Z, W); \

#define SWIZZLES_XYZW4(C, T, _, X, Y, Z, W) \
		SWIZZLES_XYZW3(C, T, X, X, Y, Z, W) \
		SWIZZLES_XYZW3(C, T, Y, X, Y, Z, W) \
		SWIZZLES_XYZW3(C, T, Z, X, Y, Z, W) \
		SWIZZLES_XYZW3(C, T, W, X, Y, Z, W) \

#define SWIZZLE_XY_SET(C, T) \
		SWIZZLES_XY1(C, T##1, , x, y)\
		SWIZZLES_XY2(C, T##2, , x, y)\
		SWIZZLES_XY3(C, T##3, , x, y)\
		SWIZZLES_XY4(C, T##4, , x, y)\
		SWIZZLES_XY1(C, T##1, , r, g)\
		SWIZZLES_XY2(C, T##2, , r, g)\
		SWIZZLES_XY3(C, T##3, , r, g)\
		SWIZZLES_XY4(C, T##4, , r, g)\
		SWIZZLES_XY1(C, T##1, , s, t)\
		SWIZZLES_XY2(C, T##2, , s, t)\
		SWIZZLES_XY3(C, T##3, , s, t)\
		SWIZZLES_XY4(C, T##4, , s, t)\

#define SWIZZLE_XYZ_SET(C, T) \
		SWIZZLES_XYZ1(C, T##1,  , x, y, z)\
		SWIZZLES_XYZ2(C, T##2,  , x, y, z)\
		SWIZZLES_XYZ3(C, T##3,  , x, y, z)\
		SWIZZLES_XYZ4(C, T##4,  , x, y, z)\
		SWIZZLES_XYZ1(C, T##1,  , r, g, b)\
		SWIZZLES_XYZ2(C, T##2,  , r, g, b)\
		SWIZZLES_XYZ3(C, T##3,  , r, g, b)\
		SWIZZLES_XYZ4(C, T##4,  , r, g, b)\
		SWIZZLES_XYZ1(C, T##1,  , s, t, p)\
		SWIZZLES_XYZ2(C, T##2,  , s, t, p)\
		SWIZZLES_XYZ3(C, T##3,  , s, t, p)\
		SWIZZLES_XYZ4(C, T##4,  , s, t, p)\

#define SWIZZLE_XYZW_SET(C, T) \
		SWIZZLES_XYZW1(C, T##1, , x, y, z, w)\
		SWIZZLES_XYZW2(C, T##2, , x, y, z, w)\
		SWIZZLES_XYZW3(C, T##3, , x, y, z, w)\
		SWIZZLES_XYZW4(C, T##4, , x, y, z, w)\
		SWIZZLES_XYZW1(C, T##1, , r, g, b, a)\
		SWIZZLES_XYZW2(C, T##2, , r, g, b, a)\
		SWIZZLES_XYZW3(C, T##3, , r, g, b, a)\
		SWIZZLES_XYZW4(C, T##4, , r, g, b, a)\
		SWIZZLES_XYZW1(C, T##1, , s, t, p, q)\
		SWIZZLES_XYZW2(C, T##2, , s, t, p, q)\
		SWIZZLES_XYZW3(C, T##3, , s, t, p, q)\
		SWIZZLES_XYZW4(C, T##4, , s, t, p, q)\

#define simple_assignop(T1, T2, S)\
	/* Assign operator, like T1 a; a = T2(); Not trivial for input-output variables */ \
	/* Inside class definition */ \
	T1##S& operator = (const T2##S& x) { \
		return detail::assign_operation<T1##S, T2##S>(*this, x); \
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
			detail::assign_operation<T##S, T##S>(*this, other); \
		}; \
		T##S(const std::string& name): typed_variable(name) {}; \
		T##S(const std::string& name, detail::node::OpType t): typed_variable(name, t) {}; \
		T##S& SetName(const std::string& name) { return detail::SetName(name, *this); }

#define class_vec_dec_size(T, S) class T##S

#define class_vec_def_size1(T) \
	/* All vectors of size 1: vec1, ivec1, uvec1, bvec1, dvec1*/ \
	class T##1: public detail::typed_variable<detail::node::T, detail::node::DataSize(1)>{ \
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
	class T##2: public detail::typed_variable<detail::node::T, detail::node::DataSize(2)>{ \
	public: \
		default_constructors(T, 2); \
		cast_from_scalar(T, 2); \
		drop_cast(T, 2, 4); \
		drop_cast(T, 2, 3); \
		main_constructor(T, 2); \
		type_cast(T, 2); \
		simple_assignop(T, T, 2); \
		cast_from_const_literal_scalar(T, 2); \
		SWIZZLE_XY_SET(,T) \
	}

#define class_vec_def_size3(T) \
	/* All vectors of size 3: vec3, ivec3, uvec3, bvec3, dvec3*/ \
	class T##3: public detail::typed_variable<detail::node::T, detail::node::DataSize(3)>{ \
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
		SWIZZLE_XYZ_SET(,T) \
	}

#define class_vec_def_size4(T) \
	/* All vectors of size 4: vec4, ivec4, uvec4, bvec4, dvec4*/ \
	class T##4: public detail::typed_variable<detail::node::T, detail::node::DataSize(4)>{ \
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
		SWIZZLE_XYZW_SET(,T) \
	}

#define class_mat_def_(T, PT, M, N, MbyN) \
	class T##M##x##N: public detail::typed_variable<detail::node::T, detail::node::DataSize(M), detail::node::DataSize(N)>{ \
	public: \
		T##M##x##N() {}; \
		T##M##x##N(POD_##PT f) { REPEAT_ASSIGNMENT(src->data[(N + 1) *, ].d_##PT, f, M); src->optype = detail::node::OpType::o##PT; }; \
		T##M##x##N(REPEAT_WITH_ID_AND_COMMA(POD_##PT f, MbyN)) { REPEAT_ASSIGNMENT_WITH_ID(src->data[, ].d_##PT, f, MbyN); src->optype = detail::node::OpType::o##PT; }; \
	}

#define class_mat_def(T, PT, M, N) class_mat_def_(T, PT, M, N, MULL(M, N))

#define class_vec_methods_def_size1(T) \

#define class_vec_methods_def_size2(T) \
		SWIZZLE_XY_SET(T##2,T)

#define class_vec_methods_def_size3(T) \
		SWIZZLE_XYZ_SET(T##3,T)

#define class_vec_methods_def_size4(T) \
		SWIZZLE_XYZW_SET(T##4,T)

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

#define class_vec_methods_def(T) \
	class_vec_methods_def_size1(T); \
	class_vec_methods_def_size2(T); \
	class_vec_methods_def_size3(T); \
	class_vec_methods_def_size4(T)

namespace sb
{
	namespace detail
	{
		template<typename T1, typename T2>
		inline T1& GenerateSwizzle(T2* this_, const char* m)
		{
			T1* result = new T1();
			result->src->optype = detail::node::memberAccess;
			result->src->fname = m;
			if (!this_->originalsrc || this_->src == this_->originalsrc)
			{
				result->src->childs.push_back(this_->src);
				this_->originalsrc = this_->src;
			}
			else
			{
				detail::nodePtr junction(new detail::node());
				junction->optype = node::dependency;
				junction->childs.push_back(this_->src);
				junction->childs.push_back(this_->originalsrc);
				result->src->childs.push_back(junction);
			}
			result->ptrToSrcPtr.push_back(&(this_->src));

			for (std::vector<detail::nodePtr*>::iterator it = this_->ptrToSrcPtr.begin(); it != this_->ptrToSrcPtr.end(); ++it)
			{
				result->ptrToSrcPtr.push_back(*it);
			}

			this_->garbageVars.push_back(varPtr(result));
			return *result;
		}
	}

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

#undef SWIZZLE

#define SWIZZLE(C, T, M) T& C::M(){ return detail::GenerateSwizzle<T, C>(this, #M); }

	// float vector
	class_vec_methods_def(vec);
	// int vector
	class_vec_methods_def(ivec);
	// unsigned int vector
	class_vec_methods_def(uvec);
	// bool vector
	class_vec_methods_def(bvec);

	typedef vec1 Float;
	typedef ivec1 Int;
	typedef uvec1 UInt;
	typedef bvec1 Bool;
}
