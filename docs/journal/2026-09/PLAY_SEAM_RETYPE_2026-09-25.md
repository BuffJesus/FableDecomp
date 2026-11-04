# Play-seam closure: retype batch, 2026-09-25

Continues land/boot-debake after `81a1be2`. Goal: every symbol CGame::Play's link closure
needs should be defined by a genuine landed source. `link_closure.py` from CGame::Play gave a
285-symbol frontier (53 landed-under-a-stand-in-name, 34 unlanded, 5 data, 193 other).

Five parallel agents authored; the lead landed with a re-verifying payload lander
(replacement rows re-graded before the old files are removed). Everything below is genuine
C++ at MATCH/RELOCATION_MATCH with a behaviour fixture; every row got at least two
mutation checks (one GetBestAlpha mutant survives because the table can't tell it apart).

| group | replaced stand-ins | new | notable |
|---|---|---|---|
| CMainGameComponent | 21 | 1 | 14 asm bakes gone; 0x416268 is BeginInputSaving, 0x41627F BeginInputLoading; 0x4BBC00 is the folded InitLUA |
| frontend / NUISystem | 11 | 2 | CManager / CFrontEndManager members; CEngineInterpolationInfo, CXMVPlayerBuffers ctors |
| display / surface / input | 17 | 15 | SwapScreens, CPixelFormat::Initialise de-baked; GetBest*TextureFormat family; CInputEvent |
| world / engine / misc | 22 | 16 | Game/Startup definition managers, CWorld/CWorldMap/CNetworkClient, NProgressDisplay |
| strings / files / particles | 13 | 19 | CDiskFileWin32 members, NParticleEngine lifecycle, CGameEvent getters |

Totals: 84 stand-ins replaced by genuine members, 53 new functions.

## Follow-on fixes by the lead

- `CUserProfileManager::Get` (0x40D2A0) is the FableWin name; the boot leaf now builds from
  it and the `CUserProfileManagerSingleton` stand-in is gone.
- The pixel-format table is `const CD3DFormatInfo GD3DFormats[]` (FableWin name, retail
  0x0129BA40); `fable_pixel_format_table.h` is gone. CTexture::CalcByteLength uses the real
  `CPixelFormat` (still RELOCATION_MATCH; its fixture passes in place).
- Catalog parsing: 157 entries use the one-line `Key = '...'; Key = '...'` layout;
  symbol_index.py and link_closure.py now parse both layouts.

## Not matched (sources kept in the agents' folders)

GetFrameDifferenceFromCurrent 0x4167A7 (x87 store order), Draw 0x42DF9E (register choice in
an inlined setter), InitialiseEngine 0x42E204 (local CDefPointer release epilogue),
EventScanStart 0x9F4ED0 (this in ecx vs eax), CGameEventDataPackage::GetData 0x9F18F0,
CParticleEnvironmentInteraction::GetWindVelocity 0x49BE10.

## Open for the seam

- Index the bootstrap-only genuine sources too (OpenRetailBank, string operators, …).
- Link clashes to resolve at the source level: inline copies of the 3-argument
  ConvertWorldTo* helpers, SetHasInputFocus, CWideString::UnassignString's emitter.
- Name clashes still pending: 0x13B86A0, 0x13B8648 (FableWin GOnlyBuildStaticMaps),
  0x13B8616 (GUseRetailBanks), 0x415260 (NLocalisation::GetTextBankName).
- Gate gap: RELOCATION_MATCH masks relocation targets, so two stores to different globals
  in swapped order still "match" (seen in NParticleEngine::Uninitialise). Compare the
  relocation-target sequence too.

## Verification

122/122 boot leaves; ABI gate; 0 regressions across 216 header dependents (0x9e1440,
0x9f9ee0 and 0x9fa230 fail their harness build at the base revision too: relative includes);
full bootstrap exit 0; link smoke 0 unresolved.
