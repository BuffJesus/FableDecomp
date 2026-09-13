# Final-barrel gold native release chain (2026-09-10)

This closes the gap between the New Oakvale script's `AddItemToContainer` call and a collectible
world reward in the installed TLC executable.

## Script-interface insertion

- `CGameScriptInterface::AddItemToContainer` is at `0x0089E780`.
- It validates the script thing, resolves the object definition name, obtains interface type `0x3c`
  (`CTCContainer`), and calls `CTCContainer::InsertItem`.
- The installed implementation of `CTCContainer::InsertItem` is at `0x008ED1F0`. It appends the
  definition index and accompanying `CCharString` to the container's eight-byte reward-entry vector.
- Its complete read-only Ghidra decompilation is checked in as
  `ghidra_out/script_recovery/native_support_decompiles/0x0089E780.c`; the InsertItem implementation was also
  independently decompiled by its installed symbol.

## Destruction-to-pickup transition

- `CTCContainerRewardHero::OnDie` begins at `0x0074F080` in the installed executable.
- Its `0x0074F0D6` call invokes `CreateItemPickupContainer` at `0x0074EC50` when the owning object dies,
  subject to the component's normal activation/state exclusions.
- `CreateItemPickupContainer` obtains the stored reward entries, creates
  `OBJECT_CHEST_REWARD_ON_DEATH`, obtains that object's `CTCContainer`, and inserts every stored
  reward entry into it. Its alternate numeric-gold path creates `OBJECT_GOLDBAG_SMALL_WITH_COINS`.
- `CTCContainerRewardHero::ReleaseItems` at `0x0074E6E0` is the complementary pop-out path: it creates
  one world object per stored reward entry at the destroyed object's position and applies outward
  trajectories before clearing the source vector.

Therefore the script's penultimate-break insertion of `OBJECT_GOLD_1` into the dynamically re-queried
last barrel is deliberately upstream of the engine's barrel-death reward release machinery. The
expected player-visible result on the final smash is a spawned collectible, not a silent bookkeeping
change.

This is static evidence. The next interactive run should still confirm the exact installed object and
component configuration reaches this normal death branch; the existing `GOLD_ARM` and
`GOLD_INSERT_RETURNED` probes distinguish script insertion from subsequent engine release.

## New Oakvale barrel definition closure

- The nine retail TNG records named `NOVI_Barrel` all instantiate `OBJECT_BARREL_BREAKABLE`.
- Pristine compiled-definition entry 3684 contains nine components, decoded through the retail
  `names.bin` references: `CTCHitLocations`, `CTCPhysicsStandard`, `CTCGraphicAppearance`,
  `CTCGraphicAppearanceStaticMesh`, `CTCTargeted`, `CTCSinglePersonOccupiable`,
  `CTCSmashableBarrier`, **`CTCContainerRewardHero`**, and `CTCVillageMember`.
- Thus these specific barrels do carry the component whose `OnDie` implementation creates the reward
  pickup. The remaining interactive uncertainty is no longer component presence; it is whether the
  script insertion occurred before the final barrel's death and whether the resulting pickup was
  noticed/collected.
- `audit_barrel_reward_component.py` reproduces this result from the hashed retail TNG baseline,
  pristine `game.bin.retail-bak`, installed `names.bin`, and FableForge's field decoder.

## Normal OnDie guard state

- The installed component constructor at `0x0074E6B0` zeroes its three-vector storage and initializes
  both control bytes `+0x18` and `+0x19` to true. `OnDie` gates pickup creation on `+0x18`, so a newly
  constructed barrel reward component starts enabled.
- `OnDie`'s special first guard queries interface `0xA0`, identified by the retail PDB enum as
  `TCI_SUMMONABLE_CREATURE`. `OBJECT_BARREL_BREAKABLE` does not carry that component, so the
  summon-specific suppression branch does not apply to New Oakvale's barrels.
- The remaining type-byte exclusions are for non-object entity categories; every audited New Oakvale
  placement is an `Object`. Taken together, normal final-barrel destruction reaches pickup creation.

## Gold-piece definition

- `OBJECT_GOLD_1` is pristine retail game-definition entry 4644.
- Its object payload links at offset `0x45` to anonymous `CGoldDef` entry 13648, whose sole transferred
  integer is exactly `1`.
- Its five components include `CTCGold` in addition to hit, physics, and graphic components. It is
  therefore the engine's collectible one-gold world object, not a cosmetic token or placeholder.
- `audit_gold_piece_definition.py` reproduces the object index, payload link, component names, and
  one-gold amount through FableForge's retail definition decoder.

## Archived playthrough correlation

`interactive-20260910-redtimer-bullyrun-barrels.log` predates the explicit `GOLD_ARM` and native
insertion probes, but it still contains a decisive older breadcrumb:

- the quest enumerates nine `NOVI_Barrel` objects;
- eight barrel predicate-failure callbacks occur;
- the unique singular `GetThingWithScriptName('NOVI_Barrel')` lookup then succeeds;
- the ninth predicate-failure callback follows.

That singular lookup exists only in the penultimate-break reward branch. Thus the observed run reached
the branch with a valid surviving barrel before the final smash. It does not by itself certify the
unlogged native insertion call, which is why the current bundle retains `GOLD_ARM`,
`AddItemToContainer START/END`, and `GOLD_INSERT_RETURNED` probes. The analyzer records this distinction
as `legacyGoldBranchEvidence`, not as `goldNativeCallReturned`.
