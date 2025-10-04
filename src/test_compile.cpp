#include "test_compile.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/godot.hpp>
#include <iostream>
#include <vector>
#include "slang-com-ptr.h"

using namespace godot;
using Slang::ComPtr;

void TestCompile::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("test_compile"), &TestCompile::test_compile);
    ClassDB::bind_method(D_METHOD("compile_slang_to_glsl", "filename"), &TestCompile::compile_slang_to_glsl);
}

void TestCompile::test_compile()
{
    UtilityFunctions::print("This is a compilation test that can be invoked from Godot");
    // compile_slang_to_glsl("shader");
}

void TestCompile::compile_slang_to_glsl(const String &filename)
{
    // Helper function to print diagnostics if any
    auto diagnoseIfNeeded = [](ComPtr<slang::IBlob> diagnostics)
    {
        if (diagnostics && diagnostics->getBufferSize() > 0)
        {
            String diagnostic_text = String::utf8(static_cast<const char *>(diagnostics->getBufferPointer()), diagnostics->getBufferSize());
            UtilityFunctions::print("Slang diagnostics: " + diagnostic_text);
        }
    };

    // Create the global session - using ComPtr for proper resource management
    ComPtr<slang::IGlobalSession> slangGlobalSession;
    SlangResult result = slang::createGlobalSession(slangGlobalSession.writeRef());
    if (SLANG_FAILED(result))
    {
        UtilityFunctions::print("Failed to create Slang global session");
        return;
    }

    // Create a compilation session to generate GLSL code from Slang source
    slang::SessionDesc sessionDesc = {};
    slang::TargetDesc targetDesc = {};
    targetDesc.format = SLANG_GLSL;
    targetDesc.profile = slangGlobalSession->findProfile("glsl_460");
    targetDesc.flags = 0;

    sessionDesc.targets = &targetDesc;
    sessionDesc.targetCount = 1;

    ComPtr<slang::ISession> session;
    result = slangGlobalSession->createSession(sessionDesc, session.writeRef());
    if (SLANG_FAILED(result))
    {
        UtilityFunctions::print("Failed to create Slang session");
        return;
    }

    // Load the Slang module
    slang::IModule *slangModule = nullptr;
    {
        ComPtr<slang::IBlob> diagnosticBlob;
        String slang_file = filename + String(".slang");
        slangModule = session->loadModule(slang_file.utf8().get_data(), diagnosticBlob.writeRef());
        diagnoseIfNeeded(diagnosticBlob);
        if (!slangModule)
        {
            UtilityFunctions::print("Error loading Slang module: " + slang_file);
            return;
        }
    }

    UtilityFunctions::print("Loaded Slang module successfully!");

    // Find the entry point by name (assuming "computeMain" or similar)
    ComPtr<slang::IEntryPoint> entryPoint;
    slangModule->findEntryPointByName("computeMain", entryPoint.writeRef());
    if (!entryPoint)
    {
        UtilityFunctions::print("Could not find entry point 'computeMain'. Make sure your shader has [shader(\"compute\")] void computeMain(...)");
        return;
    }

    // Create a composite component type from the module and entry point
    std::vector<slang::IComponentType *> componentTypes;
    componentTypes.push_back(slangModule);
    componentTypes.push_back(entryPoint);

    ComPtr<slang::IComponentType> composedProgram;
    {
        ComPtr<slang::IBlob> diagnosticsBlob;
        result = session->createCompositeComponentType(
            componentTypes.data(),
            (SlangInt)componentTypes.size(),
            composedProgram.writeRef(),
            diagnosticsBlob.writeRef());
        diagnoseIfNeeded(diagnosticsBlob);
        if (SLANG_FAILED(result))
        {
            UtilityFunctions::print("Failed to compose program");
            return;
        }
    }

    // Generate the target code (GLSL)
    ComPtr<slang::IBlob> shaderCode;
    {
        ComPtr<slang::IBlob> diagnosticsBlob;
        result = composedProgram->getEntryPointCode(
            0, // entryPointIndex
            0, // targetIndex
            shaderCode.writeRef(),
            diagnosticsBlob.writeRef());
        diagnoseIfNeeded(diagnosticsBlob);
        if (SLANG_FAILED(result))
        {
            UtilityFunctions::print("Compilation failed!");
            return;
        }
    }

    String output_message = String("Successfully compiled ") + String::num_int64(shaderCode->getBufferSize()) + String(" bytes of GLSL code.");
    UtilityFunctions::print(output_message);

    // Save the compiled shader code to a GLSL file
    String output_filename = filename + String(".glsl");
    Ref<FileAccess> file = FileAccess::open(output_filename, FileAccess::WRITE);
    if (file.is_valid())
    {
        // Get current date and time
        Time *time = Time::get_singleton();
        Dictionary datetime = time->get_datetime_dict_from_system();

        String current_date = String::num_int64(datetime["year"]) + "-" +
                              String::num_int64(datetime["month"]).pad_zeros(2) + "-" +
                              String::num_int64(datetime["day"]).pad_zeros(2);
        String current_time = String::num_int64(datetime["hour"]).pad_zeros(2) + ":" +
                              String::num_int64(datetime["minute"]).pad_zeros(2) + ":" +
                              String::num_int64(datetime["second"]).pad_zeros(2);

        // Add generation comment with dynamic date/time and shader type
        file->store_string("// File generated from godot-slang on " + current_date + " at " + current_time + "\n");
        file->store_string("// Shader type: GLSL Compute Shader (OpenGL 4.6)\n");
        file->store_string("#[compute]\n\n");

        // Convert the shader code to a string since it's now GLSL text
        String shader_text = String::utf8(static_cast<const char *>(shaderCode->getBufferPointer()), shaderCode->getBufferSize());
        file->store_string(shader_text);
        file->close();
        UtilityFunctions::print("Shader saved to: " + output_filename);
    }
    else
    {
        UtilityFunctions::print("Failed to save shader to: " + output_filename);
    }
}
