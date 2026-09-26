"""Evidence-driven lowering of aggregate/pointer field access in Ghidra C, before `annotate`.

The generic lifter understands scalar `*(T *)(this + OFF)` fields (state maps) and interface
slot calls. Retail quest classes also use, per the PDB layouts in a unit JSON:

  master data      *(T *)(*(int *)(this + MD) + OFF)             -> GSI->Get/SetMasterGameState("Name")
  Thing members    (CScriptThing *)(BASE + OFF), operator=, vcalls -> THING_Get/THING_Set("Name") pseudo-calls
  struct arrays    *(T *)(IDX * STRIDE + BASE + MEMBER + PARENT)  -> GSI->Get/SetStateKind(__key("Teams_", IDX, "_Member"))
  array pointers   this->MyTeam = &parent->Teams[i]; *(T *)(MyTeam + M) -> index kept in state, keyed access
  thing vectors    (end - begin) / 12, *(begin + i*12)              -> LIST_Count("Name"), LIST_At("Name", i)

Every rewrite is driven by offsets in the unit evidence; nothing is inferred from names. Pseudo
calls are turned into Lua by `finish_lua` after the lifter has run. Unmatched shapes are left
untouched for the lifter's own diagnostics.
"""
from __future__ import annotations

import re
import struct
from pathlib import Path

SELF = r'(?:this|param_1)'
TYPE = r'(?:undefined1|undefined4|undefined2|int|uint|char|byte|bool|float|short|ushort|long|ulong|CScriptThing|undefined)'
BOOL_TRUE = {'1', "'\\x01'", 'true'}
BOOL_FALSE = {'0', "'\\0'", 'false'}


def off_re(n):
    return f'(?:{hex(n)}|{n})'


def _lit(value, kind):
    v = ' '.join(value.split())
    if kind == 'Bool':
        if v in BOOL_TRUE:
            return 'true'
        if v in BOOL_FALSE:
            return 'false'
    return v


# Inlined CScriptThing::operator= (VC7.1 inlines the counted-pointer assignment): the source
# thing's Data (+4) and Info (+8) are read into temporaries, the destination's old Info is
# released (refcount--, destroy+delete at zero), Data/Info are stored, the new Info is retained.
RE_COUNTED_ASSIGN = re.compile(
    r'(?P<ind>[ \t]*)(?:(?P<i>\w+) = \*\(\w+ \*{1,2}\)\((?P<src>[^;\n]+?) \+ (?:8|0x8)\);\s*(?P<d>\w+) = \*\(\w+ \*{1,2}\)\((?P=src) \+ (?:4|0x4)\);'
    r'|(?P<d2>\w+) = \*\(\w+ \*{1,2}\)\((?P<src2>[^;\n]+?) \+ (?:4|0x4)\);\s*(?P<i2>\w+) = \*\(\w+ \*{1,2}\)\((?P=src2) \+ (?:8|0x8)\);)\s*'
    r'(?P<o>\w+) = \*\(\w+ \*{1,2}\)\((?P<dst8>[^;\n]+?)\);\s*'
    r'if \((?P=o) != (?P<iref>\w+)\) \{\s*'
    r'if \((?P=o) != \(int \*\)0x0\) \{\s*'
    r'\*(?P=o) = \*(?P=o) \+ -1;\s*'
    r'if \(\*\*\(int \*\*\)\((?P=dst8)\) == 0\) \{\s*'
    r'\(\*\(code \*\)\(\*\(int \*\*\)\((?P=dst8)\)\)\[1\]\)\(\);\s*'
    r'operator_delete\(\*\(void \*\*\)\((?P=dst8)\)\);\s*\}\s*\}\s*'
    r'\*\(\w+ \*{1,2}\)\((?P<dst4>[^;\n]+?)\) = (?P<dref>\w+);\s*'
    r'\*\(\w+ \*{1,2}\)\((?P=dst8)\) = (?P=iref);\s*'
    r'if \((?P=iref) != \(int \*\)0x0\) \{\s*\*(?P=iref) = \*(?P=iref) \+ 1;\s*\}\s*\}[ \t]*\r?\n?')


def _split_offset(expr):
    m = re.fullmatch(r'\s*(.+?) \+ (0x[0-9a-f]+|\d+)\s*', expr)
    if not m:
        return None, None
    return m.group(1), int(m.group(2), 0)


def fold_counted_pointer_assign(text):
    def repl(m):
        src = m.group('src') or m.group('src2')
        iname = m.group('i') or m.group('i2')
        dname = m.group('d') or m.group('d2')
        if m.group('iref') != iname or m.group('dref') != dname:
            return m.group(0)
        base8, off8 = _split_offset(m.group('dst8'))
        base4, off4 = _split_offset(m.group('dst4'))
        if base8 is None or base8 != base4 or off8 != off4 + 4:
            return m.group(0)
        thing = f'({base8} + {hex(off4 - 4)})' if off4 - 4 else f'({base8})'
        return f'{m.group("ind")}CScriptThing::operator=((CScriptThing *){thing},(CScriptThing *){src.strip()});\n'
    return RE_COUNTED_ASSIGN.sub(repl, text)

# Release to null (`thing = CScriptThing()` / reset): old Info released, Data and Info zeroed.
RE_COUNTED_RELEASE = re.compile(
    r'(?P<ind>[ \t]*)(?P<o>\w+) = \*\(int \*\*\)\((?P<dst8>[^;\n]+?)\);\s*'
    r'(?:\w+ = [^;\n]+;\s*)*'
    r'if \((?P=o) != \(int \*\)0x0\) \{\s*'
    r'\*(?P=o) = \*(?P=o) \+ -1;\s*'
    r'if \(\*\*\(int \*\*\)\((?P=dst8)\) == 0\) \{\s*'
    r'\(\*\(code \*\)\(\*\(int \*\*\)\((?P=dst8)\)\)\[1\]\)\(\);\s*'
    r'operator_delete\(\*\(void \*\*\)\((?P=dst8)\)\);\s*\}\s*'
    r'(?P<inner>\})?\s*'
    r'\*\(undefined4 \*\)\((?P<dst4>[^;\n]+?)\) = 0;\s*'
    r'\*\(undefined4 \*\)\((?P=dst8)\) = 0;\s*'
    r'(?(inner)|\})[ \t]*\r?\n?')


def fold_counted_pointer_release(text):
    def repl(m):
        base8, off8 = _split_offset(m.group('dst8'))
        base4, off4 = _split_offset(m.group('dst4'))
        if base8 is None or base8 != base4 or off8 != off4 + 4:
            return m.group(0)
        thing = f'({base8} + {hex(off4 - 4)})' if off4 - 4 else f'({base8})'
        kept = re.findall(r'^[ \t]*(\w+ = [^;\n]+;)', m.group(0)[m.group(0).index(';') + 1:m.group(0).index('if (')], re.M)
        prefix = ''.join(m.group('ind') + k + '\n' for k in kept)
        return f'{prefix}{m.group("ind")}CScriptThing::operator=((CScriptThing *){thing},(CScriptThing *)0x0);\n'
    return RE_COUNTED_RELEASE.sub(repl, text)


def join_wrapped_statements(text: str) -> str:
    """Ghidra wraps long expressions; the lifter is line-based. Join a line into the next while its
    parentheses are open, or while it ends mid-expression (identifier, comma or operator)."""
    out, buffer = [], ''
    for line in text.splitlines():
        stripped = line.strip()
        if buffer:
            buffer += ('' if stripped.startswith('(') and buffer.rstrip()[-1:].isalnum() else ' ') + stripped
        else:
            buffer = line.rstrip()
        code = re.sub(r'"(?:[^"\\]|\\.)*"|\'(?:[^\'\\]|\\.)*\'', '', buffer)
        depth = code.count('(') - code.count(')')
        head = buffer.strip()
        tail = head[-1:] if head else ''
        unterminated = bool(head) and not head.startswith(('//', '/*', '*', '#')) and head not in ('else', 'do') \
            and (tail.isalnum() or tail in ',=+-|&<>*/!_' or head.endswith('::'))
        if (depth > 0 or unterminated) and not stripped.endswith('{'):
            continue
        out.append(buffer)
        buffer = ''
    if buffer:
        out.append(buffer)
    return '\n'.join(out) + ('\n' if text.endswith('\n') else '')


RE_LOCAL_COUNTED_RELEASE = re.compile(
    r'^[ \t]*if \(\((\w+(?:\._\d_4_|\[\d\])?) != \(int \*\)0x0\) && \(\*\1 = \*\1 \+ -1, \*\1 == 0\)\) \{\s*\r?\n'
    r'[ \t]*(?:\(\*\(code \*\)(?:\1\[1\]|\(\1 \+ 4\))\)|\(\*\*\(code \*\*\)\(\1 \+ 4\)\))\(\);\s*\r?\n[ \t]*operator_delete\((?:\(void \*\))?\1\);\s*\r?\n[ \t]*\}[ \t]*\r?\n', re.M)


RE_LOCAL_COUNTED_RELEASE2 = re.compile(
    r'^[ \t]*if \((\w+(?:\._\d_4_|\[\d\])?) != \(int \*\)0x0\) \{\s*\r?\n[ \t]*\*\1 = \*\1 \+ -1;\s*\r?\n[ \t]*if \(\*\1 == 0\) \{\s*\r?\n'
    r'[ \t]*(?:\(\*\(code \*\)(?:\1\[1\]|\(\1 \+ 4\))\)|\(\*\*\(code \*\*\)\(\1 \+ 4\)\))\(\);\s*\r?\n[ \t]*operator_delete\((?:\(void \*\))?\1\);\s*\r?\n[ \t]*\}\s*\r?\n[ \t]*\}[ \t]*\r?\n', re.M)
# the same release on a slot Ghidra typed as something else (`(CCharString)0x0`, `*(int *)X`, `(int)X + 4`)
RE_LOCAL_COUNTED_RELEASE3 = re.compile(
    r'^[ \t]*if \(\((\w+(?:\._\d_4_|\[\d\])?) != (?:\((?:int \*|undefined\d?\s*\[\d\]|\w+)\))?0x0\)\s*&&\s*\(\*\(int \*\)\1 = \*\(int \*\)\1 \+ -1, \*\(int \*\)\1 == 0\)\) \{\s*\r?\n'
    r'[ \t]*\(\*\*\(code \*\*\)\(\(int\)\1 \+ 4\)\)\(\);\s*\r?\n[ \t]*operator_delete\(\(void \*\)\1\);\s*\r?\n[ \t]*\}[ \t]*\r?\n', re.M)
# the same release whose outer `if` also nulls the handle's slots (AttackPeople 0x00DFD600: `piStack_14 = 0;
# piStack_10 = 0;` before the closing brace) -- the release goes, the nulls stay and leave the `if`: when the
# counted pointer is null the handle is already empty, so the stores are what the Lua `= nil` is lifted from
RE_LOCAL_COUNTED_RELEASE4 = re.compile(
    r'^[ \t]*if \((\w+(?:\._\d_4_|\[\d\])?) != \(int \*\)0x0\) \{\s*\r?\n[ \t]*\*\1 = \*\1 \+ -1;\s*\r?\n'
    r'[ \t]*if \(\*\1 == 0\) \{\s*\r?\n'
    r'[ \t]*(?:\(\*\(code \*\)(?:\1\[1\]|\(\1 \+ 4\))\)|\(\*\*\(code \*\*\)\(\1 \+ 4\)\))\(\);\s*\r?\n'
    r'[ \t]*operator_delete\((?:\(void \*\))?\1\);\s*\r?\n[ \t]*\}\s*\r?\n'
    r'(?P<tail>(?:[ \t]*\w+ = \((?:int|undefined4|void) \*\)0x0;[ \t]*\r?\n){1,3})'
    r'(?P<ind>[ \t]*)\}[ \t]*\r?\n', re.M)


RE_SLOT_ZERO = re.compile(r'^[ \t]*(?:\w+\._\d+_4_ = 0;|(?:[A-Za-z]+Stack_|local_)[0-9a-f]+(?:\._\d+_4_|\[0\])? = \(int \*\)0x0;)[ \t]*\r?\n', re.M)   # (`X._4_4_ = (int *)0x0` / `X[0] = (int *)0x0`: a stack thing's Data nulled beside its `= nil`, 2026-09-21)


RE_TANGLED_RELEASE_A = re.compile(
    r'^([ \t]*)if \(\((\w+) == \(int \*\)0x0\) \|\| \(\*\2 = \*\2 \+ -1, \*\2 != 0\)\)\s*\r?\n[ \t]*goto (LAB_\w+);\s*\r?\n'
    r'[ \t]*\(\*\(code \*\)\2\[1\]\)\(\);\s*\r?\n[ \t]*goto (LAB_\w+);[ \t]*\r?\n', re.M)
RE_TANGLED_RELEASE_B = re.compile(
    r'^[ \t]*if \(\((\w+) != \(int \*\)0x0\) && \(\*\1 = \*\1 \+ -1, \*\1 == 0\)\) \{\s*\r?\n[ \t]*\(\*\(code \*\)\1\[1\]\)\(\);\s*\r?\n'
    r'(LAB_\w+):\s*\r?\n[ \t]*operator_delete\(\1\);\s*\r?\n[ \t]*\}[ \t]*\r?\n', re.M)


RE_THING_PARTS = re.compile(
    r'^(?P<ind>[ \t]*)(?P<i>\w+) = \*\(int \*\*\)\((?P<src>\w+) \+ (?:8|0x8)\);[ \t]*\r?\n'
    r'[ \t]*(?P<d>\w+) = \*\(undefined4 \*\)\((?P=src) \+ (?:4|0x4)\);[ \t]*\r?\n'
    r'[ \t]*if \((?P<dst>\w+) == (?P=i)\) goto (?P<label>LAB_[0-9a-f]+);[ \t]*\r?\n', re.M)


def fold_tangled_thing_assign(text: str) -> str:
    """`dst = src` (CScriptThing::operator=) inlined and then split by the decompiler across labels:
    parts of src are loaded, `if (dst.Info == src.Info) goto DONE`, the old Info is released (with a
    label inside the delete block), the parts are stored (possibly in shared code after an if/else),
    `DONE:`. The parts load + compare + release become `dst = src`; the stores are dropped."""
    parts_seen = []
    while (m := RE_THING_PARTS.search(text)):
        i, src, d, dst, label = m.group('i'), m.group('src'), m.group('d'), m.group('dst'), m.group('label')
        release = re.compile(
            r'[ \t]*if \(\(' + dst + r' != \(int \*\)0x0\) && \(\*' + dst + r' = \*' + dst + r' \+ -1, \*' + dst + r' == 0\)\) \{\s*\r?\n'
            r'[ \t]*\(\*\(code \*\)' + dst + r'\[1\]\)\(\);\s*\r?\n(?:(?:LAB_[0-9a-f]+:\s*\r?\n)?[ \t]*operator_delete\(' + dst + r'\);|[ \t]*goto LAB_[0-9a-f]+;)\s*\r?\n[ \t]*\}[ \t]*\r?\n')
        rm = release.match(text, m.end())
        cut_end = rm.end() if rm else m.end()
        text = text[:m.start()] + f'{m.group("ind")}{dst} = {src};\n' + text[cut_end:]
        parts_seen.append((i, d, dst, label))
    for i, d, dst, label in parts_seen:
        # stores of the loaded parts (either slice or canonicalised spelling) and the source addref
        text = re.sub(r'^[ \t]*\w+(?:\._\d+_4_)? = ' + d + r';[ \t]*\r?\n', '', text, flags=re.M)
        text = re.sub(r'^[ \t]*\w+(?:\._\d+_4_)? = ' + i + r';[ \t]*\r?\n', '', text, flags=re.M)
        text = re.sub(r'^[ \t]*if \(' + i + r' != \(int \*\)0x0\) \{\s*\r?\n[ \t]*\*' + i + r' = \*' + i + r' \+ 1;\s*\r?\n[ \t]*\}[ \t]*\r?\n', '', text, flags=re.M)
    return text


RE_COLOUR_BYTE = re.compile(r'^[ \t]*(\w+)\._([0-3])_1_ = (0x[0-9a-f]+|\d+);[ \t]*\r?\n', re.M)


def fold_low_byte_flags(text: str) -> str:
    """A dword slot whose LOW byte carries a flag (`X = CONCAT31(X._1_3_, flag); if ((char)X == '\\0')`, the upper
    bytes are stale): the byte is its own local `X_flag`, and `CONCAT31((int3)(junk >> 8), (char)X)` -- the dword
    rebuilt from a dead register -- is that byte (DarkwoodTrader HandleTrader 0x00E0A510's `brain_state`, reused for
    the INFECTED / SCARED / FRIENDLY tests and then as a colour; lifted raw it left `extraout_EAX` free, 2026-09-24)."""
    for var in set(re.findall(r'\b(\w+) = CONCAT31\(\1\._1_3_,', text)):
        v = re.escape(var)
        flag = var + '_flag'
        text = re.sub(r'\b' + v + r' = CONCAT31\(' + v + r'\._1_3_,\s*([^;]+)\);', flag + r' = \1;', text)
        text = re.sub(r'CONCAT31\(\(int3\)\([^;]*?\),\s*\(char\)' + v + r'\)', flag, text)
        text = re.sub(r'\(char\)' + v + r'\b', flag, text)
    return text


def fold_dword_colours(text: str) -> str:
    """A CRGBColour stored as one dword literal (`X = -0x10000;` = 0xFFFF0000, BGRA bytes B=0 G=0 R=0xff A=0xff) and
    passed as `(CRGBColour_bv *)&X` becomes an FSE colour table (DarkwoodTrader HandleTrader's trader health bar:
    left as a raw slot the AddQuestInfoBarHealth call stayed a TODO and TraderHealthID was nil, 2026-09-24)."""
    pos = 0
    store = re.compile(r'^[ \t]*(\w+) = (-?0x[0-9a-f]+|-?\d+);[ \t]*\r?\n', re.M)
    while (m := store.search(text, pos)):
        var = m.group(1)
        scope = text[m.end():]
        nxt = re.search(r'^[ \t]*' + re.escape(var) + r' = ', scope, re.M)
        use = scope[:nxt.start()] if nxt else scope
        cast = r'\(CRGBColour(?:_bv)? \*\)&' + re.escape(var) + r'\b'
        if not re.search(cast, use) or re.search(r'(?<!\(CRGBColour_bv \*\)&)(?<!\(CRGBColour \*\)&)\b' + re.escape(var) + r'\b', re.sub(cast, '', use)):
            pos = m.end()
            continue        # not (only) a colour operand before the slot's next definition
        value = int(m.group(2), 0) & 0xffffffff
        b, g, r, a = value & 0xff, (value >> 8) & 0xff, (value >> 16) & 0xff, (value >> 24) & 0xff
        use = re.sub(cast, f'ENGINE_Colour({r}, {g}, {b}, {a})', use)
        text = text[:m.start()] + use + (scope[nxt.start():] if nxt else '')
        pos = m.start()
    return text


def gather_colour_byte_stores(text: str) -> str:
    """Two colours built at once interleave their byte stores with other plain setup lines (BanditKing Main
    0x00D0A830's Twinblade bar: `b._2_1_ = 0xff; b._3_1_ = 0xff; a._2_1_ = ..; pColour2 = &b; pColour1 = &a;
    fVar8 = 0.0; b._1_1_ = 0; b._0_1_ = 0; ...`). Inside one run of such lines (literal / address / plain copies,
    no calls or control flow) the four byte stores of a stack slot are one colour: move them together, at the
    slot's first store, so `fold_stack_colours` sees the run it expects."""
    store = re.compile(r'^[ \t]*(\w*Stack_\w+)\._([0-3])_1_ = (?:0x[0-9a-f]+|\d+);[ \t]*$')
    simple = re.compile(r'^[ \t]*\w+(?:\._[0-3]_1_)? = (?:&?\w+|-?[\d.]+|0x[0-9a-f]+);[ \t]*$')
    lines = text.split('\n')
    i = 0
    while i < len(lines):
        if not simple.match(lines[i]):
            i += 1
            continue
        j = i
        while j < len(lines) and simple.match(lines[j]):
            j += 1
        block = lines[i:j]
        stores = {}
        for k, line in enumerate(block):
            m = store.match(line)
            if m:
                stores.setdefault(m.group(1), []).append((k, int(m.group(2))))
        for var, found in stores.items():
            if sorted(b for _, b in found) != [0, 1, 2, 3]:
                continue
            ks = [k for k, _ in found]
            if ks == list(range(ks[0], ks[0] + 4)):
                continue        # already one run
            if any(re.search(r'\b' + re.escape(var) + r'\b', block[k]) for k in range(len(block)) if k not in ks and k > ks[0]
                   and not re.match(r'^[ \t]*\w+ = &' + re.escape(var) + r';', block[k])):
                continue        # the slot is read inside the run: keep the order
            moved = [block[k] for k in ks]
            rest = [line for k, line in enumerate(block) if k not in ks]
            at = ks[0] - sum(1 for k in ks if k < ks[0])
            block = rest[:at] + moved + rest[at:]
            break
        else:
            i = j
            continue
        lines[i:j] = block      # re-scan the same block for the next colour
    return '\n'.join(lines)


def fold_stack_colours(text: str) -> str:
    """A CRGBColour built on the stack byte by byte (`c._0_1_ = B; c._1_1_ = G; c._2_1_ = R; c._3_1_ = A`,
    retail ABI is BGRA) and passed by address becomes an FSE colour table."""
    text = gather_colour_byte_stores(text)
    # one colour = one run of adjacent byte stores of the same slot name (the name may serve several
    # colours, or a string, elsewhere in the function)
    pos = 0
    while (m := RE_COLOUR_BYTE.search(text, pos)):
        var, bytes_ = m.group(1), {}
        run_end = m.start()
        for st in RE_COLOUR_BYTE.finditer(text, m.start()):
            if st.start() != run_end or st.group(1) != var:
                break
            bytes_[int(st.group(2))] = int(st.group(3), 0)
            run_end = st.end()
        if set(bytes_) != {0, 1, 2, 3}:
            pos = m.end()
            continue
        b, g, r, a = (bytes_[i] for i in range(4))
        head, scope = text[:m.start()], text[run_end:]
        # the stack slot may be reused (a CCharString later on): only the uses up to the next redefinition
        # (the constructor may take the slot uncast and without `&` when Ghidra typed it as an array:
        # `CCharString::CCharString(aCStack_14c,"BanditCampEntrance",-1)`, TraderToRescue 0x00DFE0F0 -- the colour
        # ran through it and IsRegionLoaded got `ENGINE_Colour(...)` -> `""` in the readable, 2026-09-20 audit)
        nxt = re.search(r'^[ \t]*(?:\w+::\w+\((?:\(\w+ \*\))?&?' + re.escape(var) + r'\b|' + re.escape(var) + r'(?:\._\d_1_)? = )', scope, re.M)
        use, rest = (scope[:nxt.start()], scope[nxt.start():]) if nxt else (scope, '')
        use = re.sub(r'(?:\(\w+ \*\))?&?' + re.escape(var) + r'\b', f'ENGINE_Colour({r}, {g}, {b}, {a})', use)
        # VC7.1 may load the colour's address into a register BEFORE the byte stores (`pCVar11 = &CStack_230;
        # CStack_230._2_1_ = 0xff; ...; AddQuestInfoBarHealth(thing, (CRGBColour_bv *)pCVar11, ...)`, GuildTrainingMelee
        # TheRealGuildmaster 0x00D58490): that register is the colour up to its next definition, and the load goes
        # (left alone the call kept a dangling `pCVar11`, lifted to a stale string, 2026-09-20 audit)
        tail_head = re.search(r'^([ \t]*)(\w+) = &' + re.escape(var) + r';[ \t]*\r?\n(?:[ \t]*\w+ = [^;\n]+;[ \t]*\r?\n)*$', head, re.M)
        if tail_head:
            reg = tail_head.group(2)
            nxt_reg = re.search(r'^[ \t]*' + re.escape(reg) + r' = ', use, re.M)
            reg_use, reg_rest = (use[:nxt_reg.start()], use[nxt_reg.start():]) if nxt_reg else (use, '')
            reg_use = re.sub(r'(?:\(\w+ \*\))?\b' + re.escape(reg) + r'\b', f'ENGINE_Colour({r}, {g}, {b}, {a})', reg_use)
            use = reg_use + reg_rest
            head = head[:tail_head.start()] + re.sub(r'^[ \t]*' + re.escape(reg) + r' = &' + re.escape(var) + r';[ \t]*\r?\n', '', head[tail_head.start():], count=1, flags=re.M)
        text = head + use + rest
        pos = len(head)
    return text


RE_GSIVT_LOAD = re.compile(r'^([ \t]*)(\w+) = \*\*\(\w+ \*\*\)\(this \+ (0x40|4)\);[ \t]*\r?\n', re.M)   # the typing spec may have typed the spill slot (`**(CCharString **)`)


def isolate_gsi_vtable_temps(text: str) -> str:
    """Ghidra reuses one register temporary for the GSI vtable (`iVar4 = **(int **)(this + 0x40)`)
    and for ordinary values; the lifter's alias tracking then loses the ordinary values. Each vtable
    load gets its own name, scoped to its uses up to the next reassignment."""
    n = 0
    pos = 0
    while (m := RE_GSIVT_LOAD.search(text, pos)):
        var = m.group(2)
        n += 1
        alias = f'gsivt{n}'   # a plain identifier: the annotate pass collects GSI aliases by assignment shape
        head, tail = text[:m.end()], text[m.end():]
        redefine = re.compile(r'^[ \t]*' + re.escape(var) + r' = ', re.M)
        nxt = redefine.search(tail)
        cut = nxt.start() if nxt else len(tail)
        if nxt:
            # the reassignment may BE the vcall through the loaded vtable (`iVar5 = (**(code **)(iVar5 + 0x51c))(..)`,
            # Q_BanditCamp CheckAnyBanditsKilled 0x00D032F0's AddQuestInfoCounter): its right side still reads the load
            line_end = tail.find('\n', cut)
            line = tail[cut:line_end if line_end >= 0 else len(tail)]
            if re.search(r'\(\*\*\((?:code \*\*\)\((?:\(int\))?)?' + re.escape(var) + r' \+ ', line.split('=', 1)[1]):
                after = redefine.search(tail, line_end + 1) if line_end >= 0 else None
                cut = after.start() if after else len(tail)
        scope, rest = tail[:cut], tail[cut:]
        scope = re.sub(r'\(\*\*\(code \*\*\)\(' + re.escape(var) + r' \+ ', f'(**(code **)({alias} + ', scope)
        scope = re.sub(r'\(\*\*\(code \*\*\)\(\(int\)' + re.escape(var) + r' \+ ', f'(**(code **)({alias} + ', scope)
        scope = re.sub(r'\(\*\*\(' + re.escape(var) + r' \+ ', f'(**(code **)({alias} + ', scope)     # the untyped vcall spelling `(**(X + 0x118))(`
        # (other uses of the same name inside the scope are NOT this load's value: Ghidra merged two register
        # lifetimes -- Orchard's ProcessGameRulesGood 0x00DD0F60 reloads the counter handle `iVar4 = iStack_10`,
        # takes the vtable into `iVar4` on the Whisper branch, and passes the handle to RemoveQuestInfoElement on
        # the other path. Leaving the load under the register's name made the lifter treat the register as the
        # interface alias and back-fill the handle from the pool (`RemoveQuestInfoElement(ePriority)`, 2026-09-20
        # audit). The vcalls take the alias; every other use keeps the register and its previous definition.)
        scope = re.sub(r'\(\*\(this \+ ' + m.group(3) + r'\)', '(*(int **)(this + ' + m.group(3) + ')', scope)   # the receiver operand without its cast
        text = head[:m.start()] + f'{m.group(1)}{alias} = **(int **)(this + {m.group(3)});\n' + scope + rest
        pos = m.start() + 1
    return text


def drop_tangled_releases(text):
    text = RE_TANGLED_RELEASE_A.sub(lambda m: f'{m.group(1)}goto {m.group(3)};\n', text)
    for m in list(RE_TANGLED_RELEASE_B.finditer(text)):
        label = m.group(2)
        if len(re.findall(r'\b' + label + r'\b', text)) == 1:
            text = text.replace(m.group(0), '', 1)
    return text


RE_COUNTED_ADDREF = re.compile(
    r'^[ \t]*(\w+) = \*\(int \*\*\)\([^;]+\);[ \t]*\r?\n[ \t]*if \(\1 != \(int \*\)0x0\) \{\s*\r?\n[ \t]*\*\1 = \*\1 \+ 1;\s*\r?\n[ \t]*\}[ \t]*\r?\n', re.M)


def drop_local_counted_releases(text):
    text = drop_tangled_releases(text)
    text = RE_COUNTED_ADDREF.sub('', text)
    text = RE_LOCAL_COUNTED_RELEASE.sub('', text)
    text = RE_LOCAL_COUNTED_RELEASE2.sub('', text)
    text = RE_LOCAL_COUNTED_RELEASE3.sub('', text)
    text = RE_LOCAL_COUNTED_RELEASE4.sub(lambda m: ''.join(l.lstrip() and m.group('ind') + l.lstrip() for l in m.group('tail').splitlines(keepends=True)), text)
    return RE_SLOT_ZERO.sub('', text)


def fold_local_thing_copies(text: str) -> str:
    """Copy a canonicalised stack thing after its native release was removed.

    Both destination member stores must refer to the same proven thing object;
    the source's Data/Info loads and retain must agree exactly.
    """
    things = set(re.findall(r'\b(\w+) = QUESTTHING_Empty\(', text))
    pattern = re.compile(
        r'^(?P<ind>[ \t]*)(?P<info>\w+) = \*\(int \*\*\)\((?P<src>\w+) \+ (?:8|0x8)\);\s*'
        r'(?P<data>\w+) = \*\((?:int \*\*|undefined4 \*)\)\((?P=src) \+ (?:4|0x4)\);\s*'
        r'if \((?P<dst>\w+) != (?P=info)\) \{\s*'
        r'(?P=dst) = (?P=data);\s*(?P=dst) = (?P=info);\s*'
        r'if \((?P=info) != \(int \*\)0x0\) \{\s*'
        r'\*(?P=info) = \*(?P=info) \+ 1;\s*\}\s*\}', re.M)
    return pattern.sub(lambda m: f'{m["ind"]}{m["dst"]} = {m["src"]};'
                       if m['dst'] in things else m[0], text)


RE_BV_THING_CTOR = re.compile(
    r'^[ \t]*CScriptThing::CScriptThing\s*\(\s*\(CScriptThing \*\)&(stack0x[0-9a-f]+|xStack_[0-9a-f]+(?:_\d+)?),\s*(?:\(CScriptThing \*\))?(\w+)(?:,[^;]*)?\);[ \t]*\r?\n', re.M)


def fold_by_value_things(text: str, code_range=None) -> str:
    """By-value CScriptThing arguments (typed export): the copy constructor fills an outgoing slot
    (`&stack0xNN`) and the decompiler reassembles it into a `pThing._0_4_/_4_4_/_8_4_` temporary that
    is passed to the call. The argument is simply the copied source."""
    if code_range:
        lo, hi = code_range
        text = re.sub(r'^[ \t]*\w+ = (?:\([\w ]+\*+\))?0x([0-9a-f]{6,7});[ \t]*\r?\n',
                      lambda m: '' if lo <= int(m.group(1), 16) < hi else m.group(0), text, flags=re.M)
    pos = 0
    while (m := RE_BV_THING_CTOR.search(text, pos)):
        slot, src = m.group(1), m.group(2)
        outgoing = slot.startswith('stack0x')     # (a restored `xStack_` name may also be a real local copy)
        window = text[m.end():]
        use = re.search(r'^[ \t]*(\w+)\._0_4_ = in_stack_' + slot + r';[ \t]*\r?\n', window, re.M) if outgoing else None
        if not use:
            # the slot staging may already have been substituted by an older `in_stack_X = value;` line
            # (fold_outgoing_stack_slots): the first `_0_4_` reassembly after the constructor is this copy
            near = re.search(r'^[ \t]*(\w+)\._0_4_ = [^;]+;[ \t]*\r?\n', window, re.M)
            if not near or window[:near.start()].count('\n') > 8:
                if outgoing:
                    text = text[:m.start()] + text[m.end():]
                else:
                    pos = m.end()
                continue
            use = near
        var = use.group(1)
        text = text[:m.start()] + text[m.end():]
        text = re.sub(r'^[ \t]*' + re.escape(var) + r'(?:\._\d+_[14]_|_b\d+) = [^;]+;[ \t]*\r?\n', '', text, flags=re.M)
        text = re.sub(r'\b' + re.escape(var) + r'\b', src, text)
    # the copy constructor inlined: Data / Info of a thing at `E + off` are loaded, Info addref'd, then
    # stored slice-wise with the CScriptThing vtable into the by-value temporary. The temporary is
    # reused for later copies: the substitution stops at its next slice store.
    while (m := RE_BV_THING_INLINE.search(text)):
        base, off, var = m.group('base'), m.group('off'), m.group('var')
        src = f'(CScriptThing *)({base} + {int(off, 0) - 4})' if int(off, 0) != 4 else f'(CScriptThing *){base}'
        head, tail = text[:m.start()], text[m.end():]
        tail = re.sub(r'^[ \t]*' + re.escape(var) + r'\._\d_4_ = [^;]+;[ \t]*\r?\n', '', tail, count=3, flags=re.M)
        nxt = re.search(r'^[ \t]*' + re.escape(var) + r'\._\d_4_ = ', tail, re.M)
        scope, rest = (tail[:nxt.start()], tail[nxt.start():]) if nxt else (tail, '')
        scope = re.sub(r'\b' + re.escape(var) + r'\b(?!\.)', src, scope)
        text = head + scope + rest
    return text


RE_BV_THING_RESULT = re.compile(
    r'^([ \t]*)(GSI->\w+|[\w:]+)\(\(CScriptThing \*\)&stack0x[0-9a-f]+,\s*([^;]*?)\);[ \t]*\r?\n'
    r'(?=[ \t]*(\w+)\._\d_4_ = )(?:[ \t]*\4\._\d_4_ = [^;]+;[ \t]*\r?\n){1,3}', re.M)
RE_BV_THING_INLINE = re.compile(
    r'^[ \t]*(?P<data>\w+) = \*\(undefined4 \*\)\((?P<base>\w+) \+ (?P<off>0x[0-9a-f]+|\d+)\);[ \t]*\r?\n'
    r'[ \t]*(?P<info>\w+) = \*\(int \*\*\)\((?P=base) \+ (?P<off2>0x[0-9a-f]+|\d+)\);[ \t]*\r?\n'
    r'[ \t]*if \((?P=info) != \(int \*\)0x0\) \{[ \t]*\r?\n[ \t]*\*(?P=info) = \*(?P=info) \+ 1;[ \t]*\r?\n[ \t]*\}[ \t]*\r?\n'
    r'(?=(?:[ \t]*\w+\._\d_4_ = [^;]+;[ \t]*\r?\n){0,2}[ \t]*(?P<var>\w+)\._0_4_ = &PTR_[A-Za-z_]*_01238c8c;)', re.M)


RE_SIGNED_POW2_DIV = re.compile(
    r'(?:\(int\))?\((?P<x>[^;()]*(?:\([^;()]*\)[^;()]*)*?) \+ \((?P=x) >> 0x1f & (?P<m>0x[0-9a-f]+|\d+)U?\)\) >> (?P<k>0x[0-9a-f]+|\d+)')


def fold_signed_pow2_division(text: str) -> str:
    """MSVC's signed `x / 2^k`: `(x + (x >> 31 & 2^k-1)) >> k` rounds toward zero (BanditKing Main 0x00D0A830,
    `KingHealth < initial * 3 / 4`). Lua's `>>` is a logical 64-bit shift and `3U` is not a Lua number, so spell
    the division: truncate the quotient like C."""
    def fold(m):
        k = int(m.group('k'), 0)
        if int(m.group('m'), 0) != (1 << k) - 1 or not 1 <= k <= 30:
            return m.group(0)
        return f'ENGINE_Trunc(({m.group("x")}) / {1 << k})'
    text = RE_SIGNED_POW2_DIV.sub(fold, text)
    # the same quotient Ghidra already recognised (`(int)CVar8 / 2`, BanditKing's half-health taunt): an integer
    # division, where Lua's `/` would compare against 2.5 instead of 2
    return re.sub(r'\(int\)(\w+) / (\d+)\b(?!\.)', r'ENGINE_Trunc(\1 / \2)', text)


def _balanced_end(text: str, at: int):
    """Index just past the parenthesis group opening at text[at] ('('), or None."""
    depth = 0
    for i in range(at, len(text)):
        depth += {'(': 1, ')': -1}.get(text[i], 0)
        if depth == 0:
            return i + 1
    return None


def _comma_st0_call(lines, first: int, name: str):
    """(line, column) of the call whose ST0 result `name` reads when both sit in one comma expression
    (`(CALL(args), <no other call> name`), else None. Searches the statement that holds the first read."""
    start = max(0, first - 6)
    window = '\n'.join(lines[start:first + 1])
    idx = len(window) - len(lines[first]) + re.search(r'\b' + re.escape(name) + r'\b', lines[first]).start()
    prefix = window[:idx]
    boundary = max(prefix.rfind(';'), prefix.rfind('{'), prefix.rfind('}'))
    found = None
    for m in re.finditer(r'GSI->\w+\(|\(\*\*\(code \*\*\)', prefix):
        if m.start() <= boundary or not prefix[:m.start()].rstrip().endswith('('):
            continue
        end = _balanced_end(prefix, m.end() - 1 if m.group(0).startswith('GSI') else m.start())
        if end is not None and not m.group(0).startswith('GSI'):
            end = _balanced_end(prefix, end) if prefix[end:end + 1] == '(' else None
        if end is None:
            continue
        rest = re.match(r'\s*,', prefix[end:])
        if rest and not re.search(r'GSI->|\(\*\*\(code|\b\w+\(', prefix[end + rest.end():]):
            found = m.start()
    if found is None:
        return None
    line_starts = [0]
    for l in lines[start:first + 1]:
        line_starts.append(line_starts[-1] + len(l) + 1)
    k = max(i for i, s in enumerate(line_starts[:-1]) if s <= found)
    return start + k, found - line_starts[k]


def bind_st0_results(text: str) -> str:
    """Ghidra drops the float (ST0) result of an overridden vtable call and reads it back as
    `extraout_ST0[_NN]`; each such name belongs to the nearest preceding call statement whose result
    was not assigned. Bind it: `fret_NN = <call>;` and use `fret_NN` where the extraout name was read."""
    names = sorted(set(re.findall(r'\bextraout_ST0(?:_\d+)?\b', text)), key=lambda n: (len(n), n))
    # `__ftol2`'s ST0 operand (typed by the export) when Ghidra lost the producing call's float result: a
    # `float10 value[_NN];` that is never assigned and only read as `__ftol2(value)` (BanditKing Main 0x00D0A830,
    # `GetHealth` then `fistp`). The same unassigned ST0 read as extraout_ST0, spelled as the parameter's name.
    for name in re.findall(r'^[ \t]*float10 (value(?:_\d+)?);', text, re.M):
        body = re.sub(r'^[ \t]*float10 ' + name + r';', '', text, flags=re.M)
        uses = [body[max(0, m.start() - 20):m.start()] for m in re.finditer(r'\b' + name + r'\b', body)]
        if uses and all(re.search(r'__ftol2\(\s*(?:\(float10\))?$', u) for u in uses):
            names.append(name)
    if not names:
        return text
    lines = text.split('\n')
    for name in names:
        first = next((k for k, l in enumerate(lines) if re.search(r'\b' + re.escape(name) + r'\b', l)
                      and not re.match(r'^\s*float10 ' + re.escape(name) + r';', l)), None)
        if first is None:
            continue
        suffix = name.split('_', 2)[2] if name.count('_') == 2 else name.split('_', 1)[1] if name.startswith('value_') else '0'
        fret = ('fret_' if name.startswith('extraout') else 'fret_v') + suffix
        # the ST0 read sits in a comma expression after its producing call (`while ((CALL(), K == extraout_ST0_01
        # || ...))`, BCGameMaster Main 0x00D067A0): the call in that expression is the source, re-evaluated with it,
        # not the nearest preceding statement (there, an unrelated void SetQuitTavernGame)
        inline = _comma_st0_call(lines, first, name)
        if inline is not None:
            k, at = inline
            lines[k] = lines[k][:at] + fret + ' = ' + lines[k][at:]
            lines = [re.sub(r'\b' + re.escape(name) + r'\b', fret, l) for l in lines]
            lines = [l for l in lines if not re.match(r'^\s*float10 ' + re.escape(fret) + r';', l)]
            continue
        for k in range(first - 1, -1, -1):
            l = lines[k]
            if re.match(r'^\s*(?:\(\*\*\(code \*\*\)|GSI->|[\w:]+::[\w~]+\s*\(|\w+\()', l) and l.rstrip().endswith(');') and ' = ' not in l.split('(')[0]                     and not re.match(r'^\s*(?:std::|NHeroInformationScreens::|C\w+::(?:~?C\w+|_\w+)\s*\(|operator_(?:delete|new)\(|\(\*\(code \*\))', l):    # not a ctor/dtor/release of a temp
                lines[k] = re.sub(r'^(\s*)', r'\1' + fret + ' = ', l, count=1)
                break
        else:
            continue
        lines = [re.sub(r'\b' + re.escape(name) + r'\b', fret, l) for l in lines]
        lines = [l for l in lines if not re.match(r'^\s*float10 ' + re.escape(fret) + r';', l)]
        # the call may sit INSIDE an expression (a comma form), where there is no statement to bind:
        # `(CALL(args), fret_N <op> X)` -> `(fret_N = CALL(args), fret_N <op> X)`
        lines = [re.sub(r'\((?P<call>(?:GSI->|\(\*\*\(code \*\*\))[^;\n]*?\)), (?=' + re.escape(fret) + r'\b)',
                        lambda m: '(' + fret + ' = ' + m.group('call') + ', ', l) for l in lines]
    return '\n'.join(lines)


EH_FLAG_SHAPES = [
    re.compile(r'^[ \t]*(?:byte|uint|undefined4|uchar|int|CCharString) (?P<f>\w+);[ \t]*$'),          # declaration (CCharString: the typed export's spelling of the slot-shared state)
    re.compile(r'^[ \t]*(?P<f>\w+) = !(\w+) && \2;[ \t]*$'),                                        # init (false)
    re.compile(r'^[ \t]*(?P<f>\w+) = (?:0|1|\(uint\)bVar\d+);[ \t]*$'),                              # init (zero / bit 0 on a zero / a cleared bool)
    re.compile(r'^[ \t]*(?P<f>\w+) = (?P=f) [|&] (?:0x[0-9a-f]+|\d+);[ \t]*$'),                     # set / clear a bit
    re.compile(r'^[ \t]*(?:\} else )?if \(\((?P<f>\w+) & (?:0x[0-9a-f]+|\d+)\) [!=]= 0\) \{[ \t]*$'),   # bit test guard
    re.compile(r"^[ \t]*(?:\} else )?if \(\(char\)(?P<f>\w+) < '\\0'\) \{[ \t]*$"),               # top-bit test guard
    re.compile(r"^[ \t]*(?:\} else )?if \(-1 < \(char\)(?P<f>\w+)\) \{[ \t]*$"),
]


def fold_flag_relays(text: str, slots) -> str:
    """MSVC reloads a slot-shared temp-destruction flag word through a register: `R = S; S = S | 0x2000; ...
    S = R | 0x6000;`, or `R = S; if ((S & 0x200) != 0) { R = S & 0xfffffdff; ~T } if ((R & 0x100) != 0) ...`
    (Trader Escort DarkwoodTrader Main 0x00E07640). R holds S's bits there, so spell those lines on S and drop the
    copy: the slot's flag phase then runs unbroken to its first string constructor and is dropped whole. Left alone,
    the phase ended at the copy and the rest reached Lua as bit tests on a nil local (v15, 2026-09-24).
    A register line that is not a flag operation leaves the copy untouched."""
    # the top-bit test spelled with an extra pair of parentheses: `if (((S & 0x80) != 0)) {`
    text = re.sub(r'^([ \t]*(?:\} else )?)if \(\(\((\w+) & (0x[0-9a-f]+|\d+)\) ([!=]= 0)\)\) \{', r'\1if ((\2 & \3) \4) {', text, flags=re.M)
    num = r'(?:0x[0-9a-f]+|\d+)'
    for slot in sorted(slots):
        s = re.escape(slot)
        if not re.search(r'^[ \t]*' + s + r' = ' + s + r' \| ' + num + r';', text, re.M):
            continue
        while True:
            lines = text.split('\n')
            copies = [i for i, l in enumerate(lines) if re.match(r'^[ \t]*(\w+) = ' + s + r';[ \t]*$', l) and not l.strip().startswith(slot + ' ')]
            done = True
            for c in copies:
                reg = re.match(r'^[ \t]*(\w+) = ', lines[c]).group(1)
                r = re.escape(reg)
                use = re.compile(r'\b' + r + r'\b')
                flagged = [re.compile(r'^[ \t]*(?:' + r + '|' + s + r') = (?:' + r + '|' + s + r') [|&] ' + num + r';[ \t]*$'),
                           re.compile(r'^[ \t]*(?:\} else )?if \(\(' + r + r' & ' + num + r'\) [!=]= 0\) \{[ \t]*$')]
                run = []
                for i in range(c + 1, len(lines)):
                    if not use.search(lines[i]):
                        continue
                    if re.match(r'^[ \t]*' + r + r' = ' + s + r';[ \t]*$', lines[i]):
                        break                              # the next relay starts here
                    if any(f.match(lines[i]) for f in flagged):
                        run.append(i)
                        continue
                    if re.match(r'^[ \t]*' + r + r' = ', lines[i]) and not re.search(r'\b' + r + r'\b', lines[i].split('=', 1)[1]):
                        break                              # the register takes an unrelated value: the relay is over
                    run = None                             # the register carries the flag somewhere else
                    break
                if not run:
                    continue
                for i in run:
                    lines[i] = use.sub(slot, lines[i])
                del lines[c]
                text = '\n'.join(lines)
                done = False
                break
            if done:
                break
    return text


def _relay_slot_flags(text: str) -> str:
    """Components of registers joined by bit-set copies (`A = B | K`) and plain / masked register copies, plus the
    stack slots they are parked in; each component goes through `_relay_slot_flag_group`. (The generic copy groups
    in `drop_eh_state_flags` also join through literal inits (`X = 0`), which merges unrelated locals.)"""
    ident = r'(?!\d)\w+'
    edge = re.compile(r'^[ \t]*(' + ident + r') = (' + ident + r')( [|&] (?:0x[0-9a-f]+|\d+))?;[ \t]*$', re.M)
    edges = [(m.group(1), m.group(2), m.group(3) or '') for m in edge.finditer(text) if m.group(1) != m.group(2)]
    regs = lambda n: 'Stack_' not in n
    seeds = {n for a, b, k in edges if ' | ' in k and regs(a) and regs(b) for n in (a, b)}
    done = set()
    for seed in sorted(seeds):
        if seed in done:
            continue
        component, grow = {seed}, True
        while grow:
            grow = False
            for a, b, _ in edges:
                if regs(a) and regs(b) and (a in component) != (b in component):
                    component |= {a, b}
                    grow = True
        done |= component
        slots = {s for a, b, _ in edges for s, r in ((a, b), (b, a)) if not regs(s) and r in component}
        relayed = _relay_slot_flag_group(text, component | slots)
        if relayed is not None:
            text = relayed
    return text


def _relay_slot_flag_group(text: str, group):
    """A flag word that alternates between registers AND is parked in a stack slot the function also uses for other
    values (Gate1GuardOuter Main 0x00D01630: `CVar13 = CVar12 | 1; xStack_124 = CVar13; ... CVar13 = xStack_124;
    ... CVar12 = CVar13 | 0x20;`, while `xStack_124` is also the "Gate1GuardInner" string and a vtable pointer).
    The registers are one flag when every line naming them is a flag shape, a copy between them, or a store to /
    reload from the slot; the slot is only a relay when, after each flag store, nothing but a reload reads it
    before its next real definition. Then the relay lines go and the registers are spelled as one name, which the
    single-flag rule below removes. Returns None (text untouched) for any other shape."""
    slots = {n for n in group if 'Stack_' in n}
    regs = group - slots
    if not slots or len(regs) < 2:
        return None
    num = r'(?:0x[0-9a-f]+|\d+)'
    names = '|'.join(re.escape(n) for n in sorted(regs))
    member = re.compile(r'\b(?:' + names + r')\b')
    cross = re.compile(r'^[ \t]*(\w+) = (\w+)(?: [|&] ' + num + r')?;[ \t]*$')
    lines = text.split('\n')
    relay, has_set = set(), False
    for i, line in enumerate(lines):
        if not member.search(line):
            continue
        m = cross.match(line)
        if m and m.group(1) in regs and m.group(2) in regs:
            has_set |= ' | ' in line
            continue
        if m and ((m.group(1) in slots and m.group(2) in regs) or (m.group(1) in regs and m.group(2) in slots
                                                                     and not re.search(r' [|&] ', line))):
            relay.add(i)
            continue
        m = next((sh.match(line) for sh in EH_FLAG_SHAPES if sh.match(line)), None)
        if not m or m.group('f') not in regs:
            return None
    if not has_set or not relay:
        return None
    for slot in slots:
        s = re.escape(slot)
        holds_flag = False
        for i, line in enumerate(lines):
            if not re.search(r'\b' + s + r'\b', line) or re.match(r'^[ \t]*[\w ]+\*? ' + s + r'(?: \[\d+\])?;[ \t]*$', line):
                continue
            if i in relay:
                holds_flag = re.match(r'^[ \t]*' + s + r' = ', line) is not None or holds_flag
                if not re.match(r'^[ \t]*' + s + r' = ', line) and not holds_flag:
                    return None                    # a reload of a value no flag store put there
                continue
            defines = (re.match(r'^[ \t]*' + s + r' = ', line) and not re.search(r'\b' + s + r'\b', line.split('=', 1)[1])
                       or re.search(r'&' + s + r'\b', line))
            if holds_flag and not defines:
                return None                        # the parked flag would be read as a real value
            if defines:
                holds_flag = False
    one = sorted(regs)[0]
    others = '|'.join(re.escape(n) for n in sorted(regs - {one}))
    lines = [l for i, l in enumerate(lines) if i not in relay
             and not re.match(r'^[ \t]*[\w ]+\*? (?:' + others + r');[ \t]*$', l)]
    text = '\n'.join(lines)
    text = re.sub(r'^([ \t]*)(?:' + names + r') = (?:' + names + r')( [|&] ' + num + r')?;[ \t]*$',
                  lambda m: f'{m.group(1)}{one} = {one}{m.group(2)};' if m.group(2) else '', text, flags=re.M)
    return member.sub(one, text)


def drop_eh_state_flags(text: str) -> str:
    """The compiler's exception-state byte (`bVar7 = !b && b; bVar7 |= 2; ... if ((bVar7 & 2) != 0) { bVar7 &= 0xfd;
    ~CCharString(...) }`) only guards destructor calls on the unwinding path. When every line that names a
    register is one of those shapes, its bit tests are constant (never taken) and its updates vanish. A copy
    through a second register (`uVar17 = uStack_7c | 0x40; ... uStack_7c = uVar17;`) is folded first; the
    second register may be reused for unrelated values outside that window.

    The typed export spells the state through the stack slot a string temporary reuses afterwards
    (`xStack_4 = (CCharString)0x0; ... CVar4 = xStack_4; if (((uint)xStack_4 & 1) != 0) { CVar4 =
    (CCharString)((uint)xStack_4 & 0xfffffffe); ~CCharString(&xStack_4) } ... CVar4 = (CCharString)((uint)
    CVar4 | 2); ... CCharString::CCharString(&xStack_4,"DoMission",-1)`). The casts are dropped and the
    slot's flag-phase lines (everything naming it before its first constructor) move onto the register, so
    the register is the one flag the rules above see (GuildTrainingWoodsMelee Main / DoMission)."""
    # the seed spelled from two never-assigned registers (`bVar14 = !bVar16 && !bVar15;`, Q_WhiteBalverineWW Main
    # 0x00E18630): Ghidra's rendering of uninitialised stack bytes; the word then only takes flag updates. Start it at
    # 0 so the rules below see a plain flag -- left alone it reached Lua as a boolean and `& 16` killed Main, 2026-09-26
    def zero_seed(m):
        ind, f, a, b = m.group(1), m.group(2), m.group(3), m.group(4)
        # constant false: the operands are each other's negation (`bVar16 = !bVar15;` then `!bVar16 && !bVar15`, the
        # disguised `!b && b`), or never assigned at all
        negated = any(re.search(r'^[ \t]*' + re.escape(x) + r' = !' + re.escape(y) + r';', text, re.M) for x, y in ((a, b), (b, a)))
        if not negated and any(re.search(r'^[ \t]*' + re.escape(x) + r' = ', text, re.M) for x in (a, b)):
            return m.group(0)
        if not re.search(r'\b' + re.escape(f) + r' [|&] (?:0x[0-9a-f]+|\d+)', text):
            return m.group(0)
        return f'{ind}{f} = 0;'
    text = re.sub(r'^([ \t]*)(\w+) = !(\w+) && !(\w+);', zero_seed, text, flags=re.M)
    # the sign test of a byte slice is a bit test (`CVar6._0_1_ < '\0'` = bit 0x80, `._1_1_` = 0x8000): Ghidra's
    # rendering of `test byte, 0x80` on the flag word (TraderConflictEvil CTC_BanditFighter 0x00DF8970 -- the slice
    # became a fresh nil scalar `CVar6_b0` under the byte-store rename, 2026-09-20 audit)
    # CRT `rand()` printed with the stale registers as operands (`rand((int)unaff_EDI,unaff_ESI,unaff_EBP,unaff_EBX)`,
    # TraderToRescue 0x00DFE0F0) is not a use of the flag register: fold it here as `lower` does later, or the flag's
    # line check fails and `unaff_EBP | 1` / `& 0x20` survive into Lua as a free global
    text = re.sub(r'(?<![\w:])rand\((?:[^()]|\([^()]*\))*\)', 'ENGINE_Rand()', text)
    bitwise_early = {m.group(1) for m in re.finditer(r'\(uint\)(\w+) [|&] (?:0x[0-9a-f]+|\d+)', text)}
    text = re.sub(r"\b(\w+)\._([01])_1_ (<|>=|>) (?:'\\0'|-1)(?![\w'])",
                  lambda m: (f"(((uint){m.group(1)} & {'0x80' if m.group(2) == '0' else '0x8000'}) {'!=' if m.group(3) == '<' else '=='} 0)"
                             if m.group(1) in bitwise_early else m.group(0)), text)
    bitwise = {m.group(1) for m in re.finditer(r'\(uint\)(\w+) [|&] (?:0x[0-9a-f]+|\d+)', text)}    # only a name in bit ops is state (a `(CCharString)0x0` movie/handle init stays)
    text = re.sub(r'\((?:CCharString|CCharString_bv|uint|byte|uchar|int \*|undefined \*\*)\)\(\(uint\)(\w+) ([|&]) (0x[0-9a-f]+|\d+)\)', r'\1 \2 \3', text)
    text = re.sub(r'\(\(uint\)(\w+) & (0x[0-9a-f]+|\d+)\) ([!=]= 0)', r'(\1 & \2) \3', text)
    text = re.sub(r'^([ \t]*)(\w+) = \((?:CCharString|CCharString_bv|int \*|undefined \*\*)\)(0x[0-9a-f]+|\d+);',
                  lambda m: f'{m.group(1)}{m.group(2)} = {int(m.group(3), 0)};' if m.group(2) in bitwise else m.group(0), text, flags=re.M)
    # A slot that is the flag first and something else later, with NO register copy in between (GuildTrainingMelee
    # TheRealGuildmaster 0x00D58490: `int *piStack_23c` = the temp-destruction flag of the MsgIsHitByHero string,
    # then the HUD_WHISPER_ICON info-counter handle): the flag phase is every line naming the slot before its first
    # non-flag definition / address-taking; when those lines are all flag shapes they move onto a fresh register,
    # which the wholesale drop below then removes. Left alone, the `& 1` test survived under the slot's later
    # name (`if infoCounter & 1 ~= 0`, nil on the first frame of Main, 2026-09-20 audit).
    text = fold_flag_relays(text, bitwise)
    phase_shapes = [re.compile(r'^[ \t]*(\w+) = \d+;[ \t]*$'),
                    re.compile(r'^[ \t]*(\w+) = \1 [|&] (?:0x[0-9a-f]+|\d+);[ \t]*$'),
                    re.compile(r'^[ \t]*(?:\} else )?if \(\((\w+) & (?:0x[0-9a-f]+|\d+)\) [!=]= 0\) \{[ \t]*$'),
                    re.compile(r'^[ \t]*(?:CCharString|CCharString_bv|uint|byte|int|undefined4|int \*|undefined \*\*) \*?(\w+)(?: \[\d+\])?;[ \t]*$')]
    for slot in sorted(bitwise):
        lines = text.split('\n')
        name_re = re.compile(r'\b' + re.escape(slot) + r'\b')
        decl_re = re.compile(r'^[ \t]*(?:CCharString|CCharString_bv|uint|byte|int|undefined4|int \*|undefined \*\*) \*?' + re.escape(slot))
        init_re = re.compile(r'^[ \t]*' + re.escape(slot) + r' = 0;[ \t]*$')
        set_re = re.compile(r'^[ \t]*' + re.escape(slot) + r' = ' + re.escape(slot) + r' \| ')
        k = 0
        # every `X = 0;` may open a flag phase: the lines naming X from there up to the first non-flag definition /
        # address-taking (MeleeOpponent 0x00D56790: the movie handle lives in the slot FIRST, the block-stage flag
        # later -- a phase at the start only was not enough, `movie & 1` on nil left the blocks at 0/5, 2026-09-20)
        for start, l in enumerate(lines):
            if not init_re.match(l):
                continue
            phase = [start]
            for i in range(start + 1, len(lines)):
                l2 = lines[i]
                if not name_re.search(l2) or decl_re.match(l2):
                    continue
                if init_re.match(l2):
                    break                          # the next phase's own init
                if any(sh.match(l2) and sh.match(l2).group(1) == slot for sh in phase_shapes):
                    phase.append(i)
                    continue
                break
            if not any(set_re.match(lines[i]) for i in phase):
                continue
            k += 1
            reg = 'ehflag_' + re.sub(r'\W', '_', slot) + (f'_{k}' if k > 1 else '')
            for i in phase:
                lines[i] = name_re.sub(reg, lines[i])
        text = '\n'.join(lines)
        # A slot with flag shapes AND a non-flag definition (a movie / resource handle borrowed the slot between the
        # flag's top-of-function init and its use: MeleeOpponent's `xStack_bc = RESOURCE_StartMovie("")` at line 136,
        # `xStack_bc = xStack_bc | 1` at 369) has no contiguous phase; there the flag-shaped lines are the EH flag
        # wherever they are (a real bit-mask variable is never also a handle).
        lines = text.split('\n')
        flag_lines = [i for i, l in enumerate(lines) if name_re.search(l) and not decl_re.match(l)
                      and any(sh.match(l) and sh.match(l).group(1) == slot for sh in phase_shapes)]
        # (a handle life of the slot: a folded object constructor, or -- this early in the pipeline -- the inline
        # vtable stores of a movie / resource object and thing-cast reads of it)
        other_defs = [i for i, l in enumerate(lines)
                      if (re.match(r'^[ \t]*' + re.escape(slot) + r'(?:\[0\])? = (?:&PTR_|RESOURCE_\w+\(|ACTORMAP_New\(|STRINGMAP_New\(|QUESTTHING_Empty\(|GSI->\w+\()', l)
                          or re.search(r'\(CScriptThing(?:_bv)? \*\)' + re.escape(slot) + r'\b', l))
                      and i not in flag_lines]
        if 'Stack_' in slot and other_defs and any(set_re.match(lines[i]) for i in flag_lines):
            k += 1
            reg = 'ehflag_' + re.sub(r'\W', '_', slot) + (f'_{k}' if k > 1 else '')
            for i in flag_lines:
                if not init_re.match(lines[i]) or i > other_defs[0]:      # the top-of-function init before a handle stays a harmless 0
                    lines[i] = name_re.sub(reg, lines[i])
            text = '\n'.join(lines)
    for m in list(re.finditer(r'^[ \t]*(\w+) = (\w+)(?: & (?:0x[0-9a-f]+|\d+))?;[ \t]*$', text, re.M)):
        reg, slot = m.group(1), m.group(2)
        if reg == slot or not re.search(r'^[ \t]*' + re.escape(slot) + r' = 0;[ \t]*$', text, re.M):
            continue
        first_ctor = re.search(r'&' + re.escape(slot) + r'\b', text)
        head, tail = (text[:first_ctor.start()], text[first_ctor.start():]) if first_ctor else (text, '')
        if m.start() > len(head):
            continue
        phase = [l for l in head.splitlines() if re.search(r'\b' + re.escape(slot) + r'\b', l)]
        flag_shapes = [re.compile(r'^[ \t]*' + re.escape(slot) + r' = \d+;[ \t]*$'),
                       re.compile(r'^[ \t]*' + re.escape(slot) + r' = ' + re.escape(slot) + r' [|&] (?:0x[0-9a-f]+|\d+);[ \t]*$'),   # the slot's own set / clear before the register copy (WoodsWill 0x00D67890)
                       re.compile(r'^[ \t]*(?:\} else )?if \(\(' + re.escape(slot) + r' & (?:0x[0-9a-f]+|\d+)\) [!=]= 0\) \{[ \t]*$'),
                       re.compile(r'^[ \t]*' + re.escape(reg) + r' = ' + re.escape(slot) + r'(?: [|&] (?:0x[0-9a-f]+|\d+))?;[ \t]*$'),
                       re.compile(r'^[ \t]*(?:CCharString|CCharString_bv|uint|byte|int|undefined4|int \*|undefined \*\*) \*?' + re.escape(slot) + r'(?: \[\d+\])?;[ \t]*$')]
        if not phase or not all(any(sh.match(l) for sh in flag_shapes) for l in phase):
            continue
        head = re.sub(r'^[ \t]*' + re.escape(reg) + r' = ' + re.escape(slot) + r';[ \t]*\r?\n', '', head, flags=re.M)
        head = re.sub(r'^([ \t]*)' + re.escape(reg) + r' = ' + re.escape(slot) + r' ([|&] (?:0x[0-9a-f]+|\d+));', lambda mm: f'{mm.group(1)}{reg} = {reg} {mm.group(2)};', head, flags=re.M)
        head = re.sub(r'^([ \t]*)' + re.escape(slot) + r' = (\d+);', lambda mm: f'{mm.group(1)}{reg} = {mm.group(2)};', head, flags=re.M)
        head = re.sub(r'^([ \t]*)' + re.escape(slot) + r' = ' + re.escape(slot) + r' ([|&] (?:0x[0-9a-f]+|\d+));', lambda mm: f'{mm.group(1)}{reg} = {reg} {mm.group(2)};', head, flags=re.M)
        head = head.replace('(' + slot + ' & ', '(' + reg + ' & ')
        text = head + tail
    # the flag word threaded through several registers (`uVar12 = unaff_EBP | 1; .. unaff_EBP = uVar12 | 8; ..
    # uVar14 = uVar12 | 0x40; uVar12 = uVar14;`, TraderToRescue 0x00DFE0F0): a closed set of names whose every line is
    # a flag shape or a copy (with or without a set) between members is ONE flag; spell it under one name so the
    # single-register rules below see it (left alone, `unaff_EBP | 1` / `& 0x20` reached Lua as a free global)
    text = re.sub(r'\(char\)SUB41\((\w+),0\)', r'(char)\1', text)          # the low byte's sign test spelled through SUB41
    cross = re.compile(r'^[ \t]*(\w+) = (\w+)(?: [|&] (?:0x[0-9a-f]+|\d+))?;[ \t]*$')
    edges = [(m.group(1), m.group(2)) for line in text.splitlines() for m in [cross.match(line)] if m and m.group(1) != m.group(2)]
    groups = []
    for a, b in edges:
        ga = next((g for g in groups if a in g), None)
        gb = next((g for g in groups if b in g), None)
        if ga and gb and ga is not gb:
            ga |= gb
            groups.remove(gb)
        elif ga:
            ga.add(b)
        elif gb:
            gb.add(a)
        else:
            groups.append({a, b})
    text = _relay_slot_flags(text)
    for group in groups:
        if len(group) < 2 or not any(re.match(r'(?:unaff_E[A-Z]{2}|uVar\d+|bVar\d+)$', n) for n in group):
            continue
        names = '|'.join(re.escape(n) for n in group)
        pure = True
        for line in text.splitlines():
            if not re.search(r'\b(?:' + names + r')\b', line):
                continue
            m = cross.match(line)
            if m and m.group(1) in group and m.group(2) in group:
                continue
            m = next((sh.match(line) for sh in EH_FLAG_SHAPES if sh.match(line)), None)
            if not m or m.group('f') not in group:
                pure = False
                break
        if not pure:
            continue
        one = sorted(group)[0]
        text = re.sub(r'^([ \t]*)(?:' + names + r') = (?:' + names + r')( [|&] (?:0x[0-9a-f]+|\d+))?;[ \t]*$',
                      lambda m: f'{m.group(1)}{one} = {one}{m.group(2)};' if m.group(2) else '', text, flags=re.M)
        text = re.sub(r'\b(?:' + names + r')\b', one, text)
    seeds ={m.group(1) for m in re.finditer(r'^[ \t]*(\w+) = !(\w+) && \2;', text, re.M)}
    seeds |= {m.group(1) for m in re.finditer(r'^[ \t]*(\w+) = \1 \| (?:0x[0-9a-f]+|\d+);', text, re.M)}
    seeds |= {m.group(2) for m in re.finditer(r'^[ \t]*(\w+) = (\w+) \| (?:0x[0-9a-f]+|\d+);', text, re.M)}
    for flag in sorted(seeds):
        v = re.escape(flag)
        unfolded, lossy = text, False   # a set dropped with no copy back in its window is only right if the flag goes away
        for m in list(re.finditer(r'^[ \t]*(\w+) = ' + v + r'((?: \| (?:0x[0-9a-f]+|\d+))?);[ \t]*\r?\n', text, re.M)):
            g, suffix = m.group(1), m.group(2)
            pos = text.find(m.group(0))
            if g == flag or pos < 0:
                continue
            after = pos + len(m.group(0))
            nxt = re.search(r'^[ \t]*' + re.escape(g) + r' = ', text[after:], re.M)
            win_end = after + nxt.start() if nxt else len(text)
            window = text[after:win_end]
            back = re.compile(r'^([ \t]*)' + v + r' = ' + re.escape(g) + r';[ \t]*\r?\n', re.M)
            if re.search(r'\b' + re.escape(g) + r'\b', back.sub('', window)):
                continue        # the register carries the value somewhere else: not a plain copy
            if suffix and not back.search(window):
                lossy = True    # the bit set here has nowhere to go (a chain of sets, or a goto past the window)
            window = back.sub((lambda mm: f'{mm.group(1)}{flag} = {flag}{suffix};\n') if suffix else '', window)
            text = text[:pos] + window + text[win_end:]
        ok = True
        for line in text.splitlines():
            if not re.search(r'\b' + v + r'\b', line):
                continue
            m = next((sh.match(line) for sh in EH_FLAG_SHAPES if sh.match(line)), None)
            if not m or 'g' in m.groupdict():
                ok = False
                break
        if not ok:
            if lossy:
                text = unfolded
            continue
        text = re.sub(r'^[ \t]*' + v + r' = (?:!\w+ && \w+|\w+ [|&] (?:0x[0-9a-f]+|\d+)|\(uint\)\w+|0);[ \t]*\r?\n', '', text, flags=re.M)
        text = re.sub(r'\(' + v + r' & (?:0x[0-9a-f]+|\d+)\) != 0', 'false', text)
        text = re.sub(r'\(' + v + r' & (?:0x[0-9a-f]+|\d+)\) == 0', 'true', text)
        text = re.sub(r"\(char\)" + v + r" < '\\0'", 'false', text)
        text = re.sub(r"-1 < \(char\)" + v + r"\b", 'true', text)
    # the guarded destructor blocks are dead once their test is `false` (no label inside: nothing jumps in)
    text = re.sub(r'^[ \t]*if \(false\) \{[ \t]*\r?\n((?:(?![ \t]*(?:\}|LAB_|\w+:(?!:)))[^\n]*\n)*?)[ \t]*\}[ \t]*\r?\n',
                  lambda m: '' if not re.search(r'\{', m.group(1)) else m.group(0), text, flags=re.M)
    return text


def normalise_typed_decompile(text: str) -> str:
    """Typed exports (ExportTypedTranslationUnit) print a few shapes the untyped pipeline never saw."""
    # Ghidra names some jump targets `code_r0xADDR` instead of `LAB_ADDR` (Gate1GuardOuter Main 0x00D01630's
    # `if (AttackedOuterGateGuards == 0) goto code_r0x00d0283a;` -- keep waiting). Left unlifted, the condition
    # vanished and an unprovoked hero fell into GiveThingBestEnemyTarget: the gate guard attacked on arrival
    # (bc4, 2026-09-25; retail v16 on the same save stays peaceful). Spell them as ordinary labels.
    text = re.sub(r'\bcode_r0x([0-9a-f]{8})\b',
                  lambda m: f'LAB_{m.group(1)}' if f'LAB_{m.group(1)}' not in text else m.group(0), text)
    text = re.sub(r'return CONCAT31\([^;]*?,\s*(0|1)\);', lambda m: f'return {"true" if m.group(1) == "1" else "false"};', text)
    # the interface pointer cached in a STACK slot (`piStack_230 = *(int **)((int)this + 0x40);` then
    # `(**(code **)(*piStack_230 + 0x5ec))(piStack_230,true)`, Q_Arena Main 0x00F0FB70's pause/unpause around every
    # round's scenes): register copies annotate as GSI calls, but the slot was renamed by the stack-object folding
    # and its receiver dropped, so 32 such calls stayed TODOs. When every definition of the slot is the interface
    # pointer and its address is never taken, its vcalls are spelled on the pointer itself (2026-09-26)
    gsi_ptr = r'\*\((?:int|void|undefined4) \*\*\)\(\(int\)this \+ (?:4|0x40)\)|\*\((?:int|void|undefined4) \*\*\)\(this \+ (?:4|0x40)\)'
    for slot in set(re.findall(r'^[ \t]*(\w+Stack_[0-9a-f]+) = (?:' + gsi_ptr + r');', text, re.M)):
        s = re.escape(slot)
        defs = re.findall(r'^[ \t]*' + s + r' = ([^;]+);', text, re.M)
        if not defs or not all(re.fullmatch(gsi_ptr, d.strip()) for d in defs) or re.search(r'&' + s + r'\b', text):
            continue
        ptr = defs[0].strip()
        text = re.sub(r'\(\*\*\(code \*\*\)\(\*(?:\(int \*\))?' + s + r' \+ (0x[0-9a-f]+)\)\)\(' + s + r'\b',
                      lambda m: f'(**(code **)(*{ptr} + {m.group(1)}))({ptr}', text)
    # CWideScreenMagicPauseEntities (0x00CBE09A / dtor 0x00CBE0B3): a scope object whose ctor stores the interface
    # pointer and calls PauseAllNonScriptedEntities(true), whose dtor calls it with false -- the Arena's entity
    # scenes open one per scene. Its ctor, its inlined dtor (`(**(code **)(*(int *)S + 0x5ec))(0)` through the
    # stored member) and a dtor call become the interface calls they are (2026-09-26)
    for m in list(re.finditer(r'CWideScreenMagicPauseEntities::CWideScreenMagicPauseEntities\s*\(\s*(?:\([^()]*\))?&?(\w+),\s*\*\((?:int|void|undefined4) \*\)\(\(?(?:int\))?this \+ (4|0x40)\)\);', text)):
        slot, off = m.group(1), m.group(2)
        gsi = f'*(int **)(this + {off})'
        s = re.escape(slot)
        text = text.replace(m.group(0), f'(**(code **)(*{gsi} + 0x5ec))({gsi},true);')
        text = re.sub(r'\(\*\*\(code \*\*\)\(\*(?:\(int \*\)|\*\(int \*\*\))?' + s + r' \+ 0x5ec\)\)\((?:0|false)\);',
                      f'(**(code **)(*{gsi} + 0x5ec))({gsi},false);', text)
        text = re.sub(r'CWideScreenMagicPauseEntities::~CWideScreenMagicPauseEntities\s*\(\s*(?:\([^()]*\))?&?' + s + r'\);',
                      f'(**(code **)(*{gsi} + 0x5ec))({gsi},false);', text)
    # ... and through a stack slot Ghidra never saw written (the pointer was loaded into ECX for the __thiscall and
    # the receiver dropped: `(**(code **)(*(int *)CStack_8c + 0x5ec))(0)`, 45 of the Arena's entity calls). A
    # vtable offset past every CScriptThing slot (> 0x200) on such a phantom slot is the script interface; spell it
    # on the interface pointer this function uses (quest scripts `this + 0x40`, entity scripts `this + 4`)
    offsets = re.findall(r'\*\((?:int|void|undefined4) \*\*\)\(\(int\)this \+ (4|0x40)\)|\*\((?:int|void|undefined4) \*\*\)\(this \+ (4|0x40)\)', text)
    if offsets:
        counts = {}
        for a, b in offsets:
            counts[a or b] = counts.get(a or b, 0) + 1
        own = max(counts, key=counts.get)
        ptr = f'*(int **)(this + {own})'
        def phantom(m):
            slot, off, args = m.group(1), m.group(2), m.group(3)
            if int(off, 16) <= 0x200 or re.search(r'^[ \t]*' + re.escape(slot) + r' = ', text, re.M) or re.search(r'&' + re.escape(slot) + r'\b', text):
                return m.group(0)
            return f'(**(code **)(*{ptr} + {off}))({ptr}' + (f',{args}' if args.strip() else '') + ');'
        text = re.sub(r'\(\*\*\(code \*\*\)\(\*(?:\(int \*\))?(\w*Stack_[0-9a-f]+) \+ (0x[0-9a-f]+)\)\)\(([^;()]*)\);', phantom, text)
    text = re.sub(r"return CONCAT31\(\w+,\s*'\\x01' - (\w+)\);", r'return !\1;', text)
    text = re.sub(r"'\\x01' - \(([^;()]+(?:\([^;()]*\)[^;()]*)*)\)", r'!(\1)', text)
    text = re.sub(r'return CONCAT31\(\w+,\s*([^;]+)\);', r'return \1;', text)
    text = re.sub(r'^([ \t]*\w+ = )CONCAT31\(extraout_var\w*,\s*(\w+)\);', r'\1\2;', text, flags=re.M)
    text = re.sub(r'return \(uint\)extraout_var(?:_\d+)? << 8;', 'return false;', text)
    text = re.sub(r'return \(uint\)(\w+) << 8;', r'return false;', text)
    text = re.sub(r'\(int\)(this(?:_\d+)?)\b', r'\1', text)                 # (int)this + 0x40
    text = re.sub(r'\)[ \t]*\r?\n[ \t]*;', ');', text)                        # `...)` newline `;`
    text = re.sub(r'^([ \t]*\w+ = \([\w *]+\))[ \t]*\r?\n[ \t]+(?=\()', r'\1', text, flags=re.M)   # a cast alone before a wrapped vcall
    text = re.sub(r'\)\)[ \t]*\r?\n[ \t]+\(', '))(', text)                   # call head wrapped before its argument list
    text = join_wrapped_statements(text)
    # The known script interface field can inherit a CCharString pointer type
    # from a reused register (WaspBoss DoMission 0x00E12580). Its pointee is
    # still the interface, allowing the existing vtable-alias pass to work.
    text = re.sub(r'\*\(CCharString(?:_bv)? \*\*\)\(this \+ (4|0x40)\)',
                  r'*(int **)(this + \1)', text)
    text = re.sub(r'&("(?:[^"\\]|\\.)*")', r'\1', text)                      # &"literal" (propagated CCharString temp)
    text = re.sub(r'\*\((\w+ \*+)\)&(\w+)->field_0x([0-9a-f]+)', r'*(\1)(\2 + 0x\3)', text)
    text = re.sub(r'\*&(\w+)->field_0x([0-9a-f]+)', r'*(int *)(\1 + 0x\2)', text)
    text = re.sub(r'(\w+)->field_0x([0-9a-f]+)', r'*(int *)(\1 + 0x\2)', text)
    # a parent-quest int field read through the export's by-value-string cast (`CStack_180 = *(CCharString_bv *)
    # (*(int *)((int)this + 0x14) + 0x50)`: PreMelee Guildmaster 0x00D52E90's "last DummyHits" local, compared with
    # the field to re-arm the nag timer only when the count changes; with the cast the store stayed a TODO and the
    # `(CCharString_bv)0x0` init was inlined as `0x0 ~= DummyHits`, 2026-09-21): the field is the int, the local an int
    PARENT_INT_FIELD = r'\(\*\(int \*\)\(\(?(?:int\))?this \+ 0x14\) \+ (?:0x[0-9a-f]+|\d+)\)'
    typed_as_int = set(re.findall(r'^[ \t]*(\w+) = \*\(CCharString(?:_bv)? \*\)' + PARENT_INT_FIELD + r';', text, re.M))
    if typed_as_int:
        text = re.sub(r'\*\(CCharString(?:_bv)? \*\)(' + PARENT_INT_FIELD + r')', r'*(int *)\1', text)
        for name in typed_as_int:
            text = re.sub(r'^([ \t]*)' + re.escape(name) + r' = \(CCharString(?:_bv)?\)(0x[0-9a-f]+|\d+);',
                          lambda m: f'{m.group(1)}{name} = {int(m.group(2), 0)};', text, flags=re.M)
            text = re.sub(r'^([ \t]*)CCharString(?:_bv)? ' + re.escape(name) + r';', r'\1int ' + name + ';', text, flags=re.M)
    # the GSI pointer cached in a register the export mistyped as a by-value string (`CVar10 = *(CCharString_bv *)(this + 4);`
    # then `(**(code **)(*(int *)CVar10 + 0x120))((void *)CVar10, ..)`, the Skill Guildmaster's out-of-ring check
    # 0x00D5AE70): the plain interface-alias spelling, so the vcalls read `GSI->` and the alias is stripped
    # (a quest script's interface lives at this + 0x40: Q_WhiteBalverineKnotholeGlade Main 0x00E13F10 caches it in
    # `this_00` for its GetHero / GetThingWithScriptName ambush-marker poll)
    for reg, off in set(re.findall(r'^[ \t]*(\w+) = \*\(CCharString_bv \*\)\(this \+ (4|0x4|0x40)\);', text, re.M)):
        off = '4' if off in ('4', '0x4') else off
        text = re.sub(r'^([ \t]*)' + re.escape(reg) + r' = \*\(CCharString_bv \*\)\(this \+ (?:4|0x4|0x40)\);', r'\1' + reg + f' = *(int **)(this + {off});', text, flags=re.M)
        text = re.sub(r'\(\*\*\(code \*\*\)\(\*\(int \*\)' + re.escape(reg) + r' \+ (0x[0-9a-f]+)\)\)\(\(void \*\)' + re.escape(reg) + r'\b', r'(**(code **)(*' + reg + r' + \1))(' + reg, text)
    text = re.sub(r'\b(CScriptThing|CCharString|C3DVector|CRGBColour|CWideString|CRGBFloatColour)_bv\b', r'\1', text)
    text = fold_outgoing_stack_slots(text)
    text = re.sub(r'\*\) \(', '*)(', text)
    text = drop_eh_state_flags(text)
    text = re.sub(r'^[ \t]*(\w+) = !(\w+) && \2;[ \t]*\r?\n', '', text, flags=re.M)   # EH-state flag init (always false)
    text = re.sub(r'\((?:undefined\d?|uchar|char|byte)\s+\[\d+\]\)', '', text)   # array-typed value casts
    # a byte slice of a stack slot used as a scalar (`CStack_cc._3_1_ = call(); if (CStack_cc._3_1_ != 0)`)
    text = re.sub(r'\b(\w+)\._(\d+)_1_\b(?! = (?:0x[0-9a-f]+|\d+);)', r'\1_b\2', text)
    text = re.sub(r'(\b(?:this|\w+) \+ )(\d{2,})\b', lambda m: m.group(1) + hex(int(m.group(2))), text)
    text = re.sub(r'\bthis\[(\d{2,})\]', lambda m: f'this[{int(m.group(1)):#x}]', text)   # `this[100]`: a byte field printed with a decimal index
    # an int member updated through a pointer temporary (`P = (int *)(parent + 0x54); *P = *P + 1;`): the member itself
    text = re.sub(r'^([ \t]*)(\w+) = \(int \*\)\(((?:this|\*\(int \*\)\(this \+ 0x14\)) \+ (?:0x[0-9a-f]+|\d+))\);[ \t]*\r?\n[ \t]*\*\2 = \*\2 \+ (-?(?:0x[0-9a-f]+|\d+));[ \t]*\r?\n(?![ \t]*[^\n]*\b\2\b)',   # (an array element's member has its own rule)
                  lambda m: f'{m.group(1)}*(int *)({m.group(3)}) = *(int *)({m.group(3)}) + {m.group(4)};\n', text, flags=re.M)
    text = bind_st0_results(text)
    text = re.sub(r'::\s+(?=\w)', '::', text)   # `CScriptThing:: _Method_...(` after line joining
    text = re.sub(r'= \((?:CCharString|int|undefined4|uint)\)&PTR_', '= &PTR_', text)   # a cast on a vtable store (the slot Ghidra typed as a string)
    # a byte flag Ghidra merged into the dword slot below it: clearing / setting the top byte
    text = re.sub(r'^([ \t]*)(\w+) = \2 & 0xffffff;', r'\1\2_b3 = 0;', text, flags=re.M)
    text = re.sub(r'^([ \t]*)(\w+) = CONCAT13\((0x[0-9a-f]+|\d+),\s*(?:\(undefined3\))?\2\);', r'\1\2_b3 = \3;', text, flags=re.M)
    # the same flag computed through a second register: `X = CONCAT13(1,(int3)Y); if (C) { X = Y & 0xffffff; } Y = X;`
    text = re.sub(r'^([ \t]*)(\w+) = (?:\([\w *]+\))?CONCAT13\(1,\s*\((?:int3|undefined3)\)(\w+)\);[ \t]*\r?\n[ \t]*if \(([^\n]*)\) \{[ \t]*\r?\n'
                  r'[ \t]*\2 = (?:\([\w *]+\))?\(\(uint\)\3 & 0xffffff\);[ \t]*\r?\n[ \t]*\}[ \t]*\r?\n(?P<back>[ \t]*\3 = \2;[ \t]*\r?\n)?',
                  lambda m: f'{m.group(1)}{m.group(3) if m.group("back") else m.group(2)}_b3 = !({m.group(4)});\n', text, flags=re.M)
    text = re.sub(r"\(char\)\(\(uint\)(\w+) >> 0x18\) (==|!=) '\\0'", lambda m: f'{"!" if m.group(2) == "==" else ""}{m.group(1)}_b3', text)
    # Ghidra's `joined_r0x<addr>` block labels are ordinary labels to the goto passes
    def joined(m):
        name = f'LAB_{m.group(1)}'
        return name if not re.search(r'^\s*' + name + r':', text, re.M) else name + '0'
    text = re.sub(r'\bjoined_r0x([0-9a-f]+)\b', joined, text)
    # a switch on a slot Ghidra typed float prints its integer cases as denormals (k * 1.4013e-45)
    text = re.sub(r'\bcase (\d+(?:\.\d+)?e-4[45]):', lambda m: f'case {int(round(float(m.group(1)) / 1.4013e-45))}:', text)
    text = re.sub(r'\(int \*\)(\w+\._\d_4_)\b', r'\1', text)   # casts on a field slice of a stack object
    # counted pointers Ghidra typed `undefined **`: the same refcount idiom as the `int *` spelling
    text = re.sub(r'\(undefined \*\)\(\(int\)\*(\w+) \+ -1\)', r'*\1 + -1', text)
    text = re.sub(r'\*(\w+) == \(undefined \*\)0x0', r'*\1 == 0', text)
    text = re.sub(r'\(undefined \*\*\)0x0', '(int *)0x0', text)
    text = re.sub(r'return extraout_\w+;', 'return;', text)   # a void function whose EAX Ghidra guessed as a result
    # a loop counter Ghidra typed as a pointer steps through a field: `pCStack_38 = (T *)&pCStack_38->field_0x1;`
    # is `+= 1` (TraderConflictGood AttackPeople 0x00DFD600: the increment was a TODO and the index copy folded to
    # `0x0`, so the bandit loop never advanced, 2026-09-20 third audit)
    text = re.sub(r'^([ \t]*)(\w+) = \((?:[\w:]+ \*+)\)&\2->field_0x([0-9a-f]+);', lambda m: f'{m.group(1)}{m.group(2)} = {m.group(2)} + {int(m.group(3), 16)};', text, flags=re.M)
    text = re.sub(r'^([ \t]*)(\w+) = \((?:[\w:]+ \*+)\)&\*\(int \*\)\(\2 \+ (0x[0-9a-f]+|\d+)\);', lambda m: f'{m.group(1)}{m.group(2)} = {m.group(2)} + {int(m.group(3), 0)};', text, flags=re.M)   # (the same after the field rewrite)
    # the same slot doubles as an element pointer on the loop's exit paths (`pCStack_38 = (T *)(local_24 + (int)pCVar1 * 3)`
    # then a Speak operand): the counter phase -- init 0, `+= k`, the `<` test, `(int)X * 3` / `X / 0xc` indexing and the
    # statement copies `pCVar1 = X;` -- gets its own name so the lifter does not see arithmetic on a thing
    for var in set(re.findall(r'^[ \t]*(\w*Stack_[0-9a-f]+) = \1 \+ \d+;', text, re.M)):
        v = re.escape(var)
        if not re.search(r'^[ \t]*' + v + r' = (?:\([\w: ]+\*+\))?0(?:x0)?;', text, re.M) or not re.search(r'\b' + v + r' < ', text):
            continue
        if not re.search(r'^[ \t]*' + v + r' = \([\w: ]+\*+\)\(\w+ \+ ', text, re.M):
            continue        # no second life as an element pointer: the plain counter rules handle it
        ctr = 'ctr_' + (var.split('_', 1)[1] if '_' in var else var)
        text = re.sub(r'^([ \t]*)' + v + r' = ((?:\([\w: ]+\*+\))?0(?:x0)?);', r'\1' + ctr + r' = 0;', text, flags=re.M)
        text = re.sub(r'^([ \t]*)' + v + r' = ' + v + r' \+ (\d+);', r'\1' + ctr + r' = ' + ctr + r' + \2;', text, flags=re.M)
        text = re.sub(r'\b' + v + r' < ', ctr + ' < ', text)
        text = re.sub(r'\(int\)' + v + r' \* 3\b', '(int)' + ctr + ' * 3', text)
        text = re.sub(r'^([ \t]*)(\w+) = ' + v + r';', r'\1\2 = ' + ctr + ';', text, flags=re.M)
    # an integer counter kept in a slot Ghidra typed CCharString (a byte offset stepping through a vector):
    # `X = (CCharString)((int)X + 0xc);` with its `X = (CCharString)0x0;` start
    # an int flag toggled in a slot Ghidra typed CCharString: `X = (CCharString)(1 - (int)X);`
    text = re.sub(r'^([ \t]*)(\w+) = \(CCharString\)\((\d+) - \(int\)\2\);', r'\1\2 = \3 - \2;', text, flags=re.M)
    counters = set(re.findall(r'^[ \t]*(\w+) = \(CCharString\)\(\(int\)\1 \+ (?:0x[0-9a-f]+|\d+)\);', text, re.M))
    counters |= set(re.findall(r'^[ \t]*(\w+) = \(CCharString\)\(\(\(int\)\1 \+ (?:0x[0-9a-f]+|\d+)\) % \d+\);', text, re.M))   # a round-robin `x = (x + 1) % 5`
    counters |= {v for v in re.findall(r'^[ \t]*(\w+) = \d+ - \1;', text, re.M) if re.search(r'&' + re.escape(v) + r'\b', text)}   # (the toggled flag sharing a string temp's slot)
    # an int computed into the slot (`X = (CCharString)(int)ROUND(dist * k + 0.5); if (.. == (float)(int)X - 1.0) X =
    # (CCharString)((int)X + -1); CVar4 = X; rand() % (int)CVar4`, TraderConflictGood BanditExtra 0x00DFCA90 -- both
    # assignments were TODOs and the modulus read nil, 2026-09-20 third audit)
    counters |= set(re.findall(r'^[ \t]*(\w+) = \(CCharString(?:_bv)?\)\(int\)(?!\w*Stack_)\w*\(', text, re.M))
    for var in counters:
        v = re.escape(var)
        text = re.sub(r'^([ \t]*)' + v + r' = \(CCharString(?:_bv)?\)\(int\)(?=\w*\()', r'\1' + var + r' = ', text, flags=re.M)
        text = re.sub(r'^([ \t]*)' + v + r' = \(CCharString(?:_bv)?\)\(\(int\)' + v + r' \+ -1\);', r'\1' + var + r' = ' + var + r' - 1;', text, flags=re.M)
        text = re.sub(r'\(float\)\(int\)' + v + r'\b', var, text)
        text = re.sub(r'^([ \t]*)' + v + r' = \(CCharString\)\(\(int\)' + v + r' \+ (0x[0-9a-f]+|\d+)\);', r'\1' + var + r' = ' + var + r' + \2;', text, flags=re.M)
        text = re.sub(r'^([ \t]*)' + v + r' = \(CCharString\)\(\(\(int\)(\w+) \+ (0x[0-9a-f]+|\d+)\) % (\d+)\);', r'\1' + var + r' = (\2 + \3) % \4;', text, flags=re.M)   # (through a register copy too: `x = (CVar3 + 1) % 5`)
        text = re.sub(r'^([ \t]*)' + v + r' = \(CCharString\)0x0;', r'\1' + var + ' = 0;', text, flags=re.M)
        text = re.sub(r'\(int\)' + v + r'\b', var, text)
        # the slot doubles as a string temporary elsewhere (`&X`): the counter gets its own name
        # (when the slot is also read as a string buffer — `*(void **)X` — its typed null tests `X == (CCharString)0x0`
        # belong to the string, not the counter; `X == (CCharString)0x1` is always the counter)
        keep = r'(?! (?:==|!=) \(CCharString(?:_bv)?\)0x0)' if re.search(r'\*\(void \*\*\)' + v + r'\b', text) else ''
        text = re.sub(r'(?<![&\w])(?<!\*)(?<!\*\))' + v + r'\b' + keep,
                      'ctr_' + (var.split('_', 1)[1] if '_' in var else var), text)
        # the counter's own typed compares (`ctr == (CCharString)0x0` lifted to `== nil`): an int compare when the
        # slot's nearest preceding event is a counter assignment, the string's null test when it is a string fill
        slot = var.split('_', 1)[1] if '_' in var else var      # no `_<slot>` suffix: a plain local
        ctr = 'ctr_' + slot
        events = re.compile(r'^[ \t]*(?P<name>\w+_' + slot + r') = (?P<rhs>[^;]+);|&(?P<sname>\w+_' + slot + r')\b', re.M)

        def counter_compare(m, ctr=ctr, events=events):
            last = None
            for ev in events.finditer(text, 0, m.start()):
                last = ev.group('name') is not None and re.fullmatch(r'\d+|0x[0-9a-f]+|\(CCharString\)(?:0x[0-9a-f]+|\d+)|\(?\w+ \+ (?:\d+|0x[0-9a-f]+)\)?(?: % \d+)?|\d+ - \w+', ev.group('rhs').strip()) is not None
            return f'{ctr} {m.group(1)} {int(m.group(2), 0)}' if last else m.group(0)
        text = re.sub(r'\b' + re.escape(ctr) + r' ([!=<>]=?) \(CCharString(?:_bv)?(?: \*)?\)(0x[0-9a-f]+|\d+)\b', counter_compare, text)
    # x87 compare idiom: `(a < b) != (a == b)` is `a <= b` (Ghidra's rendering of fcomp/fnstsw/test 0x41)
    text = re.sub(r'(\*?\(?[\w.]+\)?(?:\([^()]*\))?) < ((?:\(float10\))?\*?[\w.]+(?:\([^()]*\))?) != \(\1 == \2\)', r'\1 <= \2', text)
    # and its negation `(a < b) == (a == b)` (both false: `test ah,0x41; jp` with neither C0 nor C3 -- PreMeleeWhisper
    # 0x00D5282C `fabs; fcomp [1.0]`) is `b < a`
    text = re.sub(r'(\*?\(?[\w.]+\)?(?:\([^()]*\))?) < ((?:\(float10\))?\*?[\w.]+(?:\([^()]*\))?) == \(\1 == \2\)', r'\2 < \1', text)
    # a float staged in a slot Ghidra typed as a CCharString array: `aCStack_1c[0] = (CCharString)(expr);`
    # read back as `(float)aCStack_1c[0]` -> a plain scalar local
    for m in list(re.finditer(r'^[ \t]*(\w+)\[0\] = \(CCharString(?:_bv)?\)', text, re.M)):
        var = m.group(1)
        scalar = 'f_stk_' + var.split('_', 1)[1] if '_' in var else 'f_' + var   # no `Stack_` in the name: a plain local to the lifter
        text = re.sub(r'\b' + re.escape(var) + r'\[0\] = \(CCharString(?:_bv)?\)([^;]+);', scalar + r' = \1;', text)
        text = re.sub(r'\(float\)' + re.escape(var) + r'\[0\]', scalar, text)
        # the same slot as an integer counter (`(int)aCStack_70[0] + 1`, `(uint)aCStack_70[0] < n`, `CVar = aCStack_70[0]`)
        text = re.sub(r'(?:\((?:int|uint)\))?\b' + re.escape(var) + r'\[0\](?!\s*=)', scalar, text)
    # the scalar spelling of the same thing: a float computed into a slot typed CCharString (`xStack_1d4 =
    # (CCharString)(float)fret_06; ... = (CCharString)(float)(((float10)f_stk_74 - fret_07) - ...); if (grade <
    # (float)xStack_1d4 ...)`, GuildTrainingMelee TheRealGuildmaster 0x00D58490's melee grade). The float phase
    # runs from the first such assignment to the slot's next string use (`&xStack_1d4`); the x87 `(float10)`
    # widenings are Lua numbers. Left alone both assignments were `TODO(native)` and the grade compare read nil.
    # (re-searched after each rewrite: offsets collected up front went stale once the first phase was rewritten,
    # and MagicBarrier Main's second force-field angle stayed a TODO and reached CreateEffectAtPos as nil, 2026-09-24)
    float_store = re.compile(r'^[ \t]*(\w+) = \(CCharString(?:_bv)?\)\(float\)', re.M)
    pos = 0
    while (m := float_store.search(text, pos)):
        var = m.group(1)
        pos = m.start() + 1
        scalar = 'f_stk_' + var.split('_', 1)[1] if '_' in var else 'f_' + var
        nxt = re.search(r'&' + re.escape(var) + r'\b', text[m.start():])
        end = m.start() + nxt.start() if nxt else len(text)
        phase = text[m.start():end]
        phase = re.sub(r'\b' + re.escape(var) + r' = \(CCharString(?:_bv)?\)\(float\)', scalar + ' = ', phase)
        phase = re.sub(r'\(float\)' + re.escape(var) + r'\b', scalar, phase)
        # bare copies of the value in the same phase (`CVar6 = CVar2; ... (float)CVar6`, RockTrollTrigger Main's
        # trigger distance: the copy read the unset string register and the distance test got nil, 2026-09-24)
        # (only a plain copy whose target is itself read as a float: the slot's later non-float life -- a handle,
        # a pointer -- shares this phase and must keep its name)
        # (the target's float read is looked for over the TARGET's life -- from the copy to its next assignment --
        # not just this slot's phase: BanditKingMissionProcess 0x00D109F0 copies the quarter-health threshold back
        # into CStack_4c, the slot is reused for a quest-name string, and only then does ModifyThingHealth read
        # `(float)CStack_4c`; judged on the phase alone the copy kept the string name and the threshold became "")
        def target_is_float(mm, base=m.start()):
            # (located in the ORIGINAL text: the phase has already been rewritten, so its offsets do not apply)
            copy = re.search(r'^[ \t]*' + re.escape(mm.group(2)) + r' = ' + re.escape(var) + r';', text[base:], re.M)
            rest = text[base + copy.end():] if copy else ''
            nxt_def = re.search(r'^[ \t]*' + re.escape(mm.group(2)) + r' = ', rest, re.M)
            life = rest[:nxt_def.start()] if nxt_def else rest
            return re.search(r'\(float\)' + re.escape(mm.group(2)) + r'\b', phase) or re.search(r'\(float\)' + re.escape(mm.group(2)) + r'\b', life)
        phase = re.sub(r'^([ \t]*)(\w+) = ' + re.escape(var) + r';',
                       lambda mm: f'{mm.group(1)}{mm.group(2)} = {scalar};' if target_is_float(mm) else mm.group(0),
                       phase, flags=re.M)
        phase = re.sub(r'\(float10\)', '', phase)
        text = text[:m.start()] + phase + text[end:]
    return text


def fold_outgoing_stack_slots(text: str) -> str:
    """With prototype overrides the decompiler sometimes models the outgoing argument area as
    `in_stack_XXXXXXXX` variables: return-address materialisations (`= (T *)0xADDR`, dropped) and
    value staging (`in_stack_X = value;` then `(int)in_stack_X` as an argument). Each staging
    assignment is inlined into the uses that follow it, up to the next assignment of that slot."""
    text = re.sub(r'^[ \t]*in_stack_[0-9a-f]+ = \([\w ]+\*+\)0x[0-9a-f]{6,7};[ \t]*\r?\n', '', text, flags=re.M)
    out, live = [], {}
    for line in text.split('\n'):
        m = re.match(r'^[ \t]*(in_stack_[0-9a-f]+) = (?:\([\w ]+\*+\))?([^;]+);[ \t]*\r?$', line)
        if m:
            live[m.group(1)] = m.group(2).strip()
            continue
        for var, value in live.items():
            line = re.sub(r'(?:\((?:int|\w+ \*+)\))?\b' + var + r'\b', lambda _: value, line)
        out.append(line)
    return '\n'.join(out)


# ---- cutscene actor maps -----------------------------------------------------------------------
# std::map<CCharString, CCountedPointer<...>> built inline (header node malloc + 4 links), filled with
# operator[] + counted-pointer assignment, run through RunCutsceneMacro_Func, destroyed with
# StdMap_Destroy_API. Lowered to the retail-resource API used by the readable New Oakvale package.
# (the head pointer may live in the stack slot's first member, spelled `xStack_48._0_4_` with `(undefined1 *)` /
# `(undefined4 *)` member casts: Q_WhiteBalverineWW Main 0x00E18630's CS_WBW_DRINK actor map, whose SetActor /
# RunMacro / DestroyActorMap then took the balverine thing as the map, 2026-09-26)
RE_MAP_NEW = re.compile(
    r'^(?P<ind>[ \t]*)(?:(?P<m0>\w+(?:\._0_4_)?) = \(undefined1 \*\)0x0;\s*)?(?P<map>\w+?)(?P<mem>\._0_4_)? = (?:\(\w+ \*\))?malloc\(0x24\);\s*'
    r'(?:[\w.]+ = 0;\s*)?\*(?:\(undefined1 \*\))?(?P=map)(?P=mem)? = 0;\s*\*\(undefined4 \*\)\((?P=map)(?P=mem)? \+ 4\) = 0;\s*'
    r'\*\((?:undefined1 \*\*|undefined4 \*)\)\((?P=map)(?P=mem)? \+ 8\) = (?P=map)(?P=mem)?;\s*'
    r'\*\((?:undefined1 \*\*|undefined4 \*)\)\((?P=map)(?P=mem)? \+ 0xc\) = (?P=map)(?P=mem)?;[ \t]*\r?\n', re.M)
RE_MAP_SET = re.compile(
    r'^(?P<ind>[ \t]*)CCharString::CCharString\(\(CCharString \*\)&?(?P<key>\w+),(?P<keyval>"[^"]*"|&DAT_[0-9a-f]+|\w+),-1\);\s*'
    r'(?P<node>\w+) = std::\s*map<CCharString,CCountedPointer<[^;]*?::operator\[\]\((?:\(map<[^;]*?\*\))?&?(?P<map>\w+),(?:\(CCharString \*\))?&?(?P=key)\);\s*'
    r'(?:[\w:]+::\w+\(\(?[\w ]*\*?\)?(?P=node)(?:,[^;]*)?\);\s*)?'
    r'(?P<body>(?:[^;{}]*;\s*){0,4})'
    r'if \(\w+ != \w+\) \{\s*if \(\w+ != \(int \*\)0x0\) \{\s*\*\w+ = \*\w+ \+ -1;\s*if \(\*\*\(int \*\*\)\((?P=node) \+ 0xc\) == 0\) \{\s*'
    r'\(\*\(code \*\)\(\*\(int \*\*\)\((?P=node) \+ 0xc\)\)\[1\]\)\(\);\s*operator_delete\(\*\(void \*\*\)\((?P=node) \+ 0xc\)\);\s*\}\s*\}\s*'
    r'\*\((?:CScriptThing|int|undefined4) \*\*?\)\((?P=node) \+ 8\) = (?P<data>\w+);\s*'
    r'\*\((?:int|undefined) \*\*\)\((?P=node) \+ 0xc\) = (?P<info>\w+);\s*'
    r'if \((?P=info) != \((?:int|undefined) \*\*?\)0x0\) \{\s*\*(?P=info) = \*(?P=info) \+ 1;\s*\}\s*\}\s*'
    r'(?:std::\s*_Cons_val<[^;(]*?\s*\(&(?P=key)\);[ \t]*\r?\n)?', re.M)
RE_MAP_RUN = re.compile(
    r'^(?P<ind>[ \t]*)CCharString::CCharString\(\(CCharString \*\)&?(?P<key>\w+),(?P<keyval>"[^"]*"|&DAT_[0-9a-f]+|\w+),-1\);\s*'
    r'RunCutsceneMacro_Func\((?:\(CCharString \*\))?&?(?P=key),&?(?P<map>\w+),\(void \*\)0x0,\(void \*\)0x0,(?P<setup>true|false),(?P<skip>true|false)\);[ \t]*\r?\n'
    r'(?:[ \t]*std::\s*_Cons_val<[^;(]*?\s*\(&?(?P=key)\);[ \t]*\r?\n)?', re.M)
RE_MAP_DESTROY = re.compile(r'^([ \t]*)StdMap_Destroy_API\(&?(\w+)(?:\.field_0x4)?\);', re.M)
RE_MAP_RUN_STRINGS = re.compile(
    r'^(?P<ind>[ \t]*)RunCutsceneMacro_Func\((?P<key>(?:\(CCharString \*\))?&?\(?[\w. +]+\)?),&?(?P<map>[\w.]+),\(void \*\)0x0,&?(?P<strings>\w+),(?P<setup>true|false),(?P<skip>true|false)\);', re.M)
RE_MAP_RUN_VAR = re.compile(
    r'^(?P<ind>[ \t]*)RunCutsceneMacro_Func\((?P<key>(?:\(CCharString \*\))?&?\(?[\w. +]+\)?),&?(?P<map>[\w.]+),\(void \*\)0x0,\(void \*\)0x0 ?,(?P<setup>true|false),(?P<skip>true|false)\);', re.M)
RE_MAP_NEW2 = re.compile(r'^([ \t]*)StdMap_Construct_API\(&?(\w+)\);', re.M)
# resource-valued maps (std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>): the value is a
# controlled-entity resource handle assigned with the resource operator=.
RE_MAP_SET2 = re.compile(
    r'^(?P<ind>[ \t]*)CCharString::CCharString\(\(CCharString \*\)&?(?P<key>\w+),(?P<keyval>"[^"]*"|&DAT_[0-9a-f]+|\w+),-1\);\s*'
    r'(?:(?P<alias>\w+) = (?P<thing>&?[\w.]+|RESLIST_At\((?:[^;()]|\([^;()]*\))*\));\s*)?(?:\w+ = 0x[0-9a-f]{6,7};\s*)?'
    r'(?P<node>\w+) = std::\s*map<CCharString,CCountedPointer<[^;]*?::operator\[\]\((?:\(\s*map<[^;]*?\*\))?\(?&?(?P<map>\w+)(?: \+ 4)?\)?,(?:\(CCharString \*\))?&?(?P=key)\);\s*'
    r'CScriptGameResourceObjectScriptedThingBase::operator=\s*\((?P=node),(?P<src>&?\w+)\);\s*'
    r'(?:std::\s*_Cons_val<[^;(]*?\s*\(&(?P=key)\);[ \t]*\r?\n)?', re.M)

# the same resource-valued map store with the resource operator= inlined to its refcount dance (bsim labels
# the node constructor arbitrarily: `CFourierAnalysis::CFourierAnalysis(node,(int)res)`)
RE_MAP_SET3 = re.compile(
    r'^(?P<ind>[ \t]*)CCharString::CCharString\(\(CCharString \*\)&?(?P<key>\w+),(?P<keyval>"[^"]*"|&DAT_[0-9a-f]+|\w+),-1\);\s*'
    r'(?P<node>\w+) = std::\s*map<CCharString,CCountedPointer<[^;]*?::operator\[\]\((?:\(\s*map<[^;]*?\*\))?\(?&?(?P<map>\w+)(?: \+ 4)?\)?,(?:\(CCharString \*\))?&?(?P=key)\);\s*'
    r'[\w:]+::\w+\((?P=node),\(int\)(?P<src>\w+)\);\s*'
    r'(?:\w+ = [^;]+;\s*){0,3}if \(\w+ != \w+\) \{(?:[^{}]|\{(?:[^{}]|\{[^{}]*\})*\})*\}[ \t]*\r?\n', re.M)


def _resolve_local_thing(body, data, info):
    """Follow `a = b;` copies inside the assignment body to the source thing of a map/actor store.
    Ghidra splits a stack CScriptThing into local_N (vtable), local_{N-4} (Data), local_{N-8} (Info)."""
    alias = dict(re.findall(r'(\w+) = (\w+);', body))
    for _ in range(4):
        data, info = alias.get(data, data), alias.get(info, info)
    m_data = re.fullmatch(r'\*\((?:\w+ \*+)\)\((\w+) \+ (?:4|0x4)\)', data)
    m_info = re.fullmatch(r'\*\((?:\w+ \*+)\)\((\w+) \+ (?:8|0x8)\)', info)
    if m_data and m_info and m_data.group(1) == m_info.group(1):
        return m_data.group(1)
    a, b = re.fullmatch(r'local_([0-9a-f]+)', data), re.fullmatch(r'local_([0-9a-f]+)', info)
    if a and b and int(a.group(1), 16) == int(b.group(1), 16) + 4:
        return f'&local_{int(a.group(1), 16) + 4:x}'
    return None


# the statement separators are `\s*`, not a plain newline: a deeply indented body wraps mid-statement and the
# unwrap pass can leave the `;` on its own line (`>> 0x10)\n    ;`)
# (the low byte may print as a literal `u0 = 0;` when the object's address is known 0 mod 256 -- Will's
# Guildmaster 0x00D5E0C0 `uVar18 = 0; uVar19 = (undefined1)((uint)aCStack_200 >> 8); ...`: the split's
# object is then the one the `>> 8` line names; unmatched, the acquire kept an earlier resource, 2026-09-20)
RE_BYTE_SPLIT = re.compile(
    r'^[ \t]*(\w+) = (?:SUB41\(\s*(&?[\w.]+)\s*,\s*0\s*\)|0x0|0)\s*;\s*'
    r'(\w+) = \(undefined1\)\(\(uint\)(&?[\w.]+) >> 8\)\s*;\s*'
    r'(\w+) = \(undefined1\)\(\(uint\)\4 >> 0x10\)\s*;\s*'
    r'(\w+) = \(undefined1\)\(\(uint\)\4 >> 0x18\)\s*;[ \t]*\r?\n', re.M)


def _split_groups(m):
    '''(u0, x, u1, u2, u3) of a RE_BYTE_SPLIT match, or None when a SUB41 object disagrees with the shifts'.'''
    u0, x0, u1, x, u2, u3 = m.groups()
    if x0 is not None and x0 != x:
        return None
    return u0, x, u1, u2, u3

# the reassembly, tolerant of the spaces the unwrap pass leaves where Ghidra wrapped the expression
RE_BYTE_CONCAT_USE = re.compile(
    r'(?:\(void \*\)\s*)?CONCAT13\(\s*(\w+)\s*,\s*CONCAT12\(\s*(\w+)\s*,'
    r'\s*CONCAT11\(\s*(\w+)\s*,\s*(\w+)\s*\)\s*\)\s*\)')


def fold_byte_literal_words(text):
    '''A dword argument Ghidra assembled from four byte locals each holding a literal
    (`u0 = 1; u1 = 0; u2 = 0; u3 = 0; ... CONCAT13(u3,CONCAT12(u2,CONCAT11(u1,u0)))`) is that literal.'''
    def repl(m):
        u3, u2, u1, u0 = m.groups()
        start = m.start()
        value = 0
        for k, name in enumerate((u0, u1, u2, u3)):
            hits = list(re.finditer(r'^[ \t]*' + re.escape(name) + r' = (0x[0-9a-f]+|\d+);[ \t]*\r?\n', text[:start], re.M))
            if not hits:
                return m.group(0)
            value |= (int(hits[-1].group(1), 0) & 0xff) << (8 * k)
        return hex(value) if value > 9 else str(value)
    # the same spelling tolerance as the pointer-split fold: the unwrap leaves a space where Ghidra wrapped
    # the expression (`CONCAT11( uVar17,uVar16)`), and a `(void *)` cast on the word goes with it
    return RE_BYTE_CONCAT_USE.sub(repl, text)


def fold_byte_split_pointers(text):
    '''A pointer pushed byte-wise (Ghidra: `u0 = SUB41(X,0); u1 = (undefined1)((uint)X >> 8); ...` then
    `CONCAT13(u3,CONCAT12(u2,CONCAT11(u1,u0)))`) is X at every use that split reaches.

    One scan in text order, keyed on the split's own object: the byte temporaries are the same four registers for
    every acquire in a function, so a reassembly belongs to the *last* split before it (one split feeds every retry
    of its own loop; the next split re-keys the registers to its object).'''
    events = [(m.start(), 0, m) for m in RE_BYTE_SPLIT.finditer(text)]
    events += [(m.start(), 1, m) for m in RE_BYTE_CONCAT_USE.finditer(text)]
    events.sort(key=lambda e: e[:2])
    source, out, pos = {}, [], 0
    for start, kind, m in events:
        if start < pos:
            continue
        out.append(text[pos:start])
        pos = m.end()
        if kind == 0:
            g = _split_groups(m)
            if g is None:
                out.append(m.group(0))
                continue
            u0, x, u1, u2, u3 = g
            source[(u3, u2, u1, u0)] = (x, m.end())
            out.append(m.group(0))          # kept here; the dead-split pass below drops it once its uses are gone
        else:
            x, split_end = source.get(m.groups(), (None, None))
            # the four registers re-keyed to literals since the split (`u0 = 0; u1 = 0; u2 = 0; u3 = 0;` for a
            # by-value zero: Will's Guildmaster `DeactivateQuestLater(name, 0)` after CreateCreature's split)
            # are a literal word, not this split's pointer: fold_byte_literal_words owns that use
            if x is not None and all(re.search(r'^[ \t]*' + re.escape(u) + r' = (?:0x[0-9a-f]+|\d+);', text[split_end:start], re.M)
                                     for u in m.groups()):
                x = None
            out.append(x if x is not None else m.group(0))
    out.append(text[pos:])
    text = ''.join(out)

    # a split whose reassemblies were all rewritten is dead. The four byte registers are reused elsewhere in the
    # function (Ghidra zero-fills a stack thing through them), so deadness is local: from the end of this block,
    # each name must be re-defined before it is read again.
    def redefined_before_read(name, rest):
        nxt = re.search(r'\b' + re.escape(name) + r'\b', rest)
        if nxt is None:
            return True
        line = rest[rest.rfind('\n', 0, nxt.start()) + 1:rest.find('\n', nxt.start())]
        return re.match(r'^[ \t]*' + re.escape(name) + r'\s*=[^=]', line) is not None

    def dead_split(m):
        g = _split_groups(m)
        if g is None:
            return m.group(0)
        u0, _x, u1, u2, u3 = g
        rest = text[m.end():]
        return '' if all(redefined_before_read(u, rest) for u in (u0, u1, u2, u3)) else m.group(0)
    text = RE_BYTE_SPLIT.sub(dead_split, text)
    return text


def reconcile_destructor_kinds(text):
    """The resource and movie destructors share one bsim label; when the text-order pairing could not settle it, the
    object's own construction decides: a `RESOURCE_NewResource()` / `TryAcquire` object is released, a
    `RESOURCE_StartMovie` object destroyed."""
    resources = set(re.findall(r'\b(\w+) = RESOURCE_NewResource\(\)', text)) | set(re.findall(r'RESOURCE_TryAcquire\((\w+),', text))
    movies = set(re.findall(r'\b(\w+) = RESOURCE_StartMovie\(', text))
    text = re.sub(r'RESOURCE_DestroyMovie\((\w+)\)', lambda m: f'RESOURCE_ReleaseResource({m.group(1)})' if m.group(1) in resources - movies else m.group(0), text)
    text = re.sub(r'RESOURCE_ReleaseResource\((\w+)\)', lambda m: f'RESOURCE_DestroyMovie({m.group(1)})' if m.group(1) in movies - resources else m.group(0), text)
    return text


def fold_actor_maps(text, resolve_string=None):
    # a map key whose ctor and operator[] use the slot restoration named 4 bytes apart (the export's per-site drift:
    # `CCharString((CCharString *)xStack_19c, "WHISPER")` .. `operator[](&map, (CCharString *)xStack_1a0)` .. dtor
    # `(xStack_19c)`, the Will Guildmaster 0x00D5E0C0): the nearest literal string built above, with no other string
    # ctor between, is the key -- spell the use by the ctor's name so the store folds pair them
    def rekey(m):
        head = text[max(0, m.start() - 1200):m.start()]
        ctors = list(re.finditer(r'^[ \t]*CCharString::CCharString\(\(CCharString \*\)&?(\w*Stack_[0-9a-f]+)(?:_\d+)?,"[^"]*",-1\);', head, re.M))
        if not ctors:
            return m.group(0)
        ctor, use = ctors[-1].group(1), m.group(2)
        try:
            delta = abs(int(ctor.rsplit('_', 1)[1], 16) - int(use.rsplit('_', 1)[1], 16))
        except ValueError:
            return m.group(0)
        if ctor != use and delta == 4 and not re.search(r'(?<![\w])' + re.escape(use) + r'\b', head[ctors[-1].end():]):
            return m.group(1) + m.group(0)[len(m.group(1)):].replace(use, ctor)
        return m.group(0)
    text = re.sub(r'(^[ \t]*\w+ = std::\s*map<CCharString,CCountedPointer<[^;]*?::operator\[\]\([^;]*?,)(?:\(CCharString \*\))?&?(\w*Stack_[0-9a-f]+)\);', rekey, text, flags=re.M)

    def keyval(v):
        if v.startswith('&DAT_') and resolve_string:
            literal = resolve_string(int(v[5:], 16))
            if literal is not None:
                return '"' + literal + '"'
        return v
    text = RE_MAP_NEW.sub(lambda m: f'{m.group("ind")}{m.group("map")} = ACTORMAP_New();\n', text)

    def set_repl(m):
        thing = _resolve_local_thing(m.group('body'), m.group('data'), m.group('info'))
        if thing is None:
            return m.group(0)
        return f'{m.group("ind")}ACTORMAP_Set({m.group("map")}, {keyval(m.group("keyval"))}, (CScriptThing *){thing});\n'
    text = RE_MAP_SET.sub(set_repl, text)
    text = RE_MAP_NEW2.sub(r'\1\2 = ACTORMAP_New();', text)

    def set2_repl(m):
        src = m.group('src')
        if m.group('alias') and src == m.group('alias'):
            src = m.group('thing') if m.group('thing').startswith(('&', 'RESLIST_At(')) else '&' + m.group('thing')
        return f'{m.group("ind")}ACTORMAP_Set({m.group("map")}, {keyval(m.group("keyval"))}, {src});\n'
    text = RE_MAP_SET2.sub(set2_repl, text)
    text = RE_MAP_SET3.sub(lambda m: f'{m.group("ind")}ACTORMAP_Set({m.group("map")}, {keyval(m.group("keyval"))}, &{m.group("src")});\n', text)
    text = RE_MAP_RUN.sub(lambda m: f'{m.group("ind")}RESOURCE_RunMacro({keyval(m.group("keyval"))}, {m.group("map")}, {m.group("setup")}, {m.group("skip")});\n', text)
    text = RE_MAP_DESTROY.sub(r'\1ACTORMAP_Destroy(\2);', text)
    text = RE_MAP_RUN_STRINGS.sub(lambda m: f'{m.group("ind")}RESOURCE_RunMacroWithStrings({_strip_addr(m.group("key"))}, {m.group("map").split(".field")[0]}, {m.group("strings")}, {m.group("setup")}, {m.group("skip")});', text)
    text = RE_MAP_RUN_VAR.sub(lambda m: f'{m.group("ind")}RESOURCE_RunMacro({_strip_addr(m.group("key"))}, {m.group("map").split(".field")[0]}, {m.group("setup")}, {m.group("skip")});', text)
    return text


# ---- retail resource objects (controlled entities, movies) ----------------------------------------
# Addresses proven by FSE's own resource implementation (FableAPI.cpp / LuaRetailResources.h).
RESOURCE_CTOR = {0x7E72A0}            # CScriptGameResourceObjectScriptedThingBase::ctor (bsim: CCarriedReadableDef)
THING_DTOR = {0x4AA840}  # CScriptThing::~CScriptThing (vtable 01238c8c, releases Info)
RESOURCE_DTOR = {0x7E74D0}            # CSGROSTB_Destroy_API
MOVIE_CTOR = {0x6E7B60}              # CScriptGameResourceObjectMovieBase ctor (stores vtable 01260ef4)
THING_CTOR = {0x6E7B40}              # CScriptThing::CScriptThing() (stores vtable 01238c8c, Info/Data = 0; disasm 2026-09-16)
MOVIE_DTOR = {0x6E7B80}               # MovieResource_Destroy_API
MOVIE_BASE_CTOR = {0x6E7A60}          # the movie object's base ctor (CBaseObject_Construct 0x99A380 + vtable 0126008c); the derived
                                      # part is inlined at the call site and already lowers to RESOURCE_StartMovie (disasm 2026-09-19)
COUNTED_ASSIGN = {0x8AB1E0}           # CCountedPointer::operator=(this, const CCountedPointer&): out-of-line refcount dance (disasm 2026-09-19)
NODE_SELF = {0x99A3B0}                # `mov eax, ecx; ret 4`: returns its receiver, no effect (bsim: CFourierAnalysis::CFourierAnalysis)
COUNTED_RELEASE = {0x6E7AB0, 0xCE1000}   # CCountedPointer release: decref [this+4], zero [this], [this+4] (disasm 2026-09-16)
BASE_OBJECT_DTOR = {0x99A430}         # CBaseIntelligentPointer::~ (bsim: CPhysicsMeshInfo::~CPhysicsMeshInfo)
RESOURCE_ACQUIRED = {0xCD23B9}        # bool __thiscall (this): [this+8] != 0, the resource's counted handle (disasm 2026-09-16)
# void __thiscall (this): drops the counted handle (bsim: CMemoryDataOutputStream::Clear) = FSE InitScriptObjectHelper2.
# Retail scripts run `if (IsAcquired(&res)) Reset(&res);` before every StartScriptingEntity -- releasing the current
# CScriptGameResourceObjectScriptedThing, whose destructor (0x903AC0) is the only ClearLocked on the thing's
# CTCScriptedControl (+0x18). PreMeleeMaze 0x00D43DB0 does it BEFORE waiting out GuildWarningOccuring, which is
# what lets PreMelee's equal-priority acquire steal the Maze (StartScriptingEntity 0x89B5B0 yields while Locked).
RESOURCE_RESET = {0xCD2770}
RESOURCE_SCRIPT_THING = {0x7E7490}    # CScriptThing __thiscall GetScriptThing(this) via hidden pointer (empty when unacquired)
# std::map<CCharString,CCharString> (cutscene string inputs): ctor 0x9AC2D0 (bsim: Std_Deque_Construct, allocates the
# 0x18-byte head node), dtor 0x9AC310 (bsim: LTextTreeWalkThrough::Dtor), operator[] 0x9AC700 (disasm 2026-09-17)
STRINGMAP_CTOR = {0x9AC2D0}
# CTimer on the stack: ctor 0xCD4450 stores GSI->RegisterTimer() (slot 0x15c) in [this]; dtor 0xCD4470 calls
# GSI->DeregisterTimer([this]) (slot 0x160) (disasm 2026-09-17). The object is its 4-byte timer id.
TIMER_CTOR = {0xCD4450}
TIMER_DTOR = {0xCD4470}
STRINGMAP_DTOR = {0x9AC310}
STRINGMAP_INDEX = {0x9AC700}


def _thing_source(arg):
    """The source operand of a CScriptThing assignment: `(int)&local_20` / `(CScriptThing *)&X` is the stack
    thing X itself (the copy takes the object, not its address); `(int)pCVar6` is the handle."""
    arg = re.sub(r'^\((?:int|CScriptThing \*)\)', '', arg.strip()).strip()
    return arg[1:] if arg.startswith('&') else arg


def _strip_addr(arg):
    arg = arg.strip()
    arg = re.sub(r'^\((?:[\w :*]+\*|int|uint|undefined4)\)', '', arg).strip()
    return arg[1:] if arg.startswith('&') else arg


# vtable pointers stored by inlined constructors (FSE: g_pCScriptGameResourceObjectScriptedThingBaseVTable,
# g_pMovieObjectVTable, g_pCScriptThingVTable)
RE_INLINE_CTOR = re.compile(
    r'^(?P<ind>[ \t]*)(?:\*\(undefined \*\*\*\))?(?P<obj>&?\w+)(?:\[0\]|\._0_4_)? = &PTR_[A-Za-z_]*_(?P<vt>0127094c|01260ef4|01238c8c);[ \t]*\r?\n'
    r'(?:[ \t]*(?:\w+ = 0;|\w+ = \(\w+ \*\)0x0;|\w+(?:\[\d\]|\._\d+_4_) = (?:\(\w+ \*\))?0(?:x0)?;|\*\(\w+ \*\)\(\w+ \+ (?:4|8|0x8)\) = 0;)[ \t]*\r?\n){0,3}', re.M)


# the member zero-stores in front of the vtable reset may be spelled under a sibling slot's name (Ghidra splits
# the object) and with either `0` or `0x0`, so they are matched name-agnostically; the object is the vtable line's
RE_INLINE_DTOR = re.compile(
    r'^(?P<ind>[ \t]*)(?:\w+(?:\[\d\]|\._\d+_4_)? = (?:\([\w ()\[\]]+\**\))?0(?:x0)?;[ \t]*\r?\n[ \t]*){0,3}'
    r'(?P<obj>\w+)(?:\[0\]|\._0_4_)? = &PTR_[A-Za-z_]*_(?P<vt>0127094c|01260ef4|0126008c);[ \t]*\r?\n'
    r'[ \t]*[\w:~]+\s*\(\(\w+ \*\)(?P=obj)\);[ \t]*\r?\n', re.M)


# the same destructor on the path where the compiler had no vtable to reset (the object is already the base):
# only the member zero-stores remain in front of the base destructor call
RE_INLINE_DTOR_NO_VTABLE = re.compile(
    r'^(?P<ind>[ \t]*)(?P<obj>\w+)(?:\[\d\]|\._\d+_4_)? = (?:\([\w ]+\**\))?0(?:x0)?;[ \t]*\r?\n'
    r'(?:[ \t]*(?P=obj)(?:\[\d\]|\._\d+_4_)? = (?:\([\w ]+\**\))?0(?:x0)?;[ \t]*\r?\n){0,2}'
    r'[ \t]*[\w:]*~\w+\s*\(\([\w ]+\*\)(?P=obj)\);[ \t]*\r?\n', re.M)


def fold_inline_destructors(text):
    """The inlined resource / movie destructor after the counted release: the handle field is zeroed,
    the vtable reset to the base and the base destructor called. That is `ReleaseResource` (resource
    vtable 0127094c) or `DestroyMovie` (movie vtables) on the object."""
    # the base vtable (0126008c) is shared by resources and movies: the object's own construction decides
    resources = set(re.findall(r'\b(\w+) = RESOURCE_NewResource\(\)', text)) | set(re.findall(r'RESOURCE_TryAcquire\((\w+),', text))
    def repl(m):
        release = m.group('vt') == '0127094c' or (m.group('vt') == '0126008c' and m.group('obj') in resources)
        return f'{m.group("ind")}{"RESOURCE_ReleaseResource" if release else "RESOURCE_DestroyMovie"}({m.group("obj")});\n'
    text = RE_INLINE_DTOR.sub(repl, text)
    # without the vtable line the shape alone is not evidence: fold only an object this function acquired,
    # otherwise the zero-stores would be read as `object = 0` and every later use of it lifts to nil
    text = RE_INLINE_DTOR_NO_VTABLE.sub(
        lambda m: f'{m.group("ind")}RESOURCE_ReleaseResource({m.group("obj")});\n' if m.group('obj') in resources else m.group(0), text)
    return text


def fold_inline_constructors(text):
    def repl(m):
        obj = m.group('obj').lstrip('&')
        vt = m.group('vt')
        if vt == '0127094c':
            return f'{m.group("ind")}{obj} = RESOURCE_NewResource();\n'
        if vt == '01260ef4':
            return f'{m.group("ind")}{obj} = RESOURCE_StartMovie("");\n'
        return f'{m.group("ind")}{obj} = QUESTTHING_Empty();\n'
    return RE_INLINE_CTOR.sub(repl, text)


RE_ALIAS_BEFORE_CTOR = re.compile(
    r'^(?P<ind>[ \t]*)(?P<p>\w+) = (?P<obj>\w+);[ \t]*\r?\n'
    r'(?P<between>(?:[ \t]*\w+ = [^;\n]+;[ \t]*\r?\n){0,2}?)'
    r'(?P<ctor>[ \t]*(?P=obj) = (?:RESOURCE_NewResource|RESOURCE_StartMovie|ACTORMAP_New|STRINGMAP_New|QUESTTHING_Empty)\([^;\n]*\);[ \t]*\r?\n)', re.M)


RE_NOOP_COMMA_ASSIGN = re.compile(r'\((?P<neg>!?)(?P<x>\w+)\) (?P<op>\|\||&&) \((?P=x) = (?P<v>true|false), ')


def drop_noop_comma_assignments(text):
    """`if ((!bVar4) || (bVar4 = true, COND))`: the comma assignment runs only when the short-circuit left side
    fell through, i.e. when the flag already holds that value (the compiler re-materialised a register). Dropping
    it leaves an ordinary condition for the lifter instead of an argument sequence."""
    def repl(m):
        known = (m.group('op') == '||') == (m.group('neg') == '!')
        if (m.group('v') == 'true') != known:
            return m.group(0)
        return f"({m.group('neg')}{m.group('x')}) {m.group('op')} ("
    return RE_NOOP_COMMA_ASSIGN.sub(repl, text)


def hoist_object_aliases(text):
    """Ghidra hoists a call's register set-up above the object's inlined construction
    (`pScriptObject = local_10; ePriority = 4; local_10[0] = &PTR_vtable`): once the vtable store became
    `local_10 = RESOURCE_NewResource()`, the pointer alias reads the object before it exists. Move the alias
    below the construction so the lifter sees a plain copy of a live object."""
    def repl(m):
        between = m.group('between')
        if re.search(r'\b(?:' + re.escape(m.group('p')) + '|' + re.escape(m.group('obj')) + r')\b', between):
            return m.group(0)
        return f"{between}{m.group('ctor')}{m.group('ind')}{m.group('p')} = {m.group('obj')};\n"
    return RE_ALIAS_BEFORE_CTOR.sub(repl, text)


def _vtable_only_body(byte_at, target):
    """`8B C1 C7 00 vt C3` / `C7 01 vt C3`: a ctor/dtor that only installs a vtable."""
    if byte_at is None:
        return False
    b = bytes(byte_at(target + i) for i in range(16))
    if b[:4] == b'\x8b\xc1\xc7\x00' and b[8:15] == b'\xc7\x40\x04\x00\x00\x00\x00' and b[15] == 0xC3:
        return True     # `mov [eax],vt; mov [eax+4],0; ret` (counted-pointer base of a resource)
    return b[:4] == b'\x8b\xc1\xc7\x00' and b[8] == 0xC3 or b[:2] == b'\xc7\x01' and b[6] == 0xC3


def drop_trivial_base_calls(text, call_labels, byte_at):
    for label, target in call_labels.items():
        if _vtable_only_body(byte_at, target):
            text = re.sub(r'^[ \t]*' + re.escape(label) + r'\s*\([^;]*\);[ \t]*\r?\n', '', text, flags=re.M)
    return text


MAP_CTOR_LABEL = 'StdMap_Construct_API'


def name_offset_objects(text, call_labels):
    """Ghidra types some stack objects as `auStack_c4 + 4` (a 4-byte prefix it split off). A resource,
    movie, thing or actor-map constructor called on such an address defines an object the folds cannot
    name; give it the identifier `<slot>_p<N>` from the constructor onward so it lowers like a plain
    stack object (canonicalise_stack_objects shifts the slot extent by N)."""
    ctor_labels = [label for label, target in call_labels.items()
                   if target in RESOURCE_CTOR | MOVIE_CTOR | THING_CTOR] + [MAP_CTOR_LABEL]
    if not ctor_labels:
        return text
    pat = re.compile(r'(?:' + '|'.join(re.escape(l) for l in ctor_labels) + r')\s*\((?:\(\w+ \*\))?\(?(\w+) \+ (4|8|0xc)\)?(?=[,)])')
    pos = 0
    while (m := pat.search(text, pos)):
        slot, off = m.group(1), m.group(2)
        name = f'{slot}_p{off.replace("0x", "")}'
        head, tail = text[:m.start()], text[m.start():]
        expr = re.escape(slot) + r' \+ ' + re.escape(off)
        # `(cast)(slot + N)` / `= (slot + N)` lose their parentheses; `f(slot + N)` keeps the call's own
        tail = re.sub(r'(?<![\w])\(' + expr + r'\)|(?<![\w])' + expr + r'(?![\w])', name, tail)
        text = head + tail
        pos = m.start() + 1
    return text


def fold_resource_objects(text, call_labels):
    """call_labels: {label text as printed in the decompile: target address}. Rewrites constructor /
    destructor calls of resource and movie objects into the retail-resource pseudo API."""
    for label, target in call_labels.items():
        if target in RESOURCE_CTOR:
            text = re.sub(r'^([ \t]*)' + re.escape(label) + r'\s*\(([^;]+?)\);', lambda m: f'{m.group(1)}{_strip_addr(m.group(2))} = RESOURCE_NewResource();', text, flags=re.M)
        elif target in THING_DTOR:
            text = re.sub(r'^[ \t]*' + re.escape(label) + r'\s*\((?:\(\w+ \*\))?&?(\w+)[^;]*\);[ \t]*\r?\n', '', text, flags=re.M)
        elif target in RESOURCE_DTOR:
            text = re.sub(r'^([ \t]*)' + re.escape(label) + r'\s*\(([^;]+?)\);', lambda m: f'{m.group(1)}RESOURCE_ReleaseResource({_strip_addr(m.group(2))});', text, flags=re.M)
        elif target in THING_CTOR:
            text = re.sub(r'^([ \t]*)' + re.escape(label) + r'\s*\(([^;,]+?)(?:,[^;]*)?\);', lambda m: f'{m.group(1)}{_strip_addr(m.group(2))} = QUESTTHING_Empty();', text, flags=re.M)
        elif target in MOVIE_CTOR:
            text = re.sub(r'^([ \t]*)' + re.escape(label) + r'\s*\(([^;,]+?)(?:,[^;]*)?\);', lambda m: f'{m.group(1)}{_strip_addr(m.group(2))} = RESOURCE_StartMovie("");', text, flags=re.M)
        elif target in MOVIE_DTOR:
            text = re.sub(r'^([ \t]*)' + re.escape(label) + r'\s*\(([^;,]+?)(?:,[^;]*)?\);', lambda m: f'{m.group(1)}RESOURCE_DestroyMovie({_strip_addr(m.group(2))});', text, flags=re.M)
        elif target in MOVIE_BASE_CTOR | NODE_SELF:
            text = re.sub(r'^[ \t]*' + re.escape(label).replace('::', r'::\s*') + r'\s*\([^;]*\);[ \t]*\r?\n', '', text, flags=re.M)
        elif target in COUNTED_RELEASE:
            # `release(X); X[0] = &PTR_<movie vtable>; base_dtor(X);` is the inlined movie destructor
            # Ghidra may spell the three member accesses (+8, +0, +0) under different slot names: the base
            # destructor's operand (a call operand the export restored) names the object
            # (the released counted pointer may be zeroed between the release and the vtable store)
            text = re.sub(r'^([ \t]*)' + re.escape(label).replace('::', r'::\s*') + r'\s*\(([^;,]+?)\);[ \t]*\r?\n'
                          r'(?:[ \t]*\w+(?:\[\d\]|\._\d_4_)? = (?:\(undefined \*\*\))?0(?:x0)?;[ \t]*\r?\n)?'
                          r'[ \t]*\w+(?:\[0\]|\._0_4_)? = &PTR_[A-Za-z_]*_0126008c;[ \t]*\r?\n'
                          r'[ \t]*[\w:~]+\s*\(\(\w+ \*\)&?(\w+)\);[ \t]*\r?\n',
                          lambda m: f'{m.group(1)}RESOURCE_DestroyMovie({m.group(3)});\n', text, flags=re.M)
        elif target in RESOURCE_SCRIPT_THING:
            # `X = (CScriptThing *)Res::GetScriptThing((Res *)&RES,(int)&HIDDEN);` -> the hidden slot is the thing
            def script_thing(m):
                res, hidden = _strip_addr(m.group(3)), _strip_addr(m.group(4))
                out = f'{m.group(1)}{hidden} = RESOURCE_ScriptThing({res});'
                if m.group(2):
                    out += f'\n{m.group(1)}{m.group(2)} = {hidden};'
                return out
            text = re.sub(r'^([ \t]*)(?:(\w+) = (?:\((?:CScriptThing|void) \*\)\s*)?)?' + re.escape(label).replace('::', r'\s*::\s*') + r'\s*\(([^;,]+?),\s*([^;]+?)\);',
                          script_thing, text, flags=re.M)
            # the same call printed with the resource only, its result in the return register (the Arena's
            # `pCVar6 = (CScriptThing *) Res::GetScriptThing((Res *)&xStack_238);`, 81 sites, 2026-09-26)
            text = re.sub(r'^([ \t]*)(\w+) = (?:\((?:CScriptThing|void) \*\)\s*)?' + re.escape(label).replace('::', r'\s*::\s*') + r'\s*\(([^;,()]+?|\([^;,()]+\)\s*&?\w+)\);',
                          lambda m: f'{m.group(1)}{m.group(2)} = RESOURCE_ScriptThing({_strip_addr(m.group(3))});', text, flags=re.M)
        elif target in TIMER_CTOR:
            text = re.sub(r'^([ \t]*)' + re.escape(label).replace('::', r'\s*::\s*') + r'\s*\(([^;]+?)\);', lambda m: f'{m.group(1)}{_strip_addr(m.group(2))} = GSI->RegisterTimer();', text, flags=re.M)
        elif target in TIMER_DTOR:
            text = re.sub(r'^([ \t]*)' + re.escape(label).replace('::', r'\s*::\s*') + r'\s*\(([^;]+?)\);', lambda m: f'{m.group(1)}GSI->DeregisterTimer({_strip_addr(m.group(2))});', text, flags=re.M)
        elif target in STRINGMAP_CTOR:
            text = re.sub(r'^([ \t]*)' + re.escape(label) + r'\s*\(([^;]+?)\);', lambda m: f'{m.group(1)}{_strip_addr(m.group(2))} = STRINGMAP_New();', text, flags=re.M)
        elif target in STRINGMAP_DTOR:
            text = re.sub(r'^([ \t]*)' + re.escape(label).replace('::', r'\s*::\s*') + r'\s*\(([^;]+?)\);', lambda m: f'{m.group(1)}STRINGMAP_Destroy({_strip_addr(m.group(2))});', text, flags=re.M)
        elif target in STRINGMAP_INDEX:
            # `N = map::operator[](M,&key); CCharString::operator=((CCharString *)N,value);` -> STRINGMAP_Set(M, key, value)
            # (the map operand may carry its own cast: `((map<..> *)&iStack_1d0, aCStack_118)`, the Skill grade text 0x00D5AE70)
            pat = re.compile(r'^([ \t]*)(\w+) = ' + re.escape(label).replace('::', r'\s*::\s*') + r'\s*\((?:\([^()]*\*\))?&?(\w+),\s*(?:\(CCharString \*\))?&?(\w+)\);[ \t]*\r?\n'
                             r'[ \t]*CCharString::operator=\(\(CCharString \*\)\2,\s*([^;]+?)(?:,\s*-1)?\);', re.M)
            text = pat.sub(lambda m: f'{m.group(1)}STRINGMAP_Set({m.group(3)}, {m.group(4)}, {m.group(5).strip()});', text)
        elif target in RESOURCE_ACQUIRED:
            # (the operand carries a cast in parentheses: `HasPhysicsMesh((C3DMeshInfo *)appuStack_30)`)
            text = re.sub(re.escape(label) + r'\s*\(((?:\([^()]*\)|[^;,()])+)\)', lambda m: f'RESOURCE_IsAcquired({_strip_addr(m.group(1))})', text)
        elif target in RESOURCE_RESET:
            text = re.sub(r'^([ \t]*)' + re.escape(label).replace('::', r'\s*::\s*') + r'\s*\(([^;,]+?)\);', lambda m: f'{m.group(1)}RESOURCE_Reset({_strip_addr(m.group(2))});', text, flags=re.M)
    # `b = IsAcquired(&R); if (b) { Reset(R); }` (or the inline test) is the runtime PrepareResource: release the
    # held handle if any. A bare IsAcquired test with no reset stays and lowers to `false` later.
    text = re.sub(r'^([ \t]*)(\w+) = RESOURCE_IsAcquired\((\w+)\);[ \t]*\r?\n[ \t]*if \(\2\) \{[ \t]*\r?\n[ \t]*RESOURCE_Reset\(\3\);[ \t]*\r?\n[ \t]*\}[ \t]*\r?\n',
                  lambda m: f'{m.group(1)}RESOURCE_PrepareResource({m.group(3)});\n', text, flags=re.M)
    text = re.sub(r'^([ \t]*)if \(RESOURCE_IsAcquired\((\w+)\)\) \{[ \t]*\r?\n[ \t]*RESOURCE_Reset\(\2\);[ \t]*\r?\n[ \t]*\}[ \t]*\r?\n',
                  lambda m: f'{m.group(1)}RESOURCE_PrepareResource({m.group(2)});\n', text, flags=re.M)
    text = re.sub(r'^([ \t]*)RESOURCE_Reset\((\w+)\);', lambda m: f'{m.group(1)}RESOURCE_PrepareResource({m.group(2)});', text, flags=re.M)
    return text


SQUARED_DISTANCE = {0xCBE512}   # float __fastcall GetSquaredDistanceBetweenThings(a, b)
STRING_CONCAT = {0x99F570, 0x99F600, 0x99F690}   # CCharString operator+ (dest, a, b) -> dest; disassembly 2026-09-16; 0x99F690 = FSE CCharString_OperatorPlus_API (const char* left)
INT_TO_STRING = {0x99F830}      # CCharString* __fastcall GFIntToCharString(CCharString* result, int value) (FSE FableAPI.h)
STRING_C_STR = {0x99E4C0}       # const char* __thiscall CCharString::operator const char*() (FSE CCharString_ToConstChar_API)
STRING_NOT_EQUAL = {0x99E960}   # bool CCharString::NotEqual(const CCharString&, const char*) (bsim label; callers test the result against 0)
DISTANCE_PREDICATES = {0xCBE2FF: 'IsDistanceBetweenThingsUnder', 0xCBE3EA: 'IsDistanceBetweenThingsOver'}   # bool __fastcall (a, b, float)
# void __fastcall AddLogbookStoryEntry(int n) (bsim label: CSubtitleRenderer::SetText): builds
# "TEXT_QST_LOG_STORY_<n>_NAME"/"_DESC" and calls GSI slot 0x4d0 AddLogBookEntry (disassembly 0xCBE87F,
# 2026-09-16). ForgeFSE binds the same address as quest:AddLogbookStoryEntry(int) (FableAPI.cpp).
STORY_LOGBOOK = {0xCBE87F}
# the two key-taking siblings (same bsim label): `__thiscall(const CCharString* key)`, disassembly 2026-09-19 —
# 0xCBE960 builds "<key>_NAME"/"_DESC" and submits category 1 (FSE AddLogbookStoryEntryString_Func; the Lua
# `AddLogbookStoryEntry` overload takes the string), 0xCBE9EE builds "<key>_TITLE", category 2, then yields
# (FSE AddLogbookTutorialEntry_Func).
# 0xCBEA81 is the PC sibling of 0xCBE9EE (disassembly 2026-09-19): title <key>_TITLE, body <key>_PC (0x122e248),
# category 2, the same trailing yield; PreMelee's Guildmaster uses it on the !IsXbox path (TEXT_QST_LOG_COMBAT_*).
KEYED_LOGBOOK = {0xCBE960: 'AddLogbookStoryEntry', 0xCBE9EE: 'AddLogbookTutorialEntry', 0xCBEA81: 'AddLogbookTutorialEntryPC'}


def _strip_ptr_cast(operand):
    """`(int *)pcVar17` / `&CStack_128` operands of the string helpers: the register / slot itself."""
    return re.sub(r'^\((?:int|char|void|CCharString|undefined4?)(?: \*)?\)(?=&?\w+$)', '', operand.strip()).lstrip('&')   # a cast on a member expression stays


def fold_engine_helpers(text, call_labels):
    """Engine helpers with a direct FSE Lua equivalent. Squared distance becomes the FSE distance
    squared (the pseudo call keeps the comparison semantics; finish_lua expands it)."""
    for label, target in call_labels.items():
        if target in SQUARED_DISTANCE:
            text = re.sub(re.escape(label) + r'\s*\(([^,;]+),([^;)]+)\)',
                          lambda m: f'ENGINE_SquaredDistance({_strip_addr(m.group(1))}, {m.group(2).strip()})', text)
        elif target in STRING_CONCAT:
            text = re.sub(r'(?:\(\w+ \*\))?' + re.escape(label) + r'\s*\((?:\([\w ]+\*\))?&?[\w.]+,\s*((?:\([\w ]+\*\))?(?:[^,;()]|\([^()]*\))+),\s*((?:\([\w ]+\*\))?(?:[^;()]|\([^()]*\))+)\)',
                          lambda m: f'ENGINE_Concat({_strip_ptr_cast(m.group(1))}, {_strip_ptr_cast(m.group(2))})', text)
        elif target in INT_TO_STRING:
            text = re.sub(r'(?:\(\w+ \*\))?' + re.escape(label) + r'\s*\((?:\([\w ]+\*\))?&?[\w.]+,\s*([^;)]+)\)',
                          lambda m: f'ENGINE_IntToString({m.group(1).strip()})', text)
        elif target in STRING_C_STR:
            text = re.sub(r'(?:\(\w+ \*\))?' + re.escape(label) + r'\s*\((?:\([\w ]+\*\))?&?([\w.]+)\)', lambda m: m.group(1), text)
        elif target in STRING_NOT_EQUAL:
            text = re.sub(r'(?:\(\w+ \*\))?' + re.escape(label) + r'\s*\(((?:\([\w ]+\*?\))?[^,;()]+),\s*((?:\([\w ]+\*?\))?[^;()]+)\)',
                          lambda m: f'ENGINE_StrNotEqual({_strip_ptr_cast(m.group(1))}, {_strip_ptr_cast(m.group(2))})', text)
        elif target in STORY_LOGBOOK:
            text = re.sub(re.escape(label) + r'\s*\((0x[0-9a-f]+|\d+)\)', lambda m: f'GSI->AddLogbookStoryEntry({int(m.group(1), 0)})', text)
            text = re.sub(re.escape(label) + r'\s*\(([A-Za-z]\w*)\)', lambda m: f'GSI->AddLogbookStoryEntry({m.group(1)})', text)   # the id chosen by a branch (`iVar8 = 0xaf / 0xaa`)
        elif target in KEYED_LOGBOOK:
            # `__thiscall(CCharString* key)`: the receiver is the key; Ghidra appends a stale register operand
            text = re.sub(re.escape(label) + r'\s*\((?:\([\w :*]+\))?&?([\w.]+)(?:,\s*(?:\([^()]*\)|[^;()])*)?\)',
                          lambda m, name=KEYED_LOGBOOK[target]: f'GSI->{name}(&{m.group(1)})', text)
        elif target in DISTANCE_PREDICATES:
            name = DISTANCE_PREDICATES[target]
            text = re.sub(re.escape(label) + r'\s*\(([^,;]+),\s*([^,;]+),\s*([^;)]+)\)',
                          lambda m, name=name: f'ENGINE_{name}({_strip_addr(m.group(1))}, {_strip_addr(m.group(2))}, {m.group(3).strip()})', text)
    return text


RE_INLINE_STRNCMP = re.compile(
    r'^(?P<ind>[ \t]*)(?P<n>\w+) = (?:0x[0-9a-f]+|\d+);\s*\r?\n[ \t]*(?P<b>\w+) = true;\s*\r?\n'
    r'[ \t]*(?P<p1>\w+) = (?P<l1>"[^"]*");\s*\r?\n[ \t]*(?P<p2>\w+) = (?P<l2>"[^"]*");\s*\r?\n'
    r'[ \t]*do \{\s*\r?\n(?P<hoist>[ \t]*\w+ = \w+;\s*\r?\n)?[ \t]*if \((?P=n) == 0\) break;\s*\r?\n[ \t]*(?P=n) = (?P=n) \+ -1;\s*\r?\n'
    r'[ \t]*(?P=b) = \*(?P=p1) == \*(?P=p2);\s*\r?\n[ \t]*(?P=p1) = (?P=p1) \+ 1;\s*\r?\n[ \t]*(?P=p2) = (?P=p2) \+ 1;\s*\r?\n'
    r'[ \t]*\} while \((?:\(bool\))?(?P=b)\);[ \t]*\r?\n', re.M)


def fold_inline_strncmp(text: str) -> str:
    """The compiler inlines `memcmp` of two string literals (a CCharString compare whose buffer is
    null falls back to comparing "" against the literal): the result is a constant."""
    text = RE_INLINE_STRNCMP.sub(lambda m: f'{m.group("ind")}{m.group("hoist").strip() + chr(10) + m.group("ind") if m.group("hoist") else ""}{m.group("b")} = {str(m.group("l1") == m.group("l2")).lower()};\n', text)
    # the constant never takes the branch that follows it
    text = re.sub(r'^([ \t]*)(\w+) = false;[ \t]*\r?\n[ \t]*if \(\2\) goto \w+;[ \t]*\r?\n', r'\1\2 = false;\n', text, flags=re.M)
    return text


def fold_name_compare(text):
    """`p = me->GetName(); if (*p == 0) ...; CBasicString<char>::Compare(**p, "S")` -> Lua string ops."""
    text = re.sub(r'\(undefined4 \*\)\*(\w+) (==|!=) \(undefined4 \*\)0x0', r'\1 \2 (CCharString *)0x0', text)
    text = re.sub(r'\*(\w+) (==|!=) \(CCharString(?:_bv)?\)0x0', r'\1 \2 (CCharString *)0x0', text)   # the typed spelling `*pCVar4 == (CCharString)0x0`
    text = re.sub(r'CBasicString<char>::Compare\(\*\(void \*\*\)\*(\w+),("[^"]*")\)', r'ENGINE_StrCmp(\1, \2)', text)
    # the same on a string object living in a stack slot (`GetDataString` into `xStack_74`): no pointer to it
    text = re.sub(r'\b(\w*Stack_\w+) (==|!=) \(CCharString(?:_bv)?\)0x0', r'\1 \2 (CCharString *)0x0', text)
    text = re.sub(r'CBasicString<char>::Compare\(\*\(void \*\*\)(\w*Stack_\w+),("[^"]*")\)', r'ENGINE_StrCmp(\1, \2)', text)
    return text


# (two spellings: the untyped one -- `(undefined4 *)` reps, `b[1] == a.y` -- and the one the typed export prints once
# GetDataString is a typed thing call -- CCharString reps, `(CCharString)0x0`, `*(int *)((int)b + 4)`, `*(void **)b`)
_NULL = r'(?:\(undefined4 \*\)|\(CCharString\))0x0'
RE_INLINE_STRING_EQUALITY = re.compile(
    r'^(?P<ind>[ \t]*)(?P<p>\w+) = (?:\(int \*\))?(?P<call>[^;\n]+);[ \t]*\r?\n'
    r'[ \t]*(?P<a>\w+) = (?:\(undefined4 \*\))?\*(?P=p);[ \t]*\r?\n'
    r'[ \t]*(?P<b>\w+) = (?P<other>[^;\n]+);[ \t]*\r?\n'
    r'[ \t]*if \((?P=b) == (?P=a)\) \{[ \t]*\r?\n[ \t]*(?P<c>\w+) = \'\\x01\';[ \t]*\r?\n[ \t]*\}[ \t]*\r?\n'
    r'[ \t]*else if \(\((?P=b) == ' + _NULL + r'\) \|\| \((?P=a) == ' + _NULL + r'\)\) \{[ \t]*\r?\n'
    r'[ \t]*(?P=c) = \'\\0\';[ \t]*\r?\n[ \t]*\}[ \t]*\r?\n'
    r'[ \t]*else if \((?:(?P=b)\[1\] == (?P=a)(?:\.y|\[1\])|\*\(int \*\)\(\(int\)(?P=b) \+ 4\) == \*\(int \*\)\(\(int\)(?P=a) \+ 4\))\) \{[ \t]*\r?\n'
    r'[ \t]*(?P<cmp>\w+) = CBasicString<char>::Compare\((?:\(void \*\)\*(?P=b)|\*\(void \*\*\)(?P=b)),'
    r'(?:\(void \*\)(?:\*(?P=a)|(?P=a)\.x)|\*\(void \*\*\)(?P=a))\);[ \t]*\r?\n'
    r'[ \t]*(?P=c) = [^;\n]*(?P=cmp)[^;\n]*;[ \t]*\r?\n[ \t]*\}[ \t]*\r?\n'
    r'[ \t]*else \{[ \t]*\r?\n[ \t]*(?P=c) = \'\\0\';[ \t]*\r?\n[ \t]*\}[ \t]*\r?\n', re.M)


def fold_inline_string_equality(text):
    """`CCharString == CCharString` inlined over the two string reps: same pointer -> true, a null rep -> false,
    same length -> `CBasicString<char>::Compare(..) == 0`, else false. The operands are a string the code just
    produced (`p = (int *)CALL; a = *p`) and another rep (`b = EXPR`): the whole block is `c = ENGINE_StrEq(p, EXPR)`
    (DarkwoodTrader Main: only the trader whose GetDataString() is the quest's TraderToTalk greets the camp
    trader; unfolded, every trader took the greeting branch and none resumed following, 2026-09-24)."""
    def repl(m):
        # the getter stays a statement (the lifter reads `p = (int *)CALL` as `p = me:GetDataString()`)
        return (f"{m.group('ind')}{m.group('p')} = (int *){m.group('call').strip()};\n"
                f"{m.group('ind')}{m.group('c')} = ENGINE_StrEq({m.group('p')}, {m.group('other').strip()});\n")
    out, pos = [], 0
    for m in RE_INLINE_STRING_EQUALITY.finditer(text):
        out.append(text[pos:m.start()])
        out.append(repl(m))
        pos = m.end()
        # the flag's next use tests it as a char (`if (c == '\0')`): the result is a boolean now
        c = re.escape(m.group('c'))
        nxt = re.search(r'\b' + c + r'\b', text[pos:])
        if nxt:
            at = pos + nxt.start()
            test = re.match(c + r" (==|!=) '\\0'", text[at:])
            if test:
                out.append(text[pos:at] + ('!' if test.group(1) == '==' else '') + m.group('c'))
                pos = at + test.end()
    out.append(text[pos:])
    return ''.join(out)


def fold_null_string_branches(text):
    """`operator==(CCharString, literal)` inlined: a null buffer takes an inlined `"" == literal` path (folded to
    `false` by `fold_inline_strncmp`), otherwise `Compare`. The Lua string compare covers both, so the general
    branch alone remains: `if (P == NULL) { V = false; } else { BODY }` and `if (P != NULL) { BODY-ending-in-goto }
    V = false;` -> BODY."""
    def dedent(block, ind):
        return re.sub(r'^' + re.escape(ind) + '  ', ind, block, flags=re.M)
    def r1(m):
        if f'ENGINE_StrCmp({m.group(2)},' not in m.group(3):
            return m.group(0)
        return dedent(m.group(3), m.group(1))
    text = re.sub(r'^([ \t]*)if \((\w+) == \(CCharString \*\)0x0\) \{[ \t]*\r?\n[ \t]*\w+ = false;[ \t]*\r?\n[ \t]*\}[ \t]*\r?\n'
                  r'[ \t]*else \{[ \t]*\r?\n((?:\1  [^\n]*\n)+?)\1\}[ \t]*\r?\n', r1, text, flags=re.M)
    def r2(m):
        body = m.group(3)
        if f'ENGINE_StrCmp({m.group(2)},' not in body or not re.search(r'^[ \t]*(?:goto \w+;|return[^\n]*;)[ \t]*$', body.rstrip('\n').rsplit('\n', 1)[-1]):
            return m.group(0)
        return dedent(body, m.group(1))
    text = re.sub(r'^([ \t]*)if \((\w+) != \(CCharString \*\)0x0\) \{[ \t]*\r?\n((?:\1  [^\n]*\n)+?)\1\}[ \t]*\r?\n'
                  r'[ \t]*\w+ = false;[ \t]*\r?\n', r2, text, flags=re.M)
    return text


STACK_OBJECT_SIZES = {'RESOURCE_NewResource': 16, 'RESOURCE_StartMovie': 16, 'ACTORMAP_New': 12, 'STRINGMAP_New': 12, 'QUESTTHING_Empty': 12}   # std::_Tree: comp/pad, _Myhead, _Mysize
RE_STACK_NAME = re.compile(r'\b([A-Za-z]+Stack_|local_)([0-9a-f]+)\b')


RE_STACK_DECL = re.compile(
    r'^[ \t]*[\w:<>,_ ]+?[ \t*]+(?P<name>(?:[A-Za-z]+Stack_|local_)(?P<off>[0-9a-f]+))[ \t]*(?P<arr>\[\s*\d+\s*\])?;', re.M)


def resolve_stack_offset_names(text: str) -> str:
    """Ghidra spells one stack slot two ways in the same body: by its declared local (`ppuStack_f8`) and, where
    the slot is only ever taken by address, as a raw frame offset (`&stack0xffffff04`). A local declared
    `X_f8` sits at entry-SP - (0xf8 + 4) = -0xfc, so the two names meet; spell the offset by the declaration so
    the object folds (construction, acquire and destructor all land on one name)."""
    slots = {}
    for m in RE_STACK_DECL.finditer(text):
        slots[-(int(m.group('off'), 16) + 4)] = (m.group('name'), bool(m.group('arr')))

    def repl(m):
        off = int(m.group('off'), 16)
        off -= 1 << 32 if off >= 1 << 31 else 0
        hit = slots.get(off)
        if hit is None:
            return m.group(0)
        name, is_array = hit
        return (m.group('cast') or '') + ('' if is_array else '&') + name
    return re.sub(r'(?P<cast>\((?:[\w :]+\*+|int|uint|undefined4)\))?&stack0x(?P<off>[0-9a-f]{8})\b', repl, text)


CTOR_KINDS = 'RESOURCE_NewResource|RESOURCE_StartMovie|ACTORMAP_New|STRINGMAP_New|QUESTTHING_Empty'
RE_MEMBER_ZERO_INIT = re.compile(
    r'^(?P<ctor>(?P<ind>[ \t]*)(?P<obj>\w+) = (?:' + CTOR_KINDS + r')\([^;\n]*\);[ \t]*\r?\n)'
    # an alias copy of the object may sit between the construction and its member zeroes (Ghidra hoists
    # the call's register set-up above the inlined constructor and hoist_object_aliases moves it back down)
    r'(?P<aliases>(?:[ \t]*\w+ = (?P=obj);[ \t]*\r?\n)*)'
    r'(?P<zeroes>(?:[ \t]*(?P=obj) = (?:\([\w ]+\*?\))?0(?:x0)?;[ \t]*\r?\n)+)', re.M)


def drop_member_zero_inits(text: str) -> str:
    """`X = RESOURCE_NewResource(); X = 0;` -- the zero is the object's own member store, not the handle.

    Ghidra spells an inlined constructor as a vtable store plus one zero per member; the vtable store is
    what folds to the construction, and canonicalise_stack_objects then folds the member slots onto the
    same name. Keeping the zeroes would kill the handle (WaspIntro 0x00E12F20 lifted
    `resources:TryAcquire(0, ...)`, which the sidecar rejects as a released resource)."""
    return RE_MEMBER_ZERO_INIT.sub(lambda m: m.group('ctor') + m.group('aliases'), text)


def canonicalise_stack_objects(text: str) -> str:
    """Ghidra names each stack slot separately (`appuStack_ac` … `uStack_a4`), so members of one
    stack object (a 16-byte resource/movie/map) appear under several names. Objects created by the
    pseudo API give their base slot and size; every slot name inside that extent becomes the base."""
    def plan(text):
        bases = []
        for m in re.finditer(r'\b([A-Za-z]+Stack_|local_)([0-9a-f]+)(?:_p([48c]))? = (RESOURCE_NewResource|RESOURCE_StartMovie|ACTORMAP_New|STRINGMAP_New|QUESTTHING_Empty)\(', text):
            shift = int(m.group(3), 16) if m.group(3) else 0
            bases.append((m.start(), m.group(1), int(m.group(2), 16) - shift, STACK_OBJECT_SIZES[m.group(4)], m.group(0)[:m.group(0).index(' =')], m.group(4)))
        # the same stack bytes may host a different object later in the function: each object's names
        # are rewritten only from its constructor up to the next constructor whose extent overlaps
        pieces = []
        for i, (start, prefix, boff, size, name, _kind) in enumerate(bases):
            def overlaps(b):
                return b[2] - b[3] < boff and boff - size < b[2] and b[2] != boff   # extents (off-size, off]
            # the object's text runs from the previous overlapping construction (its members may be read
            # before the constructor line, e.g. an iterator element copy) to the next one
            region_start = next((b[0] for b in reversed(bases[:i]) if overlaps(b)), 0)
            region_end = next((b[0] for b in bases[i + 1:] if overlaps(b)), len(text))
            pieces.append((region_start, region_end, start, prefix, boff, size, name))
        return sorted(pieces, key=lambda x: (x[0], x[1]))
    pieces = plan(text)
    if not pieces:
        return text

    # None of these objects has a float member (PDB: CScriptGameResourceObject*Base = vtable, base pointer,
    # CCountedPointer; CScriptThing = vtable, CCountedPointer; std::_Tree = comp, head, size), so a float-typed
    # local is never one of their slots. Ghidra's own stack numbering is 4 bytes adrift from the restored
    # (true-slot) base name in places: PreMeleeWhisper 0x00D524A0 keeps `float fStack_94` (true slot -0x90,
    # `fstp [esp+0x28]` at depth 184) beside the resource restored to `xStack_a0` (true -0xa0, 16 bytes), and
    # the containment test alone would fold the marker/hero height difference onto the resource.
    floats = set(re.findall(r'^[ \t]*float (\w+)(?: \[\d+\])?;', text, re.M)) | {m.group(0) for m in re.finditer(r'\bfStack_[0-9a-f]+\b', text)}

    def inside(m, boff, size):
        if m.group(0) in floats:
            return False
        off = int(m.group(2), 16)
        return boff - size < off < boff or off == boff

    def rewrite(segment, ctor_rel, boff, size, name):
        # Before the constructor line the same bytes can still be something else (CheckFriendlyAttacks: the
        # "PreMeleeMaze" temporary, a creature vector's end and capacity all sit where a CScriptThing is built
        # later). A slot that is *assigned* in that pre-range -- an assignment, a constructor receiver, an
        # out-argument -- is live as its own object and keeps its name; only bare reads of the object's
        # members (an iterator element copied before the constructor line) are folded onto the base.
        pre, post = segment[:ctor_rel], segment[ctor_rel:]
        own = {m.group(0) for m in RE_STACK_NAME.finditer(pre) if inside(m, boff, size)}
        own = {n for n in own if re.search(r'^[ \t]*(?:\(\w+ \*+\))?' + re.escape(n) + r'(?:_p[48c])? = |[(,]\s*(?:\([\w ]+\*+\))?&' + re.escape(n) + r'\b', pre, re.M)}

        def member_copy(n):
            # `local_8 = *(int **)(p0 + 4); local_4 = *(int **)(p0 + 8);` before the constructor line: the
            # object's own members written from the same offsets of a source element (a list iterator copy,
            # Artefact.OnPredicateFail) -- that is the object being built, not another live slot
            member = boff - int(RE_STACK_NAME.match(n).group(2), 16)
            writes = re.findall(r'^[ \t]*(?:\(\w+ \*+\))?' + re.escape(n) + r'(?:_p[48c])? = ([^;]+);', pre, re.M)
            return bool(writes) and all(re.fullmatch(r'\*\([\w ]+\*+\)\(\w+ \+ (0x[0-9a-f]+|\d+)\)', w.strip())
                                        and int(re.fullmatch(r'\*\([\w ]+\*+\)\(\w+ \+ (0x[0-9a-f]+|\d+)\)', w.strip()).group(1), 0) == member
                                        for w in writes)
        own = {n for n in own if not member_copy(n)}
        pre = RE_STACK_NAME.sub(lambda m: name if inside(m, boff, size) and m.group(0) != name and m.group(0) not in own else m.group(0), pre)
        # After the constructor line the object's own members may still be stored under Ghidra's slot names
        # (a map's size `iStack_28 = 0`, TraderConflictGood helper_DFDED0) and must fold. But Ghidra's drifted
        # numbering can also put an unrelated live slot inside the extent: CheckFriendlyAttacks 0x00D45060
        # keeps a creature vector's end `puStack_90` (zeroed, then `(int)puStack_90 - (int)puStack_94`) on the
        # PreMeleeMaze thing's base 0x90 after the second GetAllCreaturesExcludingHero fill; folding it read
        # `preMeleeMaze - creatures2` and killed the thread on every return to the Guild (2026-09-20). A name
        # assigned in this range AND used in pointer arithmetic / a compare against a stack slot OUTSIDE the
        # extent is its own object (a member is never subtracted from a sibling of another object).
        def outside_partner(n):
            for m in re.finditer(r'(?:\(int\))?' + re.escape(n) + r' (?:-|[!=]=) (?:\(int(?: \*)?\))?(\w+)\b|(?:\(int\))?(\w+) (?:-|[!=]=) (?:\(int(?: \*)?\))?' + re.escape(n) + r'\b', post):
                other = m.group(1) or m.group(2)
                om = RE_STACK_NAME.fullmatch(other)
                if om and not inside(om, boff, size):
                    return True
            return False
        own_post = {m.group(0) for m in RE_STACK_NAME.finditer(post) if inside(m, boff, size) and m.group(0) != name}
        # ... or assigned the RESULT of a script-interface call (`iStack_24 = GSI->AddNewConversation(...)`): no
        # member of a resource / thing / map is ever written from a GSI result, so that slot is a live scalar of
        # its own even when the restored base numbering puts it inside the extent (CheckFriendlyAttacks: the
        # conversation id folded onto the Maze resource `xStack_30`, then released as a resource, 2026-09-20)
        def gsi_result(n):
            return bool(re.search(r'^[ \t]*' + re.escape(n) + r' = (?:GSI->|\(\*\*\(code \*\*\))', post, re.M))
        own_post = {n for n in own_post if re.search(r'^[ \t]*(?:\(\w+ \*+\))?' + re.escape(n) + r'(?:_p[48c])? = ', post, re.M) and (outside_partner(n) or gsi_result(n))}
        post = RE_STACK_NAME.sub(lambda m: name if inside(m, boff, size) and m.group(0) != name and m.group(0) not in own_post else m.group(0), post)
        segment = pre + post
        segment = re.sub(r'\(' + re.escape(name) + r' \+ (?:4|8|0xc|12)\)', name, segment)
        segment = re.sub(r'&' + re.escape(name) + r'\b', name, segment)
        return segment
    # rewrite spans back to front, re-planning from the CURRENT text before each one: regions of different objects
    # overlap, and a rewrite that changes length (`&xStack_20` -> `xStack_20`) shifted the text under the next
    # splice's stale offsets (Q_WhiteBalverineKnotholeGlade Main 0x00E13F10: the second cutscene's movie came out
    # `xStack_100 = RESOURCE_StartMovie("")`, a duplicated character, so DestroyMovie freed the first movie, 2026-09-26)
    for k in reversed(range(len(pieces))):
        current = plan(text)
        if len(current) != len(pieces):
            break
        start, end, ctor, prefix, boff, size, name = current[k]
        text = text[:start] + rewrite(text[start:end], ctor - start, boff, size, name) + text[end:]
    return text


RE_OFFSET_STRING_CTOR = re.compile(
    r'^[ \t]*CCharString::CCharString\(\(CCharString \*\)\((?P<expr>\w+ \+ (?:\d+|0x[0-9a-f]+))\),(?P<lit>"[^"]*"),-1\);[ \t]*\r?\n', re.M)


def fold_sibling_slot_offsets(text: str) -> str:
    """`(CCharString *)(xStack_1c + 4)` names the object Ghidra itself constructed as `xStack_18` (the next
    stack slot up); spell it by that name so the temporary tracking sees one object."""
    def sibling(m):
        prefix, slot, off = m.group(1), int(m.group(2), 16), int(m.group(3), 0)
        target = f'{prefix}{slot - off:x}'
        if re.search(r'CCharString::(?:CCharString|operator=)\(\(CCharString \*\)&?' + re.escape(target) + r'\b', text):
            return f'(CCharString *){target}'
        return m.group(0)
    return re.sub(r'\(CCharString \*\)\((\w*Stack_)([0-9a-f]+)(?:_\d+)? \+ (4|8|0xc|12)\)', sibling, text)   # (`xStack_70_2`: a later object at the slot)


def fold_offset_string_temporaries(text: str) -> str:
    """A CCharString literal constructed at an offset inside a stack struct (`(auStack_64 + 8)`) is a
    temporary the lifter cannot track by name; substitute the literal for its uses and drop the
    constructor/destructor pair."""
    pos = 0
    while (m := RE_OFFSET_STRING_CTOR.search(text, pos)):
        expr, lit = m.group('expr'), m.group('lit')
        head, tail = text[:m.start()], text[m.end():]
        # the slot may be reused by a later construction: substitute only up to that point
        reuse = re.search(r'^[ \t]*CCharString::(?:CCharString|operator=)\(\(CCharString \*\)\(' + re.escape(expr) + r'\),', tail, re.M)
        scope, rest = (tail[:reuse.start()], tail[reuse.start():]) if reuse else (tail, '')
        scope = re.sub(r'^[ \t]*std::\s*_Cons_val<[^;(]*?\s*\(\(CCharString \*\)\(' + re.escape(expr) + r'\)\);[ \t]*\r?\n', '', scope, count=1, flags=re.M)
        scope = re.sub(r'\(CCharString \*\)\(' + re.escape(expr) + r'\)', lit, scope)
        scope = re.sub(r'(?<![\w])\(' + re.escape(expr) + r'\)', lit, scope)
        text = head + scope + rest
        pos = m.start()
    return text


SCALAR_SLOT_CAST = r'\((?:CCharString|byte|uchar|char|int|uint|undefined1|undefined4)\)'


def split_scalar_slot_tail(text):
    """A stack slot the compiler reuses: an object first (its address taken: a string temporary's ctor / dtor), a
    plain byte or int afterwards. The export keeps the object's type, so the scalar stores and tests read
    `xStack_9c = (CCharString)0x2;` / `if (xStack_9c == (CCharString)0x1)`. Every line after the slot's LAST
    address use that names it in only those shapes is the scalar life: give it a lifter-visible scalar name
    (CampHostageGuard.Main 0x00D09010: the "CampHostage" lookup string, then the guard's patrol leg 1/2 --
    left as an object name, both leg stores were dropped and the test folded to `1 == 1`, so the guard never
    walked to GuardSecondMarker and the hostage door stayed guarded, 2026-09-26)."""
    # (a hidden-result slot too: only lines after its last address use are touched)
    lines = text.split('\n')
    for name in sorted(set(re.findall(r'\b(?:[A-Za-z]+Stack_|local_)[0-9a-f]+\b', text))):
        n = re.escape(name)
        store = re.compile(r'^([ \t]*)' + n + r' = ' + SCALAR_SLOT_CAST + r'(0x[0-9a-f]+|\d+);[ \t]*\r?$')
        test = re.compile(r'\b' + n + r' ([!=]=) ' + SCALAR_SLOT_CAST + r'(0x[0-9a-f]+|\d+)(?=\W)')
        uses = [i for i, l in enumerate(lines) if re.search(r'\b' + n + r'\b', l)]
        addressed = [i for i in uses if re.search(r'&' + n + r'\b|\(\w+ \*+\)' + n + r'\b|\b' + n + r'\s*[\[.]', lines[i])]
        if not addressed:
            continue
        tail = [i for i in uses if i > addressed[-1] and not re.match(r'^[ \t]*(?:[\w:<>,]+ )+\**' + n + r'(?: \[\d+\])?;', lines[i])]
        # stores AND a test: a store-only tail is a member reset of the dead object (MagicBarrier's `= 0`s), not a variable
        if not tail or not any(store.match(lines[i]) for i in tail) or not any(test.search(lines[i]) for i in tail):
            continue
        # a state byte / counter only: code-address stores (V_Bordello Madame's `xStack_12c = (CCharString)0xe3dfe8`
        # run) are Ghidra's view of a pushed return address or pointer, not a variable
        if any(int(store.match(lines[i]).group(2), 0) > 0xffff for i in tail if store.match(lines[i])):
            continue
        if not all(store.match(lines[i]) or (test.search(lines[i]) and not re.search(r'\b' + n + r'\b', test.sub('', lines[i])))
                   for i in tail):
            continue
        m = re.fullmatch(r'([A-Za-z]*)(?:Stack_|local_)([0-9a-f]+)', name)
        scalar = f'{m.group(1) or "v"}_stk_{m.group(2)}'
        for i in tail:
            lines[i] = store.sub(lambda mm: f'{mm.group(1)}{scalar} = {int(mm.group(2), 0)};', lines[i])
            lines[i] = test.sub(lambda mm: f'{scalar} {mm.group(1)} {int(mm.group(2), 0)}', lines[i])
    return '\n'.join(lines)


def rename_scalar_stack_locals(text):
    """Ghidra stack names (`local_14`, `uStack_8`) are refused by the lifter's assignment rule (they
    are usually object slots). Ones that only ever appear as plain scalars get lifter-visible names."""
    # hidden-return slots of GSI/helper calls (`GSI->GetThingWithScriptName((CScriptThing *)auStack_1c, ...)`) are
    # the lifter's slot aliases (`r1`): keep their spelling
    # (at this stage GSI calls are still raw vcalls with the receiver as first argument, so the slot may be any argument)
    STK = r'((?:[A-Za-z]+Stack_|local_)[0-9a-f]+)'
    # any stack object passed as a call argument (bare array name, `&name` or `(CScriptThing *)name`) is an
    # address the lifter resolves through its slot table
    # (`switch(fStack_94)` / `ABS(fStack_94)` -- a keyword or Ghidra's fabs macro on a by-value float -- is not a call taking the slot's address)
    # (`ENGINE_Vector3(fStack_64, fStack_60, fStack_5c)` -- a stack vector's three float components, `fold_stack_vector_builds` -- are scalars by construction)
    hidden = set(re.findall(r'[(,]\s*(?:\(CScriptThing \*\))?&?' + STK + r'\s*[,)]',
                            re.sub(r'ENGINE_Vector3\([^)]*\)', 'ENGINE_Vector3[]', re.sub(r'\b(switch|if|while|ABS)\s*\(', r'\1[', text))))
    # a FLOAT slot passed by value (`SetFacingAngle(me, fStack_94, true)`, never `&fStack_94`) is a scalar operand, not a
    # hidden-result address (WillDummy 0x00D43450's spin angle stayed an object name and its store a TODO, 2026-09-21)
    hidden -= {h for h in hidden if h.startswith(('fStack_', 'iStack_')) and not re.search(r'&' + re.escape(h) + r'\b', text)
               and not re.search(r'\(CScriptThing \*\)' + re.escape(h) + r'\b', text)}      # (an INT by value too: a tick handle / a toggle)
    # a slot that receives a lowered value by plain assignment is a handle, not a hidden result
    # (unless it is also an explicitly cast hidden-return argument: a thing object later overwritten by a copy)
    cast_args = set(re.findall(r'[(,]\s*\(CScriptThing \*\)' + STK + r'\s*[,)]', text))
    hidden -= set(re.findall(r'^[ \t]*' + STK + r' = (?:p[A-Z]\w*|thing_\w+|r\d+|native_arg_\w+);', text, re.M)) - cast_args   # pointer-typed values only
    # a stack CScriptThing that only ever holds lowered values is a plain handle: drop its casts
    text = re.sub(r'\(CScriptThing \*\)((?:[A-Za-z]+Stack_|local_)[0-9a-f]+)\b', lambda m: m.group(0) if m.group(1) in hidden else m.group(1), text)
    text = split_scalar_slot_tail(text)
    body = re.sub(r'^[ \t]*(?:[\w:<>,]+ )+\**\w+(?: \[\d+\])?;[ \t]*\r?$', '', text, flags=re.M)   # declarations
    for name in sorted(set(re.findall(r'\b(?:[A-Za-z]+Stack_|local_)[0-9a-f]+\b', body))):
        if name in hidden:
            continue
        if re.search(r'[&*]' + re.escape(name) + r'\b|\b' + re.escape(name) + r'\s*[\[.]|\(\w+ \*+\)' + re.escape(name) + r'\b', body):
            continue
        m = re.fullmatch(r'([A-Za-z]*)(?:Stack_|local_)([0-9a-f]+)', name)
        text = re.sub(r'\b' + re.escape(name) + r'\b', f'{m.group(1) or "v"}_stk_{m.group(2)}', text)
    return text


def fold_stack_vector_builds(text):
    """A C3DVector assembled from three adjacent float slots and passed by address (`fStack_64 = X; fStack_60 = Y;
    fStack_5c = Z; F(.., (C3DVector_bv *)&fStack_64, ..)`, SkillTarget 0x00D41D00's moving-dummy segments), or
    aliased first (`pPos = (C3DVector_bv *)&fStack_a0;` then the three stores, the call after the if/else join):
    the operand becomes `ENGINE_Vector3(x, y, z)` over the three float locals (the stores stay as they are, in
    their order), and an alias is re-issued after the last component store of its block. Runs on Ghidra's
    slot names, before `restore_stack_operands` renames the address-taken slot (it drifted `fStack_64` to
    `xStack_58` and the components no longer lined up); the stores are then plain float locals."""
    def components(slot):
        prefix, num = re.match(r'(\w*Stack_)([0-9a-f]+)$', slot).groups()
        n = int(num, 16)
        return slot, f'{prefix}{n - 4:x}', f'{prefix}{n - 8:x}'
    lines = text.split('\n')
    def stored_near(idx, names):
        """the line indices of `name = ..;` float stores within the 12 lines before (and, for an alias, after) idx"""
        found = {}
        for j in range(max(0, idx - 12), min(len(lines), idx + 8)):
            m = re.match(r'[ \t]*(\w+) = (?!=)[^;]+;[ \t]*$', lines[j].rstrip('\r'))
            if m and m.group(1) in names and m.group(1) not in found:
                found[m.group(1)] = j
        return found
    # direct operand (a call line; the alias assignments are handled below, after their stores)
    for i, line in enumerate(lines):
        if re.match(r'[ \t]*\w+ = \(C3DVector_bv \*\)&', line):
            continue
        for m in re.finditer(r'\(C3DVector_bv \*\)&(f\w*Stack_[0-9a-f]+)\b', line):
            x, y, z = components(m.group(1))
            if set(stored_near(i, {x, y, z})) == {x, y, z}:
                lines[i] = lines[i].replace(m.group(0), f'ENGINE_Vector3({x}, {y}, {z})')
    # alias
    i = 0
    while i < len(lines):
        m = re.match(r'([ \t]*)(\w+) = \(C3DVector_bv \*\)&(f\w*Stack_[0-9a-f]+);[ \t]*$', lines[i].rstrip('\r'))
        if m:
            x, y, z = components(m.group(3))
            found = stored_near(i, {x, y, z})
            if set(found) == {x, y, z}:
                last = max(found.values())
                alias = f'{m.group(1)}{m.group(2)} = ENGINE_Vector3({x}, {y}, {z});'
                if last > i:
                    del lines[i]
                    lines.insert(last, alias)
                else:
                    lines[i] = alias
        i += 1
    return '\n'.join(lines)


def resolve_me_register_uses(text: str) -> str:
    """The compiler keeps `edi = this + 8` (the entity's own thing) for a whole function; Ghidra names that register
    `pCVarN` and, on paths where a SIBLING branch reassigned `pCVarN`, prints the operand as the bare register with
    no definition in force (`GetNearestWithScriptName(.., pCVar6, "StaticDummyMarker2")` in SkillTarget's dummy-2/3
    branches: `push edi` in the bytes, `me` in branch 1; lifted as nil it crashed the game at frame 0, 2026-09-21).
    A use of such a register with no assignment in an ENCLOSING block before it (block structure by braces) is
    `(CScriptThing *)(this + 8)`; only registers assigned `(this + 8)` somewhere in the function qualify."""
    regs = set(re.findall(r'^[ \t]*(p?[A-Za-z]{1,3}Var\d+) = \(CScriptThing(?:_bv)? \*\)\((?:\(int\))?this \+ 8\);', text, re.M))
    if not regs:
        return text
    # the chain of open blocks (ids) at every character: a definition dominates a later use when its chain is a
    # prefix of the use's (same block or an enclosing one); a sibling branch's chain is not
    chains, stack, block_id, cur = [], [], 0, ()
    for ch in text:
        if ch == '{':
            block_id += 1
            stack.append(block_id)
            cur = tuple(stack)
        elif ch == '}':
            if stack:
                stack.pop()
            cur = tuple(stack)
        chains.append(cur)
    def chain(pos):
        return chains[pos] if pos < len(chains) else ()
    for reg in regs:
        r = re.escape(reg)
        defs = [(m.start(), m.end(), chain(m.start()), bool(re.match(r'\(CScriptThing(?:_bv)? \*\)\((?:\(int\))?this \+ 8\)', m.group(1))))
                for m in re.finditer(r'^[ 	]*' + r + r' = ([^;]+);', text, re.M)]
        def use(m):
            pos, c = m.start(1), chain(m.start(1))
            # the explicit receiver operand of a vcall through the same register (`(*(int *)pCVar7 + 0x10c))(pCVar7, true)`)
            # is the annotator's to fold (CombatApprentice's SetFriendsWithEverythingFlag): leave it
            if m.group(0).startswith('(') and re.search(r'\*\(int \*\)' + r + r' \+ (?:0x[0-9a-f]+|\d+)\)\)$', text[max(0, m.start() - 80):m.start()]):
                return m.group(0)
            # (a definition whose own statement contains the use -- `pCVar6 = F(.., pCVar6, ..)` -- is not in force yet)
            dominating = [(dpos, is_me) for dpos, dend, dchain, is_me in defs if dend <= pos and dchain == c[:len(dchain)]]
            if dominating and not dominating[-1][1]:
                return m.group(0)                   # the nearest dominating definition is something else
            return m.group(0)[:m.start(1) - m.start()] + '(CScriptThing *)(this + 8)' + m.group(0)[m.end(1) - m.start():]
        text = re.sub(r'[(,]\s*(' + r + r')\s*(?=[,)])', use, text)
    return text


def fold_position_reads(text):
    """A position read through a pointer to a thing's C3DVector: `pf = (float *)GetPos(X); f = *pf; g = pf[1];
    h = pf[2]`, or the same through an untyped pointer (`puVar8 = (undefined4 *)GetPos(X); puVar8[2]`) and
    through a CScriptThing-typed one (`pCVar6 = GetPos(hero); *(float *)(pCVar6 + 0x8)`: PreMeleeWhisper
    0x00D524A0 compares the marker's z with the hero's z, `fld [ebx+8]; fsub [eax+8]`). The engine's static
    zero vector (DAT_0143e8e0, zero-initialised .data) stands for the position of an invalid thing. Runs in
    `lower` (for a GetPos the typed export already named) and again after the annotation pass (which names
    the vtable-slot spelling)."""
    text = re.sub(r'(?:\((?:float|C3DVector|undefined4|int) \*\))?&DAT_0143e8e0\b', 'ENGINE_ZeroVector()', text)
    vecs = set(re.findall(r'^[ \t]*(\w+) = (?:\((?:float|undefined4|int|C3DVector) \*\))?(?:CScriptThing::GetPos\(|ENGINE_ZeroVector\(\))', text, re.M))
    for v in vecs:
        text = re.sub(r'^([ \t]*)' + re.escape(v) + r' = \((?:float|undefined4|int|C3DVector) \*\)(?=CScriptThing::GetPos\()', r'\1' + v + ' = ', text, flags=re.M)
        text = re.sub(r'^[ \t]*[\w:]+ \*' + re.escape(v) + r';[ \t]*\r?\n', '', text, flags=re.M)
        # (`undefined4` / `int`: the same 4-byte member read of a member-wise copy, WB_WhiteBalverine 0x00E16290)
        text = re.sub(r'\*\((?:float|undefined4|int) \*\)\(' + re.escape(v) + r' \+ (?:4|0x4)\)', v + '.y', text)
        text = re.sub(r'\*\((?:float|undefined4|int) \*\)\(' + re.escape(v) + r' \+ (?:8|0x8)\)', v + '.z', text)
        text = re.sub(r'\*\((?:float|undefined4|int) \*\)' + re.escape(v) + r'\b', v + '.x', text)
        # (not a vtable-call head: the register is reused, and `(**(code **)(*piVar5 + 0x5ec))(piVar5,..)` is the
        # interface's PauseAllNonScriptedEntities while piVar5 = *(int **)(this + 4) -- EndTrader Main had twelve
        # such calls turned into `piVar5.x + 0x5ec` TODOs, so its movies never paused the world, 2026-09-24)
        text = re.sub(r'(?<!\(\*\*\(code \*\*\)\()\*' + re.escape(v) + r'\b', v + '.x', text)
        text = re.sub(r'\b' + re.escape(v) + r'\[1\]', v + '.y', text)
        text = re.sub(r'\b' + re.escape(v) + r'\[2\]', v + '.z', text)
        text = re.sub(r'\(float\)' + re.escape(v) + r'\.([xyz])\b', v + r'.\1', text)
    # a member-wise copy of that vector into a 12-byte stack C3DVector (`CStack_114._0_4_ = V.x; ._4_4_ = V.y;
    # ._8_4_ = V.z;`) that the next call passes by address: the call operand was restored to its true slot
    # (`(C3DVector *)xStack_114_3` for Ghidra's `auStack_118 + 4`), so a store target with the same slot
    # number is that object (PreMeleeGuildmaster 0x00D52E90: `CreateEffect(.., "SMASH_DUMMY_01", &dummyPos, ..)`)
    def vector_copy(m):
        ind, slot, v = m.group('ind'), m.group('slot'), m.group('v')
        rest = text[m.end():]
        window = '\n'.join(rest.split('\n')[:6])
        operand = re.search(r'\(C3DVector \*\)(?:&)?(xStack_' + re.escape(slot) + r'(?:_\d+)?)\b', window)
        name = operand.group(1) if operand else m.group('obj')
        return f'{ind}{name} = ENGINE_VectorCopy({v});\n'
    text = re.sub(r'^(?P<ind>[ \t]*)(?P<obj>\w*Stack_(?P<slot>[0-9a-f]+))(?:\._0_4_|\[0\]) = (?P<v>\w+)\.x;[ \t]*\r?\n'
                  r'[ \t]*(?P=obj)(?:\._4_4_|\[1\]) = (?P=v)\.y;[ \t]*\r?\n'
                  r'[ \t]*(?P=obj)(?:\._8_4_|\[2\]) = (?P=v)\.z;[ \t]*\r?\n', vector_copy, text, flags=re.M)
    # the same copy with the compiler's unrelated scalar stores scheduled between the member stores (DarkwoodTrader
    # Main: `._4_4_ = V.y; fVar21 = 3.0; ._8_4_ = V.z;` -- the waypoint position of every IsDistanceFromPositionOver
    # test, 2026-09-24); the interleaved stores are kept, after the copy
    def vector_copy_interleaved(m):
        between = [l for l in (m.group('a') or '', m.group('b') or '') if l]
        if any(re.search(r'\b(?:' + re.escape(m.group('obj')) + '|' + re.escape(m.group('v')) + r')\b', l) for l in between):
            return m.group(0)
        return vector_copy(m) + ''.join(between)
    other = r'(?:[ \t]*\w+ = [^;\n]*;[ \t]*\r?\n)?'
    text = re.sub(r'^(?P<ind>[ \t]*)(?P<obj>\w*Stack_(?P<slot>[0-9a-f]+))(?:\._0_4_|\[0\]) = (?P<v>\w+)\.x;[ \t]*\r?\n'
                  r'(?P<a>' + other + r')[ \t]*(?P=obj)(?:\._4_4_|\[1\]) = (?P=v)\.y;[ \t]*\r?\n'
                  r'(?P<b>' + other + r')[ \t]*(?P=obj)(?:\._8_4_|\[2\]) = (?P=v)\.z;[ \t]*\r?\n', vector_copy_interleaved, text, flags=re.M)
    # the three member stores in any order with computed values (Q_WhiteBalverineKnotholeGlade Main 0x00E13F10: the
    # ambush marker's position raised 9 units, stored z, y, x: `._8_4_ = V.z + 9.0; ._4_4_ = V.y; ._0_4_ = V.x;`)
    member = {'_0_4_': 0, '_4_4_': 1, '_8_4_': 2}
    def vector_build(m):
        rows = [(m.group(f'k{i}'), m.group(f'e{i}')) for i in range(3)]
        obj = m.group('obj')
        if sorted(member[k] for k, _ in rows) != [0, 1, 2] or any(re.search(r'\b' + re.escape(obj) + r'\b', e) for _, e in rows):
            return m.group(0)
        # a position computed from values: an all-constant fill (the zero member-init, a float's bit pattern as an int,
        # KickedChicken's `._8_4_ = 0x3f800000`) is left to the passes that know those shapes
        if not any(re.search(r'\b[A-Za-z_]\w*\b', re.sub(r'\b0x[0-9a-fA-F]+\b', '', e)) for _, e in rows):
            return m.group(0)
        comp = dict((member[k], e) for k, e in rows)
        return f"{m.group('ind')}{obj} = ENGINE_Vector3({comp[0]}, {comp[1]}, {comp[2]});\n"
    store = r'[ \t]*(?P=obj)\.(?P<k{n}>_[048]_4_) = (?P<e{n}>[^;\n]+);[ \t]*\r?\n'
    text = re.sub(r'^(?P<ind>[ \t]*)(?P<obj>\w*Stack_[0-9a-f]+)\.(?P<k0>_[048]_4_) = (?P<e0>[^;\n]+);[ \t]*\r?\n'
                  + store.format(n=1) + store.format(n=2), vector_build, text, flags=re.M)
    return text


def fold_by_value_thing_release(text):
    """A by-value CScriptThing (a parameter the callee owns) is destroyed inline through its Info word `T._8_4_`:
    `if ((T._8_4_ != 0) && (*T._8_4_ = *T._8_4_ + -1, *T._8_4_ == 0)) { destroy; [DEL:] operator_delete; }`, and on
    another exit the same test inverted, `if ((T._8_4_ == 0) || (..., *T._8_4_ != 0)) goto L; destroy; goto DEL;`
    (DEL falls through to L). Both are refcount bookkeeping the Lua GC owns: the forward block keeps only its label,
    the inverted one becomes `goto L;`. A name released this way is a counted thing, so its Data word `T._4_4_`
    tested against zero is validity, and as IsEqualTo's operand (retail slot 0x138 takes other.Data) it is the thing
    (WatchForPickpocketing 0x00E04F10 -- the dropped label left an exit falling back into the loop, 2026-09-24)."""
    names = set(re.findall(r'\((\w+)\._8_4_ [!=]= 0\) (?:&&|\|\|) \(\*\1\._8_4_ = \*\1\._8_4_ \+ -1,', text))
    for name in names:
        n = re.escape(name)
        test = r'\*' + n + r'\._8_4_ = \*' + n + r'\._8_4_ \+ -1, \*' + n + r'\._8_4_'
        destroy = r'[ \t]*\(\*\*\(code \*\*\)\(' + n + r'\._8_4_ \+ 4\)\)\(\);[ \t]*\r?\n'
        text = re.sub(r'^[ \t]*if \(\(' + n + r'\._8_4_ != 0\) && \(' + test + r' == 0\)\) \{[ \t]*\r?\n' + destroy
                      + r'(?:(?P<label>\w+):[ \t]*\r?\n)?[ \t]*operator_delete\(\(void \*\)' + n + r'\._8_4_\);[ \t]*\r?\n[ \t]*\}[ \t]*\r?\n',
                      lambda m: f'{m.group("label")}:\n' if m.group('label') else '', text, flags=re.M)
        text = re.sub(r'^([ \t]*)if \(\(' + n + r'\._8_4_ == 0\) \|\| \(' + test + r' != 0\)\)[ \t]*\r?\n[ \t]*goto (\w+);[ \t]*\r?\n'
                      + destroy + r'[ \t]*goto \w+;[ \t]*\r?\n', r'\1goto \2;\n', text, flags=re.M)
        text = re.sub(r'\b' + n + r'\._4_4_ == 0\b(?!x)', f'!__thing_valid({name})', text)
        text = re.sub(r'\b' + n + r'\._4_4_ != 0\b(?!x)', f'__thing_valid({name})', text)
        text = re.sub(r'(CScriptThing::IsEqualTo\([^,;()]+,\s*)' + n + r'\._4_4_\)', r'\1' + name + ')', text)
    return text


RE_THING_COPY_FROM_POINTER = re.compile(
    r'^(?P<ind>[ \t]*)(?P<info>\w+) = \*\(int \*\*\)\((?P<src>\w+) \+ 0x8\);[ \t]*\r?\n'
    r'[ \t]*(?P<data>\w+) = \*\(undefined4 \*\)\((?P=src) \+ 0x4\);[ \t]*\r?\n'
    r'[ \t]*if \((?P<dst>\w+)\._8_4_ != (?P=info)\) \{[ \t]*\r?\n'
    r'[ \t]*(?P=dst)\._4_4_ = (?P=data);[ \t]*\r?\n'
    r'[ \t]*(?P=dst)\._8_4_ = (?P=info);[ \t]*\r?\n'
    r'[ \t]*if \((?P=info) != \(int \*\)0x0\) \{[ \t]*\r?\n'
    r'[ \t]*\*(?P=info) = \*(?P=info) \+ 1;[ \t]*\r?\n'
    r'[ \t]*\}[ \t]*\r?\n'
    r'[ \t]*\}[ \t]*\r?\n', re.M)


def fold_thing_copy_from_pointer(text):
    """`CScriptThing::operator=` inlined from a returned thing pointer (read Info/Data, compare, store, retain):
    `dst = src` (MagicBarrier Main 0x00E03F70 keeps each CreateEffectAtPos result in a stack thing it later
    removes; lifted raw the copy was four TODOs and the effect handle was lost, 2026-09-24)."""
    return RE_THING_COPY_FROM_POINTER.sub(lambda m: f"{m.group('ind')}{m.group('dst')} = (CScriptThing *){m.group('src')};\n", text)


def fold_char_flags(text):
    """A byte flag (`cVar = '\\0'` / `'\\x01'`, tested `cVar != '\\0'`) mixes numeric literals with boolean tests once
    lifted: `cVar = 0 ... if cVar then` is always taken (0 is truthy in Lua) and `if not cVar then` never is. When a
    plain local is only ever tested against '\\0' and never used in arithmetic, its literals are the booleans
    (WatchForPickpocketing: a null trader took the "killed" branch; WaspHelper's one-shot lines never played;
    WaspBoss treated a null queen as killed; V_TourGuide left its watch loop, 2026-09-24)."""
    candidates = set(re.findall(r"\b([A-Za-z_]\w*) [!=]= '\\0'", text)) | set(re.findall(r'\(bool\)([A-Za-z_]\w*)\b', text))
    for var in candidates:
        v = re.escape(var)
        if re.search(r'Stack_|_stk_|_b\d$', var):
            continue        # a stack slot / merged byte: the lifter keeps literal slot stores as temporaries, and a
                            # `= false` there would vanish where the `= '\0'` it replaces stays a visible TODO
        if re.search(r'(?:[\w.]\.|->|\*|&)' + v + r'\b', text):
            continue        # a field, a dereference or an address, not a plain local
        if re.search(r'\b' + v + r'\s*(?:[-+*/%&|^<>]|<<|>>)(?!=)|(?:[-+*/%&|^]|<<|>>)\s*(?:\(\w+\))?' + v + r'\b', text):
            continue        # used arithmetically: keep the numbers
        if re.search(r'\b' + v + r' [!=]= (?!\'\\0\')', text):
            continue        # compared against something else
        # a byte local (`undefined1 uVar2;` / char / bool) takes 0 / 1 stores too: WaspHelper Main's one-shot flags
        # (its 0 / 1 stores are the flag's literals only when every other store is a literal or a truth value: a call
        # result such as `cVar2 = MsgIsKilledBy(..)` is a bool, a stored count is not)
        byte = re.search(r'^[ \t]*(?:undefined1|char|bool|byte)\s+' + v + r';', text, re.M)
        numeric_ok = byte and not re.search(r'\b' + v + r' = (?!0;|1;|\'\\0\';|\'\\x01\';|\(bool\)|!|\(?\w+\s*[!=<>]=?|[\w:]*(?:Is|Msg|Has|Get\w*Bool)\w*\()', text)
        if numeric_ok:
            text = re.sub(r'\b' + v + r' = 0;', var + ' = false;', text)
            text = re.sub(r'\b' + v + r' = 1;', var + ' = true;', text)
        text = re.sub(r'\b' + v + r" = '\\0';", var + ' = false;', text)
        text = re.sub(r'\b' + v + r" = '\\x01';", var + ' = true;', text)
        text = re.sub(r'\b' + v + r" != '\\0'", var, text)
        text = re.sub(r'\b' + v + r" == '\\0'", '!' + var, text)
        text = re.sub(r'\(bool\)' + v + r'\b', var, text)
    return text


def fold_things_killed_vectors(text):
    """`MsgGetThingsKilled(thing, &V)` whose words the script reads. Retail (CGameScriptThing 0x008D3DD0) pushes one
    CEventKilledCreature::CreatureGroupOfKilledThing word per kill this frame into the std::vector<ulong> {V, E},
    returning true when it pushed one. Q_BanditCamp CheckAnyBanditsKilled 0x00D032F0 counts the words with bit 4
    (`*(byte *)(R + i * 4) & 4` through register copies of V / E); the area-massacre checks add the count
    (`E - (int)V >> 2`) for the hero and each follower / summon, then erase the words. The sidecar binding
    MsgGetThingsKilledGroups returns them as a Lua list, so: the call fills V, the count is its length, a word is
    `V[i + 1]`, and the ctor / zeroing / erase / free are list resets. Every script read of the vector and its
    register copies must be one of those shapes, or the function is left as it was."""
    for vec in sorted(set(re.findall(r'CScriptThing::MsgGetThingsKilled\([^;]+?,\s*&(\w+)\)', text))):
        v = re.escape(vec)
        ends = set(re.findall(r'(\w+) - (?:\(int\))?' + v + r' >> 2', text))
        if len(ends) != 1:
            continue
        end = ends.pop()
        e = re.escape(end)
        new = text
        results = []
        def call(m):
            results.append(m.group(2))
            return (f'{m.group(1)}{vec} = CScriptThing::MsgGetThingsKilledGroups({m.group(3)});\n'
                    f'{m.group(1)}{m.group(2)} = ENGINE_ListLen({vec}) != 0;')
        new = re.sub(r'^([ \t]*)(\w+) = CScriptThing::MsgGetThingsKilled\(([^;]+?),\s*&' + v + r'\);', call, new, flags=re.M)
        for r in results:   # a char-typed result (`cVar5 != '\0'`) now holds a Lua boolean
            new = re.sub(r'\b' + re.escape(r) + r" != '\\0'", r, new)
            new = re.sub(r'\b' + re.escape(r) + r" == '\\0'", '!' + r, new)
        new = re.sub(r'^([ \t]*)' + v + r' = 0;', r'\g<1>' + vec + ' = ENGINE_EmptyList();', new, flags=re.M)
        new = re.sub(r'^[ \t]*' + e + r' = 0;[ \t]*\r?\n', '', new, flags=re.M)
        new = re.sub(r'^([ \t]*)CIndexBuffer::CIndexBuffer\(\(CIndexBuffer \*\)&' + v + r',[^;]*\);', r'\g<1>' + vec + ' = ENGINE_EmptyList();', new, flags=re.M)
        new = re.sub(r'^[ \t]*CFileInstaller::CActiveFile::OnReadFinished\(\(CActiveFile \*\)&' + v + r'\);[ \t]*\r?\n', '', new, flags=re.M)
        new = re.sub(r'^([ \t]*)std_vector_push_copy_element\(&' + v + r',[^;]*\);', r'\g<1>' + vec + ' = ENGINE_EmptyList();', new, flags=re.M)
        new = re.sub(r'\b' + e + r' - (?:\(int\))?' + v + r' >> 2\b', f'ENGINE_ListLen({vec})', new)
        out, alias, ok = [], {}, True
        for line in new.split('\n'):
            m = re.match(r'^[ \t]*(\w+) = (\w+);[ \t]*$', line)
            if m and m.group(2) in (vec, end) and m.group(1) not in (vec, end):
                alias[m.group(1)] = 'begin' if m.group(2) == vec else 'end'
                continue
            m = re.match(r'^[ \t]*(\w+) = ', line)
            if m and m.group(1) in alias:
                del alias[m.group(1)]             # the register takes another value: its copy of the vector ends
            begins = [vec] + [n for n, k in alias.items() if k == 'begin']
            ends_ = [end] + [n for n, k in alias.items() if k == 'end']
            for b in begins:
                line = re.sub(r'\*\((?:byte|uint|int|ulong|undefined4) \*\)\(' + re.escape(b) + r' \+ (\w+) \* 4\)',
                              lambda mm: f'ENGINE_ListWord({vec}, {mm.group(1)})', line)
                for en in ends_:
                    line = re.sub(r'\b' + re.escape(en) + r' - (?:\(int\))?' + re.escape(b) + r' >> 2\b',
                                  f'ENGINE_ListLen({vec})', line)
            if any(re.search(r'\b' + re.escape(n) + r'\b', line) for n in alias) or re.search(r'\b' + e + r'\b', line) \
                    and not re.match(r'^[ \t]*[\w ]+ \**' + e + r';', line):
                ok = False                         # a copy of the vector used some other way
                break
            out.append(line)
        if ok:
            text = '\n'.join(out)
            # a kill total kept in a slot the export typed as a string (the area-massacre checks' CStack_74:
            # `= 0`, `+= count`, `<= ReadGlobalGameDataFloat(0xe78)`, later the slot of an empty name string)
            for total in set(re.findall(r'^[ \t]*(\w+) = \(CCharString(?:_bv)?\)\(\(int\)\1 \+ \(?ENGINE_ListLen\(' + v + r'\)\)?\);',
                                        text, re.M)):
                t = re.escape(total)
                text = re.sub(r'^([ \t]*)' + t + r' = \(CCharString(?:_bv)?\)\(\(int\)' + t + r' \+ \(?(ENGINE_ListLen\(' + v + r'\))\)?\);',
                              r'\g<1>' + total + ' = ' + total + r' + \2;', text, flags=re.M)
                text = re.sub(r'^([ \t]*)' + t + r' = \(CCharString(?:_bv)?\)0x0;', r'\g<1>' + total + ' = 0;', text, flags=re.M)
                text = re.sub(r'\(float\)\(int\)' + t + r'\b', '(float)' + total, text)
                # the counter's life (its `= 0` up to the slot's next use as a string, `&slot`) as a local: the
                # lifter keeps stack-slot stores as temporaries and leaves a slot's self-update unlifted
                init = re.search(r'^[ \t]*' + t + r' = 0;', text, re.M)
                if init:
                    stop = re.search(r'&' + t + r'\b', text[init.start():])
                    cut = init.start() + stop.start() if stop else len(text)
                    text = text[:init.start()] + re.sub(r'\b' + t + r'\b', 'native_arg_kills_' + total, text[init.start():cut]) + text[cut:]
    return text


def lower_after_annotate(text, thing_slots=None):
    text = fold_name_compare(text)
    text = fold_inline_strncmp(text)
    text = fold_null_string_branches(text)
    text = fold_inline_string_equality(text)
    text = fold_by_value_thing_release(text)
    text = fold_thing_copy_from_pointer(text)
    # a no-operand thing method called through the stack thing's own vtable word (`(**(code **)(X._0_4_ + 0x12c))()`
    # = X.IsAlive()) on a slot the function uses as a CScriptThing (MagicBarrier's force-field handles, 2026-09-24)
    if thing_slots:
        things = (set(re.findall(r'\(CScriptThing \*\)(\w+)\b', text)) | set(re.findall(r'^[ \t]*undefined1 (\w+) \[12\];', text, re.M))
                  | set(re.findall(r'^[ \t]*(\w+) = QUESTTHING_\w+\(', text, re.M))
                  # a stack thing the export typed as such and some call fills (`&X` operand: a CreateCreature
                  # result -- WatchForSurprisingBalverines waits on `(**(code **)(xStack_24._0_4_ + 0x12c))()`, the
                  # surprise balverine's IsAlive). Never filled, the name has no Lua value (SickChild IngredientOwner's
                  # CStack_d8 is only ever read): that call stays unresolved rather than indexing a nil global
                  | {x for x in re.findall(r'^[ \t]*CScriptThing (\w+);', text, re.M) if re.search(r'&' + re.escape(x) + r'\b', text)})
        def own_vtable_call(m, scale=1):
            entry = thing_slots.get(int(m.group(2), 0) * scale)
            name = entry[0] if isinstance(entry, tuple) else entry
            if m.group(1) not in things or not name or not str(entry[1] if isinstance(entry, tuple) else '').endswith('XZ'):
                return m.group(0)
            return f'CScriptThing::{name}({m.group(1)})'
        text = re.sub(r'\(\*\*\(code \*\*\)\((\w+)\._0_4_ \+ (0x[0-9a-f]+)\)\)\(\)', own_vtable_call, text)
        # the vtable word indexed as an array of code pointers (`(*(code *)X[0x4b])()` = slot 0x12c, IsAlive: the
        # M_EndTheQuestHere / trader liveness checks in Trader Escort's WatchForMissionRules, 2026-09-24)
        text = re.sub(r'\(\*\(code \*\)(\w+)\[(0x[0-9a-f]+|\d+)\]\)\(\)', lambda m: own_vtable_call(m, 4), text)
    text = fold_char_flags(text)
    # `MsgGetThingsKilled(thing, &uidVector)` (retail vtable 0xDC: bool + a std::vector<ulong> the script frees):
    # the sidecar binding (2026-09-21) owns that vector and returns the count, so the out operand, its zeroing
    # and its `if (v != 0) free(v)` guard are the binding's business (TraderConflictGood::WatchForKilledPeople)
    for vec in set(re.findall(r'CScriptThing::MsgGetThingsKilled\([^,;]+,\s*&(\w+)\)', text)):
        v = re.escape(vec)
        if re.search(r'\w+ - (?:\(int\))?' + v + r' >> 2', text):
            continue        # the script reads the words: fold_things_killed_vectors (after the element fold)
        text = re.sub(r'(CScriptThing::MsgGetThingsKilled\([^,;]+),\s*&' + v + r'\)', r'\1)', text)
        text = re.sub(r'^[ \t]*' + v + r' = \(void \*\)0x0;[ \t]*\r?\n', '', text, flags=re.M)
        text = re.sub(r'^[ \t]*if \(' + v + r' != \(void \*\)0x0\) \{[ \t]*\r?\n[ \t]*free\(' + v + r'\);[ \t]*\r?\n[ \t]*\}[ \t]*\r?\n', '', text, flags=re.M)
        text = re.sub(r'^[ \t]*void \*' + v + r';[ \t]*\r?\n', '', text, flags=re.M)
    """Rewrites that need the GSI names: quest-side entity acquisition through a resource object."""
    # `M = RESOURCE_StartMovie("")` (the inlined movie object ctor) followed by `GSI->StartMovieSequence(&name, M)`:
    # retail makes ONE call, the object is its out-parameter. Emitting both started two sequences, and retail
    # StartMovieSequence (0x89B110) yields every frame while `GSI+0x2c` (a sequence is active) is set -- the second
    # call parked the coroutine for good (ApprenticeSpeedTest's boast, 2026-09-19). Drop the GSI call.
    # (the object operand may carry a pointer cast: Trader Escort's intro prints `(CScriptThing *)xStack_50`)
    for movie in set(re.findall(r'^[ \t]*(\w+) = RESOURCE_StartMovie\(', text, re.M)):
        text = re.sub(r'^[ \t]*GSI->StartMovieSequence\([^;,]+,\s*(?:\(\w+ \*\))?&?' + re.escape(movie) + r'\);[ \t]*\r?\n', '', text, flags=re.M)
    # the same call printed without its operands (an untyped decompile: TraderToRescue 0x00DFE0F0 prints
    # `(**(code **)(.. + 0x5c8))();`) right after the movie ctor (only the name's CCharString ctor between them)
    text = re.sub(r'^([ \t]*\w+ = RESOURCE_StartMovie\([^\n]*\n(?:[ \t]*CCharString::CCharString\([^\n]*\n)?)[ \t]*GSI->StartMovieSequence\(\);[ \t]*\r?\n',
                  r'\1', text, flags=re.M)
    def try_acquire(m):
        args = _split_top(m.group(3))          # (an operand may carry its own commas: `RESLIST_At(arr, i / 0x10)`)
        if len(args) != 3:
            return m.group(0)
        return f'{m.group(1)}{(m.group(2) + " = ") if m.group(2) else ""}RESOURCE_TryAcquire({_strip_addr(args[1])}, {args[0].strip()}, {args[2].strip()});'
    text = re.sub(r'^([ \t]*)(?:(\w+) = )?GSI->StartScriptingEntity\(([^;]+)\);', try_acquire, text, flags=re.M)
    # a thing returned straight into an outgoing by-value slot (`F((CScriptThing *)&stack0xNN, ...)` with no
    # result) that the decompiler then "reassembles" from unrelated registers: the temporary is the result
    text = RE_BV_THING_RESULT.sub(lambda m: f'{m.group(1)}{m.group(4)} = {m.group(2)}({m.group(3)});\n', text)
    text = fold_local_thing_vectors(text, thing_slots)
    text = fold_things_killed_vectors(text)
    # 0x00704580 (bsim-labelled CCountedPointer<CDiskFileWin32>::operator=) assigns a {Data, Info} counted pair:
    # release the old Info, copy both words, add-ref. Applied to a returned thing's +4 pair it is
    # CScriptThing::operator= (BanditKingMissionProcess 0x00D109F0 keeps CreateCreature's Twinblade in xStack_90
    # this way; left as a TODO the king was nil in every later call and the boss fight could not run)
    text = re.sub(r'^([ \t]*)CCountedPointer<CDiskFileWin32>::operator=__at704580\(\(CCountedPointer<CDiskFileWin32> \*\)&?(\w+),'
                  r'\s*\(int\)&\*\(int \*\)\((\w+) \+ 0x4\)\);',
                  r'\1\2 = (CScriptThing *)\3;', text, flags=re.M)
    # `GSI->DeregisterTimer(unaff_REG)`: Ghidra lost the register holding the id across the block; when the
    # function registers exactly one timer that is the id
    timers = re.findall(r'^[ \t]*(\w+) = (?:\(\w+\))?GSI->RegisterTimer\(\);', text, re.M)
    # several timers, one of them held in a register Ghidra lost (`unaff_EBX` in every Get/Set/DeregisterTimer that
    # names it) while exactly one registered id is never named by any timer call: they are the same timer
    # (TraderToRescue 0x00DFE0F0, four timers; `unaff_EBX` reached Lua as a free global)
    for reg in set(re.findall(r'GSI->\w*Timer\w*\((?:[^;()]*?,\s*)?(unaff_E[A-Z]{2})\b', text)):
        if any(re.search(r'\b' + reg + r'\b', line) and 'Timer' not in line and not re.match(r'[ \t]*\w+ ' + reg + r';', line)
               for line in text.splitlines()):
            continue        # the register carries something else too (its declaration does not count)
        unnamed = [t for t in dict.fromkeys(timers) if not re.search(r'GSI->\w*Timer\w*\((?:[^;()]*?,\s*)?(?:\(int\))?&?' + re.escape(t) + r'\b', text)]
        if len(unnamed) == 1:
            text = re.sub(r'(GSI->\w*Timer\w*\((?:[^;()]*?,\s*)?)' + reg + r'\b', r'\g<1>' + unnamed[0], text)
    if len(set(timers)) == 1:
        text = re.sub(r'GSI->DeregisterTimer\(unaff_E[A-Z]{2}\)', f'GSI->DeregisterTimer({timers[0]})', text)
        # the register holding the id is reused for the (meaningless) DeregisterTimer result and later values
        # (`iVar4 = GSI->DeregisterTimer(iVar4)`); a stack copy made right after registration is the stable id
        copy = re.search(r'^[ \t]*' + re.escape(timers[0]) + r' = (?:\(\w+\))?GSI->RegisterTimer\(\);[ \t]*\r?\n[ \t]*(\w+) = ' + re.escape(timers[0]) + r';', text, re.M)
        if copy and len(re.findall(r'^[ \t]*' + re.escape(copy.group(1)) + r' = ', text, re.M)) == 1:   # (a slot reused for other values is no stable id)
            text = re.sub(r'^([ \t]*)(?:' + re.escape(timers[0]) + r' = )?GSI->DeregisterTimer\((?:' + re.escape(timers[0]) + '|' + re.escape(copy.group(1)) + r')\);',
                          lambda m: f'{m.group(1)}GSI->DeregisterTimer({copy.group(1)});', text, flags=re.M)
            # every other timer call through the register reads the id the compiler reloads from that slot at
            # the loop head (AppleGirl: `mov ebx, [esp+0x10]` before GetTimer, which Ghidra folded into the
            # register name it also gave the conversation id)
            text = re.sub(r'GSI->(\w*Timer\w*)\(' + re.escape(timers[0]) + r'(?=[,)])', lambda m: f'GSI->{m.group(1)}({copy.group(1)}', text)
    elif timers:
        # several timers: each `X = RegisterTimer(); C = X;` pair routes the timer calls through X that follow it
        # in text order (until X registers the next one) to that stable slot C
        current = {}
        out = []
        for line in text.splitlines(keepends=True):
            reg = re.match(r'^[ \t]*(\w+) = (?:\(\w+\))?GSI->RegisterTimer\(\);', line)
            if reg:
                current.pop(reg.group(1), None)
                out.append(line)
                continue
            cp = re.match(r'^[ \t]*(\w+) = (\w+);[ \t]*\r?$', line)
            if cp and cp.group(2) in timers and out and re.match(r'^[ \t]*' + re.escape(cp.group(2)) + r' = (?:\(\w+\))?GSI->RegisterTimer\(\);', out[-1]) \
                    and len(re.findall(r'^[ \t]*' + re.escape(cp.group(1)) + r' = ', text, re.M)) == 1:
                current[cp.group(2)] = cp.group(1)
                out.append(line)
                continue
            for X, C in current.items():
                line = re.sub(r'GSI->(\w*Timer\w*)\(' + re.escape(X) + r'(?=[,)])', lambda m: f'GSI->{m.group(1)}({C}', line)
            out.append(line)
        text = ''.join(out)
    # several timers, a deregister whose operand is never assigned (a drifted slot name: `xStack_160` / `xStack_168` for
    # the timers at -0x164 / -0x16c, the export's per-site drift on TraderToRescue 0x00DFE0F0's epilogue): the still-open
    # timer whose slot number is nearest (within 8 bytes), when that choice is unique
    # (every registered timer is a candidate: another path may deregister the same timers under their true names)
    open_timers = list(dict.fromkeys(timers))
    def slot_of(name):
        m = re.fullmatch(r'\w*(?:Stack_|_stk_)([0-9a-f]+)(?:_\d+)?', name)
        return int(m.group(1), 16) if m else None
    def drifted_dereg(m):
        name = m.group(2)
        if name in timers or re.search(r'^[ \t]*' + re.escape(name) + r' = ', text, re.M):
            return m.group(0)
        s = slot_of(name)
        if s is None:
            return m.group(0)
        near = sorted((abs(slot_of(t) - s), t) for t in open_timers if t not in used and slot_of(t) is not None and abs(slot_of(t) - s) <= 8)
        if len(near) == 1 or (len(near) > 1 and near[0][0] < near[1][0]):
            used.add(near[0][1])             # (a destructor run names each timer once: the epilogue's second drifted name is the other timer)
            return f'{m.group(1)}GSI->DeregisterTimer({near[0][1]});'
        return m.group(0)
    used = set()
    if len(set(timers)) > 1:
        text = re.sub(r'^([ \t]*)GSI->DeregisterTimer\((?:\(int\))?&?(\w+)\);', drifted_dereg, text, flags=re.M)
    # the same for every create/destroy pair: a destroy operand that is never assigned in the function
    # (a drifted slot name) when the function creates exactly one object of that kind
    for creator, destroyer in (('GSI->RegisterTimer', 'GSI->DeregisterTimer'), ('RESOURCE_StartMovie', 'RESOURCE_DestroyMovie'),
                               ('RESOURCE_NewResource', 'RESOURCE_ReleaseResource'), ('ACTORMAP_New', 'ACTORMAP_Destroy'),
                               ('STRINGMAP_New', 'STRINGMAP_Destroy')):
        created = sorted(set(re.findall(r'^[ \t]*(\w+) = (?:\(\w+\))?' + re.escape(creator) + r'\(', text, re.M)))
        if len(created) != 1:
            continue
        def fix(m, created=created[0]):
            name = m.group(1)
            if name == created or re.search(r'^[ \t]*' + re.escape(name) + r' = ', text, re.M):
                return m.group(0)
            return m.group(0).replace(name, created, 1)
        text = re.sub(re.escape(destroyer) + r'\((\w+)\)', fix, text)
    # a by-value CScriptThing copy stores its GSI pointer field as four byte stores of the receiver alias
    # (`thing_b8 = (char)piVar1; thing_b9 = (char)((uint)piVar1 >> 8); ...`): construction, not script logic
    for alias in set(re.findall(r'^[ \t]*(\w+) = \*\(int \*\*\)\(this \+ (?:4|0x40)\);', text, re.M)):
        a = re.escape(alias)
        text = re.sub(r'^[ \t]*[\w.]+ = \((?:char|undefined1)\)' + a + r';[ \t]*\r?\n(?:[ \t]*[\w.]+ = \((?:char|undefined1)\)\(\(uint\)' + a + r' >> (?:0x)?[0-9a-f]+\);[ \t]*\r?\n){3}', '', text, flags=re.M)
    # a local actor map's inlined destructor after its stack slots were canonicalised to one name
    # (`if (M != 0) { StdMap_DestroyNode(M, ...); ... M = 0; } if (M != 0) { free(M); }`)
    text = re.sub(r'^([ \t]*)if \((\w+) != (?:\([\w ]+\*\))?0(?:x0)?\) \{[ \t]*\r?\n[ \t]*StdMap_DestroyNode\(&?\2,[^;\n]*\);[ \t]*\r?\n(?:[ \t]*[^\n]*;[ \t]*\r?\n){0,4}?[ \t]*\}[ \t]*\r?\n'
                  r'(?:[ \t]*if \(\2 != (?:\([\w ]+\*\))?0(?:x0)?\) \{[ \t]*\r?\n[ \t]*free\(\2\);[ \t]*\r?\n[ \t]*\}[ \t]*\r?\n)?',
                  lambda m: f'{m.group(1)}ACTORMAP_Destroy({m.group(2)});\n', text, flags=re.M)
    # an element receiver named by the vector folds repeated as the explicit `this` operand
    text = re.sub(r'(CScriptThing::\w+\(((?:LOCAL|QUEST|ENTITY)LIST_At\w*\([^()]*\)), ?)(?:\(int \*\))?\2(?:,\s*|(?=\)))', r'\1', text)
    # receiver aliases (`this_00 = *(int **)(this + 0x40);`) are dead once their vcalls read `GSI->`
    text = drop_dead_local_stores(text)
    # position members through a pointer the annotation pass has only now named (`X = GetPos(Y)`)
    text = fold_position_reads(text)
    text = fold_actor_map_releases(text)
    return text


RE_VEC_DTOR_LOOP = re.compile(
    r'^[ \t]*for \(; (?:\w+ = \w+, )?(\w+) != (\w+); \1 = \1 \+ 3\) \{\s*\r?\n(?:[ \t]*\w+ = \w+;[ \t]*\r?\n)?'
    r'[ \t]*(?:\w+ = )?\(\*\*\(code \*\*\)\*\1\)\(0\);[ \t]*\r?\n(?:[ \t]*\w+ = \w+;[ \t]*\r?\n){0,2}[ \t]*\}[ \t]*\r?\n', re.M)
RE_VEC_FREE = re.compile(
    r'^[ \t]*if \((\w+) != \(undefined4 \*\)0x0\) \{\s*\r?\n[ \t]*free\(\1\);[ \t]*\r?\n(?:[ \t]*\w+ = \w+;[ \t]*\r?\n)*[ \t]*\}[ \t]*\r?\n', re.M)


def canonicalise_split_array_vectors(text):
    '''Ghidra sometimes types a local std::vector<CScriptThing> as one 12-byte stack array and spells its
    begin/end as sub-fields (`auStack_54._0_4_` / `auStack_54._4_4_`, capacity `uStack_4c` on its own) and
    passes the array bare to the filling GSI call (`GSI->GetAllThingsWithScriptName(&name, auStack_54)`).
    Respell it as the three-slot form the vector folds already read: the array name is the begin
    (`(int)auStack_54 + i` element bytes, `(int)auStack_54 - auStack_54` the byte count), the `_4_4_`
    end reads become the begin-slot spelling too, and the zeroed capacity slot (8 bytes above) vanishes
    (RunTutorials' `AppleMarker` loop, 0x00D45DD0).'''
    for arr in sorted(set(re.findall(r'\b(\w*[Ss]tack_[0-9a-f]+)\._0_4_\b', text))):
        if not re.search(r'GSI->GetAllThings\w+\([^;]*?\b' + re.escape(arr) + r'\);', text):
            continue
        a = re.escape(arr)
        # construction: begin/end zeroed through the sub-fields, capacity zeroed under its own slot name
        text = re.sub(r'^[ \t]*' + a + r'\._0_4_ = (?:\([\w ]+\*+\))?0(?:x0)?;[ \t]*\r?\n', f'    {arr} = (undefined4 *)0x0;\n', text, flags=re.M)
        text = re.sub(r'^[ \t]*' + a + r'\._4_4_ = (?:\([\w ]+\*+\))?0(?:x0)?;[ \t]*\r?\n', '', text, flags=re.M)
        slot = int(re.search(r'_([0-9a-f]+)$', arr).group(1), 16)
        cap = r'\w+_(?:stk_)?%x' % (slot - 8)
        if len(re.findall(r'\b' + cap + r'\b', text)) == 1:
            text = re.sub(r'^[ \t]*' + cap + r' = (?:\([\w ]+\*+\))?0(?:x0)?;[ \t]*\r?\n', '', text, flags=re.M)
        # the byte count `(int)(END - BEGIN)` -> `(int)V - V`; element bytes `(BEGIN + i)` -> `((int)V + i)`
        # (parenthesised, the spelling the fold's count-compare pattern reads; the sign-fix temporary
        # `iVar = (int)(END - BEGIN) >> 0x1f;` goes here, since the fold's own pattern expects it unparenthesised)
        text = re.sub(r'^[ \t]*(\w+) = \(int\)\(' + a + r'\._4_4_ - ' + a + r'\._0_4_\) >> 0x1f;[ \t]*\r?\n', '', text, flags=re.M)
        text = re.sub(r'\(int\)\(' + a + r'\._4_4_ - ' + a + r'\._0_4_\)', f'((int){arr} - {arr})', text)
        text = re.sub(r'\(\(int\)' + a + r' - ' + a + r'\) / 0xc \+ (\w+) (!=|==) \1\b', lambda m, arr=arr: f'LOCALLIST_Count({arr}) {m.group(2)} 0', text)
        # (a thing vcall through the element's Data pointer reads `*(int *)(V + i)`, the elem pass's spelling)
        text = re.sub(r'\*\(int \*\)\(' + a + r'\._0_4_ \+ (\w+)\)', lambda m, arr=arr: f'*(int *)({arr} + {m.group(1)})', text)
        text = re.sub(r'\(' + a + r'\._0_4_ \+ (\w+)\)', lambda m, arr=arr: f'((int){arr} + {m.group(1)})', text)
        text = re.sub(r'\b' + a + r'\._0_4_\b', arr, text)
    return text


RESLIST_RESIZE = {0xCD3CCB}    # CArray<resource>::push_back(n, template): n copies of an unacquired resource (WoodsWill's bandits)
RESLIST_DTOR = {0xCD319B}      # the array's destructor: every element released, the buffer freed


def fold_local_resource_arrays(text, call_labels):
    """A local dynamic array of resource objects (16 bytes each; `pvStack_2c = 0; uStack_28 = 0; uStack_24 = 0;`
    then `tmpl = resource ctor(&stackTmpl); push_back(&arr, COUNT, tmpl);` -- GuildTrainingWoodsWill 0x00D67890 holds
    one resource per WillBandit): the Lua list `arr = RESLIST_New(COUNT)`; an element at a byte offset
    (`(void *)((int)pvStack_2c + iVar12)` / `pvStack_2c + 0x10` / the bare begin pointer) is `RESLIST_At(arr, off / 0x10)`,
    the destructor releases every element. Runs before constant propagation turns the zeroed begin pointer into
    `(0x0 + iVar12)`. The stack template's own ctor/release stay (an unacquired resource, harmless)."""
    resize = [l for l, t in call_labels.items() if t in RESLIST_RESIZE]
    dtor = [l for l, t in call_labels.items() if t in RESLIST_DTOR]
    if not resize:
        return text
    arrays = []
    ctors = [l for l, t in call_labels.items() if t in RESOURCE_CTOR]
    for label in resize:
        pat = re.compile(r'^([ \t]*)(?:\w+ = )?' + re.escape(label).replace('::', r'\s*::\s*') + r'\s*\(\([^()]*\*\)&(\w+),\s*([^,;]+),\s*(\w+)\);[ \t]*\r?\n', re.M)
        def new(m):
            arrays.append(m.group(2))
            return f'{m.group(1)}{m.group(2)} = RESLIST_New({m.group(3).strip()});\n'
        for m in list(pat.finditer(text))[::-1]:
            # the template: `iVar5 = <resource ctor>(aCStack_10);` right above -- a stack resource of its own
            # (constructed, never acquired, released later with the rest), spelled as one
            head = text[:m.start()]
            for ctor in ctors:
                head = re.sub(r'^([ \t]*)' + re.escape(m.group(4)) + r' = ' + re.escape(ctor).replace('::', r'\s*::\s*') + r'\s*\(&?(\w+)\);[ \t]*\r?\n\Z',
                              lambda mm: f'{mm.group(1)}{mm.group(2)} = RESOURCE_NewResource();\n', head, flags=re.M)
            text = head + new(m) + text[m.end():]
    for arr in arrays:
        a = re.escape(arr)
        # the zeroed begin / end / capacity slots are the array's own construction
        slot = int(arr.rsplit('_', 1)[1], 16)
        text = re.sub(r'^[ \t]*' + a + r' = \(void \*\)0x0;[ \t]*\r?\n', '', text, flags=re.M)
        for k in (4, 8):
            text = re.sub(r'^[ \t]*\w+Stack_%x = 0;[ \t]*\r?\n' % (slot - k), '', text, flags=re.M)
        text = re.sub(r'\(void \*\)\(\(int\)' + a + r' \+ (0x[0-9a-f]+|\d+)\)', lambda m: f'RESLIST_At({arr}, {int(m.group(1), 0) // 16})', text)
        text = re.sub(r'\(void \*\)\(\(int\)' + a + r' \+ (\w+)\)', lambda m: f'RESLIST_At({arr}, ({m.group(1)}) / 0x10)', text)
        text = re.sub(r'^([ \t]*)(\w+) = ' + a + r';', lambda m: f'{m.group(1)}{m.group(2)} = RESLIST_At({arr}, 0);', text, flags=re.M)
        for label in dtor:
            text = re.sub(r'^([ \t]*)' + re.escape(label).replace('::', r'\s*::\s*') + r'\s*\(\([^()]*\*\)&' + a + r'\);', lambda m: f'{m.group(1)}RESLIST_Destroy({arr});', text, flags=re.M)
        text = re.sub(r'^[ \t]*void \*' + a + r';[ \t]*\r?\n', '', text, flags=re.M)
    return text


VECTOR_SLOT_NAME = r'(?:\w*[Ss]tack_[0-9a-f]+|\w+_stk_[0-9a-f]+)'


def fold_vector_register_aliases(text, vec):
    """Ghidra's loop-carried register copies of a local vector's end pointer: `puVar1 = E; while (E = puVar1,
    puVar2 = V, !done) { ... (int)puVar1 - (int)V ... puVar2 = E; puVar1 = V; ... }`. Retail re-reads both slots
    from the stack at every size computation (TraderComment Main 0x00E05410 / 0x00E054B4: `mov edx,[esp+0x1c]`
    begin, `mov ecx,[esp+0x20]` end); the registers only feed the vector's destructor. Taken literally the copy
    back `E = puVar1` would set end = begin after the first pass and the size would read a nil register. When a
    register R is assigned only from E and V, and E is assigned only from such registers (or zero), E never
    changes: the size reads E and the write-backs vanish."""
    v = re.escape(vec)
    def sources(name):
        # statement and comma-expression assignments (`X = Y;`, `(X = Y, ...`, `, X = Y,`)
        return [s.strip() for s in re.findall(r'(?<![\w.>*])' + re.escape(name) + r' = ((?:\([\w ]+\*?\))?\w+)\s*[;,)]', text)]
    def bare(s):
        return re.sub(r'^\([\w ]+\*?\)', '', s)
    for reg in sorted(set(re.findall(r'\(int\)(\w*Var\d+) - \(int\)' + v + r'\b', text))):
        srcs = {bare(s) for s in sources(reg)}
        ends = srcs - {vec}
        if len(ends) != 1:
            continue
        end = ends.pop()
        if not re.fullmatch(VECTOR_SLOT_NAME, end) or re.search(r'&' + re.escape(end) + r'\b', text):
            continue
        aliases = [r for r in set(re.findall(r'(?<![\w.>*])(\w*Var\d+) = (?:\([\w ]+\*?\))?' + re.escape(end) + r'\b', text))]
        if not all({bare(s) for s in sources(a)} <= {end, vec} for a in aliases):
            continue
        written = {bare(s) for s in sources(end)}
        if not written <= set(aliases) | {'0', '0x0'}:
            continue
        text = re.sub(r'\(int\)' + re.escape(reg) + r' - \(int\)' + v + r'\b', f'(int){end} - (int){vec}', text)
        for a in aliases:
            text = re.sub(r'^[ \t]*' + re.escape(end) + r' = (?:\([\w ]+\*?\))?' + re.escape(a) + r';[ \t]*\r?\n', '', text, flags=re.M)
            text = re.sub(r'(?<![\w.>*])' + re.escape(end) + r' = (?:\([\w ]+\*?\))?' + re.escape(a) + r',\s*', '', text)
    return text


def fold_local_thing_vectors(text, thing_slots=None):
    '''A local std::vector<CScriptThing> filled by a GSI `GetAllThings*` slot: the Lua binding returns a table.
    `n = GSI->GetAllThingsWithDefName(&name,&vec);` -> `vec = GSI->...(&name); n = LOCALLIST_Count(vec);`,
    `(CScriptThing *)((int)vec + byteOffset)` -> `LOCALLIST_At(vec, byteOffset / 0xc)`; the element destructor
    loops (`for (; p != end; p += 3) (**(code **)*p)(0);`), `free(begin)` and the zeroed begin/end/capacity
    slots are the vector's own lifetime and vanish.'''
    text = canonicalise_split_array_vectors(text)
    # a pointer temporary to the out-slot (`pOutFollowers = &xStack_c;`) is the slot itself, up to the
    # register's next definition
    for m in reversed(list(re.finditer(r'^[ \t]*(\w+) = &(\w+);[ \t]*\r?\n', text, re.M))):
        var = m.group(1)
        tail = text[m.end():]
        nxt = re.search(r'^[ \t]*' + re.escape(var) + r' = ', tail, re.M)
        scope, rest = (tail[:nxt.start()], tail[nxt.start():]) if nxt else (tail, '')
        scope2 = re.sub(r'(GSI->\w+\([^;]*?,)' + re.escape(var) + r'\)', lambda mm, s=m.group(2): f'{mm.group(1)}&{s})', scope)
        if scope2 != scope and not re.search(r'\b' + re.escape(var) + r'\b', scope2):
            text = text[:m.start()] + scope2 + rest
    # an interface call filling a member list through its out-argument returns the table
    text = re.sub(r'^([ \t]*)GSI->(\w+)\(((?:[^;()]*?),\s*)?(QUEST|ENTITY)LIST_Ref\("(\w+)"\)\);',
                  lambda m: f'{m.group(1)}lst_{m.group(5)} = GSI->{m.group(2)}({(m.group(3) or "").rstrip().rstrip(",")});\n{m.group(1)}{m.group(4)}LIST_Set("{m.group(5)}", lst_{m.group(5)});', text, flags=re.M)
    vectors = []
    # any other interface call whose last operand is a zero-constructed local vector (`V = (undefined4 *)0x0;`)
    constructed = set(re.findall(r'^[ \t]*(\w+) = \(undefined4 \*\)0x0;', text, re.M))
    # an `int`-typed begin slot (`iStack_c = 0;`) whose end slot 4 bytes above is zeroed too
    for m in re.finditer(r'^[ \t]*(\w*[Ss]tack_|\w+_stk_)([0-9a-f]+) = 0;', text, re.M):
        slot = int(m.group(2), 16)
        if re.search(r'^[ \t]*\w+_(?:stk_)?%x = 0;' % (slot - 4), text, re.M):
            constructed.add(m.group(1) + m.group(2))
    def call(m):
        if not m.group(3).startswith('GetAllThings') and m.group(5) not in constructed:
            return m.group(0)
        vectors.append(m.group(5))
        return f'{m.group(1)}{m.group(5)} = GSI->{m.group(3)}({m.group(4) or ""});\n{m.group(1)}{m.group(2)} = LOCALLIST_Count({m.group(5)});'
    # the leading operands are optional: `GetAllCreaturesExcludingHero(&vec)` has none
    text = re.sub(r'^([ \t]*)(\w+) = (?:\(\w+\))?GSI->(\w+)\((?:([^;]*?),)?&?(\w+)\);', call, text, flags=re.M)
    # the void spelling (the count is computed from the begin/end slots afterwards)
    def call_void(m):
        if not m.group(2).startswith('GetAllThings') and m.group(4) not in constructed:
            return m.group(0)
        vectors.append(m.group(4))
        return f'{m.group(1)}{m.group(4)} = GSI->{m.group(2)}({m.group(3) or ""});'
    text = re.sub(r'^([ \t]*)GSI->(\w+)\((?:([^;]*?),)?&?(\w+)\);', call_void, text, flags=re.M)
    # the by-value spelling (Ghidra recognised the hidden return slot): `vec = GSI->GetAllThings...(&name);`
    for m in re.finditer(r'^[ \t]*(\w+) = (?:GSI->GetAllThings\w+\([^;,]*\)|(?:QUEST|ENTITY)LIST_Copy\("\w+"\));', text, flags=re.M):
        vectors.append(m.group(1))
    if not vectors:
        return text
    def thing_call_local(receiver, slot_hex, next_char):
        slot = (thing_slots or {}).get(int(slot_hex, 0))
        if not slot:
            return None
        return f'CScriptThing::{slot[0]}({receiver}{", " if next_char not in (")", "") else ""}'
    elems = 0
    for vec in vectors:
        v = re.escape(vec)
        # the zero-initialised begin/end/capacity slots before the call are the vector's construction
        text = re.sub(r'^[ \t]*' + v + r' = (?:\([\w ]+\*?\))?0(?:x0)?;[ \t]*\r?\n', '', text, flags=re.M)
        text = re.sub(r'\(CScriptThing(?:_bv)? \*\)\(\(int\)' + v + r' \+ (\w+)\)', lambda m, vec=vec: f'LOCALLIST_At({vec}, ({m.group(1)}) / 0xc)', text)
        # pointer arithmetic on the `undefined4 *` begin: `V + (int)i * 3` (3 dwords = one CScriptThing), `(int)V + i` (bytes)
        text = re.sub(r'\(\(CScriptThing(?:_bv)? \*\)\(' + v + r' \+ \(int\)(\w+) \* 3\)\)', lambda m, vec=vec: f'LOCALLIST_At({vec}, {m.group(1)})', text)   # parenthesised
        text = re.sub(r'\(CScriptThing(?:_bv)? \*\)\(' + v + r' \+ \(int\)(\w+) \* 3\)', lambda m, vec=vec: f'LOCALLIST_At({vec}, {m.group(1)})', text)
        text = re.sub(r'\(' + v + r' \+ \(int\)(\w+) \* 3\)', lambda m, vec=vec: f'LOCALLIST_At({vec}, {m.group(1)})', text)
        text = re.sub(r'\(\(int\)' + v + r' \+ (\w+)\)', lambda m, vec=vec: f'LOCALLIST_At({vec}, ({m.group(1)}) / 0xc)', text)
        text = re.sub(r'\(CScriptThing(?:_bv)? \*\)\((\w+) \+ \(int\)' + v + r'\)', lambda m, vec=vec: f'LOCALLIST_At({vec}, ({m.group(1)}) / 0xc)', text)
        # ... the same with the thing cast already stripped (TraderComment Main 0x00E05469: the distance test's
        # operand `(iVar5 + (int)xStack_1c)`, `lea edx, [esi + eax]` = begin + byte offset)
        # (a bare call operand only: under a dereference the same sum is an element's Data pointer counted from
        # the +4 field -- TraderConflictEvil's `(**(**(int **)(iVar5 + (int)xStack_c) + 0x138))(` IsEqualTo)
        text = re.sub(r'(?:(?<=\()|(?<=, )|(?<=,))\((\w+) \+ \(int\)' + v + r'\)(?=\s*[,)])',
                      lambda m, vec=vec: f'LOCALLIST_At({vec}, ({m.group(1)}) / 0xc)', text)
        # the begin slot typed `int` by Ghidra: `(CScriptThing *)(V + i)` with a NAMED byte counter (`iVar9 += 0xc`)
        # is the element at i / 0xc (RunTutorials 0x00D45DD0's leftover-apple RemoveThing loop lifted to
        # `appleRed01 + scratchValue4`, arithmetic on a table, 2026-09-20 audit); a literal `V + k` stays element k
        text = re.sub(r'\(CScriptThing(?:_bv)? \*\)\(' + v + r' \+ ([A-Za-z_]\w*)\)', lambda m, vec=vec: f'LOCALLIST_At({vec}, ({m.group(1)}) / 0xc)', text)
        # The bare name is the vector only while the vector is live: from its fill to its destructor / free.
        # Outside that range the same slot is whatever else Ghidra put there -- RunTutorials 0x00D45DD0 reuses
        # the AppleMarker vector's slot -0x54 as the hidden by-value result of 15 GetThingWithScriptName calls
        # (before AND after the vector's life) and as the acquired thing of a TryAcquire; rewritten to
        # LOCALLIST_At the lifter lost the result slot, shed the real string operand as an "extra" and
        # back-filled from the LIFO pool, rotating the literals of that call and the two after it
        # (`CreateObject("PreMeleeDummy", GetThingWithScriptName("OBJECT_STRAW_DUMMY_01"):GetPos(), ...)`), and
        # the acquire read `theRealGuildmaster[0 + 1]` (2026-09-20). A fill spelled by value with no visible
        # destructor keeps the whole-text behaviour.
        live = []
        for fm in re.finditer(r'^[ \t]*(?:' + v + r' = (?:\(\w+\))?GSI->\w+\(|GSI->\w+\([^;\n]*?&' + v + r'\);|' + v + r' = (?:QUEST|ENTITY)LIST_Copy\()', text, re.M):
            endm = re.search(r'~vector[^\n]*\b' + v + r'\)|^[ \t]*free\(' + v + r'\);', text[fm.end():], re.M)
            live.append((fm.start(), fm.end() + endm.end() if endm else len(text)))
        def in_live(pos):
            return not live or any(a <= pos < b for a, b in live)
        text = re.sub(r'\(CScriptThing \*\)' + v + r'\b',
                      lambda m, vec=vec: f'LOCALLIST_At({vec}, 0)' if in_live(m.start()) else m.group(0), text)
        # the begin pointer itself (Ghidra typed it `CScriptThing *`) as a call operand is element 0, `V + k` element k
        def element(m):
            if not in_live(m.start()):
                return m.group(0)
            before = text[max(0, m.end(1) - 16):m.end(1)]     # up to and including the `(` / `,` before the name
            if re.search(r'(?:LOCALLIST_(?:Count|At)|free)\($', before):
                return m.group(0)
            return f'{m.group(1)}LOCALLIST_At({vec}, {m.group(2) or 0})'
        text = re.sub(r'([(,]\s*)' + v + r'(?: \+ (\d+))?(?=\s*[,)])', element, text)
        text = fold_vector_register_aliases(text, vec)
        # the end-pointer slot 4 bytes above the begin slot under its own Ghidra name (`pu_stk_20` for `xStack_24`)
        slot = re.search(r'_(?:stk_)?([0-9a-f]+)$', vec)
        # (restore_stack_operands may have respelled the begin by its true slot -- `xStack_84` for Ghidra's
        # `puStack_94` -- while the end kept Ghidra's numbering, `pu_stk_90`: a stack name that appears only
        # in the `(int)E - (int)V` idiom and is never assigned is that end pointer, whatever its number)
        for e in set(re.findall(r'\(int\)(\w+_(?:stk_)?[0-9a-f]+) - \(int\)' + v + r'\b', text)):
            if not re.search(r'^[ \t]*(?:\([\w ]+\*+\))?' + re.escape(e) + r' = (?!(?:\([\w ]+\*?\))?0(?:x0)?;)', text, re.M) and not re.search(r'&' + re.escape(e) + r'\b', text):
                text = re.sub(r'\(int\)' + re.escape(e) + r' - \(int\)' + v + r'\b', f'(int){vec} - {vec}', text)
                text = re.sub(r'^[ \t]*' + re.escape(e) + r' = (?:\([\w ]+\*?\))?0(?:x0)?;[ \t]*\r?\n', '', text, flags=re.M)
        # the uncast spelling with the end under a drifted number that is NOT begin-4 (`i_stk_8c - xStack_8c`,
        # PreMeleeWhisper 0x00D524A0: Ghidra gave the end the begin's own number): a stack name whose only
        # assignments are zero (the vector's construction) and that is otherwise read only in `E - V` is the end
        # (either side may carry an `(int)` cast: `(int)x_stk_60 - iStack_68`, BirdKiller 0x00D4DEA0's marker list)
        for e in set(re.findall(r'(?<![\w)])(?:\(int\))?(\w+_(?:stk_)?[0-9a-f]+) - (?:\(int\))?' + v + r'\b', text)):
            if e == vec:
                continue
            assigned = re.findall(r'^[ \t]*(?:\([\w ]+\*+\))?' + re.escape(e) + r' = ([^;]+);', text, re.M)
            # (never assigned at all -- its zeroing already folded away under the drifted name -- counts too)
            if all(re.fullmatch(r'(?:\([\w ]+\*?\))?0(?:x0)?', a.strip()) for a in assigned) and not re.search(r'&' + re.escape(e) + r'\b', text):
                text = re.sub(r'(?<![\w)])(?:\(int\))?' + re.escape(e) + r' - (?:\(int\))?' + v + r'\b', f'(int){vec} - {vec}', text)
                text = re.sub(r'^[ \t]*' + re.escape(e) + r' = (?:\([\w ]+\*?\))?0(?:x0)?;[ \t]*\r?\n', '', text, flags=re.M)
        if slot:
            end_slot = int(slot.group(1), 16) - 4
            text = re.sub(r'\(int\)\w+_(?:stk_)?%x - \(int\)%s\b' % (end_slot, vec), f'(int){vec} - {vec}', text)
            text = re.sub(r'(?<![\w)])(?:\(int\))?\w+_(?:stk_)?%x - (?:\(int\))?%s\b' % (end_slot, vec), f'(int){vec} - {vec}', text)   # uncast spelling `(i_stk_c0 - iStack_c4) / 0xc`
            # the zeroed end slot before the by-value call is the vector's construction too
            text = re.sub(r'^[ \t]*\w+_(?:stk_)?%x = (?:\([\w ]+\*?\))?0(?:x0)?;[ \t]*\r?\n' % end_slot, '', text, flags=re.M)
            # an element-pointer walk (`p = V; while (p = p + 3, p != END) { ... p[1] ... erase(&V, p) }`): an index
            end_names = r'(?:\w+_(?:stk_)?%x|\(int\)%s|%s)' % (end_slot, vec, vec)
            for it in set(re.findall(r'^[ \t]*(\w+) = ' + v + r';', text, re.M)):
                if not re.search(r'\b' + it + r' = ' + it + r' \+ 3\b', text):
                    continue
                text = re.sub(r'^([ \t]*)' + it + r' = ' + v + r';', r'\1' + it + ' = 0;', text, flags=re.M)
                text = re.sub(r'\b' + it + r' = ' + it + r' \+ 3\b', f'{it} = {it} + 1', text)
                text = re.sub(r'\b' + it + r' ([!=]=) (?:\(int \*\))?' + end_names, lambda m, it=it: f'{it} {m.group(1)} LOCALLIST_Count({vec})', text)
                text = re.sub(r'\b' + it + r'\[1\]', f'LOCALLIST_At({vec}, {it})', text)
                # the begin pointer itself against the end: the vector is not empty
                text = re.sub(r'(?<![\w)])' + v + r' ([!=]=) (?:\(int \*\))?' + end_names, lambda m, vec=vec: f'LOCALLIST_Count({vec}) {m.group(1)} 0', text)
        # erase(iterator) on the local vector: `std::vector::erase(&V, it)` (label 0xCD3152 under any name)
        text = re.sub(r'^([ \t]*)\w+\(&' + v + r',\s*(?:\(int\))?(\w+)\);', lambda m, vec=vec: f'{m.group(1)}LOCALLIST_Erase({vec}, {m.group(2)});', text, flags=re.M)
        # an element's Data pointer by index (`V[(int)i * 3 + 1]`) is the element; a vcall through it a thing call
        text = re.sub(r'\(int \*\)' + v + r'\[\(int\)(\w+) \* 3 \+ 1\]|' + v + r'\[\(int\)(\w+) \* 3 \+ 1\]',
                      lambda m, vec=vec: f'LOCALLIST_At({vec}, {m.group(1) or m.group(2)})', text)
        def at_vcall_local(m, vec=vec):
            out = thing_call_local(f'LOCALLIST_At({vec}, {m.group(1)})', m.group(2), text[m.end():m.end() + 1])
            return out or m.group(0)
        # (the index is a name, or the byte offset already spelled `(i) / 0xc` by the rewrites above --
        # CheckFriendlyAttacks 0x00D45060: four `GetDefName` vcalls on `((int)V + i)` elements stayed
        # native and the def-name compares lifted to `nil == "CREATURE_..."`, 2026-09-20)
        text = re.sub(r'\(\*\*\(code \*\*\)\(\*(?:\(int \*\))?LOCALLIST_At\(' + v + r', (\w+|\(\w+\) / 0xc)\) \+ (0x[0-9a-f]+|\d+)\)\)\s*\(', at_vcall_local, text)
        # ... the element's vtable read straight off the begin pointer by index (`V[i * 3]`, three dwords per
        # thing), with or without the explicit `this` operand `V + i * 3` (WatchForSurprisingBalverines 0x00E0619F:
        # `(**(code **)(V[uVar10 * 3] + 0x18))(V + uVar10 * 3)` = GetPos of element i, the CreateCreature position)
        def at_index_vcall(m, vec=vec):
            out = thing_call_local(f'LOCALLIST_At({vec}, {m.group(1)})', m.group(2), ')')
            return out or m.group(0)
        text = re.sub(r'\(\*\*\(code \*\*\)\(' + v + r'\[(?:\(int\))?(\w+) \* 3\] \+ (0x[0-9a-f]+|\d+)\)\)\s*\((?:' + v + r' \+ (?:\(int\))?\1 \* 3(?=\s*[,)]))?',
                      at_index_vcall, text)
        # element count from the begin/end slots (both canonicalised to the vector's name):
        # `iVar = (int)V - V >> 0x1f;` (sign fix) then `((int)V - V) / 0xc + iVar != iVar` (count != 0)
        text = re.sub(r'^[ \t]*(\w+) = \(int\)' + v + r' - ' + v + r' >> 0x1f;[ \t]*\r?\n', '', text, flags=re.M)
        text = re.sub(r'\(\(int\)' + v + r' - ' + v + r'\) / 0xc \+ (\w+) (!=|==) \1\b', lambda m, vec=vec: f'LOCALLIST_Count({vec}) {m.group(2)} 0', text)
        text = re.sub(r'\(int\)' + v + r' - ' + v + r'\b', f'LOCALLIST_Count({vec}) * 0xc', text)
        # the uncast self-difference: the end slot reached the fold already under the vector's own name
        # (WatchForMissionRules 0x00E06440: iStack_b0/iStack_ac/uStack_a8 all respelled `xStack_b0`, the slot's
        # earlier actor map); a vector minus itself can only be its end minus its begin
        text = re.sub(r'\(' + v + r' - ' + v + r'\) / 0xc', f'LOCALLIST_Count({vec})', text)
        text = re.sub(r'\(LOCALLIST_Count\(' + v + r'\) \* 0xc\) / 0xc', f'LOCALLIST_Count({vec})', text)
        # a thing vcall through an element's Data pointer (byte index counted from the +4 field:
        # `iVar5 = 4; ... (**(code **)(**(int **)(iVar5 + (int)V) + OFF))(`)
        def data_vcall(m, vec=vec):
            out = thing_call_local(f'LOCALLIST_At({vec}, ({m.group(1)} - 4) / 0xc)', m.group(2), text[m.end():m.end() + 1])
            return out or m.group(0)
        text = re.sub(r'\(\*\*\(code \*\*\)\(\*\*\(int \*\*\)\((\w+) \+ (?:\(int\))?' + v + r'\) \+ (0x[0-9a-f]+|\d+)\)\)\s*\(', data_vcall, text)   # (`(int)` absent on an int-typed begin slot: TraderConflictGood's follower loop)
        # ... the same printed without its casts (`(**(**(iVar3 + iStack_c) + 0x138))(`: TraderConflictGood's
        # follower-vs-AllCreatures IsEqualTo loop 0x00DFC0xx, an untyped `int` begin slot)
        text = re.sub(r'\(\*\*\(\*\*\((\w+) \+ ' + v + r'\) \+ (0x[0-9a-f]+|\d+)\)\)\s*\(', data_vcall, text)
        # a thing vcall on an element addressed by byte offset: name the element first
        # (the cast is absent on an int-typed begin slot: PreMeleeWhisper 0x00D524A0's `GetDataString` on each
        # PreMeleeChatMarker, `(**(code **)(*(int *)(iVar10 + iStack_90) + 0xc))(`, 2026-09-21)
        text = re.sub(r'\*\(int \*\)\((\w+) \+ (?:\(int\))?' + v + r'\)', lambda m, vec=vec: f'*(int *)({vec} + {m.group(1)})', text)   # `(i + (int)V)` -> `(V + i)`
        pat = re.compile(r'^([ \t]*)([^\n]*?)\(\*\*\(code \*\*\)\(\*\(int \*\)\(' + v + r' \+ (?:\(int\))?(\w+)\) \+ (0x[0-9a-f]+|\d+)\)\)\(', re.M)
        def elem(m):
            nonlocal elems
            elems += 1
            name = f'elem_{elems}'
            slot = int(m.group(4), 0)
            method = (thing_slots or {}).get(slot)
            head = f'{m.group(1)}{name} = LOCALLIST_At({vec}, ({m.group(3)}) / 0xc);\n'
            if method:
                return head + f'{m.group(1)}{m.group(2)}CScriptThing::{method[0]}({name}, '
            return head + f'{m.group(1)}{m.group(2)}(**(code **)(*(int *){name} + {m.group(4)}))('
        text = pat.sub(elem, text)
    # element destructor calls through vtable slot 0 (`(**(code **)*p)(0)`), the storage free, the zeroed
    # begin/end/capacity slots; then loops / guards left with only pointer bookkeeping
    text = re.sub(r'^[ \t]*(?:\w+ = )?\(\*\*\(code \*\*\)\*\w+\)\(0?\);[ \t]*\r?\n', '', text, flags=re.M)
    text = re.sub(r'^[ \t]*free\(\w+\);[ \t]*\r?\n', '', text, flags=re.M)
    # the begin pointer used as the base of a count / an index is the vector, not element 0
    text = re.sub(r'LOCALLIST_Count\(LOCALLIST_At\((\w+), 0\)\)', r'LOCALLIST_Count(\1)', text)
    text = re.sub(r'LOCALLIST_At\(LOCALLIST_At\((\w+), 0\), ', r'LOCALLIST_At(\1, ', text)
    text = re.sub(r'^[ \t]*free\(LOCALLIST_At\(\w+, 0\)\);[ \t]*\r?\n', '', text, flags=re.M)
    # storage-pointer bookkeeping around the frees (`puVar6 = pu_stk_20;` / `pu_stk_14 = puVar6;`)
    text = re.sub(r'^[ \t]*(?:\w+ = pu_stk_\w+|pu_stk_\w+ = \w+);[ \t]*\r?\n', '', text, flags=re.M)
    text = re.sub(r'^[ \t]*\w+ = \(undefined4 \*\)0x0;[ \t]*\r?\n', '', text, flags=re.M)
    junk = r'(?:[ \t]*\w+ = (?:\w+|\w+ \+ -?\d+|\(undefined4 \*\)0x0);[ \t]*\r?\n)*'   # (`end = end + -3`: VC7.1's clear() walks back from the end, WoodsWill 0x00D67890)
    for _ in range(3):
        text = re.sub(r'^[ \t]*for \((?:;|\w+ = \w+;)[^\n]*\) \{[ \t]*\r?\n' + junk + r'[ \t]*\}[ \t]*\r?\n', '', text, flags=re.M)
        text = re.sub(r'^[ \t]*do \{[ \t]*\r?\n' + junk + r'[ \t]*\} while \(\w+ != \w+\);[ \t]*\r?\n', '', text, flags=re.M)
        text = re.sub(r'^[ \t]*if \(\w+ != (?:\(undefined4 \*\)0x0|\w+)\) \{[ \t]*\r?\n' + junk + r'[ \t]*\}[ \t]*\r?\n', '', text, flags=re.M)
        # a guard left with nothing on either side (the terminating / normal destructor paths)
        text = re.sub(r'^[ \t]*if \([^\n]*\) \{[ \t]*\r?\n[ \t]*\}[ \t]*\r?\n(?:[ \t]*else \{[ \t]*\r?\n[ \t]*\}[ \t]*\r?\n)?', '', text, flags=re.M)
    # element-walk pointer copies of the vector (`puVar1 = V;`) that nothing reads any more
    for vec in vectors:
        for m in list(re.finditer(r'^[ \t]*(\w+) = ' + re.escape(vec) + r';[ \t]*\r?\n', text, re.M)):
            var = m.group(1)
            if not re.search(r'\b' + re.escape(var) + r'\b', re.sub(r'^[ \t]*[\w ]+ \**' + re.escape(var) + r';[ \t]*\r?\n|^[ \t]*' + re.escape(var) + r' = ' + re.escape(vec) + r';[ \t]*\r?\n', '', text, flags=re.M)):
                text = re.sub(r'^[ \t]*' + re.escape(var) + r' = ' + re.escape(vec) + r';[ \t]*\r?\n', '', text, flags=re.M)
    return preserve_repeated_script_name_fills(text)


def preserve_repeated_script_name_fills(text: str) -> str:
    """Consecutive native script-name queries append to their output vector.

    Retail 0x008A8570 pushes at [out+4], advances it by 12, and never clears
    the destination; 0x008ACD30 reserves capacity while preserving its size.
    Only combine fills in one straight-line block with no intervening use of
    the vector (other than reading its count). Other lifetimes stay untouched.
    """
    pattern = re.compile(r'^([ \t]*)(\w+) = GSI->GetAllThingsWithScriptName\(([^;\n]*)\);', re.M)
    previous, edits = {}, []
    for m in pattern.finditer(text):
        vec = m[2]
        prev = previous.get(vec)
        if prev is not None:
            between = text[prev.end():m.start()]
            between = re.sub(r'LOCALLIST_Count\(' + re.escape(vec) + r'\)', '', between)
            if not re.search(r'[{}]|\b(?:goto|return)\b|LAB_\w+:|\b' + re.escape(vec) + r'\b', between):
                added = f'append_{vec}_{len(edits)}'
                edits.append((m.start(), m.end(),
                    f'{m[1]}{added} = GSI->GetAllThingsWithScriptName({m[3]});\n'
                    f'{m[1]}LOCALLIST_Append({vec}, {added});'))
        previous[vec] = m
    for start, end, replacement in reversed(edits):
        text = text[:start] + replacement + text[end:]
    return text


class LoweringSpec:
    def __init__(self, unit, owner, *, entity, thing_slots):
        self.entity = entity
        self.thing_slots = thing_slots or {}
        q = unit['quest']
        self.parent_fields = {int(k, 16): tuple(v) for k, v in q['fields'].items()}
        self.parent_things = {int(k, 16): v for k, v in q.get('thingFields', {}).items()}
        self.parent_arrays = [dict(a, base=int(a['base'], 16), members={int(k, 16): tuple(v) for k, v in a['members'].items()},
                                   things={int(k, 16): v for k, v in a['things'].items()},
                                   pointers={int(k, 16): v for k, v in a.get('pointers', {}).items()}) for a in q.get('arrays', [])]
        self.parent_lists = {int(x['offset'], 16): x['name'] for x in q.get('unmappedFields', [])
                             if x['type'].startswith('vector<CScriptThing')}
        master = unit.get('master', {})
        self.master_fields = {int(k, 16): tuple(v) for k, v in master.get('fields', {}).items()}
        if entity:
            e = unit['entities'][owner]
            self.self_fields = {int(k, 16): tuple(v) for k, v in e['fields'].items()}
            self.self_things = {int(k, 16): v for k, v in e.get('thingFields', {}).items()}
            self.self_arrays = []
            self.self_lists = {int(x['offset'], 16): x['name'] for x in e.get('unmappedFields', [])
                               if x['type'].startswith('vector<CScriptThing')}
            self.self_pointers = self._array_pointers(e.get('unmappedFields', []), self.parent_arrays)
            self.parent_off, self.master_off = 0x14, 0x18
        else:
            self.self_fields, self.self_things = self.parent_fields, self.parent_things
            self.self_arrays, self.self_lists, self.self_pointers = self.parent_arrays, self.parent_lists, {}
            self.parent_off, self.master_off = None, 0x44
        self.diagnostics = []

    @staticmethod
    def _array_pointers(unmapped, arrays):
        out = {}
        for x in unmapped:
            m = re.fullmatch(r'(\w+) \*', x['type'])
            if not m:
                continue
            for a in arrays:
                if a.get('element') == m.group(1):
                    out[int(x['offset'], 16)] = {'name': x['name'], 'array': a}
        return out


def _next_use_is_redefinition(text, pos, var):
    """Whether the next mention of `var` after `pos` is an assignment to it at a line start (or there is none):
    a definition before `pos` is then dead."""
    nxt = re.search(r'\b' + re.escape(var) + r'\b', text[pos:])
    if not nxt:
        return True
    at_line_start = text[pos:pos + nxt.start()].rsplit('\n', 1)[-1].strip() == ''
    return bool(at_line_start and re.match(r'\s*=(?!=)', text[pos + nxt.end():]))


def lower_global_definition_strings(text, call_labels):
    """Retail 415D70 constructs a CString in its hidden result from a CDefString token.

    Keep a runtime table read: decoded script.bin values are evidence, not constants.
    Only the exact callee and a global-definition field with a named output slot qualify.
    """
    for label, target in call_labels.items():
        if target != 0x415D70:
            continue
        head = re.escape(label).replace('::', r'\s*::\s*')
        pattern = re.compile(
            r'(?<![\w:])' + head + r'\s*\(\s*\(CDefString \*\)\(DAT_0143e90c \+ '
            r'(0x[0-9a-f]+|\d+)\),\s*(?:\(int\)|\(CCharString \*\))?&(\w+)\s*\)\s*;')
        text = pattern.sub(lambda m: f'{m[2]} = ENGINE_GlobalGameDataString({m[1]});', text)
    return text


def lower(source: str, spec: LoweringSpec) -> tuple[str, list[str]]:
    diag = []
    text = source
    # 0. member functions: Ghidra's untyped `param_1` is `this`; typed pointer derefs of the parent
    text = re.sub(r'\*\((?:C\w+Script) \*\*\)\((' + SELF + r') \+ 0x14\)', r'*(int *)(\1 + 0x14)', text)
    text = re.sub(r'\*\((?:C\w+MasterData) \*\*\)\((' + SELF + r') \+ (0x18|0x44)\)', r'*(int *)(\1 + \2)', text)
    text = re.sub(r'\bparam_1\b', 'this', text)
    text = normalise_typed_decompile(text)
    text = fold_local_resource_arrays(text, getattr(spec, 'call_labels', None) or {})
    text = resolve_stack_offset_names(text)
    for m in set(re.findall(r'^[ \t]*(\w+) = \*\(int \*\*\)\(this \+ (4|0x40)\);', text, re.M)):
        text = re.sub(r'^([ \t]*)(\w+) = \*' + re.escape(m[0]) + r';', lambda mm, off=m[1]: f'{mm.group(1)}{mm.group(2)} = **(int **)(this + {off});', text, flags=re.M)
    text = fold_by_value_things(text, getattr(spec, 'code_range', None))
    if spec.entity:
        # DarkwoodAssassinSpawn 0xE02E50 keeps its own thing address in a
        # register typed CCharString by a later lifetime. The entity layout,
        # not that register type, identifies the explicit this+8 address.
        text = re.sub(r'\(CCharString\)\(this \+ 8\)',
                      '(CScriptThing *)(this + 8)', text)
        text = resolve_me_register_uses(text)
    text = isolate_gsi_vtable_temps(text)
    text = fold_tangled_thing_assign(text)
    text = fold_low_byte_flags(text)
    text = fold_dword_colours(text)
    text = fold_stack_colours(text)
    # a literal byte store left after the colour folding is an ordinary flag byte of a merged slot
    text = re.sub(r'\b([A-Za-z]+Stack_[0-9a-f]+(?:_\d+)?)\._(\d+)_1_(?= = (?:0x[0-9a-f]+|\d+);)', r'\1_b\2', text)
    if spec.entity:
        # the entity's own CScriptThing lives at this+8: its Data pointer (this+0xc) passed as an argument is `me`
        text = re.sub(r'\*\(int \*\)\(this \+ (?:0xc|12)\)', '(CScriptThing *)(this + 8)', text)
    # vcall through a thing's Data pointer: (**(code **)(**(int **)(E + OFF+4) + SLOT))( == vcall on the thing at E + OFF
    text = re.sub(r'\(\*\*\(code \*\*\)\(\*\*\(int \*\*\)\(([^;()]*?(?:\([^;()]*\)[^;()]*?)*) \+ (0x[0-9a-f]+|\d+)\) \+ (0x[0-9a-f]+|\d+)\)\)\s*\((?:\*\(int \*\*\)\(\1 \+ \2\)(?:,\s*)?)?',
                  lambda m: m.group(0) if (m.group(1).endswith('this') and int(m.group(2), 0) in (4, 0x40)) or (m.group(1).endswith('0x14)') and int(m.group(2), 0) == 0x40)
                  else f'(**(code **)(*(int *)({m.group(1)} + {hex(int(m.group(2), 0) - 4)}) + {m.group(3)}))(', text)
    text = fold_counted_pointer_assign(text)
    text = fold_counted_pointer_release(text)
    # `CCountedPointer<..>::operator=(&P, &thing.Data)` keeps a handle on the thing: P is the thing; the
    # Data-pointer vcalls / null tests on P are thing vcalls / validity tests
    handles = set()
    data_fields = {}   # Ghidra's own name for a stack thing's Data field -> the thing's slot name
    def handle(m):
        if re.search(r'\(CScriptThing(?:_bv)? \*\)&?' + re.escape(m.group(2)) + r'\b', text):
            # the slot is a CScriptThing object (hidden-return target): assigning its Data is the thing copy
            return f'{m.group(1)}CScriptThing::operator=((CScriptThing *)&{m.group(2)},(int){m.group(3)});'
        # P named by its own slot 4 bytes above a stack CScriptThing that is a call operand (`piStack_14` beside
        # `(CScriptThing *)xStack_18`, ScorpionHome 0x00D643A0): P is that thing's Data field, so the assignment
        # is the thing copy and every read of P is `thing._4_4_` (validity tests / vcalls on the thing)
        slot = re.fullmatch(r'\w*Stack_([0-9a-f]+)', m.group(2))
        if slot:
            obj = re.search(r'\(CScriptThing(?:_bv)? \*\)&?(\w*Stack_' + format(int(slot.group(1), 16) + 4, 'x') + r')\b', text)
            if obj:
                data_fields[m.group(2)] = obj.group(1)
                return f'{m.group(1)}CScriptThing::operator=((CScriptThing *)&{obj.group(1)},(int){m.group(3)});'
        handles.add(m.group(2))
        return f'{m.group(1)}{m.group(2)} = {m.group(3)};'
    # the same copy spelled on an array-typed stack thing's Data field (`(auStack_160 + 4)`, the Guildmaster's XP orb
    # 0x00D52E90: the orb comes back through a hidden-result slot and is copied into the thing the wait loop polls)
    text = re.sub(r'^([ \t]*)CCountedPointer<\w+>::operator=\s*\(\(CCountedPointer<\w+> \*\)\((\w*Stack_[0-9a-f]+) \+ (?:4|0x4)\),\s*\(int\)&\*\(int \*\)\((\w+) \+ (?:4|0x4)\)\);',
                  lambda m: f'{m.group(1)}CScriptThing::operator=((CScriptThing *){m.group(2)},(int){m.group(3)});', text, flags=re.M)
    text = re.sub(r'^([ \t]*)CCountedPointer<\w+>::operator=\s*\(\(CCountedPointer<\w+> \*\)&(\w+),\s*\(int\)&\*\(int \*\)\((\w+) \+ (?:4|0x4)\)\);', handle, text, flags=re.M)
    for field, obj in data_fields.items():
        text = re.sub(r'^[ \t]*int \*' + re.escape(field) + r';[ \t]*\r?\n', '', text, flags=re.M)
        text = re.sub(r'(?<![\w.])' + re.escape(field) + r'\b', f'{obj}._4_4_', text)
    for h in handles:
        text = re.sub(r'\(\*\*\(code \*\*\)\(\*' + re.escape(h) + r' \+ (0x[0-9a-f]+|\d+)\)\)\(', r'(**(code **)(*(int *)' + h + r' + \1))(', text)
        text = re.sub(r'\b' + re.escape(h) + r' == \(int \*\)0x0\b', f'!__thing_valid({h})', text)
        text = re.sub(r'\b' + re.escape(h) + r' != \(int \*\)0x0\b', f'__thing_valid({h})', text)
    text = name_offset_objects(text, getattr(spec, 'call_labels', {}))
    # bool __fastcall AreAllThingsInVectorDead(const vector<CScriptThing>&) 0xCBED00 (disassembly 2026-09-20: counts the
    # elements that are !IsAlive() (slot 0x12c) or IsUnconscious() (0xf4), true when the count is the size) on a quest
    # list member -- no FSE binding; expanded inline by finish_lua (TraderConflictGood WatchForTradersFreed 0x00DFCC10
    # read `this + 72` as an undefined global, third audit)
    for label, target in getattr(spec, 'call_labels', {}).items():
        if target == 0xCBED00:
            lists = getattr(spec, 'parent_lists', {})
            text = re.sub(re.escape(label) + r'\s*\(\s*(?:\(void \*\))?\(\s*(?:\(int\))?this \+ (0x[0-9a-f]+|\d+)\s*\)\s*\)',
                          lambda m: f'ENGINE_IsAllDead(QUESTLIST_Copy("{lists[int(m.group(1), 0)]}"))' if int(m.group(1), 0) in lists else m.group(0), text)
            # the same predicate on a LOCAL vector: the lifted local is already a Lua list
            # (WaspBoss Main 0x00E0EA40 polls its HornetDrone / WaspChaser / WaspAttacker lookups)
            text = re.sub(re.escape(label) + r'\s*\(\s*&((?:[A-Za-z]+Stack_|local_)[0-9a-f]+)\s*\)',
                          lambda m: f'ENGINE_IsAllDead({m.group(1)})', text)
    text = fold_byte_split_pointers(text)
    text = fold_byte_literal_words(text)
    text = fold_actor_maps(text, getattr(spec, 'resolve_string', None))
    text = fold_resource_objects(text, getattr(spec, 'call_labels', {}))
    # an actor-map store spelled by address whose object is a resource (`pCVar19 = xStack_20` after the ctor) is
    # the resource itself, not a CScriptThing address (CheckFriendlyAttacks' "HERO" actor lifted as a TODO
    # `&xStack_20` and the BADHERO cutscene ran without the hero, 2026-09-20)
    for res in set(re.findall(r'\b(\w+) = RESOURCE_(?:NewResource|StartMovie)\(', text)):
        text = re.sub(r'(ACTORMAP_Set\(\w+, (?:"[^"]*"|\w+), )&' + re.escape(res) + r'\)', r'\1' + res + ')', text)
    text = drop_trivial_base_calls(text, getattr(spec, 'call_labels', {}), getattr(spec, 'byte_at', None))
    text = hoist_object_aliases(fold_inline_constructors(text))
    text = drop_noop_comma_assignments(text)
    text = fold_inline_destructors(text)
    text = canonicalise_stack_objects(text)
    text = drop_member_zero_inits(text)  # the object's member zeroes now share its name
    text = fold_inline_destructors(text)    # again: the canonical names may only now agree across the three lines
    text = reconcile_destructor_kinds(text)
    # A resource (CScriptGameResourceObjectScriptedThingBase) is modelled on the base sub-object its acquire /
    # reset / IsAcquired calls take (`ecx = -216`), but the derived object starts one slot below and that is what
    # its destructor gets (`ecx = -220`, GuildTrainingMelee MeleeOpponent 0x00D56ECF): a release of a slot that
    # holds no resource while the slot one word up does releases THAT resource (the lift read `ReleaseResource("")`
    # -- the dead `""` temporary that lived at -220 earlier -- third audit 2026-09-20)
    resources = set(re.findall(r'^[ 	]*(\w*Stack_[0-9a-f]+) = RESOURCE_NewResource\(\)', text, re.M))
    def release_head(m):
        slot = m.group(2)
        if slot in resources:
            return m.group(0)
        prefix, num = re.match(r'(\w*Stack_)([0-9a-f]+)$', slot).groups()
        up = f'{prefix}{int(num, 16) - 4:x}'
        return f'{m.group(1)}RESOURCE_ReleaseResource({up});' if up in resources else m.group(0)
    text = re.sub(r'^([ 	]*)RESOURCE_ReleaseResource\((\w*Stack_[0-9a-f]+)\);', release_head, text, flags=re.M)
    text = fold_sibling_slot_offsets(text)
    text = fold_offset_string_temporaries(text)
    text = fold_engine_helpers(text, getattr(spec, 'call_labels', {}))
    float_at = getattr(spec, 'float_at', None)
    if float_at:
        def dat_float(m):
            value = float_at(int(m.group(2), 16))
            return (m.group(1) or '') + (repr(value) if value is not None else m.group(0))
        text = re.sub(r'(\(float(?:10)?(?: \*)?\))?_?DAT_([0-9a-f]{6,8})\b(?![\w(])', lambda m: dat_float(m) if m.group(1) else m.group(0), text)
        text = re.sub(r'^([ \t]*f\w+ = )_?DAT_([0-9a-f]{6,8});', lambda m: m.group(1) + (repr(float_at(int(m.group(2), 16))) if float_at(int(m.group(2), 16)) is not None else '_DAT_' + m.group(2)) + ';', text, flags=re.M)
        # `_DAT_x` (an unnamed data item read as a value) beside a float comparison is that float constant
        def dat_value(m):
            value = float_at(int(m.group(1), 16))
            return repr(value) if value is not None and abs(value) < 1e6 else m.group(0)
        text = re.sub(r'\b_DAT_([0-9a-f]{8})\b(?=\s*(?:[<>=!]=?|\)|,|;))', dat_value, text)
        # ... and as a factor of a float product (`fVar5 * _DAT_0126b7dc + 0.5`, BanditExtra 0x00DFCA90's rounded distance)
        text = re.sub(r'(\bf\w+ [*/] )_DAT_([0-9a-f]{8})\b', lambda m: m.group(1) + (repr(float_at(int(m.group(2), 16))) if float_at(int(m.group(2), 16)) is not None else '_DAT_' + m.group(2)), text)
    # Ghidra's ROUND (x87 round-to-nearest) in an integer conversion: Lua `math.floor(x + 0.5)`
    text = re.sub(r'\bROUND\(', 'ENGINE_Round(', text)
    byte_at = getattr(spec, 'byte_at', None)
    if byte_at:
        # `DAT_x` passed as a call argument where the bytes are a 0/1 fill: a bool constant
        def dat_bool(m):
            raw = bytes(byte_at(int(m.group(2), 16) + k) for k in range(4))
            return m.group(1) + ('true' if raw == b'\x01\x01\x01\x01' else 'false' if raw == b'\x00\x00\x00\x00' else m.group(0)[len(m.group(1)):])
        text = re.sub(r'([,(]\s*)(?:\((?:u?int|byte|bool|char|undefined\d?)\))?DAT_([0-9a-f]{8})\b(?=\s*[,)])', dat_bool, text)
    resolve_wide = getattr(spec, 'resolve_wide', None)
    if resolve_wide:
        # CCharString::AssignFromWide(&local, L"...") on a stack string: the same as constructing it from
        # the UTF-16 .rdata literal (the fail-reason messages of SetQuestAsFailed)
        def assign_wide_local(m):
            literal = resolve_wide(int(m.group(3), 16))
            if literal is None:
                return m.group(0)
            literal = literal.replace('\\', '\\\\').replace('"', '\\"')
            return f'{m.group(1)}CCharString::CCharString((CCharString *)&{m.group(2)},"{literal}",-1);'
        text = re.sub(r'^([ \t]*)CCharString(?:::|__)AssignFromWide\((?:\(CCharString \*\))?&(\w+),\s*(0x[0-9a-f]+)\);', assign_wide_local, text, flags=re.M)
    resolve = getattr(spec, 'resolve_string', None)
    if resolve:
        # `&DAT_xxxxxxxx` string addresses (the empty string and other pooled literals) -> literals
        def dat_literal(m):
            literal = resolve(int(m.group(1), 16))
            if literal is None and getattr(spec, 'byte_at', None) and spec.byte_at(int(m.group(1), 16)) == 0:
                literal = ''   # the pooled empty string (a lone NUL) is not a "string" to the resolver
            return '"' + literal.replace('\\', '\\\\').replace('"', '\\"') + '"' if literal is not None else m.group(0)
        text = re.sub(r'&DAT_([0-9a-f]{8})\b', dat_literal, text)
    # reads from the global game-data table (runtime pointer at DAT_0143e90c): keep the offset; a local
    # holding the pointer (`iVar14 = DAT_0143e90c;`) stands for it until the register is reused
    table_registers = set(re.findall(r'^[ \t]*(\w+) = (?:\(\w+ \*+\))?DAT_0143e90c;', text, re.M))   # (before the scoped pass removes the alias lines)
    pos = 0
    while (m := re.search(r'^[ \t]*(\w+) = (?:\(\w+ \*+\))?DAT_0143e90c;[ \t]*\r?\n', text[pos:], re.M)):
        var = m.group(1)
        head, tail = text[:pos + m.start()], text[pos + m.end():]
        nxt = re.search(r'(?<![\w.>])' + re.escape(var) + r' = (?!=)', tail)
        scope, rest = (tail[:nxt.start()], tail[nxt.start():]) if nxt else (tail, '')
        scope = re.sub(r'(?<![\w.>])' + re.escape(var) + r'\b', 'DAT_0143e90c', scope)
        text = head + scope + rest
        pos = len(head)
    # a float array the table points at (`pf = *(float **)(table + N); ... *pf ...; pf = pf + 1;`): the
    # pointer becomes (offset, running index)
    pos = 0
    while (m := re.search(r'^([ \t]*)(\w+) = \*\(float \*\*\)\(DAT_0143e90c \+ (0x[0-9a-f]+|\d+)\);[ \t]*\r?\n', text[pos:], re.M)):
        var, off = m.group(2), m.group(3)
        idx = 'ix' + var[2:] if var.startswith('pf') else var + '_i'      # (a `<prefix>VarN` name the lifter treats as a local)
        head = text[:pos + m.start()] + f'{m.group(1)}{idx} = 0;\n'
        tail = text[pos + m.end():]
        nxt = re.search(r'^[ \t]*' + re.escape(var) + r' = (?!' + re.escape(var) + r' \+ )', tail, re.M)
        scope, rest = (tail[:nxt.start()], tail[nxt.start():]) if nxt else (tail, '')
        scope = re.sub(r'^([ \t]*)' + re.escape(var) + r' = ' + re.escape(var) + r' \+ (\d+);', r'\1' + idx + ' = ' + idx + r' + \2;', scope, flags=re.M)
        scope = re.sub(r'\*' + re.escape(var) + r'\b', f'ENGINE_GlobalGameDataFloatAt({off}, {idx})', scope)
        scope = re.sub(r'\b' + re.escape(var) + r'\[(\w+)\]', lambda mm: f'ENGINE_GlobalGameDataFloatAt({off}, {idx} + {mm.group(1)})', scope)
        text = head + scope + rest
        pos = len(head)
    # a register that held the table pointer on another path and is dereferenced with a table offset
    # after an intervening reuse (`iVar6 = ENGINE_Trunc(..)` in a sibling branch): the deref is a table read
    # (the alias lines are gone by now -- `table_registers` was collected first; ArtifactThief 0x00D62480's second
    # `iVar6 = DAT_0143e90c` scope ended at a reload before its `*(float *)(iVar6 + 0xf14)` read)
    for var in table_registers:
        text = re.sub(r'\*\((float|int|undefined4|uint) \*\)\(' + re.escape(var) + r' \+ (0x[0-9a-f]{3,}|\d{3,})\)',
                      r'*(\1 *)(DAT_0143e90c + \2)', text)
    # an int field the export typed as a string slot: `X = *(CCharString *)(DAT_0143e90c + OFF);` whose next use
    # is `(int)X` (DarkwoodAssassinSpawn Main's spawn distance 0xe14; left alone the distance test read nil)
    for m in list(re.finditer(r'^([ \t]*)(\w+) = \*\(CCharString(?:_bv)? \*\)\(DAT_0143e90c \+ (0x[0-9a-f]+|\d+)\);', text, re.M)):
        var = re.escape(m.group(2))
        at = text.find(m.group(0))
        tail = text[at + len(m.group(0)):]
        use = re.search(r'(\(int\))?(?<![\w&])' + var + r'\b', tail)
        if not use or not use.group(1):
            continue
        end = re.search(r'&' + var + r'\b|^[ \t]*' + var + r' = ', tail, re.M)
        scope, rest = (tail[:end.start()], tail[end.start():]) if end else (tail, '')
        text = (text[:at] + f'{m.group(1)}{m.group(2)} = ENGINE_GlobalGameData({m.group(3)});'
                + re.sub(r'\(int\)' + var + r'\b', m.group(2), scope) + rest)
    text = lower_global_definition_strings(text, getattr(spec, 'call_labels', {}))
    text = re.sub(r'\*\(float \*\)\(DAT_0143e90c \+ (0x[0-9a-f]+|\d+)\)', r'ENGINE_GlobalGameDataFloat(\1)', text)
    text = re.sub(r'\*\((?:int|undefined4|uint) \*\)\(DAT_0143e90c \+ (0x[0-9a-f]+|\d+)\)', r'ENGINE_GlobalGameData(\1)', text)
    text = fold_position_reads(text)
    text = fold_signed_pow2_division(text)
    # CRT truncation of an x87 value (`__ftol2((float10)x)`, typed with its ST0 operand by the export)
    text = re.sub(r'\b__ftol2\(\s*(?:\(float10\))?', 'ENGINE_Trunc(', text)
    # inlined `CScriptThing::GetDataString()` into a hidden-result slot: the empty global string (DAT_0143e8ec)
    # when the thing has no Data, else the Data vcall (whose result operand Ghidra dropped)
    text = re.sub(r'CCharString::CCharString\((?:\(CCharString \*\))?&(\w+),\(CCharString(?:_bv)? \*\)&DAT_0143e8ec\);', r'\1 = ENGINE_EmptyString();', text)
    # a float constant loaded into an integer register (`iVar7 = 0x3f800000;` then pushed as a float operand)
    text = re.sub(r'= (0x(?:3[a-f]|4[0-9a]|b[a-f]|c[0-9a])[0-9a-f]{6});', lambda m: f'= {struct.unpack("<f", struct.pack("<I", int(m.group(1), 16)))[0]!r};', text)
    # CRT `rand()` (MSVCR71; Ghidra prints the stale registers as operands): 0..RAND_MAX
    text = re.sub(r'(?<![\w:])rand\((?:[^()]|\([^()]*\))*\)', 'ENGINE_Rand()', text)
    # a by-value string operand cast on its stack slot (`(CCharString *)&xStack_2c`) is the slot
    text = re.sub(r'\((?:CCharString(?:_bv)?) \*\)&(\w*Stack_\w+)\b', r'&\1', text)

    parent = r'\*\(int \*\)\(this \+ 0x14\)'
    # 1. alias locals for parent / master pointers, substituted in place (assignment removed)
    def inline_alias(text, base_pattern, kinds):
        alias_re = re.compile(r'^[ \t]*(\w+) = (?:\([\w ]+\*\))?' + base_pattern + r';[ \t]*\r?\n', re.M)
        pos = 0
        while (m := alias_re.search(text, pos)):
            var = m.group(1)
            raw = m.group(0).split('= ', 1)[1].rstrip().rstrip(';').strip()
            value = raw if raw.startswith('*(') else '(' + raw + ')'
            head, tail = text[:m.start()], text[m.end():]
            # the alias holds the pointer until the register is reused for something else: substitute only
            # after the assignment (the declaration block keeps `int iVarN;`) and only up to that reuse
            nxt = re.search(r'(?<![\w.>])' + re.escape(var) + r' = (?!=)', tail)   # also inside a comma expression
            scope, rest = (tail[:nxt.start()], tail[nxt.start():]) if nxt else (tail, '')
            scope = re.sub(r'(?<![\w.>])' + re.escape(var) + r'\b', lambda _: value, scope)
            text = head + scope + rest
            pos = m.start()
        return text
    if spec.entity:
        # the parent pointer re-loaded in a loop condition (`while (iVar7 = *(int *)(this + 0x14), COND)`): the pointer
        # never changes, so the load hoists in front of the loop
        text = re.sub(r'^([ \t]*)while \((\w+) = (' + parent + r'), ', r'\1\2 = \3;\n\1while (', text, flags=re.M)
        text = inline_alias(text, parent, 'parent')
    text = inline_alias(text, r'\*\(int \*\)\(this \+ ' + off_re(spec.master_off) + r'\)', 'master')
    if spec.entity:
        text = inline_alias(text, r'\*\(int \*\)\(' + parent + r' \+ 0x44\)', 'parent master')
    text = text.replace('((*(int *)(this + 0x14)))', '(*(int *)(this + 0x14))')

    # 2. master data (self, or parent's for entities)
    master_bases = [r'\*\(int \*\)\(this \+ ' + off_re(spec.master_off) + r'\)', r'\(\*\(int \*\)\(this \+ ' + off_re(spec.master_off) + r'\)\)']
    if spec.entity:
        master_bases += [r'\*\(int \*\)\(' + parent + r' \+ 0x44\)', r'\(\*\(int \*\)\(' + parent + r' \+ 0x44\)\)']
    # 2a. a register holding the ADDRESS of a master field (`piVar1 = (int *)(MASTER + 0xa4);` then
    # `*piVar1 = *piVar1 + iVar5;` -- VC7.1's read-modify-write of one field, SkillTarget's SkillScore
    # 0x00D41D00): every `*piVar1` up to the register's next assignment is that field's dereference,
    # so the store/load folds below see the plain form (the TODO(native) residue was the archery score)
    def inline_field_pointer(text, base_pattern):
        alias_re = re.compile(r'^[ \t]*(\w+) = \((' + TYPE + r') \*\)\(' + base_pattern + r' \+ (0x[0-9a-f]+|\d)\);[ \t]*\r?\n', re.M)
        pos = 0
        while (m := alias_re.search(text, pos)):
            var, kind, off = m.group(1), m.group(2), m.group(3)
            base_text = m.group(0).split('= ', 1)[1].strip().rstrip(';')
            base_text = base_text[base_text.index(')(') + 2:].rsplit(' + ', 1)[0]      # the base as printed
            value = f'*({kind} *)({base_text} + {off})'
            head, tail = text[:m.start()], text[m.end():]
            nxt = re.search(r'(?<![\w.>*])' + re.escape(var) + r' = (?!=)', tail)     # `*piVar1 = ..` is a store, not a reuse
            scope, rest = (tail[:nxt.start()], tail[nxt.start():]) if nxt else (tail, '')
            scope = re.sub(r'\*' + re.escape(var) + r'\b', lambda _: value, scope)
            text = head + scope + rest
            pos = m.start()
        return text
    for base in master_bases:
        text = inline_field_pointer(text, base)
    # (a single-digit member offset prints in decimal: `+ 4` is PostSavePosition, the Gameflow campaign stage)
    for base in master_bases:
        def master_store(m):
            off = int(m.group(2), 0)
            f = spec.master_fields.get(off)
            if not f:
                return m.group(0)
            return f'{m.group(1)}GSI->SetMasterGameState("{f[0]}", {_lit(m.group(3), f[1])});'
        text = re.sub(r'^([ \t]*)\*\(' + TYPE + r' \*\)\(' + base + r' \+ (0x[0-9a-f]+|\d)\) =\s*([^;]+);', master_store, text, flags=re.M)
        def master_load(m):
            off = int(m.group(1), 0)
            f = spec.master_fields.get(off)
            return f'GSI->GetMasterGameState("{f[0]}")' if f else m.group(0)
        text = re.sub(r'\*\(' + TYPE + r' \*\)\(' + base + r' \+ (0x[0-9a-f]+|\d)\)', master_load, text)

    # 3. bases for field families: (regex, fields, things, arrays, lists, receiver tag)
    families = []
    if spec.entity:
        families.append((parent, spec.parent_things, spec.parent_arrays, spec.parent_lists, 'QUEST'))
        families.append((r'this', spec.self_things, spec.self_arrays, spec.self_lists, 'ENTITY'))
    else:
        families.append((r'this', spec.self_things, spec.self_arrays, spec.self_lists, 'QUEST'))

    def thing_call(receiver_expr, slot_hex, rest_start, text_after):
        slot = spec.thing_slots.get(int(slot_hex, 0))
        if not slot:
            return None
        tail = ', ' if text_after[:1] not in (')', '') else ''
        return f'CScriptThing::{slot[0]}({receiver_expr}{tail}'

    for base, things, arrays, lists, tag in families:
        # 3a. struct arrays with dynamic index: *(T *)(IDX * STRIDE + BASE+MEMBER + PARENTBASE)
        for a in arrays:
            stride = a['stride']
            for member_off, (mname, kind) in a['members'].items():
                absolute = a['base'] + member_off
                key = f'__key("{a["name"]}_" .. {{idx}} .. "_{mname}")' if mname else f'__key("{a["name"]}_" .. {{idx}})'
                idx_form = r'(?P<idx>\*\(int \*\)\(this \+ 0x[0-9a-f]+\)|(?:QUEST|ENTITY)STATE_GetInt\(\"[^\"]*\"\)|\w+) \* ' + off_re(stride)
                pat_store = re.compile(r'^([ \t]*)\*\(' + TYPE + r' \*\)\(' + idx_form + r' \+ ' + off_re(absolute) + r' \+ ' + base + r'\) =\s*([^;]+);', re.M)
                text = pat_store.sub(lambda m, k=key, kind=kind, tag=tag: f'{m.group(1)}QUESTSTATE_Set{kind}({k.format(idx=m.group("idx"))}, {_lit(m.group(3), kind)});', text)
                pat_load = re.compile(r'\*\(' + TYPE + r' \*\)\(' + idx_form + r' \+ ' + off_re(absolute) + r' \+ ' + base + r'\)')
                text = pat_load.sub(lambda m, k=key, kind=kind, tag=tag: f'{tag}STATE_Get{kind}({k.format(idx=m.group("idx"))})', text)
                if kind == 'String':
                    for pat in (r'\((?:CWideString|CCharString) \*\)\(' + base + r' \+ ' + idx_form + r' \+ ' + off_re(absolute) + r'\)',
                                r'\((?:CWideString|CCharString) \*\)\(' + idx_form + r' \+ ' + off_re(absolute) + r' \+ ' + base + r'\)'):
                        text = re.sub(pat, lambda m, k=key, tag=tag: f'{tag}STATE_GetString({k.format(idx=m.group("idx"))})', text)
            # scalar sub-arrays with a trailing byte index: *(T *)(BASE + ABS + IDX * 4)  (Teams[0].StateCounter[i])
            for member_off, (mname, kind) in a['members'].items():
                sub = re.match(r'(.+)_(\d+)$', mname)
                if not sub or sub.group(2) != '0':
                    continue
                absolute = a['base'] + member_off
                key = f'__key("{a["name"]}_0_{sub.group(1)}_" .. {{idx}})'
                # the compiler folds Teams[T].StateCounter[S] into one index: S + T * (stride/4)
                ints = stride // 4
                idx_expr = r'(?:\*\(int \*\)\(this \+ 0x[0-9a-f]+\)|(?:QUEST|ENTITY)STATE_GetInt\("[^"]*"\)|\w+)'
                flattened = {}
                for m in list(re.finditer(r'^[ \t]*(\w+) = (' + idx_expr + r') \+ (' + idx_expr + r') \* ' + off_re(ints) + r';[ \t]*\r?\n', text, re.M)):
                    flattened[m.group(1)] = (m.group(2), m.group(3), m.group(0))
                pat_store = re.compile(r'^([ 	]*)\*\(' + TYPE + r' \*\)\(' + base + r' \+ ' + off_re(absolute) + r' \+ (?P<idx>(?:QUEST|ENTITY)STATE_GetInt\(\"[^\"]*\"\)|\w+) \* 4\) =\s*([^;]+);', re.M)
                text = pat_store.sub(lambda m, k=key, kind=kind: f'{m.group(1)}QUESTSTATE_Set{kind}({k.format(idx=m.group("idx"))}, {_lit(m.group(3), kind)});', text)
                pat_load = re.compile(r'\*\(' + TYPE + r' \*\)\(' + base + r' \+ ' + off_re(absolute) + r' \+ (?P<idx>(?:QUEST|ENTITY)STATE_GetInt\(\"[^\"]*\"\)|\w+) \* 4\)')
                text = pat_load.sub(lambda m, k=key, kind=kind, tag=tag: f'{tag}STATE_Get{kind}({k.format(idx=m.group("idx"))})', text)
                for var, (sidx, tidx, line) in flattened.items():
                    two_d = f'__key("{a["name"]}_" .. {tidx} .. "_{sub.group(1)}_" .. {sidx})'
                    if f'_{sub.group(1)}_" .. {var})' in text:
                        text = text.replace(f'__key("{a["name"]}_0_{sub.group(1)}_" .. {var})', two_d)
                        if not re.search(r'\b' + re.escape(var) + r'\b', text.replace(line, '')):
                            text = text.replace(line, '', 1)
            for member_off, (mname, kind) in a['members'].items():
                absolute = a['base'] + member_off
                key = f'__key("{a["name"]}_" .. {{idx}} .. "_{mname}")'
                pat = re.compile(r'^([ \t]*)(\w+) = \(int \*\)\((?P<idx>\*\(int \*\)\(this \+ 0x[0-9a-f]+\)|(?:QUEST|ENTITY)STATE_GetInt\("[^"]*"\)|\w+) \* ' + off_re(stride) + r' \+ ' + off_re(absolute) + r' \+ ' + base + r'\);[ \t]*\r?\n', re.M)
                pos = 0
                while (m := pat.search(text, pos)):
                    var, k = m.group(2), key.format(idx=m.group('idx'))
                    head, tail = text[:m.start()], text[m.end():]
                    nxt = re.search(r'^[ \t]*' + re.escape(var) + r' = ', tail, re.M)
                    scope, rest = (tail[:nxt.start()], tail[nxt.start():]) if nxt else (tail, '')
                    scope = re.sub(r'^([ \t]*)\*' + re.escape(var) + r' = \*' + re.escape(var) + r' \+ (-?(?:0x[0-9a-f]+|\d+));',
                                   lambda mm, k=k, kind=kind: f'{mm.group(1)}QUESTSTATE_Set{kind}({k}, QUESTSTATE_Get{kind}({k}) + {mm.group(2)});', scope, flags=re.M)
                    scope = re.sub(r'^([ \t]*)\*' + re.escape(var) + r' = ([^;]+);', lambda mm, k=k, kind=kind: f'{mm.group(1)}QUESTSTATE_Set{kind}({k}, {mm.group(2).strip()});', scope, flags=re.M)
                    scope = re.sub(r'\*' + re.escape(var) + r'\b', f'{tag}STATE_Get{kind}({k})', scope)
                    text = head + scope + rest
                    pos = m.start()
            # element address taken: IDX * STRIDE + BASE + PARENT  -> array pointer value (index)
            text = re.sub(r'(?P<idx>\*\(int \*\)\(this \+ 0x[0-9a-f]+\)|(?:QUEST|ENTITY)STATE_GetInt\(\"[^\"]*\"\)|\w+) \* ' + off_re(stride) + r' \+ ' + off_re(a['base']) + r' \+ ' + base,
                          lambda m, a=a: f'__element("{a["name"]}", {m.group("idx")})', text)
            for index in range(a['count']):
                text = re.sub(r'(?:\(int\))?\(' + base + r' \+ ' + off_re(a['base'] + index * stride) + r'\)',
                              f'__element("{a["name"]}", {index})', text)
                text = re.sub(r'(?<![\w(])' + base + r' \+ ' + off_re(a['base'] + index * stride) + r'(?![\w)])',
                              f'__element("{a["name"]}", {index})', text)
            # element pointer members (Teams[i].EnemyTeam): stored as the sibling index, read through
            for pmember, pname in a.get('pointers', {}).items():
                for index in range(a['count']):
                    absolute = a['base'] + index * stride + pmember
                    text = re.sub(r'^([ \t]*)\*\(\w+ \*\*?\)\(' + base + r' \+ ' + off_re(absolute) + r'\) =\s*__element\("' + a['name'] + r'", (\d+)\);',
                                  lambda m, k=f'{a["name"]}_{index}_{pname}': f'{m.group(1)}QUESTSTATE_SetInt("{k}", {m.group(2)});', text, flags=re.M)
            # temporaries holding an element address: substitute and drop the assignment
            pat = re.compile(r'^[ \t]*(\w+) = (?:\(int \*\))?(__element\("' + a['name'] + r'", [^;]+\));[ \t]*\r?\n', re.M)
            pos = 0
            while (m := pat.search(text, pos)):
                var, value = m.group(1), m.group(2)
                head, tail = text[:m.start()], text[m.end():]
                # the temporary holds the element address until it is reassigned (scalar reuse of the register)
                nxt = re.search(r'^[ \t]*' + re.escape(var) + r' = ', tail, re.M)
                scope, rest = (tail[:nxt.start()], tail[nxt.start():]) if nxt else (tail, '')
                scope = re.sub(r'(?<![\w.>])' + re.escape(var) + r'\b', lambda _: value, scope)
                text = head + scope + rest
                pos = m.start()
            # pointer-to-member temporaries: P = (int *)(__element("A", I) + OFF); *P = *P + N / *P
            for member_off, (mname, kind) in a['members'].items():
                pat = re.compile(r'^[ \t]*(\w+) = \(int \*\)\(__element\("' + a['name'] + r'", ([^;]+?)\) \+ ' + off_re(member_off) + r'\);[ \t]*\r?\n', re.M)
                for m in list(pat.finditer(text)):
                    var, idx = m.group(1), m.group(2)
                    key = f'__key("{a["name"]}_" .. {idx} .. "_{mname}")'
                    tail = text[m.end():]
                    if re.search(r'^[ \t]*' + re.escape(var) + r' = ', tail, re.M):
                        tail_end = re.search(r'^[ \t]*' + re.escape(var) + r' = ', tail, re.M).start()
                    else:
                        tail_end = len(tail)
                    scope = tail[:tail_end]
                    scope = re.sub(r'^([ \t]*)\*' + re.escape(var) + r' = ([^;]+);', lambda mm: f'{mm.group(1)}QUESTSTATE_Set{kind}({key}, {mm.group(2).strip()});', scope, flags=re.M)
                    scope = re.sub(r'\*' + re.escape(var) + r'\b', f'QUESTSTATE_Get{kind}({key})', scope)
                    text = text[:m.start()] + scope + tail[tail_end:]
            # static index forms already expanded in evidence as scalar fields (Teams_0_MemberCount): lifter state map
        # 3b. Thing members
        for off, name in things.items():
            recv = f'{tag}THING_Get("{name}")'
            text = re.sub(r'CScriptThing::operator=\(\(CScriptThing \*\)\(' + base + r' \+ ' + off_re(off) + r'\),\s*([^;]+)\);',
                          lambda m, name=name, tag=tag: f'{tag}THING_Set("{name}", {"nil" if m.group(1).strip() == "(CScriptThing *)0x0" else _thing_source(m.group(1))});', text)
            text = re.sub(r'CScriptThing::~CScriptThing\(\(CScriptThing \*\)\(' + base + r' \+ ' + off_re(off) + r'\)\);', '', text)
            # vcall on the thing: (**(code **)(*(int *)(BASE + OFF) + SLOT))(
            def vcall(m, recv=recv):
                out = thing_call(recv, m.group(1), m.end(), text[m.end():m.end() + 1])
                return out if out else m.group(0)
            text = re.sub(r'\(\*\*\(code \*\*\)\(\*\(int \*\)\(' + base + r' \+ ' + off_re(off) + r'\) \+ (0x[0-9a-f]+|\d+)\)\)\s*\(', vcall, text)
            # the thing's counted pointer Data (+4) compared to null: (BASE+OFF+4)
            text = re.sub(r'\*\(int \*\)\(' + base + r' \+ ' + off_re(off + 4) + r'\)', f'__thing_valid({recv})', text)
            text = re.sub(r'\(CScriptThing \*\)\(' + base + r' \+ ' + off_re(off) + r'\)', recv, text)
        # 3c. Thing vectors
        for off, name in lists.items():
            begin = r'\*\(int \*\)\(' + base + r' \+ ' + off_re(off) + r'\)'
            # a begin pointer cached in a slot (`iStack_4 = *(int *)(this + 0x48); ... (CScriptThing *)(iVar9 + iStack_4)`,
            # TraderConflictGood WatchForHittingEnemies 0x00DFC630: the element operand of GiveThingBestEnemyTarget was
            # lost, third audit): inline the cache into its element reads up to the slot's next definition
            base_text = re.sub(r'\\(.)', r'\1', base)     # the family's base as text (the pattern is an escaped literal)
            for cm in list(re.finditer(r'^[ \t]*(\w+) = ' + begin + r';[ \t]*\r?\n', text, re.M)):
                cache = cm.group(1)
                tail = text[cm.end():]
                nxt = re.search(r'^[ \t]*' + re.escape(cache) + r' = ', tail, re.M)
                scope, rest = (tail[:nxt.start()], tail[nxt.start():]) if nxt else (tail, '')
                scope2 = re.sub(r'\((\w+) \+ ' + re.escape(cache) + r'\)', lambda mm: f'({mm.group(1)} + *(int *)({base_text} + {off:#x}))', scope)
                scope2 = re.sub(r'\(' + re.escape(cache) + r' \+ (\w+)\)', lambda mm: f'({mm.group(1)} + *(int *)({base_text} + {off:#x}))', scope2)
                if scope2 != scope and not re.search(r'\b' + re.escape(cache) + r'\b', scope2):
                    text = text[:cm.start()] + scope2 + rest
            end = r'\*\(int \*\*?\)\(' + base + r' \+ ' + off_re(off + 4) + r'\)'
            text = re.sub(r'\(uint\)\(\(' + end + r' -\s*' + begin + r'\) / 0xc\)', f'{tag}LIST_Count("{name}")', text)
            text = re.sub(end + r' -\s*' + begin, f'({tag}LIST_Count("{name}") * 0xc)', text)
            # pointer-to-begin temporaries: P = (int *)(BASE + OFF); *P is the begin value, P[1] the end
            ptr_pat = re.compile(r'^[ \t]*(\w+) = \(int \*\)\(' + base + r' \+ ' + off_re(off) + r'\);[ \t]*\r?\n', re.M)
            pos = 0
            while (m := ptr_pat.search(text, pos)):
                pos = m.start()
                var = m.group(1)
                head, tail = text[:m.start()], text[m.end():]
                nxt = re.search(r'^[ \t]*' + re.escape(var) + r' = ', tail, re.M)
                # the redefining line's right-hand side still reads the pointer (`P = (int *)*P;`)
                cut = (tail.find('\n', nxt.start()) + 1 or len(tail)) if nxt else len(tail)
                scope, rest = tail[:cut], tail[cut:]
                scope = re.sub(r'\*' + re.escape(var) + r'\b', f'{tag}LIST_BeginValue("{name}")', scope)
                scope = re.sub(r'\b' + re.escape(var) + r'\[1\]', f'{tag}LIST_EndValue("{name}")', scope)
                scope = re.sub(r'([(,]\s*)' + re.escape(var) + r'(?=\s*[,)])', lambda mm, name=name, tag=tag: f'{mm.group(1)}{tag}LIST_Ref("{name}")', scope)
                text = head + scope + rest
            # element: *(int *)(BEGIN + IDX)  (IDX in bytes, stride 0xc) and vcalls on it
            def elem_vcall(m, name=name, tag=tag):
                out = thing_call(f'{tag}LIST_At_{name}(({m.group(1)}) / 0xc)', m.group(2), m.end(), text[m.end():m.end() + 1])
                return out if out else m.group(0)
            # the byte index may carry a stale register cast (`(int)CVar5`) and may precede the begin value
            idx = r'(?:\(int\))?(?!\d)(\w+)'
            text = re.sub(r'\(\*\*\(code \*\*\)\(\*\(int \*\)\(' + begin + r' \+ ' + idx + r'\) \+ (0x[0-9a-f]+|\d+)\)\)\s*\(', elem_vcall, text)
            text = re.sub(r'\(\*\*\(code \*\*\)\(\*\(int \*\)\(' + idx + r' \+ ' + begin + r'\) \+ (0x[0-9a-f]+|\d+)\)\)\s*\(', elem_vcall, text)
            text = re.sub(r'\(CScriptThing \*\)\(' + begin + r' \+ ' + idx + r'\)', lambda m, name=name, tag=tag: f'{tag}LIST_At_{name}(({m.group(1)}) / 0xc)', text)
            text = re.sub(r'\(CScriptThing \*\)\(' + idx + r' \+ ' + begin + r'\)', lambda m, name=name, tag=tag: f'{tag}LIST_At_{name}(({m.group(1)}) / 0xc)', text)
            text = re.sub(begin + r' \+ ' + idx + r'\b', lambda m, name=name, tag=tag: f'{tag}LIST_At_{name}(({m.group(1)}) / 0xc)', text)
            text = re.sub(r'(?<![\w)])' + idx + r' \+ ' + begin, lambda m, name=name, tag=tag: f'{tag}LIST_At_{name}(({m.group(1)}) / 0xc)', text)
            text = re.sub(begin, f'{tag}LIST_BeginValue("{name}")', text)
            text = re.sub(end, f'{tag}LIST_EndValue("{name}")', text)
            bv, ev = f'{tag}LIST_BeginValue("{name}")', f'{tag}LIST_EndValue("{name}")'
            # arithmetic on begin/end values after alias substitution
            text = text.replace(f'{ev} - {bv}', f'({tag}LIST_Count("{name}") * 0xc)')
            text = re.sub(r'\*\(int \*\)\(' + re.escape(bv) + r' \+ ' + idx + r'\)', lambda m, name=name, tag=tag: f'{tag}LIST_At_{name}(({m.group(1)}) / 0xc)', text)
            text = re.sub(r'\*' + re.escape(bv), f'{tag}LIST_At_{name}(0)', text)
            text = re.sub(re.escape(bv) + r' \+ ' + idx + r'\b', lambda m, name=name, tag=tag: f'{tag}LIST_At_{name}(({m.group(1)}) / 0xc)', text)
            text = re.sub(r'(?<![\w)])' + idx + r' \+ ' + re.escape(bv), lambda m, name=name, tag=tag: f'{tag}LIST_At_{name}(({m.group(1)}) / 0xc)', text)
            text = re.sub(r'\*\(int \*\*\)\(' + idx + r' \+ 4 \+ ' + re.escape(bv) + r'\)', lambda m, name=name, tag=tag: f'{tag}LIST_At_{name}(({m.group(1)}) / 0xc)', text)
            # vcalls on a named element (`(**(code **)(ELEM + OFF))(` / `(**(code **)(*(int *)(ELEM) + OFF))(`); a cast around it
            at = re.escape(f'{tag}LIST_At_{name}(') + r'([^;\n]*?\) / 0xc)\)'
            def at_vcall(m, name=name, tag=tag):
                out = thing_call(f'{tag}LIST_At_{name}({m.group(1)})', m.group(2), m.end(), text[m.end():m.end() + 1])
                return out if out else m.group(0)
            text = re.sub(r'\(\*\*\(code \*\*\)\((?:\*\(int \*\)\()?' + at + r'\)? \+ (0x[0-9a-f]+|\d+)\)\)\s*\(', at_vcall, text)
            text = re.sub(r'\(CScriptThing \*\)\((' + at + r')\)', r'\1', text)
            text = re.sub(r'\(int \*\)\(' + base + r' \+ ' + off_re(off) + r'\)', f'{tag}LIST_Ref("{name}")', text)
            text = re.sub(r'Vector_PushBack_ScriptThing\(\(void \*\)\(' + base + r' \+ ' + off_re(off) + r'\),\s*([^;]+)\);',
                          lambda m, name=name, tag=tag: f'{tag}LIST_Push("{name}", {m.group(1).strip()});', text)
            # a local vector copy-constructed from the member (`Vector_CopyFrom(&local, this + OFF)`): a table copy
            text = re.sub(r'Vector_CopyFrom\(\(void \*\)&(\w+),\s*\(void \*\)\(' + base + r' \+ ' + off_re(off) + r'\)\);',
                          lambda m, name=name, tag=tag: f'{m.group(1)} = {tag}LIST_Copy("{name}");', text)
            # erase(iterator): std::vector<CScriptThing>::erase at 0xCD3152 (any label)
            for label, target in getattr(spec, 'call_labels', {}).items():
                if target == 0xCD3152:
                    text = re.sub(re.escape(label) + r'\((?:\(void \*\)\(' + base + r' \+ ' + off_re(off) + r'\)|' + re.escape(f'{tag}LIST_Ref("{name}")') + r'),\s*(?:\(int\))?(\w+)\);',
                                  lambda m, name=name, tag=tag: f'{tag}LIST_Erase("{name}", ({m.group(1)}) / 0xc);', text)
            ref = f'{tag}LIST_Ref("{name}")'
            # erase(begin, end) is a clear
            text = re.sub(r'Std_Vector_Erase_Range\((?:\(int \*\))?(?:\w+|' + re.escape(ref) + r'),\s*' + re.escape(bv) + r',\s*(?:' + re.escape(ev) + r'|\(' + re.escape(f'{tag}LIST_Count("{name}")') + r' \* 0xc\))\);',
                          f'{tag}LIST_Clear("{name}");', text)
            # an interface call filling the member through its out-argument returns the table
            text = re.sub(r'^([ \t]*)GSI->(\w+)\(((?:[^;()]*?),\s*)?' + re.escape(ref) + r'\);',
                          lambda m, name=name, tag=tag: f'{m.group(1)}{tag}LIST_Set("{name}", GSI->{m.group(2)}({(m.group(3) or "").rstrip().rstrip(",")}));', text, flags=re.M)
            # iterator variables: P = begin; ... P = P + 0xc; P != end  -> byte offsets from 0
            for m in list(re.finditer(r'^[ \t]*(\w+) = (?:\(int \*\))?' + re.escape(bv) + r';', text, re.M)):
                it = m.group(1)
                if not re.search(r'\b' + it + r' = ' + it + r' \+ (?:0xc|3);|\b' + it + r' [!=]= (?:\(int \*\*?\))?' + re.escape(ev), text):
                    continue          # a plain begin-pointer temporary, handled by the element rules
                text = text.replace(m.group(0), re.sub(r'\(int \*\)' + re.escape(bv) + r'|' + re.escape(bv), '0', m.group(0)), 1)
                text = re.sub(r'\b' + it + r' = ' + it + r' \+ 3;', f'{it} = {it} + 0xc;', text)
                text = re.sub(r'\b' + it + r' ([!=]=) \(int \*\*?\)' + re.escape(ev), lambda mm, it=it, ev=ev: f'{it} {mm.group(1)} {ev}', text)
                # vcalls straight through the iterator (`(**(code **)(*it + OFF))(`) are thing calls on the element,
                # up to the register's next unrelated definition
                elem = f'{tag}LIST_At_{name}(({it}) / 0xc)'
                start = text.find(re.sub(r'\(int \*\)' + re.escape(bv) + r'|' + re.escape(bv), '0', m.group(0)))
                after = text.find('\n', start) + 1
                nxt = re.search(r'^[ \t]*' + it + r' = (?!' + it + r' \+ 0xc;)', text[after:], re.M)
                cut = after + nxt.start() if nxt else len(text)
                scope = re.sub(r'\(\*\*\(code \*\*\)\(\*' + it + r' \+ (0x[0-9a-f]+|\d+)\)\)\s*\(',
                               lambda h, elem=elem: thing_call(elem, h.group(1), h.end(), text[start:cut][h.end():h.end() + 1]) or h.group(0), text[start:cut])
                scope = re.sub(r'\*\((?:undefined4|int) \*\)\(' + it + r' \+ 4\)', elem, scope)
                text = text[:start] + scope + text[cut:]
                # the same operand anywhere the register still steps by 0xc (a rotated loop prints its body after
                # an unrelated reuse of the register, e.g. an inline strncmp counter)
                text = re.sub(r'\*\((?:undefined4|int) \*\)\(' + it + r' \+ 4\)', elem, text)
                # element copy-construction from the iterator: Info (+8), Data (+4), vtable, addref
                copy = re.compile(r'^([ \t]*)(\w+) = \*\(int \*\*\)\(' + it + r' \+ 8\);[ \t]*\r?\n[ \t]*(\w+) = \*\(int \*\*\)\(' + it + r' \+ 4\);[ \t]*\r?\n'
                                  r'[ \t]*(\w+) = (?:&PTR_[A-Za-z_]*_01238c8c|QUESTTHING_Empty\(\));[ \t]*\r?\n[ \t]*if \(\2 != \(int \*\)0x0\) \{\s*\r?\n[ \t]*\*\2 = \*\2 \+ 1;\s*\r?\n[ \t]*\}[ \t]*\r?\n', re.M)
                for cm in list(copy.finditer(text)):
                    elem = f'{tag}LIST_At_{name}(({it}) / 0xc)'
                    data = cm.group(3)
                    text = text.replace(cm.group(0), f'{cm.group(1)}{cm.group(4)} = {elem};\n', 1)
                    # vcalls through the copied Data pointer are thing calls on the element
                    text = re.sub(r'\(\*\*\(code \*\*\)\(\*' + data + r' \+ (0x[0-9a-f]+|\d+)\)\)\s*\(' + data + r'(?:,\s*)?',
                                  lambda h, elem=elem: thing_call(elem, h.group(1), h.end(), text[h.end():h.end() + 1]) or h.group(0), text)
            # element copy by byte index with the vtable literal inline in the call: the Data field read
            # (`fVar = *(float *)(IDX + 4 + BEGIN)` — a stale float register) then `F(&PTR_vtable, fVar)`
            for cm in list(re.finditer(r'^[ \t]*(\w+) = \*\((?:float|int|undefined4) \*\)\(' + idx + r' \+ 4 \+ ' + re.escape(bv) + r'\);[ \t]*\r?\n', text, re.M)):
                var, index = cm.group(1), cm.group(2)
                elem = f'{tag}LIST_At_{name}(({index}) / 0xc)'
                head, tail = text[:cm.start()], text[cm.end():]
                nxt = re.search(r'^[ \t]*' + re.escape(var) + r' = ', tail, re.M)
                scope, rest = (tail[:nxt.start()], tail[nxt.start():]) if nxt else (tail, '')
                # the by-value thing is three pushes (vtable, Data, Info) Ghidra read as two operands: whatever the
                # call took after it (a float range) is not in the decompile — an explicit lost operand, not a stale temporary
                scope, n = re.subn(r'\((?:CScriptThing(?:_bv)?|int) \*\)&PTR_[A-Za-z_]*_01238c8c,\s*' + re.escape(var) + r'\b(?=\))', elem + ', ENGINE_LostOperand()', scope)
                if n and not re.search(r'\b' + re.escape(var) + r'\b', scope):
                    text = head + scope + rest
            text = text.replace(ev, f'({tag}LIST_Count("{name}") * 0xc)')
            # the signed element count (`n = COUNT * 0xc; s = n >> 0x1f; if (n / 0xc + s != s)`, `(uint)(n / 0xc)`)
            count = f'{tag}LIST_Count("{name}")'
            for m in list(re.finditer(r'^[ \t]*(\w+) = \(' + re.escape(count) + r' \* 0xc\);[ \t]*\r?\n[ \t]*(\w+) = \1 >> 0x1f;[ \t]*\r?\n', text, re.M)):
                n, sgn = m.group(1), m.group(2)
                text = re.sub(r'\b' + n + r' / 0xc \+ ' + sgn + r' (!=|==) ' + sgn + r'\b', lambda mm: f'{count} {mm.group(1)} 0', text)
                text = re.sub(r'\(uint\)\(' + n + r' / 0xc\)|\b' + n + r' / 0xc\b', count, text)
                # each definition is dead when its register is next redefined (or never read again)
                pos = text.find(m.group(0))
                if pos < 0:
                    continue
                first, second = m.group(0).split('\n', 1)[0] + '\n', m.group(0).split('\n', 1)[1]
                if _next_use_is_redefinition(text, pos + len(m.group(0)), sgn):
                    text = text[:pos] + first + text[pos + len(m.group(0)):]
                    if _next_use_is_redefinition(text, pos + len(first), n):
                        text = text[:pos] + text[pos + len(first):]
            # the same with the count propagated into the test (`s = (COUNT * 0xc) >> 0x1f; if (((COUNT * 0xc)) / 0xc + s != s)`,
            # also as a comma expression inside a condition)
            c = re.escape(count)
            text = re.sub(r'\(\(' + c + r' \* 0xc\)\) / 0xc \+ (\w+) (!=|==) \1\b', lambda mm: f'{count} {mm.group(2)} 0', text)
            text = re.sub(r'\(uint\)\(\(' + c + r' \* 0xc\) / 0xc\)|\(' + c + r' \* 0xc\) / 0xc\b', count, text)
            # a sign definition whose register is next redefined (or never read again) is dead
            for m in reversed(list(re.finditer(r'^[ \t]*(\w+) = \(' + c + r' \* 0xc\) >> 0x1f;[ \t]*\r?\n|\((\w+) = \(' + c + r' \* 0xc\) >> 0x1f, ', text, re.M))):
                sgn = m.group(1) or m.group(2)
                nxt = re.search(r'\b' + sgn + r'\b', text[m.end():])
                if nxt:
                    at_line_start = text[m.end():m.end() + nxt.start()].rsplit('\n', 1)[-1].strip() == ''
                    redefined = at_line_start and re.match(r'\s*=(?!=)', text[m.end() + nxt.end():])
                    if not redefined:
                        continue
                if m.group(1):
                    text = text[:m.start()] + text[m.end():]
                else:
                    text = text[:m.start()] + '(' + text[m.end():]     # `(s = ..., COND)` -> `(COND)`

    # 3b'. CCharString members: construct / assign / read / destroy
    string_families = [(r'this', spec.self_fields, 'ENTITY' if spec.entity else 'QUEST')]
    if spec.entity:
        string_families.append((parent, spec.parent_fields, 'QUEST'))
    for base, fields, tag in string_families:
        for off, (name, kind) in fields.items():
            if kind != 'String':
                continue
            member = r'\(CCharString \*\)\(' + base + r' \+ ' + off_re(off) + r'\)'
            text = re.sub(r'^([ \t]*)CCharString::(?:operator=|CCharString)\(' + member + r',\s*([^;]+?)(?:,\s*-1)?\);',
                          lambda m, name=name, tag=tag: f'{m.group(1)}{tag}STATE_SetString("{name}", {m.group(2).strip()});', text, flags=re.M)
            text = re.sub(r'^[ \t]*CCharString::~CCharString\(' + member + r'\);[ \t]*\r?\n', '', text, flags=re.M)
            # CCharString::AssignFromWide(&member, L"...") (0x99B800): the literal is a UTF-16 .rdata string
            resolve_wide = getattr(spec, 'resolve_wide', None)
            if resolve_wide:
                def assign_wide(m, name=name, tag=tag):
                    literal = resolve_wide(int(m.group(2), 16))
                    if literal is None:
                        return m.group(0)
                    literal = literal.replace('\\', '\\\\').replace('"', '\\"')
                    return f'{m.group(1)}{tag}STATE_SetString("{name}", "{literal}");'
                text = re.sub(r'^([ \t]*)CCharString(?:::|__)AssignFromWide\((?:\(CCharString \*\))?\(?' + base + r' \+ ' + off_re(off) + r'\)?,\s*(0x[0-9a-f]+)\);',
                              assign_wide, text, flags=re.M)
            text = re.sub(member, f'{tag}STATE_GetString("{name}")', text)
            # the member's string rep read raw for an inlined `==` (`puVar1 = *(undefined4 **)(parent + 0x78)`,
            # DarkwoodTrader Main's TraderToTalk test; fold_inline_string_equality finishes it)
            text = re.sub(r'\*\((?:undefined4 \*\*|int \*)\)\(' + base + r' \+ ' + off_re(off) + r'\)', f'{tag}STATE_GetString("{name}")', text)
    # 3c'. helpers returning a CScriptThing through a hidden pointer: Ghidra drops the pointer push, so the
    # call reads `Helper(this);` and the result is the stack object whose Data (`X._4_4_`) is used next.
    hidden = {label for label, target in getattr(spec, 'call_labels', {}).items() if target in getattr(spec, 'hidden_thing_returns', set())}
    for label in hidden:
        pat = re.compile(r'^([ \t]*)' + re.escape(label) + r'\s*\(([^;]*)\);[ \t]*\r?\n', re.M)
        pos = 0
        while (m := pat.search(text, pos)):
            use = re.search(r'\b((?:[A-Za-z]+Stack_|local_)[0-9a-f]+)\._4_4_', text[m.end():])
            if not use:
                pos = m.end()
                continue
            obj = use.group(1)
            name = 'thing_' + re.sub(r'^.*?(?:Stack_|local_)', '', obj)
            text = text[:m.start()] + f'{m.group(1)}{name} = {label}({m.group(2)});\n' + text[m.end():]
            text = re.sub(r'\(CScriptThing \*\)' + re.escape(obj) + r'\b', name, text)
            text = re.sub(r'\b' + re.escape(obj) + r'\b', name, text)
            pos = m.start() + 1
    # Data-pointer idioms on a stack thing: `(int *)X._4_4_ == (int *)0x0` is validity, `(**(code **)(*(int *)X._4_4_ + SLOT))(` a thing call
    text = re.sub(r'(?:\(int \*\))?(\w+)\._4_4_ == \(int \*\)0x0', r'!__thing_valid(\1)', text)
    text = re.sub(r'(?:\(int \*\))?(\w+)\._4_4_ != \(int \*\)0x0', r'__thing_valid(\1)', text)
    text = re.sub(r'\(\*\*\(code \*\*\)\(\*(?:\(int \*\))?(\w+)\._4_4_ \+ (0x[0-9a-f]+|\d+)\)\)\s*\(',
                  lambda h: thing_call(h.group(1), h.group(2), h.end(), text[h.end():h.end() + 1]) or h.group(0), text)
    # the same idioms when the export named the thing's Data field with the object's own slot name
    # (`if (X != (int *)0x0) { (**(code **)(*X + SLOT))(...); }` on a stack thing X)
    stack_things = set(re.findall(r'\(CScriptThing \*\)((?:[A-Za-z]+Stack_|local_)[0-9a-f]+)\b', text))
    for name in stack_things:
        n = re.escape(name)
        # the Data field printed as the object's FIRST dword (`X._0_4_`) where the decompiler's stack model sits 4
        # bytes off for that site (Will Guildmaster 0x00D5E0C0: three byte-identical `mov ecx,[esp+0xa4]; test;
        # call [edx+0x118]` sequences, the first printed `auStack_1b4._4_4_`, the other two `auStack_1b4._0_4_`):
        # a null test or a vcall through `*(int *)X._0_4_ + SLOT` is never the vtable word (that vcall would read
        # `X._0_4_ + SLOT`), so on a stack thing it is the Data idiom, 2026-09-21
        # (not the counted-pointer release `(X._0_4_ != 0) && (*X._0_4_ = *X._0_4_ + -1, ..)`: the release rules)
        text = re.sub(r'(?:\(int \*\))?' + n + r'\._0_4_ == \(int \*\)0x0(?!\) \|\| \(\*(?:\(int \*\))?' + n + r'\._0_4_ = )', f'!__thing_valid({name})', text)
        text = re.sub(r'(?:\(int \*\))?' + n + r'\._0_4_ != \(int \*\)0x0(?!\) &&\s*\(\*(?:\(int \*\))?' + n + r'\._0_4_ = |\) \{[ \t]*\r?\n[ \t]*\*(?:\(int \*\))?' + n + r'\._0_4_ = )', f'__thing_valid({name})', text)
        text = re.sub(r'\(\*\*\(code \*\*\)\(\*(?:\(int \*\))?' + n + r'\._0_4_ \+ (0x[0-9a-f]+|\d+)\)\)\s*\(',
                      lambda h, name=name: thing_call(name, h.group(1), h.end(), text[h.end():h.end() + 1]) or h.group(0), text)
        # the counted-pointer release idiom on the same name (`(X != 0) && (*X = *X + -1, ...)`) belongs to the release rules
        text = re.sub(r'\b' + n + r' == \(int \*\)0x0(?!\) \|\| \(\*' + n + r' = )', f'!__thing_valid({name})', text)
        text = re.sub(r'\b' + n + r' != \(int \*\)0x0(?!\) && \(\*' + n + r' = |\) \{[ \t]*\r?\n[ \t]*\*' + n + r' = )', f'__thing_valid({name})', text)
        text = re.sub(r'\(\*\*\(code \*\*\)\(\*(?:\(int \*\))?' + re.escape(name) + r' \+ (0x[0-9a-f]+|\d+)\)\)\s*\(',
                      lambda h, name=name: thing_call(name, h.group(1), h.end(), text[h.end():h.end() + 1]) or h.group(0), text)
    # 3d. vcalls through a saved vtable temporary of a lowered thing: V = *(int *)(THING); (**(code **)(V + SLOT))(
    RE_VT_TEMP = re.compile(r'^[ \t]*(\w+) = \*\(int \*\)\(((?:QUEST|ENTITY)(?:THING_Get|LIST_At_\w+)\([^;\n]*\))\);[ \t]*\r?\n', re.M)
    for m in list(RE_VT_TEMP.finditer(text)):
        var, recv = m.group(1), m.group(2)
        pat = re.compile(r'\(\*\*\(code \*\*\)\(' + re.escape(var) + r' \+ (0x[0-9a-f]+|\d+)\)\)\s*\(')
        after = text[m.end():]
        nxt = re.search(r'^[ \t]*' + re.escape(var) + r' = ', after, re.M)
        scope, rest = (after[:nxt.start()], after[nxt.start():]) if nxt else (after, '')
        hits = list(pat.finditer(scope))
        if not hits or any(thing_call(recv, h.group(1), 0, '') is None for h in hits):
            continue
        scope = pat.sub(lambda h: thing_call(recv, h.group(1), h.end(), scope[h.end():h.end() + 1]), scope)
        text = text[:m.start()] + scope + rest
    # 3e. vcalls on CScriptThing parameters: (**(code **)(*(int *)param + SLOT))(
    sig = re.search(r'\(\s*\w+ \*this(?:,([^)]*))?\)\s*\r?\n\r?\n?\{', text)
    pnames = re.findall(r'\b(\w+)\s*(?:,|$)', sig.group(1)) if sig and sig.group(1) else []
    for pname in list(pnames):
        pnames += re.findall(r'^[ \t]*(\w+) = ' + re.escape(pname) + r';', text, re.M)
    for pname in pnames:
        pat = re.compile(r'\(\*\*\(code \*\*\)\(\*\(int \*\)' + re.escape(pname) + r' \+ (0x[0-9a-f]+|\d+)\)\)\s*\((?:' + re.escape(pname) + r'\s*(?:,\s*&\w+\s*(?=\)))?(?:,\s*)?)?')
        text = pat.sub(lambda h: thing_call(pname, h.group(1), h.end(), text[h.end():h.end() + 1]) or h.group(0), text)
    # 3f. inline CScriptThing copies out of a list element into a stack thing (counted-pointer form)
    RE_ELEM_PARTS = re.compile(r'^[ \t]*(\w+) = \*\(int \*\*\)\((\w+) \+ 8 \+ (\w+)\);[ \t]*\r?\n[ \t]*(\w+) = \*\(undefined4 \*\)\(\2 \+ 4 \+ \3\);[ \t]*\r?\n', re.M)
    for m in list(RE_ELEM_PARTS.finditer(text)):
        ptr, basevar, idx, data = m.groups()
        src = re.search(r'^[ \t]*' + re.escape(basevar) + r' = ((?:QUEST|ENTITY)LIST_BeginValue\("(\w+)"\));', text[:m.start()], re.M)
        if not src:
            continue
        tag = src.group(1).split('LIST_')[0]
        elem = f'{tag}LIST_At_{src.group(2)}(({idx}) / 0xc)'
        assign = re.compile(r'^([ \t]*)if \((\w+) != ' + ptr + r'\) \{\s*\r?\n[ \t]*if \(\(\2 != \(int \*\)0x0\) && \(\*\2 = \*\2 \+ -1, \*\2 == 0\)\) \{\s*\r?\n[ \t]*\(\*\(code \*\)\2\[1\]\)\(\);\s*\r?\n[ \t]*operator_delete\(\2\);\s*\r?\n[ \t]*\}\s*\r?\n'
                            r'(?:[ \t]*(\w+) = \w+;[ \t]*\r?\n)*?'
                            r'[ \t]*\w+ = ' + data + r';[ \t]*\r?\n[ \t]*\2 = ' + ptr + r';[ \t]*\r?\n[ \t]*if \(' + ptr + r' != \(int \*\)0x0\) \{\s*\r?\n[ \t]*\*' + ptr + r' = \*' + ptr + r' \+ 1;\s*\r?\n[ \t]*\}\s*\r?\n[ \t]*\}[ \t]*\r?\n', re.M)
        am = assign.search(text, m.end())
        if not am:
            continue
        inner = re.findall(r'^[ \t]*(\w+ = \w+;)[ \t]*$', text[am.start():am.end()], re.M)
        keep = [ln for ln in inner if not ln.startswith((am.group(2) + ' =',)) and not ln.endswith((f'= {data};', f'= {ptr};'))]
        repl = ''.join(f'{am.group(1)}{ln}\n' for ln in keep) + f'{am.group(1)}{am.group(2)} = {elem};\n'
        text = text[:am.start()] + repl + text[am.end():]
        text = text[:m.start()] + text[m.end():]
    # 3g. return-by-hidden-pointer of a CScriptThing: *ret = &vtable; ret[1] = X; ret[2] = X; if (X) *X += 1
    text = re.sub(r'^([ \t]*)\*(\w+) = &PTR_[A-Za-z_]*_01238c8c;[ \t]*\r?\n[ \t]*\2\[1\] = (\w+);[ \t]*\r?\n[ \t]*\2\[2\] = \3;[ \t]*\r?\n[ \t]*if \(\3 != \(int \*\)0x0\) \{\s*\r?\n[ \t]*\*\3 = \*\3 \+ 1;\s*\r?\n[ \t]*\}[ \t]*\r?\n',
                  lambda m: f'{m.group(1)}ret_thing = {m.group(3)};\n', text, flags=re.M)
    if 'ret_thing = ' in text:
        text = re.sub(r'return \(int\)in_stack_\w+;', 'return ret_thing;', text)

    # 4. entity pointer-into-parent-array fields (MyTeam): store index, keyed access through it
    if spec.entity:
        for off, info in spec.self_pointers.items():
            a, pname = info['array'], info['name']
            ptr = r'\*\(int \*\)\(this \+ ' + off_re(off) + r'\)'
            text = re.sub(r'^([ \t]*)(\w+) = (?:\(int \*\))?__element\("' + a['name'] + r'", ([^;]+)\);\s*\n[ \t]*\*\(int \*\*?\)\(this \+ ' + off_re(off) + r'\) = \2;',
                          lambda m, pname=pname: f'{m.group(1)}ENTITYSTATE_SetInt("{pname}", {m.group(3).strip()});', text, flags=re.M)
            text = re.sub(r'^([ \t]*)\*\(int \*\*?\)\(this \+ ' + off_re(off) + r'\) =\s*(?:\(int \*\))?__element\("' + a['name'] + r'", ([^;]+)\);',
                          lambda m, pname=pname: f'{m.group(1)}ENTITYSTATE_SetInt("{pname}", {m.group(2).strip()});', text, flags=re.M)
            idx = f'ENTITYSTATE_GetInt("{pname}")'
            for member_off, (mname, kind) in a['members'].items():
                key = f'__key("{a["name"]}_" .. {idx} .. "_{mname}")'
                if member_off == 0:
                    text = re.sub(r'^([ \t]*)\*\*\(int \*\*\)\(this \+ ' + off_re(off) + r'\) =\s*([^;]+);',
                                  lambda m, key=key, kind=kind: f'{m.group(1)}QUESTSTATE_Set{kind}({key}, {_lit(m.group(2), kind)});', text, flags=re.M)
                    text = re.sub(r'\*\*\(int \*\*\)\(this \+ ' + off_re(off) + r'\)', f'QUESTSTATE_Get{kind}({key})', text)
                text = re.sub(r'^([ \t]*)\*\(' + TYPE + r' \*\)\(' + ptr + r' \+ ' + off_re(member_off) + r'\) =\s*([^;]+);',
                              lambda m, key=key, kind=kind: f'{m.group(1)}QUESTSTATE_Set{kind}({key}, {_lit(m.group(2), kind)});', text, flags=re.M)
                text = re.sub(r'\*\(' + TYPE + r' \*\)\(' + ptr + r' \+ ' + off_re(member_off) + r'\)', f'QUESTSTATE_Get{kind}({key})', text)
            for member_off, (mname, kind) in a['members'].items():
                sub = re.match(r'(.+)_0$', mname)
                if not sub or member_off != 0:
                    continue
                skey = f'__key("{a["name"]}_" .. {idx} .. "_{sub.group(1)}_" .. {{sidx}})'
                pat = re.compile(r'^([ \t]*)(\w+) = \(int \*\)\(' + ptr + r' \+ (?P<sidx>[^;]+?) \* 4\);[ \t]*\r?\n', re.M)
                pos = 0
                while (m := pat.search(text, pos)):
                    var, key = m.group(2), skey.format(sidx=m.group('sidx').strip())
                    head, tail = text[:m.start()], text[m.end():]
                    nxt = re.search(r'^[ \t]*' + re.escape(var) + r' = ', tail, re.M)
                    scope, rest = (tail[:nxt.start()], tail[nxt.start():]) if nxt else (tail, '')
                    scope = re.sub(r'^([ \t]*)\*' + re.escape(var) + r' = \*' + re.escape(var) + r' \+ (-?(?:0x[0-9a-f]+|\d+));',
                                   lambda mm, key=key: f'{mm.group(1)}QUESTSTATE_SetInt({key}, QUESTSTATE_GetInt({key}) + {mm.group(2)});', scope, flags=re.M)
                    scope = re.sub(r'\*' + re.escape(var) + r'\b', f'QUESTSTATE_GetInt({key})', scope)
                    text = head + scope + rest
                    pos = m.start()
            for pmember, pname in a.get('pointers', {}).items():
                # *(T *)(*(int *)(MyTeam + EnemyTeam) + M) -> member M of Teams[Teams_<MyTeam>_EnemyTeam]
                chained_idx = f'QUESTSTATE_GetInt(__key("{a["name"]}_" .. {idx} .. "_{pname}"))'
                chained = r'\*\(int \*\)\(' + ptr + r' \+ ' + off_re(pmember) + r'\)'
                for member_off, (mname, kind) in a['members'].items():
                    key = f'__key("{a["name"]}_" .. {chained_idx} .. "_{mname}")'
                    text = re.sub(r'^([ \t]*)\*\(' + TYPE + r' \*\)\(' + chained + r' \+ ' + off_re(member_off) + r'\) =\s*([^;]+);',
                                  lambda m, key=key, kind=kind: f'{m.group(1)}QUESTSTATE_Set{kind}({key}, {_lit(m.group(2), kind)});', text, flags=re.M)
                    text = re.sub(r'\*\(' + TYPE + r' \*\)\(' + chained + r' \+ ' + off_re(member_off) + r'\)', f'QUESTSTATE_Get{kind}({key})', text)
                for member_off, tname in a['things'].items():
                    recv = f'QUESTTHING_Get(__key("{a["name"]}_" .. {chained_idx} .. "_{tname}"))'
                    def vcall(m, recv=recv):
                        out = thing_call(recv, m.group(1), m.end(), text[m.end():m.end() + 1])
                        return out if out else m.group(0)
                    text = re.sub(r'\(\*\*\(code \*\*\)\(\*\(int \*\)\(' + chained + r' \+ ' + off_re(member_off) + r'\) \+ (0x[0-9a-f]+|\d+)\)\)\s*\(', vcall, text)
                    text = re.sub(r'\(CScriptThing \*\)\(' + chained + r' \+ ' + off_re(member_off) + r'\)', recv, text)
                    text = re.sub(r'(?<![\w*(])' + chained + r' \+ ' + off_re(member_off) + r'(?![\w])', recv, text)
            for member_off, tname in a['things'].items():
                recv = f'QUESTTHING_Get(__key("{a["name"]}_" .. {idx} .. "_{tname}"))'
                def vcall(m, recv=recv):
                    out = thing_call(recv, m.group(1), m.end(), text[m.end():m.end() + 1])
                    return out if out else m.group(0)
                text = re.sub(r'\(\*\*\(code \*\*\)\(\*\(int \*\)\(' + ptr + r' \+ ' + off_re(member_off) + r'\) \+ (0x[0-9a-f]+|\d+)\)\)\s*\(', vcall, text)
                text = re.sub(r'\*\(int \*\)\(' + ptr + r' \+ ' + off_re(member_off + 4) + r'\)', f'__thing_valid({recv})', text)
                text = re.sub(r'\(CScriptThing \*\)\(' + ptr + r' \+ ' + off_re(member_off) + r'\)', recv, text)
                # bare member address as a by-reference argument, also parenthesised after a cast was stripped
                text = re.sub(r'(?<![\w*(])' + ptr + r' \+ ' + off_re(member_off) + r'(?![\w])', recv, text)
                text = re.sub(r'\(' + ptr + r' \+ ' + off_re(member_off) + r'\)', recv, text)
                text = re.sub(r'CScriptThing::operator=\(' + re.escape(recv) + r',\s*([^;]+)\);',
                              lambda m, a=a, idx=idx, tname=tname: f'QUESTTHING_Set(__key("{a["name"]}_" .. {idx} .. "_{tname}"), {_thing_source(m.group(1))});', text)
    text = re.sub(r'\b(\d+(?:\.\d+)?)e\+?(-?\d+)\b', lambda m: repr(float(m.group(0))), text)
    # a remaining CScriptThing::operator= on a plain local is a handle copy (members were lowered above)
    text = re.sub(r'^([ \t]*)CScriptThing::operator=\(\(CScriptThing \*\)&?(\w+),\s*\((?:int|CScriptThing \*)\)(\w+)\);', r'\1\2 = \3;', text, flags=re.M)
    text = drop_local_counted_releases(text)
    text = fold_local_thing_copies(text)
    text = re.sub(r'^[ \t]*(?:[A-Za-z]+Stack_|local_)[0-9a-f]+ = (?:\(\w+\))?this;[ \t]*\r?\n', '', text, flags=re.M)
    text = re.sub(r'^[ \t]*(\w+) = \((?:\w+ \*+)\)\w+;[ \t]*\r?\n(?=[ \t]*\1 = )', '', text, flags=re.M)
    text = drop_dead_local_stores(text)
    text = rename_scalar_stack_locals(text)
    text = fold_local_string_vectors(text, getattr(spec, 'call_labels', None) or {})
    text = fold_counted_map_stores(text, getattr(spec, 'call_labels', None) or {})
    text = fold_drifted_actor_values(text)
    return text, diag


def fold_actor_map_releases(text: str) -> str:
    """`RESOURCE_ReleaseResource(map)` on an ACTOR MAP is impossible (a map is destroyed, never released): the export put
    the hero resource's destructor at the map's slot (CheckFriendlyAttacks 0x00D45060's BADHERO block: `ReleaseResource
    (actorMap)` raised "Invalid or released retail resource" in-game, 2026-09-21). The operand is the nearest resource
    acquired above that has no release between its acquire and this line."""
    maps = set(re.findall(r'^[ \t]*(\w+) = ACTORMAP_New\(\)', text, re.M))
    if not maps:
        return text
    def fix(m):
        if m.group(2) not in maps:
            return m.group(0)
        head = text[:m.start()]
        # (the acquire is still `GSI->StartScriptingEntity(thing, &res, n)` here; lower_after_annotate respells it)
        for acq in reversed(list(re.finditer(r'RESOURCE_TryAcquire\((\w+),|GSI->StartScriptingEntity\([^,;]+,\s*&?(\w+),', head))):
            res = acq.group(1) or acq.group(2)
            if not re.search(r'RESOURCE_ReleaseResource\(' + re.escape(res) + r'\)', head[acq.end():]):
                return f'{m.group(1)}RESOURCE_ReleaseResource({res});'
        return m.group(0)
    return re.sub(r'^([ \t]*)RESOURCE_ReleaseResource\((\w+)\);', fix, text, flags=re.M)


def fold_drifted_actor_values(text: str) -> str:
    """An actor-map value is a resource object. `ACTORMAP_Set(map, "HERO", &xStack_204_2)` names a slot at which no
    resource was ever constructed while one WAS acquired 4 bytes below (`xStack_200`, the hero's TryAcquire): the
    decompiler's 4-byte ESP drift (see `_drifted_byte_slices`) on a byte-split address operand -- the Will
    Guildmaster 0x00D5E0C0, where the bytes push the same `[esp+0x21c]` to TryAcquire and to the map store. The
    operand is that resource; any other unknown value is left as it is."""
    resources = set(re.findall(r'^[ \t]*(\w+) = RESOURCE_NewResource\(\)', text, re.M)) | set(re.findall(r'RESOURCE_TryAcquire\((\w+),', text))
    def fix(m):
        name = m.group(3)
        if name in resources:
            return m.group(0)
        slot = re.fullmatch(r'(\w*Stack_)([0-9a-f]+)(?:_\d+)?', name)
        if not slot:
            return m.group(0)
        below = f'{slot.group(1)}{int(slot.group(2), 16) - 4:x}'
        return f'{m.group(1)}ACTORMAP_Set({m.group(2)}, {below})' if below in resources else m.group(0)   # (resources are spelled bare by now)
    return re.sub(r'^([ \t]*)ACTORMAP_Set\(([^;]*?, "[^"]*"), &(\w+)\)', fix, text, flags=re.M)


def fold_counted_map_stores(text: str, call_labels: dict) -> str:
    """A resource stored into a cutscene actor map through the out-of-line counted-pointer assignment
    (Gameflow's credits: `key ctor; node = map::operator[](map, &key); assign((P *)(node + 8), (int)res);
    key dtor`) is `ACTORMAP_Set(map, "key", &res)`. Runs after the slot canonicalisation, when the key
    literal is resolved and the resource object carries its own name."""
    for label, target in call_labels.items():
        if target not in COUNTED_ASSIGN:
            continue
        text = re.sub(r'^(?P<ind>[ \t]*)CCharString::CCharString\(\(?(?:CCharString \*\))?&?(?P<key>\w+),(?P<keyval>"[^"]*"),-1\);[ \t]*\r?\n'
                      r'[ \t]*(?P<node>\w+) = std::\s*map<CCharString,CCountedPointer<[^;]*?::operator\[\]\((?:\(\s*map<[^;]*?\*\))?\(?&?(?P<map>\w+)\)?,(?:\(CCharString \*\))?&?(?P=key)\);[ \t]*\r?\n'
                      r'(?:[ \t]*[\w:<>]+\((?P=node),\(int\)\w+\);[ \t]*\r?\n)?'     # (the no-op node helper, when not yet dropped)
                      r'[ \t]*' + re.escape(label).replace('::', r'::\s*') + r'\s*\(\([\w<> :*]+\)\((?P=node) \+ 8\),\(int\)(?P<src>\w+)\);[ \t]*\r?\n'
                      r'(?:[ \t]*std::\s*_Cons_val<[^;(]*?\s*\(&(?P=key)\);[ \t]*\r?\n)?',
                      lambda m: f'{m.group("ind")}ACTORMAP_Set({m.group("map")}, {m.group("keyval")}, {m.group("src")});\n', text, flags=re.M)
    return text


# std::vector<CCharString> on the stack (three pointers; Ghidra types the slot CScriptThing): push_back 0x44BFF0
# (grows by 4, copy-constructs through CCharString 0x99EC30) and the destructor 0x414EA0 (destroys each 4-byte
# element through 0x99EAE0, frees the buffer). Both disassembly-verified; the bsim labels on them are noise
# (`std::vector::push_back`, `CDefendingCombatantInfo::CCombatWheel::ResetRings`).
VECTOR_CCHARSTRING_PUSH_BACK = 0x44BFF0
VECTOR_CCHARSTRING_DTOR = 0x414EA0


def fold_local_string_vectors(text: str, call_labels: dict) -> str:
    """A local `std::vector<CCharString>` filled with literals and handed to a GSI call (Gameflow's
    `ActivateMultipleQuestsWithoutLoadingResources`) becomes a Lua sequence: `vec = LOCALLIST_NewStrings();`
    before the first push of a run, `LOCALLIST_PushString(vec, "literal")` per element (the literal's
    constructor and destructor lines fold in), and the vector destructor call is dropped."""
    pushes = {label for label, addr in call_labels.items() if addr == VECTOR_CCHARSTRING_PUSH_BACK}
    dtors = {label for label, addr in call_labels.items() if addr == VECTOR_CCHARSTRING_DTOR}
    if not pushes:
        return text
    spell = lambda labels: '|'.join(re.escape(l) for s in labels for l in {s, s.replace('::', '__'), s.split('::')[-1]})
    push_re = re.compile(r'^(?P<ind>[ \t]*)(?:' + spell(pushes) + r')\(&?(?P<vec>\w+),\s*(?:\(int\))?&?(?P<elem>\w+)\);[ \t]*$')
    dtor_re = re.compile(r'^[ \t]*(?:' + spell(dtors) + r')\((?:\([\w *]+\))?&?(?P<vec>\w+)\);[ \t]*$') if dtors else None
    ctor_re = re.compile(r'^[ \t]*CCharString::CCharString\(&?(?P<slot>\w+),\s*(?P<lit>"(?:[^"\\]|\\.)*"),\s*-1\);[ \t]*$')
    elem_dtor_re = re.compile(r'^[ \t]*std::_(?:Cons|Dest)_val<[^;]*?>\s*\(&?(?P<slot>\w+)\);[ \t]*$')
    lines = text.split('\n')
    out, open_vectors = [], set()
    i = 0
    while i < len(lines):
        line = lines[i]
        m = push_re.match(line)
        if m:
            vec, elem, ind = m.group('vec'), m.group('elem'), m.group('ind')
            value = elem
            prev = out[-1] if out else ''
            c = ctor_re.match(prev)
            if c and c.group('slot') == elem:
                value = c.group('lit')
                out.pop()
            if vec not in open_vectors:
                out.append(f'{ind}{vec} = LOCALLIST_NewStrings();')
                open_vectors.add(vec)
            out.append(f'{ind}LOCALLIST_PushString({vec}, {value});')
            nxt = lines[i + 1] if i + 1 < len(lines) else ''
            d = elem_dtor_re.match(nxt)
            if value != elem and d and d.group('slot') == elem:
                i += 1     # the literal temporary's destructor
            i += 1
            continue
        if dtor_re and (d := dtor_re.match(line)) and d.group('vec') in open_vectors:
            open_vectors.discard(d.group('vec'))
            i += 1
            continue
        for vec in list(open_vectors):
            # any other definition of the slot (a thing result, an actor map) ends the vector's life
            if re.match(r'^[ \t]*' + re.escape(vec) + r' = ', line) or re.search(r'\(' + re.escape(vec) + r',\s*&', line):
                open_vectors.discard(vec)
        out.append(line)
        i += 1
    return '\n'.join(out)


RE_DEAD_STORE = re.compile(r'^[ \t]*(this_\d+|(?:[A-Za-z]+Stack_|local_)[0-9a-f]+|[A-Za-z]{1,5}Var\d+(?:_\d+)?)(?:\[0\])? = ([^;]+);[ \t]*\r?\n', re.M)


def drop_dead_local_stores(text: str) -> str:
    '''Destructor-selection aliases (`this_00 = &CStack_18;`), by-value staging of `this`
    (`CStack_4 = (CCharString)this;`) and inlined-constructor residue (`ppuStack_80[0] = 0;`) survive the
    folds when their object was lowered away. A store to a local never read elsewhere, whose value is a
    call-free expression, has no effect and is dropped (declarations are not references).'''
    names = []
    for m in RE_DEAD_STORE.finditer(text):
        if m.group(1) not in names:
            names.append(m.group(1))
    for var in names:
        # positions are recomputed per name: earlier names may have removed lines
        ms = [m for m in RE_DEAD_STORE.finditer(text) if m.group(1) == var]
        dead = [m for m in ms if not (re.search(r'\w\s*\(|\)\s*\(|->', re.sub(r'\((?:\w+ \*+|\w+)\)', '', m.group(2))) or '=' in m.group(2))]
        if not dead:
            continue      # every store carries a call or nested assignment: keep
        v = re.escape(var)
        dtor = re.compile(r'^[ \t]*std::\s*_Cons_val<[^;(]*?\s*\(' + v + r'\);[ \t]*\r?\n', re.M)
        decl = re.compile(r'^[ \t]*[\w :*]+\b' + v + r'(?:\s*\[\d+\])?;[ \t]*\r?\n', re.M)
        any_store = re.compile(r'(?<![\w.>])' + v + r'(?:\[0\])? = (?!=)')
        if len(dead) == len(ms):
            rest = dtor.sub('', decl.sub('', re.sub(r'^[ \t]*' + v + r'(?:\[0\])? = [^;]+;[ \t]*\r?\n', '', text, flags=re.M)))
            if not re.search(r'\b' + v + r'\b', rest):
                text = dtor.sub('', re.sub(r'^[ \t]*' + v + r'(?:\[0\])? = [^;]+;[ \t]*\r?\n', '', text, flags=re.M))
            continue
        # some stores carry calls: a call-free *pointer* store (`pCVar7 = &xStack_44;`, a destructor-selection
        # alias) is dead when nothing reads the local before its next store; scalar stores stay — a later
        # store inside a loop body may not run before the read after the loop
        for m in reversed(dead):
            if not re.match(r'^(?:\(\w+ \*+\))?&?(?:[A-Za-z]+Stack_|local_|this\b)', m.group(2).strip()):
                continue
            tail = text[m.end():]
            nxt = any_store.search(tail)
            region = tail[:nxt.start()] if nxt else tail
            if not re.search(r'\b' + v + r'\b', dtor.sub('', region)):
                text = text[:m.start()] + dtor.sub('', region) + (tail[nxt.start():] if nxt else '')
    return text


GSI_RECEIVER = r'(?:(?:\(\w+ \*\*?\))?\*\((?:int|void|undefined4|CScriptThing_bv|CCharString_bv|C3DVector_bv) \*\*\)\(this \+ (?:4|0x40)\)|DAT_0143e8f8)'
ME_RECEIVER = r'(?:\(CScriptThing(?:_bv)? \*\))?\(this \+ 8\)'


def strip_receiver_arguments(text: str) -> str:
    """Typed exports show the __thiscall receiver as an explicit first argument of overridden
    calls; the lifter's `GSI->Name(` / `CScriptThing::Name(me, ` forms carry it implicitly."""
    text = re.sub(r'(GSI->\w+\()' + GSI_RECEIVER + r'(?:,\s*|(?=\)))', r'\1', text)
    # aliases of the interface pointer (`this_00 = *(int **)(this + 0x40);`) as explicit receivers
    aliases = set(re.findall(r'^[ \t]*(\w+) = \*\((?:int|void|undefined4) \*\*\)\(this \+ (?:4|0x40)\);', text, re.M))
    aliases |= {m.group(1) for m in re.finditer(r'^[ \t]*(\w*Stack_[0-9a-f]+) = (\w+);', text, re.M) if m.group(2) in aliases}
    for alias in aliases:
        text = re.sub(r'(GSI->\w+\()' + re.escape(alias) + r'(?:,\s*|(?=\)))', r'\1', text)
        text = re.sub(r'^[ \t]*' + re.escape(alias) + r' = (?:\w+|\*\((?:int|void|undefined4) \*\*\)\(this \+ (?:4|0x40)\));[ \t]*\r?\n', '', text, flags=re.M) if 'Stack_' in alias else text
    # a REGISTER alias (`CVar10 = *(int **)(this + 4);`, a string temporary elsewhere in the function -- the Skill
    # Guildmaster's out-of-ring check 0x00D5AE70) and the compiler's spill of it (`CStack_1e8 = CVar10;`) are dead
    # once the `GSI->` calls no longer read it before the register's next assignment
    lines = text.split('\n')
    i = 0
    while i < len(lines):
        m = re.match(r'[ \t]*(\w+) = \*\((?:int|void|undefined4) \*\*\)\(this \+ (?:4|0x40)\);', lines[i])
        if m and m.group(1) in aliases and 'Stack_' not in m.group(1):
            alias = re.escape(m.group(1))
            j, spills, other = i + 1, [], False
            while j < len(lines) and not re.match(r'[ \t]*' + alias + r' = ', lines[j]):
                if re.match(r'[ \t]*\w*Stack_[0-9a-f]+ = ' + alias + r';', lines[j]):
                    spills.append(j)
                elif re.search(r'(?<![\w.>])' + alias + r'\b', lines[j]):
                    other = True
                j += 1
            if not other:
                for k in sorted(spills + [i], reverse=True):
                    del lines[k]
                continue
        i += 1
    text = '\n'.join(lines)
    text = re.sub(r'(CScriptThing::\w+\(me, ?)' + ME_RECEIVER + r'(?:,\s*|(?=\)))', r'\1', text)
    # a pointer alias of the entity's own thing (`p0 = (CScriptThing *)(this + 8);`) as the explicit receiver
    # next to the annotated one: `MsgIsHitBySpecialAbilityFrom((CScriptThing *)(this + 8), p0, 0xe, &name)` --
    # the int slot took `p0` (-> `me`) and the ability enum fell off (TraderConflict bandits/villager/guard,
    # 2026-09-20 audit)
    for alias in set(re.findall(r'^[ \t]*(\w+) = ' + ME_RECEIVER + r';', text, re.M)):
        receiver = r'(?:\(void \*\))?' + re.escape(alias)
        text = re.sub(r'(CScriptThing::\w+\(' + ME_RECEIVER + r', ?)' + receiver + r'(?:,\s*|(?=\)))', r'\1', text)
        text = re.sub(r'(CScriptThing::\w+\(me, ?)' + receiver + r'(?:,\s*|(?=\)))', r'\1', text)
    text = re.sub(r'(CScriptThing::\w+\((\w+), ?)\2(?:,\s*|(?=\)))', r'\1', text)
    text = re.sub(r'(CScriptThing::\w+\((LOCALLIST_At\([^()]*\)), ?)(?:\(int \*\))?\2(?:,\s*|(?=\)))', r'\1', text)   # an element receiver repeated as the explicit `this`
    # typed by-value placeholders read as their real classes
    text = text.replace('_bv *', ' *').replace('_bv)', ')')
    return text


LUA_PSEUDO = [
    (re.compile(r'QUESTTHING_Get\('), 'quest:GetStateThing('),
    (re.compile(r'QUESTTHING_Set\('), 'quest:SetStateThing('),
    (re.compile(r'ENTITYTHING_Get\('), '__native_entity_state:GetStateThing('),
    (re.compile(r'ENTITYTHING_Set\('), '__native_entity_state:SetStateThing('),
    (re.compile(r'ENTITYSTATE_GetInt\('), '__native_entity_state:GetStateInt('),
    (re.compile(r'ENTITYSTATE_SetInt\('), '__native_entity_state:SetStateInt('),
    (re.compile(r'QUESTLIST_Copy\('), 'quest:GetStateListCopy('),
    (re.compile(r'ENTITYLIST_Copy\('), '__native_entity_state:GetStateListCopy('),
    (re.compile(r'QUESTLIST_Count\('), 'quest:GetStateListCount('),
    (re.compile(r'QUESTLIST_At\('), 'quest:GetStateListAt('),
    (re.compile(r'QUESTLIST_Push\('), 'quest:StateListPush('),
    (re.compile(r'QUESTLIST_Erase\('), 'quest:StateListErase('),
    (re.compile(r'QUESTLIST_Clear\('), 'quest:StateListClear('),
    (re.compile(r'ENTITYLIST_Clear\('), '__native_entity_state:StateListClear('),
    (re.compile(r'QUESTLIST_Set\('), 'quest:StateListSet('),
    (re.compile(r'ENTITYLIST_Set\('), '__native_entity_state:StateListSet('),
    (re.compile(r'ENTITYLIST_Erase\('), '__native_entity_state:StateListErase('),
    (re.compile(r'QUESTLIST_(?:BeginValue|Ref)\('), 'quest:GetStateListRef('),
    (re.compile(r'QUESTLIST_EndValue\('), 'quest:GetStateListEnd('),
    (re.compile(r'ENTITYLIST_(?:BeginValue|Ref)\('), '__native_entity_state:GetStateListRef('),
    (re.compile(r'ENTITYLIST_EndValue\('), '__native_entity_state:GetStateListEnd('),
    (re.compile(r'ENTITYLIST_Push\('), '__native_entity_state:StateListPush('),
    (re.compile(r'ENTITYLIST_Count\('), '__native_entity_state:GetStateListCount('),
    (re.compile(r'ENTITYLIST_At\('), '__native_entity_state:GetStateListAt('),
    (re.compile(r'ACTORMAP_New\('), 'resources:NewActorMap('),
    (re.compile(r'QUESTTHING_Empty\(\)'), 'nil'),
    (re.compile(r'ENGINE_LostOperand\(\)'), 'nil --[[operand lost by the decompiler]]'),
    (re.compile(r'ENGINE_Rand\(\)'), 'math.random(0, 32767)'),
    (re.compile(r'ENGINE_EmptyString\(\)'), '""'),
    (re.compile(r'ENGINE_ZeroVector\(\)'), '{x = 0, y = 0, z = 0}'),
    (re.compile(r'ENGINE_VectorCopy\((\w+)\)'), r'{x = \1.x, y = \1.y, z = \1.z}'),
    (re.compile(r'ENGINE_Vector3\(([^,()]+), ([^,()]+), ([^,()]+)\)'), r'{x = \1, y = \2, z = \3}'),
    (re.compile(r'ENGINE_GlobalGameDataFloatAt\('), 'quest:ReadGlobalGameDataFloatAt('),
    (re.compile(r'ENGINE_GlobalGameDataFloat\('), 'quest:ReadGlobalGameDataFloat('),
    (re.compile(r'ENGINE_GlobalGameDataString\('), 'quest:ReadGlobalGameDataString('),
    (re.compile(r'ENGINE_GlobalGameData\('), 'quest:ReadGlobalGameData('),
    (re.compile(r'ACTORMAP_Set\('), 'resources:SetActor('),
    (re.compile(r'ACTORMAP_Destroy\('), 'resources:DestroyActorMap('),
    # retail 0xCD23B9 tests the resource's counted handle ([this+8] != 0). A constant `false` held only for a freshly
    # constructed resource: V_TourGuide's guide acquires first, and its closing-time walk to M_TG_ClosingTimeExit sat
    # behind `if false and ...` (2026-09-24). An unacquired resource yields an empty thing, so this is false there too.
    (re.compile(r'RESOURCE_IsAcquired\((\w+)\)'), r'(not resources:ScriptThing(\1):IsNull())'),
    (re.compile(r'LOCALLIST_Count\((\w+)(?:\[0 \+ 1\])?\)'), r'#\1'),   # `vec[0 + 1]` is the vector's begin field, not an element
    (re.compile(r'LOCALLIST_Append\((\w+), (\w+)\)'),
     r'for _, appendedThing in ipairs(\2) do \1[#\1 + 1] = appendedThing end'),
    (re.compile(r'\bGFCharStringToInt\('), 'tonumber('),               # ?GFCharStringToInt@@YIJABVCCharString@@@Z (0x99E7F0)
    (re.compile(r'STRINGMAP_New\('), 'resources:NewStringMap('),
    (re.compile(r'STRINGMAP_Set\('), 'resources:SetString('),
    (re.compile(r'STRINGMAP_Destroy\('), 'resources:DestroyStringMap('),
    (re.compile(r'RESOURCE_(\w+)\('), r'resources:\1('),
    (re.compile(r'QUESTSTATE_(Get|Set)(Int|Bool|Float|String)\('), r'quest:\1State\2('),
    (re.compile(r'ENTITYSTATE_(Get|Set)(Int|Bool|Float|String)\('), r'__native_entity_state:\1State\2('),
    (re.compile(r'GSI->(Get|Set)State(Int|Bool|Float|String|Thing)\('), r'quest:\1State\2('),
    (re.compile(r'GSI->(Get|Set)MasterGameState\('), r'quest:\1MasterGameState('),
]
KEY = re.compile(r'__key\(')


def _split_top(args):
    out, depth, cur, quote = [], 0, '', None
    for ch in args:
        if quote:
            cur += ch
            if ch == quote:
                quote = None
            continue
        if ch in '"\'':
            quote = ch
        elif ch in '([{':
            depth += 1
        elif ch in ')]}':
            depth -= 1
        elif ch == ',' and depth == 0:
            out.append(cur.strip()); cur = ''
            continue
        cur += ch
    if cur.strip():
        out.append(cur.strip())
    return out


def _expand_calls(text, name, render):
    """Replace every `name(<balanced args>)` with render(args_list), innermost first."""
    while True:
        i = text.find(name + '(')
        if i < 0:
            return text
        j, depth = i + len(name) + 1, 1
        while j < len(text) and depth:
            depth += {'(': 1, ')': -1}.get(text[j], 0)
            j += 1
        inner = text[i + len(name) + 1:j - 1]
        text = text[:i] + render(_split_top(inner)) + text[j:]


def finish_lua(text: str) -> str:
    """Turn lowering pseudo-calls into Lua after the lifter has run."""
    text = re.sub(r'ENGINE_(IsDistanceBetweenThings(?:Under|Over))\(', r'quest:\1(', text)
    text = _expand_calls(text, '__thing_valid', lambda a: f'({a[0]} ~= nil and not {a[0]}:IsNull())')
    text = _expand_calls(text, 'ENGINE_Colour', lambda a: '{R = %s, G = %s, B = %s, A = %s}' % tuple(a))
    text = _expand_calls(text, 'ENGINE_Concat', lambda a: '(' + ' .. '.join(a) + ')')
    text = _expand_calls(text, 'ENGINE_IntToString', lambda a: f'tostring({a[0]})')
    text = _expand_calls(text, 'ENGINE_StrNotEqual', lambda a: f'(({a[0]} ~= {a[1]}) and 1 or 0)')
    text = _expand_calls(text, 'ENGINE_SquaredDistance', lambda a: f'(quest:GetDistanceBetweenThings({", ".join(a)}) ^ 2)')
    text = _expand_calls(text, 'LOCALLIST_At', lambda a: f'{a[0]}[{a[1]} + 1]')
    # a local array of resource objects (fold_local_resource_arrays): a Lua list of fresh resources
    text = _expand_calls(text, 'RESLIST_At', lambda a: f'{a[0]}[{a[1]} + 1]')
    text = _expand_calls(text, 'RESLIST_New', lambda a: f'(function(n) local t = {{}} for i = 1, n do t[i] = resources:NewResource() end return t end)({a[0]})')
    text = re.sub(r'^([ \t]*)RESLIST_Destroy\((\w+)\)', r'\1for _, r in ipairs(\2) do resources:ReleaseResource(r) end', text, flags=re.M)
    text = _expand_calls(text, 'LOCALLIST_Erase', lambda a: f'table.remove({a[0]}, {a[1]} + 1)')
    text = _expand_calls(text, 'LOCALLIST_PushString', lambda a: f'table.insert({a[0]}, {a[1]})')
    text = re.sub(r'LOCALLIST_NewStrings\(\)', '{}', text)
    # a word list returned by a sidecar binding (MsgGetThingsKilledGroups): length, 0-based word, reset
    text = _expand_calls(text, 'ENGINE_ListLen', lambda a: f'#{a[0]}' if re.fullmatch(r'\w+', a[0]) else f'#({a[0]})')
    text = _expand_calls(text, 'ENGINE_ListWord', lambda a: f'{a[0]}[({a[1]}) + 1]')
    text = _expand_calls(text, 'ENGINE_EmptyList', lambda a: '{}')
    text = _expand_calls(text, 'ENGINE_Trunc', lambda a: f'math.tointeger(math.modf({a[0]}))')   # integral part (truncated toward zero), one value in every operand position
    # Ghidra's `ABS(x)` is the x87 `fabs` (PreMeleeWhisper 0x00D5282C-0x00D5283D: `fld; fabs; fcomp [1.0]; fnstsw; test ah,0x41; jp`)
    text = re.sub(r'(?<![\w.:])ABS\(', 'math.abs(', text)
    text = _expand_calls(text, 'ENGINE_StrEq', lambda a: f'({a[0]} == {a[1]})' if len(a) == 2 else 'ENGINE_StrEq(' + ', '.join(a) + ')')
    text = _expand_calls(text, 'ENGINE_StrCmp', lambda a: f'(({a[0]} == {a[1]}) and 0 or 1)' if len(a) == 2 else 'ENGINE_StrCmp(' + ', '.join(a) + ')')
    text = _expand_calls(text, 'ENGINE_Round', lambda a: f'math.floor(({a[0]}) + 0.5)' if len(a) == 1 else 'ENGINE_Round(' + ', '.join(a) + ')')
    text = text.replace('ENGINE_IsAllDead(', '__native_all_dead(')     # the helper is defined per file by convert_quest_unit
    # its parameter is the vector itself: an element index here is the list rewrite overreaching
    text = re.sub(r'__native_all_dead\((\w+)\[[^\]]*\]\)', r'__native_all_dead(\1)', text)
    text = re.sub(r'(QUEST|ENTITY)LIST_At_(\w+)\(', lambda m: ('quest:GetStateListAt(' if m.group(1) == 'QUEST' else '__native_entity_state:GetStateListAt(') + '"' + m.group(2) + '", ', text)
    for pattern, repl in LUA_PSEUDO:
        text = pattern.sub(repl, text)
    text = KEY.sub('(', text)
    text = re.sub(r'&("[^"]*")', r'\1', text)
    return text
