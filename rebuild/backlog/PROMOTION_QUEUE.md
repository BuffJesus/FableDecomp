# Curated C++ promotion queue

Generated: `2026-07-22T18:26:38-06:00`

Uncompiled auto-RE candidates: **26**. Showing: **26**.

Ranking favors checker/integrity/signature PASS, then the smallest declaration, dependency, VC7.1, and source-size repair surface. It does not claim semantic correctness; every promotion still needs a focused behavior oracle and retail comparison.

| Rank | Address | Owner/function | Lane | Signature | Missing deps | VC7.1 fixes | Source bytes | First blocker |
|---:|---|---|---|---|---:|---:|---:|---|
| 1 | `0x00BF3980` | `CEngineLandscapePatch::CEngineLandscapePatch::RelocateData` | `manual-lift` | `PASS` | 1 | 18 | 3473 | 'CEngineLandscapePatch' has not been declared |
| 2 | `0x00401067` | `entry` | `manual-lift` | `PASS` | 3 | 50 | 6600 | static assertion failed |
| 3 | `0x00A7A5E0` | `CNavQuadTree::InitialiseLines` | `manual-lift` | `PASS` | 6 | 5 | 1309 | 'C2DLineF' was not declared in this scope |
| 4 | `0x00A76F30` | `CNavQuadTree::IsAreaBlockedByLines` | `manual-lift` | `PASS` | 9 | 24 | 6581 | 'list' in namespace 'std' does not name a template type |
| 5 | `0x00A781A0` | `CNavQuadTree::UpdateLines` | `manual-lift` | `PASS` | 12 | 75 | 16425 | 'C2DVector' does not name a type |
| 6 | `0x00A79B30` | `CNavQuadTreeNode::Initialise` | `manual-lift` | `PASS` | 18 | 78 | 16120 | 'CANavQuadTreeNode' does not name a type |
| 7 | `0x0050A650` | `CWorldMap::GetMapNavigationAreaInit` | `manual-lift` | `PASS` | 26 | 93 | 26227 | use of enum 'EHeroMorphType' without previous declaration |
| 8 | `0x00401C00` | `GFHandleSystemInitError` | `vc71-port` | `REVIEW` | 0 | 5 | 9788 |  |
| 9 | `0x00409090` | `Engine_FindPrimitiveInArray` | `vc71-port` | `REVIEW` | 0 | 10 | 2191 |  |
| 10 | `0x00403480` | `_WinMain@16` | `vc71-port` | `REVIEW` | 0 | 16 | 1865 |  |
| 11 | `0x00402130` | `MyDocuments_CheckWritePermissions` | `vc71-port` | `REVIEW` | 0 | 29 | 6575 |  |
| 12 | `0x00AB5D00` | `CMouseDX::CMouseDX::CMouseDX` | `vc71-port` | `REVIEW` | 0 | 35 | 4084 |  |
| 13 | `0x007266C0` | `CTCPhysicsBase::CTCPhysicsBase` | `manual-lift` | `REVIEW` | 1 | 16 | 1401 | static assertion failed |
| 14 | `0x00713140` | `CActionDoCreatureAction::CActionDoCreatureAction` | `manual-lift` | `REVIEW` | 1 | 24 | 2734 | invalid use of incomplete type 'class NTCScriptedControl::CActionDoCreatureAction' |
| 15 | `0x00A65FF0` | `CMemoryAllocatorFixedSize::CMemoryAllocatorFixedSize::CMemoryAllocatorFixedSize` | `manual-lift` | `REVIEW` | 1 | 41 | 2986 | 'CMemoryAllocatorFixedSize' does not name a type |
| 16 | `0x00BFD420` | `CLandscapeLayerMesh::CLandscapeLayerMesh` | `manual-lift` | `REVIEW` | 1 | 47 | 4210 | 'CLandscapeLayerMesh' does not name a type |
| 17 | `0x004022B0` | `GFInitialise` | `manual-lift` | `REVIEW` | 1 | 47 | 4991 | invalid use of incomplete type 'struct CDisplayManager' |
| 18 | `0x006B0730` | `CTCPhysicsBase::CTCPhysicsBase` | `manual-lift` | `REVIEW` | 2 | 14 | 1971 | 'C3DVector' does not name a type |
| 19 | `0x00CAF680` | `CEngineLandscapePatch::CEngineLandscapePatch::AllocateTask` | `manual-lift` | `REVIEW` | 2 | 28 | 2195 | 'CEngineLandscapePatch' has not been declared |
| 20 | `0x00CAF580` | `CEngineLandscapePatch::CEngineLandscapePatch::Destructor` | `manual-lift` | `REVIEW` | 2 | 30 | 4306 | invalid use of incomplete type 'class CCharString' |
| 21 | `0x00C3B900` | `CLandscapeLayerMesh::CLandscapeLayerMesh` | `manual-lift` | `REVIEW` | 3 | 15 | 1978 | 'CEngineLandscapeMap' does not name a type |
| 22 | `0x00BFDDD0` | `CLandscapeLayerMesh::CLandscapeLayerMesh` | `manual-lift` | `REVIEW` | 7 | 30 | 4322 | 'CGraphicInfoFrame' does not name a type |
| 23 | `0x00A79680` | `CNavQuadTree::ConnectVerticalMapEdge` | `manual-lift` | `REVIEW` | 9 | 58 | 13026 | 'CNavConnectorNode' does not name a type |
| 24 | `0x00A793D0` | `CNavQuadTree::ConnectVerticalMapEdge` | `manual-lift` | `REVIEW` | 11 | 60 | 12933 | 'CNavConnectorNode' does not name a type |
| 25 | `0x00A7A8D0` | `CNavQuadTree::Initialise` | `manual-lift` | `PASS` | 14 | 130 | 26232 | 'C2DLineF' was not declared in this scope |
| 26 | `0x0040CEC0` | `CUserProfileManager_Constructor` | `vc71-port` | `REVIEW` | 0 | 99 | 14968 |  |
