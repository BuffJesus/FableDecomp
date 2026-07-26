# Active task list — decomp, custom quests, and downstream tools

*Refreshed 2026-07-25 from the canonical organized-tree build.*

## Current verified state

- Curated reconstruction: **1,850 / 1,850** VC7.1 compile + behavior PASS.
- Retail parity: **1,523** (`914 EXACT + 609 RELOCATION_MATCH`), with
  `199 DIFFER` and `128 ORACLE_MISSING`.
- Auto-RE intake: **573 generated / 565 structural PASS**; semantic quarantine: **115**.
- Durable source/tests are address-sharded and indexed by `rebuild/ARTIFACT_INDEX.tsv`.
- Custom quest assets are staged offline: nine card defs, 36 custom English strings,
  and one childhood mysterious-NPC TNG addition. Nothing has been deployed to the live game.

Generated queues:

- `rebuild/backlog/promotion_queue.tsv` — ranked uncompiled candidates with semantic hazards.
- `rebuild/backlog/forgefse-binding-queue.tsv` — current binding reconstruction lane.
- `rebuild/backlog/functions.tsv` — canonical reconstruction gaps.

## P0 — review the current Quest-wrapper tail

The organized-tree full build and parity refresh are complete. Review these generated wrappers
before any promotion:

1. Reconstruct the failed lookup paths in `DisplayTutorial @ 0x0089E710`,
   `EnableGuards @ 0x00896270`, `EnableVillagerDefTypes @ 0x008962D0`, and
   `GetNumberOfItemsOfTypeInInventory @ 0x00897190`; each currently dereferences an end sentinel.
2. Correct the unrelated-object fallback in the inventory wrapper and the implausible lip-sync
   container identity in `ClearGossip @ 0x008AA010`.
3. Verify the raw slot and null behavior in `GetWaterHeightAtPosition @ 0x0088DF60`.
4. Promote only through VC7.1 compile, focused behavior, signature audit, and retail comparison.

## P0 — make the secret hunt playable

### Registration

1. Register the persistent parent quest `SecretHunt`.
2. Register `MysteryHunt1` through `MysteryHunt9` in ForgeFSE and
   `data/Levels/FinalAlbion.qst`; the companion script currently activates these names.
3. Bind the childhood entity script to `MysteriousChildhoodNPC`.
4. Keep the card, text, TNG, and Lua files in one deployable/rollback unit:
   - `data/CompiledDefs/game.bin`
   - `data/lang/English/text.big`
   - `data/Levels/FinalAlbion/StartOakValeEast.tng`
   - `FSE/SecretHunt/...`

### Region probe

Log `Quest:GetRegionName()` in the intended maps before finalizing the hunt table.
`Greatwood`, `Witchwood`, `Darkwood`, `HookCoast`, and `Oakvale` are plausible;
`Graveyard`, `HobbeCave`, and `Snowspire` are explicitly unconfirmed.

### Runtime smoke order

1. Start a new childhood game and find the NPC at `M_BarrelManHiddenPos`.
2. Confirm talk UI renders the new `TEXT_SECRET_HUNT_NPC_*` strings.
3. Accept the secret; confirm no quest card is created during childhood.
4. Save/reload as a child and confirm the NPC uses the reminder line. This gates
   `OnPersist`/`PersistTransferBool`.
5. Cross the time-skip and confirm `CanOfferAdultSecretHunt()` is true.
6. Activate `MysteryHunt1`; verify F9 shows:
   - title `The Footprint That Vanished`;
   - its custom summary and objective;
   - 200 gold / 50 renown.
7. Complete/deactivate Hunt 1 and activate Hunt 2 to prove distinct cards cycle.
8. Restore all backups after the smoke.

Acceptance: childhood discovery survives save and time-skip, the adult card appears in
F9 with custom text/rewards, and retail restoration hashes match.

## P0 — high-value fse2 decomp candidates

These directly support quest/NPC/runtime features. Run them before general length order.

| Address | Bytes | Function | Why next |
|---|---:|---|---|
| `00896F60` | 97 | `SetDeathRecoveryMarkerName` | save/death state wrapper |
| `00896DF0` | 104 | `KickOffCreditsScreen` | screen-flow wrapper |
| `00894300` | 111 | `MsgIsQuestionAnsweredYesOrNo` | mysterious-NPC interaction |
| `0089E4C0` | 120 | `AddScreenTitleMessage` | custom quest messaging |
| `00891220` | 125 | `OverrideMusic` | pairs with verified `StopOverrideMusic` |
| `00896EE0` | 125 | `GetDeathRecoveryMarkerName` | save/death state wrapper |
| `008913F0` | 148 | `AddQuestCard` | central custom-card ABI |
| `0089AD90` | 155 | `IsQuestStartScreenActive` | quest UI state |

Gate for every address:

1. validate the oracle boundary;
2. author C++03-compatible source and a concrete behavior test;
3. require `check_one.py` `MATCH/RELOCATION_MATCH + PASS`;
4. require independent `verify_and_land.py` agreement;
5. land only after both gates.

## P1 — short fse2 candidates

After the P0 product targets, work the short snapshot-backed candidates:

| Address | Bytes | Function |
|---|---:|---|
| `00891610` | 97 | `GetMostRecentValidUsedTarget` |
| `008A8E40` | 98 | `EntityDecapitate` |
| `0089D7C0` | 99 | `SetAbilityAvailability` |
| `00893F80` | 100 | `MsgOnHeroCastSpell` |
| `008979C0` | 101 | `IsEntityMarriedToHero` |
| `00897A30` | 101 | `IsEntityMarriable` |
| `00899770` | 101 | `GetHeroRoughExperienceLevel` |
| `008A8EB0` | 102 | `EntitySetAttackThingImmediately` |
| `008A1C50` | 104 | `RepopulateVillage` |
| `0089C8A0` | 104 | `IsDeedWitnessed` |
| `008974F0` | 105 | `IsEntityWieldingWeapon` |
| `00898A50` | 105 | `GiveHeroTutorial` |
| `0089A000` | 105 | `GetHeroMoralityCategory` |
| `00894490` | 106 | `RemoveDeadCreature` |
| `008A0FD0` | 107 | `SetReadableObjectTextTag` |
| `00899390` | 109 | `GetHeroStrengthLevel` |
| `00899400` | 109 | `GetHeroSkillLevel` |
| `00899470` | 109 | `GetHeroWillLevel` |
| `00893EC0` | 110 | `MsgOnHeroSlept` |

Full fse2 ordering, including the 46 larger functions, is in
`rebuild/backlog/fse2_remaining_ranked.tsv`.

### Explicit hold

`008997E0 GetHeroExperienceAvailableToSpend` is behavior `PASS` but best parity is
`DIFFER(101v97)`. It is a documented register-allocation/spill residue. Do not blind
regrind it; retry only with a new liveness/register hypothesis or a different compiler build.

## P1 — clear small stalled batches

These are compact cleanup pools after fse2:

1. batch13: **6 remaining**.
   - `00456389 OnReadFinished` (36 B)
   - `004549CE vector_deleting_destructor` (50 B)
   - `00456010 ~CPatchTesselationEdgeStrip` (61 B)
   - `00455008 UpdateShadowScene` (65 B)
   - `0045641A OnDie` (67 B)
   - `00454AEF CopyBackBufferToTexture` (93 B)
2. batch12: **16 remaining**, including eight 30-byte
   `CopyBackBufferToTexture` variants.
3. batch11: **26 remaining**, starting with
   `0044C1CD CTCNoiseDef::GetSizeofClass` (15 B) and
   `0044C12F CInventoryItemDef::GetSizeofClass` (23 B).

These are previous non-wins. Validate boundaries before authoring; many tiny failures are
merged-oracle or signature problems rather than difficult source.

## P2 — ForgeFSE binding retries

fse1 has **69** unlanded binding targets. Start with the smallest:

- `0089CD50 CanThingBe_Smelled_ByOtherThing` (20 B)
- `0088E130 GiveHeroWeapon` (23 B)
- `008CFE30 CGameScriptThing::GetHomePos` (24 B)
- `0088F480 ApplyScriptBrush` (25 B)
- `0088E3D0 MiniMapAllowRouteBetweenRegions` (33 B)
- `008A9E30 AddGossipVillage` (34 B)
- `008A9DD0 AddNewRumourToCategory` (34 B)
- `0088F9B0 GetGuildSealRecallPos` (36 B)
- `00891550/80/B0/E0` boast wrappers (36 B)

Because this is a retry lane, record the prior mismatch class before spending another
authoring pass. Prefer oracle repair, ABI correction, or annotated-diff-driven changes.

## P2 — harden FableForge custom text

1. Review and isolate the text-port changes from the already-dirty FableForge worktree.
2. Commit the port as a coherent unit:
   - BIG magic preservation and type histogram;
   - text encode/upsert/import;
   - quest-card custom-text flags;
   - tests and `docs/CUSTOM_QUEST_CARD_TEXT.md`.
3. Add a mod-package/deploy command that treats `game.bin + text.big + TNG + Lua` atomically.
4. Add a rollback manifest containing original and staged hashes.
5. Add additional language manifests only after English runtime validation.

Acceptance: FableForge remains 7/7 tests passing, an imported archive reopens, all untouched
retail records remain byte-identical, and no command edits the install without `--in-place`.

## P3 — broad backlog

Current remaining staged supply:

- batch14: 292
- batch16: 205
- batch17: 125
- batch15: 108
- batch3: 58
- batch4: 46
- batch5: 59
- retry1: 48

Do not launch these ahead of the product-facing fse queues and small batch11–13 cleanup.
For `retry1` and the old batch3–5 residue, repair merged/over-length oracle rows before
another agent wave.

## Resume rules

- One authoring lane at a time unless rate limits are demonstrably stable.
- Never count self-reported wins; independent VC7.1 verification is mandatory.
- Keep behavior-only `DIFFER` functions out of byte-match totals.
- Preserve automation-owned dirty files and commit scoped changes only.
- Update the three generated TSVs after every landing wave so completed addresses disappear.
