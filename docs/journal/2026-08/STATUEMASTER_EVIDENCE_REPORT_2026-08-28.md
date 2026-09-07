# Statue Master and TraderToEscort: exhaustive evidence report

Audience: Fable Script Extender developer and Fable TLC reverse engineers  
Date: 2026-08-28  
Scope: TLC retail PC, two PDB-backed PC gameplay builds, locally retained Xbox data-lineage headers, extracted level/definition/localization data, AlbionSecrets' Lua port, and discoverable historical public sources. No visual work was requested.

## Direct answer

The released PC game implements two Statue Master rewards: the Greatwood Piercing Augmentation chest and the Bowerstone tavern cellar. The cellar has a hidden second condition: the dynamically spawned `QR_EscortTrader` NPC named `TraderToEscort` must exist. The Guild direction is detected but has no PC presentation/reward branch; it falls through to “Nowhere.”

There was almost certainly an earlier augmentation-keyed Statue Master design. Five orphaned inscriptions—Fire, Steel, Silver, Diamond, and Lightning—match five surviving augmentation inventory categories exactly. In retail terminology, Steel became Sharpening and Diamond became Piercing. The relevant augmentation items exist as ordinary chest loot, but no surviving placed object, quest, definition, or PC executable refers to the five inscription tags or implements a hit-response puzzle.

The one especially suggestive physical remnant is the Greatwood secret: its Piercing chest is flanked by two unique, unscripted Lookout Point statue objects. That is a plausible design fossil, not proof that the retail objects were ever active hit targets.

## Verified PC implementation

- TLC retail native functions: `CStatueMasterStatue::Main` at `0x00ED41D0`, `GetStatuePointingPosition` at `0x00ED4420`, cellar at `0x00ED45A0`, chest at `0x00ED4830`.
- The direction markers are `SM_Nothing`, `SM_Bowerstone`, `SM_Guild`, and `SM_Greatwood`. All are initialized, but the readable-text branch only distinguishes Bowerstone and Greatwood. Guild uses the Nowhere result.
- Greatwood's scripted chest survives only for direction 3 and contains `OBJECT_PIERCING_AUGMENTATION`.
- The Bowerstone cellar unlocks only for direction 1 while `GetThingWithScriptName("TraderToEscort")` is non-null.
- `TraderToEscort` is created by `QR_EscortTrader`, not placed permanently. The manager reactivates this escort opportunity at story milestones. Across the TLC executable, the only non-escort consumer of that exact script name is the Statue Master cellar.
- AlbionSecrets' Lua port comments out this lookup and unlocks unconditionally. That is the material parity difference to report upstream.

## The five augmentation clues

Retail English localization preserves:

- Fire: “I feel the blaze.”
- Steel: “I await the keenest.”
- Silver: “I yield to the undead's bane.”
- Diamond: “Pierce my stony heart.”
- Lightning: “Strike and spark.”

The compiled definition bank preserves the corresponding categories:

| Old category | Shipped UI label | Shipped object |
|---|---|---|
| `AUGMENT_FLAME` | Fire | Flame Augmentation |
| `AUGMENT_EXTRA_DAMAGE` | Steel | Sharpening Augmentation |
| `AUGMENT_SILVER` | Silver | Silver Augmentation |
| `AUGMENT_DIAMOND` | Diamond | Piercing Augmentation |
| `AUGMENT_LIGHTNING` | Lightning | Lightning Augmentation |

This exact five-way match is high-confidence evidence of design intent. It does not establish puzzle topology: one statue with five stages, five statues, a Guild reward, or any particular final reward remain unknown.

## Exhaustive negative checks

The five inscription symbols are absent from TLC `Fable.exe`, PDB-backed `FableWin.exe`, PDB-backed `ego_r.exe`, all extracted TNG/GTG/QST/INI files, both current and retail-backup `game.bin`/`script.bin` catalogs, and searches for plausible numeric-ID and CRC encodings. No surviving entity points to the tags.

All placed augmentation references in extracted levels are chest `ContainerContents`; none is a scripted world hit target. The Greatwood flanking statues have empty readable overrides and no `ScriptName`. Their ordinary text says the plants beneath have been disturbed, not one of the five riddles.

The missing `StatueMaster.ini` named by the test quest registration is a stale test-harness filename; no copy survives locally. PDB class inventories reveal only the released master/statue/cellar/chest classes, not another augmentation-puzzle entity script.

## Guild: what is known and what is not

Retail localization contains `TEXT_QST_061_STATUE_GUILD`: “The statue is pointing to the Guild.” The symbol exists in both PC and Xbox-lineage generated headers. It is absent from all three available PC gameplay executables, while the other direction strings have exact native references. PC code therefore had the Guild presentation branch removed.

The original Xbox executable is a material evidence gap. The local Xbox listing proves a `default.xbe` existed, but the binary is not present. A 2004 forum eyewitness recalled that the original Xbox interaction named Guild Hall, Greatwood, and Bowerstone. This is anecdotal, but it aligns with the orphaned Guild string closely enough that the Xbox build must be checked before claiming the Guild message never shipped in any version.

No Guild reward, quest flag, bound object, level section, debug configuration, or extra PC class was found. A missing presentation-only branch is established; a missing Guild reward is possible but unsupported.

## Historical record

Published guides document the Greatwood chest and Bowerstone cellar, not a Guild reward ([Prima Anniversary guide](https://primagames.com/eguides/fable-anniversary-eguide/walkthrough/lookout-point-to-greatwood-caves/bowerstone-south), [Gameswelt TLC walkthrough](https://www.gameswelt.ch/fable-the-lost-chapters/komplettloesung/komplettloesung-799/21)). A 2008 GameFAQs discussion reports that pointing at the Guild or Picnic Area does nothing ([GameFAQs discussion](https://gamefaqs.gamespot.com/boards/929075-fable-the-lost-chapters/43531211)). Community claims about Guild potions, Picnic Area restocking, the Singing Sword, sundials, or blue nymphs are not backed by recovered code or level data. A modern Nexus “Lost Content” mod adds related material, but describes a reconstruction rather than proof of vanilla implementation ([Nexus mod page](https://www.nexusmods.com/fablethelostchapters/mods/93)).

## Recommendations for FSE development

1. Restore the `TraderToEscort` null check in the Lua port or label its removal as an intentional quality-of-life change.
2. Preserve the five clue tags and old augmentation-category mapping in FSE documentation as recovered API/design evidence, not runnable retail behavior.
3. Add hit-message APIs (`MsgIsHitByHeroWithWeapon`, special-ability/projectile variants) to an optional reconstruction example, but do not present that example as decompiled parity.
4. Acquire a legally sourced original Xbox `default.xbe` and relevant language bank for the remaining cross-version branch check.
5. Keep Guild-message support separate from any invented reward; the former has primary localization evidence, the latter does not.

## Proposed restoration architecture (inference, not source recovery)

The most evidence-compatible recreation is a hub-and-satellites environmental puzzle. The rotating Lookout Point statue acts as locator and global coordinator; five satellite monuments display the recovered inscriptions and accept hits from weapons bearing Flame, Sharpening/Steel, Silver, Piercing/Diamond, or Lightning. Each success records a persistent master-owned seal and reveals a local secret. Greatwood should anchor the design because its Piercing chest and two flanking unscripted statues are the strongest surviving physical linkage. Bowerstone should retain the native `TraderToEscort` gate with a readable or henchman hint.

The Guild-facing state could teach the mechanic, display progress, or resolve all five seals. Only its direction message is sourced, so every Guild mechanism or reward must be labeled reconstructed. A unique five-aspect weapon would be a coherent new reward; the Singing Sword has no recovered connection. Implementation should use one namespaced persistent master quest, small targetable/indestructible entity scripts, hero-hit message handling, striking-weapon augmentation inspection, one-shot local mechanisms, and in-world feedback rather than an F9 quest-card checklist.

## Limitations and stopping rule

No original Xbox executable, source-control history, or authenticated Lionhead design document was available. Web sources cannot prove code behavior and were used only for historical corroboration or rumor bounds. Research stopped after three PC builds, level/definition/script/localization banks, debug symbols, test registrations, and targeted historical searches converged; further PC searching was returning only duplicate localization and ordinary loot placements. The original Xbox binary remains the sole consequential technical gap.

## Claim-to-source ledger

| Claim | Source | Publisher/author | Date | Access note |
|---|---|---|---|---|
| PC native behavior and exact xrefs | TLC `Fable.exe`; PDB-backed `FableWin.exe` and `ego_r.exe`; local Ghidra analyses | Lionhead binaries; local RE | Builds predating/including TLC | Locally verified binary evidence |
| Greatwood chest, flanking statues, cellar, escort placements | Extracted `FinalAlbion` TNG/GTG/QST files | Lionhead game data | TLC retail | Locally verified primary data |
| Guild line and five inscriptions | English `text.big`, retail `text.h`, Xbox-lineage `text_xbox.h` | Lionhead game data | Xbox/TLC lineage | Locally decoded primary data |
| Five old category names and shipped object definitions | `game.bin` compiled definition catalog | Lionhead game data | TLC retail | Locally decoded/indexed primary data |
| Lua parity difference | `StatueMaster.zip` | AlbionSecrets / eeeeeAeoN | 2026-08-28 | User-provided source attachment |
| Publicly documented two secrets | [Prima Anniversary guide](https://primagames.com/eguides/fable-anniversary-eguide/walkthrough/lookout-point-to-greatwood-caves/bowerstone-south) | Prima Games | Anniversary era | Secondary walkthrough |
| Contemporary TLC walkthrough | [Gameswelt walkthrough](https://www.gameswelt.ch/fable-the-lost-chapters/komplettloesung/komplettloesung-799/21) | Gameswelt | 2005 | Secondary walkthrough |
| Original Xbox Guild-line recollection | [Secret hints forum thread](https://www.neoseeker.com/forums/2232/t441561-secret-hints-from-man-himself/19.htm) | Neoseeker user | 2004 | Anecdotal; version-specific lead only |
| Guild/Picnic “nothing happens” report | [GameFAQs discussion](https://gamefaqs.gamespot.com/boards/929075-fable-the-lost-chapters/43531211) | GameFAQs users | 2008 | Anecdotal corroboration |
| Modern reconstruction is not vanilla proof | [Fable – The Lost Content](https://www.nexusmods.com/fablethelostchapters/mods/93) | Nexus Mods author | Modern | Community mod description |
