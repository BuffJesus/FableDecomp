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

Resume in batch 161 and mark each reviewed address in both crawl tried ledgers before
generating batch 162. Keep the readable-source rule: semantic/PDB names when supported,
explicit offset names otherwise, and no landing on a misleading owner or prototype.
