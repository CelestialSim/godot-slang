#include "register_types.h"
#include "slang_compiler.h"
#include "doc_data_godot_slang.gen.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

void initialize_module(ModuleInitializationLevel p_level)
{
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		ClassDB::register_class<SlangCompiler>();
	}

	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR)
	{
		typedef void (*GDExtensionInterfaceEditorHelpLoadXMLFromUTF8CharsAndLen)(const char *p_data, int p_size);
		GDExtensionInterfaceEditorHelpLoadXMLFromUTF8CharsAndLen editor_help_load_xml_from_utf8_chars_and_len =
			(GDExtensionInterfaceEditorHelpLoadXMLFromUTF8CharsAndLen)internal::gdextension_interface_get_proc_address("editor_help_load_xml_from_utf8_chars_and_len");

		if (editor_help_load_xml_from_utf8_chars_and_len)
		{
			editor_help_load_xml_from_utf8_chars_and_len(_doc_data, _doc_data_size);
		}
	}
}

void uninitialize_module(ModuleInitializationLevel p_level)
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		return;
	}
}

extern "C"
{
	// Initialization.
	GDExtensionBool GDE_EXPORT godot_slang_init(
		GDExtensionInterfaceGetProcAddress p_get_proc_address,
		const GDExtensionClassLibraryPtr p_library,
		GDExtensionInitialization *r_initialization)
	{
		godot::GDExtensionBinding::InitObject init_obj(
			p_get_proc_address, p_library, r_initialization);

		init_obj.register_initializer(initialize_module);
		init_obj.register_terminator(uninitialize_module);
		init_obj.set_minimum_library_initialization_level(
			MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}