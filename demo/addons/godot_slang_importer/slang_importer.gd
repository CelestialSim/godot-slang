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
	return "res"

func _get_resource_type():
	return "RDShaderFile"

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
				{
					"name": "debug_messages",
					"default_value": false,
					"property_hint": PROPERTY_HINT_NONE
				}
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
	var debug = options.get("debug_messages", false)
	
	if debug:
		print("Slang Importer: Starting import of " + source_file)
		print("Slang Importer: Save path = " + save_path)
		print("Slang Importer: Options = " + str(options))
	
	# Check if SlangCompiler class is available
	if not ClassDB.class_exists("SlangCompiler"):
		printerr("Slang Importer: SlangCompiler class not found. Make sure the godot-slang extension is loaded.")
		return ERR_UNAVAILABLE
	
	# Create SlangCompiler instance to use the compilation function
	var compiler = SlangCompiler.new()
	
	# Convert res:// path to filesystem path
	var filesystem_path = ProjectSettings.globalize_path(source_file)
	if debug:
		print("Slang Importer: Filesystem path = " + filesystem_path)
	
	# Get the filename without extension
	var base_path = filesystem_path.get_base_dir()
	var filename_no_ext = filesystem_path.get_file().get_basename()
	var full_path_no_ext = base_path.path_join(filename_no_ext)
	
	if debug:
		print("Slang Importer: Compiling " + full_path_no_ext)
	
	# Compile the Slang shader to SPIR-V bytecode
	# This returns a PackedByteArray with the compiled SPIR-V data
	var spirv_data = compiler.to_spirv_bytes(full_path_no_ext)
	
	# Check if compilation succeeded
	if spirv_data.is_empty():
		printerr("Slang Importer: Failed to compile Slang shader: " + source_file)
		return ERR_COMPILATION_FAILED
	
	if debug:
		print("Slang Importer: Successfully compiled " + str(spirv_data.size()) + " bytes of SPIR-V code")
	
	# Create an RDShaderFile resource to hold the SPIR-V bytecode
	var shader_file = RDShaderFile.new()
	
	# Create an RDShaderSPIRV object to hold the compiled bytecode
	var spirv = RDShaderSPIRV.new()
	spirv.set_stage_bytecode(RenderingDevice.SHADER_STAGE_COMPUTE, spirv_data)
	
	# Set the SPIR-V bytecode for the shader
	# RDShaderFile expects an RDShaderSPIRV object, not raw bytes
	shader_file.set_bytecode(spirv)
	
	# Save the RDShaderFile resource
	var final_path = "%s.%s" % [save_path, _get_save_extension()]
	if debug:
		print("Slang Importer: Saving RDShaderFile resource to " + final_path)
	
	var err = ResourceSaver.save(shader_file, final_path)
	if err != OK:
		printerr("Slang Importer: Failed to save resource: " + final_path + " (error: " + str(err) + ")")
		return err
	
	if debug:
		print("Slang Importer: Successfully imported Slang shader: " + source_file + " -> " + final_path)
	
	return OK
