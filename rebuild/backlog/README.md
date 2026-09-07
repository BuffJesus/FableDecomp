# Reconstruction backlog

Generated: `2026-08-28T20:12:49-06:00`

The order favors generated candidate clusters first, then extension of proven modules, small prototype closures, and finally bulk signature work.

## Highest-value modules

| Rank | Track | Module | Functions | Prototype gaps | Agent candidates | Verified lifts |
|---:|---|---|---:|---:|---:|---:|
| 1 | candidate-cluster | `_global` | 11366 | 9315 | 493 | 0 |
| 2 | candidate-cluster | `CGameScriptInterface` | 953 | 5 | 441 | 0 |
| 3 | candidate-cluster | `NUISystem::CSprite` | 86 | 86 | 86 | 0 |
| 4 | candidate-cluster | `CTCPhysicsBase` | 118 | 69 | 69 | 1 |
| 5 | candidate-cluster | `CThingPlayerCreature` | 68 | 37 | 37 | 0 |
| 6 | candidate-cluster | `CTCPhysicsStandard` | 77 | 36 | 36 | 0 |
| 7 | candidate-cluster | `CPersistContext_TransferableVector` | 35 | 35 | 35 | 0 |
| 8 | candidate-cluster | `CPersistContext_TransferableComponent` | 34 | 34 | 34 | 0 |
| 9 | candidate-cluster | `CTCInventoryClothing` | 67 | 30 | 30 | 0 |
| 10 | candidate-cluster | `NUISystem::CMenuEntry` | 30 | 30 | 30 | 0 |
| 11 | candidate-cluster | `CTextureRenderer` | 33 | 32 | 28 | 0 |
| 12 | candidate-cluster | `CThingAICreature` | 55 | 28 | 27 | 0 |
| 13 | candidate-cluster | `NUISystem::CList` | 26 | 26 | 25 | 0 |
| 14 | candidate-cluster | `NUISystem::CManager` | 24 | 24 | 24 | 0 |
| 15 | candidate-cluster | `CTCInventoryExperience` | 84 | 26 | 22 | 0 |
| 16 | candidate-cluster | `CTCPhysicsRigidBody` | 46 | 23 | 21 | 0 |
| 17 | candidate-cluster | `CTCInventoryQuests` | 42 | 20 | 20 | 0 |
| 18 | candidate-cluster | `NParticleEngine::CPSCBase` | 20 | 20 | 20 | 0 |
| 19 | candidate-cluster | `NUISystem::CIconText` | 20 | 20 | 20 | 0 |
| 20 | candidate-cluster | `NUISystem::CScrollBar` | 20 | 20 | 20 | 0 |
| 21 | candidate-cluster | `CTCInventory` | 112 | 22 | 19 | 0 |
| 22 | candidate-cluster | `NUISystem::CText` | 18 | 18 | 18 | 0 |
| 23 | candidate-cluster | `CTCInventoryTrade` | 87 | 20 | 18 | 0 |
| 24 | candidate-cluster | `CTCInventoryWeapons` | 60 | 17 | 17 | 0 |
| 25 | candidate-cluster | `NCameraModes::CBase` | 17 | 17 | 17 | 0 |

## Small candidate quick wins

Uncompiled auto-RE candidates ordered by authoritative retail function size, then candidate source size.

| Rank | Address | Module | Retail bytes | Candidate bytes | Blocker |
|---:|---|---|---:|---:|---|
| 1 | `0046bf72` | `_global` | 0 | 205 | 'CSoundPair' was not declared in this scope |
| 2 | `00429fe3` | `_global` | 0 | 367 |  |
| 3 | `0045bbc5` | `_global` | 0 | 510 | 'CRGBColour' has not been declared |
| 4 | `0045bcc1` | `_global` | 0 | 525 |  |
| 5 | `004495d0` | `_global` | 0 | 530 | 'CSoundPair' was not declared in this scope |
| 6 | `0045493e` | `_global` | 0 | 532 |  |
| 7 | `0045efe5` | `_global` | 0 | 533 |  |
| 8 | `0045f008` | `_global` | 0 | 533 |  |
| 9 | `00464aab` | `_global` | 0 | 539 |  |
| 10 | `00c20ef0` | `_global` | 0 | 555 |  |
| 11 | `0042a008` | `_global` | 0 | 562 |  |
| 12 | `0042cde0` | `_global` | 0 | 569 | 'CompareDataOffset' was not declared in this scope |
| 13 | `00462f1f` | `_global` | 0 | 573 |  |
| 14 | `0046780e` | `_global` | 0 | 585 |  |
| 15 | `00429fbe` | `_global` | 0 | 627 |  |

## First function queue

| Rank | Address | Module | Gap | Compiler blocker |
|---:|---|---|---|---|
| 1 | `00402130` | `_global` | signature-audit |  |
| 2 | `00403480` | `_global` | signature-audit |  |
| 3 | `00409090` | `_global` | signature-audit |  |
| 4 | `0040cec0` | `_global` | signature-audit |  |
| 5 | `0040f4a0` | `_global` | signature-audit |  |
| 6 | `0040f6b0` | `_global` | signature-audit |  |
| 7 | `0040fbf0` | `_global` | signature-audit | 'CWideString' was not declared in this scope |
| 8 | `00410460` | `_global` | signature-audit | '_Copy_impl' is not a member of 'std'; did you mean '__apply_impl'? |
| 9 | `00410550` | `_global` | signature-audit |  |
| 10 | `00410d80` | `_global` | signature-audit |  |
| 11 | `00410ed0` | `_global` | signature-audit |  |
| 12 | `00410f30` | `_global` | signature-audit |  |
| 13 | `004110d0` | `_global` | signature-audit |  |
| 14 | `00411630` | `_global` | signature-audit |  |
| 15 | `004117c0` | `_global` | signature-audit |  |
| 16 | `00411910` | `_global` | signature-audit |  |
| 17 | `00411a10` | `_global` | signature-audit |  |
| 18 | `00411b90` | `_global` | signature-audit |  |
| 19 | `00411db0` | `_global` | signature-audit | 'CSubtitledMessage' was not declared in this scope |
| 20 | `00411f10` | `_global` | signature-audit |  |
| 21 | `004121d0` | `_global` | signature-audit |  |
| 22 | `00412330` | `_global` | signature-audit | 'CWideString' was not declared in this scope |
| 23 | `00412450` | `_global` | signature-audit | 'CWideString' does not name a type |
| 24 | `004124c0` | `_global` | signature-audit |  |
| 25 | `004125a0` | `_global` | signature-audit |  |
| 26 | `004126e0` | `_global` | signature-audit |  |
| 27 | `004127e0` | `_global` | signature-audit |  |
| 28 | `00412b70` | `_global` | signature-audit | incomplete type 'CCharString' used in nested name specifier |
| 29 | `0041641f` | `_global` | signature-audit | incomplete type 'CScriptThing' used in nested name specifier |
| 30 | `00417a58` | `_global` | signature-audit | 'CDefString' does not name a type |
| 31 | `00419f50` | `_global` | signature-audit |  |
| 32 | `0041c090` | `_global` | signature-audit | incomplete type 'CGraphicFrameData' used in nested name specifier |
| 33 | `0041ceb3` | `_global` | signature-audit |  |
| 34 | `0041cfb2` | `_global` | signature-audit | 'NGameText' does not name a type |
| 35 | `0041d0a8` | `_global` | signature-audit | cannot call constructor 'CSoundInit::CSoundInit' directly |
| 36 | `00429fbe` | `_global` | signature-audit |  |
| 37 | `00429fe3` | `_global` | signature-audit |  |
| 38 | `0042a008` | `_global` | signature-audit |  |
| 39 | `0042a82b` | `_global` | signature-audit | 'next' was not declared in this scope |
| 40 | `0042a879` | `_global` | signature-audit | 'next' was not declared in this scope |
