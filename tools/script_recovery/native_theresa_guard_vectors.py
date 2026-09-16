"""Three retained guard vectors and their exact removal/destruction helpers."""
import hashlib
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from tools.script_recovery.native_theresa_control import verify as verify_control
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_resource_lifetime import check_single_resource_lifetime

GETTERS = {0xdb9cbe: 364, 0xdba14c: 304, 0xdba647: 316}
REMOVALS = {0xdb9cd8: 364, 0xdba169: 304, 0xdba664: 316}
DESTRUCTORS = {0xdb9e7d: 364, 0xdba308: 304, 0xdba7f7: 316,
               0xdbaf7f: 304, 0xdbafb1: 304, 0xdbafe3: 316, 0xdbb014: 316}


def verify(data):
    control = verify_control(data)
    for address, length, digest in ((0xcbed82, 56, '4413c70ec94bfffe057e5d493bb4548e1437519e571512567583297a5d4d6c67'),
                                    (0x8ac970, 50, '804b6b863d9405337d2e256db023e9c512614766507073be77773f0d217e2a6d')):
        raw = data.bytes_at(address, length)
        if raw is None or hashlib.sha256(raw).hexdigest() != digest:
            raise ValueError('Theresa guard vector helper changed')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32); decoder.detail = True
    start, size = control['address'], control['size']
    instructions = list(decoder.disasm(data.bytes_at(start, size), start))
    events = {}
    for inventory, target, count, operation in ((GETTERS, '+ 0x12c]', 2, 'start'),
                                               (REMOVALS, '0xcbed82', 1, 'use'),
                                               (DESTRUCTORS, '0x8ac970', 0, 'end')):
        if {i.address for i in instructions if i.mnemonic == 'call' and i.op_str.endswith(target)} != set(inventory):
            raise ValueError('Theresa guard vector coverage changed')
        for site, slot in inventory.items():
            setup = read_call_window(data, start, size, site, {}, argument_count=count)
            identity = (setup.stack_arguments[1] if operation == 'start' else
                        setup.edx if operation == 'use' else setup.ecx) if setup else None
            if identity != ('stack', slot): raise ValueError('Theresa guard vector identity changed')
            events[site] = (operation, identity)
    if not check_single_resource_lifetime(instructions, events):
        raise ValueError('Theresa guard vector lifetime changed')
    return dict(status='verified-guard-vector-caller-lifetimes', mainSha256=control['sha256'],
                getters=GETTERS, removals=REMOVALS, destructors=DESTRUCTORS,
                remaining='Getter allocation/retention, removal false operand wider-scope proof and runtime adapter')
