# SPDX-License-Identifier: MIT
"""Blender (bpy) layer for Fable TLC ANIM (3DAF) <-> Blender armature actions.

Import direction: decode a retail ANIM entry (tools/parse_anim_xseq.py) and
build a Blender Action on an imported Fable rig (the armature built by
import_op._build_armature from the mesh's REAL bind poses).

Export direction: sample an action per anim frame and clone-a-donor-clip
(tools/anim_build.py recipe): parse the donor 3DAF image, replace each track's
quat/pos pools + palettes with resampled data, re-serialize, recompress and
inject via big_write.rebuild into a STAGED .big.

Coordinate convention (probed against retail, see docs/ANIM_WRITER.md §7):
the importer maps Fable model space to Blender space VERBATIM (no axis swap),
so an anim track's data is, in the armature's space:

    pos key  p = parent-relative local translation of the bone (bind units)
    quat key q = CONJUGATE of the mathutils-convention local rotation
                 (q_anim(x,y,z,w) with q_local = Quaternion((w,-x,-y,-z)))

For a stationary bone the anim key equals the mesh bind-local transform
exactly (up to i16 quantization / quaternion sign), which is how the
convention was pinned down.

Blender pose-bone math used both directions (exact inverses of each other):

    rest_local = parent.bone.matrix_local^-1 @ bone.matrix_local (root: m_l)
    basis_q = conj(rest_q) @ q_local          q_local = rest_q @ basis_q
    basis_p = conj(rest_q) @ (p - rest_t)     p = rest_t + rest_q @ basis_p

Anim frame i lives at Blender frame i + 1 (FRAME0). fps is stored on the
action as the custom prop "fable_fps" (Blender scene fps is only cosmetic).
"""
import os
import struct

import bpy
from bpy.props import BoolProperty, StringProperty
from bpy.types import Operator
from bpy_extras.io_utils import ExportHelper, ImportHelper
from mathutils import Quaternion, Vector

from . import fable_core as fc

FRAME0 = 1.0  # Blender frame of anim frame 0

# Lazily imported anim tool handles (from the repo tools/ dir).
parse_anim_xseq = None
anim_build = None
big_write = None


def _load_anim_tools(tools_dir=None):
    global parse_anim_xseq, anim_build, big_write
    if parse_anim_xseq is not None:
        return
    fc._load_readers(tools_dir)   # puts tools/ on sys.path (or raises)
    import parse_anim_xseq as _pa
    import anim_build as _ab
    import big_write as _bw
    parse_anim_xseq, anim_build, big_write = _pa, _ab, _bw


# ---------------------------------------------------------------------------
# rest-pose helpers
# ---------------------------------------------------------------------------
def _rest_local(bone):
    """Bone's rest matrix relative to its parent (armature space)."""
    if bone.parent is not None:
        return bone.parent.matrix_local.inverted() @ bone.matrix_local
    return bone.matrix_local.copy()


def _rest_qt(bone):
    m = _rest_local(bone)
    return m.to_quaternion(), m.to_translation()


def _anim_quat_to_local(q_anim):
    """(x,y,z,w) anim quat -> mathutils local rotation (conjugate)."""
    return Quaternion((q_anim[3], -q_anim[0], -q_anim[1], -q_anim[2]))


def _local_quat_to_anim(q_local):
    """mathutils local rotation -> (x,y,z,w) anim quat (conjugate)."""
    return (-q_local.x, -q_local.y, -q_local.z, q_local.w)


# ---------------------------------------------------------------------------
# Action fcurve container (legacy Action.fcurves OR slotted actions, 4.4+)
# ---------------------------------------------------------------------------
def _fcurve_container(action, arm_obj=None, create=False):
    """Return an object with .new(data_path, index=, ...)/.find(data_path,
    index=) for this action, coping with both the legacy Action.fcurves API
    and slotted actions (Blender 4.4+/5.x channelbags)."""
    fcs = getattr(action, "fcurves", None)
    if fcs is not None:
        return fcs
    # slotted-only build
    if not action.slots:
        if not create:
            return None
        action.slots.new(id_type="OBJECT",
                         name=(arm_obj.name if arm_obj else "Fable"))
    slot = action.slots[0]
    if not action.layers:
        if not create:
            return None
        action.layers.new("Layer")
    layer = action.layers[0]
    if not layer.strips:
        if not create:
            return None
        layer.strips.new(type="KEYFRAME")
    strip = layer.strips[0]
    return strip.channelbag(slot, ensure=True).fcurves


def _fc_new(fcs, data_path, index, group):
    try:
        return fcs.new(data_path, index=index, action_group=group)
    except TypeError:
        return fcs.new(data_path, index=index)


def _assign_action(arm_obj, action):
    ad = arm_obj.animation_data or arm_obj.animation_data_create()
    ad.action = action
    # slotted actions need the slot picked explicitly
    if hasattr(ad, "action_slot") and getattr(action, "slots", None):
        try:
            ad.action_slot = action.slots[0]
        except Exception:
            pass


# ---------------------------------------------------------------------------
# IMPORT: decoded Anim -> Blender Action on an imported Fable armature
# ---------------------------------------------------------------------------
def build_action_from_anim(arm_obj, anim, name="FableAnim"):
    """Create a Blender Action on `arm_obj` (an armature imported by this
    addon) from a parse_anim_xseq.Anim. Returns (action, stats dict).

    Per track (skipping bones absent from the armature): keyframes every anim
    frame i at Blender frame i+FRAME0, rotation_quaternion and/or location
    channels only when the track carries the respective pool. LINEAR
    interpolation; quat keys are sign-aligned along time for clean slerp."""
    action = bpy.data.actions.new(name)
    fcs = _fcurve_container(action, arm_obj, create=True)
    stats = dict(tracks=0, skipped=[], rot_keys=0, pos_keys=0, frames=0)
    max_fps = 0.0

    for t in anim.tracks:
        pb = arm_obj.pose.bones.get(t.bone_name)
        if pb is None:
            stats["skipped"].append(t.bone_name)
            continue
        if not t.rot_pool and not t.pos_pool:
            continue
        pb.rotation_mode = "QUATERNION"
        rest_q, rest_t = _rest_qt(pb.bone)
        rest_qi = rest_q.conjugated()
        n = t.frame_count
        stats["tracks"] += 1
        stats["frames"] = max(stats["frames"], n)
        max_fps = max(max_fps, t.fps)
        path = 'pose.bones["%s"]' % t.bone_name

        if t.rot_pool:
            quats = []
            prev = None
            for i in range(n):
                q_local = _anim_quat_to_local(t.rot_at(i))
                bq = rest_qi @ q_local
                if prev is not None and bq.dot(prev) < 0.0:
                    bq = -bq
                prev = bq
                quats.append((bq.w, bq.x, bq.y, bq.z))
            for k in range(4):
                f = _fc_new(fcs, path + ".rotation_quaternion", k, t.bone_name)
                f.keyframe_points.add(n)
                co = [0.0] * (2 * n)
                for i in range(n):
                    co[2 * i] = FRAME0 + i
                    co[2 * i + 1] = quats[i][k]
                f.keyframe_points.foreach_set("co", co)
                for kp in f.keyframe_points:
                    kp.interpolation = "LINEAR"
                f.update()
            stats["rot_keys"] += n

        if t.pos_pool:
            locs = []
            for i in range(n):
                p = Vector(t.pos_at(i))
                bp = rest_qi @ (p - rest_t)
                locs.append((bp.x, bp.y, bp.z))
            for k in range(3):
                f = _fc_new(fcs, path + ".location", k, t.bone_name)
                f.keyframe_points.add(n)
                co = [0.0] * (2 * n)
                for i in range(n):
                    co[2 * i] = FRAME0 + i
                    co[2 * i + 1] = locs[i][k]
                f.keyframe_points.foreach_set("co", co)
                for kp in f.keyframe_points:
                    kp.interpolation = "LINEAR"
                f.update()
            stats["pos_keys"] += n

    action["fable_fps"] = max_fps
    action["fable_frames"] = stats["frames"]
    action["fable_cyclic"] = 1 if anim.is_cyclic else 0
    action["fable_duration"] = anim.duration
    action["fable_rig"] = anim.object_name
    _assign_action(arm_obj, action)
    return action, stats


# ---------------------------------------------------------------------------
# EXPORT: Blender Action -> donor-cloned 3DAF image
# ---------------------------------------------------------------------------
def _sample_basis(fcs_by_path, path, frame):
    """Evaluate a pose bone's basis (quat, loc) from action fcurves at a
    (float) Blender frame. Missing channels default to identity/zero."""
    q = [1.0, 0.0, 0.0, 0.0]
    for k in range(4):
        f = fcs_by_path.get((path + ".rotation_quaternion", k))
        if f is not None:
            q[k] = f.evaluate(frame)
    p = [0.0, 0.0, 0.0]
    for k in range(3):
        f = fcs_by_path.get((path + ".location", k))
        if f is not None:
            p[k] = f.evaluate(frame)
    return Quaternion(q), Vector(p)


def _pool_palette(items):
    """Keyframe-reduce a per-frame list of hashable items into
    (pool, palette). Dense (no reduction win) -> empty palette."""
    pool, pal, index = [], [], {}
    for it in items:
        j = index.get(it)
        if j is None:
            j = len(pool)
            index[it] = j
            pool.append(it)
        pal.append(j)
    if len(pool) == len(items):
        return pool, []
    return pool, pal


def export_action_to_image(arm_obj, action, donor_img, frame_count=None):
    """Sample `action` on `arm_obj` and write it over a donor 3DAF image
    (bytes, decompressed) whose rig matches the armature -- the
    docs/ANIM_WRITER.md §4 donor-clone recipe, automated.

    Every donor track whose bone exists in the armature gets its non-empty
    rot/pos pools + palettes REPLACED by data sampled from the action at the
    donor track's own fps/frame grid (anim frame i == Blender frame i+FRAME0).
    Donor tracks with empty pools, tracks for bones missing from the armature,
    helper tracks, MVEC/TMEV/AMSK chunks and all header bytes are kept
    verbatim. frame_count=None keeps each donor track's count; an int retimes
    every replaced track and recomputes the ANRT duration.

    Returns (anim3daf, stats). Serialize with anim3daf.serialize(); payload
    via anim_build.compose_payload()."""
    _load_anim_tools()
    a3 = anim_build.parse_image(donor_img)
    fcs = _fcurve_container(action, arm_obj, create=False)
    fcs_by_path = {}
    if fcs is not None:
        for f in fcs:
            fcs_by_path[(f.data_path, f.array_index)] = f

    stats = dict(replaced_rot=0, replaced_pos=0, kept=0, missing_bones=[])
    retimed = frame_count is not None
    for node, t in a3.tracks():
        pb = arm_obj.pose.bones.get(t.bone_name)
        has_rot = len(t.rot_raw) >= 16
        has_pos = len(t.pos_int) >= 1
        if pb is None:
            if has_rot or has_pos:
                stats["missing_bones"].append(t.bone_name)
            stats["kept"] += 1
            continue
        if not has_rot and not has_pos:
            stats["kept"] += 1     # e.g. Scene Root / movement_dummy stubs
            if retimed:
                t.frame_count = frame_count
            continue
        n = frame_count if retimed else t.frame_count
        rest_q, rest_t = _rest_qt(pb.bone)
        path = 'pose.bones["%s"]' % t.bone_name

        quats, locs = [], []
        prev = None
        for i in range(n):
            bq, bp = _sample_basis(fcs_by_path, path, FRAME0 + i)
            q_local = rest_q @ bq
            qa = _local_quat_to_anim(q_local)
            # sign-continuity along time (engine slerps between pool keys)
            if prev is not None and sum(a * b for a, b in zip(qa, prev)) < 0.0:
                qa = tuple(-c for c in qa)
            prev = qa
            # store at f32 precision so palette dedup sees storage-identical keys
            quats.append(struct.unpack('<4f', struct.pack('<4f', *qa)))
            p = rest_t + rest_q @ bp
            locs.append((p.x, p.y, p.z))

        if has_rot:
            pool, pal = _pool_palette(quats)
            t.rot_keys = pool
            t.rot_palette = pal
            stats["replaced_rot"] += 1
        if has_pos:
            t.set_pos_keys(locs)          # quantize i16 + auto positionFactor
            pool, pal = _pool_palette(t.pos_int)
            t.pos_int = pool
            t.pos_palette = pal
            stats["replaced_pos"] += 1
        if retimed:
            t.frame_count = n

    if retimed:
        dur = 0.0
        for _n, t in a3.tracks():
            if t.fps > 0 and t.frame_count:
                dur = max(dur, t.frame_count / t.fps)
        for nd in a3.find(b'ANRT'):
            nd.duration = dur
        stats["duration"] = dur
    return a3, stats


# ---------------------------------------------------------------------------
# Operators
# ---------------------------------------------------------------------------
class ImportFableAnim(Operator, ImportHelper):
    """Import a Fable ANIM_* entry from graphics.big as an action on the
    active (Fable-imported) armature."""
    bl_idname = "import_scene.fable_anim"
    bl_label = "Import Fable TLC Animation"
    bl_options = {"REGISTER", "UNDO"}
    filename_ext = ".big"
    filter_glob: StringProperty(default="*.big", options={"HIDDEN"})
    entry: StringProperty(name="ANIM entry", default="ANIM_CREATURE_GOBLIN_ATTACK_01")
    tools_dir: StringProperty(name="Tools dir (optional)", default="")

    def execute(self, context):
        arm = context.active_object
        if arm is None or arm.type != "ARMATURE":
            self.report({"ERROR"}, "Select the imported Fable armature first.")
            return {"CANCELLED"}
        try:
            _load_anim_tools(self.tools_dir or None)
        except ImportError as ex:
            self.report({"ERROR"}, str(ex))
            return {"CANCELLED"}
        b, entries = parse_anim_xseq.load_anim_entries(self.filepath)
        match = [e for e in entries if e["name"] == self.entry] or \
                [e for e in entries if self.entry.upper() in e["name"].upper()]
        if not match:
            self.report({"ERROR"}, "No ANIM entry matching %r" % self.entry)
            return {"CANCELLED"}
        e = match[0]
        anim = parse_anim_xseq.parse_anim(b[e["offset"]:e["offset"] + e["size"]])
        if anim is None or not anim.tracks:
            self.report({"ERROR"}, "%s decoded to 0 tracks" % e["name"])
            return {"CANCELLED"}
        action, st = build_action_from_anim(arm, anim, name=e["name"])
        action["fable_src_big"] = self.filepath
        action["fable_entry"] = e["name"]
        msg = ("%s: %d tracks -> action %r (%d frames @ %g fps)"
               % (e["name"], st["tracks"], action.name, st["frames"],
                  action["fable_fps"]))
        if st["skipped"]:
            msg += "; bones not in armature: %s" % ", ".join(st["skipped"])
        self.report({"INFO"}, msg)
        return {"FINISHED"}


class ExportFableAnim(Operator, ExportHelper):
    """Export the active armature's action as a Fable ANIM entry into a
    STAGED .big (donor-clone; the source install is never touched)."""
    bl_idname = "export_scene.fable_anim"
    bl_label = "Export Fable TLC Animation (staged .big)"
    bl_options = {"REGISTER"}
    filename_ext = ".big"
    filter_glob: StringProperty(default="*.big", options={"HIDDEN"})
    src_big: StringProperty(
        name="Source graphics.big",
        description="Bank holding the donor clip (defaults to the action's "
                    "fable_src_big tag set by the anim importer)",
        default="", subtype="FILE_PATH")
    donor_entry: StringProperty(
        name="Donor ANIM entry",
        description="Retail clip for the SAME rig; its skeleton/track scaffold "
                    "is cloned and the pools replaced (defaults to the "
                    "action's fable_entry tag)",
        default="")
    out_name: StringProperty(
        name="Output entry name",
        description="Same as donor = in-place edit; a new name = added TOC "
                    "entry (new clip)",
        default="")
    tools_dir: StringProperty(name="Tools dir (optional)", default="")

    def execute(self, context):
        arm = context.active_object
        if arm is None or arm.type != "ARMATURE":
            self.report({"ERROR"}, "Select the imported Fable armature first.")
            return {"CANCELLED"}
        ad = arm.animation_data
        action = ad.action if ad else None
        if action is None:
            self.report({"ERROR"}, "Armature has no active action.")
            return {"CANCELLED"}
        src_big = self.src_big or action.get("fable_src_big", "")
        donor = self.donor_entry or action.get("fable_entry", "")
        out_name = self.out_name or donor
        if not src_big or not os.path.isfile(src_big):
            self.report({"ERROR"}, "Source graphics.big not found: %r" % src_big)
            return {"CANCELLED"}
        if not donor:
            self.report({"ERROR"}, "No donor ANIM entry given (and the action "
                        "carries no fable_entry tag).")
            return {"CANCELLED"}
        if os.path.abspath(self.filepath) == os.path.abspath(src_big):
            self.report({"ERROR"}, "Refusing to overwrite the source .big; "
                        "choose a staged output path.")
            return {"CANCELLED"}
        try:
            _load_anim_tools(self.tools_dir or None)
        except ImportError as ex:
            self.report({"ERROR"}, str(ex))
            return {"CANCELLED"}

        b, entries = parse_anim_xseq.load_anim_entries(src_big)
        by_name = {e["name"]: e for e in entries}
        de = by_name.get(donor)
        if de is None:
            self.report({"ERROR"}, "Donor entry %r not found" % donor)
            return {"CANCELLED"}
        img = parse_anim_xseq.decompress_payload(
            b[de["offset"]:de["offset"] + de["size"]])
        a3, st = export_action_to_image(arm, action, img)
        payload = anim_build.compose_payload(a3.serialize())

        if out_name in by_name:
            out = big_write.rebuild(b, edits={(de["bank"], out_name): payload})
        else:
            dur = st.get("duration")
            info = de["info"]
            if dur is not None and len(info) >= 24:
                mvec = struct.unpack_from('<3f', info, 8)
                rot = struct.unpack_from('<f', info, 20)[0]
                info = anim_build.anim_info_blob(dur, dur, mvec, rot)
            out = big_write.rebuild(b, adds=[dict(
                sub=de["bank"], name=out_name, payload=payload,
                type=de["type"], info=info, crc=de["crc"], deps=de["deps"])])
        with open(self.filepath, "wb") as f:
            f.write(out)
        msg = ("Exported action %r over donor %s -> %s as %s (%d rot / %d pos "
               "tracks replaced, %d kept)."
               % (action.name, donor, os.path.basename(self.filepath), out_name,
                  st["replaced_rot"], st["replaced_pos"], st["kept"]))
        if st["missing_bones"]:
            msg += " Donor bones not in armature (kept verbatim): " + \
                   ", ".join(st["missing_bones"])
        self.report({"INFO"}, msg)
        return {"FINISHED"}


def menu_func_import(self, context):
    self.layout.operator(ImportFableAnim.bl_idname, text="Fable TLC Animation (.big)")


def menu_func_export(self, context):
    self.layout.operator(ExportFableAnim.bl_idname,
                         text="Fable TLC Animation (staged .big)")


_CLASSES = (ImportFableAnim, ExportFableAnim)


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
