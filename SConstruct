#!/usr/bin/env python
import os
import sys
import subprocess
import shutil
import editor_builders

LIB_NAME = "libgodot-slang" # Must have "lib" as prefix

BUILD_PATH = "demo/addons/godot_slang_importer"

# Slang is always built in Release mode to match godot-cpp's CRT settings
# (both template_debug and template_release use release CRT with static linkage)
SLANG_INCLUDE_PATHS = [
    "slang/build/Release/include/",  # Single-config generators (Ninja, Makefiles)
    "slang/build/include/",           # Multi-config generators (Visual Studio)
]
SLANG_LIB_PATHS = [
    "slang/build/Release/lib/",       # Single-config generators
    "slang/build/lib/Release/",       # Multi-config generators (Visual Studio)
    "slang/build/external/miniz/",
    "slang/build/external/miniz/Release/",
    "slang/build/external/lz4/build/cmake/",
    "slang/build/external/lz4/build/cmake/Release/"
]

SLANG_LIBS = ["slang", "compiler-core", "core",  "miniz", "lz4"]


def build_slang(target, source, env):
    print(f"Environment target: {env['target']}")
    """Build Slang library using CMake"""
    slang_dir = "slang"
    build_dir = os.path.join(slang_dir, "build")
    # Both template_debug and template_release use Release build for Slang
    # This matches godot-cpp's optimization settings and CRT linkage
    build_type = "Release"
    
    # Prepare environment for subprocess - inherit current environment to get ccache settings
    cmake_env = os.environ.copy()
    
    # Step 1: Configure with CMake (only if not already configured)
    configure_file = os.path.join(build_dir, "CMakeCache.txt")
    if not os.path.exists(configure_file):
        print(f"Configuring Slang with CMake (STATIC library)...")
        os.makedirs(build_dir, exist_ok=True)
        
        # Use standard CMake configuration approach instead of presets for cross-platform compatibility
        configure_cmd = [
            "cmake",
            "-S", ".",
            "-B", "build",
            "-DSLANG_LIB_TYPE=STATIC",
            "-DCMAKE_BUILD_TYPE=" + build_type
        ]
        
        # On Windows, match godot-cpp's CRT linkage (static CRT by default)
        # This prevents LNK2038 errors about RuntimeLibrary mismatch
        if env["platform"] == "windows":
            # Use static CRT (/MT for release, /MTd for debug) to match godot-cpp's default
            configure_cmd.append("-DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded$<$<CONFIG:Debug>:Debug>")
        
        # Enable ccache if available for faster rebuilds
        if shutil.which("ccache"):
            configure_cmd.extend([
                "-DCMAKE_C_COMPILER_LAUNCHER=ccache",
                "-DCMAKE_CXX_COMPILER_LAUNCHER=ccache"
            ])
        
        # Use appropriate generator based on platform
        if env["platform"] == "windows":
            configure_cmd.extend(["-G", "Visual Studio 17 2022"])
        else:
            # Use Ninja generator if available on other platforms
            try:
                ninja_check = subprocess.run(["ninja", "--version"], capture_output=True)
                if ninja_check.returncode == 0:
                    configure_cmd.extend(["-G", "Ninja"])
            except FileNotFoundError:
                pass
        
        result = subprocess.run(configure_cmd, cwd=slang_dir, env=cmake_env)
        if result.returncode != 0:
            print("Error: CMake configuration failed!")
            return result.returncode
    
    # Step 2: Build with CMake
    print(f"Building Slang library ({build_type})...")
    build_cmd = [
        "cmake",
        "--build", "build",
        "--config", build_type,
        "--target", "slang",
        "--parallel"
    ]
    result = subprocess.run(build_cmd, cwd=slang_dir, env=cmake_env)
    if result.returncode != 0:
        print("Error: CMake build failed!")
        return result.returncode
    
    print(f"Slang library built successfully ({build_type})")
    return None


env = SConscript("godot-cpp/SConstruct")

# Build Slang library first
slang_marker = "slang/.slang_built"
slang_build = env.Command(slang_marker, [], env.Action(build_slang, "Building Slang library..."))
env.AlwaysBuild(slang_build)

# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["src/"])
sources = Glob("src/*.cpp")

# Make source files depend on Slang build (so headers are available during compilation)
for src in sources:
    env.Depends(src, slang_build)

# Build docs
docs_xml = []
docs_xml += Glob("src/doc_classes/*.xml")
docs_xml = sorted(docs_xml)
docs_header = "src/doc_data_godot_slang.gen.h"
env.Command(docs_header, docs_xml, env.Action(editor_builders.make_doc_header, "Generating documentation header."))

# Always use Release build paths for Slang (matches CRT settings)
env.Append(CPPPATH=SLANG_INCLUDE_PATHS)
env.Append(LIBPATH=SLANG_LIB_PATHS)
env.Append(LIBS=SLANG_LIBS)

# Add Windows system libraries required by Slang
if env["platform"] == "windows":
    env.Append(LIBS=["ole32", "advapi32"])

# Define SLANG_STATIC since we're linking against a static library
# This is required on Windows to prevent __declspec(dllimport) decoration
env.Append(CPPDEFINES=["SLANG_STATIC"])

file_name = LIB_NAME + "." + env["target"][9:] # Trim "template_"

if env["platform"] == "macos":
    file_path = "{0}/bin/{1}.{2}/{3}.framework/{3}".format(
        BUILD_PATH, env["platform"], env["arch"], file_name
    )
    library = env.SharedLibrary(file_path, source=sources)
elif env["platform"] == "ios":
    file_path = "{0}/bin/{1}.{2}/{3}{4}.a".format(
        BUILD_PATH, env["platform"], env["arch"], file_name, ".simulator" if env["ios_simulator"] else ""
    )
    library = env.StaticLibrary(file_path, source=sources)
else:
    file_path = "{0}/bin/{1}.{2}/{3}{4}".format(
        BUILD_PATH, env["platform"], env["arch"], file_name, env["SHLIBSUFFIX"]
    )
    library =  env.SharedLibrary(file_path, source=sources)

# Make library depend on Slang build
env.Depends(library, slang_build)

scons_cache_path = os.environ.get("SCONS_CACHE")
if scons_cache_path is not None:
    CacheDir(scons_cache_path)
    Decider("MD5")

Default(library)

