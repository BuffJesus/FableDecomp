# EgoMP donor audit

Source: [`98thrxse/egomp`](https://github.com/98thrxse/egomp) at
commit [`816e58f9a0acd099ac8289b6a125bbf95c590b4b`](https://github.com/98thrxse/egomp/commit/816e58f9a0acd099ac8289b6a125bbf95c590b4b),
dated 2026-05-19.

## Useful evidence

The strongest boot-adjacent evidence is
[`Core/SDK/Fable/MainGameComponent.cpp`](https://github.com/98thrxse/egomp/blob/816e58f9a0acd099ac8289b6a125bbf95c590b4b/Core/SDK/Fable/MainGameComponent.cpp):

- `0x013B86A0` is treated as the `CMainGameComponent*` singleton slot.
- `CMainGameComponent + 0x14` is treated as the text-bank member, independently
  corroborating the progress-display object slot already used here.
- `CMainGameComponent + 0x1C` is treated as `CPlayerManager*`.
- `CMainGameComponent + 0x24` is treated as `CWorld*`.
- Runtime hooks use `Init @ 0x004184BD`,
  `PostInit @ 0x00416953`, `Update @ 0x00418289`, and
  `Shutdown @ 0x004175E5`. These addresses agree with the corresponding
  identities already present in this reconstruction.
- EgoMP runs post-init and update callbacks after the retail method and
  shutdown callbacks before the retail shutdown method. This is useful
  lifecycle-order evidence when the reconstructed process reaches the full
  main-game component.

Other corroborated address and ABI leads:

- [`CCharString`](https://github.com/98thrxse/egomp/blob/816e58f9a0acd099ac8289b6a125bbf95c590b4b/Core/SDK/Fable/CharString.cpp):
  constructor `0x0099EBF0`, character-array conversion `0x00403A10`.
- [`CDefStringTable`](https://github.com/98thrxse/egomp/blob/816e58f9a0acd099ac8289b6a125bbf95c590b4b/Core/SDK/Fable/DefStringTable.cpp):
  singleton object `0x013CA828`, `GetString @ 0x009D49B0`.
- [`CDefinitionManager`](https://github.com/98thrxse/egomp/blob/816e58f9a0acd099ac8289b6a125bbf95c590b4b/Core/SDK/Fable/DefinitionManager.cpp):
  singleton slot `0x013B879C`,
  `GetDefGlobalIndexFromName @ 0x009AD410`.
- [`SDK`](https://github.com/98thrxse/egomp/blob/816e58f9a0acd099ac8289b6a125bbf95c590b4b/Core/SDK/Fable/SDK.cpp):
  game allocator/operator new `0x00BFEA1A`.
- [`CWorld`](https://github.com/98thrxse/egomp/blob/816e58f9a0acd099ac8289b6a125bbf95c590b4b/Core/SDK/Fable/World.cpp):
  `CWorldMap*` at `+0x14`, region-load status at `+0x104`, start position
  at `+0x108`, and start angle at `+0x114`. Its hook addresses corroborate
  `SetAsPaused @ 0x0049D8F0`,
  `SetAsLoadingRegion @ 0x0049E2C0`,
  `HandleMoveHeroToRegionGameEvent @ 0x0049EAF0`,
  `UpdateRegionLoad @ 0x004A3740`, and
  `PostRegionLoad @ 0x005064C0`.

These are address/layout leads, not imported implementations. Each must still
be checked against retail bytes, Ghidra, and the Ego PDB before entering a
canonical header.

## Important limits

EgoMP injects a DLL into a suspended retail `Fable.exe` and hooks hardcoded
absolute addresses through MinHook. It does not reconstruct the executable's
startup. The repository contains no D3D9 renderer, texture, archive, bank, or
resource-loading implementation; the launcher is only a
`CreateProcessW`/`LoadLibraryW` injection harness.

It also has no executable hash/version guard or signature scanning. Several
runtime labels conflict with byte-proven identities in this reconstruction:

| Address | EgoMP label | Current byte-backed identity |
|---|---|---|
| `0x00413120` | `CGame::Initialise` | `GFInitialise_SetupProgressDisplay`, 128 bytes |
| `0x00487DC0` | `CPlayer::GetPControlledCreature` | 8-byte STL constructor identity |
| `0x004C73D0` | `CThing::GetPos` | `CVertexBufferWin32::DoSizeof`, 7 bytes |
| `0x0066FB20` | `CTCCoopSpirit::GetScore` | `CWorld::DrawGetEnvironment` |

Those conflicts make EgoMP a valuable hypothesis generator, but not an
authoritative naming donor.

## Licensing boundary

EgoMP is GPLv3. Do not copy its source or vendored binaries into this
reconstruction without an explicit compatible licensing decision. Factual
addresses, independently verified layouts, and independently written
implementations are the safe default.
