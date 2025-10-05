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

## How to build for Linux

2. Build the project (both debug and release)
```bash
scons target=template_debug
# or
scons target=template_release
```

# Acknowledgments

The Slang compilation examples in this project are based on the official [Slang repository examples](https://github.com/shader-slang/slang/tree/master/examples), particularly:
- [hello-world](https://github.com/shader-slang/slang/blob/master/examples/hello-world/main.cpp) for the basic compilation patterns

The documentation generation system (`editor_builders.py`) is adapted from the [threen project](https://github.com/deralmas/threen/tree/doc) by deralmas, which demonstrates embedded XML documentation in Godot GDExtensions.
