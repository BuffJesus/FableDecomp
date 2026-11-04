# Fabletlcmod.com attachment corpus audit, 2026-10-02

Inspected the user's `D:/Downloads/fabletlcmod.7z` for future FableForge mod-manager
compatibility and conversion. No game installs or FableForge source were changed.

## Provenance and preservation

Archive: 885,915,884 bytes; SHA256
`ed3fc8d4c9a5b71fea70d6d6e7692f437518da30563fc3acc61dd2898443e948`.
32,926 archive entries; 2,506,683,274 uncompressed bytes.
The local `fabletlcmod.com/forum/index.html` identifies HTTrack and a mirror time
of **2019-09-27 00:48:07 GMT**. The user supplied forest's account of a 2019 mirror
with attachments but without the dev category. Treat this as an incomplete historic
snapshot, not the entire mod library. The supplied pCloud link was not fetched.

Original archive remains in Downloads. Local inventory, extracted forum pages,
original attachment files, normalized copies and inspection artifacts live under
`work/fmp_corpus_20261002/` (ignored by git). Binary attachments are not committed.
Selected extraction retained HTML/PHP/TXT, FMP and ZIP/RAR/7z files; images and
other loose assets were inventoried but not extracted.

## What's in it

These are descriptions inferred from attachment titles and package banks, not
claims of successful installation or verified in-game behavior:

- Weapons: custom swords, bows, staffs, warhammers; Archon's Platinum weapons,
  Adrahcoul's Fang, All Longbows Headshot, staffs added to Bowerstone/Snowspire.
- Clothing and appearance: armour recolours, Assassin Mask/Hood, Apprentice outfit
  variants, morph changes, tattoos, skin/hair/scar edits and hero model changes.
- Gameplay: zero-key silver chests, ageing slowdown/increase, guard loot,
  armour balancing, creature drops and lifting weapon/magic restrictions.
- Spells and creatures: multiarrow/multistrike, lightning, berserk, ghost swords,
  summon variants and Brown/White Balverine transformation packages.
- World changes: spawn edits, Arena packages, weapon placement and WAD level data.
- Platform-labelled files include Xbox and Anniversary mods. Do not silently
  assume every attachment targets PC TLC.

## Measured corpus

995 direct FMP files + 240 FMP members in first-level ZIP/RAR/7z attachments =
**1,235 occurrences, 1,142 unique source byte sequences, 93 duplicate occurrences**.
Nested members: 138 ZIP, 89 RAR and 13 7z. Archives inside those archives were not
recursively scanned. `FableHook9d5f.rar` could not be indexed by rarfile, but a
separate successful `tar -tf` lists six non-FMP files (DLL/EXE/settings/docs).

The existing `D:/Code/FableForge/build/forge-tools.exe` reader accepts 1,060 unique
original files directly. Another **73** contain `u32 12345` followed by zlib:
bounded inflation reaches EOF without trailing bytes and yields a BIG FMP.
All 73 normalized copies are accepted, giving **1,133 reader-accepted packages**.
Original bytes, SHA256 and origin paths are retained for every normalized package.
Accepted content types: **1,109 type 510; 24 type 459**.

All **1,133** accepted packages also rewrite successfully with identical entry
metadata and payload SHA256s, checked independently of the CLI's listing output.
Only five rewrites are byte-identical; the other 1,128 change physical layout.
The semantic comparison checks bank names/IDs/block sizes, entry order, magic,
IDs/types, dev fields/sources, names, subheaders, payload lengths and hashes.
It intentionally excludes physical offsets and derived type-count tables.
Keep original packages for archival preservation even when semantic conversion passes.

Nine unique files remain unsupported (`01 00 01 ...` headers): Arena Mod fixed,
ASSASSIN_HOOD and seven unlock-region files from
`unbanned_magicNweaponsVer1.593ac.zip`. Their layout has not been identified;
do not classify them as corrupt just because the BIG reader rejects them.

Filename-matched local forum references: **491 unique packages, 2,821 page links**.
These are leads for instructions/attribution, not verified authorship or dependencies.

## Compatibility priorities

Cross-checked package contents against current `tools/forge-cli/main.cpp`
(`fmpApply`, around line 7894) and `libs/forgecore/src/big.cpp`:

1. Accept the outer `12345 + zlib` form with bounded inflation. This collection
   supplies 73 real examples; normalized copies already demonstrate reader compatibility.
2. Recover records with blank names. **132 packages contain 3,559 unnamed
   GameBINEntries**; the current name lookup cannot resolve them and typed entries
   can fall into the add-record branch. There is also an unnamed frontend record.
   Investigate entry IDs and legacy installer behavior before implementing resolution.
3. Link metadata and cross-container relocation: **883 packages** have
   GameBINLinkMetaData, **309** graphicsLinkMetaData. Current apply explicitly skips
   link fixups. Raw payload ingestion is not evidence of a playable conversion.
4. Asset-bank installation: **406** packages have maintextures, **314** graphics,
   **283** text and **9** effects. Current apply does not write these ChocolateBox
   banks. Legacy case-sensitive MainTextures/Graphics/AssetLinks also occur.
5. Shared names and all BINs: three packages edit script.bin, one frontend.bin;
   current apply warns and skips them. Brown/White Balverine transformations,
   Scriptbin REAL PAN and Working Stick 1000000 are concrete fixtures.
6. Level containers: **23** packages have FinalAlbionWAD and one FinalAlbionSTB
   (Nostro's Sword Graveyard Spawn). Current apply stages expected WAD level paths;
   STB is not handled there.
7. Conflict previews: CREATURE_HERO occurs in **82** distinct source packages,
   GHOSTLY_SWORD in **60**, Berserk def in **39**. A shared identity is a potential
   overlap, not proof that the payloads conflict. Compare decoded fields/payloads.
8. Keep source platform, instructions, attribution and originals with converted
   output. A successful package-table read is not an installation compatibility grade.

## Reproduction and artifacts

Reusable audit tool: `tools/audit_fmp_corpus.py`.

```powershell
python tools/audit_fmp_corpus.py work/fmp_corpus_20261002/extracted work/fmp_corpus_20261002/audit --forge-tools D:/Code/FableForge/build/forge-tools.exe --roundtrip
```

- `archive_inventory.json`: every outer archive path and declared size.
- `provenance.json`: original archive fingerprint and context.
- `audit/corpus.sqlite3`: packages, all origin paths, local page references,
  bank/record identity, definition and length; SQL index on `(bank,name)`.
- `audit/packages.json`: reader results, header bytes, source fingerprints,
  normalized paths/fingerprints and optional rewrite verification.
- `audit/summary.json`: aggregate counts, failures and limitations.
- `audit/representative_cases.json`: ten initial examples covering 24 observed
  bank/dialect/normalization/unnamed-record features.
- `audit/normalized_fmps/`: inflated copies; originals retained separately.

Reader executable SHA256:
`b404892d329a457fb950995f122f0c72701715e2e9e044b6bfc29b1aaad1b900`.
Python syntax compilation passes. No game was launched and no package was installed.
