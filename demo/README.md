# Godot Slang Extension Demo

This is a demo project showcasing the Godot Slang extension, which integrates the Slang shader language into Godot.

## About

The Godot Slang extension allows you to write and compile Slang shaders directly in your Godot projects, providing advanced shader capabilities and cross-platform shader compilation.

## Installation

1. Copy this entire `demo` folder to your Godot project directory
2. The extension should be automatically detected by Godot
3. Open the project in Godot and explore the example scenes and scripts

## Contents

- `bin/` - Contains the compiled GDExtension binaries
- `addons/godot_slang_importer` folder contains the EditorImportPlugin that can be activated to automatically import slang shaders in the project
- `scenes/` - Example scenes demonstrating Slang shader usage
- `Scripts/` - Example scripts showing how to work with Slang shaders
- `project.godot` - Godot project file
- `*.slang` - Example Slang shader files

## Project Repository

For more information, documentation, and the latest updates, visit the main project repository:

**https://github.com/celestialsim/godot-slang**

## Platform Support

This demo includes binaries for:
- Linux x86_64

## License

See the LICENSE file in the main repository for licensing information.

## Getting Started

1. Open this project in Godot
2. Navigate to the `scenes/` folder and open `Main.tscn`
3. Run the scene to compile the Slang shader into a glsl file
4. Examine the `.slang` files to understand the shader syntax
5. Check the scripts in `Scripts/` to see how to load and use Slang shaders from code

## Support

For issues, questions, or contributions, please visit the GitHub repository linked above.
