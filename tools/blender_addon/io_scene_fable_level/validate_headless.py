# SPDX-License-Identifier: MIT
"""Headless validation gate for io_scene_fable_level.

Run:
  blender --background --factory-startup --python validate_headless.py -- \
      <BarrowFields.lev> <scratch_dir>

Gate:
  1. Import BarrowFields.lev -> assert a heightmap mesh with EXACTLY
     (w+1)*(h+1) verts, and every vertex Z == lev_rw's cell height for that cell.
  2. Edit ONE vertex's Z, export to a scratch .lev (staged), re-read via lev_rw,
     and confirm EXACTLY that one cell's height changed and all others are
     unchanged -- both by cell-height comparison AND by byte-diff against a
     clean re-stage of the unedited mesh.
  3. Save a .blend as evidence.

Exit code 0 = PASS, 1 = FAIL.
"""
import os
import sys

import bpy

# --- locate our package + lev_rw ---
HERE = os.path.dirname(os.path.abspath(__file__))
PKG_PARENT = os.path.abspath(os.path.join(HERE, os.pardir))  # tools/blender_addon
TOOLS_DIR = os.path.abspath(os.path.join(HERE, os.pardir, os.pardir))  # tools/
for p in (PKG_PARENT, TOOLS_DIR):
    if p not in sys.path:
        sys.path.insert(0, p)

from io_scene_fable_level import level_core as lc  # noqa: E402
from io_scene_fable_level import level_io as lio  # noqa: E402

lev_rw = lc.load_lev_rw(TOOLS_DIR)


def _argv_after_dashes():
    if "--" in sys.argv:
        return sys.argv[sys.argv.index("--") + 1:]
    return []


def fail(msg):
    print("FAIL:", msg)
    sys.exit(1)


def main():
    args = _argv_after_dashes()
    if len(args) < 2:
        fail("usage: ... -- <BarrowFields.lev> <scratch_dir>")
    lev_path = os.path.abspath(args[0])
    scratch = os.path.abspath(args[1])
    os.makedirs(scratch, exist_ok=True)
    print("== io_scene_fable_level headless validation ==")
    print("LEV     :", lev_path)
    print("scratch :", scratch)

    # clean scene
    bpy.ops.wm.read_factory_settings(use_empty=True)

    # ---- 1. IMPORT ----
    hm = lc.read_heightmap(lev_path, tools_dir=TOOLS_DIR)
    obj = lio.build_terrain_mesh(hm, collection=bpy.context.scene.collection)
    me = obj.data
    expected_verts = hm.cols * hm.rows
    print("[1] imported %dx%d grid -> %d verts (faces %d)"
          % (hm.cols, hm.rows, len(me.vertices), len(me.polygons)))
    if len(me.vertices) != expected_verts:
        fail("vertex count %d != expected %d" % (len(me.vertices), expected_verts))

    # ground truth: re-read the LEV and compare every cell height to vertex Z
    lev_truth = lev_rw.read_file(lev_path)
    cols = hm.cols
    max_dz = 0.0
    for i, v in enumerate(me.vertices):
        x = i % cols
        y = i // cols
        truth = lev_truth.get_height(x, y)
        dz = abs(v.co.z - truth)
        if dz > max_dz:
            max_dz = dz
    print("[1] max |vertZ - cellHeight| across all %d verts = %g"
          % (expected_verts, max_dz))
    # heights are float32 in the file; Blender stores float32 -> expect exact.
    if max_dz > 1e-3:
        fail("vertex Z does not match cell heights (max dz %g)" % max_dz)
    print("[1] PASS: heightmap verts match lev_rw cell heights")

    # verify custom props tagged for export
    for k in ("fable_lev_source", "fable_lev_cols", "fable_lev_rows"):
        if k not in obj:
            fail("missing custom prop %s" % k)

    # ---- 2a. control export (no edit) -> should equal source bytes ----
    control_out = os.path.join(scratch, "BarrowFields_control.lev")
    rep0 = lio.export_terrain_mesh(obj, control_out, tools_dir=TOOLS_DIR,
                                   make_backup=False)
    print("[2] control export: %d cell(s) changed -> %s"
          % (rep0["changed_cells"], control_out))
    with open(lev_path, "rb") as f:
        src_bytes = f.read()
    with open(control_out, "rb") as f:
        ctrl_bytes = f.read()
    if ctrl_bytes != src_bytes:
        # find first diff for diagnostics
        n = min(len(ctrl_bytes), len(src_bytes))
        first = next((i for i in range(n) if ctrl_bytes[i] != src_bytes[i]), n)
        fail("control (un-edited) export is NOT byte-identical to source; "
             "first diff @%d (len src=%d ctrl=%d)"
             % (first, len(src_bytes), len(ctrl_bytes)))
    print("[2] PASS: un-edited export is byte-identical to source LEV")

    # ---- 2b. edit ONE vertex, export, re-read ----
    tx, ty = cols // 2, hm.rows // 2
    ti = ty * cols + tx
    before_z = me.vertices[ti].co.z
    delta = 500.0
    me.vertices[ti].co.z = before_z + delta
    me.update()
    print("[2] edited vertex (%d,%d) idx %d: Z %.3f -> %.3f"
          % (tx, ty, ti, before_z, me.vertices[ti].co.z))

    edited_out = os.path.join(scratch, "BarrowFields_edited.lev")
    rep = lio.export_terrain_mesh(obj, edited_out, tools_dir=TOOLS_DIR,
                                  make_backup=False)
    print("[2] edited export: %d cell(s) changed -> %s"
          % (rep["changed_cells"], edited_out))

    # re-read via lev_rw and confirm ONLY the target cell changed value
    lev_edited = lev_rw.read_file(edited_out)
    changed_cells = []
    for y in range(hm.rows):
        for x in range(cols):
            if lev_edited.get_height(x, y) != lev_truth.get_height(x, y):
                changed_cells.append((x, y))
    print("[2] cells whose height value changed: %s" % (changed_cells,))
    if changed_cells != [(tx, ty)]:
        fail("expected exactly cell (%d,%d) to change, got %s"
             % (tx, ty, changed_cells))
    new_h = lev_edited.get_height(tx, ty)
    if abs(new_h - (before_z + delta)) > 1e-2:
        fail("edited cell height %.3f != expected %.3f" % (new_h, before_z + delta))
    print("[2] target cell (%d,%d) height %.3f -> %.3f (expected %.3f)"
          % (tx, ty, lev_truth.get_height(tx, ty), new_h, before_z + delta))

    # ---- 2c. byte-diff: edited vs control differ ONLY in that cell's 4 height bytes ----
    with open(edited_out, "rb") as f:
        edited_bytes = f.read()
    if len(edited_bytes) != len(ctrl_bytes):
        fail("edited file length %d != control length %d"
             % (len(edited_bytes), len(ctrl_bytes)))
    byte_diffs = [i for i in range(len(edited_bytes))
                  if edited_bytes[i] != ctrl_bytes[i]]
    # expected byte range: prefix + (cellIndex*21) + OFF_HEIGHT(5) .. +4
    cell_base = (lev_truth._prefix_len
                 + (ty * cols + tx) * lev_rw.CELL_SIZE + lev_rw.OFF_HEIGHT)
    expected_bytes = list(range(cell_base, cell_base + 4))
    print("[2] byte diffs edited-vs-control: %s (expected %s)"
          % (byte_diffs, expected_bytes))
    if byte_diffs != expected_bytes:
        fail("byte-diff not confined to the target cell's 4 height bytes")
    print("[2] PASS: edit round-trip confined to exactly one cell's height bytes")

    # ---- 3. save evidence .blend ----
    blend_out = os.path.join(scratch, "BarrowFields_terrain.blend")
    bpy.ops.wm.save_as_mainfile(filepath=blend_out)
    print("[3] saved evidence .blend -> %s" % blend_out)

    print("\nALL CHECKS PASS")
    sys.exit(0)


if __name__ == "__main__":
    main()
