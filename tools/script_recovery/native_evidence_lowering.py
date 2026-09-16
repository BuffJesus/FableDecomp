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
    r'^[ \t]*if \(\((\w+) != \(int \*\)0x0\) && \(\*\1 = \*\1 \+ -1, \*\1 == 0\)\) \{\s*\r?\n'
    r'[ \t]*\(\*\(code \*\)\1\[1\]\)\(\);\s*\r?\n[ \t]*operator_delete\(\1\);\s*\r?\n[ \t]*\}[ \t]*\r?\n', re.M)


RE_LOCAL_COUNTED_RELEASE2 = re.compile(
    r'^[ \t]*if \((\w+) != \(int \*\)0x0\) \{\s*\r?\n[ \t]*\*\1 = \*\1 \+ -1;\s*\r?\n[ \t]*if \(\*\1 == 0\) \{\s*\r?\n'
    r'[ \t]*\(\*\(code \*\)\1\[1\]\)\(\);\s*\r?\n[ \t]*operator_delete\(\1\);\s*\r?\n[ \t]*\}\s*\r?\n[ \t]*\}[ \t]*\r?\n', re.M)
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
    label inside the delete block), the parts are stored, `DONE:`. Everything up to DONE is the
    assignment."""
    while (m := RE_THING_PARTS.search(text)):
        label = m.group('label')
        done = re.search(r'^[ \t]*' + label + r':[ \t]*\r?\n', text[m.end():], re.M)
        if not done:
            break
        between = text[m.end():m.end() + done.start()]
        names = {m.group('i'), m.group('d'), m.group('dst')}
        ok = True
        for line in between.split('\n'):
            st = line.strip()
            if not st or st in ('}', '{') or re.fullmatch(r'LAB_[0-9a-f]+:', st):
                continue
            if not any(re.search(r'\b' + re.escape(n) + r'\b', st) for n in names):
                ok = False
                break
        if not ok:
            break
        inner_labels = re.findall(r'^[ \t]*(LAB_[0-9a-f]+):', between, re.M)
        text = text[:m.start()] + f'{m.group("ind")}{m.group("dst")} = {m.group("src")};\n' + text[m.end() + done.end():]
        for lab in inner_labels + [label]:
            if len(re.findall(r'\b' + lab + r'\b', text)) == 0:
                continue
            # jumps that targeted a label inside the assignment now land after it
            text = re.sub(r'\bgoto ' + lab + r';', f'goto {label};', text)
        if re.search(r'\bgoto ' + label + r';', text):
            text = text.replace(f'{m.group("ind")}{m.group("dst")} = {m.group("src")};\n',
                                f'{m.group("ind")}{m.group("dst")} = {m.group("src")};\n{label}:\n', 1)
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
    return RE_SLOT_ZERO.sub('', text)


RE_BV_THING_CTOR = re.compile(
    r'^[ \t]*CScriptThing::CScriptThing\s*\(\s*\(CScriptThing \*\)&stack0x([0-9a-f]+),\s*(?:\(CScriptThing \*\))?(\w+)(?:,[^;]*)?\);[ \t]*\r?\n', re.M)


def fold_by_value_things(text: str, code_range=None) -> str:
    """By-value CScriptThing arguments (typed export): the copy constructor fills an outgoing slot
    (`&stack0xNN`) and the decompiler reassembles it into a `pThing._0_4_/_4_4_/_8_4_` temporary that
    is passed to the call. The argument is simply the copied source."""
    if code_range:
        lo, hi = code_range
        text = re.sub(r'^[ \t]*\w+ = (?:\([\w ]+\*+\))?0x([0-9a-f]{6,7});[ \t]*\r?\n',
                      lambda m: '' if lo <= int(m.group(1), 16) < hi else m.group(0), text, flags=re.M)
    while (m := RE_BV_THING_CTOR.search(text)):
        slot, src = m.group(1), m.group(2)
        text = text[:m.start()] + text[m.end():]
        use = re.search(r'^[ \t]*(\w+)\._0_4_ = in_stack_' + slot + r';[ \t]*\r?\n', text, re.M)
        if not use:
            continue
        var = use.group(1)
        text = re.sub(r'^[ \t]*' + re.escape(var) + r'\._\d+_4_ = [^;]+;[ \t]*\r?\n', '', text, flags=re.M)
        text = re.sub(r'\b' + re.escape(var) + r'\b', src, text)
    return text


def normalise_typed_decompile(text: str) -> str:
    """Typed exports (ExportTypedTranslationUnit) print a few shapes the untyped pipeline never saw."""
    text = re.sub(r'return CONCAT31\([^;]*?,\s*(0|1)\);', lambda m: f'return {"true" if m.group(1) == "1" else "false"};', text)
    text = re.sub(r"return CONCAT31\(\w+,\s*'\\x01' - (\w+)\);", r'return !\1;', text)
    text = re.sub(r'return \(uint\)extraout_var(?:_\d+)? << 8;', 'return false;', text)
    text = re.sub(r'return \(uint\)(\w+) << 8;', r'return false;', text)
    text = re.sub(r'\(int\)(this(?:_\d+)?)\b', r'\1', text)                 # (int)this + 0x40
    text = re.sub(r'\)[ \t]*\r?\n[ \t]*;', ');', text)                        # `...)` newline `;`
    text = re.sub(r'\)\)[ \t]*\r?\n[ \t]+\(', '))(', text)                   # call head wrapped before its argument list
    text = join_wrapped_statements(text)
    text = re.sub(r'&("(?:[^"\\]|\\.)*")', r'\1', text)                      # &"literal" (propagated CCharString temp)
    text = re.sub(r'\*\((\w+ \*+)\)&(\w+)->field_0x([0-9a-f]+)', r'*(\1)(\2 + 0x\3)', text)
    text = re.sub(r'\*&(\w+)->field_0x([0-9a-f]+)', r'*(int *)(\1 + 0x\2)', text)
    text = re.sub(r'(\w+)->field_0x([0-9a-f]+)', r'*(int *)(\1 + 0x\2)', text)
    text = re.sub(r'\b(CScriptThing|CCharString|C3DVector|CRGBColour|CWideString|CRGBFloatColour)_bv\b', r'\1', text)
    text = fold_outgoing_stack_slots(text)
    text = re.sub(r'\*\) \(', '*)(', text)
    text = re.sub(r'(\b(?:this|\w+) \+ )(\d{2,})\b', lambda m: m.group(1) + hex(int(m.group(2))), text)
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
    r'^(?P<ind>[ \t]*)CCharString::CCharString\(\(CCharString \*\)&(?P<key>\w+),(?P<keyval>"[^"]*"|&DAT_[0-9a-f]+|\w+),-1\);\s*'
    r'(?P<node>\w+) = std::\s*map<CCharString,CCountedPointer<[^;]*?::operator\[\]\((?:\(map<[^;]*?\*\))?&?(?P<map>\w+),(?:\(CCharString \*\))?&(?P=key)\);\s*'
    r'(?:[\w:]+::\w+\(\(?[\w ]*\*?\)?(?P=node)\);\s*)?'
    r'(?P<body>(?:[^;{}]*;\s*){0,4})'
    r'if \(\w+ != \w+\) \{\s*if \(\w+ != \(int \*\)0x0\) \{\s*\*\w+ = \*\w+ \+ -1;\s*if \(\*\*\(int \*\*\)\((?P=node) \+ 0xc\) == 0\) \{\s*'
    r'\(\*\(code \*\)\(\*\(int \*\*\)\((?P=node) \+ 0xc\)\)\[1\]\)\(\);\s*operator_delete\(\*\(void \*\*\)\((?P=node) \+ 0xc\)\);\s*\}\s*\}\s*'
    r'\*\((?:CScriptThing|int|undefined4) \*\*?\)\((?P=node) \+ 8\) = (?P<data>\w+);\s*'
    r'\*\((?:int|undefined) \*\*\)\((?P=node) \+ 0xc\) = (?P<info>\w+);\s*'
    r'if \((?P=info) != \((?:int|undefined) \*\*?\)0x0\) \{\s*\*(?P=info) = \*(?P=info) \+ 1;\s*\}\s*\}\s*'
    r'(?:std::\s*_Cons_val<[^;(]*?\s*\(&(?P=key)\);[ \t]*\r?\n)?', re.M)
RE_MAP_RUN = re.compile(
    r'^(?P<ind>[ \t]*)CCharString::CCharString\(\(CCharString \*\)&(?P<key>\w+),(?P<keyval>"[^"]*"|&DAT_[0-9a-f]+|\w+),-1\);\s*'
    r'RunCutsceneMacro_Func\(&(?P=key),&(?P<map>\w+),\(void \*\)0x0,\(void \*\)0x0,(?P<setup>true|false),(?P<skip>true|false)\);\s*'
    r'(?:std::\s*_Cons_val<[^;(]*?\s*\(&(?P=key)\);[ \t]*\r?\n)?', re.M)
RE_MAP_DESTROY = re.compile(r'^([ \t]*)StdMap_Destroy_API\(&?(\w+)(?:\.field_0x4)?\);', re.M)
RE_MAP_RUN_VAR = re.compile(
    r'^(?P<ind>[ \t]*)RunCutsceneMacro_Func\((?P<key>(?:\(CCharString \*\))?&?\(?[\w. +]+\)?),&?(?P<map>[\w.]+),\(void \*\)0x0,\(void \*\)0x0 ?,(?P<setup>true|false),(?P<skip>true|false)\);', re.M)
RE_MAP_NEW2 = re.compile(r'^([ \t]*)StdMap_Construct_API\(&?(\w+)\);', re.M)
# resource-valued maps (std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>): the value is a
# controlled-entity resource handle assigned with the resource operator=.
RE_MAP_SET2 = re.compile(
    r'^(?P<ind>[ \t]*)CCharString::CCharString\(\(CCharString \*\)&(?P<key>\w+),(?P<keyval>"[^"]*"|&DAT_[0-9a-f]+|\w+),-1\);\s*'
    r'(?:(?P<alias>\w+) = (?P<thing>&?[\w.]+);\s*)?(?:\w+ = 0x[0-9a-f]{6,7};\s*)?'
    r'(?P<node>\w+) = std::\s*map<CCharString,CCountedPointer<[^;]*?::operator\[\]\((?:\(map<[^;]*?\*\))?&?(?P<map>\w+),(?:\(CCharString \*\))?&(?P=key)\);\s*'
    r'CScriptGameResourceObjectScriptedThingBase::operator=\((?P=node),(?P<src>&?\w+)\);\s*'
    r'(?:std::\s*_Cons_val<[^;(]*?\s*\(&(?P=key)\);[ \t]*\r?\n)?', re.M)


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
    text = RE_MAP_RUN.sub(lambda m: f'{m.group("ind")}RESOURCE_RunMacro({keyval(m.group("keyval"))}, {m.group("map")}, {m.group("setup")}, {m.group("skip")});\n', text)
    text = RE_MAP_DESTROY.sub(r'\1ACTORMAP_Destroy(\2);', text)
    text = RE_MAP_RUN_VAR.sub(lambda m: f'{m.group("ind")}RESOURCE_RunMacro({_strip_addr(m.group("key"))}, {m.group("map").split(".field")[0]}, {m.group("setup")}, {m.group("skip")});', text)
    return text


# ---- retail resource objects (controlled entities, movies) ----------------------------------------
# Addresses proven by FSE's own resource implementation (FableAPI.cpp / LuaRetailResources.h).
RESOURCE_CTOR = {0x7E72A0}            # CScriptGameResourceObjectScriptedThingBase::ctor (bsim: CCarriedReadableDef)
THING_DTOR = {0x4AA840}  # CScriptThing::~CScriptThing (vtable 01238c8c, releases Info)
RESOURCE_DTOR = {0x7E74D0}            # CSGROSTB_Destroy_API
MOVIE_CTOR = {0x6E7B40, 0x6E7B60}     # CScriptGameResourceObjectMovieBase ctors
MOVIE_DTOR = {0x6E7B80}               # MovieResource_Destroy_API


def _strip_addr(arg):
    arg = arg.strip()
    arg = re.sub(r'^\([\w :*]+\*\)', '', arg).strip()
    return arg[1:] if arg.startswith('&') else arg


# vtable pointers stored by inlined constructors (FSE: g_pCScriptGameResourceObjectScriptedThingBaseVTable,
# g_pMovieObjectVTable, g_pCScriptThingVTable)
RE_INLINE_CTOR = re.compile(
    r'^(?P<ind>[ \t]*)(?:\*\(undefined \*\*\*\))?(?P<obj>&?\w+)(?:\[0\]|\._0_4_)? = &PTR_[A-Za-z_]*_(?P<vt>0127094c|01260ef4|01238c8c);[ \t]*\r?\n'
    r'(?:[ \t]*(?:\w+ = 0;|\w+ = \(\w+ \*\)0x0;|\w+\[\d\] = (?:\(\w+ \*\))?0x0;|\*\(\w+ \*\)\(\w+ \+ (?:4|8|0x8)\) = 0;)[ \t]*\r?\n){0,3}', re.M)


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
    b = bytes(byte_at(target + i) for i in range(9))
    return b[:4] == b'\x8b\xc1\xc7\x00' and b[8] == 0xC3 or b[:2] == b'\xc7\x01' and b[6] == 0xC3


def drop_trivial_base_calls(text, call_labels, byte_at):
    for label, target in call_labels.items():
        if _vtable_only_body(byte_at, target):
            text = re.sub(r'^[ \t]*' + re.escape(label) + r'\s*\([^;]*\);[ \t]*\r?\n', '', text, flags=re.M)
    return text


def fold_resource_objects(text, call_labels):
    """call_labels: {label text as printed in the decompile: target address}. Rewrites constructor /
    destructor calls of resource and movie objects into the retail-resource pseudo API."""
    for label, target in call_labels.items():
        if target in RESOURCE_CTOR:
            text = re.sub(r'^([ \t]*)' + re.escape(label) + r'\s*\(([^;]+?)\);', lambda m: f'{m.group(1)}{_strip_addr(m.group(2))} = RESOURCE_NewResource();', text, flags=re.M)
        elif target in THING_DTOR:
            text = re.sub(r'^[ \t]*' + re.escape(label) + r'\s*\(\(CScriptThing \*\)(\w+)[^;]*\);[ \t]*\r?\n', '', text, flags=re.M)
        elif target in RESOURCE_DTOR:
            text = re.sub(r'^([ \t]*)' + re.escape(label) + r'\s*\(([^;]+?)\);', lambda m: f'{m.group(1)}RESOURCE_ReleaseResource({_strip_addr(m.group(2))});', text, flags=re.M)
        elif target in MOVIE_CTOR:
            text = re.sub(r'^([ \t]*)' + re.escape(label) + r'\s*\(([^;,]+?)(?:,[^;]*)?\);', lambda m: f'{m.group(1)}{_strip_addr(m.group(2))} = RESOURCE_StartMovie("");', text, flags=re.M)
        elif target in MOVIE_DTOR:
            text = re.sub(r'^([ \t]*)' + re.escape(label) + r'\s*\(([^;,]+?)(?:,[^;]*)?\);', lambda m: f'{m.group(1)}RESOURCE_DestroyMovie({_strip_addr(m.group(2))});', text, flags=re.M)
    return text


SQUARED_DISTANCE = {0xCBE512}   # float __fastcall GetSquaredDistanceBetweenThings(a, b)
STRING_CONCAT = {0x99F570, 0x99F600}   # CCharString operator+ (dest, a, b) -> dest; disassembly 2026-09-16
DISTANCE_PREDICATES = {0xCBE2FF: 'IsDistanceBetweenThingsUnder', 0xCBE3EA: 'IsDistanceBetweenThingsOver'}   # bool __fastcall (a, b, float)


def fold_engine_helpers(text, call_labels):
    """Engine helpers with a direct FSE Lua equivalent. Squared distance becomes the FSE distance
    squared (the pseudo call keeps the comparison semantics; finish_lua expands it)."""
    for label, target in call_labels.items():
        if target in SQUARED_DISTANCE:
            text = re.sub(re.escape(label) + r'\s*\(([^,;]+),([^;)]+)\)',
                          lambda m: f'ENGINE_SquaredDistance({_strip_addr(m.group(1))}, {m.group(2).strip()})', text)
        elif target in STRING_CONCAT:
            text = re.sub(r'(?:\(\w+ \*\))?' + re.escape(label) + r'\s*\((?:\([\w ]+\*\))?&?[\w.]+,\s*([^,;]+),\s*([^;)]+)\)',
                          lambda m: f'ENGINE_Concat({m.group(1).strip().lstrip("&")}, {m.group(2).strip().lstrip("&")})', text)
        elif target in DISTANCE_PREDICATES:
            name = DISTANCE_PREDICATES[target]
            text = re.sub(re.escape(label) + r'\s*\(([^,;]+),\s*([^,;]+),\s*([^;)]+)\)',
                          lambda m, name=name: f'ENGINE_{name}({_strip_addr(m.group(1))}, {_strip_addr(m.group(2))}, {m.group(3).strip()})', text)
    return text


RE_INLINE_STRNCMP = re.compile(
    r'^(?P<ind>[ \t]*)(?P<n>\w+) = (?:0x[0-9a-f]+|\d+);\s*\r?\n[ \t]*(?P<b>\w+) = true;\s*\r?\n'
    r'[ \t]*(?P<p1>\w+) = (?P<l1>"[^"]*");\s*\r?\n[ \t]*(?P<p2>\w+) = (?P<l2>"[^"]*");\s*\r?\n'
    r'[ \t]*do \{\s*\r?\n[ \t]*if \((?P=n) == 0\) break;\s*\r?\n[ \t]*(?P=n) = (?P=n) \+ -1;\s*\r?\n'
    r'[ \t]*(?P=b) = \*(?P=p1) == \*(?P=p2);\s*\r?\n[ \t]*(?P=p1) = (?P=p1) \+ 1;\s*\r?\n[ \t]*(?P=p2) = (?P=p2) \+ 1;\s*\r?\n'
    r'[ \t]*\} while \((?P=b)\);[ \t]*\r?\n', re.M)


def fold_inline_strncmp(text: str) -> str:
    """The compiler inlines `memcmp` of two string literals (a CCharString compare whose buffer is
    null falls back to comparing "" against the literal): the result is a constant."""
    return RE_INLINE_STRNCMP.sub(lambda m: f'{m.group("ind")}{m.group("b")} = {str(m.group("l1") == m.group("l2")).lower()};\n', text)


def fold_name_compare(text):
    """`p = me->GetName(); if (*p == 0) ...; CBasicString<char>::Compare(**p, "S")` -> Lua string ops."""
    text = re.sub(r'\(undefined4 \*\)\*(\w+) == \(undefined4 \*\)0x0', r'\1 == (CCharString *)0x0', text)
    text = re.sub(r'CBasicString<char>::Compare\(\*\(void \*\*\)\*(\w+),("[^"]*")\)', r'ENGINE_StrCmp(\1, \2)', text)
    return text


STACK_OBJECT_SIZES = {'RESOURCE_NewResource': 16, 'RESOURCE_StartMovie': 16, 'ACTORMAP_New': 16, 'QUESTTHING_Empty': 12}
RE_STACK_NAME = re.compile(r'\b([A-Za-z]+Stack_|local_)([0-9a-f]+)\b')


def canonicalise_stack_objects(text: str) -> str:
    """Ghidra names each stack slot separately (`appuStack_ac` … `uStack_a4`), so members of one
    stack object (a 16-byte resource/movie/map) appear under several names. Objects created by the
    pseudo API give their base slot and size; every slot name inside that extent becomes the base."""
    bases = []
    for m in re.finditer(r'\b([A-Za-z]+Stack_|local_)([0-9a-f]+) = (RESOURCE_NewResource|RESOURCE_StartMovie|ACTORMAP_New|QUESTTHING_Empty)\(', text):
        bases.append((m.group(1), int(m.group(2), 16), STACK_OBJECT_SIZES[m.group(3)], m.group(1) + m.group(2)))
    if not bases:
        return text

    def repl(m):
        prefix, off = m.group(1), int(m.group(2), 16)
        for bprefix, boff, size, name in bases:
            if boff - size < off < boff:
                return name
        return m.group(0)
    text = RE_STACK_NAME.sub(repl, text)
    # `&base + N` / `(base + N)` spellings of the same object collapse to the base
    for _, _, _, name in bases:
        text = re.sub(r'\(' + re.escape(name) + r' \+ (?:4|8|0xc|12)\)', name, text)
        text = re.sub(r'&' + re.escape(name) + r'\b', name, text)
    return text


RE_OFFSET_STRING_CTOR = re.compile(
    r'^[ \t]*CCharString::CCharString\(\(CCharString \*\)\((?P<expr>\w+ \+ (?:\d+|0x[0-9a-f]+))\),(?P<lit>"[^"]*"),-1\);[ \t]*\r?\n', re.M)


def fold_offset_string_temporaries(text: str) -> str:
    """A CCharString literal constructed at an offset inside a stack struct (`(auStack_64 + 8)`) is a
    temporary the lifter cannot track by name; substitute the literal for its uses and drop the
    constructor/destructor pair."""
    for m in list(RE_OFFSET_STRING_CTOR.finditer(text)):
        expr, lit = m.group('expr'), m.group('lit')
        if text.count(m.group(0)) != 1:
            continue
        head, tail = text[:m.start()], text[m.end():]
        tail = re.sub(r'^[ \t]*std::\s*_Cons_val<[^;(]*?\s*\(\(CCharString \*\)\(' + re.escape(expr) + r'\)\);[ \t]*\r?\n', '', tail, count=1, flags=re.M)
        tail = re.sub(r'\(CCharString \*\)\(' + re.escape(expr) + r'\)', lit, tail)
        tail = re.sub(r'(?<![\w])\(' + re.escape(expr) + r'\)', lit, tail)
        text = head + tail
    return text


def rename_scalar_stack_locals(text):
    """Ghidra stack names (`local_14`, `uStack_8`) are refused by the lifter's assignment rule (they
    are usually object slots). Ones that only ever appear as plain scalars get lifter-visible names."""
    for name in sorted(set(re.findall(r'\b(?:[A-Za-z]+Stack_|local_)[0-9a-f]+\b', text))):
        if re.search(r'[&*]' + re.escape(name) + r'\b|\b' + re.escape(name) + r'\s*[\[.]|\(\w+ \*+\)' + re.escape(name) + r'\b', text):
            continue
        m = re.fullmatch(r'([A-Za-z]*)(?:Stack_|local_)([0-9a-f]+)', name)
        text = re.sub(r'\b' + re.escape(name) + r'\b', f'{m.group(1) or "v"}_stk_{m.group(2)}', text)
    return text


def lower_after_annotate(text):
    text = fold_name_compare(text)
    text = fold_inline_strncmp(text)
    """Rewrites that need the GSI names: quest-side entity acquisition through a resource object."""
    text = re.sub(r'^([ \t]*)(?:(\w+) = )?GSI->StartScriptingEntity\(([^,;]+),([^,;]+),([^,;]+)\);',
                  lambda m: f'{m.group(1)}{(m.group(2) + " = ") if m.group(2) else ""}RESOURCE_TryAcquire({_strip_addr(m.group(4))}, {m.group(3).strip()}, {m.group(5).strip()});', text, flags=re.M)
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
    text = fold_tangled_thing_assign(text)
    if spec.entity:
        # the entity's own CScriptThing lives at this+8: its Data pointer (this+0xc) passed as an argument is `me`
        text = re.sub(r'\*\(int \*\)\(this \+ (?:0xc|12)\)', '(CScriptThing *)(this + 8)', text)
    # vcall through a thing's Data pointer: (**(code **)(**(int **)(E + OFF+4) + SLOT))( == vcall on the thing at E + OFF
    text = re.sub(r'\(\*\*\(code \*\*\)\(\*\*\(int \*\*\)\(([^;()]*?(?:\([^;()]*\)[^;()]*?)*) \+ (0x[0-9a-f]+|\d+)\) \+ (0x[0-9a-f]+|\d+)\)\)\s*\((?:\*\(int \*\*\)\(\1 \+ \2\)(?:,\s*)?)?',
                  lambda m: m.group(0) if (m.group(1).endswith('this') and int(m.group(2), 0) in (4, 0x40)) or (m.group(1).endswith('0x14)') and int(m.group(2), 0) == 0x40)
                  else f'(**(code **)(*(int *)({m.group(1)} + {hex(int(m.group(2), 0) - 4)}) + {m.group(3)}))(', text)
    text = fold_counted_pointer_assign(text)
    text = fold_counted_pointer_release(text)
    text = fold_actor_maps(text, getattr(spec, 'resolve_string', None))
    text = fold_resource_objects(text, getattr(spec, 'call_labels', {}))
    text = drop_trivial_base_calls(text, getattr(spec, 'call_labels', {}), getattr(spec, 'byte_at', None))
    text = fold_inline_constructors(text)
    text = canonicalise_stack_objects(text)
    text = fold_offset_string_temporaries(text)
    text = fold_engine_helpers(text, getattr(spec, 'call_labels', {}))
    resolve = getattr(spec, 'resolve_string', None)
    if resolve:
        # `&DAT_xxxxxxxx` string addresses (the empty string and other pooled literals) -> literals
        def dat_literal(m):
            literal = resolve(int(m.group(1), 16))
            if literal is None and getattr(spec, 'byte_at', None) and spec.byte_at(int(m.group(1), 16)) == 0:
                literal = ''   # the pooled empty string (a lone NUL) is not a "string" to the resolver
            return '"' + literal.replace('\\', '\\\\').replace('"', '\\"') + '"' if literal is not None else m.group(0)
        text = re.sub(r'&DAT_([0-9a-f]{8})\b', dat_literal, text)
    # reads from the global game-data table (runtime pointer at DAT_0143e90c): keep the offset
    text = re.sub(r'\*\((?:int|float|undefined4|uint) \*\)\(DAT_0143e90c \+ (0x[0-9a-f]+|\d+)\)', r'ENGINE_GlobalGameData(\1)', text)

    parent = r'\*\(int \*\)\(this \+ 0x14\)'
    # 1. alias locals for parent / master pointers, substituted in place (assignment removed)
    def inline_alias(text, base_pattern, kinds):
        alias_re = re.compile(r'^[ \t]*(\w+) = (?:\([\w ]+\*\))?' + base_pattern + r';\s*$', re.M)
        for m in list(alias_re.finditer(text)):
            var = m.group(1)
            if re.search(r'^[ \t]*' + re.escape(var) + r' = (?!(?:\([\w ]+\*\))?' + base_pattern + ';)', text, re.M):
                diag.append(f'alias {var} of {kinds} is reassigned; left as is')
                continue
            raw = m.group(0).split('= ', 1)[1].rstrip().rstrip(';').strip()
            value = raw if raw.startswith('*(') else '(' + raw + ')'
            start = text.index(m.group(0))
            head, tail = text[:start], text[start:].replace(m.group(0), '', 1)
            # substitute only after the assignment: the declaration block must keep `int iVarN;`
            tail = re.sub(r'(?<![\w.>])' + re.escape(var) + r'\b', lambda _: value, tail)
            text = head + tail
        return text
    if spec.entity:
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
                pat_store = re.compile(r'^([ 	]*)\*\(' + TYPE + r' \*\)\(' + base + r' \+ ' + off_re(absolute) + r' \+ (?P<idx>(?:QUEST|ENTITY)STATE_GetInt\(\"[^\"]*\"\)|\w+) \* 4\) =\s*([^;]+);', re.M)
                text = pat_store.sub(lambda m, k=key, kind=kind: f'{m.group(1)}QUESTSTATE_Set{kind}({k.format(idx=m.group("idx"))}, {_lit(m.group(3), kind)});', text)
                pat_load = re.compile(r'\*\(' + TYPE + r' \*\)\(' + base + r' \+ ' + off_re(absolute) + r' \+ (?P<idx>(?:QUEST|ENTITY)STATE_GetInt\(\"[^\"]*\"\)|\w+) \* 4\)')
                text = pat_load.sub(lambda m, k=key, kind=kind, tag=tag: f'{tag}STATE_Get{kind}({k.format(idx=m.group("idx"))})', text)
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
            # temporaries holding an element address: substitute and drop the assignment
            for m in list(re.finditer(r'^[ \t]*(\w+) = (?:\(int \*\))?(__element\("' + a['name'] + r'", [^;]+\));\s*$', text, re.M)):
                var, value = m.group(1), m.group(2)
                if len(re.findall(r'^[ \t]*' + re.escape(var) + r' = ', text, re.M)) == 1:
                    text = text.replace(m.group(0), '')
                    text = re.sub(r'(?<![\w.>])' + re.escape(var) + r'\b', lambda _: value, text)
            # static index forms already expanded in evidence as scalar fields (Teams_0_MemberCount): lifter state map
        # 3b. Thing members
        for off, name in things.items():
            recv = f'{tag}THING_Get("{name}")'
            text = re.sub(r'CScriptThing::operator=\(\(CScriptThing \*\)\(' + base + r' \+ ' + off_re(off) + r'\),\s*([^;]+)\);',
                          lambda m, name=name, tag=tag: f'{tag}THING_Set("{name}", {"nil" if m.group(1).strip() == "(CScriptThing *)0x0" else m.group(1).strip()});', text)
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
            end = r'\*\(int \*\)\(' + base + r' \+ ' + off_re(off + 4) + r'\)'
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
                scope, rest = (tail[:nxt.start()], tail[nxt.start():]) if nxt else (tail, '')
                scope = re.sub(r'\*' + re.escape(var) + r'\b', f'{tag}LIST_BeginValue("{name}")', scope)
                scope = re.sub(r'\b' + re.escape(var) + r'\[1\]', f'{tag}LIST_EndValue("{name}")', scope)
                text = head + scope + rest
            # element: *(int *)(BEGIN + IDX)  (IDX in bytes, stride 0xc) and vcalls on it
            def elem_vcall(m, name=name, tag=tag):
                out = thing_call(f'{tag}LIST_At_{name}(({m.group(1)}) / 0xc)', m.group(2), m.end(), text[m.end():m.end() + 1])
                return out if out else m.group(0)
            text = re.sub(r'\(\*\*\(code \*\*\)\(\*\(int \*\)\(' + begin + r' \+ (\w+)\) \+ (0x[0-9a-f]+|\d+)\)\)\s*\(', elem_vcall, text)
            text = re.sub(r'\(CScriptThing \*\)\(' + begin + r' \+ (\w+)\)', lambda m, name=name, tag=tag: f'{tag}LIST_At_{name}(({m.group(1)}) / 0xc)', text)
            text = re.sub(begin + r' \+ (\w+)\b', lambda m, name=name, tag=tag: f'{tag}LIST_At_{name}(({m.group(1)}) / 0xc)', text)
            text = re.sub(begin, f'{tag}LIST_BeginValue("{name}")', text)
            text = re.sub(end, f'{tag}LIST_EndValue("{name}")', text)
            bv, ev = f'{tag}LIST_BeginValue("{name}")', f'{tag}LIST_EndValue("{name}")'
            # arithmetic on begin/end values after alias substitution
            text = text.replace(f'{ev} - {bv}', f'({tag}LIST_Count("{name}") * 0xc)')
            text = re.sub(r'\*\(int \*\)\(' + re.escape(bv) + r' \+ (\w+)\)', lambda m, name=name, tag=tag: f'{tag}LIST_At_{name}(({m.group(1)}) / 0xc)', text)
            text = re.sub(r'\*' + re.escape(bv), f'{tag}LIST_At_{name}(0)', text)
            text = re.sub(re.escape(bv) + r' \+ (\w+)\b', lambda m, name=name, tag=tag: f'{tag}LIST_At_{name}(({m.group(1)}) / 0xc)', text)
            text = re.sub(r'\(int \*\)\(' + base + r' \+ ' + off_re(off) + r'\)', f'{tag}LIST_Ref("{name}")', text)
            text = re.sub(r'Vector_PushBack_ScriptThing\(\(void \*\)\(' + base + r' \+ ' + off_re(off) + r'\),\s*([^;]+)\);',
                          lambda m, name=name, tag=tag: f'{tag}LIST_Push("{name}", {m.group(1).strip()});', text)

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
    text = re.sub(r'\(int \*\)(\w+)\._4_4_ == \(int \*\)0x0', r'!__thing_valid(\1)', text)
    text = re.sub(r'\(int \*\)(\w+)\._4_4_ != \(int \*\)0x0', r'__thing_valid(\1)', text)
    text = re.sub(r'\(\*\*\(code \*\*\)\(\*\(int \*\)(\w+)\._4_4_ \+ (0x[0-9a-f]+|\d+)\)\)\s*\(',
                  lambda h: thing_call(h.group(1), h.group(2), h.end(), text[h.end():h.end() + 1]) or h.group(0), text)
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
                # bare member address as a by-reference argument
                text = re.sub(r'(?<![\w*(])' + ptr + r' \+ ' + off_re(member_off) + r'(?![\w])', recv, text)
                text = re.sub(r'CScriptThing::operator=\(' + re.escape(recv) + r',\s*([^;]+)\);',
                              lambda m, a=a, idx=idx, tname=tname: f'QUESTTHING_Set(__key("{a["name"]}_" .. {idx} .. "_{tname}"), {m.group(1).strip()});', text)
    text = re.sub(r'\b(\d+(?:\.\d+)?)e\+?(-?\d+)\b', lambda m: repr(float(m.group(0))), text)
    text = drop_local_counted_releases(text)
    text = rename_scalar_stack_locals(text)
    return text, diag


GSI_RECEIVER = r'(?:(?:\(\w+ \*\*?\))?\*\((?:int|void|undefined4|CScriptThing_bv|CCharString_bv|C3DVector_bv) \*\*\)\(this \+ (?:4|0x40)\)|DAT_0143e8f8)'
ME_RECEIVER = r'(?:\(CScriptThing(?:_bv)? \*\))?\(this \+ 8\)'


def strip_receiver_arguments(text: str) -> str:
    """Typed exports show the __thiscall receiver as an explicit first argument of overridden
    calls; the lifter's `GSI->Name(` / `CScriptThing::Name(me, ` forms carry it implicitly."""
    text = re.sub(r'(GSI->\w+\()' + GSI_RECEIVER + r'(?:,\s*|(?=\)))', r'\1', text)
    # aliases of the interface pointer (`this_00 = *(int **)(this + 0x40);`) as explicit receivers
    for alias in set(re.findall(r'^[ \t]*(\w+) = \*\((?:int|void|undefined4) \*\*\)\(this \+ (?:4|0x40)\);', text, re.M)):
        text = re.sub(r'(GSI->\w+\()' + re.escape(alias) + r'(?:,\s*|(?=\)))', r'\1', text)
    text = re.sub(r'(CScriptThing::\w+\(me, ?)' + ME_RECEIVER + r'(?:,\s*|(?=\)))', r'\1', text)
    text = re.sub(r'(CScriptThing::\w+\((\w+), ?)\2(?:,\s*|(?=\)))', r'\1', text)
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
    (re.compile(r'QUESTLIST_Count\('), 'quest:GetStateListCount('),
    (re.compile(r'QUESTLIST_At\('), 'quest:GetStateListAt('),
    (re.compile(r'QUESTLIST_Push\('), 'quest:StateListPush('),
    (re.compile(r'QUESTLIST_(?:BeginValue|Ref)\('), 'quest:GetStateListRef('),
    (re.compile(r'QUESTLIST_EndValue\('), 'quest:GetStateListEnd('),
    (re.compile(r'ENTITYLIST_(?:BeginValue|Ref)\('), '__native_entity_state:GetStateListRef('),
    (re.compile(r'ENTITYLIST_EndValue\('), '__native_entity_state:GetStateListEnd('),
    (re.compile(r'ENTITYLIST_Push\('), '__native_entity_state:StateListPush('),
    (re.compile(r'ENTITYLIST_Count\('), '__native_entity_state:GetStateListCount('),
    (re.compile(r'ENTITYLIST_At\('), '__native_entity_state:GetStateListAt('),
    (re.compile(r'__thing_valid\('), 'IsThingValid('),
    (re.compile(r'ACTORMAP_New\('), 'resources:NewActorMap('),
    (re.compile(r'QUESTTHING_Empty\(\)'), 'nil'),
    (re.compile(r'ENGINE_GlobalGameData\('), 'quest:ReadGlobalGameData('),
    (re.compile(r'ACTORMAP_Set\('), 'resources:SetActor('),
    (re.compile(r'ACTORMAP_Destroy\('), 'resources:DestroyActorMap('),
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
    text = _expand_calls(text, 'ENGINE_Concat', lambda a: '(' + ' .. '.join(a) + ')')
    text = _expand_calls(text, 'ENGINE_SquaredDistance', lambda a: f'(quest:GetDistanceBetweenThings({", ".join(a)}) ^ 2)')
    text = _expand_calls(text, 'ENGINE_StrCmp', lambda a: f'(({a[0]} == {a[1]}) and 0 or 1)' if len(a) == 2 else 'ENGINE_StrCmp(' + ', '.join(a) + ')')
    text = re.sub(r'(QUEST|ENTITY)LIST_At_(\w+)\(', lambda m: ('quest:GetStateListAt(' if m.group(1) == 'QUEST' else '__native_entity_state:GetStateListAt(') + '"' + m.group(2) + '", ', text)
    for pattern, repl in LUA_PSEUDO:
        text = pattern.sub(repl, text)
    text = KEY.sub('(', text)
    text = re.sub(r'&("[^"]*")', r'\1', text)
    return text
