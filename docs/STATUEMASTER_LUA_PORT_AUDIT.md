# StatueMaster Lua port audit

Date: 2026-08-28

Source reviewed: `D:\Downloads\StatueMaster.zip`, authored by AlbionSecrets,
the original developer of Fable Script Extender (FSE). Upstream project:
<https://github.com/eeeeeAeoN/FableScriptExtender>.

Retail evidence:

- Native TLC `Fable.exe` clean decompilation:
  - `0x00ED41D0` `CStatueMasterStatue::Main`
  - `0x00ED4420` `CV_StatueMasterScript::GetStatuePointingPosition`
  - `0x00ED45A0` `CStatueMasterCellarDoors::Main`
  - `0x00ED4830` `CStatueMasterChest::Main`
- FableWin PDB-backed reference implementation:
  - `0x03AAA0F0` `CV_StatueMasterScript::Main`
  - `0x03AAA840` `CStatueMasterStatue::Init`
  - `0x03AAA8D0` `CStatueMasterStatue::Main`
  - `0x03AAAC70` `CV_StatueMasterScript::GetAngleBasedOnTimeOfDay`
  - `0x03AAAD50` `CV_StatueMasterScript::GetStatuePointingPosition`
  - `0x03AAAF40` `CStatueMasterCellarDoors::Main`
  - `0x03AAB420` `CStatueMasterChest::Main`

## Result

The port reproduces the central behavior closely, but the submitted version is
not behaviorally identical in one gameplay-significant branch.

### Cellar-door discrepancy

Retail unlocks the cellar only when both conditions are true:

1. `GetStatuePointingPosition() == 1`; and
2. `GetThingWithScriptName("TraderToEscort")` returns a non-null thing.

The Lua port comments out condition 2 and sets `unlockCellar = true`
unconditionally at pointing position 1. Restore the lookup and null test:

```lua
if GetStatuePointingPosition(quest) == 1 then
    local trader = quest:GetThingWithScriptName("TraderToEscort")
    if trader and not trader:IsNull() then
        unlockCellar = true
    end
end
```

The exact null convention exposed by the FSE wrapper should be used. If its
`GetThingWithScriptName` returns Lua `nil` for a native null object, the
`not trader:IsNull()` half is redundant; retaining it matches the native test's
intent and the submitted comments.

### What `TraderToEscort` actually is

This is not a generic nearby merchant, a permanent TNG object, or an unrelated
story NPC. It is the dynamically spawned creature belonging to the repeatable
`QR_EscortTrader` quest:

- `CQR_EscortTraderScript::DoMission` waits for Lookout Point, resolves
  `M_ET_TraderSpawnPoint`, creates `CREATURE_TRADER_01`, and assigns the script
  name `TraderToEscort`.
- The route runs Lookout Point -> Greatwood Entrance -> Greatwood Lake ->
  Orchard Farm. Region TNG sections provide the spawn point, bandit ambush
  triggers/spawns, and Orchard Farm arrival/exit markers.
- The quest's termination thread calls `EndMission`, waits for the trader to
  finish or disappear, then deactivates the quest. Therefore the cellar test is
  deliberately checking whether this particular escort opportunity is live.

The `QR_EscortTrader_Manager` activates the repeatable quest in phases. It
offers one opportunity, then waits for each of these story completions before
offering the next: `Q_TraderEscort`, `Q_BanditCamp`, `Q_Arena`, and
`Q_OpeningGraveyardSecretPassage`. After the graveyard-passage phase it enters
a gated recurring loop. Every activation waits until Lookout Point is unloaded,
increments `OrchardFarmTraderEscortCounter`, and activates `QR_EscortTrader`.
The global game-flow script activates the manager alongside the early
Orchard-Farm/Trophy-introduction setup and deactivates it during end-game
cleanup.

This makes the native cellar behavior more specific than the familiar public
secret description: pointing toward Bowerstone is necessary, but the hidden
door unlock is also timed to the presence of the repeatable escort trader.

### Exhaustive cross-reference result

An exact-string reference scan of the analyzed TLC executable found five code
references to `TraderToEscort`: the repeatable quest's binding, spawn/follow and
cleanup paths, its attacking-bandit behavior, and
`CStatueMasterCellarDoors::Main`. No other quest, reward, story event, or global
manager reads that name.

The four direction-marker strings `SM_Nothing`, `SM_Bowerstone`, `SM_Guild`,
and `SM_Greatwood` each have exactly one native reference, all in StatueMaster's
own marker initialization. The three bound-object names likewise each have one
reference in StatueMaster registration. Installed level data contains no
additional StatueMaster logic outside the Lookout Point statue/markers,
Bowerstone cellar door, and Greatwood chest/comment marker (the WAD duplicates
the extracted TNG content).

Accordingly, current PC/TLC static evidence supports these boundaries:

- Bowerstone direction: cellar access, conditional on `TraderToEscort`.
- Guild direction: detected and displayed as "Nowhere," with no implemented
  reward or external event.
- Greatwood direction: controls the one-time chest containing
  `OBJECT_PIERCING_AUGMENTATION`.
- The Bowerstone tavern-cellar chest itself contains
  `OBJECT_HERO_SHIRT_LEATHERARMOUR`; it is ordinary cellar level data, not a
  second StatueMaster script.

This is strong static evidence that there is no additional statue event in the
three available PC gameplay executables. It cannot prove that designers never
intended one, nor does it settle the original 2004 Xbox executable: the local
Xbox reference set has its generated text header but not `default.xbe`. The
unused Guild direction remains credible cut-content scaffolding, but no PC
executable consumer or level-data payload for such a reward was found.

### Recovered cut-content clue: Guild localization

The retail English `text.big` bank retains an orphaned entry that materially
strengthens the cut-feature interpretation:

```text
TEXT_QST_061_STATUE_GUILD = "The statue is pointing to the Guild."
```

It is retail text ID 3497 and is also preserved in the shipped `text.h` enum.
The neighboring implemented strings are the Nowhere, Greatwood, Bowerstone,
and cellar-locked messages.

Exact-string scans of TLC `Fable.exe` and the PDB-backed `FableWin.exe` find
references to the Nowhere, Greatwood, and Bowerstone tags in
`CStatueMasterStatue::Main`, but the `TEXT_QST_061_STATUE_GUILD` symbol is not
present in either executable. Thus this is not merely an unreachable live
branch: localization was authored for a Guild-facing state and the code that
selected it was removed before these builds. Retail consequently falls through
to the Nowhere message for direction 2.

No matching Guild reward name, entity binding, TNG section, quest flag, or
additional `QST_061` message survives. The clue proves an intended distinct
Guild presentation state, but does not identify what—if anything—was supposed
to happen beyond displaying that text. A reward remains possible design
history, not recoverable shipped behavior.

### Earlier augmentation/weapon puzzle evidence

Five more orphaned retail localization entries use the `STATUE_MASTER` name:

```text
TXT_CLICK_STATUE_MASTER_FIRE      A sign at the base reads, "I feel the blaze."
TXT_CLICK_STATUE_MASTER_STEEL     A sign at the base reads, "I await the keenest."
TXT_CLICK_STATUE_MASTER_SILVER    A sign at the base reads, "I yield to the undead's bane."
TXT_CLICK_STATUE_MASTER_DIAMOND   A sign at the base reads, "Pierce my stony heart."
TXT_CLICK_STATUE_MASTER_LIGHTNING A sign at the base reads, "Strike and spark."
```

None of these five symbolic names occurs in any of the three available PC
gameplay executables. They are also absent from every extracted TNG, GTG, QST,
and INI file and from the compiled game/script definition catalogs, including
searches for their plausible numeric IDs and engine CRC forms. No surviving
placed entity or active script therefore points at these text entries.

There is nevertheless a precise data-side connection to real augmentations.
The shipped `game.bin` retains exactly five older augmentation inventory
categories whose internal names and localized UI labels match the riddles:

| Internal category | UI label | Retail item definition |
|---|---|---|
| `INVENTORY_CATEGORY_AUGMENT_FLAME` | Fire | `OBJECT_FLAME_AUGMENTATION` |
| `INVENTORY_CATEGORY_AUGMENT_EXTRA_DAMAGE` | Steel | `OBJECT_SHARPENING_AUGMENTATION` |
| `INVENTORY_CATEGORY_AUGMENT_SILVER` | Silver | `OBJECT_SILVER_AUGMENTATION` |
| `INVENTORY_CATEGORY_AUGMENT_DIAMOND` | Diamond | `OBJECT_PIERCING_AUGMENTATION` |
| `INVENTORY_CATEGORY_AUGMENT_LIGHTNING` | Lightning | `OBJECT_LIGHTNING_AUGMENTATION` |

The Diamond terminology is especially revealing: no
`OBJECT_DIAMOND_AUGMENTATION` survives, but the inventory category and orphaned
item-name text `TXT_OBJT_DIAMOND_AUGMENTATION = "Diamond"` do. The released
counterpart is Piercing, matching the riddle "Pierce my stony heart." Likewise,
the UI calls the extra-damage category "Steel," while its released object is
Sharpening. This makes an earlier augmentation-keyed Statue Master puzzle the
best-supported interpretation, rather than a guess based only on prose.

Those five retail augmentation objects are ordinary inventory rewards placed
in chests around Albion. They do not carry entity scripts and are not themselves
hit targets. The Statue Master's own Greatwood chest contains the Piercing
augmentation, creating one surviving thematic link to the old Diamond/Piercing
clue. The other four matching augmentation types occur in ordinary chests in
several unrelated regions; none is bound to a StatueMaster script.

The Greatwood secret alcove also has two unique
`OBJECT_LOOKOUT_POINT_STATUE` objects flanking that Piercing chest. Both have an
empty readable-text override and no script name. Their shipped generic localized
click text is instead `TXT_CLICK_LOOKOUT_POINT_STATUE_01` ("The statue points
into the distance. The plants beneath have been disturbed."). They may be
physical remnants of the broader statue concept, but there is no data reference
from either statue to the five orphaned inscriptions and no hit-message handler.

Therefore the evidence supports an abandoned augmentation puzzle family, but
not a functioning hidden target waiting to be struck in TLC. It also does not
connect any one clue specifically to the Guild direction or identify a final
reward.

The surviving henchman lines reinforce the intended scope without naming a
missing prize: good henchmen say the Statue Master can "reveal many secrets"
or "unlock secrets around Albion," while an evil henchman says it "points at
places of interest."

### Direct bound-entity access

Retail `CStatueMasterStatue::Init`, `CStatueMasterStatue::Main`,
`CStatueMasterCellarDoors::Main`, and `CStatueMasterChest::Main` operate on the
entity bound to the running entity script. The port instead looks the entity up
again by `MY_SCRIPT_NAME`. This is normally equivalent when TNG section/script
names are unique and correctly renamed, but it adds a lookup failure mode.
Prefer the callback's `me` object for literal parity wherever the required FSE
method is available on it; otherwise document the lookup as an adapter
constraint rather than retail behavior.

### Behaviors confirmed

- The time-of-day conversion is `(minute / 60 + hour) / 24`.
- Pointing tests use strict lower and upper comparisons with tolerance `0.04`.
- Only indices 1 through 3 are tested; index 0 (`SM_Nothing`) is computed but
  never selected by `GetStatuePointingPosition`.
- `SM_Guild` is index 2 and has no special readable text or reward branch.
- Statue facing is `fmod(timeFraction + 0.5, 1.0)` and updates every script
  frame.
- Readable text maps position 1 to Bowerstone, position 3 to Greatwood, and all
  other positions (including Guild) to Nowhere.
- The chest survives only at position 3 and is removed otherwise.
- The locked-cellar interaction displays `TEXT_QST_061_CELLAR_LOCKED` and waits
  until the message is clicked past while yielding each script frame.

## Custom quest layer notes

Separately reviewed community quest-layer experiments authored by odarenkoas
(these are not AlbionSecrets/FSE upstream code):

- `D:\Downloads\customlayer_2026-08-06 (1).lua`
- `D:\Downloads\oas1st.lua` (2026-08-07)
- `D:\Downloads\FSE_Master.lua`

The 2026-08-07 dynamic thread adapter has a cross-quest collision hazard.
`ensureAdapter(name)` installs a global `_G[name]`, but if another quest already
installed the same thread name it silently reuses that adapter. Common names
such as `Logging`, `Update`, or `RegionThread` can therefore dispatch into the
wrong quest table/proxy. Generate quest-qualified global adapter names (for
example `YOURNAME .. "__" .. methodName`) and pass that qualified name to
`CreateThread`, or maintain one global dispatcher keyed by the actual quest
object and method name.

Other integration cautions:

- Replace `YOURNAME` and `YOURCUSTOMPREFIX_`; unchanged persistence prefixes
  collide across quests.
- `TypeMap.number = "int"` accepts non-integral Lua numbers even though the
  persistence API is integer-only. Reject or explicitly round non-integral
  values during registration.
- `FSE_Master.PersistentGlobals` remains the authoritative type list. A quest's
  local `RegisterGlobal` call does not add the key to the master's persistence
  list, so both declarations must remain synchronized.
- The module-level singleton `proxy` permanently captures the first quest
  object supplied to `wrap`. This is safe only if FSE guarantees one stable
  quest wrapper for the lifetime of that Lua environment. Rebind or key proxies
  by quest object if callbacks can receive replacement wrappers after load.

## Restoration design boundary

The evidence-constrained restoration proposal is documented separately in
`docs/STATUEMASTER_DEEP_RESEARCH_REPORT.md`. Keep it out of the parity port:
augmentation-responsive satellite statues, persistent seals, a Guild finale,
and any new final weapon are plausible reconstructed content, not decompiled
retail behavior. The only mandatory gameplay correction for parity remains the
native `TraderToEscort` condition.
