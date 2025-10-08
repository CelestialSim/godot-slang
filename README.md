# Godot Slang

Run [Slang](https://shader-slang.org/) shaders directly in Godot

- No runtime dependencies
- Automatically updates when the Slang shader changes

# Installation
1. Download the latest release
2. Copy the `addons/godot_slang_importer` folder (which includes the bin directory) to your project's `addons` folder
3. From the top menu open the Project/Project Settings page. Navigate to the Plugins tab and activate the Godot Slang Importer Plugin
4. A corresponding .glsl file will be created for each .slang file

**WORK IN PROGRESS!**

## Supported Platforms

- 🐧 Linux (x86_64)
- 🏁 Windows (x86_64)
- 🍎 macOS (Universal)

## How to build

The project uses SCons for building. You need to have the following installed:
- Python 3.x
- SCons
- CMake
- Ninja (or other build system)
- Platform-specific development tools (Visual Studio for Windows, Xcode for macOS, GCC/Clang for Linux)

### Linux
```bash
scons platform=linux arch=x86_64 target=template_debug
scons platform=linux arch=x86_64 target=template_release
```

### Windows
```bash
scons platform=windows arch=x86_64 target=template_debug
scons platform=windows arch=x86_64 target=template_release
```

### macOS
```bash
scons platform=macos arch=universal target=template_debug
scons platform=macos arch=universal target=template_release
```

# Acknowledgments

The Slang compilation examples in this project are based on the official [Slang repository examples](https://github.com/shader-slang/slang/tree/master/examples), particularly:
- [hello-world](https://github.com/shader-slang/slang/blob/master/examples/hello-world/main.cpp) for the basic compilation patterns

The documentation generation system (`editor_builders.py`) is adapted from the [threen project](https://github.com/deralmas/threen/tree/doc) by deralmas, which demonstrates embedded XML documentation in Godot GDExtensions.
