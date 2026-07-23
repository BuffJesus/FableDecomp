# ForgeTest Native Terrain — Honest Status & Action Plan

**Date:** 2026-07-20 · **Branch:** gui-m4-start · **Scope of this run:** native STB baker retarget + validation harness (Track A), and the Quest:GetGroundHeightAt / ground+1 spawn fix (Track B). No deploy, no live-save edits were performed.

**Bottom line up front:** The hero **spawn** is fixed in code (Track B, pending an in-game smoke test of a non-standard DLL). The **white terrain is NOT fixed** and cannot be fixed by rewriting the chunk bytes — the root cause lives in the engine's map-open bank/theme resolution, not in the STB chunk. The retarget "fix" we can ship today is a **verified no-op** (byte-identical to what is already deployed and already whiting out).

---

## 1. Does ForgeTest render correct terrain now? (white-out fixed?)

**No. The white-out is NOT fixed.** When you next teleport in, expect the same behavior: no visible ForgeTest terrain, and the Oakvale origin level whiting out on teleport.

Why, honestly:

- The staged "retargeted" chunk (`ForgeTest_chunk_retargeted.bin`) is **byte-identical** to the chunk already deployed in `FinalAlbion_RT.stb` (confirmed by `cmp`). Shipping it changes nothing observable.
- The deployed chunk is **structurally clean and self-textured**: S1 roundtrip PASS, 26/26 frames decode byte-exact, the background-LOD quadtree is 4/4 correctly wired at ForgeTest coords (2784–2816), and each patch carries its own **inline DXT1 texture** in-chunk (CPixelFormatInit `e3 04 00 00`). There are **zero** NUL-terminated texture-name strings anywhere in the chunk, so the old "unresolved texture-name palette" theory does **not** apply to this chunk. The chunk bytes are not the fix surface.
- The white-out is an **engine map-open** problem. Two mechanisms are implicated, and I am being explicit that neither is fully proven with staged decomp yet:
  1. **Bind-by-name bank resolution.** `OpenRetailStaticMap` binds resources by map *name* (name→offset map), never via InfoBlock `BankFileIndex` (so `BankFileIndex=426` is a confirmed red herring). ForgeTest's graphic/scenery bank resources were cloned from donor `Darkwood9_Leadout_01` and only exist under the donor's name, so the by-name resolve for `ForgeTest.lev` misses and appears to mutate **shared** bank state — matching the Oakvale-whites-out tell. *Caveat: the precise `CreateNewBankMap` / `CBankFile::FindIndexBySymbol` corruption path is asserted from reasoning, not yet staged as a decomp log.*
  2. **Theme-by-index resolution (newly flagged, un-modeled by the harness).** `BuildThemes@0x00b687d0` resolves terrain themes **by index** through the global `CDefinitionManager` (`GetDef<CSoundThemeDef>('ENGINE_THEME', idx)`, `DAT_01436e98`). The donor's theme *indices* may be invalid/out-of-range in ForgeTest's region context, which independently could whiten a shared global theme table and take Oakvale down with it. No validation check covers this yet.

**Verdict: white terrain unresolved. Do not present the retargeted chunk as a render fix.**

---

## 2. Is the hero valid-spawn fixed? (GetGroundHeightAt + ground+1) — is the DLL sound?

**Code: yes. DLL trust: conditional on one in-game smoke test.**

- `Quest:GetGroundHeightAt(x,y)` is implemented, backed by `CWorldMap::GetGroundSizeZAt @0x0063A300`. The address and ABI were **re-derived by direct disassembly of the deployed Fable.exe** (not just the handoff): `__thiscall`, one `C3DVector const&` pointer arg, `ret 4`, reads x/y and ignores z, routes through `IsPosInMap (0x4fc600)` to the CMap **collision/ground** getter `0x81ea50` (distinct from the render-mesh `DrawGetSizeZAt 0x8F7630`). It returns the surface the hero stands on. A PDB-listed alternate address (`0x6E7EC0`) was correctly rejected — that PDB is from a different binary and `0x6E7EC0` is mid-instruction garbage in Fable.exe.
- `GoToMapSlot` now **internally snaps** the spawn Z to `GetGroundHeightAt(x,y)+1`, computed **after** the synchronous `LoadRegion` force-load so the probe hits a resident sub-map. Falls back to the passed z on the `-10000` sentinel. **No FSE_Master.lua change is required** for the snap — every existing `Quest:GoToMapSlot(slot,x,y,z)` call auto-snaps.
- Wiring across all 6 files is coherent; the build links cleanly as a valid x86 PE with all binding names embedded (incl. the new `GetGroundHeightAt`) and no unresolved externals.

**The one real risk — DLL size.** The built DLL is **4,465,664 bytes**, NOT the known-good, in-game-proven **5,209,600 bytes** currently deployed. The 744 KB deficit is entirely in `.text` (~730 KB less code); `.rdata`/`.data` match. This is confirmed to be a **pre-existing toolchain/codegen property** (pristine HEAD also builds ~4.44 MB under VS18/MSVC 14.51), **not** a regression from this change, and symbol/string parity checks show nothing was dropped. But it was produced by a divergent toolchain from the validated 5.2 MB build, so it **cannot be trusted to load/run identically without an in-game smoke test**.

**Verdict: spawn fix is correct in code; the DLL is not proven-safe until you confirm it loads in-game (see deploy steps). Do NOT overwrite the deployed 5.2 MB DLL until that smoke test passes.**

Also unverified (out of scope, needs in-game): whether `GetGroundHeightAt(2800,2576)` post-LoadRegion actually returns a real height (~38–44) rather than the sentinel/pass-through z. If it returns the sentinel, the ForgeTest sub-map isn't in CWorldMap's list at probe time and the snap silently no-ops.

---

## 3. Native emitter status

| Deliverable | Status |
|---|---|
| `forge::stbbake::parseQuadDir` + `retargetChunk` | **Built & validated** — but retarget on ForgeTest is a **verified no-op** (byte-identical output). It validates/verifies a chunk; it does not and cannot cure the white-out. |
| `forge::stbvalidate` harness (L1/L2/L3 + CLI) | **Built & green.** Structural + byte-isolation predictor only. |
| Native `emitChunk` from an authored heightfield | **Partially implemented (TASK #17, 2026-07-21).** The chunk-assembly write path is shipped and byte-exact; the authored-heightfield *mesh emit* is blocked on two RE items — see §3a below. |
| Bank / graphic-bank writer (the actual fix surface) | **NOT owned by forgecore.** This is the real blocker. |

### §3a — `forge::stbbake::emitChunk` (TASK #17) — what shipped, and the precise blocker

**Shipped & tested** (`libs/forgecore/{include/forge,src}/stbbake.{hpp,cpp}`, test `testStbEmit` in `tests/test_formats.cpp`, built green under the MinGW/ninja build):

- `emitChunk(Chunk, EmitOptions) -> EmitResult` and `emitIdentity(bytes)` — the EMIT/ENCODE half that complements the already-proven parse half. It assembles a chunk from the segment model, re-encoding FRAME segments (RawPassthrough or lzo1x_1 Recompress, or authored `FrameEdit` body replacements), re-laying frames end-to-end and **rewiring the background-LOD quadtree directory** (the 4 wired entries) to each frame's new offset/span.
- **Identity round-trip is BYTE-EXACT on real chunks** — verified in-test against both `Darkwood_Filler_08.lev` (retail donor, 18 frames) and `ForgeTest_chunk_retargeted.bin` (132632 B): `parse -> emitChunk(RawPassthrough) -> == input`, and `parse -> emit -> parse` is structurally identical. This is the shippable write path today: forge can now *own* re-emitting a chunk (and rewiring its quadtree) natively, not just diff it.
- `EmitResult` reports `identity`, `framesReencoded`, `quadEntriesRewired`, and — critically — refuses to claim `ok` when it produced a not-load-safe chunk (see blocker).

**BLOCKER — authored-hill mesh emit needs two more RE items.** A size-changing re-encode of a *real* chunk is honestly reported `ok=false` by `emitChunk` (test asserts the "NOT wired" note fires). Two concrete, measured reasons:

1. **Page alignment is load-bearing and not yet modeled.** The retail chunk does **not** pack frames contiguously — it **page-aligns** them (donor frames sit at 4096/12288/20480/28672/38912… i.e. 0x1000-aligned starts, with PAD filling the gaps; 13455 PAD bytes / 3397 HDR bytes in the donor). `emitChunk` currently re-lays frames contiguously, which collapses that alignment; a re-parse of the collapsed layout mis-anchors (`walkFramedBlocks` found 7 frames instead of 18). **Fix:** the emitter must re-establish each frame's page alignment (pad to the next 0x1000 boundary, or preserve the donor's per-frame alignment rule) when a preceding frame's size changes. This is mechanical once the alignment rule is pinned (looks like "each frame starts on a 0x1000 page"), but it was not RE-confirmed as a hard invariant this run.
2. **Foreground/local-detail frames are wired by InfoBlock/subheader *position dwords*, not the quadtree.** Only the 4 background-LOD frames are referenced by the quadtree dir (which `emitChunk` rewires correctly). The other ~14 foreground/local-detail frames are located by the 5 absolute-offset dwords documented in `NATIVE_STB_WRITER_SCOPE.md` §3 (`+0x20`,`+0x24`,`+0x58`,`+0x5C`,`+0x71` — landscape/localdetail subheader + chunk-end + two dword0 pointers) plus the CStaticMapInfoBlock (0x5C) pointers. When one of those frames changes size, those absolute dwords dangle. `emitChunk` deliberately does **not** rebase them (it flags the condition instead), because rebasing requires the InfoBlock/subheader layout model that lives in `NATIVE_STB_WRITER_SCOPE.md` §3–4 and the Python assembler `work/stb_baker/stb_infoblock_baker.py`, not yet ported to forge.

**Consequence for the authored hill:** authoring new terrain heights means re-encoding the foreground/local-detail mesh frames (bigger/different geometry ⇒ different frame sizes), which trips *both* items above. So the authored-heightfield emit needs: (a) the frame-page-alignment rule ported into the re-lay, and (b) the 5 position-dword + InfoBlock rebase ported from `stb_infoblock_baker.py` into `emitChunk` (add an `EmitOptions` hook to accept the rebased dwords, or compute them from the new layout). Both are mechanical ports of already-prototyped Python — **no new engine RE** — but were out of scope for shipping the round-trip-exact emit first, per the task's fallback instruction. The mesh *codec* itself is NOT a blocker: `CRangeCompressor` RAW-flag passthrough + lzo1x framing already emit engine-decodable frame bodies (proven: every donor frame decodes byte-exact and re-encodes losslessly through the `FrameEdit` path in-test).

**Bottom line:** the write-path skeleton (assemble + rewire quadtree + honest load-safety verdict) is native and byte-exact. Authored-hill emit is one focused follow-up (port page-alignment + the 5 position dwords + 0x5C InfoBlock rebase from the existing Python bakers) away — and, as everywhere in this doc, still gated behind the unproven map-open shared-bank white-out, which no emitter can cure.

**Retarget works** (mechanically). **Full emit does not exist yet.** Even a correct native `emitChunk` would reproduce the same white-out on its own, because **bank-symbol registration** (and possibly theme-index residency) is the blocker, not the mesh bytes.

**Harness caveat you must not over-read:** `validateTextureResolve` (L3) degrades to WARN when topology diverges — and donor≠ForgeTest is always the real case here — so `RESULT: OK` means *structurally load-consistent*, **not** *render-safe*. No harness check models the white-out mechanism (bank-symbol resolve or theme-by-index residency).

---

## 4. Deploy steps for the user (game MUST be closed)

> **Nothing in this run needs deploying to fix rendering** — the chunk is byte-identical to what's already installed. The only thing worth deploying is the **new DLL**, and only after a size/smoke gate. Do the DLL smoke test as an isolated step first.

**Pre-flight:** Close Fable completely. `deploy.ps1` refuses to run if `Fable.exe` is live.

### Step A — Smoke-test the new DLL in isolation (REQUIRED before trusting it)
1. **Back up** the current known-good DLL:
   `C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\FableScriptExtender.dll` (5,209,600 bytes) → copy to a safe location.
2. Copy the new DLL over it:
   `D:\Code\ForgeFSE\Release\FableScriptExtender.dll` (**confirm 4,465,664 bytes**) → game dir.
3. Launch via `C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\FSE_Launcher.exe`.
4. **Confirm in the FSE log** it prints `Fable API Pointers Initialized` and that a `Quest:GoToMapSlot(...)` round-trip works.
5. In-game, verify `Quest:GetGroundHeightAt(2800,2576)` returns ~38–44 (a real height), not `-10000`.
   - **If the DLL fails to load or the probe returns the sentinel → restore the 5.2 MB backup and stop.** Do not proceed; rebuild with the toolchain that produced the 5.2 MB DLL, or fix the sub-map-residency timing.

### Step B — Full smoke deploy (only if Step A passed)
The batch deployer stages everything at once with SHA256-backed rollback:
- Script: `D:\Documents\FableTLC\work\smoke_test\deploy.ps1`
- It replaces (and backs up on first run): `FinalAlbion_RT.stb`, the `.big`/`.wld`/`.wad`/`.bwd`/`.lug` set, and **`FSE\Master\FSE_Master.lua`**, and adds `data\Levels\FinalAlbion\ForgeTest.tng`.
- **FSE_Master.lua change:** none is required for the ground+1 snap (it is compiled into `GoToMapSlot`). The staged `FSE_Master.lua` at `...\work\smoke_test\stage\FSE\Master\FSE_Master.lua` is the current teleport driver — deploy it as-is. If you want to *directly* probe height, add a call to the new `Quest:GetGroundHeightAt(x,y)` binding; otherwise no script edit is needed.
- Run: `powershell -ExecutionPolicy Bypass -File D:\Documents\FableTLC\work\smoke_test\deploy.ps1`
- Launch: `FSE_Launcher.exe`. Rollback: `revert.ps1` in the same folder.

> ⚠️ **Set expectations:** after Step B you will get a hero that spawns at ground+1 (walkable *if* the cell is navigable), but the terrain will **still be white / invisible** and Oakvale will **still white out on teleport**. That is the unfixed bank/theme problem, not a deploy error.

---

## 5. If native correct-texture rendering is not achieved — the editor bake is the route

**It is not achieved, and for this chunk the editor bake (UnifiedFable / ego_r `BAKE_FORGETEST_TERRAIN.md`) is the correct route to a rendering ForgeTest.** Why:

- The white-out is created at **map open**, by how the engine binds bank resources **by map name** and resolves terrain themes **by global index**. A hand-relocated donor chunk carries the donor's name-bound resources and theme indices; those don't exist / don't line up under `ForgeTest.lev`, so the resolve misses and corrupts shared state.
- The editor bake is the only path today that produces a chunk **whose bank resources and theme indices are registered under ForgeTest's own name/region** — i.e. it emits the chunk *and* the matching bank/theme registration together, which is exactly the part forgecore does not yet own.
- Forgecore can byte-validate, retarget geometry, and diff against an oracle — but until it owns a **bank/graphic-bank writer** (or an FSE map-open name-alias), it cannot emit the resource registration the renderer needs. So the editor bake is both the near-term unblock and the eventual **exact-topology oracle** that lets us reverse the remaining formats.

---

## 6. The single most important next action

**Own the fix surface, not the symptom: stage the map-open decomp and build the bank/theme registration path.** Concretely, the highest-leverage next step is a small, decisive RE spike that turns the root cause from *asserted* into *proven and actionable*:

> **Headless-decompile `CreateNewBankMap@0x009e0360`, `CBankFile::FindIndexBySymbol`/`AddEntry`, and `BuildThemes@0x00b687d0`, and confirm which one mutates shared bank/theme state when `ForgeTest.lev` fails to resolve.** Then pick the cheapest real fix: either (a) an **FSE map-open name-alias** so `ForgeTest.lev` resolves the donor's already-resident resources (fastest, no new writer), or (b) run the **editor bake** to register ForgeTest's own resources/theme indices (the durable route from §5).

If you want one thing to *test in-game right now* while that RE spike runs: do **Step A** (the DLL smoke test) so the spawn fix is banked and the 4.4 MB-DLL question is answered — that is the only new, testable win this run produced.
