# Continue Game — end-to-end reconstruction path (worklist)

Maps the retail control flow behind **Continue / Load Game** so it can be
reconstructed byte-exact through the crawl. Feeds remake threads #4 (Continue),
#2 (save), #1 (world load). Grounded in the 2026-08-10 canonical manifest.

## Call path (frontend action -> resume)

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
(*) real body length after trimming manifest over-capture; all rows currently
`retail_parity = -` except LoadGame which is `DIFFER`.

## 2026-08-11 — tier-1 byte-parity PROBE result (important scoping finding)

`CPlayerManager::LoadGameState` 0x449e60 (smallest coordinator, 297B, 13 calls) was
probed for byte-exact recovery. Result: **DIFFER(287v297), behaviour PASS — NOT
byte-matchable with RTM 3077.** ~97% reproduced exactly (frame, LoopA, the Transfer
field-tag calls + CCharString temp ctor/dtor scheduling, LoopC out-of-line found-block).
The entire 10-byte gap is one **register-allocator artifact in LoopB**: retail keeps
`begin` in ebp with two base copies (edi=result base, edx=walk) so the found-deref goes
OUT-OF-LINE (the 0x449f7f block) + reload bytes; VC7.1 keeps `begin` in a single live reg
and FOLDS both derefs into one shared `mov ecx,[reg+ecx*4]`. Behaviorally identical; not
flippable from C++ source shape (≥4 shapes tried) nor the /GS,/Oa×pragma sweep. Only lever
left is a QFE-4035 cl.exe (retail built ~11 objs with it). Faithful 287B source +
passing test preserved in scratchpad t2_probe/.

**Implication:** the load-path coordinators/deserializers are largely a byte-parity DEFER
class (reg-alloc/temp-scheduling artifacts scale with call count → the 300–740B fns will
miss more, not less). Pursue this lane as **documented-semantics + behavior-verified**
reconstruction, NOT byte-exact catalog landings, unless a QFE-4035 compiler is wired into
the harness sweep. The tractable byte-exact wins here are the small seam leaves (done: 7
CUserProfileManager fns incl. GetEmptySlotName).

## Tractability (order to reconstruct)

1. **Small / near-term** (byte-parity feasible now):
   - `CWorld::LoadWorldFromFile` 0x0049e220 (67B) — multi-vtbl-call + two calls to a
     float-returning helper 0x9e1bc0 (result discarded, likely a profiler/timer);
     mangled virtual so author as a real member. Trim trailing int3 first.
2. **Medium** (300-800B, doable, several vtbl/def-table calls each):
   `CPlayerManager::LoadGameState` (304B), `CQuestManager::LoadGameState` (352B),
   `CWorldMap::LoadLevel` (560B), `CWorld::LoadGameState` (752B),
   `Subsystem_LoadWorldMapAndNotifications` (736B), `LoadLevel_ParseObjects` (768B).
3. **Large** (1.8-2.9KB, multi-session, the actual save deserializers):
   `CWorld::LoadGameStateInternal` (2944B), `CUserProfileManager::LoadGameState`
   (2800B), `CThingManager::LoadGameState` (2272B), `CScriptBase::LoadGameState`
   (1792B).

## Known blockers / findings (2026-08-10)

- **`CNewFrontendGameComponent::LoadGame` is a codegen-artifact DEFER, not a gap.**
  Real body is 31B (the 66B manifest span over-captures a following accessor cluster
  — trim to `568bf18b06ff5010ff7424088d8e84000000c6462901e87f6840005ec20400`).
  Verified byte-recovery is BLOCKED: retail forwards the load-name via `push [esp+8]`
  (push-memory), but VC7.1 emits `mov reg,[esp+8]; push reg` for the equivalent C++
  — the documented `push [mem]` one-off reject class. Semantics ARE recovered:
  `self->vtbl[4](); self->f29 = 1; (self+0x84).Assign(name);` (helper 0x99b7d0 is a
  thiscall taking one stack arg). Its DIFFER catalog state is correct; leave it.
- **Native save metadata/load ownership is still stubbed** (`save_metadata_runtime_boundary.cpp`:
  `FableMetGetAutoSaveName` returns null, `StoreNull`). The on-disk enumeration
  and metadata read contract is no longer a data gap: `tools/save_metadata.py`
  joins the `Profile.bin` registry to validated `.sav` `HEADER` fields and
  emits the recovered row actions (`0x11` valid / `0xDC` missing or corrupt).
  The remaining seam is wiring those rows into the native C++ frontend and then
  into `CWorld::LoadGameState`.
- World load bottoms out at the level/region streaming (`CWorldMap::LoadLevel`,
  `FinalAlbion.wad`); registration requires the FinalAlbion_RT.stb common-header
  chunk (see NEW_LEVEL_ASSEMBLY.md / OpenRetailStaticMap 0xB41E50 gotcha).

## Resume

Start byte-parity work at tractability tier 1-2 through the normal crawl
(`verify_and_land`, trim over-captures first). The large deserializers and the save
enumeration seam are the multi-session core; the map above is the dependency order.

## 2026-08-10 — save-enumeration seam opened

The seam lives in **CUserProfileManager** (not a separate CSaveGameManager): it owns
save filename/display-name generation + file-list enumeration. First byte-exact
landings (RELOCATION_MATCH): `GetAutoSaveFileName` 0x00406690 and 0x004065d0 — both
are the stack-hidden-return CWideString factory shape (`return CWideString(literal)`
via ctor 0x99b6b0), same family as the GetActionName factories. So the filename
getters are directly landable; the manifest over-captures them (int3-padded 32/64B
spans = 21B real fns — trim first). Remaining cluster (un-landed, sizes in the doc
above): GetManualSaveFileName 0x406610 (128B), GetAutoSaveDisplayName 0x4069e0/0x406c20
(160B), GetEmptySlotName 0x406a80 (86B: g_13b86a0 check -> CWideString ctor 0x99ebf0 +
call 0x9c95e0 + dtor 0x99eae0, else the 0x99b6b0 factory), GetManualSaveDisplayName
0x406ae0 (320B), GetAutoSavePathName 0x406f70 (192B), GetSaveDisplayNameFromFileName
0x407e10 (448B), LoadFileList 0x4091c0 (544B). The DISPLAY-name getters are what the
frontend save list shows ("AutoSave"/"Save 1/2/3"); LoadFileList enumerates the slots.

### 2026-08-10 — display-name tier: 4 more landed (6 total in the seam)

Landed byte-exact (RELOCATION_MATCH): GetAutoSaveDisplayName 0x4069e0 + 0x406c20
(the frontend "AutoSave" display strings — two-branch: format from save-manager
singleton g_13b86a0->f14 / g_13b871c->f60 via 0x9c95e0, else literal factory
0x99b6b0), GetManualSaveFileName 0x406610 (multi-temp CWideString concat: default
ctor 0x99aed0 + format 0x99ba70 + concat 0x99b720/0x99be70 + dtors), and
GetAutoSavePathName 0x406f70 (with __security_cookie + /GS + a 0x104 path buffer
via 0x406e30 + WConcat 0x595080). Save seam now has 6 byte-exact fns.

Deferred (semantics recovered, byte-blocked — see agent notes):
- ~~GetEmptySlotName 0x406a80 DIFFER(94v88)~~ **LANDED 2026-08-11 (RELOCATION_MATCH 88/88).**
  Prior +6B was NRVO shape: modeled as `return mgr->f14->Format(&temp);` (struct-return method
  NRV-elides into the outer return slot) with `temp = CWideString(lit 0x122df80, -1)` in the
  if-branch and `return CWideString(lit 0x122df5c)` in the else. Seam now 7 byte-exact fns.
- GetManualSaveDisplayName 0x406ae0 DIFFER(393v314): 2-branch multi-temp concat +
  tail to GetManualSaveFileName; VC temp/dtor scheduling won't match.
- GetSaveDisplayNameFromFileName 0x407e10: SEH-unwind cleanup bitmask (test bl,0x10/8/4/2/1
  -> conditional dtors) not reproducible from C++; dispatches to GetAutoSave/ManualDisplayName.
- LoadFileList 0x4091c0: __security_cookie + STL vector<save-entry> iteration (rep movsd of
  0x1c-byte entries, begin/next 0x999760/0x999850); enumerates slots into the caller's vector.
