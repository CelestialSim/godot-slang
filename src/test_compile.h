#ifndef GODOT_SLANG_TEST_COMPILE_H
#define GODOT_SLANG_TEST_COMPILE_H

#include <godot_cpp/classes/object.hpp>
#include <slang.h>

using namespace godot;

class TestCompile : public Object
{
    GDCLASS(TestCompile, Object);

protected:
    static void _bind_methods();

public:
    void test_compile();

    /**
     * @brief Compiles a Slang shader file to GLSL format using the Slang compilation API
     *
     * This function demonstrates how to use the Slang compiler API to cross-compile
     * shader code from Slang to GLSL. The implementation follows the patterns from
     * the createComputePipelineFromShader function in the official Slang hello-world example:
     * https://github.com/shader-slang/slang/blob/master/examples/hello-world/main.cpp
     *
     * @param filename The base filename (without extension) of the Slang shader to compile.
     *                 The function will look for filename.slang and generate filename.glsl
     *
     * @details Process overview:
     * 1. Create a global Slang session (compiler instance)
     * 2. Set up a local session with GLSL target configuration
     * 3. Load the Slang module from the source file
     * 4. Find the compute shader entry point ("computeMain")
     * 5. Compose the module and entry point into a program
     * 6. Generate the target GLSL code
     * 7. Save the result with metadata headers
     *
     * @note Requirements:
     * - Input file must have a compute shader entry point named "computeMain"
     * - Entry point must be tagged with [shader("compute")] attribute
     * - Slang compiler must be properly initialized and linked
     *
     * @note Output:
     * - Generates a .glsl file with Godot-compatible annotations
     * - Includes generation timestamp and shader type information
     * - Targets OpenGL 4.6 GLSL specification
     */
    void compile_slang_to_glsl(const String &filename);
};

#endif // GODOT_SLANG_TEST_COMPILE_H
