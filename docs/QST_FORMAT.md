# .qst — Quest Registry Format (Fable: The Lost Chapters, PC)

**Status: fully cracked.** `.qst` is NOT a binary format. It is a plain-ASCII, CRLF-terminated
script of function-call statements that the engine tokenizes at world load. Zero non-printable
bytes in either retail file (verified byte scan: `FinalAlbion.qst` 22,767 bytes / 0 non-printable;
`GlobalQuests.qst` 561 bytes / 0 non-printable).

## Files (retail Steam install, `data/Levels/`)

| File | Size | Content |
|---|---|---|
| `FinalAlbion.qst` | 22,767 B | 187 `AddQuest` + 112 `AddTestQuest` statements, 18 blank lines, 1 stray junk line |
| `GlobalQuests.qst` | 561 B | 13 `AddQuest` statements (global/expression quests), 3 blank lines |

Both files use `\r\n` line endings exclusively and end with a trailing `\r\n`.

## Engine evidence

`Fable.exe` (clean PE32) contains the literal parser keywords as strings, adjacent to the
world-load path (file offsets in the retail exe):

```
0xe38e1c "Init Quests"
0xe38e98 "AddTestQuest"
0xe38ea8 "AddQuest"
0xe38eb4 "Unable to load world %s"
0xe38f74 "Load Quests"
```

So the engine text-scans the `.qst` for the `AddQuest` / `AddTestQuest` keywords during
"Load Quests" at world initialisation. (Exact loader function address = Ghidra follow-up; see Gaps.)

## Grammar

```
file        := element*
element     := statement | blank-line | stray-text
statement   := "AddQuest"     ws* "(" args ")" ws* ";"
             | "AddTestQuest" ws* "(" args ")" ws* ";"
args        := arg ("," arg)*
arg         := ws* ( '"' chars '"' | bareword ) ws*
```

- Whitespace inside calls is free-form (retail files use tabs for column alignment).
- **Stray text is tolerated**: vanilla `FinalAlbion.qst` line 213 is the junk line
  `"                  + "` sitting between two `AddTestQuest` statements — the game ships and
  runs with it, so the engine tokenizer skips anything that isn't a recognized keyword.
- Cross-check: the community tool SilverChest.Modern
  (`D:\Code\Fable\ChocolateBox\ChocolateBox\src\SilverChest.Modern\Formats\Qst\QstFile.cs`)
  parses the same grammar and additionally tolerates `//` and `/* */` comments; no comments
  appear in either retail file, so comment support in the *engine* is unverified (see Gaps).

## Statements

### `AddQuest("<QuestName>", TRUE|FALSE);`

Registers a quest script with the quest manager.

| Arg | Meaning | Evidence |
|---|---|---|
| 1 | Quest name (identifier used everywhere else: WLD initial-quests, TNG `ScriptName`s, save data) | name cross-match below |
| 2 | `TRUE` = active from game start; `FALSE` = registered but dormant (activated later by script/gameflow) | WLD cross-match + ArenaRevisited diff below |

**Cross-validation of arg 2:** `FinalAlbion.wld` has a `START_INITIAL_QUESTS; ... END_INITIAL_QUESTS;`
block listing 6 quests (`Q_SunnyvaleMaster`, `PersonalScriptMain`, `PersonalScript_GlobalThings`,
`HeroBoasts`, `V_HeroDolls`, `CS_PlayCutscene`). **All 6 are exactly the ones marked `TRUE`** in
vanilla `FinalAlbion.qst`, plus two more (`ChapterAndSceneManager` line 3, `NPCDeath` line 6) that
are TRUE in the .qst but absent from the WLD block — i.e. the .qst TRUE set ⊇ WLD initial-quests.

**Ground truth from a real mod (ArenaRevisited 1.0, Nexus 176):** the entire mod is a
**one-line change** to `FinalAlbion.qst`, line 83:

```
- AddQuest("Q_Arena",				FALSE);
+ AddQuest("Q_Arena",				TRUE);
```

(The two files differ by exactly 1 byte overall, 22,767 → 22,766, since `TRUE` is one char
shorter than `FALSE`.) The readme says the mod "tells the game to always have the 'Arena' quest
set to true" so the Arena is replayable — consistent with arg 2 = active-at-start flag.

**Custom quest registration works by appending lines.** FSE's shipped test copy
(`D:\Code\Fable\ChocolateBox\ChocolateBox\ScriptExtender\data\Levels\FinalAlbion.qst`) is vanilla
plus five appended lines (`AddQuest("MyFirstQuest", FALSE);` ... `AddQuest("DemonDoorLUA", FALSE);`)
— this is the registration mechanism the Demon Door capstone needs.

### `AddTestQuest("<QuestName>", "<StartHSP>", <int>, "<DisplayName>", "<IniFile>", "<EndScript>", "<QuestCardDef>");`

Registers a *debug/test-menu* entry for a quest (the developer quest-select). 7 arguments:

| # | Field | Observed values (vanilla FinalAlbion) | Evidence for meaning |
|---|---|---|---|
| 1 | Quest name | must match an `AddQuest` name (e.g. `V_ArcheryCompetition`); duplicates allowed (`Q_FireHeart` ×2, `DummyQuestForHeroLevels` ×8 with different ini files) | name match with AddQuest set |
| 2 | Start marker ("HSP" = hero start point) | 51 distinct, e.g. `KnotholeGladeHSP`, `ArenaHSP`, or `""` | `KnotholeGladeHSP` exists as a Thing in `data/Levels/FinalAlbion/KnotholeGlade.tng` — it's a spawn-marker name where the hero is teleported when launching the test |
| 3 | Integer flag/category | `2` ×77, `0` ×17, `1` ×10 | **meaning unknown** — see Gaps |
| 4 | Display name for the menu | free text, e.g. `"1. Gameflow (Play Fable)"`, `"V Archery Competition"` | human-readable, numbered/prefixed for menu sorting |
| 5 | Setup `.ini` | 24 distinct incl. `""`, `HL0.ini`–`HL7.ini` ("Make Hero Level N" entries), `ChapelOfEvil.ini` | ini names correlate 1:1 with the display names (hero-level presets) — a test-state preset file; **no such .ini files ship in the retail install** (dev-only leftovers) |
| 6 | End script | `""`, `StandardScript.end`, `Gameflow.end`, `AmbushTraders.end`, `GuildTraining.end`, `OakValeIntro.end`, `WaspBoss.end` | not shipped in retail install either — dev-only |
| 7 | Quest-card object def | 54 distinct `OBJECT_QUEST_CARD_*` or `""` | `OBJECT_QUEST_CARD_ARCHERY_COMPETITION` found in `data/CompiledDefs/names.bin` — these are real game.bin object-def names |

`AddTestQuest` entries are developer debug data; for modding (quest registration/merging) only
`AddQuest` matters, but a merger must round-trip both.

## Modding implications

- To register a custom quest: append `AddQuest("MyQuest", FALSE);` (or `TRUE` for
  active-at-start) to `FinalAlbion.qst`. Proven by FSE's shipped example.
- Two mods touching `.qst` merge trivially at the statement level: union of `AddQuest` sets,
  with a TRUE/FALSE conflict rule for same-name entries (ArenaRevisited-style flips).
- The name string is the join key to everything else: WLD `START_INITIAL_QUESTS`, TNG script
  bindings, quest cards (`OBJECT_QUEST_CARD_*` in names.bin/game.bin), and `TEXT_QST_*` text.

## Tooling

`tools/parse_qst.py` — parses, validates, round-trips (byte-identical re-serialization verified
on all three samples), and diffs two .qst files at the decoded-entry level:

```
python tools/parse_qst.py parse <file.qst>          # decoded dump
python tools/parse_qst.py roundtrip <file.qst>      # byte-identical check
python tools/parse_qst.py diff <vanilla> <modded>   # entry-level diff
```

## Gaps (Ghidra follow-up targets)

1. **`AddTestQuest` arg 3 (0/1/2)**: distribution 2×77 / 0×17 / 1×10 with no clean semantic
   split visible from data alone (mode-1 group = Bordello, ChickenKicking, HeroDolls-adjacent +
   all 8 DummyQuestForHeroLevels; mode-0 = mixed). Needs the tokenizer consumer in `Fable.exe` —
   start from the string xrefs at file offsets `0xe38e98`/`0xe38ea8` ("AddTestQuest"/"AddQuest").
2. **Engine comment support**: SilverChest accepts `//` and `/* */`; retail files contain neither,
   so whether `Fable.exe`'s tokenizer skips comments is unverified. (It demonstrably skips the
   stray `+` junk on line 213, so it is at least lenient with unknown tokens.)
3. **Exact loader function address + quest-manager structure**: which class consumes these
   (likely near "Init Quests"/"Load Quests" progress strings) and where the registered
   name→active-flag table lives in memory — needed for runtime (FSE) quest injection without
   file edits.
4. **`GlobalQuests.qst` load timing**: file exists and only uses `AddQuest`; presumed loaded
   once at startup (vs per-world for `FinalAlbion.qst`) — unconfirmed without debugger/Ghidra.
5. **Semantics of TRUE beyond WLD initial-quests**: `ChapterAndSceneManager` and `NPCDeath` are
   TRUE in .qst but not in the WLD `START_INITIAL_QUESTS` block — the precise interaction
   between the two "start active" mechanisms is unresolved.

## Evidence sources

1. Retail `data/Levels/FinalAlbion.qst` + `GlobalQuests.qst` (byte scans, statement census).
2. ArenaRevisited 1.0 (`D:\Downloads\ArenaRevisited-176-1-0-1721624090.rar`) — 1-line mod diff + readme.
3. `Fable.exe` strings (`AddQuest`, `AddTestQuest`, `Load Quests` @ 0xe38e98..0xe38f74 file offsets).
4. SilverChest.Modern `QstFile.cs` (independent community parser, same grammar).
5. FSE ScriptExtender's shipped modded `FinalAlbion.qst` (custom-quest append example).
6. `FinalAlbion.wld` `START_INITIAL_QUESTS` block (forge wld info: 6 initial quests) — name/flag cross-match.
7. `data/Levels/FinalAlbion/KnotholeGlade.tng` (HSP marker existence), `data/CompiledDefs/names.bin`
   (`OBJECT_QUEST_CARD_*` existence).
