"""Expand nested comma assignments in if conditions without eager evaluation."""
import re

from tools.script_recovery.native_arguments import split_arguments
from tools.script_recovery.native_conditions import _unwrap, _LITERALS, conditional_call_assignment


def _tree(text):
    text = _unwrap(text)
    parts = split_arguments(text)
    if len(parts) > 1:
        if not all(re.match(r'^(?:[A-Za-z]{1,3}Var\d+(?:_\d+)?|native_arg_\w+)\s*=(?!=)', part) for part in parts[:-1]):
            raise ValueError('unsupported sequence effect')
        return ('sequence', parts[:-1], _tree(parts[-1]))
    masked = _LITERALS.sub(lambda m: ' ' * len(m[0]), text)
    for operator in ('||', '&&'):
        depth = 0
        for i, char in enumerate(masked):
            if char in '([{':
                depth += 1
            elif char in ')]}':
                depth -= 1
            elif depth == 0 and masked[i:i + 2] == operator:
                return (operator, _tree(text[:i]), _tree(text[i + 2:]))
    if re.search(r'(?<![=!<>])=(?!=)|\?|\+\+|--', masked):
        raise ValueError('unsupported conditional expression')
    return ('leaf', text)


def expand_sequence_conditions(statements, reserved, preserve=None):
    """Return statements and changed assignment targets; other conditions stay intact."""
    used, assigned, output = set(reserved), set(), []
    serial = 0
    def temporary():
        nonlocal serial
        while True:
            serial += 1
            name = f'native_arg_sequence_{serial}'
            if name not in used:
                used.add(name)
                return name
    def has_sequence(node):
        return node[0] == 'sequence' or node[0] != 'leaf' and any(has_sequence(n) for n in node[1:])
    def emit(node, target):
        kind = node[0]
        if kind == 'leaf':
            return [f'if ({node[1]}) {{', f'{target} = true;', '}', 'else {', f'{target} = false;', '}']
        if kind == 'sequence':
            for effect in node[1]:
                assigned.add(re.match(r'^\w+', effect)[0])
            return [effect + ';' for effect in node[1]] + emit(node[2], target)
        lines = emit(node[1], target)
        guard = target if kind == '&&' else '!' + target
        return lines + [f'if ({guard}) {{'] + emit(node[2], target) + ['}']
    for statement in statements:
        if preserve is not None and preserve(statement):
            output.append(statement)
            continue
        match = re.fullmatch(r'\s*if\s*\((.*)\)\s*(?:goto\s+(LAB_[0-9a-f]+);|\{)\s*', statement)
        if match and match[2] is None and conditional_call_assignment(statement):
            output.append(statement)
            continue
        if match:
            try:
                tree = _tree(match[1])
            except ValueError:
                tree = None
            if tree is not None and has_sequence(tree):
                target = temporary()
                assigned.add(target)
                branch = f'if ({target}) goto {match[2]};' if match[2] else f'if ({target}) {{'
                output.extend([f'{target} = false;'] + emit(tree, target) + [branch])
                continue
        output.append(statement)
    return output, assigned
