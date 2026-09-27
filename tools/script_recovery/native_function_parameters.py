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
