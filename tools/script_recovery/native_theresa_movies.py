"""Theresa movie ownership, including the four-way shared destructor join."""
import json
from dataclasses import asdict
from pathlib import Path
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from tools.script_recovery.native_theresa_control import verify as verify_control
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_resource_lifetime import check_resource_lifetimes


def verify(data, witness=None):
    control = verify_control(data)
    w = witness or json.loads(Path(__file__).with_name('native_theresa_movies_witness.json').read_text())
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(data.bytes_at(control['address'], control['size']), control['address']))
    inventory = {}
    for ins in instructions:
        if ins.mnemonic != 'call': continue
        name = ('start' if ins.op_str.endswith('+ 0x5c8]') else
                'construct' if ins.op_str == '0x6e7b60' else 'destroy' if ins.op_str == '0x6e7b80' else None)
        if name: inventory[ins.address] = name
    if len(w['events']) != len(inventory) or {e['site']: e['name'] for e in w['events']} != inventory:
        raise ValueError('Theresa movie coverage changed')
    selections = {int(site): tuple(identity) for site, identity in w['selections'].items()}
    expected = {0xdba3cf: ('stack', 288), 0xdba89f: ('stack', 384),
                0xdbaba7: ('stack', 252), 0xdbade6: ('stack', 328)}
    if selections != expected: raise ValueError('Theresa movie selections changed')
    by_address = {i.address: i for i in instructions}
    for site, (_, offset) in selections.items():
        ins = by_address[site]
        if (ins.mnemonic, ins.op_str) != ('lea', f'ecx, [esp + {offset:#x}]'):
            raise ValueError('Theresa movie selection operands changed')
    events = {}
    for event in w['events']:
        count = 2 if event['name'] == 'start' else 0
        if event['argumentCount'] != count: raise ValueError('Theresa movie argument count changed')
        setup = read_call_window(data, control['address'], control['size'], event['site'], {}, argument_count=count)
        if setup is None or json.loads(json.dumps(asdict(setup))) != event['setup']:
            raise ValueError('Theresa movie operands changed')
        identity = setup.stack_arguments[1] if count else setup.ecx
        events[event['site']] = ({'construct': 'start', 'start': 'use', 'destroy': 'end'}[event['name']], identity)
    if not check_resource_lifetimes(instructions, events, receiver_register='ecx', selections=selections):
        raise ValueError('Theresa movie lifetime changed')
    return dict(w, status='verified-movie-caller-lifetimes', mainSha256=control['sha256'],
                remaining='Pause operands, helper implementation, cutscene bindings and runtime lowering')
