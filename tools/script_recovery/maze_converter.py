"""Generate an inactive Maze converter candidate with separately reported gaps."""
import json
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.maze_native import recover
from tools.script_recovery.maze_lifecycle_audit import audit as audit_lifecycle
from tools.script_recovery.lift_native_lua import (
    ROOT, CLUSTERS, RData, Lifter, annotate, load_manifest, load_slots,
    load_thing_tables, thing_signatures)


def convert(output=None):
    output = Path(output or ROOT / 'work/maze_converter/draft').resolve()
    if not output.is_relative_to((ROOT / 'work/maze_converter').resolve()):
        raise ValueError('Maze candidate must stay under work/maze_converter')
    bodies, witness = recover()
    lifecycle = audit_lifecycle()
    files = {}
    for owner in ('HistoryBookcase', 'EmptyGrave'):
        files['Entities/' + owner + '.lua'] = '\n'.join(
            'function ' + role + '(quest, me)\n' + bodies[owner + '.' + role] + 'end\n'
            for role in ('Init', 'Main'))
    manifest, slots, data = load_manifest(), load_slots(), RData()
    things, returning = load_thing_tables(manifest, slots)
    lifter = Lifter(manifest, {'0x48': ('SwordTaken', 'Bool'), '0x49': ('BookRead', 'Bool')},
        'quest', False, 'MazeResearch', data, thing_sigs=thing_signatures(things),
        live_termination=True, readable_locals=True, native_gotos=True)
    cluster = json.loads((CLUSTERS / 'V_MazeResearch.json').read_text(encoding='utf-8-sig'))
    root, diagnostics = [], {}
    for fn in cluster['lifecycle']:
        if fn['role'] not in ('Init', 'Main', 'OnPersist'):
            continue
        if fn['role'] == 'OnPersist':
            root += ['function OnPersist(quest, context)',
                '    quest:SetStateBool("SwordTaken", quest:PersistTransferBool(context, "SwordTaken", quest:GetStateBool("SwordTaken"), false))',
                '    quest:SetStateBool("BookRead", quest:PersistTransferBool(context, "BookRead", quest:GetStateBool("BookRead"), false))',
                'end', '']
            diagnostics['OnPersist'] = []
            continue
        body = lifter.lift(fn['role'], annotate(fn['decompile'], slots, things, returning, entity=False))
        root += ['function ' + fn['role'] + '(quest)', *body, 'end', '']
        diagnostics[fn['role']] = lifter.todo[:]
    root += ['function UnLimboSword(quest)', bodies['MazeResearch.UNLIMBO'], 'end']
    files['MazeResearch.lua'] = '\n'.join(root) + '\n'
    report = {'registrationEnabled': False, 'status': 'isolated converter candidate; pending integration',
        'nativeEvidence': witness, 'lifecycleAudit': lifecycle, 'rootDiagnostics': diagnostics, 'syntax': {},
        'pendingCapabilities': ['AddMiniMapMarkerByScriptName'],
        'remaining': witness['review']['limits'] + [
            'Root lifecycle is generic-lifter output, not audited as complete.',
            'Retained Sword native-reference release at quest destruction/persistence requires host review.']}
    for name, source in files.items():
        LuaRuntime().execute('return function()\n' + source + '\nend')
        report['syntax'][name] = 'Lua 5.4 passed'
        path = output / name
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text('-- Disabled Maze native candidate; see COVERAGE.json.\n' + source)
    (output / 'quests.lua').write_text('-- Registration disabled: pending semantic and host integration review.\n')
    (output / 'COVERAGE.json').write_text(json.dumps(report, indent=2) + '\n')
    return report


if __name__ == '__main__':
    result = convert()
    print(json.dumps({key: result[key] for key in ('status', 'syntax', 'rootDiagnostics', 'pendingCapabilities')}, indent=2))
