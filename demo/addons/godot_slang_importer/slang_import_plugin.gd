@tool
extends EditorPlugin

var import_plugin

func _enter_tree():
	import_plugin = preload("slang_importer.gd").new()
	add_import_plugin(import_plugin)
	print("Godot Slang Importer plugin enabled")

func _exit_tree():
	remove_import_plugin(import_plugin)
	import_plugin = null
	print("Godot Slang Importer plugin disabled")
