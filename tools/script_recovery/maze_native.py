"""Fail-closed correspondence for the isolated retail Maze entity recovery."""
import hashlib
import json
from dataclasses import asdict
from pathlib import Path

from tools.script_recovery.lift_native_lua import (
    ROOT, CLUSTERS, RData, Lifter, annotate, load_manifest, load_slots,
    load_thing_tables, thing_signatures)
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.maze_program import EMPTY_INIT, EMPTY_MAIN, UNLIMBO


def recover(data=None):
    data = data or RData()
    witness = json.loads(Path(__file__).with_name('maze_native_witness.json').read_text())
    persist = witness['persist']
    raw = data.bytes_at(persist['address'], persist['size'])
    cluster = json.loads((CLUSTERS / 'V_MazeResearch.json').read_text(encoding='utf-8-sig'))
    original = next(fn for fn in cluster['lifecycle'] if fn['role'] == 'OnPersist')
    if (raw is None or hashlib.sha256(raw).hexdigest() != persist['nativeSha256'] or
        hashlib.sha256(original['decompile'].encode()).hexdigest() != persist['sourceSha256']):
        raise ValueError('Maze persistence correspondence changed')
    sources = {}
    for name, fn in witness['functions'].items():
        raw = data.bytes_at(fn['address'], fn['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != fn['nativeSha256']:
            raise ValueError('Maze native function changed: ' + name)
        source = (ROOT / fn['path']).read_text()
        if hashlib.sha256(source.encode()).hexdigest() != fn['decompileSha256']:
            raise ValueError('Maze decompile correspondence changed: ' + name)
        sources[name] = source
    for region in witness['regions']:
        raw = data.bytes_at(region['address'], region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != region['sha256']:
            raise ValueError('Maze native supporting region changed')
    for address, expected in witness['strings'].items():
        actual = ('' if expected == '' and data.bytes_at(int(address, 16), 1) == b'\0'
                  else data.string_at(int(address, 16)))
        if actual != expected:
            raise ValueError('Maze native literal changed: ' + address)
    for call in witness['calls']:
        actual = read_call_window(data, call['base'], call['size'], call['site'],
            {int(k): tuple(v) for k, v in call['known'].items()}, argument_count=call['count'])
        if actual is None or json.loads(json.dumps(asdict(actual))) != call['expected']:
            raise ValueError('Maze call operands changed: ' + hex(call['site']))
    manifest, slots = load_manifest(), load_slots()
    things, returning = load_thing_tables(manifest, slots)
    lifter = Lifter(manifest, {}, 'quest', True, 'MazeResearch', data,
        thing_sigs=thing_signatures(things),
        parent_state={'0x48': ('SwordTaken', 'Bool'), '0x49': ('BookRead', 'Bool')},
        live_termination=True, native_gotos=True, readable_locals=True,
        callee_names={'CCreatureAction_TrollWhackGroundBase::Initialise': 'IsActiveThreadTerminating'})
    source = annotate(sources['HistoryBookcase.Main'], slots, things, returning, entity=True)
    old = 'pCVar5 = pCVar1;\n    GSI->MiniMapAddMarker();'
    if source.count(old) != 1:
        raise ValueError('Maze initial marker source correspondence changed')
    source = source.replace(old, 'GSI->MiniMapAddMarker(this + 8,"HUD_ORB_QUEST_VIGNETTE");')
    body = '\n'.join(lifter.lift('Main', source)) + '\n'
    old = ('        ppVar4 = quest:GetThingWithScriptName("EmptyGrave")\n'
           '        quest:MiniMapAddMarker(ppVar4, "HUD_ORB_QUEST_VIGNETTE")')
    if body.count(old) != 1 or lifter.todo:
        raise ValueError('Maze history lowering changed: ' + repr(lifter.todo))
    body = body.replace(old, '        quest:AddMiniMapMarkerByScriptName("EmptyGrave", "HUD_ORB_QUEST_VIGNETTE")')
    return {'HistoryBookcase.Main': body, 'HistoryBookcase.Init': '',
            'EmptyGrave.Main': EMPTY_MAIN, 'EmptyGrave.Init': EMPTY_INIT,
            'MazeResearch.UNLIMBO': UNLIMBO}, witness
