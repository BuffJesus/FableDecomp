# Auto-RE terrain queue completion

Completed: 2026-07-22 10:47:21 -06:00

Primary: **12 PASS / 4 FAIL**. Failure-only retry: **2 PASS / 2 FAIL**. Missing records: **0**.

| Address | Function | Primary | Retry | Final |
|---|---|---:|---:|---:|
| 0x00BFE050 | CLandscapeLayerMesh::LoadForeground | FAIL | PASS | PASS |
| 0x00BF3B60 | CEngineLandscapePatch::LoadForegroundPatch | FAIL | FAIL | FAIL |
| 0x00BE7D70 | CLandscapeBackgroundPatch::Load | FAIL | FAIL | FAIL |
| 0x00BE8920 | CLandscapeBackgroundPatch::LoadCompressed | FAIL | PASS | PASS |
| 0x00BF3A90 | CEngineLandscapePatch::LoadHeader | PASS | - | PASS |
| 0x00BEBED0 | CEnginePrimitiveManagerMeshBase::GetComposedTexture | PASS | - | PASS |
| 0x00BEC000 | CEnginePrimitiveManagerMeshBase::PreloadDiffuseTexture | PASS | - | PASS |
| 0x00BEC470 | CEnginePrimitiveManagerMeshBase::PreloadBumpTexture | PASS | - | PASS |
| 0x00BEC950 | CEnginePrimitiveManagerMeshBase::GetOverlaidTextures | PASS | - | PASS |
| 0x00BEAAA0 | CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures | PASS | - | PASS |
| 0x00BEB4D0 | CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures | PASS | - | PASS |
| 0x00BF5C20 | CEngineLandscapeRenderer::GetForegroundSpotLightVertexShader | PASS | - | PASS |
| 0x00BF5C60 | CEngineLandscapeRenderer::GetForegroundSpotLightVertexShader | PASS | - | PASS |
| 0x00BF5CA0 | CEngineLandscapeRenderer::GetForegroundBumpShadowedVertexShader | PASS | - | PASS |
| 0x00BF5CD0 | CEngineLandscapeRenderer::GetForegroundBumpShadowedVertexShader | PASS | - | PASS |
| 0x00BF5D00 | CEngineLandscapeRenderer::GetForegroundBumpShadowedVertexShader | PASS | - | PASS |
