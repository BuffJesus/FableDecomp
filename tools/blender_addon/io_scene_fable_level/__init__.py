# SPDX-License-Identifier: MIT
"""io_scene_fable_level -- Blender import/export addon for Fable: The Lost Chapters
LEVEL TERRAIN (.lev heightmaps).

Separate from `io_scene_fable` (the mesh/model importer) on purpose: this package
owns the terrain round-trip and touches none of that package's files.

  * IMPORT "Fable Level (.lev)": build a (width+1)x(height+1) heightmap MESH --
    one vertex per terrain cell, vertex Z = the cell's world height (raw*2048),
    quad faces between cells, a theme vertex-color layer, and custom props that
    tag the source .lev + grid dims for a safe export.                 [WORKING]
  * EXPORT "Fable Level (.lev)" (STAGED): read the edited mesh's per-vertex Z
    back into cell heights via the byte-exact tools/lev_rw.py and write a STAGED
    .lev (re-reads the source so navmesh/obstacle/theme spans stay verbatim;
    makes a .bak if overwriting; refuses to write into a game install). [WORKING]

Honest scope: editing terrain heights does NOT regenerate the navmesh / region
graph / obstacle-brush data -- those are preserved opaque, so the engine's
runtime navigation still reflects the ORIGINAL terrain. TNG "thing" placement is
NOT included here (deferred). See docs/TERRAIN_BLENDER.md.

All LEV logic lives in the bpy-FREE `level_core` module (wraps tools/lev_rw.py,
unit-testable standalone). `level_io` is the thin bpy layer.
"""

bl_info = {
    "name": "Fable: The Lost Chapters level terrain (io_scene_fable_level)",
    "author": "FableTLC RE project",
    "version": (0, 1, 0),
    "blender": (3, 0, 0),
    "location": "File > Import/Export > Fable Level (.lev)",
    "description": "Import a Fable TLC .lev as a heightmap mesh, sculpt it, and "
                   "export the edited heights back to a byte-exact staged .lev.",
    "warning": "Height edits do NOT regenerate the navmesh (preserved opaque).",
    "category": "Import-Export",
}

# Support both fresh loads and Blender's addon "Reload Scripts".
if "bpy" in locals():  # pragma: no cover - reload path
    import importlib
    from . import level_core, level_io
    importlib.reload(level_core)
    importlib.reload(level_io)
else:
    from . import level_core, level_io  # noqa: F401


def register():
    level_io.register()


def unregister():
    level_io.unregister()


if __name__ == "__main__":
    register()
