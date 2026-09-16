# New Oakvale readable Lua running through stock FSE + NoviCompatibility sidecar — 2026-09-14

## Outcome
`work/new-oakvale-original-fse-20260912/local-candidate-v3` launches retail Fable with the
**unmodified community FSE** (`FableScriptExtender.dll`, empty registry) plus the
**NoviCompatibility.dll sidecar**, and runs the **converter readable Lua verbatim**
(`work/oakvale_readable_playtest_fix2_20260915/FSE` == `refs/script_recovery/lifted/NewOakValeIntro/readable/FSE`
+ `retail_override.lua`). First live run: override armed, chained hook fired, PartyMode overlay active,
NewOakValeIntro Init/Main clean, all 16 entity hosts Init+Main clean, 4 quest threads registered,
persist pass clean, Bully hit/health-bar reactions firing. **Zero Lua runtime errors.**

## Root causes of the evening's "adult instead of child / no NPC behaviour / no audio"
1. The sidecar DLL in `sidecar-source`/`sidecar-final-source` was a ForgeFSE snapshot **without** the
   scalar-ABI work, so the readable Lua's resource API (`SetInitialOakvaleObjective`,
   `TurnOakvaleHeroIntoChild`, `InitializeTeddyGirlActor`, `RegisterBoundConsciousCondition`,
   `IsHeroNearBarrelGuard`, ~170 methods in total) resolved to nil and quest Init/Main aborted before
   the child transition. Ad-hoc shims could never cover that surface.
2. The candidate bundle registered `Q_NewOakValeIntro` as a **custom quest** in `quests.lua`. The
   readable package is designed for the **retail-override** path (`retail_override.lua` replaces the
   native `Q_NewOakValeIntro` allocator; `quests.lua` holds only PartyMode). Registering it as a custom
   quest also lets the stock FSE registry double-register it.
3. Copying `ForgeFSE-retail-shadow` files without the chain include produced a DLL with no
   `NoviCompatibilityStart` export → launcher status 4.

## What fixed it
`work/oakvale_entity_scalar_abi_integration/oakvale-entity-scalar-abi.patch` applies cleanly **only** to
the `D:\Code\ForgeFSE-retail-shadow` working tree. Build recipe (now `tools/script_recovery/build_novi_compat_bundle.py`):
canonical source → apply patch → re-apply 3 sidecar deltas (base path `/NoviCompatibility`, disable
map-alias/startup-alias/shadow-runner calls, `NoviCompatibilityChain.inl` replacing MyHook/InstallHook/DllMain)
→ MSBuild Release|x86 (dash switches) → bundle with stock FSE + readable package.
Patched tree: `work/new-oakvale-original-fse-20260912/sidecar-abi-v2`.
API-gap check (`resources:`/`quest:` method names in the readable Lua vs C++ bindings): 171/171, 97/97 bound.

## Known non-blocking noise
- Sidecar log warns `FinalAlbion.qst not found at .../NoviCompatibi/data/...`: the quest-registry
  self-heal derives the game dir by stripping 4 chars (`/FSE`) from the base path. Only affects custom-quest
  activation, which the override path does not use.

## Not yet verified (needs the user at the keyboard)
Child-age start after New Game, Father scene, Escape, deed branches, Theresa, raid, save/reload, audio.
Log the run's `NoviCompatibility/FableScriptExtender.log` with any report.
