# Godot Slang

Run [Slang](https://shader-slang.org/) shaders directly in Godot

- No runtime dependencies
- Automatically updates when the Slang shader changes

# Installation
1. Dowload the latest release
2. Copy the bin and addons/godot_slang_importer in your project
3. From the top menu open the Project/Project Settings page. Navigate to the Plugins tab and activate the Godot Slang Importer Plugin
4. A corresponding .glsl file will be created for each .slang file

**WORK IN PROGRESS!**

## How to build for Linux

1. Build Slang like this:
```
# Build the generators
# You need a recent CMake verstion
cd slang
cmake --workflow --preset generators --fresh
mkdir build-platform-generators
cmake --install build --config Release --prefix ../build-platform-generators --component generators

# Set up compiling environment
cmake \
  --preset default \
  --fresh \
  -DSLANG_LIB_TYPE=STATIC \
  -DSLANG_GENERATORS_PATH=../build-platform-generators/bin \
  -Dwhatever-other-necessary-options-for-your-cross-build \
  # for example \
  -DCMAKE_C_COMPILER=my-arch-gcc \
  -DCMAKE_CXX_COMPILER=my-arch-g++

# Build slang library
cmake --build --preset release --target slang
```

2. Build the project
```
scons target=template_release
```

# Acknowledgments

The Slang compilation examples in this project are based on the official [Slang repository examples](https://github.com/shader-slang/slang/tree/master/examples), particularly:
- [hello-world](https://github.com/shader-slang/slang/blob/master/examples/hello-world/main.cpp) for the basic compilation patterns

The documentation generation system (`editor_builders.py`) is adapted from the [threen project](https://github.com/deralmas/threen/tree/doc) by deralmas, which demonstrates embedded XML documentation in Godot GDExtensions.
