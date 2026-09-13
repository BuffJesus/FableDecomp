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
    return {'parameters': params, 'returnKind': ('bool' if kind[1] in ('bool', 'char') else 'number') if kind else None}


def rename_parameters(source, signature):
    names = {p['native']: p['lua'] for p in signature['parameters']}
    if not names:
        return source
    pattern = re.compile(r'(?P<protected>/\*[\s\S]*?\*/|//[^\n]*|"(?:[^"\\]|\\.)*"|\'(?:[^\'\\]|\\.)*\')|'
                         r'\b(?P<name>' + '|'.join(re.escape(n) for n in names) + r')\b')
    return pattern.sub(lambda m: names[m['name']] if m['name'] else m[0], source)
