"""Eight resource-derived health Things, including the nonlocal cleanup join."""
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from tools.script_recovery.native_theresa_control import verify as verify_control
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_resource_lifetime import check_single_resource_lifetime

# getter, health query, comparison entry, destructor, output at baseline ESP
SITES = (
    (0xdba17f, 0xdba188, 0xdba18e, 0xdba1a8, 464),
    (0xdba32b, 0xdba334, 0xdba33a, 0xdba354, 428),
    (0xdba561, 0xdba56a, 0xdba570, 0xdba822, 416),
    (0xdba67a, 0xdba683, 0xdba689, 0xdba6a3, 500),
    (0xdba967, 0xdba970, 0xdba976, 0xdba990, 440),
    (0xdbaa26, 0xdbaa2f, 0xdbaa35, 0xdbaa4f, 476),
    (0xdbab07, 0xdbab10, 0xdbab16, 0xdbab30, 488),
    (0xdbad46, 0xdbad4f, 0xdbad55, 0xdbad6f, 452),
)


def verify(data):
    control = verify_control(data)
    if data.bytes_at(0x122dedc, 4) != bytes(4):
        raise ValueError('Theresa health threshold changed')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    start, size = control['address'], control['size']
    instructions = list(decoder.disasm(data.bytes_at(start, size), start))
    by_address = {i.address: i for i in instructions}
    getters = {e['site']: e for e in control['events'] if e['name'] == 'get_thing'}
    if set(getters) != {row[0] for row in SITES}:
        raise ValueError('Theresa health getter coverage changed')
    events, regions = {}, []
    for create, query, comparison, destroy, output in SITES:
        identity = ('stack', output)
        if getters[create]['setup']['stack_arguments'] != [list(identity)]:
            raise ValueError('Theresa health output changed')
        q = read_call_window(data, start, size, query, {create: (4, 'thing')}, argument_count=1)
        d = read_call_window(data, start, size, destroy, {}, argument_count=0)
        if (q is None or d is None or not by_address[query].op_str.endswith('+ 0x420]')
                or q.ecx != ('memory', ('address', ('register', 'ebp'), 4))
                or q.stack_arguments != (('result', create, 'thing'),)
                or d.ecx != identity or d.target != ('constant', 0x4aa840)):
            raise ValueError('Theresa health actor/cleanup changed')
        for site, operation in ((create, 'start'), (query, 'use'), (destroy, 'end')):
            events[site] = (operation, identity)
        regions.append(dict(address=comparison, size=destroy + 5 - comparison, output=output, result='bl'))
    if not check_single_resource_lifetime(instructions, events):
        raise ValueError('Theresa health Thing lifetime changed')
    return dict(mainSha256=control['sha256'], status='verified-health-caller-lifetimes', branches=regions,
                limits='Health API and Thing destructor bodies are separate integration gates')
