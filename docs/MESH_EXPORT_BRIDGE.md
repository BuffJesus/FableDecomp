# Mesh export bridge — custom models INTO graphics.big (Route B: native, no EgoCore exe)

*Authored 2026-07-19. The "get in" half of the Blender ⇄ Fable bridge
(`CONTENT_AUTHORING_PLAN.md` B2), built natively per user's Route-B choice. The
"get out" (import) half is done — see `BLENDER_ADDON.md` / `BIG_MESH_GEOMETRY.md`.*

## Status

| Piece | Status |
|-------|--------|
| Native serializer (`mesh_rw.py`) | **DONE** — segment-capture + stored-chunk write |
| Round-trip oracle | **DONE** — 58/60 sampled meshes semantic-identical; the 2 fails are a DECODE bug (`#10`), not the writer |
| `replace_block` edit mechanism | **DONE** — identity swap verified |
| Blender "Export to Fable" operator | **TODO** (`#5`) — needs the vertex-field encoder + `.big` write |
| In-game load test | **user-only** (can't be automated here) |

## How the write path works (`tools/blender_addon/io_scene_fable/mesh_rw.py`)

The on-disk compiled mesh is plain header/stats fields **interleaved** with
**Fable chunked-LZO** sub-blocks (vertex buffer, index buffer, bones, etc.;
`[u16 clen][clen LZO]` filling `size-3`, then 3 trailing plain bytes — see
`BIG_MESH_GEOMETRY.md`). The writer avoids re-serialising every field:

1. **`parse_segments(pay, type)`** walks the entry exactly like the decoder and
   emits an ordered list: `('plain', bytes)` for un-framed spans (copied verbatim)
   and `('block', decoded_bytes, tag)` for each LZO sub-block (captured *decoded*).
2. **`serialize_segments(segs)`** re-emits plain spans verbatim and each block as a
   **STORED chunk** — `b'\x00\x00' + block` — which `DecompressLZO`'s `clen==0`
   path reads back byte-for-byte. **No LZO compressor is needed** (real game data
   already uses stored chunks for small blocks; we use them for all).
3. **`replace_block(segs, tag, new_bytes)`** swaps one block; everything else stays
   byte-identical. Editing a mesh = replace the `('vbuf', i)` and/or `('ibuf', i)`
   block, re-serialise.

**Oracle (`roundtrip_report`)**: re-parse the original and the re-serialised
payload and compare every primitive's vertex/index bytes → proves
`decode(serialize(parse(x))) == decode(x)`.

## The vertex-field editing model (for `#5`)

A safe, minimal geometry edit **overwrites only the position (and UV) fields
in-place** in the existing vertex buffer, leaving normals / bone indices / weights
untouched — so we never have to *regenerate* normals or skin weights:

- **Position** is the first stride element: packed `s16×3` (`is_pos_comp`) at
  offset 0, or `FLOAT3` at offset 0. Re-encode a packed position as the inverse of
  `UnpackPOSPACKED3` using the primitive's own `CVertexCompressionParams`
  (scale/offset): `i = round((v - offset) / (scale·S))` — lossless for unedited
  verts, nearest-quantised for moved ones.
- **UV** at `u_off` (see `GltfExporter` offset table): s11e4 `int16` pair
  (`DecompressUV` inverse) or float pair.
- Index buffer edits (retopo/added faces) re-emit the whole `('ibuf', i)` block.

For a *brand-new* mesh (not an edit of an existing one) the encoder must also
build the stats/material/bone scaffold — larger, but the same block writer applies.

## Blender "Export to Fable" operator (`#5`, planned)

Blender mesh → per-vertex position/UV → encode into the vertex buffer (model above)
→ `replace_block` → **write graphics.big** via `forge::big` (or a native BIG
writer), output **STAGED and reversible** (`forge stage`/`unstage`). Regenerate the
entry **Info** metadata blob (bbox / LOD sizes / texture IDs — EgoCore
`UpdateMetadata` equivalent) so the bank entry stays consistent.

## Gates before promising custom models

- **Round-trip oracle** — DONE (semantic identity on decodable meshes).
- **Decode edge case** (`#10`) — 2/60 meshes fail LZO decode (`bad match distance`);
  affects import + export equally; fix before a broad "any mesh" claim.
- **Staging/reversibility** — never write the Steam install directly; stage + back up.
- **In-game load** — the one link only a live run confirms (the game actually loading
  the edited `graphics.big`). Automatable up to the launch; the launch is the user's.

## Where this fits

This is one axis of the creation suite. The custom-dungeon capstone (`#11`, e.g. a
Demon Door opening into a full custom region) combines this (architecture/props)
with the LEV terrain writer (`LEV_WRITER.md`, DONE), `forge::tng` placements, and the
new-level + navmesh RE (`#12`). Optional size optimisation: a native LZO1X
compressor (`#7`) lets the writer emit compressed instead of stored chunks.
