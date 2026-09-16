"""Build a disabled readable New Oakvale review package and completion ledger."""
from __future__ import annotations

import argparse
import hashlib
import json
import re
import sys
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.script_recovery.benchmark_lifter import LuaSyntaxChecker
from tools.script_recovery.affair_man_complete import generate
from tools.script_recovery.generate_book_trader_resource_candidate import generate as generate_book_trader
from tools.script_recovery.readable_lua import readable_source, wrap_local_declarations
from tools.script_recovery.native_new_oakvale_conditions import recover as recover_entry_condition
from tools.script_recovery.readable_book_trader import readable_book_source
from tools.script_recovery.structure_book_trader_lua import structure_book
from tools.script_recovery.generate_affair_woman_resource_candidate import generate as generate_woman
from tools.script_recovery.readable_affair_woman import readable_woman_source
from tools.script_recovery.structure_affair_woman_lua import structure_woman
from tools.script_recovery.wife_complete_candidate import generate as generate_wife
from tools.script_recovery.readable_book_trader_conditions import readable_book_conditions
from tools.script_recovery.readable_affair_woman_final import readable_woman_final
from tools.script_recovery.native_barrel_man_setup import recover as recover_barrel_man_setup
from tools.script_recovery.native_barrel_camera_cancellation import recover as recover_barrel_camera_cancellation
from tools.script_recovery.native_barrel_man_health_branches import recover as recover_barrel_health_branches
from tools.script_recovery.generate_barrel_man_resource_candidate import generate as generate_barrel
from tools.script_recovery.bully_main_structure import generate as generate_bully
from tools.script_recovery.guard_candidate import generate as generate_guard
from tools.script_recovery.teddy_girl_candidate import generate as generate_teddy_girl
from tools.script_recovery.generate_theresa_resource_candidate import generate as generate_theresa
from tools.script_recovery.generate_live_father_resource_candidate import generate as generate_live_father
from tools.script_recovery.generate_victim_resource_candidate import generate as generate_victim
from tools.script_recovery.generate_dead_father_readable_candidate import generate as generate_dead_father
from tools.script_recovery.generate_barrel_thug_readable_candidate import generate as generate_barrel_thug
from tools.script_recovery.native_villager_loop_termination import recover as recover_villager_termination
from tools.script_recovery.native_villager_bound_operands import recover as recover_villager_operands
from tools.script_recovery.villager_quest_speech_lists import recover as recover_villager_quest_lists
from tools.script_recovery.readable_watch_barrels import lower as lower_watch_barrels
from tools.script_recovery.readable_post_attack_cutscene import lower as lower_post_attack_cutscene
from tools.script_recovery.readable_new_oakvale_main_bindings import lower as lower_main_bindings
from tools.script_recovery.native_oakvale_objective import lower as lower_initial_objective
from tools.script_recovery.readable_manage_quest_core_markers import lower as lower_quest_markers
from tools.script_recovery.readable_start_barrel_timer import lower as lower_start_barrel_timer
from tools.script_recovery.readable_post_attack_body import lower as lower_post_attack_body
from tools.script_recovery.readable_oakvale_lifecycle import lower as lower_oakvale_lifecycle
from tools.script_recovery.readable_oakvale_deeds import lower as lower_oakvale_deeds
from tools.script_recovery.readable_oakvale_mission import lower as lower_oakvale_mission
from tools.script_recovery.readable_oakvale_progress import lower as lower_oakvale_progress
from tools.script_recovery.readable_oakvale_main import lower as lower_oakvale_main
from tools.script_recovery.generate_villager_resource_candidate import generate as generate_villager
from tools.script_recovery.lift_native_lua import RData

RAW = ROOT / 'refs/script_recovery/lifted/NewOakValeIntro'
PDB_CATALOG = ROOT / 'refs/script_recovery/new_oakvale_intro/pdb_locals.json'


def build(out=RAW / 'readable', *, raw=RAW):
    report = json.loads((raw / 'CONVERSION_REPORT.json').read_text(encoding='utf-8'))
    pdb_catalog = json.loads(PDB_CATALOG.read_text(encoding='utf-8')) if PDB_CATALOG.exists() else None
    pdb_symbols = {(row['owner'], row['function']): row['symbols']
                   for row in pdb_catalog['correspondence']} if pdb_catalog else {}
    paths = [p['path'] for p in report['packages']] + [report['sharedHelpers']['path']]
    sources, originals, maps = {}, {}, {}
    candidate_report = None
    book_candidate_report = None
    woman_candidate_report = None
    wife_candidate_report = None
    bully_candidate_report = None
    guard_candidate_report = None
    teddy_girl_candidate_report = None
    theresa_candidate_report = None
    live_father_candidate_report = None
    victim_candidate_report = None
    dead_father_candidate_report = None
    barrel_thug_candidate_report = None
    watch_barrels_report = None
    post_attack_cutscene_report = None
    main_bindings_report = None
    initial_objective_report = None
    quest_markers_report = None
    start_barrel_timer_report = None
    post_attack_body_report = None
    lifecycle_report = None
    deed_reports = {}
    mission_report = None
    progress_report = None
    main_report = None
    for relative in paths:
        path = raw / relative
        source = path.read_text(encoding='utf-8')
        origin = 'raw converter'
        operand_recovery = None
        structure = None
        helper_readability = None
        presentation = None
        if relative.endswith('/NOVI_AffairMan.lua'):
            source, candidate_report = generate(draft_path=path)
            operand_recovery = candidate_report['completePass']
            origin = 'structured AffairMan phases with native Init and CString/raw-byte animation recovery'
        elif relative.endswith('/NOVI_BookTrader.lua'):
            source, book_candidate_report = generate_book_trader(draft_path=path)
            origin = 'verified BookTrader resource/movie candidate generator; remaining wrapper gaps reported'
        elif relative.endswith('/NOVI_AffairWoman.lua'):
            source, woman_candidate_report = generate_woman(draft=path)
            operand_recovery = woman_candidate_report['control']
            origin = 'verified woman resource/movie/retained actor candidate generator'
        elif relative.endswith('/NOVI_AffairWife.lua'):
            source, wife_candidate_report = generate_wife(draft=path)
            operand_recovery = wife_candidate_report['completionPass']
            origin = 'structured Wife phases with native Init and conversation CString lifetime recovery'
        elif relative.endswith('/NOVI_BarrelMan.lua'):
            source, operand_recovery = generate_barrel(draft=path)
            origin = 'structured Barrel resource/movie/marker candidate; staged runtime adapters'
        elif relative.endswith('/NOVI_Bully.lua'):
            source, bully_candidate_report = generate_bully(draft_path=path)
            operand_recovery = bully_candidate_report
            origin = 'verified Bully resource/movie/retained actor candidate; staged runtime adapters'
        elif relative.endswith('/NOVI_Guard.lua'):
            source, guard_candidate_report = generate_guard(draft_path=path)
            operand_recovery = guard_candidate_report
            origin = 'structured Guard native phase composition and copied-Thing Init; staged runtime adapters'
        elif relative.endswith('/NOVI_TeddyGirl.lua'):
            source, teddy_girl_candidate_report = generate_teddy_girl(draft_path=path)
            operand_recovery = teddy_girl_candidate_report
            origin = 'structured TeddyGirl native phase/lifecycle composition; staged runtime adapters'
        elif relative.endswith('/NOVI_Theresa.lua'):
            source, theresa_candidate_report = generate_theresa(draft_path=path)
            operand_recovery = theresa_candidate_report
            origin = 'structured Theresa phase/Init/dispatcher composition; staged runtime adapters'
        elif relative.endswith('/NOVI_LiveFather.lua'):
            source, live_father_candidate_report = generate_live_father(draft_path=path)
            operand_recovery = live_father_candidate_report
            origin = 'structured LiveFather native phase/Init composition; staged runtime adapters'
        elif relative.endswith('/NOVI_Victim.lua'):
            source, victim_candidate_report = generate_victim(draft_path=path)
            operand_recovery = victim_candidate_report
            origin = 'structured Victim native phase/Init composition; staged runtime adapters'
        elif relative.endswith('/OVI_DeadFather.lua'):
            source, dead_father_candidate_report = generate_dead_father(draft_path=path)
            operand_recovery = dead_father_candidate_report
            origin = 'structured DeadFather native phase/Init composition; staged runtime adapters'
        elif relative.endswith('/NOVI_BarrelThug.lua'):
            source, barrel_thug_candidate_report = generate_barrel_thug(draft_path=path)
            operand_recovery = barrel_thug_candidate_report
            origin = 'structured BarrelThug native phase/Init composition; staged runtime adapters'
        elif relative.endswith('/NewOakValeIntro.lua'):
            source, operand_recovery = recover_villager_quest_lists(source,RData())
            source, progress_report = lower_oakvale_progress(source)
            source, start_barrel_timer_report = lower_start_barrel_timer(source)
            source, watch_barrels_report = lower_watch_barrels(source)
            source, post_attack_cutscene_report = lower_post_attack_cutscene(source)
            source, post_attack_body_report = lower_post_attack_body(source)
            source, main_bindings_report = lower_main_bindings(source)
            source, initial_objective_report = lower_initial_objective(source)
            source, main_report = lower_oakvale_main(source)
            source, quest_markers_report = lower_quest_markers(source)
            source, deed_reports['quest'] = lower_oakvale_deeds(source)
            source, lifecycle_report = lower_oakvale_lifecycle(source)
            source, mission_report = lower_oakvale_mission(source)
            # Retail allocates both quest timers during Init.  Preserve those
            # handles so StartBarrelTimer and villager ambient speech can run.
            timer_init = ('function Init(quest)\n'
                          '    quest:SetStateInt("TalkIntermittentTimer", quest:RegisterTimer())\n'
                          '    quest:SetStateInt("WatchTimer", quest:RegisterTimer())\n'
                          '    quest:SetTimer(quest:GetStateInt("TalkIntermittentTimer"), 0)\n')
            source, count = re.subn(r'function Init\(quest\)\n', timer_init, source, count=1)
            if count != 1:
                raise ValueError('NewOakValeIntro Init boundary changed; timer registration not inserted')
            origin = 'raw converter with native-verified owned Villager speech-list initialization'
        elif relative.endswith('/native_quest_helpers.lua'):
            source, deed_reports['shared'] = lower_oakvale_deeds(source,shared=True)
            origin = 'structured native deed helpers shared with entity callers'
        elif relative.endswith('/NOVI_Villager.lua'):
            source, villager_candidate_report = generate_villager()
            operand_recovery = villager_candidate_report
            origin = 'structured Villager dialogue/resource composition; staged runtime and quest-owner adapters'
        if relative.endswith('/NOVI_AffairWife.lua'):
            # The wife generator already restores and verifies this condition.
            entry_condition = wife_candidate_report['entryCondition']
        elif relative.endswith('/NOVI_BarrelMan.lua'):
            entry_condition = operand_recovery['entryCondition']
        elif relative.endswith('/NOVI_Bully.lua'):
            entry_condition = bully_candidate_report['entryCondition']
        elif relative.endswith('/NOVI_Guard.lua'):
            entry_condition = guard_candidate_report['entryCondition']
        elif relative.endswith('/NOVI_TeddyGirl.lua'):
            entry_condition = teddy_girl_candidate_report['entry']
        elif relative.endswith('/NOVI_Villager.lua'):
            entry_condition = villager_candidate_report['entryCondition']
        elif relative.endswith('/NOVI_Theresa.lua'):
            entry_condition = theresa_candidate_report['entryCondition']
        elif relative.endswith('/NOVI_LiveFather.lua'):
            entry_condition = live_father_candidate_report['entryCondition']
        elif relative.endswith('/NOVI_Victim.lua'):
            entry_condition = victim_candidate_report['entryCondition']
        elif relative.endswith('/OVI_DeadFather.lua'):
            entry_condition = dead_father_candidate_report['entryCondition']
        elif relative.endswith('/NOVI_BarrelThug.lua'):
            entry_condition = barrel_thug_candidate_report['entryCondition']
        else:
            source, entry_condition = recover_entry_condition(Path(relative).stem, source)
        if relative.endswith('/NOVI_BookTrader.lua'):
            output, functions = readable_book_source(source)
        elif relative.endswith('/NOVI_AffairWoman.lua'):
            output, functions = readable_woman_source(source)
        elif relative.endswith('/NOVI_AffairWife.lua'):
            output, functions = readable_source(source, inline_literals=True)
            structure = {'status': 'structured Wife phases with recovered Init and conversation scope',
                         'mainImplementation': '__resource_main',
                         'unstructuredJoinsRetained': False,
                         'validationLimits': wife_candidate_report['completionPass']['limits']}
        else:
            output, functions = readable_source(source, inline_literals=True)
        if relative.endswith('/NOVI_Bully.lua'):
            structure = {'status': 'structured native-backed item, hit and runoff phase composition',
                         'mainImplementation': 'resourceBody',
                         'nativeMainHelpers': [f['function'] for f in functions
                                               if f['function'].startswith('Bully')],
                         'unstructuredJoinsRetained': False,
                         'validationLimits': 'Native acquisition/classifier comparisons and source-to-source whole-body checks; merged engine ownership/state/scheduler remain pending.'}
        elif relative.endswith('/NOVI_Guard.lua'):
            structure = {'status': 'native phase composition with explicit cancellation returns',
                         'mainImplementation': 'resourceBody',
                         'nativeMainHelpers': [f['function'] for f in functions
                                               if f['function'].startswith('Guard')],
                         'unstructuredJoinsRetained': False,
                         'validationLimits': guard_candidate_report['limits']}
        elif relative.endswith('/NOVI_Villager.lua'):
            structure = {'status': 'dialogue helpers composed with explicit cancellation and cleanup',
                         'mainImplementation': 'Main',
                         'nativeMainHelpers': ['handleVillagerAttackedDialogue','handleVillagerConversation','addVillagerAmbientLine'],
                         'unstructuredJoinsRetained': False,
                         'validationLimits': villager_candidate_report['remaining']}
        elif relative.endswith('/NOVI_TeddyGirl.lua'):
            structure = {'status': 'native phase composition with retained objects and explicit cancellation cleanup',
                         'mainImplementation': 'resourceBody',
                         'nativeMainHelpers': [f['function'] for f in functions
                             if f['function'].startswith('TeddyGirl') and f['function'] not in ('TeddyGirlInitialize','TeddyGirlGiven')],
                         'unstructuredJoinsRetained': False,
                         'validationLimits': teddy_girl_candidate_report['limits']}
        elif relative.endswith('/NOVI_Theresa.lua'):
            structure = {'status': 'readable native phase composition; dispatcher boundaries tested separately',
                         'mainImplementation': 'runTheresaMainAfterCondition',
                         'nativeMainHelpers': [f['function'] for f in functions
                             if f['function'] not in ('Main','Init','initializeTheresa','runTheresaMainAfterCondition')],
                         'unstructuredJoinsRetained': False,
                         'validationLimits': theresa_candidate_report['remaining']}
        elif relative.endswith('/NOVI_LiveFather.lua'):
            structure = {'status': 'native entry/intro/routine/payment/hit composition',
                         'mainImplementation': 'LiveFatherMain',
                         'nativeMainHelpers': [f['function'] for f in functions
                             if f['function'].startswith('LiveFather') and f['function'] not in ('LiveFatherMain','LiveFatherInit')],
                         'unstructuredJoinsRetained': False,
                         'validationLimits': live_father_candidate_report['limits']}
        elif relative.endswith('/NOVI_Victim.lua'):
            structure = {'status': 'native entry/subdued/talk/hit/repeat/complaint composition',
                         'mainImplementation': 'VictimMain',
                         'nativeMainHelpers': [f['function'] for f in functions
                             if f['function'].startswith('Victim') and f['function'] not in ('VictimMain','VictimInit')],
                         'unstructuredJoinsRetained': False,
                         'validationLimits': victim_candidate_report['pending']}
        elif relative.endswith('/OVI_DeadFather.lua'):
            structure = {'status': 'native full lifecycle, Init and empty predicate-fail override',
                         'mainImplementation': 'DeadFatherMain',
                         'nativeMainHelpers': [f['function'] for f in functions
                             if f['function'].startswith('DeadFather') and f['function'] not in ('DeadFatherMain','DeadFatherInit')],
                         'unstructuredJoinsRetained': False,
                         'validationLimits': dead_father_candidate_report['pending']}
        elif relative.endswith('/NOVI_BarrelThug.lua'):
            structure = {'status': 'native entry/intro/conversation/timed-remarks/hit composition',
                         'mainImplementation': 'runBarrelThugMainAfterCondition',
                         'nativeMainHelpers': [f['function'] for f in functions
                             if f['function'] not in ('Main','Init') and f['function'] not in ('runBarrelThugMainAfterCondition','initializeBarrelThug')],
                         'unstructuredJoinsRetained': False,
                         'validationLimits': barrel_thug_candidate_report['pending']}
        labels = {}
        if relative.endswith('/NOVI_AffairMan.lua'):
            structure = {'status': 'structured AffairMan phases with native Init and animation scope',
                         'mainImplementation': '__resource_main',
                         'unstructuredJoinsRetained': False,
                         'validationLimits': candidate_report['completePass']['pending']}
        elif relative.endswith('/NOVI_BookTrader.lua'):
            output, structure = structure_book(output)
            output, presentation = readable_book_conditions(output)
        elif relative.endswith('/NOVI_AffairWoman.lua'):
            output, structure = structure_woman(output)
            output, presentation = readable_woman_final(output)
        output, declaration_wraps = wrap_local_declarations(output)
        originals[relative] = source
        sources[relative] = output
        maps[relative] = {'origin': origin, 'inputSha256': hashlib.sha256(source.encode()).hexdigest(),
                          'outputSha256': hashlib.sha256(output.encode()).hexdigest(),
                          'functions': functions, 'labels': labels, 'structure': structure,
                          'declarationWraps': declaration_wraps, 'entryCondition': entry_condition}
        maps[relative]['operandRecovery'] = operand_recovery
        maps[relative]['helperReadability'] = helper_readability
        maps[relative]['presentation'] = presentation
    checker = LuaSyntaxChecker()
    syntax = checker.check(sources)
    # Renaming must preserve each file's syntax outcome, even for broken drafts.
    for path, output in sources.items():
        original = originals[path]
        if checker.check({path: original})['ok'] != checker.check({path: output})['ok']:
            raise ValueError('readability changed syntax status: ' + path)
    ledger = []
    for row in report['functions']:
        function = '__resource_main' if row['owner'] in ('NOVI_AffairMan', 'NOVI_BookTrader', 'NOVI_AffairWoman', 'NOVI_AffairWife', 'NOVI_BarrelMan') and row['function'] == 'Main' else row['function']
        if row['owner'] == 'NOVI_Bully' and row['function'] == 'Main':
            function = 'resourceBody'
        if row['owner'] == 'NOVI_Guard' and row['function'] == 'Main':
            function = 'resourceBody'
        if row['owner'] == 'NOVI_TeddyGirl':
            function = {'Main':'resourceBody','Init':'TeddyGirlInitialize','GivenTeddy':'TeddyGirlGiven'}.get(row['function'],row['function'])
        if row['owner'] == 'NOVI_Theresa':
            function = {'Main':'runTheresaMainAfterCondition','Init':'initializeTheresa'}.get(row['function'],row['function'])
        if row['owner'] == 'NOVI_LiveFather':
            function = {'Main':'LiveFatherMain','Init':'LiveFatherInit'}.get(row['function'],row['function'])
        if row['owner'] == 'NOVI_Victim':
            function = {'Main':'VictimMain','Init':'VictimInit'}.get(row['function'],row['function'])
        if row['owner'] == 'OVI_DeadFather':
            function = {'Main':'DeadFatherMain','Init':'DeadFatherInit','OnPredicateFail':'DeadFatherOnPredicateFail'}.get(row['function'],row['function'])
        if row['owner'] == 'NOVI_BarrelThug':
            function = {'Main':'runBarrelThugMainAfterCondition','Init':'initializeBarrelThug'}.get(row['function'],row['function'])
        mapping = next((f for f in maps[row['path']]['functions'] if f['function'] == function), None)
        bases = Counter(v['basis'] for v in mapping['locals'].values()) if mapping else Counter()
        if row['owner'] == 'NOVI_Bully' and row['function'] == 'Main':
            for helper in maps[row['path']]['functions']:
                if helper['function'].startswith('Bully'):
                    bases.update(v['basis'] for v in helper['locals'].values())
        if row['owner'] == 'NOVI_Guard' and row['function'] == 'Main':
            for helper in maps[row['path']]['functions']:
                if helper['function'].startswith('Guard'):
                    bases.update(v['basis'] for v in helper['locals'].values())
        if row['owner'] == 'NOVI_TeddyGirl' and row['function'] == 'Main':
            for helper in maps[row['path']]['functions']:
                if helper['function'].startswith('TeddyGirl') and helper['function'] not in ('TeddyGirlInitialize','TeddyGirlGiven'):
                    bases.update(v['basis'] for v in helper['locals'].values())
        presentation = maps[row['path']]['presentation']
        if row['function']=='Main' and presentation:
            bases['reused or unresolved native temporary'] -= presentation.get('removedScratchComparisons',0)
            removed=set(presentation.get('removedPredicateLocals',[]))
            if mapping:
                for local in mapping['locals'].values():
                    if local['name'] in removed:bases[local['basis']]-=1
        helper_maps = maps[row['path']]['helperReadability']
        if row['function'] == 'Main' and helper_maps:
            # These locals were named separately before the enclosing function;
            # retain their provenance and include them in their native Main row.
            for helper_functions in helper_maps['helperMaps'].values():
                for helper in helper_functions:
                    bases.update(v['basis'] for v in helper['locals'].values())
        ledger.append({'owner': row['owner'], 'function': row['function'], 'nativeAddress': row['address'],
                       'debugSymbolEvidence': pdb_symbols.get((row['owner'], row['function']), []),
                       'path': row['path'], 'rawDiagnostics': row['todo'],
                       'diagnosticClasses': dict(Counter(t.split(':', 1)[0] for t in row['todo'])),
                       'rawSyntaxPassed': row['syntax']['ok'], 'renamedLocals': sum(bases.values()),
                       'semanticNames': sum(n for k, n in bases.items() if k != 'reused or unresolved native temporary'),
                       'scratchNames': bases['reused or unresolved native temporary'],
                       'behaviorValidation': 'incomplete; see evidence and focused tests',
                       'entryCondition': maps[row['path']]['entryCondition'] if row['function']=='Main' else None,
                       'status': 'review-only-not-validated-complete'})
        if row['owner'] in ('NOVI_AffairMan', 'NOVI_AffairWife'):
            ledger[-1]['implementationFunction'] = function
        if row['function']=='destructor' and row['path'].endswith('/NewOakValeIntro.lua'):
            ledger[-1]['implementationFunction']='LuaQuestHost::Destructor'
            ledger[-1]['implementationLanguage']='C++'
            ledger[-1]['requiredNativeLifetime']='NewOakValeIntro'
        if row['function']=='RegisterMain' and row['path'].endswith('/NewOakValeIntro.lua'):
            ledger[-1]['implementationFunction']='LuaQuestHost::RegisterMain'
            ledger[-1]['implementationLanguage']='C++'
            ledger[-1]['requiredNativeLifetime']='NewOakValeIntro'
        if row['owner'] == 'NOVI_Bully':
            ledger[-1]['implementationFunction'] = function
        if row['owner'] == 'NOVI_Guard':
            ledger[-1]['implementationFunction'] = function
        if row['owner'] == 'NOVI_TeddyGirl':
            ledger[-1]['implementationFunction'] = function
        if row['owner'] == 'NOVI_Theresa':
            ledger[-1]['implementationFunction'] = function
        if row['owner'] == 'NOVI_LiveFather':
            ledger[-1]['implementationFunction'] = function
        if row['owner'] == 'NOVI_Victim':
            ledger[-1]['implementationFunction'] = function
        if row['owner'] == 'OVI_DeadFather':
            ledger[-1]['implementationFunction'] = function
        if row['owner'] == 'NOVI_BarrelThug':
            ledger[-1]['implementationFunction'] = function
    totals = {key: sum(row[key] for row in ledger) for key in ('renamedLocals', 'semanticNames', 'scratchNames')}
    result = {'schema': 'new-oakvale-readable-review/0.1', 'status': 'disabled-incomplete',
              'summary': dict(totals, functions=len(ledger), rawDiagnostics=report['summary']['todo']),
              'syntax': syntax, 'functions': ledger, 'sourceMap': maps,
              'husbandCandidate': candidate_report,
              'bookTraderCandidate': book_candidate_report,
              'womanCandidate': woman_candidate_report,
              'wifeCandidate': wife_candidate_report,
              'postAttackCutscene': post_attack_cutscene_report,
              'mainBindings': main_bindings_report,
              'initialObjective': initial_objective_report,
              'questMarkers': quest_markers_report,
              'startBarrelTimer': start_barrel_timer_report,
              'postAttackBody': post_attack_body_report,
              'hostLifecycle': lifecycle_report,
              'deedHelpers': deed_reports,
              'mission': mission_report,
              'progressHelpers': progress_report,
              'mainBody': main_report,
              'bullyCandidate': bully_candidate_report,
              'guardCandidate': guard_candidate_report,
              'teddyGirlCandidate': teddy_girl_candidate_report,
              'theresaCandidate': theresa_candidate_report,
              'liveFatherCandidate': live_father_candidate_report,
              'victimCandidate': victim_candidate_report,
              'deadFatherCandidate': dead_father_candidate_report,
              'barrelThugCandidate': barrel_thug_candidate_report,
              'watchBarrels': watch_barrels_report,
              'nativeEntryConditions': {'count': sum(m['entryCondition'] is not None for m in maps.values()),
                  'proposal': 'work/new_oakvale_conditions/condition-integration.patch',
                  'validation': 'x86 native predicate and real binding checks pass; full DLL/gameplay pending'},
              'pdbLocalCatalog': {'path': PDB_CATALOG.relative_to(ROOT).as_posix(),
                                  'sha256': hashlib.sha256(PDB_CATALOG.read_bytes()).hexdigest(),
                                  'use': 'original names/types/scopes; no automatic retail local mapping'} if pdb_catalog else None,
              'limits': ['Raw diagnostic counts are retained conservatively, including candidate-resolved items.',
                         'Renaming is reversible; semantic and unresolved scratch names are distinguished.',
                         'Syntax, readability and mocked behavior do not establish gameplay parity.']}
    lines = ['# New Oakvale Intro readable review package', '',
             'Disabled and incomplete. Generated from native conversion output; no working-port copying.',
             'Local names and provenance are in READABILITY_REPORT.json. Native comments remain unchanged.', '',
             'Original debug names/types/scopes are cataloged in `refs/script_recovery/new_oakvale_intro/pdb_locals.json`.',
             'The report links matching debug function symbols; their offsets are not retail offsets.', '',
             '| Owner | Function | Raw diagnostics | Semantic names | Scratch names |',
             '|---|---|---:|---:|---:|']
    for row in ledger:
        lines.append(f"| {row['owner']} | {row['function']} | {len(row['rawDiagnostics'])} | {row['semanticNames']} | {row['scratchNames']} |")
    lines += ['', 'Summary: `' + json.dumps(result['summary']) + '`', '',
              f"Syntax: {syntax['passed']}/{syntax['checked']} files. Remaining failures:", '']
    lines.extend('- ' + item['message'] for item in syntax['errors'])
    out = Path(out)
    if out.resolve() == raw.resolve():
        raise ValueError('readable output must be separate from the raw evidence package')
    for relative, source in sources.items():
        destination = out / relative
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_text(source, encoding='utf-8')
    # No custom quests: NewOakValeIntro is loaded through retail_override.lua.
    (out / 'FSE/quests.lua').write_text('Quests = {}\n', encoding='utf-8')
    (out / 'READABILITY_REPORT.json').write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
    (out / 'README.md').write_text('\n'.join(lines) + '\n', encoding='utf-8')
    return result


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out', type=Path, default=RAW / 'readable')
    args = parser.parse_args()
    result = build(args.out)
    print(json.dumps({'summary': result['summary'], 'syntax': result['syntax']}, indent=2))
