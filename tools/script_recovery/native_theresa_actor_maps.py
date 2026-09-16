"""Theresa's meeting, acceptance and outro map lifetimes and resource bindings."""
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from tools.script_recovery.native_theresa_control import verify as verify_control
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_resource_lifetime import check_resource_lifetimes

CONSTRUCT = {0xdb9a17: 228, 0xdb9ce1: 60, 0xdbb1aa: 60}
DESTROY = {0xdb9e71: 60, 0xdb9eb4: 228, 0xdbaf18: 228, 0xdbb2af: 60}
# lookup, assignment, map slot, key slot, source resource slot
BINDINGS = ((0xdb9a46, 0xdb9a4d, 228, 128, 344), (0xdb9a85, 0xdb9a8c, 228, 220, 24),
            (0xdb9d0d, 0xdb9d14, 60, 160, 344), (0xdb9d43, 0xdb9d4a, 60, 108, 24),
            (0xdbb1d0, 0xdbb1d7, 60, 56, 328), (0xdbb203, 0xdbb20a, 60, 56, 24))
MACROS = {0xdb9b28: 228, 0xdb9d7a: 60, 0xdbb238: 60}


def verify(data):
    control = verify_control(data)
    decoder = Cs(CS_ARCH_X86, CS_MODE_32); decoder.detail = True
    address, size = control['address'], control['size']
    instructions = list(decoder.disasm(data.bytes_at(address, size), address))
    targets = {'0xcdbf70': set(CONSTRUCT), '0xcdbfb0': set(DESTROY),
               '0xcd3d2e': {r[0] for r in BINDINGS}, '0x8abd10': {r[1] for r in BINDINGS},
               '0xcbfb7d': set(MACROS)}
    for target, sites in targets.items():
        if {i.address for i in instructions if i.mnemonic == 'call' and i.op_str == target} != sites:
            raise ValueError('Theresa actor map call coverage changed')
    known = {row[0]: (4, 'entry') for row in BINDINGS}
    events = {}
    for sites, operation in ((CONSTRUCT, 'start'), (DESTROY, 'end')):
        for site, slot in sites.items():
            setup = read_call_window(data, address, size, site, {}, argument_count=0)
            if setup is None or setup.ecx != ('stack', slot): raise ValueError('Theresa map identity changed')
            events[site] = (operation, ('stack', slot))
    for lookup, assign, slot, key, resource in BINDINGS:
        q = read_call_window(data, address, size, lookup, {}, argument_count=1)
        a = read_call_window(data, address, size, assign, known, argument_count=1)
        if (q is None or a is None or q.ecx != ('stack', slot) or q.stack_arguments != (('stack', key),)
                or a.ecx != ('result', lookup, 'entry') or a.stack_arguments != (('stack', resource),)):
            raise ValueError('Theresa actor map binding changed')
        events[lookup] = events[assign] = ('use', ('stack', slot))
    for site, slot in MACROS.items():
        setup = read_call_window(data, address, size, site, {}, argument_count=4)
        if setup is None or setup.edx != ('stack', slot): raise ValueError('Theresa macro map changed')
        events[site] = ('use', ('stack', slot))
    if not check_resource_lifetimes(instructions, events): raise ValueError('Theresa actor map lifetime changed')
    return dict(status='verified-actor-map-caller-lifetimes', mainSha256=control['sha256'],
                bindings=BINDINGS, macroMaps=MACROS,
                remaining='Map implementation, retained resource copies, key/flag execution and runtime integration')
