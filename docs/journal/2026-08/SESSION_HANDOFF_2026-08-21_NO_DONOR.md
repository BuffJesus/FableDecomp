# ForgeTest64 donor-free terrain handoff — 2026-08-21 shutdown

> **SUPERSEDED IN PART (2026-08-22).** The "Runtime state" and "Exact
> continuation order" sections below stop at stage7. Fourteen further runtime
> packages (stage8-stage21) were built and deployed on 2026-08-22. For the real
> frontier, the stage ledger, the stage20 reference result, and the crash
> diagnosis, read **`docs/journal/2026-09/ACTIVE_TASK_LIST.md` -> "Runtime stage ledger —
> stage7 through stage21"** first. The authored artifacts, hashes, and recovered
> binary layouts in this document remain accurate.

## Objective

Create ForgeTest64 as a genuinely authored 64x64 map using
`D:\Downloads\penis_terrain_pack.zip`, with its own heightfield, inline custom
textures, empty TNG, and dedicated single-map region. The final package must not
clone a retail LEV, TNG, STB chunk, foreground topology, scenery, or texture
palette.

## Runtime state

- The last runtime test used the older clone-derived diagnostic package. It
  teleported successfully but rendered donor ruins/void and is not acceptable.
- No donor-free chunk has been deployed yet.
- Do not promote `stage3`, `stage4`, or `stage5` as the final custom map.
- The static `About (ForgeFSE Active)` text edit is currently visible. It is
  unrelated to terrain and can remain during testing.

## Authored inputs and artifacts

- Source ZIP SHA-256:
  `e902e642e4c61bd36e811844ed4c31a064e1125a95c83a5df487a56d1a45eeef`
- Final authored LEV:
  `work/no_donor_terrain_pack/ForgeTest64_final.lev`
  SHA-256 `efcb8b52da97d5a146759f064b961ad9f105fb40fc536198bfebc9e4e2abbb46`
- Empty authored TNG:
  `work/no_donor_terrain_pack/ForgeTest64.tng` (12 bytes)
- Sixteen donor-free leaf frames:
  `work/no_donor_terrain_pack/ForgeTest64_background.frames.bin`
  SHA-256 `953dd156e7047f7885bdefcd1513994d70ff9e01b8b92d989e5101d5e48ff8b1`
- Foreground patch headers:
  `work/no_donor_terrain_pack/ForgeTest64_background.quaddir.bin`
  SHA-256 `6fb719f335261cb3f503741dc5c5f97c2a4d3c18618c22848c1985a14155df3b`
- Nine native inline DXT1 streams are under
  `work/no_donor_terrain_pack/inline_textures/`.

The imported terrain is a 65x65 vertex grid covering 64x64 cells at world box
`(3328,2304)..(3392,2368)`. Heights are `72.000..83.962`; center height is
`81.949`. Runtime teleport Z must be derived from that center plus clearance,
not the old hard-coded value 74.

## Completed implementation

In `D:\Code\FableForge`:

- `lev import-heightmap` and `lev author-surface` create the custom LEV.
- The supplied texture images compile to 128x128 DXT1, six-mip native inline
  streams with the recovered 19-byte `CTexture::LoadFromDataStream` header.
- Donor-free background leaf generation emits sixteen authored 16x16 patches,
  LZO-framed and page-aligned.
- Global texture palette, background tree header, empty local-detail palette,
  local-detail 0x2C header, 12-byte local-detail root descriptor, deterministic
  section planner, InfoBlock constructor, and minimal generated foreground
  writers exist in `forge::stbbake`.
- Background tree headers now have a matching parser and byte-exact
  serialize/parse/serialize test.

## Critical retail-oracle correction

The array previously called the background quadtree directory is actually the
foreground `CEngineLandscapePatch::SaveHeader` array.

- Base is `0x800`, not `0x7FC`.
- Record size is `0x24`.
- Layout is `[u32 frameOffset][u32 frameSpan][6*f32 AABB][u32 flags]`.
- All sixteen live retail records use flags `0x00C92E01`.
- The following terminator record is entirely zero.

The parser, generator, retarget rewiring, XY translation, Z-bound mutation,
tests, and generated artifact were corrected to this layout. The regenerated
artifact has 16/16 decodable patches, every frame offset is 2048-byte aligned,
all live flags match retail, and the terminator is zero.

## Verification at shutdown

From `D:\Code\FableForge`:

```powershell
cmake --build build --target forge forge_tests
ctest --test-dir build --output-on-failure -R '^forge_tests$'
```

Result: `forge_tests` passed, 100%, zero failures.

Regeneration command:

```powershell
D:\Code\FableForge\build\forge.exe stb create-background `
  work\no_donor_terrain_pack\ForgeTest64_final.lev `
  work\no_donor_terrain_pack\inline_textures\SPOOKYGROUND_diffuse.bin `
  3328 2304 work\no_donor_terrain_pack\ForgeTest64_background
```

Validation command:

```powershell
D:\Code\FableForge\build\forge.exe stb patchinfo `
  work\no_donor_terrain_pack\ForgeTest64_background.frames.bin
```

Result: 16 frames / 16 mesh patches, bounds correct, heights
`72.000..83.962`.

## Exact continuation order

1. Finish the real background root/parent tree and file-block wiring. Do not
   confuse it with the foreground header array at `0x800`.
2. Assemble canonical empty local detail: reserved 12-byte root descriptor,
   aligned tree block, typed 0x2C root header, zero-object contents, and empty
   palette. Prove every offset/span closes.
3. Add `forge stb create-terrain` to assemble an entirely new chunk from the
   authored LEV and nine inline textures without opening a donor chunk.
4. Generate the common-header InfoBlock and append the new STB entry. Verify
   deterministic bytes, page alignment, pointer closure, monotonic STB offsets,
   codec round-trips, in-box AABBs, and absence of donor names/data.
5. Add/install a dedicated ForgeTest region whose only contained map is slot
   399. Remove region-55 attachment and dual ownership.
6. Derive teleport Z from the authored center height, deploy through the
   rollback harness, launch FSE, monitor logs automatically, then have the user
   drive only the in-game trigger if necessary.

Native write-order evidence is in
`ghidra_out/decomp_background_fileblocks.txt` and
`ghidra_out/terrain_bake_decomp.txt`. `SaveRootFileBlockAndChildren` writes a
placeholder root header, recursively emits child/file-block data, then seeks
back and patches the wired header. Local-detail generation reserves 12 bytes,
writes/aligned-patches the tree, writes the root header plus palette, and then
backfills `[headerPos, headerSpan, present]`.

## Dirty-worktree warning

The FableForge worktree already contains substantial user/session work. Relevant
modified files include:

- `apps/forge/main.cpp`
- `libs/forgecore/include/forge/stbbake.hpp`
- `libs/forgecore/src/stbbake.cpp`
- `tests/test_formats.cpp`

Preserve all existing changes; do not reset or check out these files.

## 2026-08-22 continuation — canonical empty local detail

Recovered the remaining native empty-root write contract from FableWin
`CQuadTreeElement<CLocalDetailCacheMap>::SaveTree` (`0x02E3E890`),
`SaveHeader` (`0x02E3E680`), and `SaveFileBlock` (`0x02E3EB20`). An empty root
file block is exactly five zero `s32` values: zero object-cache groups followed
by four absent-child markers (20 bytes total). The later root header stores that
aligned block's absolute position, size 20, and offset zero; the descriptor
span is the `0x2C` header plus the five-byte empty global-name palette (`0x31`).

Implemented `forge::stbbake::buildEmptyLocalDetailSection` in FableForge. It
emits the reserved descriptor, alignment padding, 20-byte root block, wired
header, and empty palette from scratch, with signed-offset and alignment gates.
The regression test proves descriptor/header pointer closure at a deliberately
unaligned nonzero section offset. `forge_tests` passes (1/1, zero failures).

Next: integrate this completed local-detail section with the still-outstanding
real background root/parent tree in the pure `stb create-terrain` chunk layout.

## 2026-08-22 background-tree continuation

The native `CLandscapeBackgroundTreeNode::SaveHeader` body at FableWin
`0x02DEABE0` is now recovered directly. It confirms the existing 47-byte base
and 13-byte inclusive LOD records, plus two hard gates now reflected in code:
`FirstBand >= 1`, and every live LOD `FileBlockPos` is bank-page aligned. The
old `0x800` directory comments were also corrected: those fixed 0x24 records
are foreground `CEngineLandscapePatch::SaveHeader` values, not background-tree
headers.

Retail Darkwood_3 was used only as a structural oracle. Its root header is at
chunk offset `0x22000`, has local bounds `0,0..64,64`, bands `1,5,7`, and owns
a root file block at `0x22000` of span `0x9C4`; its logical child-header area is
at relative offset `0x932`. This confirms the root header is a page-aligned
placeholder patched after descendant headers/file blocks are laid out, matching
`SaveRootFileBlockAndChildren`. No donor bytes are being carried into output.

The new `forge stb backgroundtreeinfo <chunk> <root-offset>` forensic command
walks this structure with cycle, range, duplicate-header, and exact child-
partition checks. Against Darkwood_3 at `0x22000` it closes the complete retail
tree: 33 nodes, 17 leaves, and 73 LOD records. The extra leaf is a terrain-
dependent 16x16 -> two 16x8 split and is not copied into ForgeTest.

`buildBackgroundTreeShape64` now creates ForgeTest's deterministic authored
shape from its heightfield: 31 nodes, sixteen 16x16 leaves, alternating X/Y
splits, 71 unwired retail-schedule LOD records, map-local rectangles, and exact
world-space height AABBs. Next work is generating each LOD payload and assigning
its page-aligned file-block tuple before serializing/backpatching the tree.

LOD forensics show retail groups many patch payloads into page-aligned blocks by
band; individual records carry offsets within those blocks. A zero tuple is
valid when `OptimizedBandRemap` reuses another band (the Darkwood root remaps
band 6 to band 5). Forge can therefore author its own grouping and does not need
retail block allocation. As the payload prerequisite,
`buildBackgroundPatchRect` now authors rectangular node patches (including
16x32 and 32x64 parents) with independent X/Y UV scaling and guarded 16-bit
vertex/triangle counts. Its 16x32 encode/decode regression passes.

`layoutBackgroundTree` completes the background control/file-block stage. It
emits the 31-node tree in native postorder, assigns one page-aligned authored
LZO payload per node, uses `OptimizedBandRemap` to reuse that payload for higher
bands, and backpatches every split node to the shared root tree block. A full
serialize/reparse gate validates all 31 nodes and payload tuples. Work has moved
to pure chunk assembly; the 20-byte `CEngineLandscapeMap` root descriptor now
has an explicit serializer for palette, foreground-header, and background-root
position/span wiring.

Texture-palette RE closed another ambiguity: `CEngineTexturePalette::Save/Load`
has no inline-entry branch. `external=false` writes a count and symbol names;
`external=true` writes only the flag and treats mesh texture values as global
IDs. Inline texture bytes belong to each background patch payload, while
foreground layers must use generated palette symbols or explicit global IDs.

`layoutForegroundFrames` now emits sixteen authored `CLandscapeLayerMesh`
frames with generated `0x24` foreground headers and height-derived AABBs.
`buildTerrainChunk64` assembles the first complete donor-free chunk model:
control page, palette, foreground directory/frames, 31-node background tree,
empty local detail, 20-byte landscape descriptor, and `StaticMapInfoBlock`.
Tests recursively parse both terrain paths, close all pointers, enforce page
alignment/in-box AABBs, and build twice for identical chunk and InfoBlock bytes.
The core builder passes; next is the `stb create-terrain` CLI texture manifest
that maps the nine supplied assets onto foreground symbols/background payloads.

`forge stb create-terrain` is now implemented and exercised on ForgeTest64. It
accepts no donor argument and emitted:

- `ForgeTest64_terrain.chunk.bin`: 876,613 bytes, SHA-256
  `34f4adddced73dd0be3b245ba21805fadd54f0dbbfda5bd5a0af6497395b9d43`
- `ForgeTest64_terrain.info.bin`: 92 bytes, SHA-256
  `fcd3072aba739e6c937664b3561cff4bad2d353d44d7f68e0c613bf8a6bd1e99`

An independent repeat build produced both hashes identically. Structural
inspection reports foreground directory `0x800`, background root `0xE000`,
local-detail descriptor `0xD5D76`, 16 foreground frames with exact round-trip,
31 background mesh patches, and a recursively valid 31-node/16-leaf/71-record
tree. All reported geometry stays inside `(3328,2304)..(3392,2368)` with height
range `72.000..83.962`. The next gate is explicit donor-name/byte absence, then
STB append/common-header integration. Foreground palette symbols and background
inline bytes are deliberately separate because native palette RE proved there
is no inline palette-entry branch.

## 2026-08-22 offline container gate

The packaging bridge is now implemented as
`forge::stbbake::buildTerrainCommonRecord` plus
`forge stb terrainrecord <info.bin> <chunk.bin> <common-record.bin>`.
The original 0x80-byte zero control stream crashed during static-map loading.
Stage8 instead emits the retail-shaped 0xAE-byte landscape control stream,
copies the generated local-detail header/palette into common data, and uses the
external texture-palette flag plus verified retail numeric texture IDs. The
full 10-test CTest suite passes.

- `ForgeTest64_terrain.common.bin`: 128 bytes, SHA-256
  `387b2555babdb96defbd2619e764d296bbdd1e3b9e7c4554d2068f8400251442`.
- `stage6/FinalAlbion_RT.stb`: SHA-256
  `c91e166afd4351e798408268d156761c114d19e4c9e640ab0493d67c03d87522`.
- The stage reopens as 425 entries / 399 static maps. ForgeTest64 is id 426,
  876,613 bytes at page-aligned offset 598,276,096; extraction reproduces the
  authored chunk SHA-256 exactly.
- The rebased record closes at common offset 293,464 with pointers
  `293428/293449/293464` and nested ends `293448/293461`.
- A normalized streaming comparison of the complete pristine prefix, excluding
  only header bytes `0x14..0x1F`, has SHA-256
  `4036d4d9d241371419b83d218859501db5795325525617b9c4a8e2faf929be8d`
  on both source and stage.
- Case-insensitive binary scans find no known donor map names in the generated
  chunk or common record. Nothing was copied to the live installation.

Next: assemble slot 399 and the dedicated ForgeTest64-only region around this
staged STB, validate WLD/BWD/WAD ownership, then use the rollback harness for
the first runtime smoke.

## 2026-08-22 dedicated-region stage

`work/no_donor_terrain_pack/stage7_region` was assembled without the donor
installer. Starting from pristine WLD/BWD/WAD, `forge world add-level` created
map slot 399 at `(3328,2304)..(3392,2368)` and dedicated region 142
`ForgeTest64`; the authored LEV and empty TNG are loose overrides and the
donor-free stage6 STB supplies the static map. The retail WAD is unchanged.

`forge world inspect` proves slot 399 has exactly one owner and one visibility
source, both `ForgeTest64`; region 55 has no claim. `forge validate` is CLEAN:
399 maps, 142 regions, 796 WAD entries, 399 static maps, zero missing LEV/TNG/
STB records, zero off-grid origins, zero unresolved region references, zero
BWD coherence issues, and zero STB bounds mismatches. Stage hashes:

- BWD `07b7c443eb23c5575a4f2bafc70e2a66d689044592540a1784b203f9603a76a0`
- WLD `0df49280adcba1c0de4a172371c4cb0ab8ba1f4e51d9ec729c6af8984bcf4610`
- WAD `0941eb84952ea30824be69e08b2193dead268a2de57bad023b77ef0b975af3e1`
- STB `c91e166afd4351e798408268d156761c114d19e4c9e640ab0493d67c03d87522`
- LEV `efcb8b52da97d5a146759f064b961ad9f105fb40fc536198bfebc9e4e2abbb46`
- TNG `1ae77ea3bec5a507ce1f9ff0e9b8eff66cdd28b7baacfc9aa634a49d5c02e4d6`

The remaining runtime risk is now isolated to dedicated-region membership.
Correction: the FSE probe measured a 142-entry runtime region vector, so the
earlier hard 141-region-cap theory is refuted; the appended region does load.
A historical package still returned zero from `GetRegionNumberMapIsIn(399)`,
so stage7 must prove that its freshly generated region owns slot 399 in memory.
Attaching to region 55 would violate the final single-owner requirement and is
not an acceptable workaround. The exact operator gate is recorded in
`work/no_donor_terrain_pack/STAGE7_RUN_SHEET.md`; the deployable stage contains
only its six intended `data/Levels` files.

A one-shot live candidate is staged separately at
`work/no_donor_terrain_pack/stage7_runtime_package`. It combines those six
terrain files with the current probe-enabled `FableScriptExtender.dll` and a
terrain-only `FSE_Master.lua`. The script was corrected from the historical
package to require map `ForgeTest64`, dedicated arrival region `ForgeTest64`,
bounds `(3328,2304)..(3392,2368)`, expected probe result
`region_vector_size=142 / owner=142`, and teleport Z `82.949` (authored center
height plus 1.0). `STAGE7_RUNTIME_MANIFEST.tsv` verifies all 9 files byte-for-
byte and Forge validation remains CLEAN. The ninth file is the authored
`FSE/Master/Entities/ForgeNPC.lua`, closing the master script's entity-binding
dependency. `STAGE7_RUN_SHEET.md` is the operator procedure. Read-only live
preflight confirms Fable is closed, `FinalAlbion.qst` enables `FSE_Master`, the
launcher is `FSE_Launcher.exe`, and the installed DLL already matches the probe
DLL. The install currently contains the older diagnostic level containers;
the deploy harness will snapshot all of them before replacement. The package
has not been deployed.

## 2026-08-22 first donor-free live attempt

Stage7 was deployed once through the harness, passed installed validation and
9/9 hash verification, and launched under FSE. During static-map enumeration
the engine successfully reached the new entry and logged
`opening 'Data\Levels\FinalAlbion\ForgeTest64.lev'`, then immediately crashed
before `FSE_Master Main()` or any region probe. Application Error 1000:
`0xC0000005`, `Fable.exe+0x0059EC13` (VA `0x0099EC13`), report id
`60e6867d-f5fa-40fd-a8c2-2b14d0512744`. The faulting instruction is
`cmp byte ptr [eax],0` in the `CCharString(char const*,long)` construction path,
pointing the next investigation at generated landscape control/palette string
loading. Evidence is under
`work/no_donor_terrain_pack/runtime_evidence/20260822_0903_crash/`.

Backup set `20260822_090312` was immediately restored and hash-verified: eight
files restored, the newly added LEV removed, and the prior installation is back
in place. Do not redeploy unchanged stage7.

## 2026-08-22 stage8 landscape-control correction

Retail common records proved the landscape tail at `+0x5C` is a live control
stream, not zero padding: it carries the foreground directory, background root
and span, followed by a present local-detail descriptor whose target header and
palette are stored in common data. Retail chunks also begin with the external
texture-palette flag. The stage7 generator violated both contracts, explaining
the string-constructor access violation before `FSE_Master Main()`.

`buildTerrainCommonRecord(info, chunk)` now emits the 0xAE-byte retail-shaped
control stream and copies the generated empty local-detail header/palette.
`create-terrain` accepts numeric global texture IDs and stage8 uses the retail-
verified tuple `(4185,4185,4304)` with a one-byte external palette. All 16
foreground frames decode cleanly. Extraction from the appended STB reproduces
the generated 876,613-byte chunk exactly, SHA-256
`BD6E7095F215FA884846341708F2478A74EC55C93859CD54A956422B61607FAA`.

The isolated candidate is
`work/no_donor_terrain_pack/stage8_runtime_package`; its nine-file manifest is
`STAGE8_RUNTIME_MANIFEST.tsv` and verifies 9/9. `forge validate` is CLEAN and
all 10 CTest targets pass. Its STB SHA-256 is
`B2DAEFE50720A171D1351D2D8CDCE5B71BE189A61E3E23A1185DCB6297EE05E5`.
The live installation remains rolled back. The next action is a rollback-safe
stage8 smoke; never redeploy stage7.

## 2026-08-22 stage11/12 transition isolation

Runtime clamps the searchable region vector before appended region 142, so
ForgeTest64 was moved to a single in-range owner, region 95. Stage11 proved
`GetRegionNumberMapIsIn(399) == 95` but crashed in synchronous `LoadRegion(95)`.
Stage12 replaced that call with retail `EntityTeleportToPosition`; the call
returned successfully, then destination activation crashed before the current
map changed or any ForgeTest render probe fired. Evidence is under
`runtime_evidence/20260822_stage11_host95_crash/` and
`runtime_evidence/20260822_stage12_post_teleport_crash/`. Stage12 was rolled
back exactly with backup set `20260822_100156`.

The next binary defect is now isolated to generated static-map activation.
Retail background-tree comparison exposes a concrete contract violation:
generated nodes advertise LOD band ranges but most advertised higher-band
file-block descriptors are `(0,0,0)`. Retail supplies real shared block offsets,
spans, and per-node offsets for every advertised band (except intentionally
absent/remapped cases such as retail root band 6). Fix background LOD block
packing/backpatching before any further live deployment.
