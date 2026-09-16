"""Verify Theresa's three control locals against complete Main and aligned CFG."""
import hashlib
import json
from dataclasses import asdict
from pathlib import Path
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from tools.script_recovery.native_affair_woman_resources import HELPERS
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_resource_lifetime import check_resource_lifetimes


def verify(data, witness=None):
    w = witness or json.loads(Path(__file__).with_name('native_theresa_control_witness.json').read_text())
    if (w['address'], w['size']) != (0xDB97A0, 7013):
        raise ValueError('Theresa Main boundary changed')
    raw = data.bytes_at(w['address'], w['size'])
    if raw is None or hashlib.sha256(raw).hexdigest() != w['sha256']:
        raise ValueError('Theresa Main bytes changed')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw, w['address']))
    helpers = dict(HELPERS)
    helpers.update({0x7e72a0: 'construct', 0x7e73e0: 'animation'})
    inventory = {}
    for ins in instructions:
        if ins.mnemonic != 'call': continue
        name = helpers.get(int(ins.op_str, 16) if ins.op_str.startswith('0x') else 0)
        if ins.op_str.endswith('+ 0x20]'): name = 'acquire'
        if name: inventory[ins.address] = name
    if len(w['events']) != len(inventory) or {e['site']: e['name'] for e in w['events']} != inventory:
        raise ValueError('Theresa control coverage changed')
    known = {i.address: (0, 'GetHero') for i in instructions
             if i.mnemonic == 'call' and i.op_str.endswith('+ 0x118]')}
    events = {}
    for event in w['events']:
        count = {'acquire': 3, 'speak': 6, 'get_thing': 1, 'animation': 7}.get(event['name'], 0)
        if event['argumentCount'] != count: raise ValueError('Theresa control argument count changed')
        setup = read_call_window(data, w['address'], w['size'], event['site'], known, argument_count=count)
        if setup is None or json.loads(json.dumps(asdict(setup))) != event['setup']:
            raise ValueError('Theresa control operands changed')
        identity = setup.stack_arguments[1] if event['name'] == 'acquire' else setup.ecx
        if identity not in (('stack', 24), ('stack', 344), ('stack', 328)):
            raise ValueError('Theresa control identity changed')
        if event['name'] == 'acquire' and setup.stack_arguments[2] != ('constant', 4):
            raise ValueError('Theresa control priority changed')
        operation = 'start' if event['name'] == 'construct' else 'end' if event['name'].startswith('destroy') else 'use'
        events[event['site']] = (operation, identity)
    if not check_resource_lifetimes(instructions, events):
        raise ValueError('Theresa control lifetime changed')
    return dict(w, status='verified-control-caller-lifetimes',
                remaining='Movie, cutscene binding map, temporary Thing lifetimes, action semantics and runtime validation')
