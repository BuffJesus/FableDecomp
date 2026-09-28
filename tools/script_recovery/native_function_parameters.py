"""Preserve explicit native method parameters separately from the implicit script receiver."""
import re

from tools.script_recovery.native_arguments import split_arguments


def function_parameters(source, *, member=False):
    header = re.sub(r'/\*[\s\S]*?\*/|//[^\n]*', '', source).split('{', 1)[0].strip()
    match = re.search(r'\(([^()]*)\)\s*$', header)
    if not match:
        return {'parameters': [], 'returnKind': None, 'problem': 'unparsed native signature'}
    operands = split_arguments(match[1])
    if operands == ['void']:
        operands = []
    if (member or '__thiscall' in header) and operands:
        operands = operands[1:]  # Ghidra's explicit representation of ECX.
    params = []
    for operand in operands:
        name = re.search(r'\b([A-Za-z_]\w*)\s*$', operand)
        if not name:
            return {'parameters': [], 'returnKind': None, 'problem': 'unparsed parameter: ' + operand}
        params.append({'native': name[1], 'lua': 'native_arg_' + name[1],
                       'type': operand[:name.start()].strip()})
    kind = re.match(r'\s*(bool|char|long|int|uint|float|double)\b', header)
    # the ego_r bsim signature in the leading comment is the reviewed prototype: when it says void, Ghidra's
    # guessed int is a leftover register (a pointer temporary), not a result
    comment = re.search(r'/\*\s*\[bsim[^\]]*\]\s*([\s\S]*?)\*/', source)
    # ... and its `class CScriptThing const &` / `class C3DVector const &` parameters outrank an untyped `int`
    # (V_BeardyBaldy SetWanderPointAndDistance 0x00E53F60 exported as (int param_1, int param_2): the thing
    # operand of all four wander calls was lifted as a number and dropped, 2026-09-28)
    proto = comment and re.search(r'__thiscall\s+[\w:~<>]+\(([^()]*)\)', comment.group(1))
    if proto:
        reviewed = [] if proto.group(1).strip() in ('', 'void') else split_arguments(proto.group(1))
        if len(reviewed) == len(params):
            for p, r in zip(params, reviewed):
                ref = re.fullmatch(r'\s*class\s+(CScriptThing|C3DVector)\s+const\s*&\s*', r)
                if ref and p['type'] in ('int', 'undefined4'):
                    p['type'] = ref.group(1) + ' *'
        # ... and parameters the export dropped: V_BookCollecting BS_Teacher AskForBook 0x00E55CE0 is
        # `AskForBook(long, class CCharString)` in ego_r (RET 8), exported as one `CCharString_bv param_1` that the body
        # uses as the book index while it reads the refusal key at `stack0x00000008`. When every reviewed parameter is
        # one dword and each missing one's stack slot (this-call: 4 * position) is referenced, the missing parameters
        # are appended under their slot names, and the reviewed scalar types replace the export's confused ones.
        dword = re.compile(r'\s*(?:(?:unsigned\s+)?(?:long|int|char|bool|short|float)|class\s+CCharString|[\w:<> ]+\s*[*&])\s*')
        if len(reviewed) > len(params) and all(dword.fullmatch(r) for r in reviewed):
            body = source.split('{', 1)[1] if '{' in source else ''
            missing = [f'stack0x{4 * (i + 1):08x}' for i in range(len(params), len(reviewed))]
            if all(re.search(r'\b' + slot + r'\b', body) for slot in missing):
                for p, r in zip(params, reviewed):
                    scalar = re.fullmatch(r'\s*((?:unsigned\s+)?(?:long|int|bool|short|float))\s*', r)
                    if scalar and 'CCharString' in p['type']:
                        p['type'] = scalar.group(1)
                for i, slot in enumerate(missing, start=len(params)):
                    r = reviewed[i]
                    params.append({'native': slot, 'lua': f'native_arg_param_{i + 1}',
                                   'type': 'CCharString' if 'CCharString' in r else r.replace('class ', '').strip()})
    bsim_void = bool(comment and re.search(r'\bvoid\s+__thiscall\b', comment.group(1)))
    # likewise a reviewed `bool __thiscall` outranks Ghidra's `int` (IsThingCarryingCrate: a Lua boolean
    # result must not be tested with `~= 0`)
    bsim_bool = bool(comment and re.search(r'\bbool\s+__thiscall\b', comment.group(1)))
    string_value = bool(re.match(r'CCharString\s+__thiscall\b', header))
    return {'parameters': params,
            'returnKind': None if bsim_void else 'bool' if bsim_bool else 'string' if string_value else (('bool' if kind[1] in ('bool', 'char') else 'number') if kind else None),
            'bsimVoid': bsim_void}


def rename_parameters(source, signature):
    names = {p['native']: p['lua'] for p in signature['parameters']}
    if not names:
        return source
    pattern = re.compile(r'(?P<protected>/\*[\s\S]*?\*/|//[^\n]*|"(?:[^"\\]|\\.)*"|\'(?:[^\'\\]|\\.)*\')|'
                         r'\b(?P<name>' + '|'.join(re.escape(n) for n in names) + r')\b')
    return pattern.sub(lambda m: names[m['name']] if m['name'] else m[0], source)
