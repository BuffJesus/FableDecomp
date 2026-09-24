#!/usr/bin/env python3
"""Controlled sprite lifetime/order evidence; not a full renderer parity gate.

The actual component Draw traverses the definition-derived coastal tree. Sprite
Draw is a callback implementing the separately verified visibility decision.
Persistent submission/release is represented by native AddToList/RemoveFromList,
not by engine factories, handles or refcount destruction. No depth sorting or
rendering occurs. The optional existing same-mask node models an unknown prior
scene primitive; it does not assert that any particular frontend page has one.
"""
import json
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX, UC_X86_REG_EIP, UC_X86_REG_ESP
from check_cgame_play import ROOT, pe_oracle
from oracle_frontend_hierarchy import FrontendHierarchyOracle


class PrimitiveChain:
    def __init__(self, anchor):
        self.machine = Uc(UC_ARCH_X86, UC_MODE_32)
        self.machine.mem_map(0x400000, 0x1100000)
        image = pe_oracle.EXE.read_bytes()
        for _, va, _, raw, size in pe_oracle.pe_sections(image): self.machine.mem_write(0x400000+va, image[raw:raw+size])
        self.machine.mem_map(0x20000000, 0x20000)
        self.stop, self.stack, self.head = 0x20000000, 0x20008000, 0x20010000
        self.live, self.identities = set(), {}
        if anchor: self.submit(-1)

    def node(self, key): return 0x20011000+(key+1)*0x80
    def invoke(self, address, node, *args):
        machine = self.machine
        machine.mem_write(self.stack, struct.pack('<'+'I'*(len(args)+1), self.stop, *args))
        machine.reg_write(UC_X86_REG_ESP, self.stack); machine.reg_write(UC_X86_REG_ECX, node)
        machine.emu_start(address, self.stop, count=10000)
        if machine.reg_read(UC_X86_REG_EIP) != self.stop: raise RuntimeError('Primitive-list call failed')

    def submit(self, key):
        if key in self.live: return
        node = self.node(key); self.identities[node] = key
        self.machine.mem_write(node, b'\0'*0x48)
        self.machine.mem_write(node+0x30, struct.pack('<I', 0x80000000))
        self.invoke(0xB8FDF0, node, self.head); self.live.add(key)

    def release(self, key):
        if key not in self.live: return
        self.invoke(0xB8FE60, self.node(key)); self.live.remove(key)

    def order(self):
        def word(address): return struct.unpack('<I', self.machine.mem_read(address, 4))[0]
        result, link = [], self.head
        while word(link):
            node = word(link)
            if len(result) >= len(self.live) or word(node+0x3C) != link: raise RuntimeError('Invalid primitive chain')
            result.append(self.identities[node]); link = node+0x38
        if set(result) != self.live: raise RuntimeError('Primitive chain membership differs')
        return result


def scenario(anchor):
    oracle, chain = FrontendHierarchyOracle(685), PrimitiveChain(anchor)
    machine = oracle.machine
    tiles = [key for key, definition in oracle.definitions.items() if definition['type'] == 0]
    ids = {oracle.nodes[key]: i for i, key in enumerate(tiles)}
    owners = {child: frame for group in (686, 687) for frame in oracle.definitions[group]['children']
              for child in oracle.definitions[frame]['children']}
    sprite_table, draw = 0x20018000, 0x20019000
    machine.mem_write(sprite_table, bytes(machine.mem_read(oracle.table, 0x23C)))
    oracle.put(sprite_table+8, draw); machine.mem_write(draw, b'\xc2\x14\x00')
    for receiver in ids: oracle.put(receiver, sprite_table)
    visits, releases, layers = [], [], set()
    def callback(machine, address, size, data):
        if address != draw: return
        node = machine.reg_read(UC_X86_REG_ECX); key = ids[node]
        esp = machine.reg_read(UC_X86_REG_ESP); layers.add(oracle.word(esp+12))
        visits.append(key)
        alpha = machine.mem_read(node+0x97, 1)[0]
        zoom_x, zoom_y = struct.unpack('<ff', machine.mem_read(node+0x7C, 8))
        pending = machine.mem_read(node+0x170, 1)[0]
        invisible = alpha == 0 or (zoom_x <= 0 and zoom_y <= 0)
        if invisible and pending == 1:
            if key in chain.live: releases.append(key)
            chain.release(key)
        else:
            machine.mem_write(node+0x170, bytes((int(invisible),)))
            chain.submit(key)
    machine.hook_add(UC_HOOK_CODE, callback)
    snapshots = []
    for frame in range(80):
        oracle.update(0.0 if frame == 0 else 0.25)
        visits.clear(); releases.clear()
        machine.mem_write(oracle.stack, struct.pack('<IIIIII', oracle.stop, 0, 0, 0, 0, 0))
        machine.reg_write(UC_X86_REG_ESP, oracle.stack); machine.reg_write(UC_X86_REG_ECX, oracle.receiver)
        machine.emu_start(0x530260, oracle.stop, count=100000)
        if machine.reg_read(UC_X86_REG_EIP) != oracle.stop: raise RuntimeError('Component draw did not return')
        if len(visits) != 42 or len(set(visits)) != 42: raise RuntimeError('Component traversal lost/duplicated tiles')
        order = chain.order()
        if frame in (0, 1, 2, 9, 33, 35, 43, 67, 79):
            visible = [key for key in order if key >= 0 and machine.mem_read(oracle.nodes[tiles[key]]+0x97, 1)[0]]
            blocks = []
            for key in visible:
                owner = owners[tiles[key]]
                if not blocks or blocks[-1]['frame'] != owner: blocks.append({'frame': owner, 'tiles': []})
                blocks[-1]['tiles'].append(tiles[key])
            snapshots.append({'frame': frame, 'live_primitives': len(order), 'released': [tiles[key] for key in releases],
                              'visible_frame_blocks': blocks})
    if layers != {7}: raise RuntimeError(f'Unexpected controlled sprite layers: {layers}')
    return {'existing_same_mask_anchor': anchor, 'drawn_tile_layer': 7, 'snapshots': snapshots}


def main():
    report = {'scope': __doc__, 'scenarios': [scenario(False), scenario(True)]}
    directory = ROOT/'work/frontend_lifetime_order'; directory.mkdir(parents=True, exist_ok=True)
    (directory/'report.json').write_text(json.dumps(report, indent=2)+'\n')
    print('FRONTEND_LIFETIME_ORDER PASS scenarios=2 updates=160 tile_draw_visits=6720')
    return 0


if __name__ == '__main__': raise SystemExit(main())
