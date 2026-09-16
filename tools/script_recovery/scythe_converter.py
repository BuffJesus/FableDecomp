"""Generate a disabled ScytheInfo draft without touching canonical ports."""
import argparse
import hashlib
import json
from pathlib import Path

from tools.script_recovery.lift_native_lua import (ROOT, CLUSTERS, RData, Lifter,
    annotate, load_manifest, load_slots, load_thing_tables, thing_signatures)
from tools.script_recovery.native_scythe_main import recover_scythe_main
from tools.script_recovery.native_scythe_entities import recover_scythe_entity, recover_scythe_behavior
from tools.script_recovery.scythe_cutscene_audit import audit as audit_cutscene_adapter


def convert(output=None):
    output = Path(output) if output else ROOT / 'work/scythe_converter/draft'
    # This converter is intentionally separate from canonical generated ports.
    allowed = (ROOT / 'work/scythe_converter').resolve()
    if not output.resolve().is_relative_to(allowed):
        raise ValueError('Scythe draft output must stay under work/scythe_converter')
    cluster = json.loads((CLUSTERS / 'QS_ScytheInfo.json').read_text(encoding='utf-8-sig'))
    manifest, slots, data = load_manifest(), load_slots(), RData()
    things, returning = load_thing_tables(manifest, slots)
    functions = {f['role']: f for f in cluster['lifecycle']}
    sources = {name: annotate(fn['decompile'], slots, things, returning, entity=False)
               for name, fn in functions.items() if name in ('Init', 'Main')}
    sources['Main'], evidence = recover_scythe_main(functions['Main'], sources['Main'], data, manifest)
    if not evidence or evidence[0]['status'] != 'recovered':
        raise ValueError('Scythe native correspondence rejected: ' + repr(evidence))
    if hashlib.sha256(functions['Init']['decompile'].encode()).hexdigest() != evidence[0]['initSourceSha256']:
        raise ValueError('Scythe Init source correspondence changed')
    state = {offset: tuple(field) for offset, field in evidence[0]['state'].items()}
    lifter = Lifter(manifest, state, 'Quest', False, 'ScytheInfo', data,
                    thing_sigs=thing_signatures(things), live_termination=True,
                    native_gotos=True, readable_locals=True)
    report = {'script': 'QS_ScytheInfo', 'registrationEnabled': False,
              'nativeMainEvidence': evidence, 'functions': {},
              'cutsceneAdapterAudit': audit_cutscene_adapter(data),
              'remaining': ['ScytheNearOracle Main and PlayCutscene require further operand and lifetime recovery.']}
    chunks = ['-- Disabled native ScytheInfo draft; see COVERAGE.json.', 'local Quest', '']
    for name in ('Init', 'Main'):
        body = lifter.lift(name, sources[name])
        chunks += ['function ' + name + '(questObject)', '    Quest = questObject', *body, 'end', '']
        report['functions'][name] = {'todo': lifter.todo[:], 'calls': lifter.calls[:]}
    text = '\n'.join(chunks)
    from lupa.lua54 import LuaRuntime
    LuaRuntime().execute('return function()\n' + text + '\nend')
    report['syntax'] = 'Lua 5.4 passed'
    output.mkdir(parents=True, exist_ok=True)
    (output / 'ScytheInfo.lua').write_text(text, encoding='utf-8')
    report['entities'] = {}
    for owner, roles in (('ScytheMarker', [('Main', '0x00E29F50')]),
                         ('ScytheNearOracle', [('Init', '0x00E2A0F0'), ('Main', '0x00E2A320')])):
        entity_lifter = Lifter(manifest, {}, 'quest', True, 'ScytheInfo', data,
            thing_sigs=thing_signatures(things), parent_state=state,
            live_termination=True, native_gotos=True, readable_locals=True,
            callee_names={'CCreatureAction_TrollWhackGroundBase::Initialise':
                          'IsActiveThreadTerminating',
                          # E2A5CF targets the script's E2A900 helper, whose
                          # actor-resource map differs from the host binding.
                          'PlayCutscene': 'UnresolvedScythePlayCutscene'})
        body_chunks = ['-- Disabled native entity draft; see COVERAGE.json.', '']
        entity_report = {}
        if owner == 'ScytheMarker':
            body_chunks += ['-- Native Init uses the shared empty entity initializer.',
                            'function Init(quest, me)', 'end', '']
        for role, address in roles:
            raw = (ROOT / 'work/scythe_converter/evidence/decompiles' / (address + '.c')).read_text()
            fn = {'address': address, 'decompile': raw}
            source = annotate(raw, slots, things, returning, entity=True)
            source, checked = recover_scythe_entity(fn, source, data, manifest)
            if checked and checked[0]['status'] != 'recovered':
                raise ValueError('Scythe entity correspondence rejected: ' + repr(checked))
            source, behavior = recover_scythe_behavior(fn, source, data, manifest)
            if behavior and behavior[0]['status'] != 'recovered':
                raise ValueError('Scythe behavior correspondence rejected: ' + repr(behavior))
            body = entity_lifter.lift(role, source)
            body_chunks += ['function ' + role + '(quest, me)', *body, 'end', '']
            entity_report[role] = {'todo': entity_lifter.todo[:], 'evidence': checked, 'behaviorEvidence': behavior}
        entity_text = '\n'.join(body_chunks)
        try:
            LuaRuntime().execute('return function()\n' + entity_text + '\nend')
            entity_report['syntax'] = 'Lua 5.4 passed'
        except Exception as error:
            entity_report['syntax'] = str(error)
        (output / 'Entities').mkdir(exist_ok=True)
        (output / 'Entities' / (owner + '.lua')).write_text(entity_text, encoding='utf-8')
        report['entities'][owner] = entity_report
    (output / 'quests.lua').write_text('-- Registration disabled: incomplete native recovery.\n', encoding='utf-8')
    (output / 'COVERAGE.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    return report


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--out', type=Path)
    args = parser.parse_args()
    report = convert(args.out)
    print(json.dumps({'registrationEnabled': False, 'questSyntax': report['syntax'],
        'questDiagnostics': {name: len(row['todo']) for name, row in report['functions'].items()},
        'entities': {name: {'syntax': row['syntax'],
            'diagnostics': {role: len(details['todo']) for role, details in row.items()
                            if isinstance(details, dict) and 'todo' in details}}
            for name, row in report['entities'].items()}}, indent=2))
