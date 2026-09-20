# Sidecar DLL bindings for the GuildTraining + TraderConflict units (2026-09-19)

The converter-generated Lua for GuildTraining (`refs/script_recovery/lifted/GuildTraining/readable_converter/FSE`)
and TraderConflict (`refs/script_recovery/lifted/TraderConflict/readable/FSE`) called 13 methods the playtest DLL did
not bind (Lua: "attempt to call a nil value" at the call site). All 13 are now bound in the scratch sidecar repo
`work/new-oakvale-original-fse-20260912/sidecar-abi-v2` (commit `a7b6568`, on top of `05c56fc`), the DLL is rebuilt,
and the delta is regenerated into `tools/script_recovery/sidecar_patches/novi-unit-bindings.patch`.

Two of the 13 (`StateListSet`, `GetStateListCopy`) were already in the patch file but had never been committed to the
sidecar source — the patch had been hand-edited ahead of the repo, so the DLL never contained them. They are now in
`NoviUnitBindings.h` proper and the patch is a pure `git diff 6e19dfd HEAD` again.

## Evidence rule used

Every slot was confirmed from two independent sources: (a) the FSE DLL's own typedef + slot index
(`GameInterface.cpp` `pVTable[n]` / `EntityScriptingAPI.h` `CScriptThingVTable` offsets, mirrored in
`refs/script_recovery/typing/gsi_prototypes.json`), and (b) the retail vtable bytes read straight out of `Fable.exe`
(section-mapped) resolved through `ghidra_out/labels_rtti_port.tsv` mangled names. GhidraMCP was not running
(`list_instances` empty), so (b) replaced the live lookup:

```
CScriptThing vftable 0x1238C8C          CGameScriptInterface vftable 0x1260F0C
  0x54 0x4aab30 ?MsgIsHitBy@CScriptThing@@UBE_NABVCCharString@@@Z
  0xa8 0x4aad40 ?MsgIsHitByAnySpecialAbilityFrom@CScriptThing@@UBE_NABVCCharString@@@Z
  0xbc 0x4aade0 ?MsgHitFriendWithMeleeWeapon@CScriptThing@@UBE_NXZ
  0xc4 0x4aae20 ?MsgHitFriendWithRangedWeapon@CScriptThing@@UBE_NXZ
  0xcc 0x4aae60 ?MsgHitFriendWithBareHands@CScriptThing@@UBE_NXZ
  0x108 0x8905e0 ?IsPlayerHoldingLockTargetButton@CGameScriptInterface@@UBE_NXZ
  0x10c 0x890610 ?IsPlayerHoldingFireRangedWeaponButton@CGameScriptInterface@@UBE_NXZ
  0x114 0x890670 ?IsHeroInProjectileWeaponMode@CGameScriptInterface@@UBE_NXZ
  0x190 0x89f9e0 ?CreateEffect@...@@UBE?AVCScriptThing@@ABVCCharString@@ABVC3DVector@@0M_N2@Z   (at position)
  0x194 0x89f910 ?CreateEffect@...@@UBE?AVCScriptThing@@ABVCCharString@@ABV2@00_N2@Z           (on thing)
  0x598 0x892070 ?TextEntryExists@CGameScriptInterface@@UBE_NABVCCharString@@@Z
  0x9b4 0x890a50 ?EntitySetAsOpinionSource@CGameScriptInterface@@UBEXABVCScriptThing@@J@Z      (long)
  0x9b8 0x890a20 ?EntitySetAsOpinionSource@CGameScriptInterface@@UBEXABVCScriptThing@@ABVCCharString@@@Z
```

## Bindings (what / why / evidence)

| Lua | Native | Body | Call sites |
|---|---|---|---|
| `quest:IsPlayerHoldingLockTargetButton()` | GSI 0x108, `bool() const` | `IsPlayerHoldingLockTargetButton_API(gsi)` (the DLL already resolved the pointer, nothing bound it) | TraderConflict intro 6 |
| `quest:IsPlayerHoldingFireRangedWeaponButton()` | GSI 0x10C | same shape | GuildTraining ranged tutorial 5 |
| `quest:IsHeroInProjectileWeaponMode()` | GSI 0x114 | same shape | GuildTraining 2 |
| `quest:TextEntryExists(key)` | GSI 0x598, `bool(const CCharString&) const` | `FableString` key → `TextEntryExists_API` | TraderToRescue 2 (see gap below) |
| `quest:EntitySetAsOpinionSource(thing, source)` | GSI 0x9B8 (string) / 0x9B4 (int) | dispatches on the Lua type of `source`, the native overload pair; bodies = `LuaQuestState::EntitySetAsOpinionSourceByString/ByInt` | TraderConflictEvil 1 + GuildTraining 1, both strings |
| `quest:CreateEffect(result, name, where, a4, a5, indep, always)` | GSI 0x190 / 0x194 | `where` table + `a5` number → at-position (`a4` = scriptName, `a5` = angle); `where` thing + `a5` string → on-thing (`a4` = bone, `a5` = scriptName). Bodies = `LuaQuestState::CreateEffectAtPos/OnThing`. `result` is the hidden-return slot: when a thing handle is passed it is assigned (destroy + copy-construct) and the handle is also returned | GuildTraining PreMelee 3 (see gap below) |
| `quest:StateListSet(name, table)` / `GetStateListCopy(name)` | GSI out-argument fill / `std::vector<CScriptThing>` copy ctor | as in the patch file (side-table lists, `NoviCopyThing` per element) | TraderConflict 3 + 1 |
| `thing:MsgIsHitBy(name)` | CScriptThing 0x54 | identical to `LuaEntityAPI::MsgIsHitByHero`, which passes `"SCRIPT_NAME_HERO"`; the name is forwarded as a `FableString`. Retail `CGameScriptThing::MsgIsHitBy` 0x8D0FB0: empty name = any `CEventHitBy` in the frame window, a non-empty name adds `CIsHitEventHitByThingWithScriptName` | GuildTraining 6 (`hero:MsgIsHitBy("MeleeOpponent")`), TraderConflict 7 (`""`, `"TC_BanditFighter"`) |
| `thing:MsgIsHitByAnySpecialAbilityFrom(name)` | CScriptThing 0xA8 | identical to `MsgIsHitByAnySpecialAbilityFromHero` with the name forwarded | TraderConflict 4 |
| `thing:MsgHitFriendWithMeleeWeapon()` / `...RangedWeapon()` / `...BareHands()` | CScriptThing 0xBC / 0xC4 / 0xCC, `bool() const` | direct vtable call, null-guarded like the siblings | GuildTraining 1 each, on `hero` |

No behaviour was invented: each body is the sibling binding's body with the hard-coded operand replaced by the
Lua argument, or a direct call of the already-declared slot.

## Converter gaps found while binding (the readable output, NOT the DLL)

- `TraderToRescue.lua:268/503`: `quest:TextEntryExists()` — the key (`scratchValue29`) and the result store
  (`scratchValue10`) were dropped by the readable pass. The binding takes `sol::optional<std::string>`; a call without a
  key logs `TextEntryExists called without a key (converter gap)` and returns false instead of raising.
- `TheRealGuildmaster.lua:249/1274/1756` (GuildTrainingPreMelee): `quest:CreateEffect(thing, "SMASH_DUMMY_01",
  actorMap, "", 0.0, false, false)` — the native is the at-position overload (the `CStack_114 = *puVar11` C3DVector copy
  right above is the position), but the third operand was resolved to an actor-map id / an unrelated thing. The binding
  logs `third operand is neither a position table nor a thing (converter gap)` and creates nothing at those sites.
  Fix belongs in the lowering pass (`puVar11` = the dummy's position vector), not here.

## Commands

```
# build (VS2022 MSBuild, same switches as build_novi_compat_bundle.py)
"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" ^
  work\new-oakvale-original-fse-20260912\sidecar-abi-v2\FableScriptExtender.sln -t:Build -p:Configuration=Release -p:Platform=x86 -m -nologo -v:m
#   -> Release\FableScriptExtender.dll  5,719,552 bytes  sha256 51baaa6a2be4ba580fbb8a7b6759b782d4e3f8c5d8443f9f8bd30c883295ed57
#      (0 warnings; log: sidecar-abi-v2\build-bindings3.log)
# patch (from the sidecar repo)
git diff 6e19dfd HEAD -- FableScriptExtender/LuaManager.cpp FableScriptExtender/NoviUnitBindings.h > tools/script_recovery/sidecar_patches/novi-unit-bindings.patch
#   verified: git apply --check of both sidecar_patches on a 6e19dfd worktree is clean
# static check
python .scratch_tmp\api_check.py work/new-oakvale-original-fse-20260912/local-candidate-v6/NoviCompatibility   -> missing methods: 0
```

Not done on purpose (per the task): the playtest bundles (local-candidate-v5/v6) were not rebuilt, the converter units
were not touched, the game was not launched. The next bundle build (`build_novi_compat_bundle.py`) picks the patch up.
