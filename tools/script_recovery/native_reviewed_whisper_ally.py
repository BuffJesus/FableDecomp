"""Arena WhisperAlly.Main (0x00F22D80): creature-counter sums and the wrapper IsAlive calls.

- Three loops total the quest's TotalCreatures array (quest +0xD0, stride 4, three
  elements -- the unit member map) through a byte cursor: `iVar12 = 0xd0; do { ...;
  xStack_108 += *(int *)(*(int *)(this + 0x14) + iVar12); iVar12 += 4; } while
  (iVar12 < 0xdc)`. They become the same loops over element indices 0..2 reading
  `TotalCreatures_<i>`; the cursor is dead after each loop (next use redefines it).
- F23A3D / F23E37 call slot 0x12C of the CScriptThing wrapper held in the -0xE8 slot
  (`lea ecx,[esp+0x3c]; mov edx,[esp+0x3c]; call [edx+0x12c]`). Wrapper vtable
  0x01238C8C (stored by the copy constructor 0x004ABE90) slot 0x12C is 0x004AB130,
  CScriptThing::IsAlive -- the same 28 bytes the GTDI_Maze predicate witness verified.

Native bytes of Main, the vtable entry and IsAlive are hash/byte gated; every rewrite
must match its reviewed count, else the function is left unchanged.
"""
import hashlib
import re

MAIN = 0x00F22D80
MAIN_SHA256 = '775e39552329c22c4f512f8af026506f971acafbd6b1d8bb2918abfd904a377a'
WRAPPER_VTABLE = 0x01238C8C
IS_ALIVE = 0x004AB130
IS_ALIVE_SHA256 = '2f058b3a91d179c5bd531a6c5a536915b23fc9e4edce96b78b2029d7d2376000'
SUM_LOOP = re.compile(
    r'(?m)^(?P<i>[ \t]*)xStack_108 = 0;\n'
    r'(?P=i)iVar12 = 0xd0;\n'
    r'(?P=i)do \{\n'
    r'(?P<body>(?:(?P=i)  (?!.*\biVar12\b).*\n)*?)'
    r'(?P=i)  xStack_108 = xStack_108 \+ \*\(int \*\)\(\*\(int \*\)\(this \+ 0x14\) \+ iVar12\);\n'
    r'(?P=i)  iVar12 = iVar12 \+ 4;\n'
    r'(?P=i)\} while \(iVar12 < 0xdc\);\n')
IS_ALIVE_VCALL = 'cVar4 = (**(code **)((int)xStack_e8 + 0x12c))();'


def _cursor_dead_after(text, end):
    # the next mention of the cursor after a loop must be a fresh assignment
    m = re.search(r'\biVar12\b.*', text[end:])
    return m is None or re.match(r'iVar12 = (?!.*\biVar12\b)', m[0]) is not None


def recover_whisper_ally(source, fn, rdata):
    if int(fn['address'], 16) != MAIN:
        return source
    raw = rdata.bytes_at(MAIN, int(fn['bodyEndExclusive'], 16) - MAIN)
    if raw is None or hashlib.sha256(raw).hexdigest() != MAIN_SHA256:
        return source
    entry = rdata.bytes_at(WRAPPER_VTABLE + 0x12C, 4)
    body = rdata.bytes_at(IS_ALIVE, 28)
    if (entry is None or int.from_bytes(entry, 'little') != IS_ALIVE or body is None
            or hashlib.sha256(body).hexdigest() != IS_ALIVE_SHA256):
        return source
    newline = '\r\n' if '\r\n' in source else '\n'
    text = source.replace('\r\n', '\n')
    loops = list(SUM_LOOP.finditer(text))
    if len(loops) != 3 or not all(_cursor_dead_after(text, m.end()) for m in loops):
        return source
    if text.count(IS_ALIVE_VCALL) != 2:
        return source
    # The -0x108 slot holds the creature total for these loops and both `while` tests, and conversation ids /
    # a timer in between. Every path back to the second test passes the end-of-body re-sum (all its gotos
    # target LAB_00f241aa just before it), so the total lifetime is separable: give it its own local.
    tests = ('while ((xStack_108 == 0 && ', 'while ((xStack_108 != 0 && ')
    if any(text.count(t) != 1 for t in tests) or text.count('LAB_00f241aa:\n      xStack_108 = 0;\n') != 1:
        return source
    if not re.search(r'(?m)^  int xStack_108;\n', text) or re.search(r'\bcreatureTotal\b', text):
        return source
    for m in reversed(loops):
        i = m['i']
        text = (text[:m.start()] + f'{i}creatureTotal = 0;\n{i}iVar12 = 0;\n{i}do {{\n' + m['body']
                + f'{i}  creatureTotal = creatureTotal + QUESTSTATE_GetInt(__key("TotalCreatures_" .. iVar12));\n'
                + f'{i}  iVar12 = iVar12 + 1;\n{i}}} while (iVar12 < 3);\n' + text[m.end():])
    for t in tests:
        text = text.replace(t, t.replace('xStack_108', 'creatureTotal'))
    text = re.sub(r'(?m)^  int xStack_108;\n', '  int xStack_108;\n  int creatureTotal;\n', text, count=1)
    text = text.replace(IS_ALIVE_VCALL,
                        'cVar4 = CScriptThing::_IsAlive_CScriptThing__UBE_NXZ((CScriptThing *)xStack_e8);')
    return text.replace('\n', newline)
