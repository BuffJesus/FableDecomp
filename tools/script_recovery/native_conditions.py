"""Recognize a short-circuit condition with one conditional native call assignment."""
import re

try:
    from .native_arguments import split_arguments
except ImportError:
    from native_arguments import split_arguments


_LITERALS = re.compile(r'"(?:[^"\\]|\\.)*"|\'(?:[^\'\\]|\\.)*\'|/\*[\s\S]*?\*/|//[^\n]*')


def _unwrap(text):
    text = text.strip()
    while text.startswith('(') and text.endswith(')'):
        try:
            split_arguments(text[1:-1])
        except ValueError:
            break
        text = text[1:-1].strip()
    return text


def conditional_call_assignment(line):
    """Return emitter operands, or None for ambiguous/multiple logical operators.

    Parentheses may surround either operand or the complete condition. Delimiter
    validation prevents a nested call's commas/operators becoming outer syntax.
    """
    header = re.fullmatch(r'\s*(if|while)\s*\((.*)\)\s*\{\s*', line)
    if not header:
        return None
    condition = _unwrap(header[2])
    try:
        if len(split_arguments(condition)) != 1:
            return None
    except ValueError:
        return None
    masked = _LITERALS.sub(lambda m: ' ' * len(m[0]), condition)
    depth, operators = 0, []
    for i, char in enumerate(masked):
        if char in '([{':
            depth += 1
        elif char in ')]}':
            depth -= 1
        elif depth == 0 and masked[i:i + 2] in ('&&', '||'):
            operators.append(i)
    if len(operators) != 1:
        return None
    at = operators[0]
    left, right = _unwrap(condition[:at]), _unwrap(condition[at + 2:])
    if not left:
        return None
    try:
        operands = split_arguments(right)
    except ValueError:
        return None
    if len(operands) != 2:
        return None
    call = re.fullmatch(r'(\w+)\s*=\s*(GSI->|CScriptThing::|CCharString::)(\w+)\s*\((.*)\)', operands[0])
    if not call:
        return None
    try:
        split_arguments(call[4])
    except ValueError:
        return None
    return header[1], left, condition[at:at + 2], call[1], call[2], call[3], call[4], operands[1]
