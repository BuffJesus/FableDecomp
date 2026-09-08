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

Batches 174 through 178 added five more functions. `CTCHeroExperience::ApplyPenaltyForDeath`
and `CFadeInFadeOutBase::IsOpen` are exact matches using original state names.
`CTCTargetingPlayer::SetAsTargetNextThingInDirection`, `CConsoleCommandLine::Reset`, and
`CLandscapeBackgroundLoadTask::ReleaseFileBlockLoad` are relocation matches with typed
state and independently identified callees. Layout-conflicting landscape, quadric, and
definition aliases were rejected; the readable `FtoL` model remains unlanded because it
does not reproduce retail's explicit x87 store sequence.

Batches 179 through 182 added three functions. `CThingObject::ResetDieOffTimer` and
`CMusicManager::ResetMusicEntriesOffsets` are exact matches using the PDB-named nested
definition and registered-music fields. `CMemoryFile::Clone` is a relocation match with
a documented 0x24 retail overlay versus the 0x28 donor class. The animation-component
allocation cluster remains unlanded because its propagated member prototypes are actually
static no-argument factories; the typed physics bitfield model is behavior-correct but
does not yet reproduce retail's combined 32-byte update.

Batches 183 and 184 added five relocation matches. Four definition serializers now use
their decorated PDB signatures and original members: `CTavernTableDef::Transfer` writes
`AddTankards` and `HighQualityTankards`; `CClockDef::Transfer` writes `Sound` and
`HandType`; `CRumbleDef::Transfer` writes `QuakeIntensities` and `QuakeDurations`; and
`CCarryingDef::Transfer` writes `AvailableCarrySlots` and `OverriddenDummyObject`. The
last three use explicit 12-byte retail container overlays where the donor container is
16 bytes. `CTCRegionDisplay::IsDrawing` expresses the original `CurrentAlpha` and
`CurrentAreaName` condition. Manual prototype corrections reject the four corrupted
`CEngine *, C3DBoundingBox *` guesses. Batch 185 is active: the typed navigator position
accessor passes behavior but remains unlanded at 36 versus 33 bytes, and the propagated
`PayFines`/callee labels were rejected where their bodies contradict PDB layouts.

Batches 186 through 195 added 68 verified functions. Four are byte-identical readable
helpers: `CTCHeroRangedAttacks::ResetProjectileTargetingZoomParameter`,
`GFGet2DDistance`, `NPlayerGui::GFToScreen`, and
`CTCHeroStats::HasMarriageInState`. Three PDB-owned mesh/animation chunk writers use
typed `WriteULONG`/`WriteFloat` calls rather than the poisoned `WriteCBYTE` labels.
`CDrunkennessDef::Transfer` uses the retail `DrunkennessThresholdMult` schema literal
and a corrected `CPersistContext&` prototype. The remaining sixty functions are
PDB-owned creature/combat action `Clone` methods recovered as ordinary
`new Derived(*this)` copy construction with verified retail allocation sizes. Every
landing passed its focused behavior fixture and selected VC7.1 build; relocation
matches preserve the allocator, copy-constructor, string, or stream call sites.
Batch 196 is active and continues the same constructor-verified clone seam.
### Batches 196-200: action-clone continuation and typed field leaves

The marathon continued past checkpoint `1898ef4` through batches 196-200. Retail allocation immediates and PDB owners supported 39 additional ordinary action copy-clones; every clone uses `return new Derived(*this)`, passed its behavior oracle, reproduced retail under relocation masking, and passed the selected VC7.1 build gate. Batch 199 also recovered `CDrawGuildSeal::ClearMessages` as the readable `while (FirstMessage) RemoveMessage(0)` loop. Batch 200 recovered the exact 35-byte `CTCInventoryBase::SetInventoryAnimRenderBox` field copy using the shared `C2DBoxF` member names and an explicit retail `+0xbc` overlay.

Ambiguous batch-198/199/200 rows were not promoted merely because a plausible behavioral implementation compiled. In particular, the two `SetAsKeyboardKeyHeld` rows disagree with their propagated double-parameter prototype and stack cleanup, three `DestroyWaterGenerator` labels cover differing layouts/callees, and the `CMemoryAllocatorFixedSize::Validate` row carries a contradictory `CEngineLandscapePatch*` receiver. Plausible implementations of `CWorldMap::GetLevelScriptName`, `EnlargedUnsignedDivide`, `ResetGlobalScriptDef`, `_Fill_n`, and `CTCHeroStats::SetMaxStamina` passed behavior checks but failed retail parity, so remain unlanded. Batches through 200 are ledgered; batch 201 is active.
