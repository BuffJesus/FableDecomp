"""Apply native register provenance to the reviewed barrel timer wait loop."""
import hashlib
import json
from dataclasses import asdict
from pathlib import Path

from capstone import Cs, CS_ARCH_X86, CS_MODE_32

from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_register_origins import register_origins


def recover_loop_timer(function, source, rdata, state):
    witness = json.loads(Path(__file__).with_name('native_loop_timer_witness.json').read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []

    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]

    for text, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(text.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    start, size = int(witness['address'], 16), witness['size']
    raw = rdata.bytes_at(start, size)
    if raw is None or hashlib.sha256(raw).hexdigest() != witness['bytesSha256']:
        return reject('native loop body changed')
    # These reviewed MSVC x86 implementations preserve ESI. Pin all return
    # paths, including GetTimer's absent-timer branch and the frame early exit.
    for callee in witness['callees']:
        address = int(callee['address'], 16)
        body = rdata.bytes_at(address, callee['size'])
        if body is None or hashlib.sha256(body).hexdigest() != callee['bytesSha256']:
            return reject(callee['name'] + ' implementation changed')
        if callee['vtableEntry']:
            pointer = rdata.bytes_at(int(callee['vtableEntry'], 16), 4)
            if pointer != address.to_bytes(4, 'little'):
                return reject(callee['name'] + ' dispatch changed')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw, start))
    if not instructions or instructions[-1].address + instructions[-1].size != start + size:
        return reject('incomplete native instruction extent')
    base = witness['baseRegister']
    profiles = {int(site, 16): (base,) for site in witness['preservedSites']}
    facts = register_origins(instructions, profiles)
    site = int(witness['callSite'], 16)
    if facts is None or not facts.get(site) or facts[site].get(base) != 'ecx':
        return reject('loop receiver origin unresolved')
    for ins in instructions:
        if int(witness['windowStart'], 16) <= ins.address < site:
            if any(ins.reg_name(reg) in (base, 'si') for reg in ins.regs_access()[1]):
                return reject('receiver changes during argument setup')
    setup = read_call_window(rdata, start, size, site, argument_count=1)
    gsi = ('memory', ('constant', 0x143e8f8))
    expected_target = ('memory', ('address', ('memory', ('address', gsi, 0)), 0x168))
    if setup is None or setup.target != expected_target or setup.ecx != gsi:
        return reject('loop timer call unresolved')
    operand = setup.stack_arguments[0]
    if (len(operand) != 2 or operand[0] != 'memory' or len(operand[1]) != 3
            or operand[1][:2] != ('address', ('register', base))):
        return reject('loop timer field unresolved')
    offset = hex(operand[1][2])
    field = state.get(offset)
    if not field or field[1] != 'Int':
        return reject('loop timer integer state mapping unavailable')
    if source.count('GSI->GetTimer();') != 1:
        return reject('loop timer source correspondence changed')
    recovered = source.replace('GSI->GetTimer();', f'GSI->GetTimer(*(int *)(this + {offset}));', 1)
    return recovered, [dict(witness, status='recovered', fieldOffset=offset,
                           fieldName=field[0], setup=asdict(setup), registerOrigins=facts[site])]
