# Update for Niko (OpenFable) — 2026-07-21

*Delta since `NIKO_DOC_COMPARISON.md`. Fable: The Lost Chapters, retail `Fable.exe`. New this
session: the terrain-draw contradiction is now settled with decompiled write-side proof, plus
several artifacts you can cross-check against your function manual.*

---

## 1. The one hard contradiction (retail terrain draw) — now decompiled on the WRITE side

Our comparison flagged exactly one substantive disagreement (§c1): OpenFable builds a heightmesh
from the `.lev` grid at load; we said retail draws a **pre-baked STB mesh** and the `.lev` feeds
only nav/collision. This session we decompiled the **editor's bake path** (FableWin.exe, the level
editor that PRODUCES the STB), which is the write-side ground truth. It confirms our read.

**The baked landscape patch mesh is a real serialized VB/IB, not derived at load.** From
`CLandscapeBackgroundPatch::Save @0x02ce3220` (and the foreground `CEngineLandscapePatch::Save`,
same grammar), each 16×16-cell patch (17×17 = 289-vertex grid) writes:

```
u16 worldX, u16 worldY, u16 coord0, u16 coord1
EBOOL isWaterOnly, u8 detailMode
if !isWaterOnly:
  u16 indexCount, u16 vertexCount, u8 texExtX, u8 texExtY, EBOOL isDXT
  <inline texture: CTexture::SaveToDataStream = 19-byte header + raw DXT>   // baked INLINE
  VERTEX BUFFER, 16 bytes/vertex:
    u16 gridX, u16 gridY, f32 HEIGHT(=PeekLandscapeHeight), u32 packedNormal, u16 uv, u16 uv
    -> CRangeCompressor::Compress(stride=0x10)   (flags==0 = RAW/stored fallback)
  INDEX BUFFER (u16 * indexCount*3) -> CRangeCompressor
4x CPatchTesselationEdgeStrip::Save ; EBOOL hasWater [+ water subpatch]
```

Outer frame = `[uncompLen s32LE][compLen s32LE][lzo1x body]` (editor uses lzo1x_999; stock decoder).

**Why this resolves the disagreement in our favor for retail-faithfulness:** the height *is*
serialized per vertex into the STB (offset +4 of each 16-byte record) at BAKE time by the editor,
alongside a packed normal and an **inline DXT texture per patch**. At load, retail reads this mesh
directly (`CLocalDetailCacheMap::OpenStaticMap @0x00bdf010`); it does not re-triangulate from the
`.lev`. The `.lev` grid still feeds height/collision/nav (we both agree height scale = 2048), but
it is not the render source. So OpenFable's load-time heightmesh is a valid *reimplementation
choice*, not what the retail GPU draws.

**Useful to you if you ever want retail-exact LOD tiles:** the per-patch inline texture + the
background-LOD quadtree (0x24-byte dir entries: flags, frameOffset, frameSpan, 6× f32 AABB) are the
retail LOD cache. Full byte grammar in our `docs/TERRAIN_NATIVE_BAKE.md` (we're porting it to a
native writer so forge can bake terrain without the editor).

---

## 2. Independent confirmation of your CGSI vtable base

Your `SCRIPT_BINDINGS_CANDIDATES` cites the `CGameScriptInterface` vtable base at `0x1260F0C`. We
independently located it by scanning `.rdata` for the pointer run maximally overlapping the 891
CGSI method addresses: **vtable at `0x01260efc`**, 891/891 methods matched, and validated the slot
frame against 7 live ForgeFSE anchors (Destructor=0, StartScriptingEntity=8, IsRegionLoaded=12).
Same structure, cross-confirmed. We dumped all **921 slots** with demangled signatures
(`fable_cgsi_slots.h`) — e.g. `AddNewConversation`=slot 364 `long(CScriptThing const&, bool, bool)`,
`JamDoor`=slot 429 `void(CScriptThing const&)` (single-arg), `GetAllThingsInLevel`=slot 85
`long(CCharString const&, std::vector<CScriptThing>&)`.

---

## 3. Artifacts you can cross-check against your 200k-line function manual

We ran an evidence-based naming surge on retail `Fable.exe` this session (workflow: decomp +
named-caller/callee + string-ref context → propose → adversarial-verify → apply):

- **Named coverage 80% → 93.1%** (`FUN_*` 8,899 → 3,413; 46,140 named of 49,553).
- **`engine_api.tsv`** — 46,140 named functions with addr + recovered ABI (cc/ret/params). Directly
  diffable against your per-function VA buckets; disagreements on a name/signature would be worth a
  look either way.
- **`fable_cgsi_slots.h`** — the 921-slot CGSI map with demangled sigs.
- **`fable_types.h`** — 3,807 engine class member-layouts (offset/type/name) from the debug PDB;
  e.g. `CTCQuestCard.ObjectiveName@+0x1c`, `GoldReward@+0x2c`. Complements your GPU-side layouts.

If any of these would be useful in your tree, they're regenerable and we can hand them over.

---

## 4. Corrections still open on our side (from the comparison, unchanged)

- `LEV_WRITER.md` says 256 theme slots; our OpenFable-facing note + your `LEV_FORMAT.md` both say
  **512** — our internal doc is the outlier, reconciling to 512.
- Texture enum `0x20`: we have it write-validated as **DXT3** (not "DXT5-class"); worth your
  correction if you carry the loose grouping.
- Skinned bone-palette cap: you observe **18** (54 regs ÷ 3); our authoring cap is a conservative
  16 — we can raise it, your number is the HW-derived truth.

---

*Everything here is decompile- or byte-validated; addresses are retail `Fable.exe` VAs (ImageBase
0x400000) unless noted as FableWin editor (0x02xxxxxx). The terrain write-side is the headline —
it's the concrete resolution of the one place our models diverged.*
