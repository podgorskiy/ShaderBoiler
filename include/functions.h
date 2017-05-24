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

#define function_def_oneArg(N, T1, T2) \
	T1 N(const T2& a) { \
		T1 result; \
		result.src->optype = detail::node::functionCall; \
		result.src->fname = #N; \
		result.src->childs.push_back(a.src); \
		return result; \
	}

#define function_def_twoArg(N, T1, T2, T3) \
	T1 N(const T2& a, const T3& b) { \
		T1 result; \
		result.src->optype = detail::node::functionCall; \
		result.src->fname = #N; \
		result.src->childs.push_back(a.src); \
		result.src->childs.push_back(b.src); \
		return result; \
	}

namespace sb
{
#define function_def_genType_genType(N) \
	function_def_oneArg(N, vec1, vec1) \
	function_def_oneArg(N, vec2, vec2) \
	function_def_oneArg(N, vec3, vec3) \
	function_def_oneArg(N, vec4, vec4)

#define function_def_genIType_genIType(N) \
	function_def_oneArg(N, ivec1, ivec1) \
	function_def_oneArg(N, ivec2, ivec2) \
	function_def_oneArg(N, ivec3, ivec3) \
	function_def_oneArg(N, ivec4, ivec4)

#define function_def_genType_genType_genType(N) \
	function_def_twoArg(N, vec1, vec1, vec1) \
	function_def_twoArg(N, vec2, vec2, vec2) \
	function_def_twoArg(N, vec3, vec3, vec3) \
	function_def_twoArg(N, vec4, vec4, vec4)

#define function_def_genIType_genIType_genIType(N) \
	function_def_twoArg(N, ivec1, ivec1, ivec1) \
	function_def_twoArg(N, ivec2, ivec2, ivec2) \
	function_def_twoArg(N, ivec3, ivec3, ivec3) \
	function_def_twoArg(N, ivec4, ivec4, ivec4)

#define function_def_genUType_genUType_genUType(N) \
	function_def_twoArg(N, uvec1, uvec1, uvec1) \
	function_def_twoArg(N, uvec2, uvec2, uvec2) \
	function_def_twoArg(N, uvec3, uvec3, uvec3) \
	function_def_twoArg(N, uvec4, uvec4, uvec4)

#define function_def_genType_genType_float(N) \
	function_def_twoArg(N, vec2, vec2, vec1) \
	function_def_twoArg(N, vec3, vec3, vec1) \
	function_def_twoArg(N, vec4, vec4, vec1)

#define function_def_genIType_genIType_int(N) \
	function_def_twoArg(N, ivec2, ivec2, ivec1) \
	function_def_twoArg(N, ivec3, ivec3, ivec1) \
	function_def_twoArg(N, ivec4, ivec4, ivec1)

#define function_def_genUType_genUType_uint(N) \
	function_def_twoArg(N, uvec2, uvec2, uvec1) \
	function_def_twoArg(N, uvec3, uvec3, uvec1) \
	function_def_twoArg(N, uvec4, uvec4, uvec1)

	// Angle and Trigonometry Functions
	function_def_genType_genType(radians);
	function_def_genType_genType(degrees);
	function_def_genType_genType(sin);
	function_def_genType_genType(cos);
	function_def_genType_genType(tan);
	function_def_genType_genType(asin);
	function_def_genType_genType(acos);
	function_def_genType_genType_genType(atan);
	function_def_genType_genType(atan);
	function_def_genType_genType(sinh);
	function_def_genType_genType(cosh);
	function_def_genType_genType(tanh);
	function_def_genType_genType(asinh);
	function_def_genType_genType(acosh);
	function_def_genType_genType(atanh);

	// Exponential Functions
	function_def_genType_genType_genType(pow);
	function_def_genType_genType(exp);
	function_def_genType_genType(log);
	function_def_genType_genType(exp2);
	function_def_genType_genType(log2);
	function_def_genType_genType(sqrt);
	function_def_genType_genType(inversesqrt);

	// Common Functions
	function_def_genType_genType(abs);
	function_def_genIType_genIType(abs);
	function_def_genType_genType(sign);
	function_def_genIType_genIType(sign);
	function_def_genType_genType(floor);
	function_def_genType_genType(trunc);
	function_def_genType_genType(round);
	function_def_genType_genType(roundEven);
	function_def_genType_genType(ceil);
	function_def_genType_genType(fract);
	function_def_genType_genType_genType(mod);
	function_def_genType_genType_float(mod);
	//todo modf
	function_def_genType_genType_genType(min);
	function_def_genType_genType_float(min);
	function_def_genIType_genIType_genIType(min);
	function_def_genIType_genIType_int(min);
	function_def_genUType_genUType_genUType(min);
	function_def_genUType_genUType_uint(min);
	function_def_genType_genType_genType(max);
	function_def_genType_genType_float(max);
	function_def_genIType_genIType_genIType(max);
	function_def_genIType_genIType_int(max);
	function_def_genUType_genUType_genUType(max);
	function_def_genUType_genUType_uint(max);

	function_def_twoArg(dot, vec1, vec1, vec1);
	function_def_twoArg(dot, vec1, vec2, vec2);
	function_def_twoArg(dot, vec1, vec3, vec3);
	function_def_twoArg(dot, vec1, vec4, vec4);	
}

#undef function_def_oneArg
#undef function_def_twoArg
#undef  function_def_genType_genType
#undef  function_def_genIType_genIType
#undef  function_def_genType_genType_genType
#undef  function_def_genIType_genIType_genIType
#undef  function_def_genUType_genUType_genUType
#undef  function_def_genType_genType_float
#undef  function_def_genIType_genIType_int
#undef  function_def_genUType_genUType_uint
