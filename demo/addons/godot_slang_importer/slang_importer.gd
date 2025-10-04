@tool
extends EditorImportPlugin

enum Presets {DEFAULT}

func _get_importer_name():
	return "godot_slang.importer"

func _get_visible_name():
	return "Slang Shader"

func _get_recognized_extensions():
	return ["slang"]

func _get_save_extension():
	return "tres"

func _get_resource_type():
	return "Resource"

func _get_preset_count():
	return Presets.size()

func _get_preset_name(preset_index):
	match preset_index:
		Presets.DEFAULT:
			return "Default"
		_:
			return "Unknown"

func _get_import_options(path, preset_index):
	match preset_index:
		Presets.DEFAULT:
			return [
				# {
				# 	"name": "entry_point",
				# 	"default_value": "computeMain",
				# 	"property_hint": PROPERTY_HINT_PLACEHOLDER_TEXT,
				# 	"hint_string": "Entry point function name"
				# },
				# {
				# 	"name": "shader_type",
				# 	"default_value": "compute",
				# 	"property_hint": PROPERTY_HINT_ENUM,
				# 	"hint_string": "compute,vertex,fragment"
				# },
				# {
				# 	"name": "glsl_version",
				# 	"default_value": "460",
				# 	"property_hint": PROPERTY_HINT_ENUM,
				# 	"hint_string": "330,400,410,420,430,440,450,460"
				# }
			]
		_:
			return []

func _get_option_visibility(path, option_name, options):
	return true

func _get_priority():
	return 1.0

func _get_import_order():
	return 0

func _import(source_file, save_path, options, r_platform_variants, r_gen_files):
	print("Slang Importer: Starting import of " + source_file)
	print("Slang Importer: Save path = " + save_path)
	print("Slang Importer: Options = " + str(options))
	
	# Check if TestCompile class is available
	if not ClassDB.class_exists("TestCompile"):
		printerr("Slang Importer: TestCompile class not found. Make sure the godot-slang extension is loaded.")
		return ERR_UNAVAILABLE
	
	# Create TestCompile instance to use the compilation function
	var test_compile = TestCompile.new()
	
	# Convert res:// path to filesystem path
	var filesystem_path = ProjectSettings.globalize_path(source_file)
	print("Slang Importer: Filesystem path = " + filesystem_path)
	
	# Get the filename without extension
	var base_path = filesystem_path.get_base_dir()
	var filename_no_ext = filesystem_path.get_file().get_basename()
	var full_path_no_ext = base_path.path_join(filename_no_ext)
	
	print("Slang Importer: Compiling " + full_path_no_ext)
	
	# The compile_slang_to_glsl function expects a path without extension
	# and will look for .slang file and create .glsl file
	test_compile.compile_slang_to_glsl(full_path_no_ext)
	
	# The output GLSL file
	var glsl_file = full_path_no_ext + ".glsl"
	
	print("Slang Importer: Looking for generated GLSL at " + glsl_file)
	
	# Check if the GLSL file was created
	if not FileAccess.file_exists(glsl_file):
		printerr("Slang Importer: Failed to compile Slang shader: " + source_file)
		printerr("Slang Importer: Expected GLSL output at: " + glsl_file)
		return ERR_COMPILATION_FAILED
	
	# Read the generated GLSL content
	var file = FileAccess.open(glsl_file, FileAccess.READ)
	if not file:
		printerr("Slang Importer: Failed to read generated GLSL file: " + glsl_file)
		return ERR_FILE_CANT_READ
	
	var glsl_content = file.get_as_text()
	file.close()
	
	print("Slang Importer: Successfully read " + str(glsl_content.length()) + " bytes of GLSL code")
	
	# Create a Resource that stores a reference to the generated GLSL file
	# The .glsl file itself will be imported by Godot's built-in GLSL importer
	var resource = Resource.new()
	resource.take_over_path(source_file)
	
	# Save a simple marker resource
	var final_path = "%s.%s" % [save_path, _get_save_extension()]
	print("Slang Importer: Saving marker resource to " + final_path)
	
	var err = ResourceSaver.save(resource, final_path)
	if err != OK:
		printerr("Slang Importer: Failed to save resource: " + final_path + " (error: " + str(err) + ")")
		return err
	
	print("Slang Importer: Successfully imported Slang shader: " + source_file + " -> " + final_path)
	print("Slang Importer: Generated GLSL file at: " + glsl_file)
	print("Slang Importer: Note: Use the .glsl file directly in your shaders, not the .slang file")
	
	return OK
