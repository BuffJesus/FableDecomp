# SPDX-License-Identifier: MIT
"""bpy-FREE core for the Fable LEV terrain <-> Blender heightmap bridge.

This module holds ALL the LEV logic and knows NOTHING about Blender, so it is
unit-testable standalone. The thin bpy layer (level_io.py) imports it and maps
its plain-data structures onto Blender datablocks.

It wraps the project's byte-exact reader/writer `tools/lev_rw.py` (DONE, 399/399
retail LEVs round-trip byte-identical). We never re-implement the format here --
we only:

  * build a plain heightmap (a flat list of per-vertex Z = cell world-height, in
    the same (w+1)x(h+1) row-major order the mesh will use), plus grid metadata;
  * read edited per-vertex Z back into the live LevFile via `set_height` and
    write a STAGED .lev with `write_file`.

Honest scope (inherited from lev_rw): editing cell heights does NOT regenerate
the navmesh / region graph / obstacle-brush data -- those are preserved opaque.
The engine's runtime navigation still reflects the ORIGINAL terrain. See
docs/TERRAIN_BLENDER.md.
"""
import importlib.util
import os
import shutil
import sys

# ---------------------------------------------------------------------------
# Locate + import tools/lev_rw.py (the byte-exact reader/writer we wrap).
# The addon may be installed anywhere, so we search a few sensible roots.
# ---------------------------------------------------------------------------
_lev_rw = None


def _candidate_tools_dirs(explicit=None):
    """Yield plausible directories that contain lev_rw.py, most-specific first."""
    seen = set()

    def _add(d):
        if d:
            d = os.path.abspath(d)
            if d not in seen:
                seen.add(d)
                return d
        return None

    out = []
    if explicit:
        out.append(_add(explicit))
    # This file lives at <repo>/tools/blender_addon/io_scene_fable_level/level_core.py
    here = os.path.dirname(os.path.abspath(__file__))
    # -> tools/blender_addon -> tools
    tools = os.path.abspath(os.path.join(here, os.pardir, os.pardir))
    out.append(_add(tools))
    # a copy vendored next to this module
    out.append(_add(here))
    # env var escape hatch
    out.append(_add(os.environ.get("FABLETLC_TOOLS")))
    return [d for d in out if d]


def load_lev_rw(tools_dir=None):
    """Import lev_rw as a module object (cached). Raises ImportError if not found."""
    global _lev_rw
    if _lev_rw is not None and tools_dir is None:
        return _lev_rw
    for d in _candidate_tools_dirs(tools_dir):
        cand = os.path.join(d, "lev_rw.py")
        if os.path.isfile(cand):
            spec = importlib.util.spec_from_file_location("fable_lev_rw", cand)
            mod = importlib.util.module_from_spec(spec)
            # ensure it can find its own stdlib-only deps; register so it's stable
            sys.modules.setdefault("fable_lev_rw", mod)
            spec.loader.exec_module(mod)
            _lev_rw = mod
            return mod
    raise ImportError(
        "Could not locate lev_rw.py. Set the 'Tools dir' to the FableTLC "
        "tools/ folder (the one containing lev_rw.py), or set the FABLETLC_TOOLS "
        "environment variable. Searched: %s"
        % ", ".join(_candidate_tools_dirs(tools_dir)))


# ---------------------------------------------------------------------------
# Plain-data structures (no bpy, no lev_rw types leaked)
# ---------------------------------------------------------------------------
class Heightmap:
    """A plain, Blender-ready description of a LEV's terrain grid.

    Vertices are (w+1)x(h+1) in row-major order (row y outer, col x inner),
    identical to lev_rw's cell ordering, so the mesh vertex index maps 1:1 to a
    cell via `index = y * cols + x`.

      cols, rows   : grid dims  = lev.cells_x, lev.cells_y  (= width+1, height+1)
      heights      : flat list[float], world-space height per vertex (raw*2048)
      themes       : flat list[int],  theme_index(layer 0) per vertex (for color)
      source_path  : the .lev this came from (staged provenance for export)
      map_width/height, sub_version, uid : metadata echoed onto the mesh object
    """
    __slots__ = ("cols", "rows", "heights", "themes",
                 "source_path", "map_width", "map_height", "sub_version",
                 "uid", "cell_size_xy")

    def __init__(self):
        self.cols = 0
        self.rows = 0
        self.heights = []
        self.themes = []
        self.source_path = ""
        self.map_width = 0
        self.map_height = 0
        self.sub_version = None
        self.uid = 0
        # World XY spacing between adjacent cells. LEV stores no explicit cell
        # pitch in the fields we parse; Fable terrain cells are a fixed world
        # unit apart. We use 1.0 grid-unit per cell for the mesh so the mesh is
        # a faithful, evenly-spaced heightfield; height (Z) carries the real
        # world value. XY scale is cosmetic for sculpting and never exported.
        self.cell_size_xy = 1.0

    @property
    def vert_count(self):
        return self.cols * self.rows

    def build_faces(self):
        """Quad faces between adjacent verts. Returns list[tuple(4 ints)]."""
        faces = []
        cols = self.cols
        for y in range(self.rows - 1):
            row0 = y * cols
            row1 = row0 + cols
            for x in range(cols - 1):
                a = row0 + x
                b = a + 1
                c = row1 + x + 1
                d = row1 + x
                faces.append((a, b, c, d))
        return faces

    def vertex_coords(self):
        """Yield (x, y, z) world-ish coords for each vertex, matching `heights`
        order. X/Y are grid position * cell_size_xy; Z is the world height."""
        cols = self.cols
        s = self.cell_size_xy
        for i, z in enumerate(self.heights):
            gx = i % cols
            gy = i // cols
            yield (gx * s, gy * s, z)


# ---------------------------------------------------------------------------
# IMPORT side: LEV -> Heightmap
# ---------------------------------------------------------------------------
def read_heightmap(lev_path, tools_dir=None, want_themes=True):
    """Parse a .lev and return a Heightmap (plain data, no bpy)."""
    lev_rw = load_lev_rw(tools_dir)
    lev = lev_rw.read_file(lev_path)
    hm = Heightmap()
    hm.cols = lev.cells_x
    hm.rows = lev.cells_y
    hm.source_path = os.path.abspath(lev_path)
    hm.map_width = lev.width
    hm.map_height = lev.height
    hm.sub_version = lev.sub_version
    hm.uid = lev.uid

    heights = hm.heights
    themes = hm.themes
    cols = hm.cols
    for y in range(hm.rows):
        for x in range(cols):
            c = lev.cell(x, y)
            heights.append(c.height)
            if want_themes:
                themes.append(c.theme_index(0))
    return hm


# ---------------------------------------------------------------------------
# EXPORT side: edited per-vertex Z -> STAGED .lev
# ---------------------------------------------------------------------------
def stage_export(source_lev_path, out_path, cols, rows, get_z,
                 tools_dir=None, make_backup=True, eps=0.0):
    """Read edited heights back into the source LEV and write a STAGED copy.

    We RE-READ the original .lev (source_lev_path) so that every opaque span --
    obstacle/brush region, navmesh, theme, walkability -- is preserved verbatim;
    only cell heights the user actually changed are written through.

      cols, rows : must match the source LEV grid exactly (safety check).
      get_z(x,y) : callback returning the edited world-height for vertex (x,y).
      out_path   : STAGED destination (never the game install -- caller enforces).
      make_backup: if out_path already exists, copy it to out_path + ".bak".
      eps        : only call set_height when |new-old| > eps (0 = always write;
                   set_height is idempotent so writing an unchanged value is a
                   no-op on the bytes anyway).

    Returns a dict report: {changed_cells, cols, rows, out_path, backup}.
    """
    lev_rw = load_lev_rw(tools_dir)
    if not os.path.isfile(source_lev_path):
        raise FileNotFoundError("source .lev not found: %s" % source_lev_path)
    lev = lev_rw.read_file(source_lev_path)

    if cols != lev.cells_x or rows != lev.cells_y:
        raise ValueError(
            "grid mismatch: mesh is %dx%d but source LEV %s is %dx%d. "
            "This exporter does not resize terrain."
            % (cols, rows, source_lev_path, lev.cells_x, lev.cells_y))

    changed = 0
    for y in range(rows):
        for x in range(cols):
            old = lev.get_height(x, y)
            new = float(get_z(x, y))
            if eps <= 0.0 or abs(new - old) > eps:
                lev.set_height(x, y, new)
                if new != old:
                    changed += 1

    backup = None
    out_path = os.path.abspath(out_path)
    os.makedirs(os.path.dirname(out_path), exist_ok=True)
    if make_backup and os.path.isfile(out_path):
        backup = out_path + ".bak"
        shutil.copy2(out_path, backup)

    lev_rw.write_file(lev, out_path)
    return dict(changed_cells=changed, cols=cols, rows=rows,
                out_path=out_path, backup=backup)
