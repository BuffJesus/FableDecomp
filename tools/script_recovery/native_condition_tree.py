"""General short-circuit condition lowering for lifted native `if` headers.

`conditional_call_assignment` handles the common single `A && (x = call(), pred)` shape. Retail
scripts also nest them: `A && ((c = f(), c != 0) || (i = g(...), (char)i == 0))`. This module
parses the condition into a tree of `&&` / `||` nodes, comma sequences and leaves, so the lifter
can emit each conditional call as a guarded statement and keep C's evaluation order:

    c = <A>
    if c then
        c2 = <(c = f(), c != 0)>          -- statements for the call, then the predicate
        if not c2 then ... end            -- `||` right operand
        c = c2
    end
    if c then

Only the structure is decided here; the lifter renders leaves and statements.
"""
from __future__ import annotations

import re

try:
    from .native_arguments import split_arguments
except ImportError:
    from native_arguments import split_arguments

_LITERALS = re.compile(r'"(?:[^"\\]|\\.)*"|\'(?:[^\'\\]|\\.)*\'')


def _unwrap(text):
    text = text.strip()
    while text.startswith('(') and text.endswith(')'):
        # only strip when the parens enclose the whole expression
        depth = 0
        masked = _LITERALS.sub(lambda m: ' ' * len(m[0]), text)
        for i, ch in enumerate(masked):
            depth += (ch == '(') - (ch == ')')
            if depth == 0 and i < len(masked) - 1:
                return text
        text = text[1:-1].strip()
    return text


def _split_top(text, ops=('&&', '||')):
    """Split at top-level occurrences of one operator kind; returns (op, parts) or (None, [text])."""
    masked = _LITERALS.sub(lambda m: ' ' * len(m[0]), text)
    for op in ops:
        depth, parts, start = 0, [], 0
        for i, ch in enumerate(masked):
            if ch in '([{':
                depth += 1
            elif ch in ')]}':
                depth -= 1
            elif depth == 0 and masked[i:i + 2] == op:
                parts.append(text[start:i]); start = i + 2
        if parts:
            parts.append(text[start:])
            return op, [p for p in parts]
    return None, [text]


ASSIGN_NAME = re.compile(r'^\s*(\w+)\s*=\s*(?!=)(.+)$', re.S)


class _Assign:
    """A sequence statement: `name = expr`, or a store through any lvalue (`*(undefined1 *)(P + 0x4d) = 1`,
    HerosOldHouse ExtraBooty) -- a top-level `=` that is not part of `==`, `!=`, `<=`, `>=`."""
    @staticmethod
    def match(item):
        if ASSIGN_NAME.match(item):
            return True
        masked = _LITERALS.sub(lambda m: ' ' * len(m[0]), item)
        depth = 0
        for i, ch in enumerate(masked):
            if ch in '([{':
                depth += 1
            elif ch in ')]}':
                depth -= 1
            elif ch == '=' and depth == 0:
                prev, nxt = masked[i - 1] if i else '', masked[i + 1] if i + 1 < len(masked) else ''
                return prev not in '=!<>' and nxt != '=' and bool(masked[:i].strip())
        return False


ASSIGN = _Assign


def parse(condition):
    """Tree: ('and'|'or', [nodes]) | ('seq', [stmt_text...], predicate_node) | ('leaf', text)."""
    text = _unwrap(condition)
    # the comma operator binds weakest, then `||`, then `&&`
    try:
        items = split_arguments(text)
    except ValueError:
        items = [text]
    if len(items) > 1 and all(ASSIGN.match(i) for i in items[:-1]):
        return ('seq', [i.strip() for i in items[:-1]], parse(items[-1]))
    op, parts = _split_top(text, ('||',))
    if op:
        return ('or', [parse(p) for p in parts])
    op, parts = _split_top(text, ('&&',))
    if op:
        return ('and', [parse(p) for p in parts])
    return ('leaf', text)


def has_call_assignment(condition):
    """True when some sequence member assigns a call result (the shapes worth lowering)."""
    def walk(node):
        kind = node[0]
        if kind == 'seq':
            # any sequence element: a Lua expression cannot hold a statement, call or not (HerosOldHouse
            # ExtraBooty's `(parent->BootyDugUp = 1, !parent->WifeAttacked)` lifted as `GetStateBool(..) = 1`)
            return bool(node[1]) or walk(node[2])
        if kind in ('and', 'or'):
            return any(walk(n) for n in node[1])
        return False
    return walk(parse(condition))


def needs_tree(condition):
    """True when the single-operator recogniser cannot express the condition: several logical
    operators, more than two operands, or a sequence whose predicate is itself compound."""
    def walk(node):
        kind = node[0]
        if kind in ('and', 'or'):
            return 1 + len(node[1]) - 2 + sum(walk(n) for n in node[1])
        if kind == 'seq':
            return (1 if node[2][0] != 'leaf' else 0) + walk(node[2])
        return 0
    return walk(parse(condition)) >= 2 if parse(condition)[0] != 'leaf' else False
