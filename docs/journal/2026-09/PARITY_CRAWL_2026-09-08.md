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
### Batches 201-207: PDB field serialization, engine flags, and container loops

Batches 201-207 added twelve readable retail-parity functions after checkpoint `5b857fe`. Exact matches recovered `CInputProcessCreatureMovementWatchForControlAngleChange::Init` with PDB member `LastJoyDirection`, `COptimisedFace::RemapIndex` with `VertexIndices`, both `CEngine::Enable*SceneRendering` bit updates, and `CAIBrain::SetAllStateGroupsEnabled` over the PDB `StateGroupList`/`Enabled` fields. Relocation matches recovered three PDB-field `TransferBinaryOut` methods, `C3DAnimFileTimingEventChunk::WriteToFile`, `CStopWatch::GetTicks`, ordinary `pair<C3DVector,CCharString>::operator=`, and `CCombatWheel::ResetRings`.

Manual signature corrections reject the propagated `CWADFile` and `CGraphicDataBank` receivers where decorated symbols prove `Init()` and `TransferBinaryOut(CDataOutputStream&)`. Repeated `GetCategoryIdentifier`, `IsAnalogueLoading`, `SetAsKeyboardKeyHeld`, `OnDie`, and deleting-destructor labels with contradictory offsets or cleanup remain rejected rather than promoted. Batches through 207 are fully ledgered and batch 208 is active.
### Batches 208-214: geometry, definition overlays, and document parts

Batches 208-214 added seven readable retail-parity functions. Exact matches recovered `CPlane::Initialise`, `C2DBoxI::ValidateOrientationAndFlip` (using a readable `SwapLong` helper to preserve retail alias/load order), `CTCBuyableHouse::MovedInNotification` as the PDB-named move-in-day minimum, and `LHHttp2::GetDocumentSize` over non-null PDB `DocumentParts` and `BufferLength`. Relocation matches recovered `CVillagerInteractionsDef::Copy`, `CCarryingDef::Copy`, and packed `CAnimComponentCombatFlourishTargetOffset::TransferBinaryOut`.

The definition and HTTP sources explicitly document retail/debug layout differences instead of renaming offsets: retail's 12-byte vector moves `OverriddenDummyObject`, the definition base shifts the interaction fields, and expanded debug strings/containers move `LHHttp2::DocumentParts`. Repeated camera-mode, navigation-tree, bank-file, input-event, and template labels whose bodies or offsets contradicted their propagated owners were ledgered without promotion. Batches through 214 are reviewed; batch 215 is active.

### Batches 215-220: typed construction, state leaves, and line extents

Batches 215-220 added nine readable retail-parity functions. The exact matches are
`CTCHeroExperience::ForceSpendableExperienceTo` and the paired
`C2DLineF::GetLowestX`/`GetHighestX` helpers; the latter retain the original-style
templated `Minimum`/`Maximum` expressions and compile to every retail byte. Relocation
matches recover two animation-component allocators as ordinary `new Derived`, the
transformation-chunk writer, `CTCCrateStack::GetNumCrates`,
`CTCVillageMember::IsAttachedToVillage`, and the default environment-theme transition.

Every landing uses PDB member identities. Retail overlays explicitly document the
debug/retail base-size differences for the mesh transform and theme-transition fields.
Plausible `CGameTimeManager::IsDay` and `CBankFile::GetUnusedSpace` versions were not
promoted because their generated control flow or register schedule differed from the
retail oracle. The nine-function combined VC7.1 build passes; 962 of 963 generated
shared headers compile, with the pre-existing `CGameDefinitionManager` quarantine the
only failure. Batches through 220 are reviewed; batch 221 is next.

### Batches 221-227: corrected identities and exact state helpers

Batches 221-227 added nine readable functions. Exact matches recover
`CPlayer::HasMode` as the inlined `std::find` over PDB member `PlayerModes`,
`CMap::AllocateMapThingUID` as a 64-bit post-increment, `CSurface::GetDimensions`
through the Direct3D `GetDesc` ABI, both Y-axis `C2DLineF` extent helpers, and the
in-place `GFLimitReturn<float>` clamp. Relocation matches recover two copies of
`CBalverineBattleDef::operator=` and `CAnimComponentCombatMisc::TransferBinaryOut`
over `MeleeFlourish` and `MeleeKnockdown`.

Two propagated identities were corrected from body evidence rather than preserved:
the first Y helper was a duplicate `GetHighestY` label despite matching the established
`GetLowestX` instruction template, and the alleged `GFLimitReturn<double>` uses 32-bit
float loads/stores and has no stable return value. Retail overlays document the smaller
prefixes before `CPlayer::PlayerModes`, `CMap::ThingUIDCount`, and the three Balverine
definition fields. All nine pass together under VC7.1. Batches through 227 are reviewed;
batch 228 is active.

### Batches 228-334: continued typed crawl and script-corpus intake

The crawl continued through batch 334. The refreshed gate now contains 18,860
compiled and behavior-tested candidates: 8,141 exact matches and 10,675
relocation matches. Genuine C++ accounts for 14,686 landed manifest entries and
393,624 matched retail bytes. Batch 335 is generated and active.

The latest typed landings include `C2DBoxI::ExtendToContain`,
`CTCExplodingObject::SetCountdownTimer`, `CDataOutputStream::Write2DVector`,
`WordWrap_FindNonWhiteSpaceForward`,
`CGameScriptThingManager::HasScriptThing`, `CCard::Turn`,
`C3DCoordI::operator+=`, and `GFSerialiseVectorBinaryOut<float>`. They use
PDB-named coordinates, vectors, card state, 64-bit script IDs, and typed vector
ranges; focused fixtures cover their branch, conversion, and serialization
behavior before parity promotion.

Identity checks remained stricter than name propagation. The crawl rejected
`CTCTavernGameCardBase::NewGame`, `CTriangleInfo::IsIdentical`,
`CPlayerManager::PrepareForLoad`, `CPlayer::HasMode`, and long runs of repeated
`CActionEventShot::Alloc` labels where retail bodies, stack cleanup, member
offsets, or donor layouts contradicted the proposed owner. Behavior-correct but
non-parity versions of `CRightHandedSet::GetRight`, the crime-to-guard filter,
and the mesh-effect smart-pointer accessors remain audit scratch rather than
inflating the landed totals.

The same checkpoint ingested Aeon's `GuildMaster` and
`LUAGameflowAssistance` Lua packages as disabled `reconstructed-source`
shadows. Their native aliases, archive provenance, generated IR, compatibility
reports, and known missing bindings are documented in
`docs/scripts/AEON_LUA_PORTS.md`; this intake does not change native parity
counts or claim Lua/retail equivalence.

### Batches 335-341: retail-layout checks and typed time/perception recovery

Batches 335 through 341 are reviewed and ledgered. Two PDB-backed functions
were promoted: `CGameTimeManager::IsDay` uses the named `Time` member and the
retail day boundaries `1/6` and `7/8`; `CTCPerceiveThing::SetSightRadius` uses
the shared `CTCPerceiveThing` and `CPerceivedThingDef` headers, preserves the
`-1.0f` restore-default sentinel, and clamps the restored extended radius to
the current sight radius. Both pass focused boundary/branch fixtures, the
strict shared-header gate, selected VC7.1 builds, and relocation-masked retail
parity. The refreshed totals are 18,862 compiled and behavior-tested
candidates, 8,141 exact matches, 10,677 relocation matches, 14,688 genuine
landed entries, and 393,737 genuine matched retail bytes.

### Batches 342-346: particle grouping and retail anomaly triage

Batches 342 through 345 were reviewed and ledgered, and batch 346 was opened.
`CEngineInternalPrimitiveParticleGroup::PrimTypeMatchesGroup` at `00bbbf00`
landed as readable C++ with a behavior fixture and an exact 57/57-byte retail
match. It documents the eight primitive type IDs accepted by the generic
particle group without introducing guessed object layout.

Two tempting candidates were deliberately withheld. The genuine adjacent
`C3DCoordI::operator-=` symbol at `00a13c20` has retail behavior that subtracts
X and Y but adds Z, plus an unused three-byte `maxps`; readable C++ reproduces
the behavior but only 54/57 bytes, so artificial assembly was not added.
`GFFloatToLongFloor` at `0048d940` also failed its first boundary fixture and
requires explicit floating-point-environment analysis before landing. The
refreshed comparer totals are 18,863 compiled and behavior-tested candidates,
8,142 exact matches, 10,677 relocation matches, 14,689 genuine landed entries,
and 393,794 genuine matched retail bytes.

Retail/debug divergence remained explicit rather than hidden. A proposed
`CTextureManager::FrameEnd` implementation confirmed behavior and the retail
`0x54` pool stride (`FailedAllocations` at `+0x58c`, versus the donor's
`+0x5cc`) but missed parity and was not landed. The crawl also rejected a
fourth low-confidence `CDrawGuildSeal::ClearMessages` identity whose `+0x3c4`
access contradicts the PDB-consistent class, and an
`IsOriginatedByThing` predicate that reads beyond the donor's eight-byte UID
filter.

Batch 346 then landed `CThingMarkerDef::Copy` as a 58-byte relocation match
using the explicit `CThingMarkerDefRetail` layout. Ego_r describes a 0x8c-byte
object whose `Graphic`, `TriggerType`, and `Properties` fields begin at +0x74,
+0x84, and +0x88. Retail `GetSizeofClass`, `Transfer`, and `Copy` independently
prove a 0x88-byte object with those fields shifted to +0x70, +0x80, and +0x84.
The fixture also verifies retail's more subtle behavior: `Copy` transfers only
the first 14 bytes of `Graphic`, leaving its two-byte tail untouched.

After that landing the comparer totals are 18,864 compiled and behavior-tested
candidates, 8,142 exact matches, 10,678 relocation matches, 14,690 genuine
landed entries, and 393,852 genuine matched retail bytes. Batch 347 is next.

### Batches 347-350: prototype and layout rejection pass

Batches 347 through 350 are reviewed and ledgered. No candidate cleared both
the readability and parity gates. This pass rejected several especially
misleading donor transfers instead of manufacturing fields around them:

- the `GetBulletTimeTicksFromAbilityLevel` body is actually the companion
  floating-point rounding helper;
- `CDrawNPCName::IsShowingScreenshotMessage` treats +0x2c as a counter where
  Ego_r identifies a string-data pointer;
- the alleged `CViewport::ClearHandle` operates at +0x28, versus the donor's
  real primitive handle at +0x18c;
- `CInputManager::UpdateChecksum` writes +0x0c/+0x18/+0x28, while its named
  donor checksum is at +0x48;
- the alleged `C3DMeshLODInfo::Sizeof` recursively invokes itself through
  +0x20/+0x24, which are counted-pointer metadata and async-file-data slots in
  the PDB layout, not child mesh-LOD nodes.

Two genuine names reached behavior-complete near matches but remain unlanded:
`CTCLook::IsWithinYZHeadRange` is 55/58 bytes because retail materializes its
PDB-confirmed `bool` as full EAX, and `CCharString::Truncate` is 60/58 bytes
despite coherent PDB layout and named callees. Batch 351 is next.
