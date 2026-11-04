# Native recreation live status 20: progress toward a visual launch

User requested a concrete assessment of visual-launch progress. Native recovery remains distinct from Lua recovery. Pending collector and acceptance preparation was put on hold for this assessment; no producer, compiler, GPU test, game launch or canonical file change occurred.

## Evidence-backed milestones

| Milestone | Evidence and current limit |
| --- | --- |
| Authored frontend checkpoint | Existing `rebuild/build/bootstrap-Release/FableTLC-Reconstruction-VisualCheckpoint.exe`, last written October 5. Its implementation is a flagged authored frontend path; it does not establish native recreation boot. The executable was not launched during this assessment. |
| Recovered rendering on a real GPU | Saved October 10 `native-indexed-raster.json` reports seven images and 114688 verified pixels through 17 recovered providers. Device, window, upload, geometry, readback and clock are harness boundaries. |
| Real retail content through that renderer | October 10 `native-retail-title-raster.json` reports PASS: title texture id 3 from frontend.big, 46080 output pixels compared exactly against an independent decoder, 18 omission checks. Root viewed retail-title.png and verified its saved report hash. The native LoadGraphicFrame path and frontend presentation loop are explicitly excluded from that pass. |
| Self-contained native startup/frontend frame | Still unproved. Latest saved broad native closure, `native-threaded-file-priority-clean-20261010`, contains 1808 sources, 445 unresolved symbols, and an empty duplicate report. This is the saved October 10 result, not a fresh October 12 build. |
| Live world/hero rendering | Still unproved; actual world construction and rendering providers remain absent from the current index. |

The earlier chat statement that no rendered frame existed was too broad. A genuine recovered-renderer GPU asset test exists. What has not been demonstrated is an integrated frame reached through native recreation startup.

## Confirmed gaps

The latest broad linker frontier was cross-checked against the current symbol index. All of the following remain without indexed providers:

- Asset loading: `CGraphicDataBank::LoadGraphicFrame`, `CSurface::Lock`, `CSurface::CopyFromUnsignedDataToSignedSurface`.
- Font lifetime: the static and streaming font bank constructors.
- Frontend integration: `NUISystem::CFrontEndManager::Init`, `BuildComponents`, `Update`, and `CSprite::DrawWithRenderManager`.
- World rendering: `CWorld` constructor, `CDisplayEngine::DoRender`, `CEngine::Render`, `CEngineScene::RenderScene`, and `CWorld::RenderUpdate`.

Current `stage2_engine_boundary.cpp:1155` defines `FableGFMainPhase10PlayBoundary` as a call counter only. Its visual branch at line 1405 invokes `FableRunVisualBootCheckpoint`; without that macro it returns zero. The genuine C++ `CGame::Play` body exists separately but this bootstrap seam does not call it.

The 445 unresolved symbols are from a broad game dependency closure and include globals and platform imports. They are not 445 equally weighted missing functions and are not a measured minimum frontier for the first native frontend frame. No visual-readiness percentage can be derived from them or from accepted shared-header contexts.

## Practical assessment

The project has demonstrated executable scaffolding, real GPU drawing through recovered renderer components, and correct drawing of one retail asset. It remains before the integrated native frontend-launch milestone. It is not at a final smoke-test-only stage: native image loading, font/UI ownership and startup-to-frame orchestration are substantial unresolved work. A live world/hero requires additional world and scene integration beyond that first frame.

The recent shared-find regression work is a prerequisite for safely promoting shared native dependencies, not a direct visual milestone. Acceptance through context 980 and the pending 44-strict/six-name-only next batch should be reported as supporting validation, never as a visual completion percentage.

For future visual-directed continuation, measure success by a frame reached through genuine startup, native asset loading and recovered draw/presentation providers; trace the smallest dependency set for that path rather than treating every broad-game linker entry as equally urgent. Existing failed source candidates and capped startup retries remain preserved. Older hero-roadmap claims of zero device-layer recovery are historical and superseded for the bounded drawing/resource providers by the October GPU evidence, without implying a complete reconstructed device lifecycle.
