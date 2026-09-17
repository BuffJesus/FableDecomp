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
    r'^[ \t]*if \(\((\w+(?:\._\d_4_|\[\d\])?) != \((?:int \*|\w+)\)0x0\) && \(\*\(int \*\)\1 = \*\(int \*\)\1 \+ -1, \*\(int \*\)\1 == 0\)\) \{\s*\r?\n'
    r'[ \t]*\(\*\*\(code \*\*\)\(\(int\)\1 \+ 4\)\)\(\);\s*\r?\n[ \t]*operator_delete\(\(void \*\)\1\);\s*\r?\n[ \t]*\}[ \t]*\r?\n', re.M)
RE_SLOT_ZERO = re.compile(r'^[ \t]*(?:\w+\._\d+_4_ = 0;|(?:[A-Za-z]+Stack_|local_)[0-9a-f]+ = \(int \*\)0x0;)[ \t]*\r?\n', re.M)


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


def fold_stack_colours(text: str) -> str:
    """A CRGBColour built on the stack byte by byte (`c._0_1_ = B; c._1_1_ = G; c._2_1_ = R; c._3_1_ = A`,
    retail ABI is BGRA) and passed by address becomes an FSE colour table."""
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
        nxt = re.search(r'^[ \t]*(?:\w+::\w+\(\(\w+ \*\)&' + re.escape(var) + r'\b|' + re.escape(var) + r'(?:\._\d_1_)? = )', scope, re.M)
        use, rest = (scope[:nxt.start()], scope[nxt.start():]) if nxt else (scope, '')
        use = re.sub(r'(?:\(\w+ \*\))?&?' + re.escape(var) + r'\b', f'ENGINE_Colour({r}, {g}, {b}, {a})', use)
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
        nxt = re.search(r'^[ \t]*' + re.escape(var) + r' = ', tail, re.M)
        scope, rest = (tail[:nxt.start()], tail[nxt.start():]) if nxt else (tail, '')
        scope = re.sub(r'\(\*\*\(code \*\*\)\(' + re.escape(var) + r' \+ ', f'(**(code **)({alias} + ', scope)
        scope = re.sub(r'\(\*\*\(code \*\*\)\(\(int\)' + re.escape(var) + r' \+ ', f'(**(code **)({alias} + ', scope)
        scope = re.sub(r'\(\*\*\(' + re.escape(var) + r' \+ ', f'(**(code **)({alias} + ', scope)     # the untyped vcall spelling `(**(X + 0x118))(`
        if re.search(r'\b' + re.escape(var) + r'\b', scope):
            pos = m.end()          # other uses of the same temporary: leave this load alone
            continue
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
    return RE_SLOT_ZERO.sub('', text)


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


def bind_st0_results(text: str) -> str:
    """Ghidra drops the float (ST0) result of an overridden vtable call and reads it back as
    `extraout_ST0[_NN]`; each such name belongs to the nearest preceding call statement whose result
    was not assigned. Bind it: `fret_NN = <call>;` and use `fret_NN` where the extraout name was read."""
    names = sorted(set(re.findall(r'\bextraout_ST0(?:_\d+)?\b', text)), key=lambda n: (len(n), n))
    if not names:
        return text
    lines = text.split('\n')
    for name in names:
        first = next((k for k, l in enumerate(lines) if re.search(r'\b' + re.escape(name) + r'\b', l)
                      and not re.match(r'^\s*float10 ' + re.escape(name) + r';', l)), None)
        if first is None:
            continue
        fret = 'fret_' + (name.split('_', 2)[2] if name.count('_') == 2 else '0')
        for k in range(first - 1, -1, -1):
            l = lines[k]
            if re.match(r'^\s*(?:\(\*\*\(code \*\*\)|GSI->|[\w:]+::[\w~]+\s*\(|\w+\()', l) and l.rstrip().endswith(');') and ' = ' not in l.split('(')[0]                     and not re.match(r'^\s*(?:std::|NHeroInformationScreens::|C\w+::(?:~?C\w+|_\w+)\s*\(|operator_(?:delete|new)\(|\(\*\(code \*\))', l):    # not a ctor/dtor/release of a temp
                lines[k] = re.sub(r'^(\s*)', r'\1' + fret + ' = ', l, count=1)
                break
        else:
            continue
        lines = [re.sub(r'\b' + re.escape(name) + r'\b', fret, l) for l in lines]
        lines = [l for l in lines if not re.match(r'^\s*float10 ' + re.escape(fret) + r';', l)]
    return '\n'.join(lines)


EH_FLAG_SHAPES = [
    re.compile(r'^[ \t]*(?:byte|uint|undefined4|uchar|int) (?P<f>\w+);[ \t]*$'),                      # declaration
    re.compile(r'^[ \t]*(?P<f>\w+) = !(\w+) && \2;[ \t]*$'),                                        # init (false)
    re.compile(r'^[ \t]*(?P<f>\w+) = (?:0|\(uint\)bVar\d+);[ \t]*$'),                                # init (zero / a cleared bool)
    re.compile(r'^[ \t]*(?P<f>\w+) = (?P=f) [|&] (?:0x[0-9a-f]+|\d+);[ \t]*$'),                     # set / clear a bit
    re.compile(r'^[ \t]*(?:\} else )?if \(\((?P<f>\w+) & (?:0x[0-9a-f]+|\d+)\) [!=]= 0\) \{[ \t]*$'),   # bit test guard
    re.compile(r"^[ \t]*(?:\} else )?if \(\(char\)(?P<f>\w+) < '\\0'\) \{[ \t]*$"),               # top-bit test guard
    re.compile(r"^[ \t]*(?:\} else )?if \(-1 < \(char\)(?P<f>\w+)\) \{[ \t]*$"),
]


def drop_eh_state_flags(text: str) -> str:
    """The compiler's exception-state byte (`bVar7 = !b && b; bVar7 |= 2; ... if ((bVar7 & 2) != 0) { bVar7 &= 0xfd;
    ~CCharString(...) }`) only guards destructor calls on the unwinding path. When every line that names a
    register is one of those shapes, its bit tests are constant (never taken) and its updates vanish. A copy
    through a second register (`uVar17 = uStack_7c | 0x40; ... uStack_7c = uVar17;`) is folded first; the
    second register may be reused for unrelated values outside that window."""
    seeds = {m.group(1) for m in re.finditer(r'^[ \t]*(\w+) = !(\w+) && \2;', text, re.M)}
    seeds |= {m.group(1) for m in re.finditer(r'^[ \t]*(\w+) = \1 \| (?:0x[0-9a-f]+|\d+);', text, re.M)}
    seeds |= {m.group(2) for m in re.finditer(r'^[ \t]*(\w+) = (\w+) \| (?:0x[0-9a-f]+|\d+);', text, re.M)}
    for flag in sorted(seeds):
        v = re.escape(flag)
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
    text = re.sub(r'return CONCAT31\([^;]*?,\s*(0|1)\);', lambda m: f'return {"true" if m.group(1) == "1" else "false"};', text)
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
    text = re.sub(r'&("(?:[^"\\]|\\.)*")', r'\1', text)                      # &"literal" (propagated CCharString temp)
    text = re.sub(r'\*\((\w+ \*+)\)&(\w+)->field_0x([0-9a-f]+)', r'*(\1)(\2 + 0x\3)', text)
    text = re.sub(r'\*&(\w+)->field_0x([0-9a-f]+)', r'*(int *)(\1 + 0x\2)', text)
    text = re.sub(r'(\w+)->field_0x([0-9a-f]+)', r'*(int *)(\1 + 0x\2)', text)
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
    # an integer counter kept in a slot Ghidra typed CCharString (a byte offset stepping through a vector):
    # `X = (CCharString)((int)X + 0xc);` with its `X = (CCharString)0x0;` start
    counters = set(re.findall(r'^[ \t]*(\w+) = \(CCharString\)\(\(int\)\1 \+ (?:0x[0-9a-f]+|\d+)\);', text, re.M))
    for var in counters:
        v = re.escape(var)
        text = re.sub(r'^([ \t]*)' + v + r' = \(CCharString\)\(\(int\)' + v + r' \+ (0x[0-9a-f]+|\d+)\);', r'\1' + var + r' = ' + var + r' + \2;', text, flags=re.M)
        text = re.sub(r'^([ \t]*)' + v + r' = \(CCharString\)0x0;', r'\1' + var + ' = 0;', text, flags=re.M)
        text = re.sub(r'\(int\)' + v + r'\b', var, text)
        # the slot doubles as a string temporary elsewhere (`&X`): the counter gets its own name
        text = re.sub(r'(?<![&\w])' + v + r'\b', 'ctr_' + var.split('_', 1)[1], text)
    # x87 compare idiom: `(a < b) != (a == b)` is `a <= b` (Ghidra's rendering of fcomp/fnstsw/test 0x41)
    text = re.sub(r'(\*?\(?[\w.]+\)?(?:\([^()]*\))?) < ((?:\(float10\))?\*?[\w.]+(?:\([^()]*\))?) != \(\1 == \2\)', r'\1 <= \2', text)
    # a float staged in a slot Ghidra typed as a CCharString array: `aCStack_1c[0] = (CCharString)(expr);`
    # read back as `(float)aCStack_1c[0]` -> a plain scalar local
    for m in list(re.finditer(r'^[ \t]*(\w+)\[0\] = \(CCharString(?:_bv)?\)', text, re.M)):
        var = m.group(1)
        scalar = 'f_stk_' + var.split('_', 1)[1] if '_' in var else 'f_' + var   # no `Stack_` in the name: a plain local to the lifter
        text = re.sub(r'\b' + re.escape(var) + r'\[0\] = \(CCharString(?:_bv)?\)([^;]+);', scalar + r' = \1;', text)
        text = re.sub(r'\(float\)' + re.escape(var) + r'\[0\]', scalar, text)
        # the same slot as an integer counter (`(int)aCStack_70[0] + 1`, `(uint)aCStack_70[0] < n`, `CVar = aCStack_70[0]`)
        text = re.sub(r'(?:\((?:int|uint)\))?\b' + re.escape(var) + r'\[0\](?!\s*=)', scalar, text)
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
RE_MAP_NEW = re.compile(
    r'^(?P<ind>[ \t]*)(?:(?P<m0>\w+) = \(undefined1 \*\)0x0;\s*)?(?P<map>\w+) = (?:\(\w+ \*\))?malloc\(0x24\);\s*'
    r'(?:\w+ = 0;\s*)?\*(?P=map) = 0;\s*\*\(undefined4 \*\)\((?P=map) \+ 4\) = 0;\s*'
    r'\*\(undefined1 \*\*\)\((?P=map) \+ 8\) = (?P=map);\s*\*\(undefined1 \*\*\)\((?P=map) \+ 0xc\) = (?P=map);[ \t]*\r?\n', re.M)
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
    r'(?:(?P<alias>\w+) = (?P<thing>&?[\w.]+);\s*)?(?:\w+ = 0x[0-9a-f]{6,7};\s*)?'
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


RE_BYTE_SPLIT = re.compile(
    r'^[ \t]*(\w+) = SUB41\((&?[\w.]+),0\);[ \t]*\r?\n'
    r'[ \t]*(\w+) = \(undefined1\)\(\(uint\)\2 >> 8\);[ \t]*\r?\n'
    r'[ \t]*(\w+) = \(undefined1\)\(\(uint\)\2 >> 0x10\);[ \t]*\r?\n'
    r'[ \t]*(\w+) = \(undefined1\)\(\(uint\)\2 >> 0x18\);[ \t]*\r?\n', re.M)


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
    return re.sub(r'CONCAT13\((\w+),\s*CONCAT12\((\w+),\s*CONCAT11\((\w+),\s*(\w+)\)\)\)', repl, text)


def fold_byte_split_pointers(text):
    '''A pointer pushed byte-wise (Ghidra: `u0 = SUB41(X,0); u1 = (undefined1)((uint)X >> 8); ...` then
    `CONCAT13(u3,CONCAT12(u2,CONCAT11(u1,u0)))`) is X at its one use.'''
    while (m := RE_BYTE_SPLIT.search(text)):
        u0, x, u1, u2, u3 = m.groups()
        concat = f'CONCAT13({u3},CONCAT12({u2},CONCAT11({u1},{u0})))'
        head, tail = text[:m.start()], text[m.end():]
        if concat not in tail:
            break
        tail = tail.replace(f'(void *){concat}', x, 1) if f'(void *){concat}' in tail else tail.replace(concat, x, 1)
        text = head + tail
    return text


def fold_actor_maps(text, resolve_string=None):
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
            src = m.group('thing') if m.group('thing').startswith('&') else '&' + m.group('thing')
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
COUNTED_RELEASE = {0x6E7AB0, 0xCE1000}   # CCountedPointer release: decref [this+4], zero [this], [this+4] (disasm 2026-09-16)
BASE_OBJECT_DTOR = {0x99A430}         # CBaseIntelligentPointer::~ (bsim: CPhysicsMeshInfo::~CPhysicsMeshInfo)
RESOURCE_ACQUIRED = {0xCD23B9}        # bool __thiscall (this): [this+8] != 0, the resource's counted handle (disasm 2026-09-16)
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
    r'(?:[ \t]*(?:\w+ = 0;|\w+ = \(\w+ \*\)0x0;|\w+\[\d\] = (?:\(\w+ \*\))?0x0;|\*\(\w+ \*\)\(\w+ \+ (?:4|8|0x8)\) = 0;)[ \t]*\r?\n){0,3}', re.M)


RE_INLINE_DTOR = re.compile(
    r'^(?P<ind>[ \t]*)(?P<obj>\w+)(?:\[0\]|\._0_4_)? = 0;[ \t]*\r?\n'
    r'(?:[ \t]*\w+(?:\[\d\]|\._\d+_4_)? = (?:\([\w ]+\**\))?0(?:x0)?;[ \t]*\r?\n){0,2}'
    r'[ \t]*(?P=obj)(?:\[0\]|\._0_4_)? = &PTR_[A-Za-z_]*_(?P<vt>0127094c|01260ef4|0126008c);[ \t]*\r?\n'
    r'[ \t]*[\w:~]+\s*\(\(\w+ \*\)(?P=obj)\);[ \t]*\r?\n', re.M)


def fold_inline_destructors(text):
    """The inlined resource / movie destructor after the counted release: the handle field is zeroed,
    the vtable reset to the base and the base destructor called. That is `ReleaseResource` (resource
    vtable 0127094c) or `DestroyMovie` (movie vtables) on the object."""
    # the base vtable (0126008c) is shared by resources and movies: the object's own construction decides
    resources = set(re.findall(r'\b(\w+) = RESOURCE_NewResource\(\)', text)) | set(re.findall(r'RESOURCE_TryAcquire\((\w+),', text))
    def repl(m):
        release = m.group('vt') == '0127094c' or (m.group('vt') == '0126008c' and m.group('obj') in resources)
        return f'{m.group("ind")}{"RESOURCE_ReleaseResource" if release else "RESOURCE_DestroyMovie"}({m.group("obj")});\n'
    return RE_INLINE_DTOR.sub(repl, text)


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
            text = re.sub(r'^([ \t]*)(?:(\w+) = (?:\(CScriptThing \*\)\s*)?)?' + re.escape(label).replace('::', r'\s*::\s*') + r'\s*\(([^;,]+?),\s*([^;]+?)\);',
                          script_thing, text, flags=re.M)
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
            pat = re.compile(r'^([ \t]*)(\w+) = ' + re.escape(label).replace('::', r'\s*::\s*') + r'\s*\(&?(\w+),\s*(?:\(CCharString \*\))?&?(\w+)\);[ \t]*\r?\n'
                             r'[ \t]*CCharString::operator=\(\(CCharString \*\)\2,\s*([^;]+?)(?:,\s*-1)?\);', re.M)
            text = pat.sub(lambda m: f'{m.group(1)}STRINGMAP_Set({m.group(3)}, {m.group(4)}, {m.group(5).strip()});', text)
        elif target in RESOURCE_ACQUIRED:
            text = re.sub(re.escape(label) + r'\s*\(([^;,]+?)\)', lambda m: f'RESOURCE_IsAcquired({_strip_addr(m.group(1))})', text)
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
    return text


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


def canonicalise_stack_objects(text: str) -> str:
    """Ghidra names each stack slot separately (`appuStack_ac` … `uStack_a4`), so members of one
    stack object (a 16-byte resource/movie/map) appear under several names. Objects created by the
    pseudo API give their base slot and size; every slot name inside that extent becomes the base."""
    bases = []
    for m in re.finditer(r'\b([A-Za-z]+Stack_|local_)([0-9a-f]+)(?:_p([48c]))? = (RESOURCE_NewResource|RESOURCE_StartMovie|ACTORMAP_New|STRINGMAP_New|QUESTTHING_Empty)\(', text):
        shift = int(m.group(3), 16) if m.group(3) else 0
        bases.append((m.start(), m.group(1), int(m.group(2), 16) - shift, STACK_OBJECT_SIZES[m.group(4)], m.group(0)[:m.group(0).index(' =')]))
    if not bases:
        return text
    # the same stack bytes may host a different object later in the function: each object's names
    # are rewritten only from its constructor up to the next constructor whose extent overlaps
    pieces = []
    for i, (start, prefix, boff, size, name) in enumerate(bases):
        def overlaps(b):
            return b[2] - b[3] < boff and boff - size < b[2] and b[2] != boff   # extents (off-size, off]
        # the object's text runs from the previous overlapping construction (its members may be read
        # before the constructor line, e.g. an iterator element copy) to the next one
        region_start = next((b[0] for b in reversed(bases[:i]) if overlaps(b)), 0)
        region_end = next((b[0] for b in bases[i + 1:] if overlaps(b)), len(text))
        pieces.append((region_start, region_end, prefix, boff, size, name))

    def rewrite(segment, boff, size, name):
        segment = RE_STACK_NAME.sub(lambda m: name if (boff - size < int(m.group(2), 16) < boff or int(m.group(2), 16) == boff) and m.group(0) != name else m.group(0), segment)
        segment = re.sub(r'\(' + re.escape(name) + r' \+ (?:4|8|0xc|12)\)', name, segment)
        segment = re.sub(r'&' + re.escape(name) + r'\b', name, segment)
        return segment
    # rewrite spans back to front so earlier offsets stay valid (spans of one extent do not overlap)
    for start, end, prefix, boff, size, name in sorted(pieces, key=lambda x: (x[0], x[1]), reverse=True):
        text = text[:start] + rewrite(text[start:end], boff, size, name) + text[end:]
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


def rename_scalar_stack_locals(text):
    """Ghidra stack names (`local_14`, `uStack_8`) are refused by the lifter's assignment rule (they
    are usually object slots). Ones that only ever appear as plain scalars get lifter-visible names."""
    # hidden-return slots of GSI/helper calls (`GSI->GetThingWithScriptName((CScriptThing *)auStack_1c, ...)`) are
    # the lifter's slot aliases (`r1`): keep their spelling
    # (at this stage GSI calls are still raw vcalls with the receiver as first argument, so the slot may be any argument)
    STK = r'((?:[A-Za-z]+Stack_|local_)[0-9a-f]+)'
    # any stack object passed as a call argument (bare array name, `&name` or `(CScriptThing *)name`) is an
    # address the lifter resolves through its slot table
    hidden = set(re.findall(r'[(,]\s*(?:\(CScriptThing \*\))?&?' + STK + r'\s*[,)]', text))
    # a slot that receives a lowered value by plain assignment is a handle, not a hidden result
    # (unless it is also an explicitly cast hidden-return argument: a thing object later overwritten by a copy)
    cast_args = set(re.findall(r'[(,]\s*\(CScriptThing \*\)' + STK + r'\s*[,)]', text))
    hidden -= set(re.findall(r'^[ \t]*' + STK + r' = (?:p[A-Z]\w*|thing_\w+|r\d+|native_arg_\w+);', text, re.M)) - cast_args   # pointer-typed values only
    # a stack CScriptThing that only ever holds lowered values is a plain handle: drop its casts
    text = re.sub(r'\(CScriptThing \*\)((?:[A-Za-z]+Stack_|local_)[0-9a-f]+)\b', lambda m: m.group(0) if m.group(1) in hidden else m.group(1), text)
    body = re.sub(r'^[ \t]*(?:[\w:<>,]+ )+\**\w+(?: \[\d+\])?;[ \t]*\r?$', '', text, flags=re.M)   # declarations
    for name in sorted(set(re.findall(r'\b(?:[A-Za-z]+Stack_|local_)[0-9a-f]+\b', body))):
        if name in hidden:
            continue
        if re.search(r'[&*]' + re.escape(name) + r'\b|\b' + re.escape(name) + r'\s*[\[.]|\(\w+ \*+\)' + re.escape(name) + r'\b', body):
            continue
        m = re.fullmatch(r'([A-Za-z]*)(?:Stack_|local_)([0-9a-f]+)', name)
        text = re.sub(r'\b' + re.escape(name) + r'\b', f'{m.group(1) or "v"}_stk_{m.group(2)}', text)
    return text


def lower_after_annotate(text, thing_slots=None):
    text = fold_name_compare(text)
    text = fold_inline_strncmp(text)
    text = fold_null_string_branches(text)
    """Rewrites that need the GSI names: quest-side entity acquisition through a resource object."""
    text = re.sub(r'^([ \t]*)(?:(\w+) = )?GSI->StartScriptingEntity\(([^,;]+),([^,;]+),([^,;]+)\);',
                  lambda m: f'{m.group(1)}{(m.group(2) + " = ") if m.group(2) else ""}RESOURCE_TryAcquire({_strip_addr(m.group(4))}, {m.group(3).strip()}, {m.group(5).strip()});', text, flags=re.M)
    # a thing returned straight into an outgoing by-value slot (`F((CScriptThing *)&stack0xNN, ...)` with no
    # result) that the decompiler then "reassembles" from unrelated registers: the temporary is the result
    text = RE_BV_THING_RESULT.sub(lambda m: f'{m.group(1)}{m.group(4)} = {m.group(2)}({m.group(3)});\n', text)
    text = fold_local_thing_vectors(text, thing_slots)
    # `GSI->DeregisterTimer(unaff_REG)`: Ghidra lost the register holding the id across the block; when the
    # function registers exactly one timer that is the id
    timers = re.findall(r'^[ \t]*(\w+) = (?:\(\w+\))?GSI->RegisterTimer\(\);', text, re.M)
    if len(set(timers)) == 1:
        text = re.sub(r'GSI->DeregisterTimer\(unaff_E[A-Z]{2}\)', f'GSI->DeregisterTimer({timers[0]})', text)
        # the register holding the id is reused for the (meaningless) DeregisterTimer result and later values
        # (`iVar4 = GSI->DeregisterTimer(iVar4)`); a stack copy made right after registration is the stable id
        copy = re.search(r'^[ \t]*' + re.escape(timers[0]) + r' = (?:\(\w+\))?GSI->RegisterTimer\(\);[ \t]*\r?\n[ \t]*(\w+) = ' + re.escape(timers[0]) + r';', text, re.M)
        if copy and len(re.findall(r'^[ 	]*' + re.escape(copy.group(1)) + r' = ', text, re.M)) == 1:   # (a slot reused for other values is no stable id)
            text = re.sub(r'^([ \t]*)(?:' + re.escape(timers[0]) + r' = )?GSI->DeregisterTimer\((?:' + re.escape(timers[0]) + '|' + re.escape(copy.group(1)) + r')\);',
                          lambda m: f'{m.group(1)}GSI->DeregisterTimer({copy.group(1)});', text, flags=re.M)
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
    return text


RE_VEC_DTOR_LOOP = re.compile(
    r'^[ \t]*for \(; (?:\w+ = \w+, )?(\w+) != (\w+); \1 = \1 \+ 3\) \{\s*\r?\n(?:[ \t]*\w+ = \w+;[ \t]*\r?\n)?'
    r'[ \t]*(?:\w+ = )?\(\*\*\(code \*\*\)\*\1\)\(0\);[ \t]*\r?\n(?:[ \t]*\w+ = \w+;[ \t]*\r?\n){0,2}[ \t]*\}[ \t]*\r?\n', re.M)
RE_VEC_FREE = re.compile(
    r'^[ \t]*if \((\w+) != \(undefined4 \*\)0x0\) \{\s*\r?\n[ \t]*free\(\1\);[ \t]*\r?\n(?:[ \t]*\w+ = \w+;[ \t]*\r?\n)*[ \t]*\}[ \t]*\r?\n', re.M)


def fold_local_thing_vectors(text, thing_slots=None):
    '''A local std::vector<CScriptThing> filled by a GSI `GetAllThings*` slot: the Lua binding returns a table.
    `n = GSI->GetAllThingsWithDefName(&name,&vec);` -> `vec = GSI->...(&name); n = LOCALLIST_Count(vec);`,
    `(CScriptThing *)((int)vec + byteOffset)` -> `LOCALLIST_At(vec, byteOffset / 0xc)`; the element destructor
    loops (`for (; p != end; p += 3) (**(code **)*p)(0);`), `free(begin)` and the zeroed begin/end/capacity
    slots are the vector's own lifetime and vanish.'''
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
        return f'{m.group(1)}{m.group(5)} = GSI->{m.group(3)}({m.group(4)});\n{m.group(1)}{m.group(2)} = LOCALLIST_Count({m.group(5)});'
    text = re.sub(r'^([ \t]*)(\w+) = (?:\(\w+\))?GSI->(\w+)\(([^;]*?),&?(\w+)\);', call, text, flags=re.M)
    # the void spelling (the count is computed from the begin/end slots afterwards)
    def call_void(m):
        if not m.group(2).startswith('GetAllThings') and m.group(4) not in constructed:
            return m.group(0)
        vectors.append(m.group(4))
        return f'{m.group(1)}{m.group(4)} = GSI->{m.group(2)}({m.group(3)});'
    text = re.sub(r'^([ \t]*)GSI->(\w+)\(([^;]*?),&?(\w+)\);', call_void, text, flags=re.M)
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
        text = re.sub(r'\(CScriptThing(?:_bv)? \*\)' + v + r'\b', f'LOCALLIST_At({vec}, 0)', text)
        # the begin pointer itself (Ghidra typed it `CScriptThing *`) as a call operand is element 0, `V + k` element k
        def element(m):
            before = text[max(0, m.end(1) - 16):m.end(1)]     # up to and including the `(` / `,` before the name
            if re.search(r'(?:LOCALLIST_(?:Count|At)|free)\($', before):
                return m.group(0)
            return f'{m.group(1)}LOCALLIST_At({vec}, {m.group(2) or 0})'
        text = re.sub(r'([(,]\s*)' + v + r'(?: \+ (\d+))?(?=\s*[,)])', element, text)
        # the end-pointer slot 4 bytes above the begin slot under its own Ghidra name (`pu_stk_20` for `xStack_24`)
        slot = re.search(r'_(?:stk_)?([0-9a-f]+)$', vec)
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
        text = re.sub(r'\(\*\*\(code \*\*\)\(\*(?:\(int \*\))?LOCALLIST_At\(' + v + r', (\w+)\) \+ (0x[0-9a-f]+|\d+)\)\)\s*\(', at_vcall_local, text)
        # element count from the begin/end slots (both canonicalised to the vector's name):
        # `iVar = (int)V - V >> 0x1f;` (sign fix) then `((int)V - V) / 0xc + iVar != iVar` (count != 0)
        text = re.sub(r'^[ \t]*(\w+) = \(int\)' + v + r' - ' + v + r' >> 0x1f;[ \t]*\r?\n', '', text, flags=re.M)
        text = re.sub(r'\(\(int\)' + v + r' - ' + v + r'\) / 0xc \+ (\w+) (!=|==) \1\b', lambda m, vec=vec: f'LOCALLIST_Count({vec}) {m.group(2)} 0', text)
        text = re.sub(r'\(int\)' + v + r' - ' + v + r'\b', f'LOCALLIST_Count({vec}) * 0xc', text)
        text = re.sub(r'\(LOCALLIST_Count\(' + v + r'\) \* 0xc\) / 0xc', f'LOCALLIST_Count({vec})', text)
        # a thing vcall through an element's Data pointer (byte index counted from the +4 field:
        # `iVar5 = 4; ... (**(code **)(**(int **)(iVar5 + (int)V) + OFF))(`)
        def data_vcall(m, vec=vec):
            out = thing_call_local(f'LOCALLIST_At({vec}, ({m.group(1)} - 4) / 0xc)', m.group(2), text[m.end():m.end() + 1])
            return out or m.group(0)
        text = re.sub(r'\(\*\*\(code \*\*\)\(\*\*\(int \*\*\)\((\w+) \+ \(int\)' + v + r'\) \+ (0x[0-9a-f]+|\d+)\)\)\s*\(', data_vcall, text)
        # a thing vcall on an element addressed by byte offset: name the element first
        text = re.sub(r'\*\(int \*\)\((\w+) \+ \(int\)' + v + r'\)', lambda m, vec=vec: f'*(int *)({vec} + {m.group(1)})', text)   # `(i + (int)V)` -> `(V + i)`
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
    junk = r'(?:[ \t]*\w+ = (?:\w+|\w+ \+ \d+|\(undefined4 \*\)0x0);[ \t]*\r?\n)*'
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


def lower(source: str, spec: LoweringSpec) -> tuple[str, list[str]]:
    diag = []
    text = source
    # 0. member functions: Ghidra's untyped `param_1` is `this`; typed pointer derefs of the parent
    text = re.sub(r'\*\((?:C\w+Script) \*\*\)\((' + SELF + r') \+ 0x14\)', r'*(int *)(\1 + 0x14)', text)
    text = re.sub(r'\*\((?:C\w+MasterData) \*\*\)\((' + SELF + r') \+ (0x18|0x44)\)', r'*(int *)(\1 + \2)', text)
    text = re.sub(r'\bparam_1\b', 'this', text)
    text = normalise_typed_decompile(text)
    for m in set(re.findall(r'^[ \t]*(\w+) = \*\(int \*\*\)\(this \+ (4|0x40)\);', text, re.M)):
        text = re.sub(r'^([ \t]*)(\w+) = \*' + re.escape(m[0]) + r';', lambda mm, off=m[1]: f'{mm.group(1)}{mm.group(2)} = **(int **)(this + {off});', text, flags=re.M)
    text = fold_by_value_things(text, getattr(spec, 'code_range', None))
    text = isolate_gsi_vtable_temps(text)
    text = fold_tangled_thing_assign(text)
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
    def handle(m):
        if re.search(r'\(CScriptThing(?:_bv)? \*\)&?' + re.escape(m.group(2)) + r'\b', text):
            # the slot is a CScriptThing object (hidden-return target): assigning its Data is the thing copy
            return f'{m.group(1)}CScriptThing::operator=((CScriptThing *)&{m.group(2)},(int){m.group(3)});'
        handles.add(m.group(2))
        return f'{m.group(1)}{m.group(2)} = {m.group(3)};'
    text = re.sub(r'^([ \t]*)CCountedPointer<\w+>::operator=\s*\(\(CCountedPointer<\w+> \*\)&(\w+),\s*\(int\)&\*\(int \*\)\((\w+) \+ (?:4|0x4)\)\);', handle, text, flags=re.M)
    for h in handles:
        text = re.sub(r'\(\*\*\(code \*\*\)\(\*' + re.escape(h) + r' \+ (0x[0-9a-f]+|\d+)\)\)\(', r'(**(code **)(*(int *)' + h + r' + \1))(', text)
        text = re.sub(r'\b' + re.escape(h) + r' == \(int \*\)0x0\b', f'!__thing_valid({h})', text)
        text = re.sub(r'\b' + re.escape(h) + r' != \(int \*\)0x0\b', f'__thing_valid({h})', text)
    text = name_offset_objects(text, getattr(spec, 'call_labels', {}))
    text = fold_byte_split_pointers(text)
    text = fold_byte_literal_words(text)
    text = fold_actor_maps(text, getattr(spec, 'resolve_string', None))
    text = fold_resource_objects(text, getattr(spec, 'call_labels', {}))
    text = drop_trivial_base_calls(text, getattr(spec, 'call_labels', {}), getattr(spec, 'byte_at', None))
    text = fold_inline_constructors(text)
    text = fold_inline_destructors(text)
    text = canonicalise_stack_objects(text)
    text = fold_inline_destructors(text)    # again: the canonical names may only now agree across the three lines
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
    for var in set(re.findall(r'^[ \t]*(\w+) = (?:\(\w+ \*+\))?DAT_0143e90c;', text, re.M)):
        text = re.sub(r'\*\((float|int|undefined4|uint) \*\)\(' + re.escape(var) + r' \+ (0x[0-9a-f]{3,}|\d{3,})\)',
                      r'*(\1 *)(DAT_0143e90c + \2)', text)
    text = re.sub(r'\*\(float \*\)\(DAT_0143e90c \+ (0x[0-9a-f]+|\d+)\)', r'ENGINE_GlobalGameDataFloat(\1)', text)
    text = re.sub(r'\*\((?:int|undefined4|uint) \*\)\(DAT_0143e90c \+ (0x[0-9a-f]+|\d+)\)', r'ENGINE_GlobalGameData(\1)', text)
    # the engine's static zero vector (DAT_0143e8e0, zero-initialised .data) as the position of an invalid thing
    text = re.sub(r'\((?:float|C3DVector) \*\)&DAT_0143e8e0\b', 'ENGINE_ZeroVector()', text)
    # a position read through a float pointer (`pf = (float *)GetPos(X); f = *pf; g = pf[1]; h = pf[2]`)
    vecs = set(re.findall(r'^[ \t]*(\w+) = (?:\(float \*\))?(?:CScriptThing::GetPos\(|ENGINE_ZeroVector\(\))', text, re.M))
    for v in vecs:
        text = re.sub(r'^([ \t]*)' + re.escape(v) + r' = \(float \*\)(?=CScriptThing::GetPos\()', r'\1' + v + ' = ', text, flags=re.M)
        text = re.sub(r'^[ \t]*[\w:]+ \*' + re.escape(v) + r';[ \t]*\r?\n', '', text, flags=re.M)
        text = re.sub(r'\*' + re.escape(v) + r'\b', v + '.x', text)
        text = re.sub(r'\b' + re.escape(v) + r'\[1\]', v + '.y', text)
        text = re.sub(r'\b' + re.escape(v) + r'\[2\]', v + '.z', text)
    # CRT truncation of an x87 value (`__ftol2((float10)x)`, typed with its ST0 operand by the export)
    text = re.sub(r'\b__ftol2\(\s*(?:\(float10\))?', 'ENGINE_Trunc(', text)

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
    for base in master_bases:
        def master_store(m):
            off = int(m.group(2), 16)
            f = spec.master_fields.get(off)
            if not f:
                return m.group(0)
            return f'{m.group(1)}GSI->SetMasterGameState("{f[0]}", {_lit(m.group(3), f[1])});'
        text = re.sub(r'^([ \t]*)\*\(' + TYPE + r' \*\)\(' + base + r' \+ (0x[0-9a-f]+)\) =\s*([^;]+);', master_store, text, flags=re.M)
        def master_load(m):
            off = int(m.group(1), 16)
            f = spec.master_fields.get(off)
            return f'GSI->GetMasterGameState("{f[0]}")' if f else m.group(0)
        text = re.sub(r'\*\(' + TYPE + r' \*\)\(' + base + r' \+ (0x[0-9a-f]+)\)', master_load, text)

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
    text = re.sub(r'^[ \t]*(?:[A-Za-z]+Stack_|local_)[0-9a-f]+ = (?:\(\w+\))?this;[ \t]*\r?\n', '', text, flags=re.M)
    text = re.sub(r'^[ \t]*(\w+) = \((?:\w+ \*+)\)\w+;[ \t]*\r?\n(?=[ \t]*\1 = )', '', text, flags=re.M)
    text = drop_dead_local_stores(text)
    text = rename_scalar_stack_locals(text)
    return text, diag


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
        text = re.sub(r'^[ \t]*' + re.escape(alias) + r' = \w+;[ \t]*\r?\n', '', text, flags=re.M) if 'Stack_' in alias else text
    text = re.sub(r'(CScriptThing::\w+\(me, ?)' + ME_RECEIVER + r'(?:,\s*|(?=\)))', r'\1', text)
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
    (re.compile(r'ENGINE_ZeroVector\(\)'), '{x = 0, y = 0, z = 0}'),
    (re.compile(r'ENGINE_GlobalGameDataFloatAt\('), 'quest:ReadGlobalGameDataFloatAt('),
    (re.compile(r'ENGINE_GlobalGameDataFloat\('), 'quest:ReadGlobalGameDataFloat('),
    (re.compile(r'ENGINE_GlobalGameData\('), 'quest:ReadGlobalGameData('),
    (re.compile(r'ACTORMAP_Set\('), 'resources:SetActor('),
    (re.compile(r'ACTORMAP_Destroy\('), 'resources:DestroyActorMap('),
    (re.compile(r'RESOURCE_IsAcquired\(\w+\)'), 'false'),   # a freshly constructed stack resource has no handle yet ([this+8] == 0)
    (re.compile(r'LOCALLIST_Count\((\w+)(?:\[0 \+ 1\])?\)'), r'#\1'),   # `vec[0 + 1]` is the vector's begin field, not an element
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
    text = _expand_calls(text, 'LOCALLIST_Erase', lambda a: f'table.remove({a[0]}, {a[1]} + 1)')
    text = _expand_calls(text, 'ENGINE_Trunc', lambda a: f'math.tointeger(math.modf({a[0]}))')   # integral part (truncated toward zero), one value in every operand position
    text = _expand_calls(text, 'ENGINE_StrCmp', lambda a: f'(({a[0]} == {a[1]}) and 0 or 1)' if len(a) == 2 else 'ENGINE_StrCmp(' + ', '.join(a) + ')')
    text = re.sub(r'(QUEST|ENTITY)LIST_At_(\w+)\(', lambda m: ('quest:GetStateListAt(' if m.group(1) == 'QUEST' else '__native_entity_state:GetStateListAt(') + '"' + m.group(2) + '", ', text)
    for pattern, repl in LUA_PSEUDO:
        text = pattern.sub(repl, text)
    text = KEY.sub('(', text)
    text = re.sub(r'&("[^"]*")', r'\1', text)
    return text
