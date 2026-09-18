"""Assemble the review/cross-reference package for Aeon: the three converted units (New Oakvale intro,
Orchard Farm, Guild Training) as readable + draft Lua with their conversion reports, the playable
local-candidate bundle (Oakvale + Orchard), and the findings docs (split proposal, FSE upstream
requirements, readable-style plan, Aeon-port audit, cross-reference notes, converter journal).

    python tools/script_recovery/build_aeon_share_zip.py            # -> work/AeonShare-<date>.zip

Retail bytes are never included (Lua, reports and markdown only; the bundle DLLs are our builds).
"""
from __future__ import annotations

import argparse
import datetime as dt
import json
import shutil
import sys
import zipfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
WORK = ROOT / 'work'
LIFTED = ROOT / 'refs/script_recovery/lifted'

UNITS = [
    # (folder, readable source, draft source, conversion report, notes)
    ('NewOakValeIntro', WORK / 'oakvale_readable_stage_20260916b/NewOakValeIntro', LIFTED / 'NewOakValeIntro/FSE/NewOakValeIntro',
     LIFTED / 'NewOakValeIntro/CONVERSION_REPORT.json',
     'Played through childhood in-game on the local-candidate bundle (v4/v5), zero Lua errors. Readable stage = the '
     'hand-structured 2026-09-16 stage that ships in the bundle (Bully intimidation line fix included). The draft is the '
     'older untyped-pipeline output (its todo notes are mostly informational label/cleanup bookkeeping); the readable is the reviewed one.'),
    ('OrchardFarm', LIFTED / 'OrchardFarm/readable/FSE/OrchardFarmRaid', LIFTED / 'OrchardFarm/draft/FSE/OrchardFarmRaid',
     LIFTED / 'OrchardFarm/draft/CONVERSION_REPORT.json',
     'Converter output, 46/46 functions, 0 TODO(native) in the draft; smoke harness clean on draft + readable; NOT yet '
     'verified in-game (the v5 bundle carries it, run pending). Readable stage = readable_style.py (quest-script style).'),
    ('GuildTraining', LIFTED / 'GuildTraining/readable_converter/FSE', LIFTED / 'GuildTraining/draft/FSE',
     LIFTED / 'GuildTraining/draft/CONVERSION_REPORT.json',
     'Converter output through the typed pipeline: 152 functions, 37/37 files compile, 772 todo notes (most are '
     'label/goto/cleanup bookkeeping), 11 smoke-harness notes (stack-slot collisions the restore cannot split yet). '
     'No runtime package/bindings yet; needs IsPlayerHoldingFireRangedWeaponButton + me:MsgIsHitBy(name). '
     'Rebuilt 2026-09-17 night: WoodsMelee/ScorpionHome is what you read as "a Lua version of the disassembly" — compare now.'),
    ('TraderConflict', LIFTED / 'TraderConflict/readable/FSE', LIFTED / 'TraderConflict/draft/FSE',
     LIFTED / 'TraderConflict/draft/CONVERSION_REPORT.json',
     'Q_TraderConflictEvil + Q_TraderConflictGood, new this night: 63 functions, 15/15 files compile, 169 todo notes, smoke '
     'harness 0 errors on draft + readable. TraderToRescue.Main (5.5 KB, Ghidra lost its stack analysis) is the rough '
     'one; its operands were read back from the machine code (see the journal). Needs me:MsgIsHitBy(name), '
     'me:MsgIsHitByAnySpecialAbilityFrom(name), quest:IsPlayerHoldingLockTargetButton(), quest:TextEntryExists(key).'),
]

DOCS = [
    ('docs/scripts/AEON_SPLIT_PROPOSAL.md', 'AEON_SPLIT_PROPOSAL.md'),
    ('docs/scripts/FSE_UPSTREAM_REQUIREMENTS.md', 'FSE_UPSTREAM_REQUIREMENTS.md'),
    ('docs/scripts/READABLE_STYLE_PLAN.md', 'READABLE_STYLE_PLAN.md'),
    ('docs/scripts/AEON_LUA_PORTS.md', 'AEON_LUA_PORTS.md'),
    ('work/aeon_new_oakvale/CROSSREF.md', 'NEW_OAKVALE_CROSSREF.md'),
    ('refs/script_recovery/orchard_farm/RUNTIME_API_GAPS.md', 'ORCHARD_RUNTIME_API_GAPS.md'),
    ('docs/journal/2026-09/CONVERTER_GENERIC_UNITS_2026-09-16.md', 'CONVERTER_JOURNAL_2026-09.md'),
    ('docs/pipeline/GOTCHAS.md', 'GOTCHAS.md'),
]


def unit_summary(report_path):
    if not report_path.exists():
        return {}
    d = json.loads(report_path.read_text(encoding='utf-8'))
    units = d.get('units') or [d]
    fns = sum(len(u.get('functions', [])) for u in units)
    todo = sum(len(f.get('todo', [])) for u in units for f in u.get('functions', []))
    return {'functions': fns, 'todoNotes': todo, 'scripts': [u.get('script') for u in units if u.get('script')]}


def build(out_zip, bundle):
    stamp = dt.date.today().isoformat()
    stage = WORK / f'AeonShare-{stamp}'
    if stage.exists():
        shutil.rmtree(stage)
    stage.mkdir(parents=True)
    summaries = {}
    for folder, readable, draft, report, note in UNITS:
        dest = stage / folder
        shutil.copytree(readable, dest / 'readable')
        shutil.copytree(draft, dest / 'draft')
        if report.exists():
            shutil.copy2(report, dest / 'CONVERSION_REPORT.json')
            md = report.with_suffix('.md')
            if md.exists():
                shutil.copy2(md, dest / 'CONVERSION_REPORT.md')
        rr = readable.parent / 'READABLE_REPORT.json' if (readable.parent / 'READABLE_REPORT.json').exists() else readable.parent.parent / 'READABLE_REPORT.json'
        if rr.exists():
            shutil.copy2(rr, dest / 'READABLE_REPORT.json')
        summaries[folder] = dict(unit_summary(report), note=note)
    docs = stage / 'docs'
    docs.mkdir()
    for src, name in DOCS:
        if (ROOT / src).exists():
            shutil.copy2(ROOT / src, docs / name)
    if bundle and bundle.exists():
        shutil.copytree(bundle, stage / 'playtest-bundle', ignore=shutil.ignore_patterns('runs', '*.log', '__pycache__'))
    (stage / 'README.md').write_text(readme(stamp, summaries, bundle), encoding='utf-8')
    out_zip = out_zip or WORK / f'AeonShare-{stamp}.zip'
    with zipfile.ZipFile(out_zip, 'w', zipfile.ZIP_DEFLATED) as z:
        for p in sorted(stage.rglob('*')):
            if p.is_file():
                z.write(p, p.relative_to(stage.parent).as_posix())
    print(f'{out_zip} ({out_zip.stat().st_size // 1024} KiB)')
    return out_zip


def readme(stamp, summaries, bundle):
    lines = [f'# Fable TLC retail quest scripts -> FSE Lua — share for Aeon ({stamp})', '',
             'Everything here is lifted mechanically from the retail `Fable.exe` quest script classes (Ghidra decompile,',
             'names/layouts from the debug PDB, FSE `GameInterface.h` typedefs for the vtable ABI) by',
             '`tools/script_recovery/convert_quest_unit.py`, then restyled by `build_readable_unit.py` (readable_style.py).',
             'Each unit ships two stages:', '',
             '- `draft/`    — the faithful, reversible audit trail (one statement per native statement, `-- TODO(native)` where',
             '                the lowering gave up, `xStack_NN`/`iVarN` names where no evidence named the value).',
             '- `readable/` — the same logic as a quest script: termination boilerplate folded into',
             '                `if not quest:NewScriptFrame(me) then return end`, single-use temporaries inlined, `state:GetInt("TeamID")`,',
             '                init-only state hoisted to locals, goto idioms -> if/else, one-line `-- Owner.Function (retail 0x...)` headers.',
             '', 'Cross-reference against your hand ports is the point: same API, same state names, same cutscene/resource lifetimes.',
             'Where they disagree, one of us has a bug — the retail bytes decide (addresses are in every header comment and in',
             '`CONVERSION_REPORT.md`).', '', '## Units', '']
    for folder, s in summaries.items():
        lines += [f'### {folder}', '', s['note'], '',
                  f"- scripts: {', '.join(s.get('scripts') or [folder])}",
                  f"- native functions converted: {s.get('functions', '?')}; todo notes in the draft: {s.get('todoNotes', '?')}", '']
    lines += ['## Docs', '',
              '- `docs/AEON_SPLIT_PROPOSAL.md` — the 161-cluster inventory to split the remaining scripts (ownership + sizes).',
              '- `docs/FSE_UPSTREAM_REQUIREMENTS.md` — the generic runtime primitives stock FSE needs to run this output',
              '  (native-quest replacement by name, state/thing/list bindings, frame return values, ...). Newest rows:',
              '  `quest:IsPlayerHoldingFireRangedWeaponButton()`, `me:MsgIsHitBy(name)`, `me:MsgIsHitByAnySpecialAbilityFrom(name)`,',
              '  `quest:IsPlayerHoldingLockTargetButton()`, `quest:TextEntryExists(key)` (`me:MsgExpressionPerformedTo()` exists: name or nil).',
              '- `docs/READABLE_STYLE_PLAN.md` — what the readable stage does and what is still open.',
              '- `docs/AEON_LUA_PORTS.md` + `docs/NEW_OAKVALE_CROSSREF.md` — the audit of your 20 ports against the PDB and the',
              '  New Oakvale cross-reference (state names, lifetimes, RNG use).',
              '- `docs/ORCHARD_RUNTIME_API_GAPS.md` — per-call gaps found while packaging Orchard Farm.',
              '- `docs/CONVERTER_JOURNAL_2026-09.md` — the converter journal (what each lowering rule is and why).',
              '- `docs/GOTCHAS.md` — one-line pitfalls (heredocs, Ghidra wrapping, char literals in Lua, ...).', '',
              '## Findings worth knowing', '',
              '- Three real converter bugs surfaced while making the output readable: a Lua boolean compared with 0 is always true',
              '  (`if c ~= 0` on a `not (...)` value — every Orchard CrateTeamMember became a bandit), a retyped class member prints',
              '  without its class so its call sites never paired (`"FETCHING" + 4` was `"REQUEST_PROTECTION"` at the wrong slot),',
              '  and `\'\\x01\' - (cond)` is `not cond`. Your ports would have caught all three in cross-reference.',
              '- FSE `quest:NewScriptFrame(me)` returns `not terminating` of the entity host for lifetime-None quests — the same',
              '  predicate as `quest:IsActiveThreadTerminating()` in an entity VM — so the Aeon-style frame idiom is exact.',
              '- Deeply indented Ghidra output wraps statements at arbitrary points (`+ 0x1d8\\n ))(`, `4)\\n ,0.5`); everything',
              '  that reads decompiles must re-join unbalanced-paren lines first.',
              '- The `Teams_<n>_<Field>` state keys are the flattened `CCrateTeamManager` members (MemberCount, StateCounter_k,',
              '  TeamCrateCarrier, CrateDropPos, EnemyTeam, TeamReinforcementsTimer); `MemberState` values 0..5 have no PDB enum.',
              '- Helpers with no PDB name keep the bsim label of the homologous body when it is a plain method name',
              '  (`GuildTrainingWoodsMelee.EndMission` = retail 0x00D66EE0, bsim `CQ_CinemaTestScript::EndMission`) — marked in a',
              '  comment on the definition; `helper_XXXXXX` means neither the PDB nor bsim named it.',
              '- Readable stage, second pass (2026-09-17 night), measured against your Fisherman / NewOakValeIntro ports: one',
              '  `local hero = quest:GetHero()` per function, temporaries named after what they hold (`guildScorpions`,',
              '  `scorpionSpawn`, `guildStagBeetle`, `count`, `infoCounter`), the retail cutscene boilerplate folded to your exact',
              '  `quest:StartCutscene({HERO = hero, WHISPER = whisper}, {}, true)` / `RunCutscene` / `EndCutscene` (LuaQuestState::StartCutscene',
              '  does precisely those native calls), helpers named by shape (`PlayHeroCutscene`), and every global-game-data read',
              '  named: `quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MeleeBeetles)` with a `SCRIPT_DEF` table at the top of the',
              '  file carrying the retail script.bin value (`GUI_MeleeBeetles = 3856, -- 10.0`). The offset -> CScriptDef field table',
              '  is `refs/script_recovery/script_def_offsets.json` (PDB layout minus header, three shrinking vector kinds).',
              '- Two real bugs found on the way, both in the previous zip: Orchard\'s whisper cutscene passed the MK_OFWB_WHISPER marker',
              '  to RunMacro instead of the actor map, and the movie / resource destructors were swapped in Guild and TraderConflict',
              '  (one bsim label for two functions). Fixed at the converter; every unit regenerated.',
              '- Operands the decompiler dropped are now read back from the machine code (pushes before the call site, typed against',
              '  the binding signature): `AddNewConversation(me, false, false)`, `AddLineToConversation(id, text, me, hero, false)` ...',
              '- Known residue you will still see: `-- TODO(native): goto LAB_...` where a jump into a sibling block could not be',
              '  restructured, `xStack_NN`/`r1_2` names where nothing named the value, `while not TryAcquire` retry loops in',
              '  entities (your `me:AcquireControl()`), and `nil --[[missing]]` operands where the pushes could not be typed.', '']
    if bundle:
        lines += ['## playtest-bundle/', '',
                  'The local-candidate bundle (`local-candidate-v5`): original FSE + our compatibility add-on, New Oakvale intro +',
                  'Orchard Farm as retail overrides. Not a public release; see its README.md — preflight with',
                  '`python local_test.py --game-dir <Fable dir>`, add `--launch` for a real run on a disposable profile.',
                  'New Oakvale plays through childhood; Orchard Farm has not had its first in-game run yet (rebuilt tonight with',
                  'the RunMacro / destructor fixes above). Your test protocol (mid-quest save+load for OnPersist, quit mid-quest',
                  'for entity control) is what we will run it through.', '']
    lines += ['## Reproduce', '', '```', 'python tools/script_recovery/convert_quest_unit.py --unit orchard_farm',
              'python tools/script_recovery/build_readable_unit.py --unit orchard_farm',
              'python tools/script_recovery/smoke_run_unit.py --unit orchard_farm --stage readable',
              'python tools/script_recovery/build_aeon_share_zip.py', '```', '',
              'Repo: BuffJesus/FableDecomp (branch feat/novi-script-recovery).', '']
    return '\n'.join(lines)


def main():
    a = argparse.ArgumentParser(description=__doc__)
    a.add_argument('--out', type=Path)
    a.add_argument('--bundle', type=Path, default=WORK / 'new-oakvale-original-fse-20260912/local-candidate-v5')
    a.add_argument('--no-bundle', action='store_true')
    args = a.parse_args()
    build(args.out, None if args.no_bundle else args.bundle)


if __name__ == '__main__':
    sys.exit(main())
