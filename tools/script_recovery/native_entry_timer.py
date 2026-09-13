"""Recover a missing first GetTimer operand from a native quest entry window.

The native first call and first decompiled statement must independently identify
the same global-GSI slot. Only a proven entry-this integer field is translated.
"""
import hashlib
import re
from dataclasses import asdict

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM

from tools.script_recovery.native_call_setup_ir import read_call_window


def recover_entry_timer(function, source, rdata, manifest, state, normalize):
    statements = normalize(source)
    if not statements:
        return source, []
    first = statements[0].strip()
    match = re.fullmatch(r'(\w+)\s*=\s*GSI->GetTimer\(\);', first)
    if not match:
        return source, []

    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]

    contract = manifest.get('GetTimer', {})
    if (contract.get('scope') != 'Quest' or contract.get('returnType') != 'int'
            or contract.get('parameters') != [{'name': 'timerId', 'type': 'int', 'optional': False}]):
        return reject('GetTimer contract changed')
    raw_source = function.get('decompile', '')
    if not re.search(r'\bvoid\s+__fastcall\s+\w+\(CScriptBase\s*\*\s*\w+\)', raw_source):
        return reject('quest entry receiver unproven')
    original = normalize(raw_source)
    expected = (match[1] + '=(**(code**)(*DAT_0143e8f8+0x168))();')
    if not original or re.sub(r'\s+', '', original[0]) != expected:
        return reject('first native source call does not match')
    try:
        start, size = int(function['address'], 16), int(function['size'])
    except (KeyError, TypeError, ValueError):
        return reject('native extent unavailable')
    raw = rdata.bytes_at(start, size)
    if raw is None or len(raw) != size:
        return reject('native bytes unavailable')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw, start))
    first_call = next((ins for ins in instructions if ins.mnemonic == 'call'), None)
    if first_call is None:
        return reject('native entry call unavailable')
    # Window-entry ECX only means function-entry ECX when no path can enter
    # this prefix midway or revisit it with a different receiver.
    for ins in instructions:
        if ins.mnemonic.startswith(('j', 'ret', 'loop')):
            if ins.address < first_call.address:
                return reject('first call is not in a straight entry prefix')
            if ins.mnemonic.startswith(('j', 'loop')):
                if (not ins.operands or ins.operands[0].type != X86_OP_IMM
                        or start <= ins.operands[0].imm <= first_call.address):
                    return reject('entry prefix can be reentered')
    setup = read_call_window(rdata, start, size, first_call.address, argument_count=1)
    gsi = ('memory', ('constant', 0x143e8f8))
    target = ('memory', ('address', ('memory', ('address', gsi, 0)), 0x168))
    if setup is None or setup.target != target or setup.ecx != gsi:
        return reject('first machine call does not match global GetTimer')
    operand = setup.stack_arguments[0]
    if (len(operand) != 2 or operand[0] != 'memory' or len(operand[1]) != 3
            or operand[1][:2] != ('address', ('register', 'ecx'))):
        return reject('timer argument is not a proven entry-this field')
    offset = hex(operand[1][2])
    field = state.get(offset)
    if not field or field[1] != 'Int':
        return reject('timer field lacks integer state mapping')
    replacement = f'{match[1]} = GSI->GetTimer(*(int *)(this + {offset}));'
    body_start = 0
    for line in source.splitlines(keepends=True):
        body_start += len(line)
        if line.rstrip() == '{':
            break
    else:
        return reject('source body span unavailable')
    position = source.find(first, body_start)
    if position < 0:
        return reject('source statement span unavailable')
    recovered = source[:position] + replacement + source[position + len(first):]
    return recovered, [{
        'status': 'recovered', 'callSite': hex(first_call.address), 'fieldOffset': offset,
        'fieldName': field[0], 'setup': asdict(setup),
        'nativeSha256': hashlib.sha256(raw).hexdigest(),
        'sourceSha256': hashlib.sha256(raw_source.encode()).hexdigest(),
    }]
