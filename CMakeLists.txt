cmake_minimum_required(VERSION 2.8.12)

project(ShaderBoiler)

option(SB_BUILD_EXAMPLES "Build shaderboiler examples" ON)

include_directories(include)

if (SB_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
