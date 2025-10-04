#!/usr/bin/env python
import os
import sys

LIB_NAME = "libgodot-slang" # Must have "lib" as prefix

BUILD_PATH = "demo/addons/godot_slang_importer"

SLANG_DEBUG_INCLUDE_PATH = "slang/build/RelWithDebInfo/include/"
SLANG_DEBUG_LIB_PATHS = [
    "slang/build/RelWithDebInfo/lib/",
    "slang/build/external/miniz/RelWithDebInfo/",
    "slang/build/external/lz4/build/cmake/RelWithDebInfo/"
]

SLANG_RELEASE_INCLUDE_PATH = "slang/build/Release/include/"
SLANG_RELEASE_LIB_PATHS = [
    "slang/build/Release/lib/",
    "slang/build/external/miniz/Release/",
    "slang/build/external/lz4/build/cmake/Release/"
]

SLANG_LIBS = ["slang", "core", "compiler-core", "miniz", "lz4"]


env = SConscript("godot-cpp/SConstruct")

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

import editor_builders

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

Default(library)

