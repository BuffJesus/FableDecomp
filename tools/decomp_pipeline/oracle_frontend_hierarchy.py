"""Retail UI hierarchy execution for the two coastal animation fixture trees.

State-map lookup and allocation are explicit doubles. Base Update, child
traversal, transform/colour updates, task queues and swapping execute natively.
Construction is controlled fixture setup, not the recovered constructor chain.
"""
import hashlib
import json
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_EIP, UC_X86_REG_ESP, UC_X86_REG_FPCW
from check_cgame_play import ROOT, pe_oracle


class FrontendHierarchyOracle:
    def __init__(self, key):
        fixture = json.loads((ROOT/'rebuild/tests/integration/frontend_background_lifecycle_fixture.json').read_text())
        frontend = pe_oracle.EXE.parent/'data/CompiledDefs/frontend.bin'
        if hashlib.sha256(frontend.read_bytes()).hexdigest() != fixture['frontend_sha256']:
            raise RuntimeError('Retail frontend definitions differ from the hierarchy fixture')
        self.definitions = {int(key): value for key, value in fixture['definitions'].items()}
        image = pe_oracle.EXE.read_bytes()
        if hashlib.sha256(image).hexdigest() != '41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10':
            raise RuntimeError('Retail oracle executable changed')
        self.machine = machine = Uc(UC_ARCH_X86, UC_MODE_32)
        machine.mem_map(0x400000, 0x1100000)
        for _, va, _, raw, size in pe_oracle.pe_sections(image): machine.mem_write(0x400000+va, image[raw:raw+size])
        machine.mem_map(0x20000000, 0x400000)
        self.stop, self.stack, self.table, self.find = 0x20000000, 0x20008000, 0x20010000, 0x20011000
        machine.mem_write(self.table, bytes(machine.mem_read(0x12485AC, 0x23C)))
        # Descendants use the changing-component Update; only the root swaps.
        self.put(self.table+4, 0x52C7E0)
        self.put(self.table+0x218, self.find); self.put(self.table+0x21C, self.find)
        self.swapping_table = self.table+0x400
        machine.mem_write(self.swapping_table, bytes(machine.mem_read(self.table, 0x23C)))
        self.put(self.swapping_table+4, 0x547380)
        machine.mem_write(self.find, b'\xc2\x04\x00')
        machine.mem_write(0xBFEA0E, b'\xc3'); machine.mem_write(0xBFEA14, b'\xc3')
        self.lookups, self.nodes, self.allocation = {}, {}, 0x200C0000
        machine.hook_add(UC_HOOK_CODE, self.hook)
        self.receiver = self.build(key, 0x2001F000)
        self.root_key = key
        self.frames = self.definitions[key]['children']

    def put(self, address, value): self.machine.mem_write(address, struct.pack('<I', value))
    def word(self, address): return struct.unpack('<I', self.machine.mem_read(address, 4))[0]

    def hook(self, machine, address, size, data):
        if address == self.find:
            receiver = machine.reg_read(UC_X86_REG_ECX)
            state = self.word(machine.reg_read(UC_X86_REG_ESP)+4)
            machine.reg_write(UC_X86_REG_EAX, self.lookups[receiver].get(state, 0))
        elif address == 0xBFEA0E:
            if self.allocation >= 0x203FFF00: raise RuntimeError('Fixture allocation arena exhausted')
            machine.reg_write(UC_X86_REG_EAX, self.allocation); self.allocation += 0x20

    def build(self, key, parent):
        definition = self.definitions[key]
        node = 0x20020000+len(self.nodes)*0x1000
        self.nodes[key] = node; self.lookups[node] = {}
        machine = self.machine; machine.mem_write(node, b'\0'*0x1000)
        self.put(node, self.swapping_table if definition['type'] == 18 else self.table)
        self.put(node+0xC8, parent)
        machine.mem_write(node+0x12D, bytes((definition['type'],)))
        machine.mem_write(node+0x12F, bytes((definition['layer'] & 255,)))
        head = node+0x300; self.put(node+0x13C, head); self.put(head, head); self.put(head+4, head)
        tree = node+0x320; self.put(node+0x11C, tree); self.put(tree+8, tree)
        deletion = node+0x340; self.put(node+0xD8, deletion); self.put(deletion, deletion); self.put(deletion+4, deletion)
        for offset in (0xBC, 0xC0, 0xC4): self.put(node+offset, node+0x380)
        for i, state in enumerate(definition['states']):
            address = node+0x400+i*0x24; self.lookups[node][i] = address
            colour = bytes(int(state['Colour'+channel]*255) for channel in 'RGBA')
            machine.mem_write(address, struct.pack('<IIffff', state['StateChangeFlag'], state['StateChangeType'],
                state['PositionX'], state['PositionY'], state['ZoomX'], state['ZoomY'])+colour+
                struct.pack('<fB', state['UpdateTime'], state['LinearChange'])+b'\0'*3)
        initial = definition['states'][0] if definition['states'] else {
            'PositionX': 0, 'PositionY': 0, 'ZoomX': 1, 'ZoomY': 1,
            'ColourR': 1, 'ColourG': 1, 'ColourB': 1, 'ColourA': 1}
        for offset in (0x34, 0x3C, 0x44, 0x54): machine.mem_write(node+offset, struct.pack('<ff', initial['PositionX'], initial['PositionY']))
        for offset in (0x5C, 0x64, 0x6C, 0x7C): machine.mem_write(node+offset, struct.pack('<ff', initial['ZoomX'], initial['ZoomY']))
        for offset in (0x74, 0x110): machine.mem_write(node+offset, struct.pack('<ff', 1, 1))
        for offset in (0x84, 0x88, 0x8C, 0x94): machine.mem_write(node+offset, bytes(int(initial['Colour'+channel]*255) for channel in 'RGBA'))
        self.put(node+0x90, 0xFFFFFFFF); machine.mem_write(node+0x140, struct.pack('<f', -1))
        children = [self.build(child, node) for child in definition['children']]
        for i, child in enumerate(children): self.put(node+0x800+i*8, child); self.put(node+0x804+i*8, 0)
        machine.mem_write(node+0xB0, struct.pack('<III', node+0x800, node+0x800+len(children)*8, node+0x800+len(children)*8))
        if definition['type'] == 18:
            entries = node+0xA00
            for i in range(len(children)): machine.mem_write(entries+i*8, struct.pack('<If', i, 0.0))
            machine.mem_write(node+0x15C, struct.pack('<III', entries, entries+len(children)*8, entries+len(children)*8))
            self.put(node+0x16C, 13); machine.mem_write(node+0x12E, b'\x08')
        return node

    def update(self, delta):
        machine = self.machine
        self.put(self.stack, self.stop); machine.mem_write(self.stack+4, struct.pack('<f', delta))
        machine.reg_write(UC_X86_REG_ESP, self.stack); machine.reg_write(UC_X86_REG_ECX, self.receiver)
        update = 0x547380 if self.definitions[self.root_key]['type'] == 18 else 0x52C7E0
        machine.reg_write(UC_X86_REG_FPCW, 0x37F); machine.emu_start(update, self.stop, count=500000)
        if machine.reg_read(UC_X86_REG_EIP) != self.stop: raise RuntimeError('Native hierarchy update did not return')
        return self.sample(self.root_key)

    def sample(self, key):
        node = self.nodes[key]
        return [self.word(node+offset) for offset in (0x144, 0x148, 0x16C, 0x168)] + [
            self.machine.mem_read(self.nodes[child]+0x97, 1)[0] for child in self.definitions[key]['children']]
