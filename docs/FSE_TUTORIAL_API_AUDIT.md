# AlbionSecrets FSE tutorial API audit

## Provenance

`ALL-INTERFACE-FUNCTIONS-FOR-FSE.h` was posted by **AlbionSecrets** on 2025-11-05 as the tutorial
reference for all Fable Script Extender functions. AlbionSecrets (`eeeeeAeoN`) is the original FSE
developer. This source is distinct from the custom quest-layer scripts later posted by
**odarenkoas**.

The audit used the supplied header, `refs/fse_api_manifest.json`, and AlbionSecrets' local upstream
FSE source under `FableScriptExtender-master`. The header is the signature/overload reference; the
manifest is a normalized catalog with one record per Lua-facing scope/name.

## Result

- Header declarations: **947**
- Unique Lua-facing function names: **933**
- Normalized manifest records after correction: **933**
- Scope split: **88 Entity**, **845 Quest**
- Additional overload declarations: **14**
- Header-only names after correction: **none**
- Manifest-only names after correction: **none**

The former 931-record manifest omitted two real Quest APIs:

- `StartAmbientConversation`
- `AddLineToConversation`

Both appear in the tutorial header and are registered in `LuaManager.cpp` by the current local FSE
source. They are therefore callable API surface, not speculative native candidates.

## Overload caveat

The manifest deliberately normalizes overload families to one scope/name record, so it does not
fully preserve the header's overload signatures. The affected names are `AddGossip`,
`AddGossipFactions`, `AddGossipVillages`, `CameraMoveBetweenLookingAt`, `CameraUseCameraPoint`,
`GetActiveGossipCategories`, `GetGossip`, `GetGossipFactions`, `GetGossipVillages`,
`GetIsGossipForPlayer`, `MakeHeroCarryItemInHand`, and `NewScriptFrame`.

Use `tools/audit_fse_tutorial_api.py` to repeat the name-set audit against a supplied copy of the
tutorial header. Consult the header itself when exact overload selection matters.
