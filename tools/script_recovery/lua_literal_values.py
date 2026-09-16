"""Inline literal staging only when its unique assignment dominates every read."""
import re
from collections import defaultdict

from tools.script_recovery.lua_local_versions import flow_graph
from tools.script_recovery.readable_lua import GENERATED, rename_identifiers, tokens


def inline_literal_locals(source):
    original = source.splitlines(keepends=True)
    structure = ''.join(re.sub(r'[^\n]', ' ', t[0]) if t.lastgroup in
                        ('comment', 'longcomment', 'string', 'longstring') else t[0]
                        for t in tokens(source)).splitlines()
    graph = flow_graph(structure)
    if graph is None:
        return source, {}
    declarations = defaultdict(list)
    for i, line in enumerate(structure):
        match = re.fullmatch(r'\s*local ([\w, ]+)\s*', line)
        if match:
            for name in map(str.strip, match[1].split(',')):
                if GENERATED.fullmatch(name):
                    declarations[name].append(i)
    output, evidence = list(original), {}
    marker = '__literal_read_marker__'
    while marker in source:
        marker += '_'
    for name, sites in declarations.items():
        if len(sites) != 1:
            continue
        declaration = sites[0]
        assignment = re.compile(r'^\s*' + re.escape(name) + r'\s*=(?!=)\s*(.*?)\s*$')
        definitions = [i for i, line in enumerate(original) if assignment.fullmatch(line)]
        if len(definitions) != 1:
            continue
        definition = definitions[0]
        expression = assignment.fullmatch(original[definition])[1]
        parts = [t for t in tokens(expression) if t.lastgroup not in ('space', 'comment', 'longcomment')]
        # Keep comments at the removed assignment, and reject multiline strings.
        literal = ''.join(t[0] for t in parts)
        is_string = len(parts) == 1 and parts[0].lastgroup == 'string'
        if not is_string and not re.fullmatch(
                r'(?:nil|true|false|-?(?:0[xX][0-9a-fA-F]+|\d+(?:\.\d*)?(?:[eE][-+]?\d+)?))', literal):
            continue
        if definition <= declaration:
            continue
        reads, unsupported = set(), False
        for i, line in enumerate(original):
            if i in (declaration, definition):
                continue
            if rename_identifiers(line, {name: marker}) == line:
                continue
            # Never replace an assignment target, shadowing declaration or a
            # table constructor key mistaken for a value expression.
            stream = [t for t in tokens(line) if t.lastgroup not in
                      ('space', 'comment', 'longcomment', 'string', 'longstring')]
            eq = next((n for n, t in enumerate(stream) if t[0] == '='), None)
            if (eq is not None and any(t[0] == name for t in stream[:eq])) or re.match(r'\s*local\b', line):
                unsupported = True
                break
            reads.add(i)
        if unsupported or not reads:
            continue
        # Removing the definition node exposes every path that could reach a
        # read with nil or a previous loop iteration's value instead of this literal.
        reachable, queue = set(), [0]
        while queue:
            i = queue.pop()
            if i == definition or i in reachable:
                continue
            reachable.add(i)
            queue.extend(graph[i] - reachable)
        if reads & reachable:
            continue
        changed = {declaration: output[declaration], definition: output[definition]}
        replacement = '(' + literal + ')'
        for i in reads:
            changed[i] = output[i]
            output[i] = rename_identifiers(output[i], {name: replacement})
        names = [n.strip() for n in output[declaration].strip().removeprefix('local ').split(',')]
        names.remove(name)
        indent = original[declaration][:len(original[declaration])-len(original[declaration].lstrip())]
        output[declaration] = indent + 'local ' + ', '.join(names) + '\n' if names else '\n'
        comments = ''.join(t[0] for t in tokens(original[definition]) if t.lastgroup in ('comment', 'longcomment'))
        output[definition] = indent + comments + '\n' if comments else '\n'
        evidence[name] = {'literal': literal, 'definitionLine': definition+1,
                          'readLines': sorted(i+1 for i in reads),
                          'basis': 'single literal definition dominates every read',
                          'previousLines': {i+1: text for i, text in changed.items()}}
    restored = list(output)
    for entry in reversed(list(evidence.values())):
        for line, text in entry['previousLines'].items():
            restored[line-1] = text
    if ''.join(restored) != source:
        raise ValueError('literal inlining changed unrelated source')
    return ''.join(output), evidence
