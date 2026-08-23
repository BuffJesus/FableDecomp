# Headless Blender: import an FBX and dump triangulated geometry (verts+uv+normal,
# faces) to JSON for tools/blender_addon/io_scene_fable/mesh_rw.compose_mesh.
# Run: blender --background --python fbx_extract.py -- <in.fbx> <out.json>
import bpy, sys, json

argv = sys.argv[sys.argv.index('--') + 1:]
fbx, out = argv[0], argv[1]

bpy.ops.wm.read_factory_settings(use_empty=True)
try:
    bpy.ops.import_scene.fbx(filepath=fbx)
except Exception:
    import addon_utils
    for a in ("io_scene_fbx", "bl_ext.blender_org.io_scene_fbx"):
        try: addon_utils.enable(a)
        except Exception: pass
    bpy.ops.import_scene.fbx(filepath=fbx)

prims = []
for obj in list(bpy.context.scene.objects):
    if obj.type != 'MESH':
        continue
    me = obj.to_mesh()
    me.calc_loop_triangles()
    try:
        me.calc_normals_split()
    except Exception:
        pass
    uvl = me.uv_layers.active
    verts = []
    faces = []
    for tri in me.loop_triangles:
        idx = []
        for li in tri.loops:
            loop = me.loops[li]
            v = me.vertices[loop.vertex_index]
            co = obj.matrix_world @ v.co
            uv = tuple(uvl.data[li].uv) if uvl else (0.0, 0.0)
            try:
                no = tuple(loop.normal)
            except Exception:
                no = tuple(v.normal)
            verts.append([co.x, co.y, co.z, uv[0], uv[1], no[0], no[1], no[2]])
            idx.append(len(verts) - 1)
        faces.append(idx)
    prims.append({'name': obj.name, 'verts': verts, 'faces': faces})
    obj.to_mesh_clear()

json.dump({'prims': prims}, open(out, 'w'))
nv = sum(len(p['verts']) for p in prims)
nf = sum(len(p['faces']) for p in prims)
print("EXTRACTED prims=%d verts=%d faces=%d" % (len(prims), nv, nf))
