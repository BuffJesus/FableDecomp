# New-Profile screen (name field + settings) — decomp / parity / purity

The profile-creation screen reached from New Game (`GotoNewProfileScreen @ 0x00595845`):
a name field plus the default game settings (volumes, brightness/gamma, toggles). Backed
by **CUserProfileManager**, which owns profile create/name/settings/save. Parity is mostly
DONE; this documents the flow, the recovered settings layout, and purity.

## Flow
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

## Parity status
**Landed byte-exact (8):** `IsEnoughSpaceForProfile` 0x004067c0, `SetProfileName` 0x0040d400,
`CreateNewProfile` 0x004945b0, `ResetSlidersToProfileValues` 0x00595356,
`AddProfileNameToScreens` 0x00595b24, `SetScoreboardOptionsFromProfile` 0x005963db,
`GetScoreboardOptionsToProfile` 0x00596550, `GotoProfileMenu` 0x00597b20.

**Deferred (semantics recovered, behaviour-verified, byte-blocked):**
- `ApplyNewProfileValues` 0x0040cca0 (535B, DIFFER 535v535, 80 masked-byte diff) — 33
  staged→active setting copies each followed by a `this->Apply()` notify (0x407370), one
  conditional (`if this->f78: 0x408820`) and a tail call (0x40a740). The gap is the
  register **rotation** across the 33 copies (retail cycles eax/ecx/edx as scratch and
  varies the `mov ecx,esi` placement); not source-reachable. Full copy map recovered below.
- `GetDirectoryNameFromProfileName` 0x004071c0 (353B) — `__security_cookie` + multi-temp
  `CWideString` concat (0x99ebf0/99eae0/99aed0/99b6b0/99b510/99bff0…). The temp/dtor
  scheduling defer class (same as the CONTINUE_GAME_PATH save-display-name getters).
- `GetValidProfileName` 0x00406350 (629B, 48 calls) — string validation, same tier.
- `SetDefaultValuesForNewProfile` 0x00409b70 (1105B, FPU) — clean const stores up front,
  then x87 (fild) + branches; the FPU tail is the fiddly class.

## Recovered profile-settings layout
Two parallel blocks in the profile object: **staged** (screen-edited, `+0xcd..0x134`) and
**active** (in-use, `+0xd..0x7c`). `ApplyNewProfileValues` copies staged→active and notifies.

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

(Copy order is not offset-sorted — see the source; `+0xec` copies before `+0xe8`, the
+0x124/+0x128 pair interleaves, etc.)

## Purity
All landed functions are **byte-exact reconstructions of the retail code** that read/write
the real profile object fields (no baking) — purity is inherent. The name field flows
through the engine's own `SetProfileName`/`GetValidProfileName`/`GetDirectoryNameFromProfileName`;
settings flow staged→active via `ApplyNewProfileValues` and persist through
CUserProfileManager's save path (the save seam, CONTINUE_GAME_PATH.md §save-enumeration).
No values are hardcoded outside `SetDefaultValuesForNewProfile`, which is the retail default
table itself.

## Next
- Verify the settings labels against the on-screen sliders (VISUAL_PARITY_STATUS.md harness)
  to name each field.
- The 4 deferred functions are behaviour-correct; leave as documented-semantics unless a
  register-level permuter lands (docs/QFE4035_COMPILER_GATE.md escalation ladder).
