# SPDX-License-Identifier: MIT
"""io_scene_fable -- Blender import addon for Fable: The Lost Chapters assets.

Imports what the FableTLC reverse-engineering project can decode TODAY from a
`graphics.big` mesh bank (+ its sibling `textures.big`/`frontend.big`):

  * Materials with REAL textures  -- each mesh material's base texture is
    LZO1X-decompressed + DXT/ARGB-decoded to RGBA, packed as a Blender image,
    and wired into a Principled BSDF (base color). [WORKING, headline feature]
  * Armature from recovered skeleton BONE NAMES (flat placeholder hierarchy;
    real bone transforms await the mesh-geometry RE).                 [WORKING]
  * Submesh / LOD / bounds tree as an Empty hierarchy so the model's
    organization is visible before geometry exists.                    [WORKING]
  * Geometry (verts/faces/UVs) -- STUBBED. `fable_core.decode_geometry()` is a
    hook that returns None until docs/BIG_MESH_GEOMETRY.md lands; the mesh-build
    call site is already wired so only that hook body needs filling later.[STUB]

All parse/convert logic lives in the bpy-FREE `fable_core` module (unit-tested
standalone). This file is the thin Blender (`bpy`) layer.
"""

bl_info = {
    "name": "Fable: The Lost Chapters model importer (io_scene_fable)",
    "author": "FableTLC RE project",
    "version": (0, 1, 0),
    "blender": (3, 0, 0),
    "location": "File > Import > Fable TLC Model (.big)",
    "description": "Import Fable TLC meshes: textured materials, skeleton bone "
                   "names, and the LOD/submesh tree (geometry pending RE).",
    "warning": "Geometry is stubbed pending the mesh-geometry RE; imports "
               "materials+textures, armature bone names, and the submesh tree.",
    "category": "Import-Export",
}

# Support both fresh loads and Blender's addon "Reload Scripts".
if "bpy" in locals():  # pragma: no cover - reload path
    import importlib
    from . import fable_core, import_op, anim_ops
    importlib.reload(fable_core)
    importlib.reload(import_op)
    importlib.reload(anim_ops)
else:
    from . import fable_core, import_op, anim_ops  # noqa: F401


def register():
    import_op.register()
    anim_ops.register()


def unregister():
    anim_ops.unregister()
    import_op.unregister()


if __name__ == "__main__":
    register()
