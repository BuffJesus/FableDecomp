#!/usr/bin/env python3
"""Offline retail ordering evidence, NOT a presenter or C++ parity gate.

Executes AddToList and the small-list, false-flag SortList path without doubles.
Stable sorting uses explicit malloc/free doubles; constructor setup uses base
constructor doubles. These probes do not prescribe a presenter draw order.
"""
import hashlib
import json
import struct

from unicorn import Uc, UC_ARCH_X86, UC_MODE_32
from unicorn.x86_const import UC_X86_REG_ECX, UC_X86_REG_EIP, UC_X86_REG_ESP
from check_cgame_play import ROOT, pe_oracle


def main():
    image = pe_oracle.EXE.read_bytes()
    digest = hashlib.sha256(image).hexdigest()
    if digest != "41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10":
        raise RuntimeError("Retail oracle executable changed")
    machine = Uc(UC_ARCH_X86, UC_MODE_32)
    machine.mem_map(0x400000, 0x1100000)
    for _, va, _, raw, size in pe_oracle.pe_sections(image):
        machine.mem_write(0x400000 + va, image[raw:raw + size])
    machine.mem_map(0x20000000, 0x20000)
    stop, stack, receiver, vector, entries = (
        0x20000000, 0x20008000, 0x20010000, 0x20011000, 0x20012000)

    def invoke(address, this, *args):
        machine.mem_write(stack, struct.pack("<" + "I" * (len(args) + 1), stop, *args))
        machine.reg_write(UC_X86_REG_ESP, stack)
        machine.reg_write(UC_X86_REG_ECX, this)
        machine.emu_start(address, stop, count=1000000)
        if machine.reg_read(UC_X86_REG_EIP) != stop:
            raise RuntimeError("Retail function exceeded instruction budget")

    def word(address):
        return struct.unpack("<I", machine.mem_read(address, 4))[0]

    chains = []
    for masks in ([1], [1, 1, 1, 1], [1, 2, 1, 3, 2, 1]):
        machine.mem_write(receiver, b"\0" * 4)
        for index, mask in enumerate(masks):
            node = entries + index * 0x80
            machine.mem_write(node, b"\0" * 0x48)
            machine.mem_write(node + 0x30, struct.pack("<I", mask))
            invoke(0xB8FDF0, node, receiver)
        order, link = [], receiver
        while word(link):
            node = word(link)
            if len(order) >= len(masks):
                raise RuntimeError("Cycle in primitive chain")
            if word(node + 0x3C) != link:
                raise RuntimeError("Broken primitive backlink")
            order.append((node - entries) // 0x80)
            link = node + 0x38
        if sorted(order) != list(range(len(masks))):
            raise RuntimeError("Lost primitive in chain")
        chains.append({"submitted_masks": list(masks), "traversal_indices": order})

    sorts = []
    for count in (1, 2, 6, 12, 16, 17, 24, 32, 100, 255):
        machine.mem_write(receiver, b"\0" * 0x100)
        machine.mem_write(vector, struct.pack("<III", entries, entries + count*8, entries + count*8))
        machine.mem_write(entries, b"".join(struct.pack("<II", 100, i) for i in range(count)))
        invoke(0xB84990, receiver, vector, 0)
        words = struct.unpack("<" + "I" * count*2, machine.mem_read(entries, count*8))
        order = list(words[1::2])
        if sorted(order) != list(range(count)) or set(words[::2]) != {100}:
            raise RuntimeError("Sort lost or changed elements")
        sorts.append({"count": count, "equal_key_order": order,
                      "preserves_input_order": order == list(range(count))})

    # The stable path allocates temporary storage. Keep allocation explicit,
    # deterministic and confined to scratch; the actual sort still executes.
    machine.mem_write(0xBFEA0E, b"\xB8" + struct.pack("<I", 0x20016000) + b"\xC3")
    machine.mem_write(0xBFEA14, b"\xC3")
    stable_sorts = []
    for count in (1, 2, 6, 16, 17, 32, 100, 255, 512):
        for mixed in (False, True):
            values = [((i * 7) % 13 if mixed else 100, i) for i in range(count)]
            machine.mem_write(vector, struct.pack("<III", entries, entries + count*8, entries + count*8))
            machine.mem_write(entries, b"".join(struct.pack("<II", *value) for value in values))
            invoke(0xB84990, receiver, vector, 1)
            words = struct.unpack("<" + "I" * count*2, machine.mem_read(entries, count*8))
            result = list(zip(words[::2], words[1::2]))
            if result != sorted(values, key=lambda value: value[0]):
                raise RuntimeError("Retail stable-sort result disagrees with stable key order")
            stable_sorts.append({"count": count, "mixed_keys": mixed, "stable": True})

    # Constructor-owned list setup, with unrelated bases explicitly doubled.
    machine.mem_write(0x99A2F0, b"\xC3")
    machine.mem_write(0xB59710, b"\xC3")
    machine.mem_write(0x1436EA4, struct.pack("<I", 0x20015000))
    machine.mem_write(receiver, b"\xA5" * 0x200)
    invoke(0xB84470, receiver)
    lists = []
    for index in range(15):
        address = receiver + 0x18 + index*0x18
        render_pass, pre_render_pass, bounding, stable, sorted_flag, enabled = struct.unpack(
            "<IIBBBB", machine.mem_read(address + 0xC, 12))
        lists.append({"type": index, "render_pass": render_pass,
                      "pre_render_pass": pre_render_pass, "bounding_volume": bool(bounding),
                      "requires_stable_sort": bool(stable), "sorted": bool(sorted_flag),
                      "enabled": bool(enabled)})
    if not lists[2]["requires_stable_sort"] or not lists[2]["sorted"]:
        raise RuntimeError("Normal 2D list no longer requests stable sorting")

    # Persistent sprite lifetime: execute SetPersistentPrimitive -> AddPrimitive
    # -> AddToList. Only the manager factory and primitive Update are doubled.
    # Grid level -1 selects the native no-spatial-position-update branch.
    manager, manager_vtable, primitive_vtable = 0x20014000, 0x20014100, 0x20014200
    factory, update, registry, renderer = 0x20014300, 0x20014320, 0x20014400, 0x20014500
    handles, description, primitives = 0x20014600, 0x20014700, 0x20016000
    machine.mem_write(0x1436E84, struct.pack("<I", registry))
    machine.mem_write(0x1436E7C, struct.pack("<I", renderer))
    machine.mem_write(registry + 0x10 + 0x23*4, struct.pack("<I", manager))
    machine.mem_write(manager, struct.pack("<I", manager_vtable))
    machine.mem_write(manager_vtable + 4, struct.pack("<I", factory))
    machine.mem_write(primitive_vtable + 0x14, struct.pack("<I", update))
    machine.mem_write(update, b"\xC2\x08\x00")
    machine.mem_write(description, struct.pack("<I", 0x23))
    machine.mem_write(handles, b"\0" * 24)
    machine.mem_write(renderer, b"\0" * 0x80)
    for index in range(3):
        primitive = primitives + index*0x80
        machine.mem_write(primitive, b"\0" * 0x80)
        machine.mem_write(primitive, struct.pack("<III", primitive_vtable, 1, 0x23))
        machine.mem_write(primitive + 0x24, b"\xFF")
        machine.mem_write(primitive + 0x30, struct.pack("<I", 4))
        machine.mem_write(factory, b"\xB8" + struct.pack("<I", primitive) + b"\xC2\x04\x00")
        machine.ctl_remove_cache(factory, factory + 16)
        invoke(0xB324A0, receiver, handles + index*8, description, 0xC0, 0)
    initial_links = [word(renderer + 0x40)] + [
        word(primitives + index*0x80 + field)
        for index in range(3) for field in (0x38, 0x3C, 0x40, 0x44)]
    if ([word(handles + index*8 + 4) for index in range(3)] !=
            [primitives + index*0x80 for index in range(3)] or
            initial_links[0] != primitives or
            [word(primitives + index*0x80 + 0x38) for index in range(3)] !=
            [primitives + 0x100, 0, primitives + 0x80]):
        raise RuntimeError("Persistent creation did not produce the expected 0,2,1 chain")
    # A reuse that mistakenly calls the factory will return null and fail.
    machine.mem_write(factory, b"\x31\xC0\xC2\x04\x00")
    machine.ctl_remove_cache(factory, factory + 16)
    for index in (1, 0, 2):
        invoke(0xB324A0, receiver, handles + index*8, description, 0xC0, 0)
    updated_links = [word(renderer + 0x40)] + [
        word(primitives + index*0x80 + field)
        for index in range(3) for field in (0x38, 0x3C, 0x40, 0x44)]
    if initial_links != updated_links:
        raise RuntimeError("Same-type persistent update changed primitive list order")

    report = {"retail_sha256": digest, "primitive_chains": chains,
              "false_flag_small_list_sorts": sorts,
              "stable_sorts_with_allocator_doubles": stable_sorts,
              "constructor_list_setup_with_base_doubles": lists,
              "persistent_same_type_update": {
                  "creation_order": [0, 1, 2], "update_order": [1, 0, 2],
                  "all_list_links_preserved": True,
                  "doubles": ["manager factory", "primitive Update"],
                  "scope": "flags 0xC0, grid level -1, same type 0x23"},
              "limitation": "Runtime flag changes, layer mapping and complete cache submissions not recovered"}
    directory = ROOT / "work/frontend_draw_order_probe"
    directory.mkdir(parents=True, exist_ok=True)
    (directory / "report.json").write_text(json.dumps(report, indent=2) + "\n")
    print("DRAW_ORDER_PROBE PASS: 3 primitive chains, 10 unstable-path lists, 18 stable-path lists, 15 list defaults")
    print("Normal 2D list defaults to stable sorting; equal keys preserve its input order.")
    print("Same-type persistent sprite updates preserve existing list links.")
    print("This does not establish live frontend draw order.")


if __name__ == "__main__":
    main()
