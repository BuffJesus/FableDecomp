# Boot-path de-bake, 2026-09-25

Reconstruction lane (not Lua). Goal: replace the hand-written `__asm`/`_emit` bakes on the
boot → `CGame::Play` path with genuine VC7.1 C++ that matches retail, so the path into the game
loop is real source. Retail Fable.exe was running (another session), so the full bootstrap
was not run; every leaf was checked with the new `tools/decomp_pipeline/gate_boot_leaves.py`,
which runs the same per-leaf parity + fixture gate as `build_bootstrap.ps1`.

## Starting state

- 40 of the bootstrap's 118 verified leaves were asm (`gate_boot_leaves.py --purity`).
- 114/118 passed. Four asm bakes failed their OWN parity gate at HEAD (unchanged since
  a9fa2f7, 2026-07-29): 009c85a0, 009ac4d0, 0098dfd0, 00403b10. So the full bootstrap was
  already red on those.

## CGame::Play 0x412f90: genuine

RELOCATION_MATCH over the full 394-byte body (23 relocs). The manifest's 203-byte length is
wrong; `boot_oracles.tsv` has the right one. Two source facts decided it:

1. The new component and `Run(&component)`'s out-param are one local. The loop's
   `lea eax,[esp+0Ch]` is the same slot as `[esp+8]` once `push ebx` has run.
2. `component->Init()` is written in both front-end arms. The compiler tail-merges it, and
   only then does the second constructor's argument `lea` use `ecx` as retail does. Flags,
   /GS, /Oa and QFE-4035 did not change that register.

Call targets were checked: three `CWideString` ctors and one `CCharString` ctor in field
order, `CWideString::operator=`, `operator new`, and the three real component constructors.
The fixture now defines those real symbols (no adapter ring) and was mutation-checked.
`fable_game.h` now uses the PDB names (`CurrentGameComponent`, `ParameterBuffer`, `Quit`,
`InitialWorldName` … `SaveGameName`, `Nowt`, `Nothing`).

## Parallel de-bake round (8 agents, one leaf or header-sharing pair each)


- 009c85a0 NGameText::CDataBank ctor: genuine RELOCATION_MATCH (was failing). Real shape is
  `CDataBank : CBankFile, CResourceBank` with retail-size layouts (retail CBankFile 0x164,
  PDB 0x174) in `fable_game_text_bank.h`.
- 0042dec5 CNewFrontendGameComponent::InitialiseInput: genuine. The adapter was
  `GFGetSystemManager()`; +0x58 is `CSystemManager::PInputManager`.
- 0059a9e0 FrontEndHistoryDequeMapAllocator::Allocate: genuine. It is malloc-backed with a
  zero-count check, so not VC7.1's `std::allocator`; the ternary shape matters.
- 0098dfd0 CThreadedFile ctor: genuine (was failing). RTTI: `CThreadedFile :
  CBaseClassNonCopyable : CBaseClass`; new `fable_base_class.h`.
- 009a9c80 CCountedPointer<CThreadedFile>::Reset: genuine. The control block is the PDB's
  `CCPPointerInfo`, built by `new CCPPointerInfo(&DeleteData, data_)`.
  Side effect: `rebuild/runtime/openretailbank/link_smoke.py` (already failing before) now
  also misses `CCountedPointer<CThreadedFile>::DeleteData`; `runtime_helpers.cpp` still
  defines the old adapter `FableThreadedFileObjectDelete_009A9040`.
- 00499ce0 CProgressDisplay ctor: genuine. Its leading 0x44 bytes are real 12-byte vectors
  (`FableVector<T>` in `fable_boot.h`); flat fields schedule differently.
- 009ea060 GetProgressDisplay: genuine once the split `g_FableProgressDisplayObject/Reference`
  globals became ONE counted-pointer global `g_FableProgressDisplay_013CAA38` (zero-initialised,
  reference-bumping copy ctor, `__fastcall` return by value puts the result pointer in ECX).
  SetProgressDisplay 009e9fd0 still matches; callers use copy-initialisation.
- 00403e40 CCountedPointer<const CIPhysicsFacesSegmentBase>::Reset: genuine, generic
  `CCountedPointer<T>` in the new `fable_counted_pointer.h` (now the one `CCPPointerInfo`).
- 004046b0 comment-list destructor, 00404720 CStringParser ctor: genuine. Retail's lists are
  STLport-shaped (one pointer to a malloc'd sentinel, CRT `free`), new `fable_string_parser.h`.
- 00403b10 CSystemManagerInit ctor: genuine (was failing). 0x4016f0 is `CDisplayManagerInit`'s
  ctor (the `CRegionDef` label was wrong); `stage2_engine_boundary.cpp` now carries an authored
  `CDisplayManagerInit` ctor as integration scaffolding (a landing target, not reconstruction).
- 009ac4d0 CRetailBank ctor (was failing), 009ac530 CContainedBankMap::operator[]: genuine.
  `CRetailBank : CBaseClassNonCopyable`; the map is the standard `lower_bound`/`insert` shape.
- 0099b800 CWideString::operator=(const wchar_t*), 0099be70 operator+(wide, wide): genuine.
  VC7.1 only drops the re-reads of the temporary when `UnassignString`/`MakeOwnCopyOfData`/
  `operator+=` are defined in the same file, as in the original CWideString.cpp.
- NOT landed: 0099bf30 / 0099bff0 (operator+ with a literal) match in scratch, but their
  objects are linked with `stage2_engine_boundary.obj` into the GFMain phase runtimes, whose
  `CWideString` stand-ins would collide (LNK2005). Candidates: agent scratch `ws/f_bf30.cpp`,
  `ws/f_bff0.cpp` (not durable; re-derive with the same same-file-members technique).

## End state

- `gate_boot_leaves.py`: 118/118 pass; 25 of 118 leaves still asm (was 40).
- Full `build_bootstrap.ps1` (run from a temporary copy that only downgrades the WinMain
  fixture's throw, because retail Fable.exe was open): exit 0, GFMain phases 1-10, visual boot
  and frontend sequences PASS. The WinMain fixture itself needs a run with Fable closed.
- `gate_header_dependents.py` (new): the 23 catalog landings that include an edited header
  show 0 regressions against HEAD's headers. 009e1440's fixture fails TCC before and after
  (pre-existing).
- All 32 `rebuild/integration/*.cpp` compile.
- Known fallout outside the gates: `rebuild/runtime/openretailbank/link_smoke.py` (already
  failing before) now also needs the real names for 0x9a9040
  (`CCountedPointer<CThreadedFile>::DeleteData`) and the 009ac530 callees; `runtime_probe.cpp`
  reads `containedBanks_.head_/.size_` under new names; the untracked-by-build
  `faithful/*009ac530*`/`*009a9c80*` traceability copies no longer compile.
- Open duplicate: `fable_string_parser.h` defines a polymorphic `CBase` while
  `fable_persistence.h` has `struct CBase { void** vtable; }` (no file includes both yet).

## Parked near-miss: CNewFrontendGameComponent ctor 0x42ea8f

Not landed (the catalog/crawl lane, not a boot leaf). 295/295 bytes under `/Os` (this TU is
size-optimised: `/O2` gives 324 bytes, and `fldz` stores and an out-of-line
`CMainGameComponentInit` ctor call only appear under `/Os`). All that remains are two
swapped instruction pairs around the second vector and the `SaveGame(L"")` push:

```
retail: [6c] [70] lea eax,[esi+78] [74] mov edi,L"" [eax] [eax+4] push edi lea ecx,[esi+84] [eax+8]
built:  lea eax,[esi+78] [6c] [70] [74] mov edi,L"" push edi lea ecx,[esi+84] [eax] [eax+4] [eax+8]
```

Tried without effect: vector ctor as init list, body stores, chained, reversed, Dinkumware
`_Vector_val` base + comma-expression `_Buy`, complete `CDateAndTime`, extern array vs `L""`,
/Oa /Ow /GS /O1, QFE-4035. Candidate + header: `rebuild/backlog/near_miss/
CNewFrontendGameComponent_ctor_0042ea8f.*`.

Facts recovered on the way (retail RTTI + FableWin symbols + bytes):
- `CGameComponent : CBaseClassNonCopyable (: CBaseClass), CDeviceResetCallback`; ctor
  `CGameComponent(CGame&)` at 0x51d8e0 (manifest label `CAIStateGroup_GazeAtHome` is wrong).
  `CDeviceResetCallback` virtuals: `OnPreDeviceReset`, `OnPostDeviceReset`,
  `ChangeTextureColourDepth(long)`.
- Retail `CNewFrontendGameComponent` is 0x148: the PDB layout with `FileNames`/`FileTimes` as
  12-byte vectors (-8 after them) and one extra `bool` at +0x144 (set true) not in Ego_r.
- `CMainGameComponent` ctor 0x418dca registers `this+4` (its `CDeviceResetCallback`) through
  0x9c0190 on the global at 0x13b8390; the manifest span also swallows a 7-byte function at
  0x418ef0.

## Round 2 (same day, branch land/boot-debake)

Eight agents on 17 more leaves, then one agent that owned the integration layer. Asm leaves
25 -> 7 of 118. Landed genuine (all RELOCATION_MATCH, fixtures rewritten onto real symbols
and mutation-checked):

- 0099ad80 CDiskFileWin32 ctor, 0099a920 Close, 00999230 CAFile::PathExists. RTTI:
  `CDiskFileWin32 : CAFile : CBaseClassNonCopyable`; new `fable_disk_file.h`, CAFile is now the
  real polymorphic interface in `fable_filesystem.h`.
- 009badd0 CPersistContext text-load ctor (only `Mode` in the init list; the rest in the body).
- 004045c0 is `CPersistContext::Transfer<bool>`, not a signed-char transfer: the vtable it builds
  is RTTI `CPersistContext_TransferableComponent<bool>`, binary-in is
  `CPersistTraits<bool>::TransferBinaryIn` (setne), binary-out `WriteEBOOL`. Genuine over all
  208 bytes including the jump table (cases written in reverse mode order, `default:
  __assume(0)`). The six split oracle rows became one `Transfer` row and the bootstrap's
  per-case loop went away. OPEN: FableWin/the manifest named it `Transfer<signed char>` and
  `CIME::LoadIMESettings` passes byte fields (0x18, 0xA0, 0x80) to it; its source still calls a
  declared `TransferSignedChar` (never linked with this leaf). Whether the original called
  `Transfer<bool>` on byte fields or two instantiations were folded is unresolved.
- 0041a540 GetSaveDir (two inlined PDB helpers), 004023f0 GetWindowTitle (boost `scoped_ptr`
  shape from the FableWin debug build; new `fable_scoped_ptr.h`), 00402130 My Documents
  write-permission probe (`__forceinline` wide `operator==`).
- 0099bf30 / 0099bff0 CWideString operator+ with a literal. The same-file helper members must
  stay in each file (removing any breaks parity), so they are `inline` pick-any COMDATs;
  0099b720/0099b510 define theirs `inline __declspec(noinline)` plus a small use so their units
  still emit them (commented as link scaffolding). stage2's CWideString stand-ins are now under
  `#if !defined(FABLETLC_ENABLE_GFMAIN_PHASE3)`, so phase 3+ runtimes run the reconstruction.
- 00401fe0 EULA::DoEULAThings (real dllimports), 00405650 CIME::Initialise (ImeUi_* are
  `__fastcall`; `Enable`/`SetScreenDimensions` defined in the same file). Several manifest
  labels on these callees were wrong.
- 0099f2e0 CCharString::LoadFromFile (models CAFile/CCharString locally because the shared
  `CCharStringData` is still a flat, brace-initialised struct: header debt), 009c9070
  CDataBank::GetPGroupEntry.
- 009fa4e0 CTexture::CopyFromTexture, 009f37e0 CSurface::CopyFromSurface. Neither the gate nor
  the bootstrap puts the Windows SDK on INCLUDE, so `fable_d3d9.h` declares the D3D9/D3DX
  subset by hand (it defers to the real d3d9.h when present). Wiring the SDK path in would be
  cleaner.
- 0098e1e0 CThreadedFile::Open (the sort-key counter must be a file static),
  00418c3b GenerateMetFilesFromLugFiles (`#pragma optimize("s", on)`, same TU as the 0x42ea8f
  near-miss).

Not landed: 009d4c30 CDefStringTable::LoadTable (matches in scratch; needs an inline
`CDiskFileWin32()` and phase-6 integration names; staged in `rebuild/backlog/staged_debakes/`).
Remaining asm leaves: Exit 00401284 (a true import thunk, allowed), GFConfigDetection
004017b0, CSystemManager::Initialise 009a6610, OpenRetailBank 009a8840, OpenIniFile 009a8170,
InitialiseConsoleVariables 00413c50, CDefStringTable::LoadTable 009d4c30.

Integration: the de-baked leaves call real names, so the Stage 3 / visual / GFMain runtimes
needed the boundary files to define those names (stand-ins keep the old adapters' behaviour,
each commented with its retail address) and now link five real reconstructed objects
(CDataBank ctor, font-bank name, CRC calc, MemCmp_Unsigned16, wide+wide operator+) instead of
stand-ins. Visible behaviour changes: the window title now carries retail's trailing space; the
write-permission probe runs in a private temp folder because 0x402130 calls the CRT's
`_wfopen`; phase 8 redirects its own kernel32 import slots so the EULA path never needs a real
eula.dll; stage 3 no longer counts save-metadata cookie checks (the CRT does them now).

0x9c9070: FableWin has both `GetPTextEntry` and `GetPGroupEntry` with identical signatures,
and retail callers of both reach this one body: the linker folded identical COMDATs
(`/OPT:ICF`), so both names are right for the address.

Checker fix: `tools/compare_candidate_objects.py` now runs `objdump -d -z`. Without `-z`,
objdump elides zero runs as `...`, so an unrelocated jump table (all zeros in the object)
vanished and `Transfer<bool>` could never reach its 208 bytes. No other leaf changed.

End state: `gate_boot_leaves.py` 118/118 (7 asm); `gate_header_dependents.py` 0 regressions
across 26 dependents; full bootstrap (temporary no-WinMain copy) exit 0 with GFMain 1-10,
GFMAIN_COMPLETE, visual boot, frontend sequences, Transfer<bool> and CGame::Play PASS. The
OpenRetailBank runtime (`rebuild/runtime/openretailbank`) is back to its pre-session failures
only, via `runtime_real_names.cpp`.

## CGame::Play's component constructors (catalog lane)

Both landed genuine (RELOCATION_MATCH, `/Os` via `#pragma optimize("s", on)`), tests mutation-checked:

- 0042ea8f CNewFrontendGameComponent ctor: the parked near-miss closed once `FileNames`/`FileTimes`
  became STLport vectors (new `fable_stlport_vector.h`: default-argument allocator temporary,
  `_Vector_base`, `_STLP_alloc_proxy`). The near-miss files in `rebuild/backlog/near_miss/` are gone.
- 00418dca CMainGameComponent ctor (294 bytes; 0x418ef0 is a separate 7-byte stub, recorded in
  `rebuild/oracles/boundary-overrides.tsv`). Retail layout = Ego_r's up to `CFrameRateSmoother`, which
  is 0x30 (its `CArray<float>` is an STLport vector), so everything after it is -8. It registers
  `this+4` via `CDisplayManager::AddDeviceResetCallback` on `g_FableDisplayManager_013B8390`.
  Callee labels were wrong: 0x419323 is CNetworkClient(), 0x419747 CFrameRateSmoother(),
  0x9f8b10 CRoughFrameCounter(), 0x415c30 the inline CDiskFileWin32() emitted out of line,
  0x415e17 CMainGameComponentInit's copy ctor.
- `fable_game.h` now has the real `CGameComponent : CBaseClassNonCopyable, CDeviceResetCallback`
  (0x10) and full retail layouts for both components; `CCountedPointer` gained its (PDB) default
  ctor. The CGame::Play fixture now builds components through `CGameComponent(CGame&)` and counts
  strings only outside component ctors.


## Round 3 completion

Six remaining non-thunk boot leaves are now genuine C++: GFConfigDetection (875 bytes),
CSystemManager::Initialise (998), OpenRetailBank, OpenIniFile (1,732),
InitialiseConsoleVariables (4,158), and CDefStringTable::LoadTable. The two component
constructors above are also included. Only Exit 0x401284 remains assembly in the 118-leaf
bootstrap gate; it is the six-byte import thunk. This count covers verified boot leaves,
not all reconstructed sources or a complete playable engine.

The interrupted round's OpenRetailBank and LoadTable work was present and passing when
resumed. LoadTable's integration links the reconstructed CDiskFileWin32::Close; the bank
runtime links and its real fonts.big probe now pass (three subbanks and 26/26 threaded
entries, 53,794,802 payload bytes).

Integration changes preserve the host boundary behavior under the real callee names:

- Configuration uses pConfig, GConfigOptions, CSystemRegistry members, and its real
  import slots. Registry operations remain stand-ins; ConfigDetect.dll is represented
  by the existing recorders. EULA's registry-key symbol aliases the same static object.
- System-manager services now expose their real member symbols. Scoped import-table
  redirection keeps fiber/COM/critical-section operations inside the host fixture.
  The CRT owns allocation and stack-cookie checks, so the former adapter allocation
  assertion is removed and the configuration adapter-cookie counter stays zero.
  Boundary-created manager objects and strings are released after each probe.
- Memory-file integration uses the new inline destructor/GetPathname and buffer layout.
- WinMain's fixture previously depended on the retail global mutex being absent. It
  now redirects its own OpenMutexW/CreateMutexW import slots to a process-specific name,
  still uses the actual Windows mutex APIs, checks the supplied retail name, tests both
  first-instance and existing-instance paths, and closes its handles. No bootstrap
  failure is skipped, and the reconstructed WinMain source is unchanged.

Verification on 2026-09-25:

- `python tools/decomp_pipeline/gate_boot_leaves.py`: 118/118 pass; purity 117 genuine.
- `python tools/decomp_pipeline/gate_header_dependents.py --rev land/boot-debake`:
  zero regressions across 28 dependent catalog entries; both new constructors pass
  RELOCATION_MATCH plus behavior. Existing Render2DDrawList_CopyBlock 009e1440 catalog
  fixture compile failure persists on both header sets.
- Unmodified `powershell -NoProfile -ExecutionPolicy Bypass -File rebuild/build_bootstrap.ps1`:
  WinMain PASS, GFMain phases 1-10 and complete PASS, Stage 3 PASS, visual checkpoint
  and frontend sequence PASS. No temporary script and no skipped WinMain check.
- `python rebuild/runtime/openretailbank/link_smoke.py`: 32/32 compile; zero unresolved.
- `python rebuild/runtime/openretailbank/runtime_probe.py`: real-bank map and threaded
  payload parity PASS against installed English/fonts.big.

The Play boundary still records a call instead of running the game. These results do
not establish playable world entry, full subsystem recovery, or complete relocation-target
validation for every leaf. Continue with the component Init/Run dependency closure and
wire Play once that execution path is supported.
