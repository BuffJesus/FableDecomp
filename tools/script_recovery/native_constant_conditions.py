"""Fold only decisive literal left operands of short-circuit conditions."""
import re

from tools.script_recovery.native_arguments import split_arguments


def fold_decisive_condition(source):
    match = re.fullmatch(r'\((nil|-?\d+|0x[0-9a-f]+)\s*(==|~=)\s*(nil|-?\d+|0x[0-9a-f]+)\)\s+(or|and)\s+(.+)', source)
    if not match:
        return source
    if (match[1] == 'nil') != (match[3] == 'nil'):
        return source
    # The RHS must be one complete parenthesized expression. Otherwise outer operators
    # may have a different precedence and must be handled by a full expression parser.
    rhs = match[5].strip()
    if not rhs.startswith('(') or not rhs.endswith(')'):
        return source
    try:
        split_arguments(rhs[1:-1])
    except ValueError:
        return source
    def value(token):
        return None if token == 'nil' else int(token, 16 if token.startswith('0x') else 10)
    left = value(match[1]) == value(match[3])
    if match[2] == '~=':
        left = not left
    if match[4] == 'or' and left:
        return 'true'
    if match[4] == 'and' and not left:
        return 'false'
    return source
