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
#include "array.h"
#include "detail/output_proxy.h"

#define define_input(T, X) static const T X(#X, detail::node::builtin_variable)
#define define_output(T, X) static const detail::output_proxy<T> X(#X)

namespace sb
{	
	// Compute shader
	namespace cs
	{
		// Requires OpenGL 4.3
		namespace gl430
		{
			define_input(uvec3, gl_NumWorkGroups);
			define_input(uvec3, gl_WorkGroupSize);
			define_input(uvec3, gl_WorkGroupID);
			define_input(uvec3, gl_LocalInvocationID);
			define_input(uvec3, gl_GlobalInvocationID);
			define_input(uvec1, gl_LocalInvocationIndex);
		}

		// Requires OpenGL 4.4
		namespace gl440
		{
			using namespace gl430;
		}

		// Requires OpenGL 4.5
		namespace gl450
		{
			using namespace gl440;
		}
	}

	// Vertex shader
	namespace vs
	{
		// Requires OpenGL 3.0
		namespace gl130
		{
			define_input(ivec1, gl_VertexID);
			define_output(vec4, gl_Position);
			define_output(vec1, gl_PointSize);
			// out float gl_ClipDistance[]; //vs 1.3
		}

		// Requires OpenGL 3.1
		namespace gl140
		{
			using namespace gl130;
			define_input(ivec1, gl_InstanceID);
		}

		// Requires OpenGL 3.2
		namespace gl150
		{
			using namespace gl140;
		}

		// Requires OpenGL 3.3
		namespace gl330
		{
			using namespace gl150;
		}

		// Requires OpenGL 4.0
		namespace gl400
		{
			using namespace gl330;
		}

		// Requires OpenGL 4.1
		namespace gl410
		{
			using namespace gl400;
		}

		// Requires OpenGL 4.2
		namespace gl420
		{
			using namespace gl410;
		}

		// Requires OpenGL 4.3
		namespace gl430
		{
			using namespace gl420;
		}

		// Requires OpenGL 4.4
		namespace gl440
		{
			using namespace gl430;
		}

		// Requires OpenGL 4.5
		namespace gl450
		{
			using namespace gl440;
			// float gl_CullDistance[];  //vs 4.5
		}

		// gl130 is taken as default
		using namespace gl130;
	}

	// Geometry shader
	namespace gs
	{
		// Requires OpenGL 3.2
		namespace gl150
		{
			//define_output(vec4, gl_Position); TODO ARRAYS
			//define_output(vec1, gl_PointSize); TODO ARRAYS
			//define_constant(vec1, gl_ClipDistance[]; ); TODO ARRAYS
			define_input(ivec1, gl_PrimitiveIDIn);
			define_output(ivec1, gl_PrimitiveID);
			define_output(ivec1, gl_Layer);
		}

		// Requires OpenGL 3.3
		namespace gl330
		{
			using namespace gl150;
		}

		// Requires OpenGL 4.0
		namespace gl400
		{
			using namespace gl330;
			define_input(ivec1, gl_InvocationID);
			// patch out float gl_TessLevelOuter[4]; // gs 4.0
			// patch out float gl_TessLevelInner[2]; // gs 4.0
		}

		// Requires OpenGL 4.1
		namespace gl410
		{
			using namespace gl400;
			define_output(ivec1, gl_ViewportIndex);
		}

		// Requires OpenGL 4.2
		namespace gl420
		{
			using namespace gl410;
		}

		// Requires OpenGL 4.3
		namespace gl430
		{
			using namespace gl420;
		}

		// Requires OpenGL 4.4
		namespace gl440
		{
			using namespace gl430;
		}

		// Requires OpenGL 4.5
		namespace gl450
		{
			using namespace gl440;
			// float gl_CullDistance[]; TODO ARRAYS
		}
	}

	// Tessellation control shader
	namespace tcs
	{
		// Requires OpenGL 4.0
		namespace gl400
		{
			//define_constant(vec4, gl_Position);  TODO ARRAYS
			//define_constant(vec1, gl_PointSize);  TODO ARRAYS
			//define_constant(vec1, gl_ClipDistance[]; ); TODO ARRAYS
			define_input(ivec1, gl_PatchVerticesIn);
			define_input(ivec1, gl_PrimitiveID);
			define_input(ivec1, gl_InvocationID);
		}

		// Requires OpenGL 4.1
		namespace gl410
		{
			using namespace gl400;
		}

		// Requires OpenGL 4.2
		namespace gl420
		{
			using namespace gl410;
		}

		// Requires OpenGL 4.3
		namespace gl430
		{
			using namespace gl420;
		}

		// Requires OpenGL 4.4
		namespace gl440
		{
			using namespace gl430;
		}

		// Requires OpenGL 4.5
		namespace gl450
		{
			using namespace gl440;
			// float gl_CullDistance[]; TODO ARRAYS
		}
	}

	// Tessellation evaluation shader
	namespace tes
	{
		// Requires OpenGL 4.0
		namespace gl400
		{
			//define_constant(vec4, gl_Position);  TODO ARRAYS
			//define_constant(vec1, gl_PointSize);  TODO ARRAYS
			define_input(ivec1, gl_PatchVerticesIn);
			define_input(ivec1, gl_PrimitiveID);
			define_input(vec3, gl_TessCoord);
			// patch in float gl_TessLevelOuter[4];
			// patch in float gl_TessLevelInner[2];
		}

		// Requires OpenGL 4.1
		namespace gl410
		{
			using namespace gl400;
		}

		// Requires OpenGL 4.2
		namespace gl420
		{
			using namespace gl410;
		}

		// Requires OpenGL 4.3
		namespace gl430
		{
			using namespace gl420;
		}

		// Requires OpenGL 4.4
		namespace gl440
		{
			using namespace gl430;
		}

		// Requires OpenGL 4.5
		namespace gl450
		{
			using namespace gl440;
			// float gl_CullDistance[]; TODO ARRAYS
		}
	}

	// Fragment shader
	namespace fs
	{
		// Requires OpenGL 3.0
		namespace gl130
		{
			define_input(vec4, gl_FragCoord);
			define_input(bvec1, gl_FrontFacing);
			define_output(vec1, gl_FragDepth);
			define_output(vec4, gl_FragColor);
			// out vec4 gl_FragData[gl_MaxDrawBuffers]; // fs 1.3
			//in float gl_ClipDistance[];
		}

		// Requires OpenGL 3.1
		namespace gl140
		{
			using namespace gl130;
		}

		// Requires OpenGL 3.2
		namespace gl150
		{
			using namespace gl140;
			define_input(vec2, gl_PointCoord);
			define_input(ivec1, gl_PrimitiveID);
		}

		// Requires OpenGL 3.3
		namespace gl330
		{
			using namespace gl150;
		}

		// Requires OpenGL 4.0
		namespace gl400
		{
			using namespace gl330;
			define_input(ivec1, gl_SampleID);
			define_input(vec2, gl_SamplePosition);
			// out int gl_SampleMask[]; // fs 4.0
		}

		// Requires OpenGL 4.1
		namespace gl410
		{
			using namespace gl400;
		}

		// Requires OpenGL 4.2
		namespace gl420
		{
			using namespace gl410;
			//in int gl_SampleMaskIn[];
		}

		// Requires OpenGL 4.3
		namespace gl430
		{
			using namespace gl420;
			define_input(ivec1, gl_Layer);
			define_input(ivec1, gl_ViewportIndex);
		}

		// Requires OpenGL 4.4
		namespace gl440
		{
			using namespace gl430;
		}

		// Requires OpenGL 4.5
		namespace gl450
		{
			using namespace gl440;
			// float gl_CullDistance[]; TODO ARRAYS
			define_input(bvec1, gl_HelperInvocation);
		}

		// gl130 is taken as default
		using namespace gl130;
	}
}

#undef define_input
#undef define_output
