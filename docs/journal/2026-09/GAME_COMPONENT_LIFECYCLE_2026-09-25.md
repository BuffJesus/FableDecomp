# Game component lifecycle recovery, 2026-09-25

Continues the reconstruction lane after boot-debake round 3. Play remains unwired;
this work closes ABI and initialization dependencies before attempting live execution.

## Vtable correction

The old shared declarations made ChangeTextureColourDepth the third device-reset
callback and omitted concrete Init/Run overrides. Constructor instruction parity
still passed because vtable relocations were masked. Retail's actual layout is:

| Class | Primary table (five slots) | Device-reset table (two slots) |
| --- | --- | --- |
| CGameComponent | 0x122ECB0 | 0x122ECA4 |
| CMainGameComponent | 0x122F180 | 0x122F174 |
| CNewFrontendGameComponent | 0x1230CA0 | 0x1230C94 |

Primary slots are destructor, Init, Run, ChangeTextureColourDepth, SetQuit.
Secondary slots are OnPreDeviceReset and OnPostDeviceReset. Run and colour depth
are pure in CGameComponent; both callbacks are pure in CDeviceResetCallback.
The callback base is at this+4. CMeshBankManager also declares its concrete callbacks.
A shared fable_device_reset_callback.h prevents the previous duplicate definitions.

`gate_game_component_abi.py` reads COFF symbols/relocations and the actual retail
vtable words. It checks complete table sizes and all 21 target identities; it rejects
the previous header (secondary table was 12 bytes rather than 8). The bootstrap now
runs this gate in addition to relocation-masked instruction checks. It does not claim
that the target implementations have all been reconstructed.

## Recovered source

- CGameComponent constructor 0x51D8E0: 49-byte RELOCATION_MATCH, including both base
  vptrs, Quit/Running false, and owning CGame pointer. RTTI and callers disprove the
  former CAIStateGroup_GazeAtHome label. Fixture checks receiver, owner, flags and padding.
- CNewFrontendGameComponent::Init 0x42F75E: 47-byte RELOCATION_MATCH; publishes this,
  loads definitions/XMV code, sets XMVCodeLoaded, initializes the console with grave
  character/key and the loaded menu font. Fixture covers fresh/existing console and order.
- InitialiseDefs 0x42F722 (60 bytes), CompileDefs 0x42F70B (23 bytes), and main
  OnPostDeviceReset 0x4175DA (11 bytes) replace assembly bakes with matching C++.
- Nine base/component lifecycle leaves now use the real shared declarations rather
  than generic T structs or assembly. Their fixtures inspect writes, preserve other
  bytes, verify the secondary-base receiver adjustment, and check return values.
  Earlier fixtures for several of these functions only printed PASS.
- FRONT_END lookup uses the PDB CFrontEndDef specialization and a named CCharString
  temporary; passing a temporary directly changes VC7.1's scheduling by three bytes.

Validation: all changed catalog leaves pass parity and behavior; both component
constructors retain RELOCATION_MATCH. Header-dependent gate: zero regressions across
18 entries. Full Release bootstrap passes, including WinMain, Stage 3 and visual boot.
The new ABI gate also passes separately and demonstrably rejects the previous header.

Next: recover the remaining constructor dependencies and real Run/Init bodies, then
connect Play. The current GFMain Play boundary is still a call counter; no playable
world-entry claim is made by these isolated checks.
