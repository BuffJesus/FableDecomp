"""Split hoisted generated locals using reaching definitions on supported Lua CFGs.

Unsupported syntax/closures fail closed. All values meeting at a read are joined;
therefore a new name never substitutes one branch's value for another's.
"""
import re
from collections import defaultdict, deque


# `if C then goto L end` and the emitter's hoisted-region form, `if C then __region_X(); goto L end`:
# the leading statements are plain calls, so the line stays one node with the jump's two successors
INLINE_JUMP = re.compile(r'if .+ then (?:\w+(?:[.:]\w+)*\([^;]*\); )*(?:goto \w+|return(?: [^;]*)?|break) end')


def closure_ranges(lines):
    """Line spans of the emitter's hoisted cleanup/region closures (`local function __x() ... end`), which run
    at their call sites rather than in place. Returns None when a closure is not a plain top-of-body block."""
    ranges, start, depth = [], None, 0
    # `elseif` / `else` continue their own `if`, so they are not openers
    opens = re.compile(r'(?:if .+ then|while .+ do|for .+ do|repeat|do|(?:local )?function .*)')
    # the chunk's own header may itself read `local function __resource_main(...)`: that is the body, not a closure
    root = next((i for i, raw in enumerate(lines) if re.match(r'(?:local )?function ', raw.strip())), -1)
    for i, raw in enumerate(lines):
        if i <= root:
            continue
        line = raw.strip()
        if start is None:
            if re.fullmatch(r'local function \w+\([^)]*\)', line):
                start, depth = i, 1
            continue
        if line == 'end' or line.startswith('until '):
            depth -= 1
            if depth == 0:
                ranges.append((start, i))
                start = None
        elif opens.fullmatch(line) and not INLINE_JUMP.fullmatch(line):
            depth += 1
    return None if start is not None else ranges


def flow_graph(lines):
    """Return successors for the emitter's structured one-statement-per-line Lua."""
    stack, blocks, owners, labels, inline = [], {}, {}, {}, {}
    root_end = None
    for i, raw in enumerate(lines):
        line = raw.strip()
        if not line:
            continue
        if re.match(r'^(?:local )?function ', line):
            if stack:
                return None  # Capturing closures need lexical binding analysis.
            stack.append({'kind': 'function', 'start': i})
        elif INLINE_JUMP.fullmatch(line):
            inline[i] = line
            if line.endswith('break end'):
                loop = next((b for b in reversed(stack) if b['kind'] in ('while', 'repeat')), None)
                if loop is None:
                    return None
                owners[i] = loop
        elif re.fullmatch(r'if .+ then', line):
            stack.append({'kind': 'if', 'start': i, 'branches': [i]})
        elif re.fullmatch(r'elseif .+ then', line) or line == 'else':
            if not stack or stack[-1]['kind'] != 'if':
                return None
            stack[-1]['branches'].append(i)
        elif re.fullmatch(r'while .+ do', line):
            stack.append({'kind': 'while', 'start': i})
        elif line in ('repeat', 'do'):
            stack.append({'kind': line, 'start': i})
        elif line == 'end' or line.startswith('until '):
            if not stack:
                return None
            block = stack.pop()
            if (block['kind'] == 'repeat') != line.startswith('until '):
                return None
            block['end'] = i
            blocks[block['start']] = block
            owners[i] = block
            for branch in block.get('branches', []):
                owners[branch] = block
            if block['kind'] == 'function':
                root_end = i
        elif re.fullmatch(r'::\w+::', line):
            labels[line[2:-2]] = i
        elif re.match(r'^(if|elseif|for|function|local function|until)\b', line):
            return None
        elif re.match(r'^\w+\s*,[\w, ]*=', line):
            return None  # Parallel assignment needs simultaneous-definition handling.
        if 'function(' in line or 'function (' in line:
            return None
        if line == 'break':
            loop = next((b for b in reversed(stack) if b['kind'] in ('while', 'repeat')), None)
            if loop is None:
                return None
            owners[i] = loop
    if stack or root_end is None or any(line.strip() for line in lines[root_end + 1:]):
        return None
    def following(i):
        target = i + 1
        while target < len(lines) and not lines[target].strip():
            target += 1
        if target >= len(lines):
            return []
        if lines[target].strip() == 'else' or lines[target].strip().startswith('elseif '):
            return following(owners[target]['end'])
        return [target]
    edges = {}
    for i, raw in enumerate(lines):
        line = raw.strip()
        targets = following(i)
        if i in inline:
            jump = re.search(r'goto (\w+) end$', line)   # the hoisted-region form puts calls before the jump
            if jump:
                if jump[1] not in labels:
                    return None
                targets += [labels[jump[1]]]
            elif line.endswith('break end'):
                targets += following(owners[i]['end'])
        elif line.startswith('goto '):
            name = line.removeprefix('goto ')
            if name not in labels:
                return None
            targets = [labels[name]]
        elif line.startswith('return') or line.startswith('do return '):
            targets = []
        elif line == 'break':
            targets = following(owners[i]['end'])
        elif line.startswith(('if ', 'elseif ')):
            block = owners[i]
            branch = block['branches'].index(i)
            next_branch = block['branches'][branch + 1] if branch + 1 < len(block['branches']) else block['end']
            targets = following(i) + [next_branch]
        elif i in blocks and blocks[i]['kind'] == 'while':
            targets = following(i) + following(blocks[i]['end'])
        elif line.startswith('until '):
            targets = following(i) + following(owners[i]['start'])
        elif line == 'end' and owners[i]['kind'] == 'while':
            targets = [owners[i]['start']]
        edges[i] = set(targets)
    return edges


def split_hoisted_locals(source):
    from tools.script_recovery.readable_lua import GENERATED, rename_identifiers, tokens
    original = source.splitlines(keepends=True)
    # Blank comments/literals for structure; retain newlines and all variable tokens.
    code = ''.join(re.sub(r'[^\n]', ' ', t[0]) if t.lastgroup in
                   ('comment', 'longcomment', 'string', 'longstring') else t[0] for t in tokens(source))
    lines = code.splitlines()
    # the hoisted cleanup/region closures do not run where they are written; blank them so the rest of the body
    # has a graph, and pin every name they touch — a closure call could read or write it at any of its sites
    ranges = closure_ranges(lines)
    if ranges is None:
        return source, {}
    pinned = set()
    for a, b in ranges:
        for i in range(a, b + 1):
            pinned.update(t[0] for t in tokens(lines[i]) if t.lastgroup == 'identifier')
            lines[i] = ''
    graph = flow_graph(lines)
    if graph is None:
        return source, {}
    declarations = {}
    local_count = len(re.findall(r'\b[A-Za-z_]\w*\b', lines[0].split('(', 1)[-1].split(')', 1)[0]))
    for i, line in enumerate(lines):
        any_local = re.match(r'\s*local ([\w, ]+?)(?:\s*=|\s*$)', line)
        if any_local:
            local_count += len(any_local[1].split(','))
        match = re.fullmatch(r'\s*local ([\w, ]+)\s*', line)
        if match:
            for name in map(str.strip, match[1].split(',')):
                if GENERATED.fullmatch(name):
                    declarations[name] = i if name not in declarations else None
    occupied = {t[0] for t in tokens(source) if t.lastgroup == 'identifier'}
    parents = defaultdict(set)
    for i, targets in graph.items():
        for target in targets:
            parents[target].add(i)
    output, info = list(original), {}
    for name, declaration in declarations.items():
        if declaration is None or name in pinned:
            continue
        pattern = re.compile(r'^(\s*)(?:local\s+)?' + re.escape(name) + r'\s*=(?!=)')
        definitions = {i for i, line in enumerate(lines) if pattern.match(line)}
        if len(definitions) < 2:
            continue
        if re.search(r'\b' + re.escape(name) + r'\b', lines[0]):
            continue
        # A nested declaration/shadowing is not a value assignment to this binding.
        if any(re.match(r'\s*local\s+' + re.escape(name) + r'\s*=', line) for line in lines):
            continue
        reads = set()
        for i, line in enumerate(lines):
            if i == declaration:
                continue
            rhs = pattern.sub('', line, count=1)
            if rename_identifiers(rhs, {name: '__read_marker__'}) != rhs:
                reads.add(i)
        if any(i < declaration for i in reads | definitions):
            continue
        incoming, outgoing = defaultdict(set), defaultdict(set)
        queue, queued = deque([0]), {0}
        visited = set()
        while queue:
            i = queue.popleft(); queued.discard(i)
            values = {-1} if i == 0 else set().union(*(outgoing[p] for p in parents[i]))
            after = {i} if i in definitions else values
            changed = incoming[i] != values or outgoing[i] != after or i not in visited
            incoming[i], outgoing[i] = values, after
            visited.add(i)
            if changed:
                for target in graph[i]:
                    if target not in queued:
                        queue.append(target); queued.add(target)
        if any(not incoming[i] for i in reads):
            continue
        representative = {i: i for i in definitions | {-1}}
        def root(i):
            while representative[i] != i:
                representative[i] = representative[representative[i]]
                i = representative[i]
            return i
        for read in reads:
            values = sorted(incoming[read])
            for value in values[1:]:
                representative[root(value)] = root(values[0])
        used_definitions = set().union(*(incoming[i] for i in reads)) if reads else set()
        dead = sorted(definitions - used_definitions)
        for definition in dead[1:]:
            representative[root(definition)] = root(dead[0])
        groups = defaultdict(list)
        for definition in definitions:
            groups[root(definition)].append(definition)
        if len(groups) < 2:
            continue
        # An uninitialized read without any reaching assignment keeps a nil local.
        if any(root(-1) == root(d) for i in reads for d in incoming[i]):
            groups.setdefault(root(-1), [])
        # Lua 5.4 permits only 200 locals. Leave space for existing initialized
        # locals and decline further splitting rather than breaking compilation.
        if local_count + len(groups) - 1 > 180:
            continue
        local_count += len(groups) - 1
        names = {}
        for index, group in enumerate(sorted(groups), 1):
            version = f'{name}_{index}'
            while version in occupied:
                version += '_1'
            occupied.add(version)
            names[group] = version
        for i, line in enumerate(output):
            if i == declaration:
                output[i] = rename_identifiers(line, {name: ', '.join(names.values())})
                continue
            if i in reads:
                version = names[root(next(iter(incoming[i])))]
                output[i] = rename_identifiers(line, {name: version})
            if i in definitions:
                # Rewrite only the assignment target; RHS may require an older version.
                output[i] = re.sub(r'^(\s*)\w+(\s*=)',
                    lambda m: m[1] + names[root(i)] + m[2], output[i], count=1)
        info[name] = {'declarationLine': declaration + 1, 'originalDeclaration': original[declaration],
                      'versions': {version: [i + 1 for i in sorted(groups[group])]
                                   for group, version in names.items()}}
    result = ''.join(output)
    reverse = {version: name for name, mapping in info.items() for version in mapping['versions']}
    restored = rename_identifiers(result, reverse).splitlines(keepends=True)
    for mapping in info.values():
        restored[mapping['declarationLine'] - 1] = mapping['originalDeclaration']
    if ''.join(restored) != source:
        raise ValueError('local versioning changed non-binding source text')
    return result, info


def prune_unused_literal_locals(source):
    """Remove only unused hoisted locals whose stores are side-effect-free literals."""
    from tools.script_recovery.readable_lua import GENERATED, rename_identifiers, tokens
    code = ''.join(re.sub(r'[^\n]', ' ', t[0]) if t.lastgroup in ('comment', 'longcomment')
                   or t.lastgroup in ('string', 'longstring') and '\n' in t[0]
                   else t[0] for t in tokens(source))
    structure = ''.join(re.sub(r'[^\n]', ' ', t[0]) if t.lastgroup in
                        ('comment', 'longcomment', 'string', 'longstring') else t[0] for t in tokens(source))
    if flow_graph(structure.splitlines()) is None:
        return source, {}
    original, lines = source.splitlines(keepends=True), code.splitlines()
    output, removed = list(original), {}
    declarations = defaultdict(list)
    for i, line in enumerate(lines):
        match = re.fullmatch(r'\s*local ([\w, ]+)\s*', line)
        if match:
            for name in map(str.strip, match[1].split(',')):
                if GENERATED.fullmatch(name):
                    declarations[name].append(i)
    marker = '__unused_local_marker__'
    while marker in source:
        marker += '_'
    for name, sites in declarations.items():
        if len(sites) != 1:
            continue
        assignments = []
        pattern = re.compile(r'^\s*' + re.escape(name) + r'\s*=\s*(.+?)\s*$')
        valid = True
        for i, line in enumerate(lines):
            match = pattern.fullmatch(line)
            if not match:
                continue
            expression = match[1]
            # Literal inlining can leave parenthesized staging stores.
            # Removing enclosing pairs is safe only if the remainder is a literal.
            while expression.startswith('(') and expression.endswith(')'):
                expression = expression[1:-1].strip()
            parts = [t for t in tokens(expression) if t.lastgroup != 'space']
            literal = (len(parts) == 1 and parts[0].lastgroup in ('string', 'longstring')) or re.fullmatch(
                r'(?:nil|true|false|[-+]?(?:0[xX][0-9a-fA-F]+|\d+(?:\.\d*)?(?:[eE][-+]?\d+)?))', expression)
            if not literal:
                valid = False
                break
            assignments.append(i)
        if not valid or rename_identifiers(source, {name: marker}).count(marker) != len(assignments) + 1:
            continue
        declaration = sites[0]
        old = output[declaration]
        names = [n.strip() for n in old.strip().removeprefix('local ').split(',')]
        if name not in names:
            continue
        names.remove(name)
        output[declaration] = (old[:len(old) - len(old.lstrip())] + 'local ' + ', '.join(names) + '\n') if names else '\n'
        for i in assignments:
            comments = ''.join(t[0] for t in tokens(original[i]) if t.lastgroup in ('comment', 'longcomment'))
            output[i] = comments + '\n'
        removed[name] = {i + 1: original[i] for i in [declaration, *assignments]}
    restored = list(output)
    for changes in removed.values():
        for line, text in changes.items():
            restored[line - 1] = text
    if ''.join(restored) != source:
        raise ValueError('literal cleanup changed unrelated source')
    return ''.join(output), removed
