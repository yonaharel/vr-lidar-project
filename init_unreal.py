import unreal
from os import listdir
from os.path import isfile, join


def onEditorStart():
    unreal.log_warning("hello world")
    # import_assets()
    newLevelName = "{new_level_name}"
    newLevelName = newLevelName.replace(" ", "_")
    if newLevelName == "NO_NAME":
        return
    print("new Level name is {}".format(newLevelName))
    # loadNewLevel(newLevelName)
    
    staticMeshOptions = buildStaticMeshImportOptions()
    fileName = "{fbx_file_name}"
    if fileName == "{fbx_file_name}":
        return
    assetPath = "/Game/ImportedAssets/Main Stage"
    unreal.log_warning("Importing assets")

    dirPath = "{directory_path}"

    fileNames = []
    if dirPath != "{directory_path}":
        fileNames = [f for f in listdir(dirPath) if isfile(join(dirPath, f))]

    all_tasks = []
    # unreal.log_warning(onlyFiles)
    for file in fileNames:
        file_name = join(dirPath, file)
        all_tasks.append(buildImportTask(file_name, "/Game/VRItems", staticMeshOptions))

    task = buildImportTask(fileName, assetPath, staticMeshOptions)
    outputPath = executeImportTasks([task])[0]
    executeImportTasks(all_tasks)
    # spawnBlueprintActor(path=outputPath, actor_location=[0, 0, 215], world_scale=unreal.Vector(4, 4, 4))

    success = loadNewLevel(newLevelName)
    if success:
        spawnBlueprintActor(path=outputPath, actor_location=[0, 0, 215], world_scale=unreal.Vector(4, 4, 4))

    return


def executeImportTasks(tasks=[]):
    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks(tasks)
    unreal.log_warning("executing Import Tasks")
    imported_asset_paths = []
    results = []
    for task in tasks:
        # for result in task.result:
        results.append(task.result)
        for path in task.get_editor_property('imported_object_paths'):
            imported_asset_paths.append(path)
        unreal.log_warning("one import task is done")
    return imported_asset_paths


def import_skeletal_mesh(fbx_path, game_path, asset_name):
    import_task = unreal.AssetImportTask()
    import_task.filename = fbx_path
    import_task.destination_path = game_path
    import_task.automated = True

    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks(
        [import_task]
    )
    imported_assets = import_task.get_editor_property(
        "imported_object_paths"
    )
    if not imported_assets:
        unreal.log_warning("No Assets were imported!")
        return


def buildStaticMeshImportOptions():
    options = unreal.FbxImportUI()
    # unreal.FbxImportUI
    options.set_editor_property('import_mesh', True)
    # unreal.FbxMeshImportData
    options.static_mesh_import_data.set_editor_property('import_translation', unreal.Vector(0.0, 0.0, 0.0))
    options.static_mesh_import_data.set_editor_property('import_rotation', unreal.Rotator(0.0, 0.0, 0.0))
    options.static_mesh_import_data.set_editor_property('import_uniform_scale', 1.0)
    # unreal.FbxStaticMeshImportData
    options.static_mesh_import_data.set_editor_property('combine_meshes', True)
    options.static_mesh_import_data.set_editor_property('generate_lightmap_u_vs', False)
    options.static_mesh_import_data.set_editor_property('auto_generate_collision', False)
    options.static_mesh_import_data.set_editor_property('remove_degenerates', True)
    options.static_mesh_import_data.set_editor_property('build_adjacency_buffer', True)
    options.static_mesh_import_data.set_editor_property('generate_lightmap_u_vs', False)
    options.static_mesh_import_data.set_editor_property('one_convex_hull_per_ucx', False)
    options.static_mesh_import_data.set_editor_property('import_mesh_lo_ds', False)
    options.static_mesh_import_data.set_editor_property('normal_import_method',
                                                        unreal.FBXNormalImportMethod.FBXNIM_COMPUTE_NORMALS)
    options.static_mesh_import_data.set_editor_property('combine_meshes', False)
    options.static_mesh_import_data.set_editor_property('vertex_color_import_option',
                                                        unreal.VertexColorImportOption.IGNORE)
    options.static_mesh_import_data.set_editor_property('normal_generation_method',
                                                        unreal.FBXNormalGenerationMethod.MIKK_T_SPACE)
    options.static_mesh_import_data.set_editor_property('static_mesh_lod_group', "None")

    return options


def showAssetsInContentBrowser(paths=[]):
    unreal.EditorAssetLibrary.sync_browser_to_objects(asset_paths=paths)


def buildImportTask(filename='', destination_path='', options=None):
    task = unreal.AssetImportTask()
    task.set_editor_property('automated', True)
    task.set_editor_property('destination_name', '')
    task.set_editor_property('destination_path', destination_path)
    task.set_editor_property('filename', filename)
    task.set_editor_property('replace_existing', True)
    task.set_editor_property('save', True)
    task.set_editor_property('options', options)
    unreal.log_warning("building Import Task")
    return task


def spawnBlueprintActor(path='', actor_location=[0, 0, 0], world_scale=unreal.Vector(1, 1, 1), object=None):
    actor_object = unreal.EditorAssetLibrary.load_asset(path) if object is None else object
    actor = unreal.EditorLevelLibrary.spawn_actor_from_object(actor_object, actor_location)
    # Edit Properties
    # for x in properties:
    #     print(x, properties[x])
    #     actor.set_editor_property(x, properties[x])
    # Complete Spawn
    actor.set_actor_scale3d(world_scale)
    unreal.log_warning(actor)
    actor.set_mobility(unreal.ComponentMobility.STATIONARY)
    unreal.log_warning("SADASDASDASDASD")
    return actor


def loadNewLevel(levelName="New LEVEL"):
    level_to_save = join('/Game/VirtualRealityBP/Maps/', levelName)
    success = unreal.EditorLevelLibrary.new_level_from_template(level_to_save,
                                                                '/Game/VirtualRealityBP/Maps/TrainingArea1')
    if success:
        unreal.EditorLevelLibrary.load_level(level_to_save)
    return success

onEditorStart()
