#!/usr/bin/env python
import os
import sys
import subprocess
import shutil
import editor_builders

LIB_NAME = "libgodot-slang" # Must have "lib" as prefix

BUILD_PATH = "demo/addons/godot_slang_importer"

SLANG_DEBUG_INCLUDE_PATH = "slang/build/Debug/include/"
SLANG_DEBUG_LIB_PATHS = [
    "slang/build/Debug/lib/",
    "slang/build/external/miniz/",
    "slang/build/external/miniz/Debug/",
    "slang/build/external/lz4/build/cmake/",
    "slang/build/external/lz4/build/cmake/Debug/"
]

SLANG_RELEASE_INCLUDE_PATH = "slang/build/Release/include/"
SLANG_RELEASE_LIB_PATHS = [
    "slang/build/Release/lib/",
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
    build_type = "Release" if env["target"] == "template_release" else "Debug"
    
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
        
        result = subprocess.run(configure_cmd, cwd=slang_dir)
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
    result = subprocess.run(build_cmd, cwd=slang_dir)
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

if env["target"] == "template_release":
    env.Append(CPPPATH=SLANG_RELEASE_INCLUDE_PATH)
    env.Append(LIBPATH=SLANG_RELEASE_LIB_PATHS)
else:
    env.Append(CPPPATH=SLANG_DEBUG_INCLUDE_PATH)
    env.Append(LIBPATH=SLANG_DEBUG_LIB_PATHS)
env.Append(LIBS=SLANG_LIBS)

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

