# CNewFrontendGameComponent loop recovered, 2026-09-25

Continues land/boot-debake after `54b14fc`. Five parallel agents each authored and
verified one piece; the lead landed them one at a time. Everything below is genuine
C++ at RELOCATION_MATCH with a behaviour fixture that fails under mutation.

| addr | function | bytes | notes |
|---|---|---|---|
| 0042dc94 | Update | 148 | clamps dt to 0.1f; controller-disconnect error via NUISystem::CManager |
| 0042dd28 | CleanUpInterfaces | 139 | was labelled `CEngineResourceManager::ChangeScene` |
| 0042ddb3 | InitialiseGraphics | 274 | menu font + GBANK_FRONT_END with six GetBest*TextureFormat queries |
| 0042e3ee | Input | 1441 | global GPInputManager 0x13B8388; compare trees, no jump table |
| 0042ebb6 | Shutdown | 198 | 0x4336B0 is CGameDefinitionManager::Set(0), not operator new |
| 0042ec7c | Run | 2191 | intro movies, banks, frame loop, attract mode, create-game handoff |
| 0042f50b | ~CNewFrontendGameComponent | 158 | body `if (!Quit) Shutdown();`; manifest's 535 bytes spanned two more functions |
| 004330d2 | deleting destructor | 28 | replaced a "CLandscapeBackgroundPatch" stand-in |

Also this commit: `CMainGameComponent::GetAverageFrameDuration` 0x416219 (float local
blocks the tail jump; replaced an asm bake), `CMainGameComponent::IsEditorActive`
0x4197B0 retyped, and CMainGameComponent's full 10-slot primary vtable (Update, GetInputs,
Render, PostInit, IsEditorActive after the base five), now checked by
gate_game_component_abi.py.

## What the frontend TU looks like

All of 0x42Dxxx-0x433xxx is size-optimised (`#pragma optimize("s", on)`); Input also
needs the frame pointer (`optimize("y", off)`). `~CGameComponent` is inline and empty
(retail inlines its vptr stores everywhere). `~CDefPointer` needed `__forceinline` to stay
inline under /Os. CCountedPointer copy-ctor and ReleaseData are out of line per T.

## Identities corrected

- Manager globals are CSystemManager fields cached by GFInitialise: 0x13B8388
  CInputManager* (GPInputManager), 0x13B837C CGraphicBankManager*, 0x13B838C CFontManager*,
  0x13B8394 CASoundManager*, 0x13B8390 CDisplayManager*. GFInitialise's source still calls
  them RenderDevice/TextureManager/ResourceManager/PrimitiveManager and reads them through a
  byte-model "FableEngineRoot" that is really CSystemManager. **Next:** retype GFInitialise
  and unify these declarations.
- `NUISystem::CFrontEndManager : CObserver` (not CManager); CManager is a separate 0xD0
  singleton deriving CObservable.
- 0x446BE0 is `~CGamePlayerInterface`; 0x743FE0 is the implicit destructor of
  `NCreatureMode::CModeIceTrollGroundSmashIcicleAssault`.

## Open

- Unproven names: 0x41A0C0 vs 0x41A0A0 (GetMiscDir / GetHDMiscDir), 0x41A330
  GetDVDDialogueDir, CASoundManager slot 18.
- Input declares `CInputEvent::GetMouseMovement` as returning a const reference; the PDB
  says by value, which makes VC7.1 add a copy.
- Play wiring: the runtime still has to supply the frontend's callee ring (see the agents'
  integration lists in this journal's commit) before GFMain can call the real Play.

## Verification

122/122 boot leaves; component ABI gate (5/10/5 primary slots); 0 regressions across 78
header-dependent catalog entries (0x9e1440 was already failing); full bootstrap exit 0
(WinMain, Stage 3, visual); OpenRetailBank link smoke 0 unresolved.
