# SPDX-License-Identifier: MIT
"""Blender (bpy) layer for io_scene_fable.

This is the ONLY module that touches `bpy`. It calls into the bpy-free
`fable_core` to do all parsing/decoding, then maps the plain-data structures
(ImageData / MaterialData / BoneData / MeshModel) onto Blender datablocks:

    ImageData    -> bpy.data.images (packed float pixels)
    MaterialData -> a Principled-BSDF material, base color = image
    BoneData     -> an Armature object (bones named; flat placeholder transforms)
    MeshModel    -> an Empty hierarchy of LOD/submesh/bounds placeholders
    (geometry)   -> filled by fable_core.decode_geometry() when it exists (stub)

The operator lets the user import ONE compiled mesh (by name/id) or ALL of them.
"""
import os

import bpy
from bpy.props import StringProperty, BoolProperty, IntProperty
from bpy.types import Operator, Panel
from bpy_extras.io_utils import ImportHelper, ExportHelper

from . import fable_core as fc


# ---------------------------------------------------------------------------
# bpy datablock builders (map plain-data -> Blender objects)
# ---------------------------------------------------------------------------
def _build_image(img):
    """Create (or reuse) a packed bpy image from a fable_core.ImageData."""
    key = "FABLE_TEX_%d_%s" % (img.tex_id, img.name[:40])
    existing = bpy.data.images.get(key)
    if existing is not None:
        return existing
    bimg = bpy.data.images.new(key, width=img.alloc_width, height=img.alloc_height,
                               alpha=True)
    # fable_core already produced Blender-order pixels (bottom row first, RGBA float)
    if len(img.pixels) == len(bimg.pixels):
        bimg.pixels.foreach_set(img.pixels)
    else:
        bimg.pixels[:] = img.pixels[:len(bimg.pixels)]
    bimg.pack()
    bimg.update()
    return bimg


def _build_material(md):
    """Create a Principled-BSDF material with the decoded base texture as base
    color from a fable_core.MaterialData."""
    mat = bpy.data.materials.new(name=md.name)
    mat.use_nodes = True
    nt = mat.node_tree
    bsdf = nt.nodes.get("Principled BSDF")
    if bsdf is None:
        bsdf = nt.nodes.new("ShaderNodeBsdfPrincipled")
    if md.image is not None:
        bimg = _build_image(md.image)
        tex = nt.nodes.new("ShaderNodeTexImage")
        tex.image = bimg
        tex.location = (-400, 200)
        nt.links.new(tex.outputs["Color"], bsdf.inputs["Base Color"])
        # DXT3/ARGB carry alpha -> wire it and enable blend for foliage/UI
        if md.image.fmt_name in ("DXT3", "A8R8G8B8"):
            nt.links.new(tex.outputs["Alpha"], bsdf.inputs["Alpha"])
            mat.blend_method = "HASHED" if hasattr(mat, "blend_method") else mat.blend_method
    else:
        # undecodable texture -> tag the material name so it's visible
        mat.name = md.name + "_NOTEX"
    # stash the source texture id for round-trip / debugging
    mat["fable_base_tex_id"] = md.base_tex_id
    mat["fable_base_tex_name"] = md.base_tex_name
    return mat


def _build_armature(model, collection):
    """Build an Armature object from the decoded skeleton.

    When the bones carry REAL bind poses (BoneData.matrix set -- decoded from the
    compiled bone blocks, docs/BIG_MESH_GEOMETRY.md section 7), each edit-bone
    gets its model-space bind matrix (head = bind translation, orientation from
    the bind rotation) and is parented per the real ParentIndex hierarchy.
    Falls back to the old stacked-placeholder layout for name-scan-only bones.
    """
    if not model.bones:
        return None
    from mathutils import Matrix, Vector

    real = all(len(b.matrix) == 16 for b in model.bones)

    arm_data = bpy.data.armatures.new(model.name + "_Armature")
    arm_obj = bpy.data.objects.new(model.name + "_Armature", arm_data)
    collection.objects.link(arm_obj)

    # bone length: reach to the first child's head; leaves get a fraction of
    # the skeleton's overall span (never zero -- Blender deletes 0-length bones)
    children = {}
    for b in model.bones:
        if real and 0 <= b.parent < len(model.bones) and b.parent != b.index:
            children.setdefault(b.parent, []).append(b)
    if real:
        heads = [Vector(b.head) for b in model.bones]
        span = max((h - heads[0]).length for h in heads) if len(heads) > 1 else 1.0
        default_len = max(span * 0.05, 0.01)

    # enter edit mode on this armature to add bones
    prev_active = bpy.context.view_layer.objects.active
    bpy.context.view_layer.objects.active = arm_obj
    bpy.ops.object.mode_set(mode="EDIT")
    ebones = arm_data.edit_bones
    made = {}
    for b in model.bones:
        eb = ebones.new(b.name)
        if real:
            m = b.matrix
            mat = Matrix((m[0:4], m[4:8], m[8:12], m[12:16]))  # translation in col 3
            head = Vector(b.head)
            kids = children.get(b.index, [])
            if kids:
                d = min((Vector(k.head) - head).length for k in kids)
                length = d if d > 1e-4 else default_len
            else:
                length = default_len
            # EditBone.matrix assignment sets head/tail/roll from a 4x4, keeping
            # the current length -- so set a nonzero length first.
            eb.head = (0.0, 0.0, 0.0)
            eb.tail = (0.0, length, 0.0)
            try:
                eb.matrix = mat
            except Exception:
                eb.head = head
                eb.tail = head + Vector((0.0, length, 0.0))
        else:
            # stack bones vertically so they're individually selectable placeholders
            y = b.index * 0.15
            eb.head = (0.0, y, 0.0)
            eb.tail = (0.0, y + 0.12, 0.0)
        made[b.index] = eb
    for b in model.bones:
        if b.parent >= 0 and b.parent in made and b.parent != b.index:
            made[b.index].parent = made[b.parent]
    bpy.ops.object.mode_set(mode="OBJECT")
    bpy.context.view_layer.objects.active = prev_active
    if real:
        arm_obj["fable_note"] = ("REAL bind pose: decoded bone names, parent "
                                 "hierarchy and inverse-bind matrices "
                                 "(BIG_MESH_GEOMETRY.md section 7)")
    else:
        arm_obj["fable_note"] = ("bone NAMES recovered by string-scan; "
                                 "transforms are placeholders")
    return arm_obj


def _apply_skin(obj, model, geo, armature):
    """Create one vertex group per bone (named by bone name), assign each vertex
    its decoded (local_bone -> weight) influences, and add an Armature modifier
    binding the mesh to `armature`. See docs/BIG_MESH_GEOMETRY.md §7.1.

    geo.skin[v] = [(local_bone_slot, weight), ...] parallel to geo.vertices.
    The armature edit-bones were created in local-slot order, so bone-name lookup
    binds each influence to the correct vertex group."""
    if armature is None or not geo.skin:
        return 0
    # bone slot -> name (local slot == BoneData.index, edit-bone name)
    slot_name = {b.index: b.name for b in model.bones}
    groups = {}   # bone name -> VertexGroup (created lazily, only for used bones)
    assigned = 0
    for vi, infl in enumerate(geo.skin):
        if vi >= len(obj.data.vertices):
            break
        for (slot, w) in infl:
            if w <= 0.0:
                continue
            name = slot_name.get(slot)
            if not name:
                continue
            vg = groups.get(name)
            if vg is None:
                vg = obj.vertex_groups.get(name) or obj.vertex_groups.new(name=name)
                groups[name] = vg
            vg.add([vi], float(w), "REPLACE")
            assigned += 1
    mod = obj.modifiers.new(name="Armature", type="ARMATURE")
    mod.object = armature
    mod.use_vertex_groups = True
    obj.parent = armature
    return assigned


def _build_geometry_mesh(model, submesh, materials, collection, parent, src_big="",
                         armature=None):
    """Build the actual mesh object for a submesh IF geometry is available.
    Returns the object or None (None today -- geometry is stubbed).
    When the submesh carries skin weights and an armature is present, binds the
    mesh with vertex groups + an Armature modifier (docs/BIG_MESH_GEOMETRY.md §7.1)."""
    geo = submesh.geometry
    if geo is None or not geo.vertices or not geo.faces:
        return None  # STUB path: fable_core.decode_geometry returned None
    me = bpy.data.meshes.new("%s_L%d_S%d" % (model.name, submesh.lod, submesh.index))
    me.from_pydata(geo.vertices, [], geo.faces)
    me.update()
    if geo.uvs:
        uv = me.uv_layers.new(name="UVMap")
        for i, co in enumerate(geo.uvs):
            if i < len(uv.data):
                uv.data[i].uv = co
    obj = bpy.data.objects.new(me.name, me)
    # Tag with source identity so "Export to Fable" can map this mesh back to the
    # exact graphics.big entry + primitive it came from.
    obj["fable_src_big"] = src_big
    obj["fable_entry"] = model.name
    obj["fable_prim"] = submesh.index
    obj["fable_type"] = model.entry_type
    obj["fable_nverts"] = submesh.n_verts
    if 0 <= submesh.material_slot < len(materials):
        me.materials.append(materials[submesh.material_slot])
    collection.objects.link(obj)
    obj.parent = parent
    # Skin: vertex groups + Armature modifier (reparents to the armature).
    if submesh.geometry.skin and armature is not None:
        obj["fable_skin_influences"] = _apply_skin(obj, model, submesh.geometry, armature)
    return obj


def _build_placeholders(model, materials, collection, root, src_big="", armature=None):
    """Build the LOD/submesh/bounds Empty hierarchy so the model's structure is
    visible even before real geometry. Where geometry IS available (future),
    build the real mesh instead of a placeholder empty."""
    for lod in model.lods:
        lod_empty = bpy.data.objects.new(
            "%s_LOD%d" % (model.name, lod.index), None)
        lod_empty.empty_display_type = "PLAIN_AXES"
        lod_empty["switch_distance"] = lod.switch_distance
        lod_empty["payload_offset"] = "0x%x" % lod.offset
        collection.objects.link(lod_empty)
        lod_empty.parent = root
        for sm in lod.submeshes:
            real = _build_geometry_mesh(model, sm, materials, collection, lod_empty,
                                        src_big, armature=armature)
            if real is not None:
                continue  # real geometry built; no placeholder needed
            sm_empty = bpy.data.objects.new(
                "%s_LOD%d_SUBM%d[%dv/%df sVert%d]"
                % (model.name, lod.index, sm.index, sm.n_verts, sm.n_faces, sm.svert),
                None)
            sm_empty.empty_display_type = "CUBE"
            sm_empty.empty_display_size = 0.1
            sm_empty["n_verts"] = sm.n_verts
            sm_empty["n_faces"] = sm.n_faces
            sm_empty["sVert"] = sm.svert
            sm_empty["material_slot"] = sm.material_slot
            sm_empty["geometry"] = "STUB (pending docs/BIG_MESH_GEOMETRY.md)"
            if 0 <= sm.material_slot < len(materials):
                sm_empty["material"] = materials[sm.material_slot].name
            collection.objects.link(sm_empty)
            sm_empty.parent = lod_empty


def build_model_in_scene(model, collection=None, src_big=""):
    """Top-level: realise one fable_core.MeshModel as Blender objects.
    Returns a small dict of created counts for reporting."""
    if collection is None:
        collection = bpy.context.scene.collection

    # root empty for the whole model
    root = bpy.data.objects.new(model.name, None)
    root.empty_display_type = "ARROWS"
    root.empty_display_size = 0.3
    c = model.bounds_center
    root.location = (c[0] * 0.0, 0.0, 0.0)  # keep at origin; bounds are opaque
    root["entry_id"] = model.entry_id
    root["entry_type"] = model.entry_type
    root["has_skeleton"] = model.has_skeleton
    for w in model.warnings:
        root["warning"] = w
    collection.objects.link(root)

    # materials (with real textures)
    materials = [_build_material(md) for md in model.materials]

    # armature (bone names)
    arm = _build_armature(model, collection)
    if arm is not None:
        arm.parent = root

    # LOD/submesh placeholders (+ real geometry when available; skinned meshes
    # get vertex groups + an Armature modifier bound to `arm`)
    _build_placeholders(model, materials, collection, root, src_big, armature=arm)

    n_tex = sum(1 for m in model.materials if m.image is not None)
    n_sub = sum(len(l.submeshes) for l in model.lods)
    return dict(root=root, materials=len(materials), textures=n_tex,
                bones=len(model.bones), lods=len(model.lods), submeshes=n_sub)


# ---------------------------------------------------------------------------
# Import operator
# ---------------------------------------------------------------------------
class ImportFableModel(Operator, ImportHelper):
    """Import a Fable: The Lost Chapters model bank (graphics.big)."""
    bl_idname = "import_scene.fable_model"
    bl_label = "Import Fable TLC Model"
    bl_options = {"REGISTER", "UNDO"}

    filename_ext = ".big"
    filter_glob: StringProperty(default="*.big;*.fmp", options={"HIDDEN"})

    entry: StringProperty(
        name="Entry (name or id)",
        description="Which compiled mesh to import (e.g. MESH_SEAGULL_01 or an "
                    "entry id). Leave blank + tick 'Import all' to import every "
                    "compiled mesh.",
        default="MESH_SEAGULL_01",
    )
    import_all: BoolProperty(
        name="Import all meshes",
        description="Import every compiled mesh in the bank (can be thousands)",
        default=False,
    )
    max_all: IntProperty(
        name="Max (when importing all)",
        description="Safety cap on how many meshes 'Import all' creates",
        default=50, min=1, max=100000,
    )
    decode_textures: BoolProperty(
        name="Decode textures",
        description="LZO1X-decompress + DXT/ARGB-decode each material's base "
                    "texture into a real image (the headline feature)",
        default=True,
    )
    tools_dir: StringProperty(
        name="Tools dir (optional)",
        description="Path to the FableTLC tools/ folder (with parse_bigb.py etc.) "
                    "if the addon is installed outside the repo. Leave blank to "
                    "auto-detect the sibling repo layout.",
        default="",
    )
    textures_big: StringProperty(
        name="textures.big (optional)",
        description="Override path to textures.big. Leave blank to auto-locate "
                    "next to graphics.big (…/graphics/pc/textures.big).",
        default="", subtype="FILE_PATH",
    )

    def execute(self, context):
        path = self.filepath
        tools_dir = self.tools_dir or None
        try:
            fc._load_readers(tools_dir)
        except ImportError as ex:
            self.report({"ERROR"}, str(ex))
            return {"CANCELLED"}

        try:
            buf, meshes = fc.open_graphics_big(path, tools_dir=tools_dir)
        except Exception as ex:
            self.report({"ERROR"}, "Failed to open %s: %s" % (path, ex))
            return {"CANCELLED"}
        if not meshes:
            self.report({"ERROR"}, "No compiled meshes found in %s" % path)
            return {"CANCELLED"}

        # texture bank(s)
        tex_bank = None
        if self.decode_textures:
            banks = []
            if self.textures_big:
                banks.append(bpy.path.abspath(self.textures_big))
            banks += fc.guess_texture_banks(path)
            banks = list(dict.fromkeys(banks))  # de-dup, keep order
            if banks:
                tex_bank = fc.TextureBank(banks, tools_dir=tools_dir)

        # collection for this import
        coll = bpy.data.collections.new("Fable_" + os.path.basename(path))
        context.scene.collection.children.link(coll)

        targets = []
        if self.import_all:
            targets = meshes[: self.max_all]
        else:
            e = fc.find_entry(buf, meshes, self.entry.strip())
            if e is None:
                self.report({"ERROR"}, "Entry %r not found" % self.entry)
                return {"CANCELLED"}
            targets = [e]

        total = dict(models=0, materials=0, textures=0, bones=0, submeshes=0)
        for e in targets:
            model = fc.build_model(buf, e, tex_bank=tex_bank,
                                   decode_textures=self.decode_textures,
                                   tools_dir=tools_dir)
            r = build_model_in_scene(model, collection=coll, src_big=path)
            total["models"] += 1
            total["materials"] += r["materials"]
            total["textures"] += r["textures"]
            total["bones"] += r["bones"]
            total["submeshes"] += r["submeshes"]

        self.report(
            {"INFO"},
            "Fable import: %d model(s), %d material(s) (%d textured), %d bone(s), "
            "%d submesh placeholder(s). Geometry stubbed pending RE."
            % (total["models"], total["materials"], total["textures"],
               total["bones"], total["submeshes"]))
        return {"FINISHED"}


# ---------------------------------------------------------------------------
# UI: File > Import menu entry + a small N-panel
# ---------------------------------------------------------------------------
def menu_func_import(self, context):
    self.layout.operator(ImportFableModel.bl_idname, text="Fable TLC Model (.big)")


class FABLE_PT_import_panel(Panel):
    bl_label = "Fable TLC Import"
    bl_idname = "FABLE_PT_import_panel"
    bl_space_type = "VIEW_3D"
    bl_region_type = "UI"
    bl_category = "Fable TLC"

    def draw(self, context):
        layout = self.layout
        layout.label(text="Import Fable: The Lost Chapters assets")
        layout.operator(ImportFableModel.bl_idname, icon="IMPORT",
                        text="Import Model (.big)")
        box = layout.box()
        box.label(text="Imports today:")
        box.label(text="- Materials + real textures", icon="TEXTURE")
        box.label(text="- Skeleton (real bind pose)", icon="ARMATURE_DATA")
        box.label(text="- LOD/submesh tree", icon="OUTLINER")
        box.label(text="- Geometry: STUB (pending RE)", icon="ERROR")


class ExportFableModel(Operator, ExportHelper):
    """Export edited mesh geometry back into a Fable graphics.big (STAGED copy).

    Operates on the selected mesh (imported from a .big, so it carries the
    fable_* tags). Reads its per-vertex positions, encodes them into the source
    primitive's vertex buffer, rebuilds the bank, and writes a STAGED .big -- the
    Steam install is NEVER touched. v1: edit vertex POSITIONS keeping the original
    topology (same vertex count/order); object-level transforms are ignored (edit
    in Edit Mode). Load the staged .big in-game to test.
    """
    bl_idname = "export_scene.fable_model"
    bl_label = "Export Fable TLC Mesh (staged .big)"
    bl_options = {"REGISTER"}
    filename_ext = ".big"
    filter_glob: StringProperty(default="*.big", options={"HIDDEN"})
    tools_dir: StringProperty(name="Tools dir (optional)", default="")

    def execute(self, context):
        from . import mesh_rw
        obj = context.active_object
        if obj is None or obj.type != "MESH" or "fable_entry" not in obj:
            self.report({"ERROR"}, "Select a mesh imported from a Fable .big "
                        "(it must carry the 'fable_entry' tag).")
            return {"CANCELLED"}
        src_big = obj.get("fable_src_big", "")
        entry_name = obj.get("fable_entry", "")
        if not src_big or not os.path.isfile(src_big):
            self.report({"ERROR"}, "Source graphics.big not found: %r" % src_big)
            return {"CANCELLED"}
        if os.path.abspath(self.filepath) == os.path.abspath(src_big):
            self.report({"ERROR"}, "Refusing to overwrite the source install .big; "
                        "choose a different (staged) output path.")
            return {"CANCELLED"}
        tools_dir = self.tools_dir or None
        try:
            fc._load_readers(tools_dir)
        except ImportError as ex:
            self.report({"ERROR"}, str(ex))
            return {"CANCELLED"}
        import big_write

        buf, meshes = fc.open_graphics_big(src_big, tools_dir=tools_dir)
        e = fc.find_entry(buf, meshes, entry_name)
        if e is None:
            self.report({"ERROR"}, "Entry %r not found in %s" % (entry_name, src_big))
            return {"CANCELLED"}
        pay = bytes(buf[e["offset"]:e["offset"] + e["size"]])
        _n, prims, _bones = fc._parse_mesh_primitives(pay, e["type"])
        segs = mesh_rw.parse_segments(pay, e["type"])

        # apply every edited mesh object that came from this same entry
        siblings = [o for o in bpy.data.objects
                    if o.type == "MESH" and o.get("fable_entry") == entry_name
                    and o.get("fable_src_big") == src_big]
        edited, skipped = 0, []
        for o in siblings:
            pi = int(o.get("fable_prim", -1))
            if not (0 <= pi < len(prims)):
                continue
            prim = prims[pi]
            nv = prim["vc"] * (prim["reps"] if prim["reps"] > 1 else 1)
            if len(o.data.vertices) != nv:
                skipped.append("%s (verts %d != %d)" % (o.name, len(o.data.vertices), nv))
                continue
            positions = [(v.co.x, v.co.y, v.co.z) for v in o.data.vertices]
            new_vb = mesh_rw.encode_positions(prim["vb"], prim["stride"], prim["init"],
                                              e["type"], positions, prim["scale"],
                                              prim["off"], prim["abc"] > 0)
            segs = mesh_rw.replace_block(segs, ("vbuf", pi), new_vb)
            edited += 1
        if edited == 0:
            self.report({"ERROR"}, "No primitives with matching topology to export. %s"
                        % ("; ".join(skipped) if skipped else ""))
            return {"CANCELLED"}

        new_pay = mesh_rw.serialize_segments(segs)
        new_gb = big_write.rebuild(buf, {(e["sub"], entry_name): new_pay})
        if os.path.isfile(self.filepath):
            try:
                os.replace(self.filepath, self.filepath + ".forgebak")
            except OSError:
                pass
        with open(self.filepath, "wb") as f:
            f.write(new_gb)
        msg = ("Exported %d primitive(s) of %s -> %s (%d bytes). Source install "
               "untouched; load this staged .big in-game to test."
               % (edited, entry_name, os.path.basename(self.filepath), len(new_gb)))
        if skipped:
            msg += " Skipped (topology changed - needs index remap): " + "; ".join(skipped)
        self.report({"INFO"}, msg)
        return {"FINISHED"}


def menu_func_export(self, context):
    self.layout.operator(ExportFableModel.bl_idname, text="Fable TLC Mesh (staged .big)")


_CLASSES = (ImportFableModel, ExportFableModel, FABLE_PT_import_panel)


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
