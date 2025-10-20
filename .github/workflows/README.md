# GitHub Actions Workflows

This directory contains GitHub Actions workflows for the Godot Slang project.

## Workflows

### `builds.yml` - Cross-Platform Builds
**Triggers:** Push to main, Pull requests, GitHub releases, Manual dispatch

This workflow builds the extension for all supported platforms:

- **Platforms:** Linux, Windows, macOS
- **Architectures:** x86_64, universal (macOS)
- **Build types:** Debug and Release
- Builds Slang library using CMake for each platform
- Builds godot-cpp dependency for each platform
- Builds the godot-slang extension using SCons
- Creates platform-specific artifacts
- Merges all platforms into a single addon and demo package
- Automatically uploads to GitHub releases (when triggered by a release)

**Artifacts:**
- `godot-slang-addon-all-platforms` - Complete addon with all platform binaries
- `godot-slang-demo-all-platforms` - Complete demo project with all platform binaries
- Individual platform artifacts for testing

### `release.yml` - Legacy Release Build (Linux only)
**Triggers:** GitHub releases, Manual dispatch

This workflow creates production-ready releases for Linux only:

- Builds only the release version for Linux
- Uses Ubuntu 25.04 container (matching the dev container)
- Builds Slang generators and static library
- Builds godot-cpp dependency
- Builds the godot-slang extension
- Packages the complete demo folder as a ready-to-use Godot project
- Creates a zip file for easy distribution
- Automatically uploads to GitHub releases (when triggered by a release)

Todo: move built libraries to addons/godot-slang

**Artifacts:**
- `godot-slang-demo-linux` - Complete demo project package
- `godot-slang-demo.zip` - Zipped demo project for distribution

## Build Process

The workflow follows this process:

1. **Environment Setup**: Install build dependencies matching the dev container
2. **Slang Build**: 
   - Build Slang generators using CMake presets
   - Configure and build Slang static library
3. **godot-cpp Build**: Build the Godot C++ bindings (release mode)
4. **Extension Build**: Build the godot-slang extension using SCons (release mode)
5. **Package**: Package the complete demo folder with built binaries

## Usage

### For Releases
1. Create a GitHub release with a tag
2. The release workflow will automatically build and attach the addon package
3. Or manually trigger the release workflow from the Actions tab

### For Development
To test builds during development, manually trigger the release workflow from the Actions tab.

## Requirements

The workflow requires no special setup - it uses the same dependencies as the dev container and should work out of the box on any repository with this project structure.

## Platform Support

The `builds.yml` workflow supports:
- **Linux:** x86_64 (Ubuntu 20.04 runner)
- **Windows:** x86_64 (Windows Latest with MSVC)
- **macOS:** Universal binaries (macOS Latest with Xcode)

Both debug and release builds are created for each platform. The `release.yml` workflow is legacy and only supports Linux x86_64.

## Troubleshooting

- Check the workflow logs in the Actions tab
- Manually trigger the release workflow to test builds
- Verify submodules are properly initialized
- Check that all dependencies are available in the container
