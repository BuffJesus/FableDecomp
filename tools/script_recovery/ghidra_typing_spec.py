"""Build the prototype spec for the typed translation-unit export from FSE's native typedefs.

FSE (ForgeFSE-retail-shadow) calls the retail engine through `__thiscall` typedefs that encode the
real ABI (hidden return slots, by-value CScriptThing/CCharString/C3DVector). This turns them into a
JSON that `ExportTypedTranslationUnit.java` applies inside a read-only Ghidra session:

  slots     GSI vtable offset -> {name, ret, params}      (GameInterface.h + GameInterface.cpp pVTable[i])
  helpers   fixed engine address -> {name, cc, ret, params} (FableAPI.cpp ASLR<t...>(0x...) lines)
  thingSlots CScriptThing vtable offset -> {name, ret, params} (EntityScriptingAPI.h CScriptThingVTable)

Types are reduced to what the decompiler needs for argument recovery: sizes and pointer-ness.
"""
from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
FSE = Path(r'D:\Code\ForgeFSE-retail-shadow\FableScriptExtender')
TYPEDEF = re.compile(r'typedef\s+(.+?)\s*\(\s*(__thiscall|__cdecl|__stdcall|__fastcall)?\s*\*\s*(t\w+)\s*\)\s*\((.*?)\)\s*;', re.S)
SLOT = re.compile(r'^\s*(\w+)\s*=\s*\(\s*(t\w+)\s*\)\s*pVTable\[(\d+)\]', re.M)
FIXED = re.compile(r'^\s*(\w+)\s*=\s*ASLR<(t\w+)>\((0x[0-9A-Fa-f]+)\)', re.M)
BY_VALUE = {'CScriptThing': 12, 'CCharString': 4, 'C3DVector': 12, 'CRGBColour': 4, 'CWideString': 4, 'CRGBFloatColour': 16}


def ghidra_type(ctype: str) -> str:
    t = ctype.strip().replace('const ', '').replace('struct ', '').strip()
    if t.endswith('&'):
        t = t[:-1].strip() + '*'
    if t.endswith('*'):
        base = t.rstrip('* ').strip()
        return f'{base} *' if base in BY_VALUE else 'void *'
    if t in ('void',):
        return 'void'
    if t in ('bool',):
        return 'bool'
    if t in ('float',):
        return 'float'
    if t in ('double',):
        return 'double'
    if t in ('unsigned __int64', 'uint64_t', '__int64'):
        return 'ulonglong'
    if t in ('unsigned int', 'unsigned long', 'size_t', 'DWORD', 'unsigned'):
        return 'uint'
    if t in ('int', 'long', 'short', 'char', 'unsigned char', 'unsigned short') or t.startswith('E'):
        return 'int'
    if t in BY_VALUE:
        return t
    return 'void *' if 'std::' in t or '<' in t else 'int'


def parse_params(text: str):
    params = []
    depth, cur = 0, ''
    for ch in text:
        if ch == '<':
            depth += 1
        elif ch == '>':
            depth -= 1
        if ch == ',' and depth == 0:
            params.append(cur); cur = ''
        else:
            cur += ch
    if cur.strip():
        params.append(cur)
    out = []
    for p in params:
        p = ' '.join(p.split())
        if p in ('', 'void'):
            continue
        m = re.match(r'(.+?)\s*([A-Za-z_]\w*)$', p)
        if m and not p.endswith('*') and not p.endswith('&'):
            ctype, name = m.group(1), m.group(2)
        else:
            ctype, name = p, f'p{len(out)}'
        out.append({'name': name, 'type': ghidra_type(ctype), 'ctype': ctype})
    return out


def drop_this(params):
    """__thiscall passes the receiver in ECX; the typedefs spell it as a first `This` parameter."""
    return params[1:] if params and params[0]['name'].lower() == 'this' else params


def typedefs(*headers):
    out = {}
    for h in headers:
        for ret, cc, name, params in TYPEDEF.findall(h.read_text(encoding='utf-8', errors='replace')):
            out[name] = {'ret': ghidra_type(ret), 'cret': ' '.join(ret.split()), 'cc': cc or '__cdecl',
                         'params': parse_params(params)}
    return out


def pdb_parameters(tsv):
    """qualified function name -> [(name, size, stack offset)] for stack parameters, in push order."""
    out, current = {}, None
    for line in Path(tsv).read_text(encoding='utf-8', errors='replace').splitlines():
        cols = line.split('\t')
        if cols[0] == 'FUNCTION' and len(cols) >= 4:
            current = cols[1]
            out.setdefault(current, [])
        elif current and len(cols) >= 10 and cols[0] == '0' and cols[6] == '3' and cols[7] == '3':
            try:
                out[current].append((cols[2], int(cols[5]), int(cols[9])))
            except ValueError:
                pass
    return {k: sorted(set(v), key=lambda t: t[2]) for k, v in out.items()}


def size_type(size):
    return {1: 'bool', 4: 'int', 12: 'CScriptThing', 8: 'ulonglong'}.get(size, 'int')


SIGNATURE = re.compile(r'/\*\s*\[bsim[^\]]*\]\s*(?:public|private|protected):\s*(?:virtual\s+)?[\w:<>,]+\s+__thiscall\s+[\w:<>~]+\((.*?)\)\s*(?:const)?\s*\*/', re.S)


def signature_types(unit_dir):
    """address -> parameter type names from the ego_r signature Ghidra's bsim match recorded in the
    decompile header (`class CScriptThing &`, `class CCharString const &`, ...). The PDB locals
    export only gives sizes for pointer/reference parameters; the signature gives the class."""
    tu = Path(unit_dir) / 'translation_unit.json'
    if not tu.exists():
        return {}
    out = {}
    for f in json.loads(tu.read_text(encoding='utf-8-sig'))['functions']:
        m = SIGNATURE.search(f.get('decompile') or '')
        if not m:
            continue
        types = []
        for raw in [t.strip() for t in m.group(1).split(',') if t.strip()] if m.group(1).strip() != 'void' else []:
            base = re.sub(r'\b(class|struct|enum|const)\b', '', raw).strip()
            ref = base.endswith('&') or base.endswith('*')
            base = base.rstrip('&* ').split('::')[-1]
            if base in ('CScriptThing', 'CCharString', 'CWideString', 'C3DVector', 'CRGBColour'):
                types.append(f'{base} *' if ref else base)
            elif base in ('float', 'bool', 'int', 'long', 'unsigned long', 'unsigned int'):
                types.append({'float': 'float', 'bool': 'bool'}.get(base, 'int'))
            else:
                types.append(None)
        out[f['address'].lower()] = types
    return out


def unit_functions(unit_dir):
    """Prototypes for a unit's own functions (quest members, entity members, helpers) from the PDB."""
    params = pdb_parameters(Path(unit_dir) / 'pdb' / 'Ego_r-pdb-locals.tsv')
    signatures = signature_types(unit_dir)
    out = {}
    for path in sorted((Path(unit_dir) / 'units').glob('*.json')):
        unit = json.loads(path.read_text(encoding='utf-8'))
        qualified = unit['nativeClass']
        rows = [(name, f['address'], f'{qualified}::{name}') for name, f in unit['quest']['functions'].items()
                if name != 'destructor' and not name.startswith('helper_')]
        void_names = {'Main', 'Init', 'OnPersist', 'RegisterMain', 'OnPredicateFail'} | {
            n for n, f in unit['quest']['functions'].items() if f.get('spawnedAs')}
        for ename, ent in unit['entities'].items():
            klass = ent['nativeClass']
            for name, f in list(ent['functions'].items()) + list(ent.get('helpers', {}).items()):
                if name in ('destructor', 'GetParentScript', 'OnInterrupted') or name.startswith('helper_'):
                    continue
                rows.append((f'{ename}.{name}', f['address'], f'{klass}::{name}'))
        for label, address, qname in rows:
            if qname not in params:
                continue
            plist = [{'name': n, 'type': size_type(sz), 'ctype': f'pdb size {sz}'} for n, sz, _ in params[qname]]
            sig = signatures.get(address.lower())
            if sig and len(sig) == len(plist):
                for entry, stype in zip(plist, sig):
                    if stype and (entry['type'] == 'int' and stype.endswith('*') or entry['type'] == 'CScriptThing' and stype == 'CScriptThing'):
                        entry['type'], entry['ctype'] = stype, 'ego_r signature'
            out.setdefault(address.lower(), {'name': qname.split('::', 1)[1].replace('::', '__'), 'cc': '__thiscall',
                                             'ret': 'void' if qname.split('::')[-1] in void_names else 'int',
                                             'params': plist, 'source': 'PDB stack parameters'})
    return out


# Engine helpers proven by disassembly (not FSE-exposed): CCharString operator+ returning through a
# hidden pointer in ECX; EDX = left operand, one stack operand; ret 4.
EXTRA_HELPERS = {
    # CRT float->int truncation: the operand arrives in ST0 (fld before the call), the result in EAX;
    # without the ST0 parameter Ghidra drops the operand (`uVar = __ftol2();`).
    '0xbfea70': {'name': '__ftol2', 'cc': '__cdecl', 'ret': 'int',
                 'params': [{'name': 'value', 'type': 'float10', 'storage': 'ST0'}],
                 'source': 'disassembly 2026-09-17'},
    '0x99f570': {'name': 'CCharString_ConcatString', 'cc': '__fastcall', 'ret': 'CCharString *',
                 'params': [{'name': 'dest', 'type': 'CCharString *'}, {'name': 'a', 'type': 'CCharString *'}, {'name': 'b', 'type': 'CCharString *'}],
                 'source': 'disassembly 2026-09-16'},
    '0x99f600': {'name': 'CCharString_ConcatCString', 'cc': '__fastcall', 'ret': 'CCharString *',
                 'params': [{'name': 'dest', 'type': 'CCharString *'}, {'name': 'a', 'type': 'CCharString *'}, {'name': 'b', 'type': 'char *'}],
                 'source': 'disassembly 2026-09-16'},
}


def main():
    a = argparse.ArgumentParser(description=__doc__)
    a.add_argument('--fse', type=Path, default=FSE)
    a.add_argument('--out', type=Path, default=ROOT / 'refs/script_recovery/typing/gsi_prototypes.json')
    a.add_argument('--unit', help='also type this unit\'s own functions from its PDB parameters')
    args = a.parse_args()
    tds = typedefs(args.fse / 'GameInterface.h', args.fse / 'FableAPI.h', args.fse / 'EntityScriptingAPI.h')
    slots = {}
    for api, td, index in SLOT.findall((args.fse / 'GameInterface.cpp').read_text(encoding='utf-8', errors='replace')):
        if td in tds:
            spec = tds[td]
            # drop the explicit `This` first parameter: __thiscall passes it in ECX
            params = drop_this(spec['params'])
            slots[hex(int(index) * 4)] = {'name': api.replace('_API', ''), 'ret': spec['ret'], 'params': params, 'typedef': td}
    helpers = {}
    for api, td, address in FIXED.findall((args.fse / 'FableAPI.cpp').read_text(encoding='utf-8', errors='replace')):
        if td in tds:
            spec = tds[td]
            cc = spec['cc']
            params = spec['params']
            if cc == '__thiscall':
                params = drop_this(params)
            helpers[hex(int(address, 16))] = {'name': api, 'cc': cc, 'ret': spec['ret'], 'params': params, 'typedef': td}
    thing_slots = {}
    header = (args.fse / 'EntityScriptingAPI.h').read_text(encoding='utf-8', errors='replace')
    block = header[header.index('struct CScriptThingVTable'):]
    block = block[:block.index('};')]
    for td, name, offset in re.findall(r'(t\w+)\s+(\w+);\s*//\s*(0x[0-9A-Fa-f]+)', block):
        if td in tds:
            spec = tds[td]
            params = drop_this(spec['params'])
            thing_slots[hex(int(offset, 16))] = {'name': name, 'ret': spec['ret'], 'params': params, 'typedef': td}
    unit_fns = {}
    if args.unit:
        from tools.script_recovery.script_units import unit as script_unit
        unit_fns = unit_functions(script_unit(args.unit)['evidence'])
        helpers.update(unit_fns)
    for address, spec in EXTRA_HELPERS.items():
        helpers.setdefault(address, spec)
        args.out = script_unit(args.unit)['evidence'] / 'typing_spec.json'
    args.out.parent.mkdir(parents=True, exist_ok=True)
    spec = {'schema': 'ghidra-typing-spec/1', 'source': str(args.fse), 'byValue': BY_VALUE,
            'slots': slots, 'helpers': helpers, 'thingSlots': thing_slots}
    args.out.write_text(json.dumps(spec, indent=1) + '\n', encoding='utf-8')
    print(json.dumps({'slots': len(slots), 'helpers': len(helpers), 'unitFunctions': len(unit_fns), 'thingSlots': len(thing_slots), 'out': str(args.out)}, indent=2))


if __name__ == '__main__':
    main()
