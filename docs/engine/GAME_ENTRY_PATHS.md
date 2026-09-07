# Game entry paths — Continue/Load Game, New Game, New-Profile screen

Merged reference (2026-09-07) for the retail control flow behind the three ways into a game session:
**Continue / Load Game** (save deserialization), **New Game** (fresh world, child prologue) and the
**New-Profile screen** (name field + default settings). Consolidates the former
`CONTINUE_GAME_PATH.md`, `NEW_GAME_PATH.md` and `NEW_PROFILE_SCREEN.md`. Feeds remake threads #4
(Continue), #2 (save), #1 (world load). Grounded in the 2026-08-10 canonical manifest. Evidence tiers
in the New Game section: **[DECODED]** = confirmed from disassembly, **[INFERRED]** = structural
inference from the name map (verify before relying on for byte-parity).

Contents

- [Dated findings](#dated-findings)
- [Shared frontend game-start staging](#shared-frontend-game-start-staging)
- [Continue and Load Game path](#continue-and-load-game-path)
- [Save-enumeration seam (CUserProfileManager)](#save-enumeration-seam-cuserprofilemanager)
- [New Game path (child prologue)](#new-game-path-child-prologue)
- [New-Profile screen](#new-profile-screen)
- [Byte-parity policy for this lane](#byte-parity-policy-for-this-lane)
- [Superseded](#superseded)

---

## Dated findings

- **2026-08-10** — `CNewFrontendGameComponent::LoadGame` 0x00594f36 is a 31B codegen-artifact DEFER
  (`push [esp+8]` push-memory class), semantics recovered; manifest 66B span over-captures a
  following accessor cluster.
- **2026-08-10** — Save-enumeration seam opened: it lives in **CUserProfileManager** (no separate
  CSaveGameManager). First byte-exact landings `GetAutoSaveFileName` 0x00406690 and 0x004065d0
  (stack-hidden-return CWideString factory shape via ctor 0x99b6b0).
- **2026-08-10** — Display-name tier: `GetAutoSaveDisplayName` 0x4069e0 + 0x406c20,
  `GetManualSaveFileName` 0x406610, `GetAutoSavePathName` 0x406f70 landed byte-exact (6 in the seam).
- **2026-08-11** — `GetEmptySlotName` 0x406a80 LANDED (RELOCATION_MATCH 88/88) via the NRVO shape
  `return mgr->f14->Format(&temp);` — seam at 7 byte-exact fns.
- **2026-08-11** — Tier-1 probe: `CPlayerManager::LoadGameState` 0x449e60 DIFFER(287v297), behaviour
  PASS — the 10-byte gap is a register-allocator artifact in LoopB (retail keeps `begin` in ebp with
  two base copies, found-deref out-of-line at 0x449f7f). Load-path coordinators declared a byte-parity
  DEFER class pending a QFE-4035 compiler.
- **2026-08-13** — Byte-pure minimap-decode target `lzo1x_decompress` @ 0x00c06b90 (578B) is a hard
  byte-parity DEFER (faithful C = 593B, diverges at prologue regalloc; permuter bottomed at score
  101574). Functional decoders (`fable_texture_decode.c`, `frontend_minimap.cpp`) byte-verified on
  95/95 MINIMAP entries serve as the differential oracle.
- **2026-08-16** — Find-loop blocker SOLVED: retail's find-loops use **index-based access**
  (`this->begin[i]`) with the found-case element load OUT-OF-LINE; a pointer walk (`++p`) folds the
  deref. With the index idiom `CPlayerManager::LoadGameState` 0x449e60 reproduces the entire 297-byte
  instruction sequence byte-for-byte; residual = local stack-slot displacement bytes (retail
  char-transfer output at +0x10, localStr at +0x20; VC reverses them). Idiom documented in
  `tools/decomp_pipeline/crawl/README.md`.
- **New Game (undated, landed)** — `CMainGameComponent::InitWorld` 0x0041735a (190B) LANDED byte-exact
  with TU flags `/O1 /Oy-`; `CWorld::PostInit` 0x004a6550 (625B) decoded, DEFER (CCharString
  temp-scheduling tier).
- **New-Profile screen (undated)** — 8 functions landed byte-exact; 4 deferred with semantics
  recovered (`ApplyNewProfileValues` 0x0040cca0 DIFFER 535v535 register rotation;
  `GetDirectoryNameFromProfileName` 0x004071c0; `GetValidProfileName` 0x00406350;
  `SetDefaultValuesForNewProfile` 0x00409b70 FPU tail).

---

## Shared frontend game-start staging

`CNewFrontendGameComponent::LoadGame @ 0x00594f36` (31B) is the shared Continue/Load/New-Game
staging entry. Decoded body **[DECODED]**:

```
mov esi,ecx                 ; this
mov eax,[esi]; call [eax+0x10]   ; vtbl slot 4 — PrepareLoad/transition
push [esp+8]                ; the save name arg
lea ecx,[esi+0x84]          ; this->pendingName (CWideString @ +0x84)
mov byte[esi+0x29],1        ; this->f_29 = 1  (pending game-start flag)
call 0x99b7d0               ; pendingName.Assign(name)
ret 4
```

So a game start = **vtbl[4]() prepare → set pending flag (+0x29) → stage a name (+0x84)**. New Game
is the same shape with an empty/new-profile name (its dedicated dispatch is in the
`CFrontEndManager::Action` switch). Semantics: `self->vtbl[4](); self->f29 = 1;
(self+0x84).Assign(name);` (helper 0x99b7d0 is a thiscall taking one stack arg).

**`LoadGame` is a codegen-artifact DEFER, not a gap (2026-08-10).** Real body is 31B (the 66B manifest
span over-captures a following accessor cluster — trim to
`568bf18b06ff5010ff7424088d8e84000000c6462901e87f6840005ec20400`). Verified byte-recovery is
BLOCKED: retail forwards the load-name via `push [esp+8]` (push-memory), but VC7.1 emits `mov
reg,[esp+8]; push reg` for the equivalent C++ — the documented `push [mem]` one-off reject class. Its
DIFFER catalog state is correct; leave it.

---

## Continue and Load Game path

### Call path (frontend action -> resume)

```
CNewFrontendGameComponent::LoadGame   0x00594f36  (31B*)  Continue/Load dispatch
    self->vtbl[4]();  self->f29 = 1;  (self+0x84).Assign(name)   // stage the pending load
        |
        v  (save enumeration/metadata layer — CMet*, currently STUBBED at the
           integration boundary: save_metadata_runtime_boundary.cpp returns null)
        |
CWorld::LoadGameState                  0x004a3200  (752B)   world-level load coordinator
    -> CWorld::LoadGameStateInternal   0x004a21f0  (2944B)  the heavy deserializer
    -> CWorld::LoadWorldFromFile       0x0049e220  (67B*)   mangled UAEX, opens CAFile
       (per-subsystem LoadGameState deserializers, called from the coordinator:)
    -> CThingManager::LoadGameState    0x005253f0  (2272B)
    -> CQuestManager::LoadGameState    0x004b6380  (352B)   (+ 0x004b5500 variant)
    -> CScriptBase::LoadGameState      0x004ca230  (1792B)  (+ 0x00cb8690 variant)
    -> CPlayerManager::LoadGameState   0x00449e60  (304B)
    -> CUserProfileManager::LoadGameState 0x0040bca0 (2800B)
        |
        v  (level/region streaming)
    -> CWorldMap::LoadLevel            0x00502620  (560B)
    -> _global::LoadLevel_ParseObjects 0x00501150  (768B)
    -> _global::LoadGameStateInternal_ParseRegions 0x005079c0
    -> _global::CThingManager_LoadGameState_Scripts 0x00520670
    -> Subsystem_LoadWorldMapAndNotifications 0x00c8b490 (736B)
```
(*) real body length after trimming manifest over-capture; all rows `retail_parity = -` except
LoadGame which is `DIFFER`.

### Tractability (order to reconstruct)

1. **Small / near-term** (byte-parity feasible now):
   - `CWorld::LoadWorldFromFile` 0x0049e220 (67B) — multi-vtbl-call + two calls to a float-returning
     helper 0x9e1bc0 (result discarded, likely a profiler/timer); mangled virtual so author as a real
     member. Trim trailing int3 first.
2. **Medium** (300-800B, doable, several vtbl/def-table calls each): `CPlayerManager::LoadGameState`
   (304B), `CQuestManager::LoadGameState` (352B), `CWorldMap::LoadLevel` (560B),
   `CWorld::LoadGameState` (752B), `Subsystem_LoadWorldMapAndNotifications` (736B),
   `LoadLevel_ParseObjects` (768B).
3. **Large** (1.8-2.9KB, multi-session, the actual save deserializers):
   `CWorld::LoadGameStateInternal` (2944B), `CUserProfileManager::LoadGameState` (2800B),
   `CThingManager::LoadGameState` (2272B), `CScriptBase::LoadGameState` (1792B).

### CPlayerManager::LoadGameState 0x449e60 — the probe history

- **2026-08-11 tier-1 byte-parity PROBE.** Smallest coordinator (297B, 13 calls) probed for
  byte-exact recovery. Result: **DIFFER(287v297), behaviour PASS — NOT byte-matchable with RTM 3077**
  at that time. ~97% reproduced exactly (frame, LoopA, the Transfer field-tag calls + CCharString
  temp ctor/dtor scheduling, LoopC out-of-line found-block). The entire 10-byte gap is one
  **register-allocator artifact in LoopB**: retail keeps `begin` in ebp with two base copies
  (edi=result base, edx=walk) so the found-deref goes OUT-OF-LINE (the 0x449f7f block) + reload
  bytes; VC7.1 kept `begin` in a single live reg and FOLDED both derefs into one shared `mov
  ecx,[reg+ecx*4]`. Not flippable from the C++ shapes tried (≥4) nor the /GS,/Oa×pragma sweep. Only
  lever thought left was a QFE-4035 cl.exe (retail built ~11 objs with it). Faithful 287B source +
  passing test preserved in scratchpad t2_probe/.
  **Implication drawn then:** the load-path coordinators/deserializers are largely a byte-parity DEFER
  class (reg-alloc/temp-scheduling artifacts scale with call count → the 300–740B fns will miss more,
  not less). Pursue as **documented-semantics + behavior-verified** reconstruction unless a QFE-4035
  compiler is wired into the harness sweep. The tractable byte-exact wins are the small seam leaves.
- **2026-08-16 find-loop blocker SOLVED; reduced to slot-layout near-match.** Retail's find-loops
  keep `begin` in a BASE register + a separate INDEX and emit the found-case element load
  OUT-OF-LINE (`je found; …; found: mov reg,[base+idx*4]; jmp use`). The prior probe used a
  **pointer walk** (`++p`), which folds the deref into one shared `mov` and misses the out-of-line
  block. **Index-based access** (`this->begin[i]`) reproduces retail's exact idiom:
  ```
  T* n = this->begin[this->defaultIdx];
  for (int i=0;i<count;i++){ if (this->begin[i]->field==target){ n=this->begin[i]; break; } }
  Use(n);
  ```
  With this, a full reconstruction of `CPlayerManager::LoadGameState` (0x449e60) reproduces the
  **entire 297-byte instruction sequence byte-for-byte** (both find-loops match) — the residual is
  only local-*slot* displacement bytes (VC allocates the CCharString/Transfer-output locals to
  different `[esp+X]` slots than retail: retail puts the char-transfer output at +0x10, localStr at
  +0x20; VC reverses them). Behaviour PASS, /O2 /Oy (esp-relative, no frame ptr), 297–299B. The
  remaining gap is a compiler stack-slot-ordering detail (needs a slot-allocation-aware permuter or
  the exact retail declaration order), NOT the regalloc blocker. Idiom documented in
  `tools/decomp_pipeline/crawl/README.md`; it applies to the other find-loop coordinators
  (CQuestManager/CWorldMap load paths).

### Known blockers / findings (2026-08-10)

- `CNewFrontendGameComponent::LoadGame` — codegen-artifact DEFER (see staging section).
- **Native save metadata/load ownership is still stubbed** (`save_metadata_runtime_boundary.cpp`:
  `FableMetGetAutoSaveName` returns null, `StoreNull`). The on-disk enumeration and metadata read
  contract is no longer a data gap: `tools/save_metadata.py` joins the `Profile.bin` registry to
  validated `.sav` `HEADER` fields and emits the recovered row actions (`0x11` valid / `0xDC` missing
  or corrupt). The remaining seam is wiring those rows into the native C++ frontend and then into
  `CWorld::LoadGameState`.
- World load bottoms out at the level/region streaming (`CWorldMap::LoadLevel`, `FinalAlbion.wad`);
  registration requires the FinalAlbion_RT.stb common-header chunk (see NEW_LEVEL_ASSEMBLY.md /
  OpenRetailStaticMap 0xB41E50 gotcha).

### Byte-pure minimap-decode targets (2026-08-13) — lzo1x_decompress is a hard defer

Pursuing the saved-games region-minimap preview as PURE container reads (no baking), the byte-pure
primitive is the engine's own `lzo1x_decompress @ 0x00c06b90` (578B, `__cdecl(src, src_len, dst,
out_len*)`). Reconstruction status: **hard byte-parity DEFER.** A faithful C reconstruction compiles
to 593B (right length class) but diverges from retail at the prologue register allocation
(ebp=in_end/ebx=out_len/esi=in/eax=op/edi=match), cascading to ~540/578 differing bytes. The permuter
(`tools/permuter/autopermute.py`, `--mutate --random 200`, 4690 evals) bottomed at score 101574
(593v578, prefix 4) — its temp-intro/reassoc mutations don't reach retail's allocation. Needs the
upstream-style full-regalloc/statement-reorder permuter (README "Next") or hand regalloc-matching.
Best source: scratchpad lzo1x.best.cpp; oracle bytes in the crawl.

The functional decoders built that session (`rebuild/integration/fable_texture_decode.c` LZO1X+DXT3,
`frontend_minimap.cpp`) are byte-verified vs the Python reference on 95/95 real
`MINIMAP_*_FRONT_END` entries — they serve as the DIFFERENTIAL ORACLE for the byte-exact
reconstruction, and as functional scaffolding until 0x00c06b90 lands. Other byte-pure targets for the
pure texture path: the chunk-decompress caller and the engine DXT3/surface upload (retail likely
uploads DXT3 to the GPU, no software decode).

### Resume

Start byte-parity work at tractability tier 1-2 through the normal crawl (`verify_and_land`, trim
over-captures first). The large deserializers and the save enumeration seam are the multi-session
core; the map above is the dependency order.

---

## Save-enumeration seam (CUserProfileManager)

The seam lives in **CUserProfileManager** (not a separate CSaveGameManager): it owns save
filename/display-name generation + file-list enumeration. The DISPLAY-name getters are what the
frontend save list shows ("AutoSave"/"Save 1/2/3"); `LoadFileList` enumerates the slots.

- **2026-08-10** — first byte-exact landings (RELOCATION_MATCH): `GetAutoSaveFileName` 0x00406690
  and 0x004065d0 — both the stack-hidden-return CWideString factory shape (`return
  CWideString(literal)` via ctor 0x99b6b0), same family as the GetActionName factories. The manifest
  over-captures them (int3-padded 32/64B spans = 21B real fns — trim first). Remaining cluster then:
  GetManualSaveFileName 0x406610 (128B), GetAutoSaveDisplayName 0x4069e0/0x406c20 (160B),
  GetEmptySlotName 0x406a80 (86B: g_13b86a0 check -> CWideString ctor 0x99ebf0 + call 0x9c95e0 + dtor
  0x99eae0, else the 0x99b6b0 factory), GetManualSaveDisplayName 0x406ae0 (320B), GetAutoSavePathName
  0x406f70 (192B), GetSaveDisplayNameFromFileName 0x407e10 (448B), LoadFileList 0x4091c0 (544B).
- **2026-08-10 display-name tier — 4 more landed (6 total):** GetAutoSaveDisplayName 0x4069e0 +
  0x406c20 (the frontend "AutoSave" display strings — two-branch: format from save-manager singleton
  g_13b86a0->f14 / g_13b871c->f60 via 0x9c95e0, else literal factory 0x99b6b0),
  GetManualSaveFileName 0x406610 (multi-temp CWideString concat: default ctor 0x99aed0 + format
  0x99ba70 + concat 0x99b720/0x99be70 + dtors), GetAutoSavePathName 0x406f70 (with
  __security_cookie + /GS + a 0x104 path buffer via 0x406e30 + WConcat 0x595080).
- **2026-08-11** — GetEmptySlotName 0x406a80 **LANDED (RELOCATION_MATCH 88/88)**; the prior
  DIFFER(94v88) +6B was NRVO shape: modeled as `return mgr->f14->Format(&temp);` (struct-return method
  NRV-elides into the outer return slot) with `temp = CWideString(lit 0x122df80, -1)` in the
  if-branch and `return CWideString(lit 0x122df5c)` in the else. **Seam now 7 byte-exact fns.**

Deferred (semantics recovered, byte-blocked):
- GetManualSaveDisplayName 0x406ae0 DIFFER(393v314): 2-branch multi-temp concat + tail to
  GetManualSaveFileName; VC temp/dtor scheduling won't match.
- GetSaveDisplayNameFromFileName 0x407e10: SEH-unwind cleanup bitmask (test bl,0x10/8/4/2/1 ->
  conditional dtors) not reproducible from C++; dispatches to GetAutoSave/ManualDisplayName.
- LoadFileList 0x4091c0: __security_cookie + STL vector<save-entry> iteration (rep movsd of 0x1c-byte
  entries, begin/next 0x999760/0x999850); enumerates slots into the caller's vector.

The New-Profile settings persist through this same CUserProfileManager save path.

---

## New Game path (child prologue)

Maps the retail control flow behind **New Game** (fresh start → hero-as-child in the Oakvale
prologue). Frontend staging is the shared `LoadGame` shape above.

### Frontend dispatch [INFERRED]

- `CFrontEndManager::Action @ 0x0059a238` — the UI action-ID switch that routes the New Game /
  profile buttons. (RELOCATION_MATCH landed per manifest.)
- `CFrontEndManager::GotoNewProfileScreen @ 0x00595845` (176B) [DECODED head] — navigates to the
  New-Profile screen (name field + settings). Sets screen flags `+0x64=1`, `+0xa0=1`, builds a
  CWideString temp, calls sub-screen setup. **This is the entry to the profile-creation screen** —
  see [New-Profile screen](#new-profile-screen).
- `CFrontEndManager::GotoProfileMenu @ 0x00597b20`, `GotoNextScreen @ 0x00596763`.

### World init (New Game ≠ Load) [DECODED — byte-exact landed]

New Game does **not** call `CWorld::LoadGameState` (that deserializes a save). It creates fresh
world state:
- **`CMainGameComponent::InitWorld @ 0x0041735a` (190B) — LANDED byte-exact (RELOCATION_MATCH).**
  Builds a 0x14-byte context `{this, this->f1c, &CGameDefinitionManager::Get()}`, then `w = operator
  new(0x198); if (w) w = w->CWorld::ctor(&ctx);` (the CWorld is 0x198=408 bytes), stores it in the
  CCountedPointer at `this->+0x24` (`DeleteData(w)` = assign), conditionally calls
  `InitEventPackageLoading`/`InitEventPackageSaving` (globals 0x13b85f8/0x13b85f7 gate them, def-mgr
  singleton 0x13b8660), shows a progress line (`DisplayProgress(CCharString("...",-1), false, 0.5f,
  0)`), then calls `world->vtbl[9]()`. **TU flags: `/O1 /Oy-`** (size-opt + frame pointer — NOT the
  usual /O2 /Oy; catalog entry records this). Reconstruction keys: the ctor is a real member (no
  edx), the null-`new` path reuses new's 0 (no extra xor), the CCharString lives in a block so its
  dtor fires before the vtbl call, and the progress arg is a `bool` (`xor dl,dl`) with a non-zero float.
- `InitPlayerManager @ 0x0041732a` (already MATCH).

#### `CWorld::PostInit @ 0x004a6550` (625B, __thiscall(CDisplayEngine&)) [DECODED — DEFER]

The **New Game subsystem bring-up**, called after the world exists. Structure: ~5 repeated blocks,
each = `CCharString("loading label", -1)` → `$E2 0x9d8240` (register?) → dtor → `CCharString` again →
`DisplayProgress(label, false, -1.0f, 0)` → dtor → `operator new(N)` + ctor + store. The
display-engine arg is kept in `ebx` (`[esp+0x18]`). Subsystems created, with their CWorld field slots:
- `+0x24` ← combat-animation set (`new 0x20`, `GetCombatAnimationSet` 0x6b1960; `Reset` 0x4ab300)
- `+0x38` ← `CGameScriptInterface` (`new 0x50`, ctor 0x6e7740 with this->f8/fc/f10 + world +
  displayEng); the prior `+0x38` value is released via its vtbl[0] with arg 1
- `+0x58` ← `CScriptInfoManager` (`new 0x20`, 0xcb5c70; `Reset` 0x4ab370; then `Init` 0xcb5d80); a
  `CQuestManager` (`new 0xb4`, ctor 0x4b4590 with world + scriptinfo + f38) registered via `Reset` on
  the global manager `0x13b89fc` (0x4a9a10)
- calls `displayEng->f2c->vtbl[7](&world->f4)` — hands the world region/params to the display engine
- `+0x6c` ← engine/render subsystem (`new 0x40`, 0x6be300; `Reset` 0x4ae650)
- tail: the render manager `g @ 0x13b8394` gets `vtbl[0x2e](0x10,0,0)`, `(0x40,0,0)`, `(0x80,0,0)` —
  three render-layer/pass setups.

**Parity: DEFER (temp-scheduling tier).** 15+ `CCharString` temp ctor/dtor pairs interleaved with the
subsystem `new`s — the exact temp scheduling is the documented not-byte-matchable class. Semantics
fully recovered above. Try `/O1 /Oy-` if revisited, but the CCharString scheduling is the real blocker.
- Also: `CWorld::PostLoadInit @ 0x0049d970`.
- The child-mode flag: **`GetHeroAge` returns 18 in the child prologue** (a special mode set for new
  games; memory fable-level-modding-gotchas).

### Level / region streaming [INFERRED]

- Starting region: **`StartOakvale_Filler`** (WORLD_AND_CHEST_INDEX.md — first playable, 9 entities).
  Streamed from FinalAlbion.wad.
- `CWorldMap::LoadLevel @ 0x00502620` (560B) → `CreateWorldLevelWad @ 0x00502170`.
- Prologue quest/script: `CQ_NewOakValeIntroScript @ 0x00d25410` / `0x00daac00` (`DoMission @
  0x00dbde40`, `AttackStuff @ 0x00dbe3c0`).

### Hero-child spawn [INFERRED]

- `CThingCreatureBase::OnCreateHero @ 0x006a9ef0`.
- `CThingManager::LoadGameState @ 0x005253f0` (creature entities), `CPlayerManager::LoadGameState @
  0x00449e60` (player state).

### Reconstruction status & order

The big world-load coordinators/deserializers are largely a **byte-parity DEFER class**
(reg-alloc/temp-scheduling artifacts that scale with call count) — pursue them as
documented-semantics + behaviour-verified, not byte-exact catalog landings. Note: QFE-4035 codegen was
proven == RTM 3077 (see docs/pipeline/QFE4035_COMPILER_GATE.md), so that lever is void; these are
genuine regalloc coin-flips — except where the 2026-08-16 index-based find-loop idiom applies.

The tractable byte-exact lane is the **frontend New-Profile screen leaves**, most of which are
landed. Verify the [INFERRED] world-init chain by disassembling `CMainGameComponent::InitWorld @
0x0041735a` and tracing its callees before committing to reconstruct that tier.

---

## New-Profile screen

The profile-creation screen reached from New Game (`GotoNewProfileScreen @ 0x00595845`): a name field
plus the default game settings (volumes, brightness/gamma, toggles). Backed by
**CUserProfileManager**, which owns profile create/name/settings/save. Parity is mostly DONE.

### Flow

```
New Game → GotoNewProfileScreen 0x00595845  (sets screen flags +0x64/+0xa0, builds name UI)
  → [user types name + adjusts sliders/toggles]
  → CreateNewProfile 0x004945b0 (frontend) → 0x0059697a (manager, p2)
      → SetDefaultValuesForNewProfile 0x00409b70   (seed default settings)
      → ResetSlidersToProfileValues 0x00595356     (push settings → sliders)
      → SetProfileName 0x0040d400                  (name field → profile)
      → GetValidProfileName 0x00406350             (validate/sanitize name)
      → GetDirectoryNameFromProfileName 0x004071c0 (name → Saves\<dir>)
      → ApplyNewProfileValues 0x0040cca0           (staged settings → active + notify)
      → save profile
```

### Parity status

**Landed byte-exact (8):** `IsEnoughSpaceForProfile` 0x004067c0, `SetProfileName` 0x0040d400,
`CreateNewProfile` 0x004945b0, `ResetSlidersToProfileValues` 0x00595356, `AddProfileNameToScreens`
0x00595b24, `SetScoreboardOptionsFromProfile` 0x005963db, `GetScoreboardOptionsToProfile`
0x00596550, `GotoProfileMenu` 0x00597b20.

**Deferred (semantics recovered, behaviour-verified, byte-blocked):**
- `ApplyNewProfileValues` 0x0040cca0 (535B, DIFFER 535v535, 80 masked-byte diff) — 33 staged→active
  setting copies each followed by a `this->Apply()` notify (0x407370), one conditional (`if
  this->f78: 0x408820`) and a tail call (0x40a740). The gap is the register **rotation** across the
  33 copies (retail cycles eax/ecx/edx as scratch and varies the `mov ecx,esi` placement); not
  source-reachable. Full copy map below.
- `GetDirectoryNameFromProfileName` 0x004071c0 (353B) — `__security_cookie` + multi-temp
  `CWideString` concat (0x99ebf0/99eae0/99aed0/99b6b0/99b510/99bff0…). The temp/dtor scheduling
  defer class (same as the save-display-name getters).
- `GetValidProfileName` 0x00406350 (629B, 48 calls) — string validation, same tier.
- `SetDefaultValuesForNewProfile` 0x00409b70 (1105B, FPU) — clean const stores up front, then x87
  (fild) + branches; the FPU tail is the fiddly class.

### Recovered profile-settings layout

Two parallel blocks in the profile object: **staged** (screen-edited, `+0xcd..0x134`) and **active**
(in-use, `+0xd..0x7c`). `ApplyNewProfileValues` copies staged→active and notifies.

Defaults (from `SetDefaultValuesForNewProfile`) and the staged→active map (from
`ApplyNewProfileValues`):

| staged | default | → active | type | likely meaning |
|---|---|---|---|---|
| +0xcd | 1 | +0x0d | bool | toggle (on) |
| +0xce | 1 | +0x0e | bool | toggle (on) |
| +0xcf..+0xd6 | 0 | +0x0f..+0x16 | bool | toggles (off) |
| +0xd7/+0xd8/+0xd9 | 1 | +0x17/+0x18/+0x19 | bool | toggles (on) |
| +0xf0/+0xf1 | 0 | +0x30/+0x32 | bool | toggles |
| +0xdc | 1.0f | +0x1c | float | volume |
| +0xe0 | 1.0f | +0x20 | float | volume |
| +0xe4 | 0.8f | +0x24 | float | slider (music?) |
| +0xe8 | 0.6f | +0x28 | float | slider |
| +0xec | 0.9f | +0x2c | float | slider |
| +0xf4/+0xf8/+0xfc/+0x100 | — | +0x38/+0x3c/+0x40/+0x44 | float | sliders |
| +0x104/+0x108 | — | +0x48/+0x4c | float | sliders |
| +0x10c | — | +0x50 | bool | toggle |
| +0x124/+0x128 | — | +0x70/+0x74 | float | sliders |
| +0x12c | 1 | +0x79 | bool | toggle |
| +0x130 | 0.5f | +0x7c | float | brightness/gamma |
| +0x134 | — | +0x6c | bool | toggle (after `if +0x78`) |

(Copy order is not offset-sorted — see the source; `+0xec` copies before `+0xe8`, the +0x124/+0x128
pair interleaves, etc.)

### Purity

All landed functions are **byte-exact reconstructions of the retail code** that read/write the real
profile object fields (no baking) — purity is inherent. The name field flows through the engine's own
`SetProfileName`/`GetValidProfileName`/`GetDirectoryNameFromProfileName`; settings flow staged→active
via `ApplyNewProfileValues` and persist through CUserProfileManager's save path (the
[save-enumeration seam](#save-enumeration-seam-cuserprofilemanager)). No values are hardcoded outside
`SetDefaultValuesForNewProfile`, which is the retail default table itself.

### Next

- Verify the settings labels against the on-screen sliders (VISUAL_PARITY_STATUS.md harness) to
  name each field.
- The 4 deferred functions are behaviour-correct; leave as documented-semantics unless a
  register-level permuter lands (docs/pipeline/QFE4035_COMPILER_GATE.md escalation ladder).

---

## Byte-parity policy for this lane

- Small seam leaves (CUserProfileManager getters, New-Profile accessors, `InitWorld`) land byte-exact
  through the normal crawl — trim manifest over-capture (int3 padding / swallowed accessor clusters)
  first.
- Multi-call coordinators: apply the **index-based find-loop idiom** (2026-08-16) before declaring a
  regalloc DEFER; what remains after it is stack-slot ordering (needs a slot-aware permuter or the
  retail declaration order).
- CCharString/CWideString temp-scheduling bodies (`PostInit`, display-name concats,
  `GetDirectoryNameFromProfileName`), SEH-unwind bitmask cleanups
  (`GetSaveDisplayNameFromFileName`), register-rotation copy chains (`ApplyNewProfileValues`),
  `push [mem]` forwarders (`LoadGame`) and x87 tails (`SetDefaultValuesForNewProfile`): documented
  semantics + behaviour-verified, not catalog byte landings.

## Superseded

- **"CPlayerManager::LoadGameState is NOT byte-matchable with RTM 3077 — regalloc artifact in LoopB"
  (2026-08-11)** — superseded 2026-08-16: the artifact was the source's pointer-walk find-loop; the
  index-based idiom reproduces all 297 instruction bytes, leaving only stack-slot displacement bytes.
- **"Only lever left is a QFE-4035 cl.exe" (2026-08-11)** — void: QFE-4035 codegen proven == RTM
  3077 (docs/pipeline/QFE4035_COMPILER_GATE.md).
- **GetEmptySlotName 0x406a80 DIFFER(94v88) as a deferred item (2026-08-10)** — landed 2026-08-11
  (RELOCATION_MATCH 88/88) via the NRVO return shape.
- **Save enumeration as a data gap** — closed by `tools/save_metadata.py` (Profile.bin registry ⨝
  `.sav` HEADER → row actions 0x11/0xDC); the remaining gap is native wiring only.
