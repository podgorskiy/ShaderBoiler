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

#define define_constant(T, X) static const T X(#X, detail::node::builtin_variable)

namespace sb
{
	// Implementation dependent constants.

	// Requires OpenGL 3.0
	namespace gl130
	{
		define_constant(ivec1, gl_MaxTextureUnits);
		define_constant(ivec1, gl_MaxVertexAttribs);
		define_constant(ivec1, gl_MaxVertexUniformComponents);
		define_constant(ivec1, gl_MaxVaryingComponents);
		define_constant(ivec1, gl_MaxVertexTextureImageUnits);
		define_constant(ivec1, gl_MaxCombinedTextureImageUnits);
		define_constant(ivec1, gl_MaxTextureImageUnits);
		define_constant(ivec1, gl_MaxFragmentUniformComponents);
		define_constant(ivec1, gl_MaxDrawBuffers);
		define_constant(ivec1, gl_MaxClipDistances);
	}

	// Requires OpenGL 3.1
	namespace gl140
	{
		using namespace gl130;
	}

	// Requires OpenGL 3.2
	namespace gl150
	{
		define_constant(ivec1, gl_MaxVertexOutputComponents);
		define_constant(ivec1, gl_MaxGeometryInputComponents);
		define_constant(ivec1, gl_MaxGeometryOutputComponents);
		define_constant(ivec1, gl_MaxFragmentInputComponents);
		define_constant(ivec1, gl_MaxGeometryTextureImageUnits);
		define_constant(ivec1, gl_MaxGeometryOutputVertices);
		define_constant(ivec1, gl_MaxGeometryTotalOutputComponents);
		define_constant(ivec1, gl_MaxGeometryUniformComponents);
		define_constant(ivec1, gl_MaxGeometryVaryingComponents);
	}

	// Requires OpenGL 3.3
	namespace gl330
	{
		using namespace gl150;
	}

	// Requires OpenGL 4.0
	namespace gl400
	{
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
	}

	// Requires OpenGL 4.1
	namespace gl410
	{
		using namespace gl400;
		define_constant(ivec1, gl_MaxViewports);
		define_constant(ivec1, gl_MaxVertexUniformVectors);
		define_constant(ivec1, gl_MaxFragmentUniformVectors);
		define_constant(ivec1, gl_MaxVaryingVectors);
	}

	// Requires OpenGL 4.2
	namespace gl420
	{
		using namespace gl420;
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
	}

	// Requires OpenGL 4.3
	namespace gl430
	{
		using namespace gl420;
		define_constant(ivec3, gl_MaxComputeWorkGroupCount);
		define_constant(ivec3, gl_MaxComputeWorkGroupSize);
		define_constant(ivec1, gl_MaxComputeUniformComponents);
		define_constant(ivec1, gl_MaxComputeTextureImageUnits);
		define_constant(ivec1, gl_MaxComputeImageUniforms);
		define_constant(ivec1, gl_MaxComputeAtomicCounters);
		define_constant(ivec1, gl_MaxComputeAtomicCounterBuffers);
	}

	// Requires OpenGL 4.4
	namespace gl440
	{
		using namespace gl430;
		define_constant(ivec1, gl_MaxTransformFeedbackBuffers);
		define_constant(ivec1, gl_MaxTransformFeedbackInterleavedComponents);
	}

	// Requires OpenGL 4.5
	namespace gl450
	{
		using namespace gl440;
		define_constant(ivec1, gl_MaxCullDistances);
		define_constant(ivec1, gl_MaxCombinedClipAndCullDistances);
		define_constant(ivec1, gl_MaxSamples);
		define_constant(ivec1, gl_MaxCombinedShaderOutputResources);
	}

	// gl130 is taken as default
	using namespace gl130;
}

#undef define_constant
