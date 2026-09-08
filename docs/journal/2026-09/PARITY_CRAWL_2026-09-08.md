# Parity crawl continuation — 2026-09-08

Resumed `feat/script-recovery-marathon` from crawl batch 155. Thirteen genuine,
human-readable C++ functions landed with behavior fixtures and VC7.1 retail parity:

- `00A8CA00 C3DMesh2::CanRelocateData` — MATCH
- `00B91340 CEnginePrimitiveManagerOcclusionBase::RenderBoundingVolume` — RELOCATION_MATCH
- `00902CA7/00902CDC` ServeFood Home/Tavern `GetBuilding` pair — RELOCATION_MATCH
- `00548710 CUserProfileManager::IsCameraResetting` — MATCH
- `009FD3F0 CGraphicInfoFrame::GetBlankTexture` — MATCH
- `00BDC150 CEngineLandscapeMap::IsBackgroundAtFullDetailInArea` — RELOCATION_MATCH
- `00A0C8E0 CWStringParser::Init` — MATCH, using PDB field names
- `00661F20 CTCUserActionPointBase::IsActionPointFree` — MATCH
- `00487520 CPlayer::IsMainPlayer` — RELOCATION_MATCH, using PDB member names
- `00A61F40 CShaderBankStateBlock::Set` — MATCH, using PDB member names
- `00B91710 CEngineSceneGridCell::InvalidateAll` — MATCH, using the named parent walk and validity bits
- `006CECD0 CTCWife::ClearAppearanceChangedNoticed` — MATCH, using a named retail overlay for the donor base shift

Every targeted `rebuild/build_candidates.ps1 -Address ...` check passed. Batch 157's
`00C620B0 ToC2DBoxF` label is not safe to land: retail converts only two integers,
while the PDB proves both `C2DBoxI` and `C2DBoxF` contain four fields. Batch 159's
`00C8A920 GetNextRegionOnRouteTo` behavior model passes, but the retail null branch
uses plain `ret` while the manifest claims two callee-cleaned parameters; correct the
prototype/calling convention before retrying. Several repeated destructor and profile
method names are propagated aliases and were deliberately not presented as source truth.

The readability sweep then converted thirteen already-landed translation units while
preserving their gates. Five automatic conversions and three manual conversions now
use shared PDB headers. Five `CEnvironment::Peek*` accessors now express named state
members instead of anonymous `pad[0x...]` structs; the smaller retail state layout is
isolated in `rebuild/include/fable_environment_retail.h` with compile-time offset checks.

Batch 162 added four exact matches with behavior fixtures:

- `00A62A60 CPackedUIntArray::Sizeof` — named `Size`/`Bits` fields and an explicit header-plus-packed-storage formula
- `004AEF50 CPersistContext::IsBinary` — PDB-backed `Mode`; unrecovered enum labels remain numeric rather than speculative
- `00A696D0 CTextureManager::Uninitialise` — named retail overlay for the donor's larger preallocation block
- `00A028B0 CHandleBasedMemoryPool::InternalSetResource` — typed handle-array/allocation traversal rather than chained offsets

The remainder of batch 162 and batch 163 added seven more verified functions:

- `0082FE00 CGameScriptInterface::EndCutFade` — shared PDB class and named fade operations
- `00A219E0 list<long>::pop_front` — named sentinel/node unlink
- `0049D310 CTCFadeOutAndIn::IsFading` — original `FadingIn`/`FadingOut` fields
- `006C4560 CThingFilter_AICreatureHasProperty::operator()` — PDB `Property` field
- `00982DB0 CThingFilter_HasBuildingAsHome::operator()` — PDB `HomeBuilding` field
- `00A0AA30 CRenderManagerCore::SetVertexFormat` — shared manager header and typed Direct3D `SetFVF` slot
- `00B28B60 CIEngine::PeekGlobalAmbientColour` — named RGBA value type

Batch 164 began with two more verified functions:

- `005BC450 CItem::IsSameItemTypeAsDefIndex` — shared `CItem` header, including the empty-stack guard
- `008350D0 CAnimFilterAnyGroup::Clone` — genuine stateless C++ clone rather than allocation pseudocode

Batch 165 added `CCreatureAbilityDef::Copy` and `CThingGroupDef::Copy`; batch 166 added
`CEngineThemeGroupDef::Copy` and `CWaspQueenBattleDef::Copy`. All four are relocation
matches with behavior fixtures and original PDB member names. Their retail overlays
document the consistent four-byte definition-base contraction instead of hiding it in
anonymous offsets.

Batches 167 through 169 were reviewed without landings. They are dominated by identical
`GetActionName` bodies propagated onto the generic `CActionDoCreatureAction` owner; one
nearby BSim label also conflicts with the Ego_r symbol at the same address. Those rows
remain unlanded until their concrete derived owners can be established. Continue after
batch 169, keeping the readable-source rule: semantic/PDB names when supported, explicit
retail overlays for proven layout deltas, and no landing on a misleading owner or prototype.

Batch 171 added `CTCOnHeroPush::IsAngry`, using the original `DistPushed` member and
the retail threshold comparison. Batch 172 added `C3DAnimationInfo::Sizeof`, including
the counted-pointer sizing rule that distinguishes a present payload from its null slot,
and the RTTI-confirmed `CVertexShaderConstantLayoutEmpty::PeekName` accessor. Batch 173
added ten PDB-owned animation/mesh file chunk `GetChunkDescription` virtuals. All thirteen
pass behavior fixtures and relocation parity. The surrounding generic `GetActionName`
cluster and register/prototype-conflicting helpers were reviewed but not landed.
