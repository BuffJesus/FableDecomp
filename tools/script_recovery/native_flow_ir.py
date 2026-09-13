"""Shared conservative control-flow representation for normalized native statements.

Edges overapproximate branch alternatives. Consumers may prove absence of a path,
but must not infer exact execution order from every represented path.
"""
from dataclasses import dataclass
import re

_NON_CODE = re.compile(r'"(?:[^"\\]|\\.)*"|\'(?:[^\'\\]|\\.)*\'|//[^\n]*|/\*[\s\S]*?\*/')


@dataclass(frozen=True)
class NativeStatement:
    index: int
    source: str
    code: str
    successors: tuple[int, ...]


@dataclass(frozen=True)
class NativeFlowGraph:
    statements: tuple[NativeStatement, ...]
    # False means extra possible paths remain, never that real edges were omitted.
    exact: bool = False


def build_flow_graph(statements):
    """Build conservative successors, rejecting unsupported structural syntax."""
    source = tuple(statements)
    lines = [_NON_CODE.sub(lambda m: ' ' * len(m[0]), line).strip() for line in source]
    labels, stack, closes, enclosing = {}, [], {}, {}
    for index, line in enumerate(lines):
        label = re.match(r'^(LAB_[0-9a-f]+):\s*(.*)$', line)
        if label:
            if label[1] in labels:
                return None
            labels[label[1]] = index
            line = lines[index] = label[2]
        enclosing[index] = tuple(stack)
        if line.startswith('}'):
            if not stack:
                return None
            opening, kind = stack.pop()
            closes[opening] = index
            if line != '}' and not (kind == 'do' and re.fullmatch(r'}\s*while\s*\(.*\);', line)):
                return None
        if line.endswith('{'):
            if re.match(r'^(?:else )?if\s*\(', line):
                kind = 'if'
            elif re.match(r'^(while|for)\s*\(', line):
                kind = 'loop'
            elif line == 'do {':
                kind = 'do'
            elif line == 'else {':
                kind = 'else'
            else:
                return None
            stack.append((index, kind))
        elif re.search(r'\b(?:switch|case|default|continue)\b', line):
            return None
    if stack:
        return None
    edges = {i: {i + 1} for i in range(len(lines))}
    for opening, closing in closes.items():
        header = lines[opening]
        if header != 'else {' and header != 'do {':
            edges[opening].add(closing + 1)
        if header == 'do {' or re.match(r'^(while|for)\s*\(', header):
            edges[closing].add(opening)
    # A completed true arm jumps past every sibling alternative. Falling through
    # an else body would erase a real reaching definition, not merely add paths.
    alternatives = set()
    label_indices = set(labels.values())
    def following(index):
        index += 1
        while index < len(lines) and not lines[index] and index not in label_indices:
            index += 1
        return index
    for opening in sorted(closes):
        if not re.match(r'^if\s*\(', lines[opening]):
            continue
        chain = [opening]
        candidate = following(closes[opening])
        while candidate in closes and re.match(r'^else(?:\s+if\s*\(|\s*\{)', lines[candidate]):
            alternatives.add(candidate)
            chain.append(candidate)
            if lines[candidate] == 'else {':
                break
            candidate = following(closes[candidate])
        if len(chain) > 1:
            join = closes[chain[-1]] + 1
            for arm in chain:
                edges[closes[arm]] = {join}
    if any(line.startswith('else') and i not in alternatives for i, line in enumerate(lines)):
        return None
    for index, line in enumerate(lines):
        jump = re.search(r'\bgoto (LAB_[0-9a-f]+);$', line)
        if jump:
            if jump[1] not in labels:
                return None
            conditional = bool(re.match(r'^if\s*\(', line))
            if not conditional and not line.startswith('goto '):
                return None
            edges[index] = {labels[jump[1]]} | ({index + 1} if conditional else set())
        elif re.search(r'\bgoto\b', line):
            return None
        if re.search(r'\bbreak;$', line):
            loop = next((opening for opening, kind in reversed(enclosing[index]) if kind in ('loop', 'do')), None)
            if loop is None:
                return None
            conditional = bool(re.match(r'^if\s*\(', line))
            if not conditional and line != 'break;':
                return None
            edges[index] = {closes[loop] + 1} | ({index + 1} if conditional else set())
        if re.match(r'^return(?:\s|;)', line):
            edges[index] = set()
    return NativeFlowGraph(tuple(NativeStatement(i, source[i], lines[i],
                            tuple(sorted(j for j in edges[i] if j < len(lines))))
                            for i in range(len(lines))))
