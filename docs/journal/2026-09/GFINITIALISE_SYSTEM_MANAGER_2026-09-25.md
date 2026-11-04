# GFInitialise reads the real CSystemManager, 2026-09-25

Continues land/boot-debake after `f248b60`. Retypes, no new addresses.

## Identities

- GFInitialise's "engine root" (`AddChildPrimitive()`, a 0x88-byte byte struct) was the
  CSystemManager singleton at 0x013CA618, returned by `GFGetSystemManager` 0x009A4EC0.
  The cached globals are its counted-pointer members:
  0x13B8394 `PSoundManager`, 0x13B8390 `PDisplayManager`, 0x13B8388 `PInputManager`
  (FableWin `GPInputManager`), 0x13B838C `PFontManager`, 0x13B837C `PGraphicBankManager`.
  The old names (PrimitiveManager, RenderDevice, ResourceManager, TextureManager) are gone.
- 0x13B8384 is `PDisplayManager->PRenderManager` (CRenderManager2D*).
- The "display dimensions" are texture limits: `D3DCAPS9.MaxTextureWidth/Height` of the
  render manager's `D3DDeviceCaps` (Ego_r CRenderManagerCore +0x3A58, so +0x3AB0/+0x3AB4),
  capped by 0x137546C. Now `g_FableMaxTextureWidth_013B7D64` / `Height_013B7D60`.
- CDisplayManager +0x1C8 is `RefreshRate`, not a width: CreateDevice 0x009BF7E0 stores it
  and divides by it for PresentationIntervalTime at +0x1D4 (Ego_r agrees through +0x1D4).
  So 0x137544C, written on success, is half the refresh rate (`g_FableHalfRefreshRate`).
  Nothing else in retail references it by absolute address.

## Changes

- fable_d3d9.h gains SDK-shaped `D3DCAPS9` (size/offset asserted); CRenderManager2D
  carries `D3DDeviceCaps`; CDisplayManager models `PRenderManager` (+0x08) and
  `RefreshRate` (+0x1C8) plus `IsTextureBitDepthSupported(unsigned long)` (PDB `_NK`).
- `GFGetSystemManager` returns `&g_SystemManager_013CA618` instead of the baked retail
  address (still RELOCATION_MATCH). Runtimes alias that symbol to boundary storage:
  system_manager_runtime_boundary (complete builds), stage2 (phase 9/10 builds), and the
  progress-phase test; the visual runtime keeps its own GFGetSystemManager but its
  storage is now the full 0x180 bytes.
- gfinitialise_engine_boundary installs its stand-in display/render managers into the
  singleton instead of returning a fake root.

## Verification

122/122 boot leaves (GFInitialise fixture now covers the requested-depth path, small
device caps and the success-only refresh cache); 0 regressions across 61 dependents;
full bootstrap exit 0; link smoke 0 unresolved.
