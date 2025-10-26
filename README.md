# Godot Slang

Run [Slang](https://shader-slang.org/) Compute shaders directly in Godot

- No runtime dependencies
- Automatically reimports shader on changes

Keep your existing workflow, to use Slang shaders it is only necessary to change the extension when loading the shader.
```gd
var shader_file := load("res://compute_example.slang")
```
# Demo project
A demo project, godot-slang-demo.zip, based on the [using compute shaders official Godot tutorial](https://docs.godotengine.org/en/4.5/tutorials/shaders/compute_shaders.html) can be dowloaded from the [releases](https://github.com/CelestialSim/godot-slang/releases) section. In the project the addon is already installed and the shader from the documentation is replaced with an equivalent Slang compute shader.

Original GLSL shader
```glsl
#[compute]
#version 450

// Invocations in the (x, y, z) dimension
layout(local_size_x = 2, local_size_y = 1, local_size_z = 1) in;

// A binding to the buffer we create in our script
layout(set = 0, binding = 0, std430) restrict buffer MyDataBuffer {
    float data[];
}
my_data_buffer;

// The code we want to execute in each invocation
void main() {
    // gl_GlobalInvocationID.x uniquely identifies this invocation across all work groups
    my_data_buffer.data[gl_GlobalInvocationID.x] *= 2.0;
}
```

Equivalent Slang shader
```slang
// A binding to the buffer we create in our script
RWStructuredBuffer<float> MyDataBuffer;

[shader("compute")]
// Invocations in the (x, y, z) dimension
[numthreads(2, 1, 1)]
// The code we want to execute in each invocation
void computeMain(uint3 threadId: SV_DispatchThreadID)
{
    // SV_DispatchThreadID.x uniquely identifies this invocation across all work groups
    MyDataBuffer[threadId.x] *= 2;
}
```

# Installattion
To install the Addon in the existing project:
1. Download the godot-slang-addon.zip from the [releases section](https://github.com/CelestialSim/godot-slang/releases/)
2. Open your project in Godot. From the AssetLib section click on import and import the addon.
3. From the top menu open the Project/Project Settings page. Navigate to the Plugins tab and activate the Godot Slang Importer Plugin

Now you will be able to import and use Slang compute shader that define a ComputeMain function like the one shown above.

## Supported Platforms

- 🐧 Linux (x86_64)
- 🏁 Windows (x86_64)
- 🍎 macOS (arm64)

## How to build

Use the devcontainer configured with all the required dependencies or create a similar environment based on the [Dockerfile](https://github.com/CelestialSim/godot-slang/blob/main/.devcontainer/Dockerfile)

To build the project run the following two commands
```bash
scons target=template_debug
scons arget=template_release
```

# Acknowledgments

- The Slang compilation examples in this project are based on the official [Slang repository examples](https://github.com/shader-slang/slang/tree/master/examples), particularly [hello-world](https://github.com/shader-slang/slang/blob/master/examples/hello-world/main.cpp) for the basic compilation patterns
- Another project to use Slang in Godot was very helpful for configuring the CI [DevPrice/godot-slang](https://github.com/DevPrice/godot-slang/tree/main) 
- The documentation generation system (`editor_builders.py`) is adapted from the [threen project](https://github.com/deralmas/threen/tree/doc) by deralmas, which demonstrates embedded XML documentation in Godot GDExtensions.
