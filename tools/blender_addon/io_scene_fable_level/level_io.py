# SPDX-License-Identifier: MIT
"""Blender (bpy) layer for io_scene_fable_level.

This is the ONLY module in this package that touches `bpy`. It calls into the
bpy-free `level_core` (which wraps the byte-exact tools/lev_rw.py) and maps its
plain-data Heightmap onto a Blender mesh, and reads an edited mesh's per-vertex
Z back out to a STAGED .lev.

  Import:  LEV -> Heightmap -> a (w+1)x(h+1) grid MESH (vert Z = cell world
           height), quad faces between cells, a theme vertex-color layer, and
           custom props tagging the source .lev + grid dims for export.
  Export:  the tagged mesh's per-vertex Z -> lev_rw set_height -> STAGED .lev
           (re-reads the source so navmesh/obstacle/theme spans stay verbatim;
           makes a .bak if overwriting).

Custom props written on the imported object (read back on export):
  ["fable_lev_source"]   absolute path to the source .lev
  ["fable_lev_cols"]     grid columns  (= width+1)
  ["fable_lev_rows"]     grid rows      (= height+1)
  ["fable_lev_subver"]   LEV sub-version (8/9)
  ["fable_lev_uid"]      map uid (string; may exceed 32-bit)
"""
import os

import bpy
from bpy.props import StringProperty, BoolProperty, FloatProperty
from bpy.types import Operator, Panel
from bpy_extras.io_utils import ImportHelper, ExportHelper

from . import level_core as lc


# ---------------------------------------------------------------------------
# builders / readers (map plain-data <-> Blender)
# ---------------------------------------------------------------------------
def build_terrain_mesh(hm, name=None, collection=None, add_theme_colors=True):
    """Realise a level_core.Heightmap as a Blender grid mesh object. Returns obj."""
    if collection is None:
        collection = bpy.context.scene.collection
    if name is None:
        name = os.path.splitext(os.path.basename(hm.source_path))[0] or "FableLevel"

    verts = list(hm.vertex_coords())
    faces = hm.build_faces()

    me = bpy.data.meshes.new(name + "_terrain")
    me.from_pydata(verts, [], faces)
    me.update()

    obj = bpy.data.objects.new(name, me)
    collection.objects.link(obj)

    # theme index as a vertex color layer (per-loop), so distinct ground themes
    # are visible while sculpting. Simple hashed palette; cosmetic only.
    if add_theme_colors and hm.themes:
        _apply_theme_colors(me, hm)

    # provenance + geometry tags for the exporter
    obj["fable_lev_source"] = hm.source_path
    obj["fable_lev_cols"] = hm.cols
    obj["fable_lev_rows"] = hm.rows
    obj["fable_lev_subver"] = -1 if hm.sub_version is None else int(hm.sub_version)
    obj["fable_lev_uid"] = str(hm.uid)
    obj["fable_lev_note"] = ("Terrain heightmap. Editing Z edits cell height on "
                             "export; navmesh is NOT regenerated.")
    return obj


def _theme_color(idx):
    """Deterministic pseudo-color for a theme index (cosmetic vertex color)."""
    # cheap integer hash -> 3 channels in [0,1]
    h = (idx * 2654435761) & 0xFFFFFFFF
    r = ((h >> 0) & 0xFF) / 255.0
    g = ((h >> 8) & 0xFF) / 255.0
    b = ((h >> 16) & 0xFF) / 255.0
    return (r, g, b, 1.0)


def _apply_theme_colors(me, hm):
    """Add a 'FableTheme' color attribute keyed by each vertex's theme index."""
    try:
        vcol = me.color_attributes.new(name="FableTheme", type="FLOAT_COLOR",
                                       domain="POINT")
    except Exception:
        return
    themes = hm.themes
    n = len(vcol.data)
    for i in range(n):
        ti = themes[i] if i < len(themes) else 0
        vcol.data[i].color = _theme_color(ti)


def export_terrain_mesh(obj, out_path, tools_dir=None, make_backup=True,
                        apply_object_transform=True):
    """Read obj's per-vertex Z back to a STAGED .lev via level_core.stage_export."""
    src = obj.get("fable_lev_source")
    cols = obj.get("fable_lev_cols")
    rows = obj.get("fable_lev_rows")
    if not src or cols is None or rows is None:
        raise ValueError(
            "Object %r is not a Fable terrain mesh (missing fable_lev_* props). "
            "Import a .lev with this addon first." % obj.name)
    cols = int(cols)
    rows = int(rows)

    me = obj.data
    if len(me.vertices) != cols * rows:
        raise ValueError(
            "Vertex count changed: mesh has %d verts but grid is %dx%d=%d. "
            "This exporter maps 1 vertex per cell and cannot add/remove verts."
            % (len(me.vertices), cols, rows, cols * rows))

    mat = obj.matrix_world if apply_object_transform else None

    # Precompute world Z per vertex index. Vertex order matches import (we built
    # from_pydata in that exact order and Blender preserves vertex indices).
    zs = [0.0] * (cols * rows)
    for i, v in enumerate(me.vertices):
        if mat is not None:
            co = mat @ v.co
            zs[i] = co.z
        else:
            zs[i] = v.co.z

    def get_z(x, y):
        return zs[y * cols + x]

    return lc.stage_export(src, out_path, cols, rows, get_z,
                           tools_dir=tools_dir, make_backup=make_backup)


# ---------------------------------------------------------------------------
# IMPORT operator
# ---------------------------------------------------------------------------
class ImportFableLevel(Operator, ImportHelper):
    """Import a Fable: The Lost Chapters level terrain (.lev) as a heightmap mesh."""
    bl_idname = "import_scene.fable_level"
    bl_label = "Import Fable Level (.lev)"
    bl_options = {"REGISTER", "UNDO"}

    filename_ext = ".lev"
    filter_glob: StringProperty(default="*.lev", options={"HIDDEN"})

    add_theme_colors: BoolProperty(
        name="Theme vertex colors",
        description="Add a vertex-color layer keyed by each cell's ground theme "
                    "index (cosmetic, for orientation while sculpting)",
        default=True,
    )
    tools_dir: StringProperty(
        name="Tools dir (optional)",
        description="Path to the FableTLC tools/ folder containing lev_rw.py, if "
                    "the addon is installed outside the repo. Blank = auto-detect.",
        default="", subtype="DIR_PATH",
    )

    def execute(self, context):
        path = self.filepath
        tools_dir = bpy.path.abspath(self.tools_dir) if self.tools_dir else None
        try:
            hm = lc.read_heightmap(path, tools_dir=tools_dir,
                                   want_themes=self.add_theme_colors)
        except Exception as ex:
            self.report({"ERROR"}, "Failed to read %s: %s" % (path, ex))
            return {"CANCELLED"}

        coll = bpy.data.collections.new("FableLevel_" + os.path.basename(path))
        context.scene.collection.children.link(coll)
        obj = build_terrain_mesh(hm, collection=coll,
                                 add_theme_colors=self.add_theme_colors)

        # select + make active
        for o in context.selected_objects:
            o.select_set(False)
        obj.select_set(True)
        context.view_layer.objects.active = obj

        self.report(
            {"INFO"},
            "Fable level import: %s terrain %dx%d verts (%d cells), sub-ver %s. "
            "Edit Z to sculpt; export re-stages a .lev (navmesh NOT regenerated)."
            % (obj.name, hm.cols, hm.rows, hm.vert_count, hm.sub_version))
        return {"FINISHED"}


# ---------------------------------------------------------------------------
# EXPORT operator (STAGED)
# ---------------------------------------------------------------------------
class ExportFableLevel(Operator, ExportHelper):
    """Export the active Fable terrain mesh's heights back to a STAGED .lev."""
    bl_idname = "export_scene.fable_level"
    bl_label = "Export Fable Level (.lev)"
    bl_options = {"REGISTER"}

    filename_ext = ".lev"
    filter_glob: StringProperty(default="*.lev", options={"HIDDEN"})

    make_backup: BoolProperty(
        name="Backup if overwriting",
        description="If the output .lev already exists, copy it to <name>.lev.bak "
                    "before writing",
        default=True,
    )
    apply_transform: BoolProperty(
        name="Apply object transform",
        description="Bake the object's world matrix into vertex Z before export "
                    "(so moving/scaling the object is honoured)",
        default=True,
    )
    tools_dir: StringProperty(
        name="Tools dir (optional)",
        description="Path to the FableTLC tools/ folder containing lev_rw.py. "
                    "Blank = auto-detect.",
        default="", subtype="DIR_PATH",
    )

    @classmethod
    def poll(cls, context):
        obj = context.active_object
        return obj is not None and obj.type == "MESH" and "fable_lev_source" in obj

    def execute(self, context):
        obj = context.active_object
        tools_dir = bpy.path.abspath(self.tools_dir) if self.tools_dir else None

        # Guard: never write into a Steam/game install.
        norm = os.path.normcase(os.path.abspath(self.filepath))
        if "steamapps" in norm or os.path.join("fable", "data") in norm:
            self.report({"ERROR"},
                        "Refusing to export into what looks like a game install "
                        "(%s). Export to a workspace/scratch path instead."
                        % self.filepath)
            return {"CANCELLED"}

        try:
            rep = export_terrain_mesh(obj, self.filepath, tools_dir=tools_dir,
                                      make_backup=self.make_backup,
                                      apply_object_transform=self.apply_transform)
        except Exception as ex:
            self.report({"ERROR"}, "Export failed: %s" % ex)
            return {"CANCELLED"}

        msg = ("Fable level export: %d cell height(s) changed, wrote %s (%dx%d)."
               % (rep["changed_cells"], rep["out_path"], rep["cols"], rep["rows"]))
        if rep["backup"]:
            msg += " Backup: %s." % rep["backup"]
        msg += " Navmesh NOT regenerated."
        self.report({"INFO"}, msg)
        return {"FINISHED"}


# ---------------------------------------------------------------------------
# UI
# ---------------------------------------------------------------------------
def menu_func_import(self, context):
    self.layout.operator(ImportFableLevel.bl_idname, text="Fable Level (.lev)")


def menu_func_export(self, context):
    self.layout.operator(ExportFableLevel.bl_idname, text="Fable Level (.lev)")


class FABLE_PT_level_panel(Panel):
    bl_label = "Fable TLC Level"
    bl_idname = "FABLE_PT_level_panel"
    bl_space_type = "VIEW_3D"
    bl_region_type = "UI"
    bl_category = "Fable TLC"

    def draw(self, context):
        layout = self.layout
        layout.label(text="Terrain heightmap round-trip")
        layout.operator(ImportFableLevel.bl_idname, icon="IMPORT",
                        text="Import Level (.lev)")
        layout.operator(ExportFableLevel.bl_idname, icon="EXPORT",
                        text="Export Level (.lev)")
        obj = context.active_object
        if obj is not None and "fable_lev_source" in obj:
            box = layout.box()
            box.label(text="Active terrain:", icon="MESH_GRID")
            box.label(text="%sx%s cells"
                      % (obj.get("fable_lev_cols"), obj.get("fable_lev_rows")))
            box.label(text="src: %s"
                      % os.path.basename(obj.get("fable_lev_source", "?")))
        box2 = layout.box()
        box2.label(text="Navmesh is NOT regenerated", icon="ERROR")
        box2.label(text="on height edits (opaque).")


_CLASSES = (ImportFableLevel, ExportFableLevel, FABLE_PT_level_panel)


def register():
    for c in _CLASSES:
        bpy.utils.register_class(c)
    bpy.types.TOPBAR_MT_file_import.append(menu_func_import)
    bpy.types.TOPBAR_MT_file_export.append(menu_func_export)


def unregister():
    bpy.types.TOPBAR_MT_file_export.remove(menu_func_export)
    bpy.types.TOPBAR_MT_file_import.remove(menu_func_import)
    for c in reversed(_CLASSES):
        bpy.utils.unregister_class(c)
