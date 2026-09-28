"""A C3DVector copied member-wise into three stack slots, read back through those slots.

V_ChickenKicking KickedChicken Main (0x00E64210) keeps FoulLineMarkerA/B's positions and the hero's position as
three dwords each (`piStack_3c = *(int **)pCVar4; piStack_38 = *(int **)&pCVar4->field_0x4; uStack_34 = ...0x8`),
then tests which side of the foul line the hero stands on with a 2D cross product over those slots. Ghidra types
some slots as pointers and reuses the hero's slots as a CScriptThing temporary elsewhere, so later passes renamed
them apart from their meaning (x and z of the hero both became `xStack_30`) and the condition read nils.

On the raw decompile, three consecutive loads of offsets 0, 4, 8 from one `C3DVector_bv *` become
`vec = ENGINE_VectorCopy(P);`, and each slot's reads up to its next store become `vec.x/.y/.z`. Only a triple whose
three slots are distinct plain locals qualifies; stores into the slots end the substitution.
"""
import re

_COMPONENT = r'\*\((?:(?:float|int|uint|undefined4) \*{1,2}|undefined1 \(\*\) \[4\])\)'


def fold_split_dword_stores(text: str) -> str:
    """A dword stored as its four bytes (`X._8_1_ = (char)P; X._9_1_ = (char)((uint)P >> 8); ... >> 0x18`) is one
    dword store `X._8_4_ = P;`. V_ChickenKicking ChickenMaster Main (0x00E64FB0) builds the by-value copy of `me`
    it hands to GSI vtable 0xD30 that way; the byte form hid the copy from the thing rules and the Lua shifted nil."""
    def fold(m):
        base = int(m.group('o0'))
        offs = [int(m.group(f'o{i}')) for i in range(4)]
        if offs != [base, base + 1, base + 2, base + 3] or len({m.group(f'x{i}') for i in range(4)}) != 1 \
                or len({m.group(f'p{i}') for i in range(4)}) != 1:
            return m.group(0)
        return f"{m.group('ind')}{m.group('x0')}._{base}_4_ = {m.group('p0')};\n"
    shifts = ['', r'\(\(uint\)(?P<p1>\w+) >> 8\)', r'\(\(uint\)(?P<p2>\w+) >> 0x10\)', r'\(\(uint\)(?P<p3>\w+) >> 0x18\)']
    parts = [r'^(?P<ind>[ \t]*)(?P<x0>\w+)\._(?P<o0>\d+)_1_ = \(char\)(?P<p0>\w+);[ \t]*\r?\n']
    for i in range(1, 4):
        parts.append(r'[ \t]*(?P<x%d>\w+)\._(?P<o%d>\d+)_1_ = \(char\)%s;[ \t]*\r?\n' % (i, i, shifts[i]))
    return re.sub(''.join(parts), fold, text, flags=re.M)


def fold_vector_component_copies(text: str) -> str:
    vectors = set(re.findall(r'^\s*C3DVector(?:_bv)? \*(\w+);', text, re.M))
    for p in vectors:
        pe = re.escape(p)
        triple = re.compile(r'^(?P<ind>[ \t]*)(?P<x>\w+) = ' + _COMPONENT + pe + r';[ \t]*\r?\n'
                            r'[ \t]*(?P<y>\w+) = ' + _COMPONENT + r'&' + pe + r'->field_0x4;[ \t]*\r?\n'
                            r'[ \t]*(?P<z>\w+) = ' + _COMPONENT + r'&' + pe + r'->field_0x8;[ \t]*\r?\n', re.M)
        pos = 0
        while (m := triple.search(text, pos)):
            slots = [m.group('x'), m.group('y'), m.group('z')]
            # stack slots only: register temporaries (Roth's `uVar14 = pCVar6->x` feeding `center._0_4_`) are the
            # member-wise copy passes' business
            if len(set(slots)) != 3 or not all(re.search(r'(?:Stack_|local_)[0-9a-f]+$', s) for s in slots):
                pos = m.end()
                continue
            name = 'vec_' + re.sub(r'^\w*?(?:Stack_|local_)', '', slots[0])
            head, tail = text[:m.start()], text[m.end():]
            # contiguous slots (x at -0x188, y at -0x184, z at -0x180) form the vector in memory: the x slot's address
            # is the vector (ChickenMaster Main 0x00E64FB0 `IsDistanceFromThingToPositionUnder(me, &uStack_188, 6.0)`)
            offs = [re.search(r'(?:Stack_|local_)([0-9a-f]+)$', s) for s in slots]
            if all(offs) and [int(o.group(1), 16) for o in offs] == [int(offs[0].group(1), 16) - 4 * i for i in range(3)]:
                x = re.escape(slots[0])
                stores = re.search(r'^[ \t]*(?:' + '|'.join(re.escape(s) for s in slots) + r')(?:\._\d+_\d+_)? = ', tail, re.M)
                cut = stores.start() if stores else len(tail)
                tail = re.sub(r'(?:\(C3DVector(?:_bv)? \*\))?&' + x + r'\b', name, tail[:cut]) + tail[cut:]
            for axis, slot in zip('xyz', slots):
                s = re.escape(slot)
                # the slot holds the component until it is stored again, has its address taken, or is reused as an
                # object (`(CScriptThing_bv *)auStack_30` as a hidden result, a `._4_4_` field)
                nxt = re.search(r'^[ \t]*' + s + r'(?:\._\d+_\d+_)? = |&' + s + r'\b|\(\w+ \*+\)\s*' + s + r'\b|\b' + s + r'\.', tail, re.M)
                cut = nxt.start() if nxt else len(tail)
                cut = tail.rfind('\n', 0, cut) + 1 if nxt else cut
                region = re.sub(r'(?:\(float\))?(?<![\w.&])' + s + r'\b(?![.\[])', f'{name}.{axis}', tail[:cut])
                tail = region + tail[cut:]
            text = head + f'{m.group("ind")}{name} = ENGINE_VectorCopy({p});\n' + tail
            pos = len(head) + 1
    return text
