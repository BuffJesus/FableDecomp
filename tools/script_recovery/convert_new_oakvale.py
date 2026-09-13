"""Generate and audit the entire inventoried New Oakvale translation unit, without port copying."""
from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.script_recovery.lift_native_lua import (
    Lifter, RData, annotate, known_callee_aliases, load_manifest, load_slots, load_thing_tables,
    thing_signatures, load_entity_state, load_entity_parent_state, load_entity_constants, lift_persist,
)
from tools.script_recovery.benchmark_lifter import LuaSyntaxChecker
from tools.script_recovery.native_function_parameters import function_parameters, rename_parameters
from tools.script_recovery.native_speech_vectors import recover_vectors, vector_prelude, SOURCE_HASH, NATIVE_HASH

EVIDENCE = ROOT / 'refs/script_recovery/new_oakvale_intro'
OUTPUT = ROOT / 'refs/script_recovery/lifted/NewOakValeIntro'
ENTITY_STATE = '''local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end
'''


def inventories(root=EVIDENCE):
    return [json.loads(p.read_text(encoding='utf-8-sig')) for p in sorted((root / 'entities').glob('*.json'))]


def convert(out=OUTPUT):
    manifest, slots, rdata = load_manifest(), load_slots(), RData()
    things, returning = load_thing_tables(manifest, slots)
    tu = json.loads((EVIDENCE / 'translation_unit.json').read_text(encoding='utf-8-sig'))
    by_address = {f['address'].lower(): f for f in tu['functions']}
    entries = inventories()
    quest_inventory = next(e for e in entries if e.get('kind') == 'quest')
    quest_helpers = {f['address'].lower(): name for name, f in quest_inventory['functions'].items()
                     if name not in ('RegisterMain', 'Main', 'Init', 'OnPersist', 'destructor')}
    shared_names, shared_inputs = set(), {}
    shared_module = 'NewOakValeIntro.native_quest_helpers'
    fields = json.loads((EVIDENCE / 'persistence_manifest.json').read_text())['fields']
    quest_state = {f['retailOffset'].lower(): (f['name'], {'bool': 'Bool', 'int': 'Int', 'float': 'Float'}[f['type']])
                   for f in fields if f['type'] in ('bool', 'int', 'float')}
    quest_arrays = {f['retailOffset'].split('..')[0].lower(): (f['name'], 'Bool')
                    for f in fields if re.fullmatch(r'bool\[\d+\]', f['type'])}
    checker = LuaSyntaxChecker()
    report = {'schema': 'new-oakvale-converter-coverage/0.1', 'source': 'native exports and reviewed inventories',
              'packages': [], 'functions': [], 'aliases': [], 'missing': [], 'syntax': {}}
    all_sources = {}
    for inventory in entries:
        owner = inventory['entity']
        entity = inventory.get('kind') != 'quest'
        state = load_entity_state(owner) if entity else quest_state
        static_vectors = recover_vectors(rdata.bytes_at) if owner == 'NOVI_Villager' or not entity else {}
        master_candidates = {}
        for access in inventory.get('masterDataAccess', []):
            if access.get('op') == 'write' and isinstance(access.get('value'), str):
                master_candidates.setdefault(access['offset'].lower(), set()).add(access['field'])
        master_strings = {offset: next(iter(names)) for offset, names in master_candidates.items() if len(names) == 1}
        lifter = Lifter(manifest, state, 'quest', entity, 'NewOakValeIntro', rdata,
                        thing_sigs=thing_signatures(things),
                        parent_state=load_entity_parent_state(owner) if entity else {},
                        native_constants=load_entity_constants(owner),
                        state_receiver='__native_entity_state' if entity else 'quest',
                        live_termination=True, state_arrays=quest_arrays if not entity else {},
                        master_strings=master_strings, native_gotos=True, static_vectors=static_vectors, readable_locals=True)
        # Address identity, not similarity-derived labels, determines local helper names.
        local_names = {f['address'].lower(): name for name, f in inventory['functions'].items()
                       if re.fullmatch(r'[A-Za-z_]\w*', name) and not f.get('coverageAlias')}
        lifter.helper_names = set(local_names.values())
        signatures = {}
        for address, helper in local_names.items():
            fn = by_address.get(address)
            if fn and fn.get('decompile'):
                signatures[helper] = function_parameters(fn['decompile'], member=True)
                lifter.helper_parameters[helper] = [p['lua'] for p in signatures[helper]['parameters']]
                if signatures[helper]['returnKind']:
                    lifter.helper_return_kinds[helper] = signatures[helper]['returnKind']
        relative = f'FSE/NewOakValeIntro/Entities/{owner}.lua' if entity else 'FSE/NewOakValeIntro/NewOakValeIntro.lua'
        chunks = [f'-- Generated native draft: {owner}. Review coverage report before use.',
                  '-- Not copied from the working port; registration remains disabled.', '']
        if entity:
            chunks.append(ENTITY_STATE)
        if static_vectors:
            chunks.append(vector_prelude(static_vectors))
            report.setdefault('staticVectors', {})[owner] = {
                'initAddress': '0x00DAADD0', 'sourceSha256': SOURCE_HASH, 'nativeSha256': NATIVE_HASH,
                'offsets': list(static_vectors), 'keys': sum(len(keys) for keys in static_vectors.values())}
        for name, spec in inventory['functions'].items():
            if spec.get('coverageAlias'):
                report['aliases'].append({'owner': owner, 'function': name, 'alias': spec['coverageAlias']})
                continue
            row = {'owner': owner, 'function': name, 'address': spec['address'], 'path': relative}
            fn = by_address.get(spec['address'].lower())
            if not fn or not fn.get('decompile'):
                path = ROOT / spec['decompileFile']
                if path.is_file():
                    fn = {'address': spec['address'], 'decompile': path.read_text(encoding='utf-8-sig'), 'calls': []}
                    row['source'] = spec['decompileFile']
                else:
                    report['missing'].append(row)
                    continue
            else:
                row['source'] = 'translation_unit.json'
            lifter.callee_names = known_callee_aliases(fn)
            lifter.parent_helpers = {}
            if entity:
                parent_candidates = {}
                for call in fn.get('calls', []):
                    address = str(call.get('target', '')).lower()
                    if not call.get('currentName'):
                        continue
                    for label in (call['currentName'], call['currentName'].split('::')[-1]):
                        parent_candidates.setdefault(label, set()).add(address)
                for label, addresses in parent_candidates.items():
                    if len(addresses) != 1:
                        continue
                    address = next(iter(addresses))
                    if address not in quest_helpers:
                        continue
                    helper = quest_helpers[address]
                    helper_signature = function_parameters(by_address[address]['decompile'], member=True)
                    if helper_signature.get('problem'):
                        continue
                    lifter.parent_helpers[label] = {'module': shared_module, 'name': helper,
                        'arity': len(helper_signature['parameters']), 'returnKind': helper_signature['returnKind']}
                    shared_names.add(helper)
            candidates = {}
            for call in fn.get('calls', []):
                if call.get('currentName') and str(call.get('target', '')).lower() in local_names:
                    candidates.setdefault(call['currentName'], set()).add(local_names[call['target'].lower()])
            lifter.callee_names.update({label: next(iter(names)) for label, names in candidates.items() if len(names) == 1})
            signature = signatures.get(name) or function_parameters(fn['decompile'], member=True)
            source = annotate(rename_parameters(fn['decompile'], signature), slots, things, returning, entity=entity)
            if name == 'OnPersist':
                body, _, calls = lift_persist(source, 'quest')
                todo = []
                params = 'quest, context'
            else:
                parameter_kinds = {p['lua']: 'number' if p['type'] in ('long', 'int', 'uint', 'EBadDeeds')
                                   else 'bool' if p['type'] == 'bool' else 'unknown'
                                   for p in signature['parameters']}
                body = lifter.lift(name, source, native_function=fn, parameters=parameter_kinds)
                calls, todo = list(lifter.calls), list(lifter.todo)
                params = 'quest, me' if entity else 'quest'
                params += ''.join(', ' + p['lua'] for p in signature['parameters'])
                if signature.get('problem'):
                    todo.append(signature['problem'])
            row['nativeSignature'] = signature
            function_source = '\n'.join([f'function {name}({params})'] + body + ['end', ''])
            if not entity and name in quest_helpers.values():
                shared_inputs[name] = (source, fn, signature, dict(lifter.callee_names))
            row.update(todo=todo, calls=calls, lines=len(body), syntax=checker.check({relative + ':' + name: function_source}))
            if name != 'OnPersist':
                row['selfWrapperArguments'] = lifter.self_wrapper_arguments
                row['literalByteComparisons'] = lifter.literal_comparisons
                row['presentedItemEvidence'] = lifter.presented_item_evidence
                row['teddyDistanceEvidence'] = lifter.teddy_distance_evidence
                row['theresaDistanceEvidence'] = lifter.theresa_distance_evidence
                row['entryTimerEvidence'] = lifter.entry_timer_evidence
                row['loopTimerEvidence'] = lifter.loop_timer_evidence
                row['barrelDistanceEvidence'] = lifter.barrel_distance_evidence
                row['barrelColourEvidence'] = lifter.barrel_colour_evidence
                row['barrelUpdateEvidence'] = lifter.barrel_update_evidence
                row['barrelCreationEvidence'] = lifter.barrel_creation_evidence
                row['questMarkerEvidence'] = lifter.quest_marker_evidence
                row['markerCleanupEvidence'] = lifter.marker_cleanup_evidence
                row['postAttackCleanupEvidence'] = lifter.post_attack_cleanup_evidence
                row['postAttackScalarEvidence'] = lifter.post_attack_scalar_evidence
                row['postAttackLogbookEvidence'] = lifter.post_attack_logbook_evidence
                row['postAttackTeleportEvidence'] = lifter.post_attack_teleport_evidence
                row['postAttackDistanceEvidence'] = lifter.post_attack_distance_evidence
                row['postAttackMovieFlagEvidence'] = lifter.post_attack_movie_flag_evidence
                row['postAttackLimboEvidence'] = lifter.post_attack_limbo_evidence
                row['postAttackResourceEvidence'] = lifter.post_attack_resource_evidence
                row['bookTraderResourceEvidence'] = lifter.book_trader_resource_evidence
                row['postAttackMovieLifetimeEvidence'] = lifter.post_attack_movie_lifetime_evidence
                row['barrelPositionEvidence'] = lifter.barrel_position_evidence
                row['guardCleanupEvidence'] = lifter.guard_cleanup_evidence
                row['affairManDistanceEvidence'] = lifter.affair_man_distance_evidence
                row['affairManWomanDistanceEvidence'] = lifter.affair_man_woman_distance_evidence
                row['cachedThingQueryEvidence'] = lifter.cached_thing_query_evidence
                row['affairFacingEvidence'] = lifter.affair_facing_evidence
                row['affairPauseEvidence'] = lifter.affair_pause_evidence
                row['affairMovieEvidence'] = lifter.affair_movie_evidence
                row['bookTraderPauseEvidence'] = lifter.book_trader_pause_evidence
                row['bookTraderRandomEvidence'] = lifter.book_trader_random_evidence
                row['bookTraderTimerEvidence'] = lifter.book_trader_timer_evidence
                row['bookTraderLineEvidence'] = lifter.book_trader_line_evidence
                row['bookTraderHealthEvidence'] = lifter.book_trader_health_evidence
                row['bookTraderAlliesEvidence'] = lifter.book_trader_allies_evidence
                row['bookTraderMovieEvidence'] = lifter.book_trader_movie_evidence
                row['bookTraderCleanupEvidence'] = lifter.book_trader_cleanup_evidence
                row['bookTraderHitEvidence'] = lifter.book_trader_hit_evidence
                row['bookTraderTerminationEvidence'] = lifter.book_trader_termination_evidence
                row['bookTraderAcquisitionEvidence'] = lifter.book_trader_acquisition_evidence
                row['speechVectorInitEvidence'] = lifter.speech_vector_init_evidence
                row['initScalarEvidence'] = lifter.init_scalar_evidence
                row['initBadDeedEvidence'] = lifter.init_bad_deed_evidence
                row['affairWifeRouteEvidence'] = lifter.affair_wife_route_evidence
                row['affairWifeDistanceEvidence'] = lifter.affair_wife_distance_evidence
                row['affairWifeHeroDistanceEvidence'] = lifter.affair_wife_hero_distance_evidence
                row['affairWifeHitEvidence'] = lifter.affair_wife_hit_evidence
                row['affairWifePauseEvidence'] = lifter.affair_wife_pause_evidence
                row['affairWifeMovieEvidence'] = lifter.affair_wife_movie_evidence
                row['affairWifeCleanupEvidence'] = lifter.affair_wife_cleanup_evidence
                row['affairWifeArgumentHitEvidence'] = lifter.affair_wife_argument_hit_evidence
                row['affairWifeMaskEvidence'] = lifter.affair_wife_mask_evidence
                row['affairWifeFacingEvidence'] = lifter.affair_wife_facing_evidence
                row['affairWifeAllyEvidence'] = lifter.affair_wife_ally_evidence
                row['affairWifeAcquisitionEvidence'] = lifter.affair_wife_acquisition_evidence
                row['affairWifeTerminationEvidence'] = lifter.affair_wife_termination_evidence
                row['affairWifeTimerEvidence'] = lifter.affair_wife_timer_evidence
                row['affairWifeRandomEvidence'] = lifter.affair_wife_random_evidence
                row['affairWifeConversationEvidence'] = lifter.affair_wife_conversation_evidence
                row['affairWifeRunningLineEvidence'] = lifter.affair_wife_running_line_evidence
                row['affairWifeArgumentConversationEvidence'] = lifter.affair_wife_argument_conversation_evidence
                row['affairWifeTextSequenceEvidence'] = lifter.affair_wife_text_sequence_evidence
                row['affairWifeReplyEvidence'] = lifter.affair_wife_reply_evidence
                row['affairWifeHealthEvidence'] = lifter.affair_wife_health_evidence
                row['affairWifeAnimationRandomEvidence'] = lifter.affair_wife_animation_random_evidence
                row['affairWifeAnimationOperandsEvidence'] = lifter.affair_wife_animation_operands_evidence
                row['affairWifePositionEvidence'] = lifter.affair_wife_position_evidence
                row['affairManHealthEvidence'] = lifter.affair_man_health_evidence
                row['affairManHitEvidence'] = lifter.affair_man_hit_evidence
                row['affairManMaskEvidence'] = lifter.affair_man_mask_evidence
                row['affairManConversationEvidence'] = lifter.affair_man_conversation_evidence
                row['affairManNearLinesEvidence'] = lifter.affair_man_near_lines_evidence
                row['affairManRangedLinesEvidence'] = lifter.affair_man_ranged_lines_evidence
                row['affairManAffectionSetupEvidence'] = lifter.affair_man_affection_setup_evidence
                row['affairManAnimationOperandsEvidence'] = lifter.affair_man_animation_operands_evidence
                row['affairManHeroDistanceEvidence'] = lifter.affair_man_hero_distance_evidence
                row['affairManQuestionEvidence'] = lifter.affair_man_question_evidence
                row['affairManQuestionCleanupEvidence'] = lifter.affair_man_question_cleanup_evidence
                row['affairManHomeEvidence'] = lifter.affair_man_home_evidence
                row['affairManQuestionEntryEvidence'] = lifter.affair_man_question_entry_evidence
                row['affairManTalkContinuationEvidence'] = lifter.affair_man_talk_continuation_evidence
                row['affairManSpeechEvidence'] = lifter.affair_man_speech_evidence
                row['affairManResourceEvidence'] = lifter.affair_man_resource_evidence
                row['readableLocalNames'] = lifter.readable_local_names
                row['randomEvidence'] = lifter.random_evidence
                row['deedEvidence'] = lifter.deed_evidence
                row['nativeJumps'] = sorted(lifter.lua_jumps)
                row['nativeLabels'] = sorted(lifter.lua_labels)
            report['functions'].append(row)
            chunks.append(function_source)
        source = '\n'.join(chunks)
        destination = out / relative
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_text(source + '\n', encoding='utf-8')
        all_sources[relative] = source
        report['packages'].append({'owner': owner, 'path': relative})
    # Include transitive quest helper dependencies; forward locals keep calls within this VM.
    changed = True
    while changed:
        changed = False
        for address, helper in quest_helpers.items():
            if helper not in shared_names:
                continue
            for call in by_address[address].get('calls', []):
                dependency = quest_helpers.get(str(call.get('target', '')).lower())
                if dependency and dependency not in shared_names:
                    shared_names.add(dependency)
                    changed = True
    shared_path = 'FSE/NewOakValeIntro/native_quest_helpers.lua'
    shared_sources, shared_diagnostics = {}, {}
    for name in sorted(shared_names):
        source, fn, signature, aliases = shared_inputs[name]
        shared_lifter = Lifter(manifest, quest_state, 'quest', False, 'NewOakValeIntro', rdata,
                               callee_names=aliases, thing_sigs=thing_signatures(things),
                               live_termination=True, execution_entity=True, state_arrays=quest_arrays,
                               native_gotos=True, readable_locals=True)
        shared_lifter.helper_names = set(quest_helpers.values())
        for address, helper in quest_helpers.items():
            sig = function_parameters(by_address[address]['decompile'], member=True)
            shared_lifter.helper_parameters[helper] = [p['lua'] for p in sig['parameters']]
            if sig['returnKind']:
                shared_lifter.helper_return_kinds[helper] = sig['returnKind']
        kinds = {p['lua']: 'number' if p['type'] in ('long', 'int', 'uint', 'EBadDeeds') else 'unknown'
                 for p in signature['parameters']}
        body = shared_lifter.lift(name, source, native_function=fn, parameters=kinds)
        params = 'quest, me' + ''.join(', ' + p['lua'] for p in signature['parameters'])
        shared_sources[name] = '\n'.join([f'function {name}({params})'] + body + ['end', ''])
        shared_diagnostics[name] = list(shared_lifter.todo)
    shared = '\n'.join(['-- Generated from the same native helper bodies as the quest draft.',
                        'local ' + ', '.join(sorted(shared_names))] +
                       [shared_sources[name] for name in sorted(shared_names)] +
                       ['return {' + ', '.join(f'{n} = {n}' for n in sorted(shared_names)) + '}\n'])
    (out / shared_path).write_text(shared, encoding='utf-8')
    report['sharedHelpers'] = {'path': shared_path, 'functions': sorted(shared_names),
                               'todo': shared_diagnostics, 'executionContext': 'calling entity thread',
                               'syntax': checker.check({shared_path: shared})}
    all_sources[shared_path] = shared
    report['syntax'] = checker.check(all_sources)
    report['summary'] = {'owners': len(entries), 'functions': len(report['functions']),
                         'missing': len(report['missing']),
                         'functionSyntaxPassed': sum(r['syntax']['passed'] for r in report['functions']),
                         'fileSyntaxPassed': report['syntax']['passed'],
                         'fileSyntaxChecked': report['syntax']['checked'],
                         'todo': sum(len(r['todo']) for r in report['functions'])}
    (out / 'FSE/quests.lua').write_text('-- Generated drafts are not enabled.\nQuests = {}\n', encoding='utf-8')
    (out / 'CONVERSION_REPORT.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    lines = ['# Full New Oakvale native conversion coverage', '',
             'Compilation is not behavioral parity. Missing bodies and unresolved operations prevent completion.', '',
             '| Owner | Function | Address | Compiles | TODO |', '|---|---|---|---|---:|']
    for row in report['functions']:
        lines.append(f"| {row['owner']} | {row['function']} | {row['address']} | {row['syntax']['passed'] == 1} | {len(row['todo'])} |")
    lines += ['', 'Shared native helper module: `' + shared_path + '`; functions: ' +
              ', '.join(sorted(shared_names)) + '. Its diagnostics are included in file syntax totals.',
              '', 'Summary: `' + json.dumps(report['summary']) + '`', '']
    (out / 'CONVERSION_REPORT.md').write_text('\n'.join(lines), encoding='utf-8')
    return report


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out', type=Path, default=OUTPUT)
    args = parser.parse_args()
    result = convert(args.out)
    print(json.dumps(result['summary'], indent=2))
