#ifndef GODOT_SLANG_SLANG_COMPILER_H
#define GODOT_SLANG_SLANG_COMPILER_H

#include <godot_cpp/classes/object.hpp>
#include <slang.h>

using namespace godot;

class SlangCompiler : public Object
{
    GDCLASS(SlangCompiler, Object);

protected:
    static void _bind_methods();

public:
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
    void to_glsl(const String &filename);

    /**
     * @brief Compiles a Slang shader file to SPIR-V format and returns the bytecode
     *
     * This function uses the Slang compiler API to cross-compile shader code from Slang
     * to SPIR-V binary format, suitable for Vulkan and other SPIR-V compatible graphics APIs.
     *
     * @param filename The base filename (without extension) of the Slang shader to compile.
     *                 The function will look for filename.slang
     *
     * @return PackedByteArray containing the compiled SPIR-V bytecode, or empty array on failure
     *
     * @details Process overview:
     * 1. Create a global Slang session (compiler instance)
     * 2. Set up a local session with SPIR-V target configuration
     * 3. Load the Slang module from the source file
     * 4. Find the compute shader entry point ("computeMain")
     * 5. Compose the module and entry point into a program
     * 6. Generate the target SPIR-V code
     * 7. Return the binary result as PackedByteArray
     *
     * @note Requirements:
     * - Input file must have a compute shader entry point named "computeMain"
     * - Entry point must be tagged with [shader("compute")] attribute
     * - Slang compiler must be properly initialized and linked
     *
     * @note Output:
     * - Returns SPIR-V binary data as PackedByteArray
     * - Targets SPIR-V 1.5 specification
     * - Does not write to disk, only returns bytecode
     */
    PackedByteArray to_spirv_bytes(const String &filename);
};

#endif // GODOT_SLANG_SLANG_COMPILER_H
