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

### `release.yml` - Create Release
**Triggers:** Manual dispatch with tag input

This workflow creates a new GitHub release:

- Takes a tag name as input (e.g., v1.0.0)
- Reuses the `builds.yml` workflow to build all platform artifacts
- Generates release notes from merged PRs since the previous tag
- Creates a git tag at the current commit
- Creates a GitHub release with the tag
- Automatically attaches the addon and demo packages to the release

**Inputs:**
- `tag` - The release tag name (required, e.g., v1.0.0)

**Release Notes Include:**
- List of merged pull requests since the previous tag
- List of contributors in the release

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
1. Navigate to the Actions tab on GitHub
2. Select the "Create Release" workflow
3. Click "Run workflow"
4. Enter a tag name (e.g., v1.0.0)
5. The workflow will:
   - Build all platform artifacts
   - Generate release notes from merged PRs
   - Create the tag and release
   - Attach the addon and demo packages

### For Development
To test builds during development, manually trigger the builds workflow from the Actions tab.

## Requirements

The workflow requires no special setup - it uses the same dependencies as the dev container and should work out of the box on any repository with this project structure.

## Platform Support

The `builds.yml` workflow supports:
- **Linux:** x86_64 (Ubuntu 20.04 runner)
- **Windows:** x86_64 (Windows Latest with MSVC)
- **macOS:** Universal binaries (macOS Latest with Xcode)

Both debug and release builds are created for each platform.

## Troubleshooting

- Check the workflow logs in the Actions tab
- Manually trigger the release workflow to test builds
- Verify submodules are properly initialized
- Check that all dependencies are available in the container
