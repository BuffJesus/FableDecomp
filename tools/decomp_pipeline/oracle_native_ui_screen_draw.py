"""Execute retail screen traversal including its real STLport iterator step."""
import itertools
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX, UC_X86_REG_ESP, UC_X86_REG_EIP
from gate_game_component_abi import DEFAULT_RETAIL, retail_bytes

BASE = 0x24000000

def collect():
    raw = DEFAULT_RETAIL.read_bytes()
    uc = Uc(UC_ARCH_X86, UC_MODE_32)
    for address, length in [(0x595222, 59), (0x4292c0, 44)]:
        uc.mem_map(address & ~4095, 4096)
        uc.mem_write(address, retail_bytes(raw, address, length))
    uc.mem_map(BASE, 65536)
    stack, stop, callback = BASE+0x8000, BASE+0xf000, BASE+0xf100
    inputs, expected = bytearray(), bytearray()
    count = mutation = calls = events = 0
    def word(address, value): uc.mem_write(address, struct.pack('<I', value))
    def read(address): return struct.unpack('<I', uc.mem_read(address, 4))[0]
    def event(kind, a=0, b=0, c=0):
        nonlocal events
        expected.extend(struct.pack('<4I', kind, a, b, c))
        expected.extend(uc.mem_read(BASE, 4096))
        events += 1
    def hook(uc, address, size, user):
        nonlocal calls
        if address != callback: return
        sp = uc.reg_read(UC_X86_REG_ESP)
        component = uc.reg_read(UC_X86_REG_ECX)
        assert uc.mem_read(sp+12, 12) == bytes(12)
        event(1, component, read(sp+4), read(sp+8))
        index = (component-BASE-0x800)//16
        if mutation == 1 and index+1 < count: word(BASE+0x400+(index+1)*32+20, 0)
        if mutation == 2 and index+1 < count: word(BASE+0x154, BASE+0x400+(index+1)*32)
        calls += 1
        uc.reg_write(UC_X86_REG_EIP, read(sp))
        uc.reg_write(UC_X86_REG_ESP, sp+24)
    uc.hook_add(UC_HOOK_CODE, hook)
    word(BASE+0x3008, callback)
    cases = 0
    for count, shape, mask, mutation, variant in itertools.product(range(9), range(3), range(4), range(3), range(2)):
        arena = bytearray((i*13+cases)%256 for i in range(4096))
        def put(offset, value): struct.pack_into('<I', arena, offset, value)
        head = BASE+0x300
        def node(index): return BASE+0x400+index*32
        def tree(indices, parent):
            if not indices: return 0
            split = 0 if shape == 0 else len(indices)-1 if shape == 1 else len(indices)//2
            index = indices[split]; offset = 0x400+index*32
            put(offset, 1); put(offset+4, parent)
            put(offset+8, tree(indices[:split], node(index)))
            put(offset+12, tree(indices[split+1:], node(index)))
            put(offset+16, index)
            present = mask == 0 or mask == 1 and index%2 == 0 or mask == 2 and index%2 == 1
            put(offset+20, BASE+0x800+index*16 if present else 0)
            put(0x800+index*16, BASE+0x3000)
            return node(index)
        root = tree(list(range(count)), head)
        put(0x300, 0); put(0x304, root)
        put(0x308, node(0) if count else head); put(0x30c, node(count-1) if count else head)
        put(0x154, head); put(0x158, count)
        inputs.extend(struct.pack('<4I', count, mutation, variant, variant)); inputs.extend(arena)
        uc.mem_write(BASE, bytes(arena)); calls=0
        uc.mem_write(stack, struct.pack('<III', stop, BASE+0xa00+variant*16, BASE+0xa80 if variant else 0))
        uc.reg_write(UC_X86_REG_ESP, stack); uc.reg_write(UC_X86_REG_ECX, BASE+0x100)
        uc.emu_start(0x595222, stop, count=10000)
        assert uc.reg_read(UC_X86_REG_EIP) == stop and uc.reg_read(UC_X86_REG_ESP) == stack+12
        event(2, calls); cases += 1
    return inputs, expected, dict(cases=cases, snapshots=events, bytes_per_snapshot=4096)
