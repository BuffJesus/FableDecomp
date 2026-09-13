"""Conservative read-before-overwrite checks over normalized native statements."""
import re

from tools.script_recovery.native_flow_ir import build_flow_graph


_NON_CODE = re.compile(r'"(?:[^"\\]|\\.)*"|\'(?:[^\'\\]|\\.)*\'|//[^\n]*|/\*[\s\S]*?\*/')


def dead_after_marker(statements, marker, names):
    """Prove every path from marker overwrites each local before reading it.

    Branches deliberately include extra paths. Unknown structures, escaping local
    addresses and missing jump targets reject proof. Revisiting the marker kills
    the old values: the replaced comparison assigns both pointers before reading.
    """
    lines = [_NON_CODE.sub(lambda m: ' ' * len(m[0]), line).strip() for line in statements]
    if lines.count(marker) != 1:
        return False
    for name in names:
        if any(re.search(r'(?<!&)\&\s*\(*\s*' + re.escape(name) + r'\b', line) for line in lines):
            return False
    graph = build_flow_graph(statements)
    if graph is None:
        return False
    lines = [node.code for node in graph.statements]
    edges = {node.index: node.successors for node in graph.statements}
    start = lines.index(marker)
    for name in names:
        pending, visited = [start + 1], set()
        token = re.compile(r'\b' + re.escape(name) + r'\b')
        write = re.compile(re.escape(name) + r'\s*=\s*(?!=)(.*);$')
        while pending:
            index = pending.pop()
            if index == start or index >= len(lines) or index in visited:
                continue
            visited.add(index)
            line = lines[index]
            assigned = write.fullmatch(line)
            if assigned and not token.search(assigned[1]):
                continue
            if token.search(line):
                return False
            pending.extend(edges[index])
    return True
