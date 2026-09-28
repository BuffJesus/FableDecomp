"""Member string vectors that Init fills entirely from wide .rdata literals.

CChickenSign (V_ChickenKicking, Init 0x00E68DC0) declares `vector<CWideString> TextKeys` at +0x1c
(Ego_r.pdb). Init resizes it to 8 and assigns each slot a UTF-16 literal; Main only reads
`*(int *)(this + 0x1c) + PrizesWon * 4`. Without this pass the member lowered to an Int state field
and Main added a nil to a number.

When every slot is assigned exactly once from a resolvable literal and every other reference in the
owner's functions is that indexed read, the vector is immutable data: the file gets
`local TextKeys = {...}`, Init's construction statements go, and reads become
`LOCALLIST_StringAt(TextKeys, i)` (expanded to `TextKeys[i + 1]`). Anything else leaves the source untouched.
"""
import re

VECTOR_TYPES = ('vector<CWideString,', 'vector<CCharString,')


def _off(offset: int) -> str:
    return rf'(?:0x{offset:x}|{offset})'


def _stmt(body: str):
    # Ghidra wraps long template names across lines: match whole statements whitespace-tolerantly
    return re.compile(r'^[ \t]*' + body + r'\s*;[ \t]*\r?\n', re.M)


def _init_block(init: str, offset: int):
    """(exact construction statements, literal addresses) when Init builds the vector from literals."""
    off = _off(offset)
    alias_m = _stmt(r'(\w+) = \([^;]*?\*\)\s*\(\(int\)this \+ ' + off + r'\)').search(init)
    if not alias_m:
        return None
    alias = re.escape(alias_m.group(1))
    resize = _stmt(r'std::vector<[^;]*?>::resize\s*\(\s*' + alias + r',\s*(\d+|0x[0-9a-f]+),\s*(\w+)\)').search(init)
    if not resize:
        return None
    count, fill = int(resize.group(1), 0), re.escape(resize.group(2))
    spans = [alias_m.group(0), resize.group(0)]
    # the fill value: a default-constructed temporary, its destructor, and the slot's staging store
    ctor = _stmt(fill + r' = CCharString::CCharString\(\(CCharString \*\)&(\w+)\)').search(init)
    if not ctor:
        return None
    tmp = re.escape(ctor.group(1))
    spans.append(ctor.group(0))
    for extra in (_stmt(r'CCharString::~CCharString\(\(CCharString \*\)&' + tmp + r'\)'), _stmt(tmp + r' = this')):
        e = extra.search(init)
        if e:
            spans.append(e.group(0))
    slots = {}
    begin = r'\*\((?:void|int) \*\*?\)' + alias
    assign = _stmt(r'CCharString(?:::|__)AssignFromWide\s*\(\s*(?:\(void \*\))?\(?\s*' + begin
                   + r'(?: \+ (0x[0-9a-f]+|\d+))?\)?,\s*(0x[0-9a-f]+)\)')
    for m in assign.finditer(init):
        index, rem = divmod(int(m.group(1) or '0', 0), 4)
        if rem or index in slots:
            return None
        slots[index] = int(m.group(2), 16)
        spans.append(m.group(0))
    if sorted(slots) != list(range(count)):
        return None
    # the alias, the fill value and its temporary are used by nothing else (declarations aside)
    rest = init
    for sp in spans:
        rest = rest.replace(sp, '', 1)
    rest = re.sub(r'^[ \t]*[\w<>,:* ]+?\**\b\w+;[ \t]*\r?$', '', rest, flags=re.M)
    if re.search(r'\b(?:' + alias + '|' + fill + '|' + tmp + r')\b', rest):
        return None
    return spans, [slots[i] for i in range(count)]


def read_pattern(offset: int):
    """`*(int *)(this + OFF) + EXPR * 4`, optionally cast; EXPR is one operand up to two paren levels deep."""
    return re.compile(r'(?:\(\w+ \*\)\s*)?\(\*\(int \*\)\(this \+ ' + _off(offset)
                      + r'\) \+ ((?:[^()]|\((?:[^()]|\([^()]*\))*\))+?) \* 4\)')


def recover(fields, functions, resolve_wide):
    """{offset: (name, [literals], init_spans)} for immutable literal string vectors.

    fields: the owner's unmappedFields rows; functions: {role: decompile}."""
    init = functions.get('Init')
    if not init or not resolve_wide:
        return {}
    found = {}
    for row in fields:
        if not row.get('type', '').startswith(VECTOR_TYPES):
            continue
        offset = int(row['offset'], 16)
        block = _init_block(init, offset)
        if not block:
            continue
        spans, addresses = block
        literals = [resolve_wide(a) for a in addresses]
        if any(v is None for v in literals):
            continue
        # every other mention of the member in the owner is the indexed read
        pat, off = read_pattern(offset), _off(offset)
        clean = True
        for role, text in functions.items():
            if role == 'Init':
                for sp in spans:
                    text = text.replace(sp, '', 1)
            if re.search(r'this \+ ' + off + r'\)', pat.sub('', text)):
                clean = False
                break
        if clean:
            found[offset] = (row['name'], literals, spans)
    return found


def apply(decompile: str, role: str, vectors) -> str:
    for offset, (name, _, spans) in vectors.items():
        if role == 'Init':
            for sp in spans:
                decompile = decompile.replace(sp, '', 1)
        decompile = read_pattern(offset).sub(lambda m, name=name: f'LOCALLIST_StringAt({name}, {m.group(1)})', decompile)
    return decompile


def prelude(vectors, owner_class: str) -> str:
    def lua(s):
        return '"' + s.replace('\\', '\\\\').replace('"', '\\"') + '"'
    out = []
    for offset, (name, literals, _) in sorted(vectors.items()):
        out.append(f'-- {owner_class}::{name} (+0x{offset:x}): filled once by native Init from .rdata literals, never written again')
        out.append(f'local {name} = {{' + ', '.join(lua(s) for s in literals) + '}')
    return '\n'.join(out) + '\n'
