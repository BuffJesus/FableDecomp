"""Split native call operands without splitting nested expressions or quoted literals."""
from __future__ import annotations

import re


_POINTER_CAST_OR_LITERAL = re.compile(
    r'(?P<literal>"(?:[^"\\]|\\.)*"|\'(?:[^\'\\]|\\.)*\')|'
    r'(?P<cast>\([A-Za-z_]\w*(?:::\w+)*<[\w:<>,*&\s]+>\s*\*+\s*\))')


def strip_template_pointer_casts(text: str) -> str:
    """Erase template pointer type spellings, preserving literals and non-pointer expressions."""
    def replace(match: re.Match) -> str:
        if match['cast'] is None:
            return match[0]
        depth = 0
        for char in match[0]:
            if char == '<':
                depth += 1
            elif char == '>':
                depth -= 1
                if depth < 0:
                    return match[0]
        return '' if depth == 0 else match[0]

    return _POINTER_CAST_OR_LITERAL.sub(replace, text)


def split_arguments(text: str) -> list[str]:
    if not text.strip():
        return []
    operands = []
    stack = []
    quote = None
    escaped = False
    comment = None
    start = 0
    index = 0
    closing = {')': '(', ']': '[', '}': '{'}
    while index < len(text):
        char = text[index]
        pair = text[index:index + 2]
        if comment == '//':
            if char == '\n':
                comment = None
        elif comment == '/*':
            if pair == '*/':
                comment = None
                index += 1
        elif quote:
            if escaped:
                escaped = False
            elif char == '\\':
                escaped = True
            elif char == quote:
                quote = None
        elif pair in ('//', '/*'):
            comment = pair
            index += 1
        elif char in ('"', "'"):
            quote = char
        elif char in '([{':
            stack.append(char)
        elif char in closing:
            if not stack or stack.pop() != closing[char]:
                raise ValueError('unbalanced call operand delimiters')
        elif char == ',' and not stack:
            operand = text[start:index].strip()
            if not operand:
                raise ValueError('empty call operand')
            operands.append(operand)
            start = index + 1
        index += 1
    if quote or stack or comment == '/*':
        raise ValueError('unterminated call operand')
    tail = text[start:].strip()
    if not tail:
        raise ValueError('empty final call operand')
    operands.append(tail)
    return operands
