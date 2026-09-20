# Oakvale Reborn kickoff — plan approved, cutscene writer landed, spike S1+S3 bundle built (2026-09-19)

**What this is.** The user's rewritten childhood intro on top of the New Oakvale Lua override. Plan
approved (`C:\Users\Cornelio\.claude\plans\prancy-hopping-possum.md`); decisions: retail StartOakVale
stage + cast, new `CCutsceneDef` macros in `script.bin` for set pieces + Lua camera for small beats,
ElevenLabs VO through `dialogue_pipeline.py --add`, the raid FMV replaced by an in-engine scene.
Story seed: a Stranger offers the child a sword to wipe out Oakvale — accept = the child fights
(spike S6 gate), refuse = the raid is averted. Authored tree
`refs/script_recovery/authored/OakvaleReborn/` (STORY.md, CHECKLIST.md, manifest/intro.yaml,
cutscenes/*.cs); tools `tools/oakvale_reborn/`; outputs `work/oakvale_reborn/`.

## CCutsceneDef is a full def, not "header + commands"

The "9-byte header" in `docs/engine/CUTSCENES.md` is the 5-byte base prefix `01 00 01 00 00` (constant
on all 595) + the crc0 tag of `Macro`. A payload is that prefix and **eight** tagged
`Vector<CCharString>` fields in Transfer order — `Macro`, `SkipCond`, `SetupCond`, `Lights`,
`LightScene`, `Sound`, `Answer0`, `Answer1` (`ghidra_out/def_schema.json` `CCutsceneDef`). What
`decodeCommands` called "trailing bytes" (56–247 per entry) are those fields. `SkipCond` is a
mini-macro run when the player skips (`FadeOut / StayFadedOut / GamePause 0.5 / HERO.FadeIn 0 …`);
retail `SetupCond` is empty or one `""`. Nothing in the payload references the entry index, so a
cutscene is the simplest possible def to append (only the names.bin crc0, already fixed in forgecore).

## Landed in FableForge (uncommitted)

- `libs/forgecore/include/forge/cutscene_script.hpp`: `Def` + `decodeDef`/`encodeDef` (whole
  payload), field names/tags; `encodeCommands` kept for `script fixup`.
- `forge-tools script cutscene-dump <root> <name> [--cs f]` — prints/writes the sectioned `.cs`
  form (`[SkipCond]` etc.; `""` = empty string entry).
- `forge-tools script cutscene-set <root> <NAME> <file.cs> [--write]` — replace (prefix kept) or
  append a `CCutsceneDef`; lints Macro+SkipCond verbs against the 184-verb table (unknown = refused,
  prefix-slop = warning); `.forgebak`; proves reload identity.
- `forge-tools script cutscene-roundtrip <root>` — decode→encode all defs: **595/595 byte-identical**
  on retail; 596/596 after the append.
- Offline append proof: `CS_OVR_SPIKE` lands as entry 611 / `indexInDefinition 595`, names.bin crc
  `0xeb8058ec` = crc0, `script validate` clean. `bin::File::save` recompresses (154,496 → 128,377
  bytes) — payloads identical on reload; same writer as the live-proven quest cards.

## Landed here

- `tools/oakvale_reborn/build_custom_intro.py` — `pristine | cutscenes | overlay | bundle | check |
  install | restore | all`; rebuilds from pristine copies; `install` extends the existing
  `forge_stage_manifest.json` (the install already carries a FableForge world stage: text.big, wad,
  wld, qst, stb, FSE_Master.lua) and `restore` undoes only our files — never `forge unstage`.
- `tools/oakvale_reborn/spike_s1.py` — copies the v4-proven readable stage and patches
  `NOVI_LiveFather.lua`: S1 runs `CS_OVR_SPIKE` before `CS_OAKVALE_INTRO_FATHER` in the same actor
  map/movie session; S3 = Lua camera beat after the retail scene (StartMovieSequence,
  CameraMoveToPosAndLookAtThing, CameraUseCameraPoint("CAM_OVIF_SHOT2"), EndMovieSequence);
  `quest:Log("OVR_SPIKE_S1/S3 …")` brackets.
- Built: `work/oakvale_reborn/{pristine,staged,overlay,bundle-spike-s1}`; 20 Lua files compile
  (lupa 5.4); bundle preflight passed. Live `CompiledDefs` verified pristine (game.bin 14761,
  script.bin 611 entries / 595 cutscenes, names.bin 396,920 B).

## Not done — user steps

`build_custom_intro.py install` (the auto-mode classifier refused the install write) and the launch;
verdict table in `refs/script_recovery/authored/OakvaleReborn/CHECKLIST.md`. Then `restore`.
