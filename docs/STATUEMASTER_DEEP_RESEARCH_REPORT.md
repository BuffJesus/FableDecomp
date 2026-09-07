# Statue Master: recovered design, retail behavior, and TraderToEscort

Date: 2026-08-28

## Bottom line

The five theorized augmentation clues correspond to real shipped item families:
Fire, Sharpening (internally/UI “Steel”), Silver, Piercing (formerly “Diamond”),
and Lightning. Those augmentation items are found as ordinary chest loot around
Albion. None is a scripted object waiting to be struck.

No surviving PC object or script implements the five-strike puzzle. The clue
text is orphaned localization. The most compelling physical remnant is the
Greatwood Statue Master reward: a Piercing Augmentation chest stands between
two unique but unscripted statues. This fits the old “Diamond” clue—“Pierce my
stony heart”—but does not prove those retail statues once ran the puzzle.

## Evidence hierarchy

### Established

- Five orphan `STATUE_MASTER` inscriptions exist in retail localization.
- `game.bin` contains five matching old augmentation categories: Flame, Extra
  Damage/Steel, Silver, Diamond, and Lightning.
- Retail object names map Steel to Sharpening and Diamond to Piercing.
- The five corresponding augmentations are placed only as container rewards in
  extracted level data; none carries an entity script.
- The Greatwood Statue Master chest contains Piercing and is flanked by two
  `OBJECT_LOOKOUT_POINT_STATUE` objects with no script.
- Three available PC gameplay executables contain no reference to any of the
  five inscription tags and no extra augmentation-puzzle StatueMaster class.

### Strong inference

An earlier design used these augmentation types as keys for a Statue Master
puzzle. The exact five-way category/text match makes this substantially more
than a thematic guess.

### Unknown

- Whether it used one statue, five statues, or multiple locations.
- Whether it was connected to the Guild-facing direction.
- What its final reward would have been.
- Whether the planned puzzle used one statue or several, how it related to the
  Guild marker, and what its final reward would have been remain unknown. The
  original Xbox executable has now been checked and contains no additional
  surviving implementation; see `docs/STATUEMASTER_XBOX_XREF.md`.

## Retail Statue Master and TraderToEscort

The released PC code has two rewards. Pointing toward Greatwood preserves the
Piercing chest. Pointing toward Bowerstone can unlock the tavern cellar—but only
while the repeatable escort quest's dynamically spawned trader exists under the
script name `TraderToEscort`.

That dependency is absent from the submitted Lua port because its lookup is
commented out. For behavioral parity, restore the lookup and null test. It is
the only gameplay-significant discrepancy found in the port.

The Guild marker is real and detected. PC code has no Guild text/reward branch,
so it displays “Nowhere.” Yet localization preserves “The statue is pointing to
the Guild.” This proves a removed presentation state, not a removed reward.

## Why the world items do not constitute a live hidden puzzle

All extracted placements of the relevant augmentation objects are chest
contents. Searches across TNG, GTG, QST, INI, compiled definition/script banks,
three PC executables, and PDB symbols found no placed object referencing the
five clue tags, no hit-message handler tied to them, and no additional
StatueMaster entity class. The Greatwood statues have blank readable overrides;
their shipped generic text concerns disturbed plants.

So the careful answer is: the items and likely physical remnants survive, but
the reactive scripting does not—at least in every available PC/TLC source.

## Historical corroboration and limits

Formal guides list only the Greatwood chest and Bowerstone cellar
([Prima](https://primagames.com/eguides/fable-anniversary-eguide/walkthrough/lookout-point-to-greatwood-caves/bowerstone-south),
[Gameswelt](https://www.gameswelt.ch/fable-the-lost-chapters/komplettloesung/komplettloesung-799/21)).
A 2008 player discussion says Guild/Picnic directions do nothing
([GameFAQs](https://gamefaqs.gamespot.com/boards/929075-fable-the-lost-chapters/43531211)).
A 2004 original-Xbox recollection says the statue named Guild Hall, Greatwood,
and Bowerstone ([Neoseeker](https://www.neoseeker.com/forums/2232/t441561-secret-hints-from-man-himself/19.htm)).
That last report is anecdotal, but it agrees with the orphan Guild line.
The executable comparison is now complete. Xbox overlay `S_VSM` contains the
reachable StatueMaster implementation and has the same dispatcher as PC: only
Bowerstone and Greatwood receive distinct text; Guild falls through to Nowhere.
The five inscription tags are unbound on Xbox as well. See
`docs/STATUEMASTER_XBOX_XREF.md` for the full evidence.

Claims tying the system to potions, Picnic Area restocks, the Singing Sword,
sundials, or blue nymphs have no support in the recovered PC code/data. Modern
mods may reconstruct such content, but are not evidence of vanilla behavior
([Nexus “Fable – The Lost Content”](https://www.nexusmods.com/fablethelostchapters/mods/93)).

## FSE developer action items

1. Restore `TraderToEscort` gating for a parity port.
2. Document the old five-category mapping as recovered design evidence.
3. If creating a demonstration reconstruction, use FSE hit-message APIs but
   label the behavior as reconstructed rather than decompiled.
4. Do not invent a Guild reward in a parity port.
5. Xbox cross-version analysis is complete: no additional Guild branch or
   inscription behavior survives in `S_VSM`.

## Evidence-constrained reconstruction concept

This section is a proposed restoration design, not a recovered implementation.
The most coherent reconstruction is a hub-and-satellites system:

- The rotating Lookout Point Statue Master acts as a timed locator and global
  coordinator.
- Smaller statues or monuments around Albion carry the five surviving signs.
- A statue accepts a hit from a weapon bearing the matching augmentation:
  Flame, Sharpening/Steel, Silver, Piercing/Diamond, or Lightning.
- Each accepted strike sets one persistent global seal and reveals a local
  secret. Incorrect hits produce feedback but do not damage the monument.
- The Guild-facing state serves most plausibly as a tutorial, progress shrine,
  or final resolution after all seals. Only the Guild message is evidenced; a
  Guild reward must remain explicitly reconstructed.

The Greatwood secret should anchor the restoration because it has the strongest
physical evidence: its Piercing reward is flanked by two unique unscripted
statues, and the old Diamond clue says “Pierce my stony heart.” Bowerstone
should preserve the native `TraderToEscort` world-state gate, but expose it
through an inscription or henchman hint so the condition is discoverable.

An FSE implementation would use one persistent master quest plus individual
entity scripts. Satellite scripts should make their statues usable and
targetable, observe hero-hit messages, inspect the striking weapon's
augmentations, set master-owned completion flags, and trigger local mechanisms.
The system should remain an environmental secret rather than an F9 quest-card
checklist. A unique five-aspect weapon is a plausible new finale; the Singing
Sword is not evidence-backed and must not be presented as parity content.

Structural QA was completed. Visual rendering was intentionally omitted per
request.
