"""Fail-closed value-use checks over a restricted generated Lua function CFG."""
import re
from collections import deque
from tools.script_recovery.readable_lua import tokens


def _masked(source):
    return ''.join(re.sub(r'[^\r\n]', ' ', t[0]) if t.lastgroup in
                   ('comment', 'longcomment', 'string', 'longstring') else t[0]
                   for t in tokens(source))


def build_cfg(source, required_alias=None):
    """Parse the restricted generated normal CFG; retain branch identities."""
    def require(ok, why):
        if not ok:
            raise ValueError('Lua staging liveness: ' + why)
    clean = _masked(source)
    lines = [line.strip() for line in clean.splitlines()]
    require(not any(';' in line for line in lines), 'multiple statements/semicolon syntax')
    header = re.fullmatch(r'function\s+[A-Za-z_]\w*\(([^()]*)\)', lines[0])
    require(header is not None and lines[-1] == 'end', 'expected one generated function')
    require(not re.search(r'\bfunction\b', '\n'.join(lines[1:])), 'nested function/closure')
    locals_ = set()
    if header[1].strip():
        names = [name.strip() for name in header[1].split(',')]
        require(all(re.fullmatch(r'[A-Za-z_]\w*', n) for n in names), 'unsupported parameters')
        require(len(names) == len(set(names)), 'shadowed/redeclared parameter')
        locals_.update(names)
    for line in lines[1:-1]:
        if line.startswith('local '):
            names = [n.strip() for n in line[6:].split('=', 1)[0].split(',')]
            require(all(re.fullmatch(r'[A-Za-z_]\w*', n) for n in names), 'unsupported declaration')
            require(not locals_.intersection(names) and len(names) == len(set(names)), 'shadowed/redeclared local')
            locals_.update(names)
    if required_alias is not None:require(required_alias in locals_, 'staging alias is not local')
    labels, nodes = {}, {}

    def sequence(pos, stops):
        result = []
        while pos < len(lines) - 1:
            line = lines[pos]
            if any(line == stop or line.startswith(stop + ' ') for stop in stops):
                return result, pos
            if re.fullmatch(r'if\s+.+\s+then\s+(?:goto\s+[A-Za-z_]\w*|return(?:\s+.+)?)\s+end', line):
                result.append(('inline', pos, line)); pos += 1
            elif re.fullmatch(r'if\s+.+\s+then', line):
                opening = pos
                body, pos = sequence(pos + 1, ('elseif', 'else', 'end'))
                arms = [(opening, body)]
                while pos < len(lines) - 1 and lines[pos].startswith('elseif '):
                    require(re.fullmatch(r'elseif\s+.+\s+then', lines[pos]) is not None, 'unsupported elseif')
                    branch = pos; body, pos = sequence(pos + 1, ('elseif', 'else', 'end')); arms.append((branch, body))
                otherwise = []
                if pos < len(lines) - 1 and lines[pos] == 'else':
                    otherwise, pos = sequence(pos + 1, ('end',))
                require(pos < len(lines) - 1 and lines[pos] == 'end', 'unbalanced if')
                result.append(('if', opening, arms, otherwise)); pos += 1
            elif re.fullmatch(r'while\s+.+\s+do', line):
                opening = pos; body, pos = sequence(pos + 1, ('end',))
                require(pos < len(lines) - 1 and lines[pos] == 'end', 'unbalanced while')
                result.append(('while', opening, body)); pos += 1
            elif line == 'do return end':
                result.append(('scoped_return', pos, line)); pos += 1
            elif line == 'break':
                result.append(('break', pos, line)); pos += 1
            elif line == 'repeat':
                opening = pos; body, pos = sequence(pos + 1, ('until',))
                require(pos < len(lines) - 1 and re.fullmatch(r'until\s+.+', lines[pos]) is not None, 'unbalanced repeat')
                result.append(('repeat', opening, body, pos)); pos += 1
            else:
                require(not re.match(r'^(?:if|elseif|else|end|until|while|for|do|break|function)\b', line), 'unsupported control syntax')
                label = re.fullmatch(r'::([A-Za-z_]\w*)::', line)
                if label:
                    require(label[1] not in labels, 'duplicate label'); labels[label[1]] = pos
                require(not line.startswith('goto ') or re.fullmatch(r'goto\s+[A-Za-z_]\w*', line) is not None, 'unsupported goto')
                result.append(('leaf', pos, line)); pos += 1
        return result, pos

    tree, end = sequence(1, ())
    require(end == len(lines) - 1, 'unexpected function ending')

    branches = {}
    def link(items, following, break_target=None, inside_loop=False):
        for node in reversed(items):
            kind, at = node[:2]
            if kind == 'if':
                no = link(node[3], following, break_target, inside_loop)
                for head, body in reversed(node[2]):
                    yes = link(body, following, break_target, inside_loop); nodes[head] = (lines[head], {yes, no}); branches[head] = (yes, no); no = head
                following = no
            elif kind == 'while':
                body = link(node[2], at, following, True); nodes[at] = (lines[at], {body, following}); following = at
            elif kind == 'repeat':
                until = node[3]; body = link(node[2], until, following, True); nodes[until] = (lines[until], {body, following}); nodes[at] = ('', {body}); following = at
            elif kind == 'break':
                require(inside_loop, 'break outside loop')
                nodes[at] = ('', {break_target}); following = at
            else:
                line = node[2]
                if kind == 'scoped_return':
                    edges = set()
                elif kind == 'inline':
                    match = re.fullmatch(r'if\s+(.+)\s+then\s+(goto\s+([A-Za-z_]\w*)|return(?:\s+.+)?)\s+end', line)
                    require(match is not None, 'invalid inline control')
                    if match[3]:
                        require(match[3] in labels, 'missing inline jump target'); edges = {following, labels[match[3]]}
                    else:
                        edges = {following}
                elif line.startswith('goto '):
                    label = line.split()[1]; require(label in labels, 'missing jump target'); edges = {labels[label]}
                elif re.match(r'^return(?:\s|$)', line):
                    edges = set()
                else:
                    edges = {following}
                nodes[at] = (line, edges); following = at
        return following

    entry = link(tree, None)
    return dict(nodes=nodes, locals=locals_, branches=branches, entry=entry)


def prove_unused_staging(source, assignment_offset, alias):
    """Allow local copies, but reject any observable read before overwrite.

    Both alternatives and loop outcomes are retained. Unknown control syntax,
    closures, duplicate declarations and missing labels reject proof. No Lua
    statement is removed by this analysis.
    """
    def require(ok, why):
        if not ok:
            raise ValueError('Lua staging liveness: ' + why)
    require(re.fullmatch(r'[A-Za-z_]\w*', alias) is not None, 'invalid alias')
    cfg = build_cfg(source, required_alias=alias)
    nodes, locals_ = cfg['nodes'], cfg['locals']
    require(0 <= assignment_offset < len(source), 'assignment offset out of range')
    marker = source[:assignment_offset].count('\n')
    require(marker in nodes and re.fullmatch(re.escape(alias) + r'\s*=\s*nil', nodes[marker][0]) is not None, 'not the staging nil assignment')
    incoming = {}; pending = deque((target, frozenset([alias])) for target in nodes[marker][1] if target is not None); copied = set(); visits = 0
    while pending:
        at, tainted = pending.popleft()
        if not tainted: continue
        merged = incoming.get(at, frozenset()) | tainted
        if incoming.get(at) == merged: continue
        incoming[at] = merged; visits += 1
        require(visits < 50000, 'unbounded flow')
        line, edges = nodes[at]; tainted = set(merged)
        assignment = re.fullmatch(r'([A-Za-z_]\w*)\s*=([^=].*|\s*)', line)
        if assignment:
            lhs, rhs = assignment.groups(); reads = set(re.findall(r'\b[A-Za-z_]\w*\b', rhs)) & tainted
            if reads:
                require(lhs in locals_ and rhs.strip() in tainted, 'live staging value read/escape')
                tainted.add(lhs); copied.add(lhs)
            else:
                tainted.discard(lhs)
        elif not re.fullmatch(r'::[A-Za-z_]\w*::|goto\s+[A-Za-z_]\w*', line):
            require(not set(re.findall(r'\b[A-Za-z_]\w*\b', line)).intersection(tainted), 'live staging value read/escape')
        for target in edges:
            if target is not None: pending.append((target, frozenset(tainted)))
    return dict(assignmentLine=marker + 1, alias=alias, copiedAliases=sorted(copied), visitedNodes=len(incoming),
                scope='generated Lua normal CFG;local copies allowed,all observable reads rejected;no statements removed')
