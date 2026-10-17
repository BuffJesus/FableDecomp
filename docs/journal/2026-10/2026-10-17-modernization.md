# Modernization lane — 2026-10-17

User: "Proceed with the 64 bit/modernization until asked to stop". This continues
[2026-10-16-modernization-prep](2026-10-16-modernization-prep.md). The recorder round trip is still
blocked on the shared install's world-load crash, so this session took work that needs no game launch.
Nothing committed.

## Modern-compiler census (fork steps 1-2 readiness)

New: `tools/modern/modern_compile_census.py` produces [MODERN_COMPILE_CENSUS](../../modern/MODERN_COMPILE_CENSUS.md)
(+ `.tsv` of every non-clean TU) and `X64_LAYOUT_DRIFT.tsv`. It syntax-checks (`cl /Zs`, VS 2022
14.38, `/std:c++17 /permissive /Zc:wchar_t-`) every non-test TU under `rebuild/src/compiled/` as
x86 and as x64. A full run takes about 15 minutes at 12 jobs, and `--report` regenerates the report
from `work/modern_census/diagnostics.json`.

| | x86 (step 1) | x64 (step 2) |
|---|---:|---:|
| clean | 20,199 (99.4%) | 18,079 (88.9%) |
| pointer-truncation warnings only | 0 | 1,594 (7.8%) |
| layout assert only | 115 | 1 |
| code error | 13 | 144 |
| inline asm (x64 has none) | - | 509 (2.5%) |

Findings:

- **VC7.1 standard-library layouts break at step 1, not only at x64.** `std::bitset<274>` is
  36 bytes in VC7.1 (32-bit words) and 40 in current MSVC (64-bit words). The `fable_world.h` assert
  on it fails in 115 TUs under a modern x86 compiler. Any PDB layout that embeds a VC7.1 container
  holds only under VC7.1, so the fork's own types (MODERNIZATION "PDB layouts are 32-bit") are a
  step-1 job.
- **126 of the 144 x64 errors are one spelling.** `operator new(unsigned int)` in
  `fable_memory_allocator.h` (84 TUs), `fable_file_installer.h` (18), `fable_boot.h` (15) and
  `fable_user_profile.h` (9) must take `size_t`. Under VC7.1 `size_t` is `unsigned int`, so the
  mangling and the bytes are unchanged. **Not applied:** all four headers have uncommitted edits
  from the concurrent native-recreation sessions. Do it when those land, then re-run the parity
  gates of the affected TUs.
- The 13 x86 code errors are listed in the report. Two are VC7.1 for-loop scope (`i` used after
  its `for`, in the `CEngineSubPrimitiveRenderer` radix sort), and most of the rest are sources whose
  methods are missing from their class header (`C2039`). One x64-only error is a local
  `typedef unsigned int size_t` (C2371). Two x64 errors are `__asm` in `fable_fpu.h` (C4235).
- x64 pointer-width diagnostics: 7,958 C4312 / 3,248 C4311+C4302 / 95 C4267. **Correction (by
  origin, added to the report later): 14,193 of the 14,549 come from 9 lines of `fable_string.h`**,
  repeated in the 1,577 TUs that include it. Those lines are the `CWideStringData`
  `unknown04`/`unknown08` property shim, which exposes `_M_finish` and `_M_end_of_storage` as
  `fable_u32` for code landed before `Data` was modelled, plus three readers that cast them back to
  `const wchar_t*`. Retyping that shim to a pointer-width alias (still `unsigned int` under VC7.1,
  so no byte change) removes them. Not applied: the header has uncommitted peer edits. What is left
  is 311 distinct sites in 118 TUs' own sources (the `unsigned long` cursor idiom of byte-matched
  code).
- `--layouts`: of the 1,499 classes with a size assert, 1,045 change size on x64 (vtables and
  pointer members). Persistence does not depend on this (member-wise `Transfer<T>`, PERSISTED_TYPES).

Method gotcha (GOTCHAS): MSVC looks up `#include "rebuild_abi.h"` in the including header's own
directory first, so an overlay `/I` directory is ignored. The tool compiles against a patched
*copy* of `rebuild/include` instead (pointer check and size asserts off for x64; live
`static_assert` for `--layouts`). Hand-written `typedef char X[cond ? 1 : -1]` asserts are rewritten
in the copy as well.

## Mouse look: stepped per world frame (FRAME_TIMING open item cleared)

Static, capstone over retail. Details in [FRAME_TIMING](../../modern/FRAME_TIMING.md).

- `CMouseDX::Update` 0xAB58E0 drains the DirectInput buffer, sums the deltas, optionally averages
  them with the previous call (filtering), queues one event per call, and re-centres the cursor.
- `CInputManager::Update` 0x9F57A0 drives mouse, keyboard and pads. In-game it is called only by
  `CMainGameComponent::GetInputs` 0x416E78, which `CMainGameComponent::Update` reaches only on a world
  frame (`GetGameEventPackageSet` → `IsTimeForServerUpdate`).
- `CInputProcessCameraLookAround::ProcessInput` 0x68D790 *sets* the look vector (0x697B10). With
  one event per world frame that loses nothing. **Mouse sensitivity does not depend on render rate.**
  The live per-render-frame calls came from the frontend.
- `DIPROP_BUFFERSIZE` = 256 records (set in `CMouseDX::SetExclusiveMode` 0xAB5710). It is drained
  once per world frame, so **4000/8000 Hz mice overflow it and lose look motion** (about 530/1,070
  records per 66.7 ms). 1000 Hz fits (about 133). Fork input service: an uncapped raw-input
  accumulator, one summed delta per world frame. Noted under FABLE_TLC_REPORTED_BUGS "Camera
  micro-stutter at high mouse polling rates" (untested in-game).
- Frontend: `CNewFrontendGameComponent::Run` 0x42EC7C runs Input/Update/Draw per render frame,
  but its key and stick repeat uses a real-time `CStopWatch` (0.333 s initial, then 0.085 s). So
  the main menu is not the "menu input repeats too rapidly at high frame rates" report.
  Annotated in FABLE_TLC_REPORTED_BUGS.

- `GFGetTime` render-side sweep (185 sites / 101 functions): every site is a real-time threshold,
  dt-scaled accumulation, profiling, a seconds-to-world-frames conversion, or a GUI container that
  passes real `dt` to its child widgets at render rate (10 functions). None feeds a render-frame
  count into state. Table in FRAME_TIMING. Left: the child widgets' `dt` bodies.
- Candidate retail mouse patch (not applied): `DIPROP_BUFFERSIZE` immediates at VA 0xAB57BA /
  0xAB5871, 256 → 8192. The read loop 0xAB4910 already refills until the device is empty.

## Save container codec: retail saves round-trip byte for byte (step 2 piece)

New module `rebuild/modern/save/` (C++23, `/W4 /permissive-`, no pointer-width dependence):
`SaveContainerCodec::decode/encode` and `ccrc`, the `fable_save_tests` unit tests and the
`fable_save_check` corpus tool. See its README.

- The decoder enforces the retail gates exactly (`VerifySignature` 0x409730: magic, bounds,
  seed-0 CRC, 0x4B000 slab, zero padding), then the chunk framing.
- **Correction to SAVE.md §2.4.** Retail is zlib 1.2.1. Level 9 / memLevel 8 / default strategy
  in upstream 1.2.1 *and* in classic zlib 1.3.1 reproduces retail's compressed bytes. The 2026-07
  "not achievable" result came from Python's zlib, which here is zlib-ng. Parameter sweep: chunk0
  matches levels 7-9 at every tested memLevel except 1, and chunk1 pins level 9 with memLevel 8 (or 9).
- Container layer alone, over the corpus (409 `FableSave!` files in the local save folder):
  404 exact whole-file rewrites, 5 section-only (all written by our Python tools), 0 rejected.
  The deeper layers below bring the final verdict to 402 exact. Identical on the x86 and x64
  builds and with the pinned zlib 1.3.1 download (`FetchContent`, SHA-256 checked).
- Layer 2 started: `PersistStream` (tagged binary `CPersistContext` primitives with pinned widths
  and retail-style tag checks) and `SaveHeader` (the 23 HEADER fields). **All 409 HEADER sections
  decode to typed fields and re-encode byte-identically**, x86 and x64.
- Body layer: `SaveBody` (the game clock `Time`/`TimeStep`/`DayCount` from 0x6BBB30, then named
  sections via StartSection/EndSection 0x9BAB60/0x9BABF0) and the typed `PlayerSection`. Two
  writer shapes, both present in the corpus: full (291) and minimal `.qs.hs` (113, ENTITIES only).
  **All 402 game-written saves pass every layer byte-exactly.** The five failures are all Python
  editor backups: three with broken section tiling, and two where ENTITIES is one byte short so the
  next section reads `LAYER`. The checker now flags both.
- ENTITIES layer: the exact SAVED_ENTITIES grammar (the head's slot count, `u64 0` empty slots,
  `29+clen` records, a 20-byte index table) replaces SAVE.md's anchor walk, which missed 7 of 32
  cells in Manual-Save1, and its per-record marker. Cells are zlib **level 1**. All 1,921 non-empty
  cells, and every game-written save end to end, re-encode byte-exactly. SAVE.md §3.6 is updated.
- `Profile.bin`: `Profile` (FablePro container, ordered `Name value;` entries, the save
  registry, edit + re-sign). All 153 local profiles re-encode byte-exactly.
- Portability: the same tree builds warning-free with MSVC x86 and x64 (`/W4 /permissive-`)
  and with GCC 15 (WinLibs, `-Wall -Wextra -Wconversion -Wpedantic`), and the corpus result is
  identical on all three. New CI `.github/workflows/modern-tree.yml` runs Ubuntu 24.04 GCC with
  system zlib and Windows MSVC x64 with the pinned zlib on `rebuild/modern/**` changes. It has not
  run yet because nothing is pushed. WSL is not installed on this machine.
- The modern CMake project now enables C (for zlib). Build dirs: `work/modern_build_{x86,x64,x64_fetch,gcc}`.

## Save text layers: NPC names and LZO (QUESTS/FACTIONS)

- **SAVED_NPC_NAMES** is `CThingManager::NPCNameMap`, `map<UID, pair<CCharString, CWideString>>`
  (PDB member; writer 0x524440). It holds u32 count, then u64 uid, counted group string and
  NUL-terminated UTF-16 name per entry, in ascending UID order. An empty group is written as
  `UNATTACHED` and read back as empty (0x525A62). The section is empty in exactly the 113 minimal
  `.qs.hs` saves (context flag +0x102). All 403 game-written saves parse, and the C++ type
  `SavedNpcNames` re-encodes them exactly.
- **LZO is stock.** Blob writer 0x996610 calls `lzo1x_999_compress` (0xC08130: the
  `lzo1x_999_compress_internal` level-8 argument set, 0x70000 bytes of work memory) on all but the last 3
  bytes and verifies with `lzo1x_decompress`. Upstream **LZO 2.10** (tarball SHA-256 c0f89294…,
  only `lzo1x_9x.c` + `lzo_init.c`) reproduces 8,698/8,700 corpus blobs. The two misses are
  `flipquest` store-mode streams.
- Framing correction: `clen` is a signed i16 in the reader (0x995FE0). -1 means a u32 follows,
  and ≤ 0 means stored raw. The writer stores `ulen ≤ 3` and incompressible text that way.
  `tools/parse_save_entities.py` now reads both forms and writes -1 + u32 above 0x7FFF.
- **Licensing decision** (the user left it to me, wanting source available and modifiable): the
  repo stays MIT. `src/Lzo.cpp` has our own LZO1X decoder. Without LZO the writer emits
  retail's stored form, a shape retail itself writes. `-DFABLE_SAVE_WITH_LZO=ON` fetches LZO
  2.10 (hash-pinned) for byte-exact output; such a binary is a GPL combined work. No LZO code
  is committed. The Linux CI job builds with LZO, Windows without.
- Result: MSVC x86 (no LZO), MSVC x64 (with and without), GCC 15 (with LZO, 0 warnings). Every
  build gives 402 exact / 2 sections / 5 malformed backups / 153 profiles. With LZO, "exact"
  now covers the text blobs too.

- **REGIONS** typed too (`Regions.hpp`). CRegion::Name and the three bools AppearOnWorldMap,
  CreatureGenerationEnabled and SoundThemesEnabled are each `Transfer("")` (tag 0). Element 0
  is skipped. All 290 sections re-encode exactly, so every body section except the entity
  streams inside cells is now typed.

- **ENTITIES head and dead-entity table named.** The 20-byte prefix is format version 2 plus
  `CThingManager::CurrentUniqueID` (writer 0x524390). The PDB offset is the evidence, and the
  value exceeds the hero UID in all 290 saves. The SAVED_ENTITIES "index" is
  `PersistedDeadEntities` (`CPersistedDeadEntity{UID, GamePersistent, SectionNameCRC}`; only
  game-persistent ones are written, by 0x521620). SectionNameCRC is seed 0 over the quest
  script name: 7/8 corpus values recovered from FinalAlbion.qst, and 0x321ACEE7 is still unknown.
  Both are typed in `Entities.hpp` (`EntitiesHead`, `PersistedDeadEntity`), and the corpus is
  unchanged at 402 exact.

- **Every save field tag is named.** Hashing all 38,672 string literals in Fable.exe (seed-0
  CRC, plus `<name><i>` index forms) names the QUESTS trailer: `NumAcceptedBoasts`, then
  `BoastScriptName<i>`/`BoastId<i>`/`BoastFailed<i>`/`BoastCompleted<i>`. The July notes
  record two failed wordlist brutes; the index suffix is what they lacked. It also names the
  FACTIONS tags (`NumberOfFactions`, `Faction<i>`) and the last dead-entity section CRC
  (`"NULL"`). QuestsSection and FactionsSection are fully typed and still exact on all 290
  saves.

## Render-rate audit finished (FRAME_TIMING)

Vtables recovered from retail RTTI (TypeDescriptor → COL → vtable; scratch `vtables.py`).
Every CThing `RenderUpdate` slot is an empty stub, except the sound emitter, which follows its
owner. Environment and camera interpolate between two world-frame states. All 50 NUISystem
`Update(dt)` bodies integrate real dt (automated scan plus spot reads). The render-frame counter
+0x16138 is never read. **No code advances game state per render frame.** The high-fps reports
in FABLE_TLC_REPORTED_BUGS are annotated: what remains is interpolation jitter or world-tick
scheduling, both of which need a live sample. Gotcha: capstone's linear sweep stops at the first
undecodable byte, so set `skipdata` for region scans.

## Bank archive codec (`rebuild/modern/bank/`, step 2 "bank headers")

C++23 BIGB codec from BIG.md: header, footer directory, TOCs with the stats header, raw
name/dep bytes, info, and payloads as views into the shared file buffer (no copy of the 533 MB
bank). The writer uses §9.1 minimal repack (original address order, boundary = largest power of
two ≤ max file Align dividing the original start). `fable_bank_check --edit` over the install:
**17/17 byte-identical** (8 retail plus 9 copies/backups). A grow edit in every sub-bank leaves
other entries unchanged and keeps alignment, and grow-then-revert is exact in the 16 aligned
files. The live `textures.big` (texture-upscale lane) is packed unaligned (6,321/6,325
payloads), so revert is only checked to decode there. MSVC x64, GCC 15 (0 warnings), and MSVC
x86 on the banks up to 250 MB. Read-only use of the install.

- **Overlap with FableForge `forgecore`** (MIT, same owner). It already covers lev/tng/wld/wad/stb,
  defs, lut and qst in C++, and uses the same LZO licence rule (clean-room LZO1X, optional
  liblzo2 for 999). Its save reader is read-only and its BIG writer copies payloads, so the fork's
  `save/` and `bank/` modules add to it rather than duplicate it. I stopped writing level/world
  serializers here; the reuse recommendation is in MODERNIZATION "Open questions".

- **The x64 inline-asm blocker is exactly the de-bake backlog.** Of the 509 TUs with `__asm`,
  503 are `__declspec(naked)` whole-function bakes and 6 are C++ with an `__asm` block. 494 are in
  `rebuild/backlog/asm_bake_list.tsv`, and the other 15 (00/47, 00/59, 00/66, 00/85 and 00/99
  shards, July to August) are not landed manifest rows: orphan sources the census sweeps, which
  coverage does not count. No genuine reconstruction needs an asm port for x64. De-baking is the
  x64 work.

- **Step-1 layouts: the PDB engine headers survive a modern x86 compiler.** The census's
  `--layouts` now takes `--arch x86`. All 1,505 size-asserted classes in the 1,044
  `rebuild/include/engine/*.h` headers keep their size under VS 2022 x86, with 0 other errors.
  The generated headers model library members as opaque byte arrays of their PDB size
  (`unsigned char Maps[0x10]; // vector<long>`), so the current library's layouts cannot move
  them. The step-1 layout break is only the hand-written shared headers (`std::bitset<274>` in
  `fable_world.h`, 115 TUs). The report also gained an "x64 readiness by fix" summary.

- `tools/modern/mouse_buffer_patch.py` packages the high-polling mouse fix. It is SHA-gated to the
  Steam exe, verifies both sites, writes to a separate output path only, and changes 2 bytes
  (256 → 8,192 records). Tested on a scratch copy; nothing in the install was touched. In-game
  test pending (needs an unshared install and a 4-8 kHz mouse).

## Open / next

- Apply the `size_t` fix to the four headers once peer edits land (126 x64 TUs).
- Fork step 1 is now measurable: the 13 code errors plus the bitset layout are the whole x86
  modern-compiler gap for the landed code.
- ~~Frontend input driver 0x129D628~~: it is slot 2 of `CInputManagerDX`'s vtable, called by the frontend `Input` at 0x42E434 once per render frame (FRAME_TIMING).
- Save layers still opaque: the entity/component streams inside cells (the reconstructed
  engine's Transfer code, not a new model), SAVED_NPC_NAMES, and the QUESTS/REGIONS/FACTIONS text
  (LZ-compressed). `parse_save_entities.py` should switch to the exact slot walk.
  (Since done: NPC names, REGIONS and the QUESTS/FACTIONS text are typed. Left: the QUESTS
  trailer's field names and the streams inside cells.)
