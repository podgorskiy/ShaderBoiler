# ShaderBoiler

**ShaderBoiler** is a C++, header only library for generating GLSL code out of C++ constructs. 

Have you ever seen code like this in OpenGL related applications?

```cpp
static const char * vs_source[] =
    {
        "void main(void)                               \n"
        "{                                             \n"
        "    gl_Position = vec4(0.0, 0.0, 0.0, 1.0);   \n"
        "}                                             \n"
    };
```

I've seen a lot, in tutorials, examples, small projects, big projects, etc.. In some cases, the process is automated, and C++ headers files are generated from shader source code. In the other cases, shaders are read from files at runtime, which is a good option in general but can be annoying if you want to keep things simple. 

Another problem is when shader should be compiled in many variations. Typically, the preprocessor is heavily used, which makes code hard to read. Also, GLSL preprocessor has limitations, which are typically solved by using an external preprocessor. However, if shader has to be generated dynamically, using the only preprocessor can be problematic.


**ShaderBoiler** can be helpful for:

1. Adding inlined shader into cpp code without using string literals.
2. Generating shader dynamically on-the-fly using C++ as a preprocessor language.

## Showcase

Let's consider the following short C++ code, which you can find [here]( https://github.com/podgorskiy/ShaderBoiler/blob/master/examples/hellow_world/main.cpp)

```cpp
#include <shaderboiler.h>
#include <iostream>

void main()
{
	using namespace sb;

	context ctx;
	vec3 AlbedoColor           = ctx.uniform<vec3>("AlbedoColor");
	vec3 AmbientLightColor     = ctx.uniform<vec3>("AmbientLightColor");
	vec3 DirectLightColor      = ctx.uniform<vec3>("DirectLightColor");
	vec3 LightPosition         = ctx.uniform<vec3>("LightPosition");

	vec3 normal   = ctx.in<vec3>("normal");
	vec3 position = ctx.in<vec3>("position");
	vec4& color   = ctx.out<vec4>("color");

	vec3 normalized_normal = normalize(normal);

	vec3 fragmentToLight = LightPosition - position;

	Float squaredDistance = dot(fragmentToLight, fragmentToLight);

	vec3 normalized_fragmentToLight = fragmentToLight / sqrt(squaredDistance);

	Float NdotL = dot(normal, normalized_fragmentToLight);

	vec3 DiffuseTerm = max(NdotL, 0.0) * DirectLightColor / squaredDistance;

	color = vec4(AlbedoColor * (AmbientLightColor + DiffuseTerm), 1.0);

	std::cout << ctx.genShader();
}
```

The output to the console will be:

```GLSL
uniform vec3 AlbedoColor;
uniform vec3 AmbientLightColor;
uniform vec3 LightPosition;
uniform vec3 DirectLightColor;

in vec3 normal;
in vec3 position;

out vec4 color;

void main(void)
{
        vec3 sb_b = LightPosition - position;
        float sb_a = dot(sb_b, sb_b);
        color = vec4(AlbedoColor * (AmbientLightColor + max(dot(normal, sb_b / sqrt(sb_a)), 0.0000000) * DirectLightColor / sb_a), 1.000000);
}
```

How does it work? The library provides a set of datatypes which are similar to ones in the GLSL. However, execution of a shader written in C++ does not perform any computation but creates a data-flow graph, which is used later to auto-generate shader code. The concept is somewhat similar to the concept used in [tensorflow](https://github.com/tensorflow/tensorflow) and similar packages, but instead of performing the actual computation, the source code is generated.

## Advantages

- Dead code elimination. This happens naturally, during data-flow graph construction.
- Allows using C++ constructs, such as conditionals (if - else) and loop controls as a high-level "preprocessor".
- Makes easy creating shader factories in C++.
- Code is syntactically checked at compile time of C++ code.  

## Limitations

This is an early alpha version of the library, and it has a lot of limitations (which will be solved soon hopefully), such as:

- No matrixes
- No user defined functions
- No interfaces blocks
- No conditionals
- No loop controls

## Perspectives

Eliminating specified above limitations will lead to a C++ tool for dynamic shader generations. However, the use cases may be broader than just shaders.

Even this alpha version of the library can be used to generate compute shaders, which are essentially kernels. I'm also considering creating a port of the library to generate OpenCL and CUDA code for runtime kernel generation.

## Installation

The library is header only, no explicit installation is required. All that is needed - specify include path to the *include*
 directory.
 
The library does not have any dependencies, all you need is C++11 compatible compiler.
 
 ## Building examples
 
To build examples, you will need CMake and C++11 compatible compiler. Use the CMakeLists.txt file in the root of the repository.
 
For example, assuming that currently, you are in the root directory of the cloned repository:
 
 ```
 mkdir build
 cd build
 cmake ../
 ```
 
 Then on a Unix-like system just type `make`, or open generated solution file on Windows. 
 
 ## Basic usage
 
 ### Namespaces
 
 All API is placed inside namespace **sb** (shaderboiler). However, there is special namespace structure for built-in variables and constants that allow having control  over the usage of different GLSL versions

By default GLSL version 1.30 is assumed (OpenGL 3.0). Built-in constants are placed directly in **sb** namespace, built-in variables are places in:

- **cs** namespace for compute shaders
- **vs** namespace for vertex shaders
- **gs** namespace for geometry shaders
- **tcs** namespace for tesselation control shaders
- **tes** namespace for tesselation evaluation shaders
- **fs** namespace for fragment shaders
  
To enable usage of variables and constants from versions of GLSL other than 1.30, you need to add the corresponding namespace:
  
- gl140
- gl150
- gl330
- gl400
- gl410
- gl420
- gl430
- gl440
- gl450
  
For example, let's consider fragment shader for GLSL version 3.30:
  
```cpp
using namespace sb;
using namespace sb::gl330; // to have access to built-in constants of GLSL version 3.30
using namespace sb::fs::gl330; // to have access to built-in variables of fragment shader of GLSL version 3.30
```
  
For more information refer to [builetin_variables.h](https://github.com/podgorskiy/ShaderBoiler/blob/master/include/builetin_variables.h) and  [builetin_constants.h](https://github.com/podgorskiy/ShaderBoiler/blob/master/include/builetin_constants.h).
  
### Context.

#### Creating context.

Creating context is the starting point of using the library. First of all, you will need to include the header *shaderboiler.h*:

```cpp
#include <shaderboiler.h>
```
Then create an instance of class **sb::context**. For example:

```cpp
using namespace sb;
context ctx;
```

#### Input/Output variables.

To write code that generates shader, you will need to start with defining inputs and outputs.

Inputs are variables that bring data in the shader. Variables with storage qualifier **in**, **uniform**, **attribute** for vertex shader and **varying** for fragment shaders are inputs. Inputs should not be modified in the shader. There are built-in inputs for some types of shaders.

Outputs are variables where the shader writes the result of the computation to. Variables with storage qualifier **out** and **varying** fragment shaders are outputs. There are built-in outputs for some types of shaders.

To create inputs and outputs, you need to create an instance of the context. All input/output variables created with an instance of the context and all derived variables are valid while the instance of context is not deconstructed. After deconstructor of the instance of the context was called, all associated variables are invalidated and any usage of them will result in Undefined Behaviour.

To create input/output variables use the following methods of the context:

```cpp
template<typename T>
T context::uniform(const std::string& name)

template<typename T>
T context::in(const std::string& name)

template<typename T>
T& context::out(const std::string& name)

template<typename T>
T& context::varying(const std::string& name)

template<typename T>
T context::attribute(const std::string& name)

template<typename T>
T& context::buffer(const std::string& name)

template<typename T>
T& context::shared(const std::string& name)

template<typename T>
T& context::shared(const std::string& name)
```

Where *typename T* is one of the following supported types (or arrays of the following types):

- vec1
- vec2
- vec3
- vec4
- bvec1
- bvec2
- bvec3
- bvec4
- ivec1
- ivec2
- ivec3
- ivec4
- uvec1
- uvec2
- uvec3
- uvec4

And *const std::string& name* - is the name of the variable that should be used in the generated shader.

Types *vec1, bvec1, ivec1, uvec1* are equivalents of corresponding *float, bool, int, uint* in GLSL. There are also aliases: *Float, Bool, Int, UInt*. Direct usage of *float, bool, int, uint* names is not possible because they are reserved keywords in C++.

For example, to create an input variable "position" of type *vec3*:

```cpp
vec3 position = ctx.in<vec3>("normal");
```

To create a uniform variable "DiffuseColor" of type *vec4*:

```cpp
vec4 diffuseColor = ctx.uniform<vec4>("DiffuseColor");
```

To create an output variable, you need to use reference, because otherwise, it will create a copy of the output variable which won't produce expected result.

Correct way of using output variables:

```cpp
vec4& color = ctx.out<vec4>("color");
color = vec4(1.0);
```

Another correct way:

```cpp
ctx.out<vec4>("color") = vec4(1.0);
```

Incorrect way:

```cpp
vec4 color = ctx.out<vec4>("color");
color = vec4(1.0); // Won't work!
```

In the last case, modifying *color* variable won't change the actual output variable created in the context.

All built-in constants and input variables are globals, defined in the corresponding namespace. Thus, they can be used directly:

```cpp
ivec1 drawBufferCount = gl_MaxDrawBuffers;
vec4 coord = gl_FragCoord;
```

Because inputs and constants can not be modified from a shader, they are defined as constant globals and are not bound to any specific context. That is not true for built-in output variables. Built-in variables are defined the same way as constants and inputs, but indirectly - through proxies. To use built-in output variables, you will need to instantiate it first using index access operator of the context:

```cpp
template<typename T>
T& context::operator [] (const detail::output_proxy<T>& l)
```

For example, to use *gl_FragColor* output:

```cpp
ctx[gl_FragColor] = vec4(1.0);
```

Or:

```cpp
vec4& gl_FragColor = ctx[gl_FragColor];
gl_FragColor = vec4(1.0);
```

But not:

```cpp
vec4 gl_FragColor = ctx[gl_FragColor];
gl_FragColor = vec4(1.0); // Won't work!
```

#### Generating shader.

After all output variables were assigned, the shader can be generated using the method:

```cpp
std::string context::genShader();
```

All assignments and operations that do not affect output variables won't present in generated code.

For example, the following code:

```cpp	
context ctx;
vec4 color = vec4(1.0);
color *= 2.0f;
color = vec4(0.0);
ctx.out<vec4>("colorOut") = color;

std::cout << ctx.genShader();
```

Will produce the following result:

```GLSL
out vec4 colorOut;

void main(void)
{
        colorOut = vec4(0.0000000, 0.0000000, 0.0000000, 0.0000000);
}
```

Note, that assignment *vec4(0.0)* to the *color* overridden previous manipulations.
