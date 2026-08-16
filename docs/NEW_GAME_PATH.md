# New Game — end-to-end path (child prologue) — worklist

Maps the retail control flow behind **New Game** (fresh start → hero-as-child in the
Oakvale prologue), the analog of `docs/CONTINUE_GAME_PATH.md`. Two evidence tiers below:
**[DECODED]** = confirmed from disassembly, **[INFERRED]** = structural inference from
the name map (verify before relying on for byte-parity).

## Frontend game-start staging  [DECODED]
`CNewFrontendGameComponent::LoadGame @ 0x00594f36` (31B) is the shared Continue/Load
staging entry. Decoded body:
```
mov esi,ecx                 ; this
mov eax,[esi]; call [eax+0x10]   ; vtbl slot 4 — PrepareLoad/transition
push [esp+8]                ; the save name arg
lea ecx,[esi+0x84]          ; this->pendingName (CWideString @ +0x84)
mov byte[esi+0x29],1        ; this->f_29 = 1  (pending game-start flag)
call 0x99b7d0               ; pendingName.Assign(name)
ret 4
```
So a game start = **vtbl[4]() prepare → set pending flag (+0x29) → stage a name (+0x84)**.
New Game is the same shape with an empty/new-profile name (its dedicated dispatch is in
the `CFrontEndManager::Action` switch — see below). LoadGame itself is a documented
`push [mem]` byte-parity DEFER (CONTINUE_GAME_PATH.md); its semantics are recovered.

## Frontend dispatch  [INFERRED]
- `CFrontEndManager::Action @ 0x0059a238` — the UI action-ID switch that routes the New
  Game / profile buttons. (RELOCATION_MATCH landed per manifest.)
- `CFrontEndManager::GotoNewProfileScreen @ 0x00595845` (176B) [DECODED head] — navigates
  to the New-Profile screen (name field + settings). Sets screen flags `+0x64=1`, `+0xa0=1`,
  builds a CWideString temp, calls sub-screen setup. **This is the entry to the profile-
  creation screen — see docs/NEW_PROFILE_SCREEN.md.**
- `CFrontEndManager::GotoProfileMenu @ 0x00597b20`, `GotoNextScreen @ 0x00596763`.

## World init (New Game ≠ Load)  [DECODED — byte-exact landed]
New Game does **not** call `CWorld::LoadGameState` (that deserializes a save). It creates
fresh world state:
- **`CMainGameComponent::InitWorld @ 0x0041735a` (190B) — LANDED byte-exact (RELOCATION_MATCH).**
  Builds a 0x14-byte context `{this, this->f1c, &CGameDefinitionManager::Get()}`, then
  `w = operator new(0x198); if (w) w = w->CWorld::ctor(&ctx);` (the CWorld is 0x198=408
  bytes), stores it in the CCountedPointer at `this->+0x24` (`DeleteData(w)` = assign),
  conditionally calls `InitEventPackageLoading`/`InitEventPackageSaving` (globals
  0x13b85f8/0x13b85f7 gate them, def-mgr singleton 0x13b8660), shows a progress line
  (`DisplayProgress(CCharString("...",-1), false, 0.5f, 0)`), then calls `world->vtbl[9]()`.
  **TU flags: `/O1 /Oy-`** (size-opt + frame pointer — NOT the usual /O2 /Oy; catalog entry
  records this). Reconstruction keys: the ctor is a real member (no edx), the null-`new`
  path reuses new's 0 (no extra xor), the CCharString lives in a block so its dtor fires
  before the vtbl call, and the progress arg is a `bool` (`xor dl,dl`) with a non-zero float.
- `InitPlayerManager @ 0x0041732a` (already MATCH).
- `CWorld::PostInit @ 0x004a6550(CDisplayEngine&)`, `CWorld::PostLoadInit @ 0x0049d970`
- The child-mode flag: **`GetHeroAge` returns 18 in the child prologue** (a special mode
  set for new games; CONTINUE_GAME_PATH.md / memory fable-level-modding-gotchas).

## Level / region streaming  [INFERRED]
- Starting region: **`StartOakvale_Filler`** (WORLD_AND_CHEST_INDEX.md — first playable,
  9 entities). Streamed from FinalAlbion.wad.
- `CWorldMap::LoadLevel @ 0x00502620` (560B) → `CreateWorldLevelWad @ 0x00502170`.
- Prologue quest/script: `CQ_NewOakValeIntroScript @ 0x00d25410` / `0x00daac00`
  (`DoMission @ 0x00dbde40`, `AttackStuff @ 0x00dbe3c0`).

## Hero-child spawn  [INFERRED]
- `CThingCreatureBase::OnCreateHero @ 0x006a9ef0`.
- `CThingManager::LoadGameState @ 0x005253f0` (creature entities),
  `CPlayerManager::LoadGameState @ 0x00449e60` (player state).

## Reconstruction status & order
Per CONTINUE_GAME_PATH.md's tier-1 probe finding, the big world-load coordinators/
deserializers are largely a **byte-parity DEFER class** (reg-alloc/temp-scheduling
artifacts that scale with call count) — pursue them as documented-semantics + behaviour-
verified, not byte-exact catalog landings, unless a QFE-4035 compiler is wired in
(note: QFE-4035 codegen was proven == RTM 3077 — see docs/QFE4035_COMPILER_GATE.md — so
that lever is void; these are genuine regalloc coin-flips).

The tractable byte-exact lane is the **frontend New-Profile screen leaves**, most of which
are already landed — see docs/NEW_PROFILE_SCREEN.md. Verify the [INFERRED] world-init chain
by disassembling `CMainGameComponent::InitWorld @ 0x0041735a` and tracing its callees before
committing to reconstruct that tier.
