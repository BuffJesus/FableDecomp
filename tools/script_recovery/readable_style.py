"""Quest-script style for the readable stage (docs/scripts/READABLE_STYLE_PLAN.md steps 1, 2, 6).

Every rewrite here is presentation on the emitter's one-statement-per-line Lua and is checked
against the control-flow graph of `lua_local_versions.flow_graph`; a function whose flow the graph
cannot model keeps its text. The draft stays the faithful audit trail; these folds only apply to
the readable copy:

  * termination boilerplate (step 1): `alive = not T(); p = not alive` -> `p = T()`, the wrapping
    `if not T() then BODY end` at a function tail -> `if T() then return end` + BODY, the frame
    idiom `NewScriptFrame(..); if T() then return end` -> `if not NewScriptFrame(..) then return end`
    (the DLL returns `not terminating` from NewScriptFrame for lifetime-None units), and checks
    that a dominating check already answered (terminating can only change across a frame/blocking
    call) are dropped; retry loops become `while not cond do ... end`
  * single-use temporaries (step 2): a temporary assigned once and read once at the very next
    statement is substituted at the read (evaluation order preserved: no completed call precedes
    the read in that statement unless the value is a pure state read)
  * boolean materialisation: `if C then v = true else v = false end` -> `v = C`,
    `if v then v = E end` -> `v = v and E`, empty then-branches inverted
  * cosmetics (step 6): redundant parentheses, `x + -1`, `v ~= false`, blank lines, one
    `local helpers = require(...)` per file, stale `-- LAB_x` comments, `local ret = x; return ret`
"""
from __future__ import annotations

import re

from tools.script_recovery.lua_local_versions import flow_graph
from tools.script_recovery.readable_lua import rename_identifiers, tokens

TERMINATING = r'(?P<recv>\w+):IsActiveThreadTerminating\(\)'
EXIT = r'(?:return|goto \w+|break)'
# calls that cannot advance the scheduler (terminating can only flip across a frame / blocking call)
PURE_METHOD = re.compile(r'^(?:Get|Is|Has|Can|Msg|SetState|GetState|SetMasterGameState|GetMasterGameState|NewResource|TryAcquire|ReleaseResource|RetailResources|ReadGlobalGameData)')
PURE_RECEIVER = {'__native_entity_state'}
STATE_READ = re.compile(r'^(?:\w+:GetState(?:Bool|Int|Float|String|Thing)\(|__native_entity_state:GetState\w+\(|\w+:GetStateList(?:Count|At)\()')


# ----------------------------------------------------------------------------------------- helpers

def _structure(lines):
    """Lines with comments and strings blanked (same shape, safe for regexes on syntax)."""
    text = ''.join(lines)
    return ''.join(re.sub(r'[^\n]', ' ', t[0]) if t.lastgroup in ('comment', 'longcomment', 'string', 'longstring')
                   else t[0] for t in tokens(text)).splitlines()


def _closure_ranges(structure):
    """Top-level `local function` blocks (cleanup helpers): opaque to the flow graph."""
    ranges, i = [], 1
    while i < len(structure):
        m = re.match(r'^(\s*)local function \w+\(', structure[i])
        if m:
            indent = m.group(1)
            j = i + 1
            while j < len(structure) and structure[j] != indent + 'end':
                j += 1
            if j >= len(structure):
                return None
            ranges.append((i, j))
            i = j
        i += 1
    return ranges


class Flow:
    """Flow graph of one function chunk with closure blocks blanked and their upvalues pinned."""

    def __init__(self, lines):
        self.lines = lines
        self.structure = _structure(lines)
        self.ok = False
        ranges = _closure_ranges(self.structure)
        if ranges is None:
            return
        self.pinned = set()
        blanked = list(self.structure)
        for a, b in ranges:
            for i in range(a, b + 1):
                self.pinned.update(t[0] for t in tokens(lines[i]) if t.lastgroup == 'identifier')
                blanked[i] = ''
        # `__cleanup_X(); return` (hoisted cleanup regions) is straight-line code before the exit
        for i, line in enumerate(blanked):
            blanked[i] = re.sub(r'^(\s*(?:if .+ then )?)\w+\(\); (return|goto \w+|break)((?: end)?)$', r'\1\2\3', line)
        self.graph = flow_graph(blanked)
        if self.graph is None:
            return
        self.preds = {i: set() for i in self.graph}
        for i, targets in self.graph.items():
            for t in targets:
                self.preds.setdefault(t, set()).add(i)
        self.ok = True

    def only_ends_after(self, i):
        """True when every path from line i onwards runs through `end` lines only (function exit)."""
        seen, queue = set(), list(self.graph.get(i, ()))
        while queue:
            j = queue.pop()
            if j in seen:
                continue
            seen.add(j)
            if self.structure[j].strip() not in ('end', 'return'):
                return False
            queue.extend(self.graph.get(j, ()))
        return True


def _reads_and_defs(structure, name):
    pattern = re.compile(r'^\s*(?:local )?' + re.escape(name) + r'\s*=(?!=)')
    defs, reads = [], []
    marker = '__style_marker__'
    for i, line in enumerate(structure):
        if re.fullmatch(r'\s*local [\w, ]+\s*', line):
            continue
        is_def = bool(pattern.match(line))
        rest = pattern.sub('', line, count=1) if is_def else line
        if rename_identifiers(rest, {name: marker}) != rest:
            reads.append(i)
        if is_def:
            defs.append(i)
    return defs, reads


def _declared_temporaries(structure):
    names = []
    for line in structure:
        m = re.fullmatch(r'\s*local ([\w, ]+)\s*', line)
        if m:
            names.extend(n.strip() for n in m.group(1).split(','))
    return names


def _remove_declaration(lines, name):
    for i, line in enumerate(lines):
        m = re.fullmatch(r'(\s*)local ([\w, ]+)\n', line)
        if m:
            names = [n for n in m.group(2).split(', ') if n != name]
            if len(names) != len(m.group(2).split(', ')):
                lines[i] = f"{m.group(1)}local {', '.join(names)}\n" if names else ''
                return


def _top_level_operators(expr):
    """Operators at parenthesis depth 0 of an expression (strings skipped)."""
    ops, depth = [], 0
    for t in tokens(expr):
        if t.lastgroup in ('string', 'longstring', 'comment', 'longcomment', 'space'):
            continue
        v = t[0]
        if v in '([{':
            depth += 1
        elif v in ')]}':
            depth -= 1
        elif depth == 0 and (v in ('and', 'or', 'not', '..', '+', '-', '*', '/', '%', '^', '==', '~=', '<', '>', '<=', '>=')):
            ops.append(v)
    return ops


def _atomic(expr):
    expr = expr.strip()
    if _wrapped(expr):
        return True
    return not _top_level_operators(expr)


def _wrapped(expr):
    """Whole expression is one parenthesised group."""
    if not (expr.startswith('(') and expr.endswith(')')):
        return False
    depth = 0
    for i, ch in enumerate(expr):
        depth += (ch == '(') - (ch == ')')
        if depth == 0 and i < len(expr) - 1:
            return False
    return True


KEYWORDS = {'and', 'or', 'not', 'if', 'elseif', 'while', 'until', 'return', 'then', 'do', 'in'}


def _calls(line):
    """(kind, receiver, name) per call: kind is ':' (method), '.' (field function) or '' (bare)."""
    calls = []
    for m in re.finditer(r'(?:(?P<recv>[\w.]+)?(?P<kind>[:.]))?\b(?P<name>\w+)\(', line):
        if m.group('name') in KEYWORDS:
            continue
        calls.append((m.group('kind') or '', m.group('recv'), m.group('name')))
    return calls


def _pure_line(structure_line, raw_line, pure_functions):
    s = structure_line.strip()
    if not s or re.fullmatch(r'(?:end|else|repeat|do|break|return|goto \w+|::\w+::|local [\w, ]+)', s):
        return True
    for kind, recv, name in _calls(raw_line):
        if kind == ':' and (recv in PURE_RECEIVER or PURE_METHOD.match(name)):
            continue
        if kind == '' and name in pure_functions:
            continue
        return False
    return True


def _negate(cond):
    cond = cond.strip()
    if cond.startswith('not ') and _atomic(cond[4:]):
        return cond[4:]
    ops = _top_level_operators(cond)
    flips = {'==': '~=', '~=': '==', '<': '>=', '>=': '<', '>': '<=', '<=': '>'}
    if len(ops) == 1 and ops[0] in flips:
        return re.sub(r' ' + re.escape(ops[0]) + r' ', f' {flips[ops[0]]} ', cond, count=1)
    return f'not {cond}' if _atomic(cond) else f'not ({cond})'


# ------------------------------------------------------------------------------------------ passes

def normalize_alive(lines):
    """`alive = not T()` + `p = not alive` -> `p = T()` when `alive` has no other readers; then
    every remaining `alive =` staging goes (`alive = NewScriptFrame(..)` becomes the bare call)."""
    structure = _structure(lines)
    pairs = []
    for i in range(len(lines) - 1):
        a = re.fullmatch(r'(\s*)alive = not (\w+:IsActiveThreadTerminating\(\))\n', lines[i])
        b = re.fullmatch(r'(\s*)(\w+) = not alive\n', lines[i + 1])
        if a and b and a.group(1) == b.group(1):
            pairs.append(i)
    defs, reads = _reads_and_defs(structure, 'alive')
    if set(reads) != {i + 1 for i in pairs}:
        return lines, 0
    count = 0
    for i in pairs:
        a = re.fullmatch(r'(\s*)alive = not (\w+:IsActiveThreadTerminating\(\))\n', lines[i])
        b = re.fullmatch(r'(\s*)(\w+) = not alive\n', lines[i + 1])
        lines[i] = ''
        lines[i + 1] = f'{b.group(1)}{b.group(2)} = {a.group(2)}\n'
        count += 1
    for i, line in enumerate(lines):
        if re.fullmatch(r'\s*(?:local )?alive = (?:true|not \w+:IsActiveThreadTerminating\(\))\n', line):
            lines[i] = ''
            count += 1
        m = re.fullmatch(r'(\s*)alive = (\w+:NewScriptFrame\([^\n]*\))\n', line)
        if m:
            lines[i] = f'{m.group(1)}{m.group(2)}\n'
            count += 1
    return lines, count


def inline_exit_checks(lines):
    """Three-line `if T() then / return / end` -> one line (termination and frame checks only)."""
    count = 0
    for i in range(len(lines) - 2):
        m = re.fullmatch(r'(\s*)if ([^\n]*(?:IsActiveThreadTerminating|NewScriptFrame)[^\n]*) then\n', lines[i])
        if not m:
            continue
        body = re.fullmatch(re.escape(m.group(1)) + r'    (' + EXIT + r')\n', lines[i + 1])
        if body and lines[i + 2] == m.group(1) + 'end\n':
            lines[i] = f'{m.group(1)}if {m.group(2)} then {body.group(1)} end\n'
            lines[i + 1] = lines[i + 2] = ''
            count += 1
            continue
        # `if T() then / __cleanup_X() / return / end`
        cleanup = re.fullmatch(re.escape(m.group(1)) + r'    (\w+\(\))\n', lines[i + 1])
        body = re.fullmatch(re.escape(m.group(1)) + r'    (' + EXIT + r')\n', lines[i + 2]) if cleanup and i + 3 < len(lines) else None
        if body and lines[i + 3] == m.group(1) + 'end\n':
            lines[i] = f'{m.group(1)}if {m.group(2)} then {cleanup.group(1)}; {body.group(1)} end\n'
            lines[i + 1] = lines[i + 2] = lines[i + 3] = ''
            count += 1
    return lines, count


def propagate_literals(lines):
    """A read reached by exactly one definition, and that definition a literal: substitute it."""
    count = 0
    flow = Flow(lines)
    if not flow.ok:
        return lines, 0
    structure = flow.structure
    literal = re.compile(r'(\s*)(\w+) = (true|false|nil|-?\d+(?:\.\d+)?)\n')
    for name in _declared_temporaries(structure):
        if name in flow.pinned:
            continue
        defs, reads = _reads_and_defs(structure, name)
        if not defs or not reads or not any(literal.fullmatch(lines[d]) for d in defs):
            continue
        def_set = set(defs)
        # reaching definitions of `name` at each line's entry ({-1} = undefined on entry)
        reach_in = {i: set() for i in flow.graph}
        reach_in[0] = {-1}
        changed = True
        while changed:
            changed = False
            for i in sorted(flow.graph):
                value = {-1} if i == 0 else set()
                for p in flow.preds.get(i, ()):
                    value |= {p} if p in def_set else reach_in[p]
                if value != reach_in[i]:
                    reach_in[i] = value
                    changed = True
        for r in reads:
            if r in def_set or len(reach_in.get(r, ())) != 1:
                continue
            d = next(iter(reach_in[r]))
            m = literal.fullmatch(lines[d]) if d >= 0 else None
            if not m or re.match(r'\s*local\b', lines[r]):
                continue
            lines[r] = rename_identifiers(lines[r], {name: m.group(3)})
            count += 1
    return lines, count


def fold_constant_conditions(lines):
    """`not true` / `not false`, `if true then BODY end` -> BODY, `if false then ... end` -> nothing."""
    count = 0
    for i, line in enumerate(lines):
        new = re.sub(r'\bnot true\b', 'false', re.sub(r'\bnot false\b', 'true', line))
        if new != line:
            lines[i] = new
            count += 1
    changed = True
    while changed:
        changed = False
        lines = [l for l in lines if l != '']
        for i, line in enumerate(lines):
            m = re.fullmatch(r'(\s*)if (true|false) then(?: [^\n]* end)?\n', line)
            if not m:
                continue
            indent, value = m.group(1), m.group(2)
            if line.rstrip().endswith(' end'):                     # inline form
                if value == 'false':
                    lines[i] = ''
                else:
                    lines[i] = indent + line.rstrip()[len(indent) + len('if true then '):-len(' end')] + '\n'
                count += 1
                changed = True
                break
            end = next((j for j in range(i + 1, len(lines)) if lines[j] == indent + 'end\n' or lines[j] == indent + 'else\n'
                        or lines[j].startswith(indent + 'elseif ')), None)
            if end is None or lines[end] != indent + 'end\n':
                continue
            if value == 'false':
                if any(re.match(r'\s*::\w+::', lines[j]) for j in range(i + 1, end)):
                    continue                                        # a goto target lives inside
                for j in range(i, end + 1):
                    lines[j] = ''
            else:
                lines[i] = lines[end] = ''
                for j in range(i + 1, end):
                    if lines[j].startswith(indent + '    '):
                        lines[j] = lines[j][4:]
            count += 1
            changed = True
            break
    return lines, count


def fold_frame_checks(lines):
    """`R:NewScriptFrame(..)` then `if R:IsActiveThreadTerminating() then X end`
    -> `if not R:NewScriptFrame(..) then X end`."""
    count = 0
    for i in range(len(lines) - 1):
        a = re.fullmatch(r'(\s*)(\w+):NewScriptFrame\(([^\n]*)\)\n', lines[i])
        if not a:
            continue
        b = re.fullmatch(re.escape(a.group(1)) + r'if ' + a.group(2) + r':IsActiveThreadTerminating\(\) then (' + EXIT + r') end\n', lines[i + 1])
        if b:
            lines[i] = ''
            lines[i + 1] = f'{a.group(1)}if not {a.group(2)}:NewScriptFrame({a.group(3)}) then {b.group(1)} end\n'
            count += 1
    return lines, count


def fold_guard_wrappers(lines):
    """`if not T() then BODY end` whose end only leads to the function exit
    -> `if T() then return end` + BODY."""
    count = 0
    while True:
        lines = [l for l in lines if l != '']
        flow = Flow(lines)
        if not flow.ok:
            return lines, count
        done = False
        for i, line in enumerate(lines):
            m = re.fullmatch(r'(\s*)if not (\w+:IsActiveThreadTerminating\(\)) then\n', line)
            if not m:
                continue
            indent = m.group(1)
            end = next((j for j in range(i + 1, len(lines)) if lines[j] == indent + 'end\n'
                        or lines[j] in (indent + 'else\n',) or lines[j].startswith(indent + 'elseif ')), None)
            if end is None or lines[end] != indent + 'end\n' or not flow.only_ends_after(end):
                continue
            lines[i] = f'{indent}if {m.group(2)} then return end\n'
            for j in range(i + 1, end):
                if lines[j].startswith(indent + '    '):
                    lines[j] = lines[j][4:]
                elif lines[j].strip():
                    raise ValueError('guard body indentation')
            lines[end] = ''
            count += 1
            done = True
            break
        if not done:
            return lines, count


def _read_reachable(flow, name, start_lines, defs):
    """A read of `name` reachable from start_lines without crossing a definition of it."""
    seen, queue = set(), list(start_lines)
    while queue:
        j = queue.pop()
        if j in seen:
            continue
        seen.add(j)
        line = flow.structure[j]
        if j in defs:
            rhs = re.sub(r'^\s*(?:local )?' + re.escape(name) + r'\s*=(?!=)', '', line, count=1)
            if rename_identifiers(rhs, {name: '__reach_marker__'}) != rhs:
                return True                       # `v = v or E` reads the value before redefining it
            continue
        if rename_identifiers(line, {name: '__reach_marker__'}) != line:
            return True
        queue.extend(flow.graph.get(j, ()))
    return False


QUERY_METHOD = re.compile(r'^(?:Get|Is|Has|Can|Msg)')


def _sink_target(flow, name, d, expr, reads, def_set):
    """The read line a definition may be moved to: the next statement, or a later one when every
    statement in between is straight-line, neither reads nor writes `name`, does not write an
    identifier the value depends on, and (for a value computed by a call) only queries state."""
    expr_ids = {t[0] for t in tokens(expr) if t.lastgroup == 'identifier'}
    expr_calls = bool(_calls(expr))
    prev, s = d, None
    for _ in range(12):
        succ = flow.graph.get(prev)
        if not succ or len(succ) != 1:
            return None
        s = next(iter(succ))
        if s == d or flow.preds.get(s) != {prev}:
            return None
        if s in reads:
            return s
        line = flow.structure[s]
        stripped = line.strip()
        if re.fullmatch(r'local [\w, ]+', stripped):      # a declaration-only line is not a statement
            prev = s
            continue
        if not stripped or s in def_set or re.match(r'\s*local\b', line) or re.match(r'(?:end|else|elseif |until |repeat|while |if |::|goto |return|break)', stripped):
            return None
        written = re.match(r'\s*(\w+)\s*=(?!=)', line)
        if written and written.group(1) in expr_ids:
            return None
        if expr_calls and any(kind != ':' or not QUERY_METHOD.match(n) for kind, _, n in _calls(flow.lines[s])):
            return None
        prev = s
    return None


def inline_single_use(lines):
    """Substitute a temporary at the read that immediately follows its assignment when no other read
    of that assignment's value can be reached (single-use per definition)."""
    count = 0
    while True:
        lines = [l for l in lines if l != '']
        flow = Flow(lines)
        if not flow.ok:
            return lines, count
        structure = flow.structure
        done = False
        for name in _declared_temporaries(structure):
            if name in flow.pinned:
                continue
            defs, reads = _reads_and_defs(structure, name)
            if not defs or not reads:
                continue
            def_set = set(defs)
            for d in defs:
                m = re.fullmatch(r'(\s*)' + re.escape(name) + r' = ([^\n]+?)\s*\n', lines[d])
                if not m or any(t.lastgroup in ('comment', 'longcomment') for t in tokens(lines[d])):
                    continue
                expr = m.group(2)
                r = _sink_target(flow, name, d, expr, reads, def_set)
                if r is None:
                    continue
                read_line = lines[r]
                if re.match(r'\s*local\b', read_line):
                    continue
                r_is_def = r in def_set
                marker = '__inline_marker__'
                rhs = re.sub(r'^(\s*)' + re.escape(name) + r'\s*=(?!=)', r'\1__lhs__ =', read_line, count=1) if r_is_def else read_line
                marked = rename_identifiers(rhs, {name: marker})
                if marked.count(marker) != 1:
                    continue
                if not r_is_def and _read_reachable(flow, name, flow.graph.get(r, ()), def_set):
                    continue
                prefix = marked.split(marker, 1)[0]
                if not STATE_READ.match(expr) and not re.fullmatch(r'[\w."]+', expr):
                    # evaluation order: calls that complete before the moved expression is evaluated
                    # may only be reordered with it when both sides are queries
                    completed, stack = [], []
                    stream = [t for t in tokens(prefix) if t.lastgroup not in ('space', 'comment', 'longcomment')]
                    for k, t in enumerate(stream):
                        if t[0] == '(':
                            callee = k > 0 and ((stream[k - 1].lastgroup == 'identifier' and stream[k - 1][0] not in KEYWORDS)
                                                or stream[k - 1].lastgroup == 'string' or stream[k - 1][0] in (')', ']'))
                            stack.append(k - 1 if callee else None)
                        elif t[0] == ')':
                            if stack and (c := stack.pop()) is not None:
                                completed.append((stream[c - 1][0] if c > 0 else '', stream[c][0]))
                    if completed:
                        queries = all(sep == ':' and QUERY_METHOD.match(n) for sep, n in completed)
                        expr_query = all(kind == ':' and (recv in PURE_RECEIVER or QUERY_METHOD.match(n)) for kind, recv, n in _calls(expr))
                        if not (queries and expr_query):
                            continue
                replacement = expr if _atomic(expr) else f'({expr})'
                lines[r] = rename_identifiers(marked, {marker: replacement}).replace('__lhs__ =', name + ' =', 1)
                lines[d] = ''
                if len(defs) == 1 and len(reads) == 1:
                    _remove_declaration(lines, name)
                count += 1
                done = True
                break
            if done:
                break
        if not done:
            return lines, count


def prune_dead_defs(lines):
    """A temporary's assignment whose value no read can reach: gone when the value is a literal or a
    state query, kept as the bare call when it has effects (`quest:NewScriptFrame(me)`)."""
    count = 0
    flow = Flow(lines)
    if not flow.ok:
        return lines, 0
    structure = flow.structure
    for name in _declared_temporaries(structure):
        if name in flow.pinned:
            continue
        defs, reads = _reads_and_defs(structure, name)
        def_set = set(defs)
        self_only = bool(reads) and all(r in def_set for r in reads)   # `v = v or E`: the value never leaves v
        for d in defs:
            m = re.fullmatch(r'(\s*)' + re.escape(name) + r' = ([^\n]+?)\s*\n', lines[d])
            if not m or any(t.lastgroup in ('comment', 'longcomment') for t in tokens(lines[d])):
                continue
            if not self_only and (d in reads or _read_reachable(flow, name, flow.graph.get(d, ()), def_set)):
                continue
            calls = _calls(m.group(2))
            if not calls or all(kind == ':' and (recv in PURE_RECEIVER or QUERY_METHOD.match(n)) for kind, recv, n in calls):
                lines[d] = ''
            elif self_only:
                continue                              # `v = v and f()`: keep the statement as is
            else:
                lines[d] = f'{m.group(1)}{m.group(2)}\n'
            count += 1
    return lines, count


def prune_unused_closures(lines):
    """A hoisted cleanup closure nothing calls any more."""
    count = 0
    structure = _structure(lines)
    for a, b in _closure_ranges(structure) or []:
        name = re.match(r'\s*local function (\w+)\(', structure[a]).group(1)
        if not any(re.search(r'\b' + name + r'\(', structure[i]) for i in range(len(structure)) if not a <= i <= b):
            for i in range(a, b + 1):
                lines[i] = ''
            count += 1
    return lines, count


def prune_empty_else(lines):
    count = 0
    for i in range(len(lines) - 1):
        m = re.fullmatch(r'(\s*)else\n', lines[i])
        if m and lines[i + 1] == m.group(1) + 'end\n':
            lines[i] = ''
            count += 1
    return lines, count


def fold_retry_loops(lines):
    """`v = E; while not v do BODY; v = E end` -> `while not E do BODY end`,
    `if E then repeat BODY until not E end` -> `while E do BODY end`, and the rotated form
    `v = E; repeat; if v then X end; BODY; v = E; until false` -> `repeat; if E then X end; BODY; until false`."""
    count = 0
    changed = True
    while changed:
        changed = False
        lines = [l for l in lines if l != '']
        structure = _structure(lines)
        for i in range(len(lines) - 4):
            m = re.fullmatch(r'(\s*)(\w+) = ([^\n]+)\n', lines[i])
            if m and lines[i + 1] == m.group(1) + 'repeat\n':
                indent, name, expr = m.group(1), m.group(2), m.group(3)
                head = re.fullmatch(re.escape(indent) + r'    if (not )?' + name + r' then(?: (' + EXIT + r') end)?\n', lines[i + 2])
                if not head:
                    continue
                if head.group(2):
                    head_end = i + 2
                else:
                    body = re.fullmatch(re.escape(indent) + r'        (' + EXIT + r')\n', lines[i + 3])
                    if not body or lines[i + 4] != indent + '    end\n':
                        continue
                    head_end = i + 4
                until = next((j for j in range(head_end + 1, len(lines)) if lines[j] == indent + 'until false\n'), None)
                if until is None:
                    continue
                last = next((j for j in range(until - 1, head_end, -1) if lines[j].strip()), None)
                if last is None or lines[last] != f'{indent}    {name} = {expr}\n':
                    continue
                defs, reads = _reads_and_defs(structure, name)
                if set(defs) != {i, last} or reads != [i + 2]:
                    continue
                cond = expr if _atomic(expr) or not head.group(1) else f'({expr})'
                lines[i + 2] = re.sub(r'^(\s*if (?:not )?)' + name + r'\b', lambda mm: mm.group(1) + cond, lines[i + 2], count=1)
                lines[i] = lines[last] = ''
                _remove_declaration(lines, name)
                count += 1
                changed = True
                break
        if changed:
            continue
        for i in range(len(lines) - 2):
            m = re.fullmatch(r'(\s*)(\w+) = ([^\n]+)\n', lines[i])
            w = re.fullmatch(r'(\s*)while (not )?(\w+) do\n', lines[i + 1]) if m else None
            if m and w and m.group(1) == w.group(1) and m.group(2) == w.group(3):
                indent, name, expr = m.group(1), m.group(2), m.group(3)
                end = next((j for j in range(i + 2, len(lines)) if lines[j] == indent + 'end\n'), None)
                if end is None:
                    continue
                last = next((j for j in range(end - 1, i + 1, -1) if lines[j].strip()), None)
                if last is None or lines[last] != f'{indent}    {name} = {expr}\n':
                    continue
                defs, reads = _reads_and_defs(structure, name)
                if set(defs) != {i, last} or reads != [i + 1]:
                    continue
                cond = expr if _atomic(expr) or not w.group(2) else f'({expr})'
                lines[i + 1] = f'{indent}while {w.group(2) or ""}{cond} do\n'
                lines[i] = lines[last] = ''
                _remove_declaration(lines, name)
                count += 1
                changed = True
                break
            m = re.fullmatch(r'(\s*)if ([^\n]+) then\n', lines[i])
            if m and lines[i + 1] == m.group(1) + '    repeat\n':
                indent, expr = m.group(1), m.group(2)
                until = next((j for j in range(i + 2, len(lines)) if lines[j].startswith(indent + '    until ')), None)
                if until is None or until + 1 >= len(lines) or lines[until + 1] != indent + 'end\n':
                    continue
                u = re.fullmatch(re.escape(indent) + r'    until not (.+)\n', lines[until])
                if not u:
                    continue
                inner = u.group(1)
                if _wrapped(inner):
                    inner = inner[1:-1]
                if inner != (expr[1:-1] if _wrapped(expr) else expr):
                    continue
                lines[i] = f'{indent}while {inner} do\n'
                lines[i + 1] = ''
                for j in range(i + 2, until):
                    if lines[j].startswith(indent + '        '):
                        lines[j] = lines[j][4:]
                lines[until] = ''
                lines[until + 1] = indent + 'end\n'
                count += 1
                changed = True
                break
    return lines, count


def prune_dead_termination_checks(lines, pure_functions):
    """A `if T() then X end` reached only through a check (or frame check) with no possible
    scheduler advance in between can never fire."""
    flow = Flow(lines)
    if not flow.ok:
        return lines, 0
    structure = flow.structure
    check = re.compile(r'^(\s*)if ' + TERMINATING + r' then (?:\w+\(\); )?' + EXIT + r' end$')
    frame = re.compile(r'^(\s*)if not \w+:NewScriptFrame\([^\n]*\) then (?:\w+\(\); )?' + EXIT + r' end$')
    until_term = re.compile(r'^(\s*)until ' + TERMINATING + r'$')
    query = re.compile(r'^(\s*)(\w+) = ' + TERMINATING + r'$')
    n = len(lines)
    # state: True = terminating known false ("clean"), False = unknown
    state_in = {i: False for i in range(n)}
    order = sorted(flow.graph)

    def transfer(i, succ):
        s = structure[i].rstrip()
        if check.match(s) or frame.match(s):
            jump = re.search(r'goto (\w+) end$', s)
            if jump:
                label = next((k for k, l in enumerate(structure) if l.strip() == f'::{jump.group(1)}::'), None)
                return succ != label
            if s.endswith('break end'):
                return succ == i + 1 or (succ > i and all(not structure[k].strip() for k in range(i + 1, succ)))
            return True
        if until_term.match(s):
            return succ < i         # back edge: the loop continues only while not terminating
        if not _pure_line(structure[i], lines[i], pure_functions):
            return False
        return state_in[i]

    changed = True
    while changed:
        changed = False
        for i in order:
            preds = flow.preds.get(i, set())
            if i == 0 or not preds:
                value = False
            else:
                value = all(transfer(p, i) for p in preds)
            if value != state_in[i]:
                state_in[i] = value
                changed = True
    count = 0
    for i in order:
        s = structure[i].rstrip()
        if check.match(s) and state_in[i]:
            lines[i] = ''
            count += 1
        elif (q := query.match(s)) and state_in[i]:
            lines[i] = f'{q.group(1)}{q.group(2)} = false\n'
            count += 1
    return lines, count


def fold_boolean_materialisation(lines):
    count = 0
    changed = True
    while changed:
        changed = False
        lines = [l for l in lines if l != '']
        structure = _structure(lines)
        for i in range(len(lines)):
            m = re.fullmatch(r'(\s*)if ([^\n]+) then\n', lines[i])
            if not m:
                continue
            indent, cond = m.group(1), m.group(2)
            # if C then v = true else v = false end
            if i + 4 < len(lines):
                a = re.fullmatch(re.escape(indent) + r'    (\w+) = (true|false)\n', lines[i + 1])
                b = re.fullmatch(re.escape(indent) + r'    (\w+) = (true|false)\n', lines[i + 3])
                if a and b and lines[i + 2] == indent + 'else\n' and lines[i + 4] == indent + 'end\n' \
                        and a.group(1) == b.group(1) and a.group(2) != b.group(2):
                    value = cond if a.group(2) == 'true' else _negate(cond)
                    lines[i] = f'{indent}{a.group(1)} = {value}\n'
                    for j in range(i + 1, i + 5):
                        lines[j] = ''
                    count += 1
                    changed = True
                    break
            # if v then v = E end  ->  v = v and E ;  if not v then v = E end -> v = v or E
            if i + 2 < len(lines) and lines[i + 2] == indent + 'end\n':
                v = re.fullmatch(r'(not )?(\w+)', cond)
                # if v then v = false end with no other reads: dead
                if v and not v.group(1) and lines[i + 1] == f'{indent}    {v.group(2)} = false\n':
                    defs, reads = _reads_and_defs(structure, v.group(2))
                    if reads == [i]:
                        lines[i] = lines[i + 1] = lines[i + 2] = ''
                        count += 1
                        changed = True
                        break
                a = re.fullmatch(re.escape(indent) + r'    (\w+) = ([^\n]+)\n', lines[i + 1]) if v else None
                if a and a.group(1) == v.group(2) and not any(t.lastgroup == 'comment' for t in tokens(lines[i + 1])):
                    expr = a.group(2)
                    op = 'or' if v.group(1) else 'and'
                    rhs = expr if not {'and', 'or'} & set(_top_level_operators(expr)) else f'({expr})'
                    lines[i] = f'{indent}{v.group(2)} = {v.group(2)} {op} {rhs}\n'
                    lines[i + 1] = lines[i + 2] = ''
                    count += 1
                    changed = True
                    break
            # empty then-branch: if C then else BODY end
            if i + 1 < len(lines) and lines[i + 1] == indent + 'else\n':
                lines[i] = f'{indent}if {_negate(cond)} then\n'
                lines[i + 1] = ''
                count += 1
                changed = True
                break
        if changed:
            continue
        # v = A ; v = v and B  ->  v = A and B
        for i in range(len(lines) - 1):
            a = re.fullmatch(r'(\s*)(\w+) = ([^\n]+)\n', lines[i])
            b = re.fullmatch(r'(\s*)(\w+) = (\w+) (and|or) ([^\n]+)\n', lines[i + 1]) if a else None
            if a and b and a.group(1) == b.group(1) and a.group(2) == b.group(2) == b.group(3) \
                    and not any(t.lastgroup == 'comment' for t in tokens(lines[i] + lines[i + 1])):
                lhs = a.group(3)
                if b.group(4) == 'and' and 'or' in _top_level_operators(lhs):
                    lhs = f'({lhs})'
                lines[i] = ''
                lines[i + 1] = f'{a.group(1)}{a.group(2)} = {lhs} {b.group(4)} {b.group(5)}\n'
                count += 1
                changed = True
                break
    return lines, count


def strip_boolean_compares(lines):
    """`v ~= false` -> `v`, `v == false` -> `not v` when every definition of v is a boolean."""
    structure = _structure(lines)
    count = 0
    text = ''.join(lines)
    for name in sorted(set(re.findall(r'\b(\w+) (?:~=|==) false\b', text))):
        defs, _ = _reads_and_defs(structure, name)
        if not defs:
            continue
        boolean = True
        for d in defs:
            m = re.fullmatch(r'\s*' + re.escape(name) + r' = ([^\n]+)\n', lines[d])
            expr = m.group(1).strip() if m else ''
            if not (expr in ('true', 'false') or expr.startswith('not ') or re.match(r'[\w.:]+:(?:Is|Has|GetStateBool)\w*\(', expr)
                    or {'==', '~=', '<', '>', '<=', '>='} & set(_top_level_operators(expr))):
                boolean = False
        if not boolean:
            continue
        for i, line in enumerate(lines):
            new = re.sub(r'\(' + re.escape(name) + r' ~= false\)', name, line)
            new = re.sub(r'\b' + re.escape(name) + r' ~= false\b', name, new)
            new = re.sub(r'\b' + re.escape(name) + r' == false\b', f'not {name}', new)
            if new != line:
                lines[i] = new
                count += 1
    return lines, count


def strip_redundant_parens(lines):
    """Drop parentheses around a whole call argument, a whole condition / right-hand side, an
    atomic operand, or an and/or operand that holds only comparisons."""
    COMPARE = {'==', '~=', '<', '>', '<=', '>=', 'not'}
    count = 0
    for i, line in enumerate(lines):
        while True:                # a stripped outer group may expose a redundant inner one
            line = lines[i]
            if '(' not in line or line.lstrip().startswith('--'):
                break
            k = _strip_parens_once(lines, i, COMPARE)
            if not k:
                break
            count += k
    return lines, count


def _strip_parens_once(lines, i, COMPARE):
    line = lines[i]
    stream = list(tokens(line))
    sig = [k for k, t in enumerate(stream) if t.lastgroup not in ('space', 'comment', 'longcomment')]
    stack, pairs = [], {}          # open sig-index -> (close sig-index, is_call)
    for n, k in enumerate(sig):
        v = stream[k][0]
        if v == '(':
            prev = stream[sig[n - 1]] if n else None
            is_call = prev is not None and ((prev.lastgroup == 'identifier' and prev[0] not in KEYWORDS)
                                            or prev.lastgroup == 'string' or prev[0] in (')', ']', '}'))
            stack.append((n, is_call))
        elif v == ')' and stack:
            open_n, is_call = stack.pop()
            pairs[open_n] = (n, is_call)
    drop = set()
    for open_n, (close_n, is_call) in pairs.items():
        if is_call:
            continue
        content = ''.join(t[0] for t in stream[sig[open_n] + 1:sig[close_n]]).strip()
        if not content or 'function' in content:
            continue
        prev = stream[sig[open_n - 1]][0] if open_n else ''
        nxt = stream[sig[close_n + 1]][0] if close_n + 1 < len(sig) else ''
        ops = set(_top_level_operators(content))
        enclosing = max((o for o, (c, _) in pairs.items() if o < open_n and c > close_n), default=None)
        if enclosing is not None and pairs[enclosing][1] and prev in ('(', ',') and nxt in (',', ')'):
            drop.add(open_n)                                            # a whole call argument
        elif prev in ('if', 'elseif', 'while', 'until', '=', 'return') and nxt in ('then', 'do', ''):
            drop.add(open_n)                                            # a whole condition / rhs
        elif not ops:
            drop.add(open_n)                                            # atomic operand
        elif ops <= COMPARE and prev in ('and', 'or', 'if', 'elseif', 'while', 'until', 'return', '=')                     and nxt in ('and', 'or', 'then', 'do', ''):
            drop.add(open_n)                                            # and/or operand
    if drop:
        skip = {sig[o] for o in drop} | {sig[pairs[o][0]] for o in drop}
        lines[i] = ''.join(t[0] for k, t in enumerate(stream) if k not in skip)
        return len(drop)
    return 0


def drop_stale_label_comments(lines):
    """`-- LAB_x: (native jump target)` for a label no goto reaches any more."""
    count = 0
    text = ''.join(lines)
    for i, line in enumerate(lines):
        m = re.fullmatch(r'\s*-- (LAB_\w+): \(native jump target\)\n', line)
        if m and not re.search(r'\b(?:goto ' + m.group(1) + r'|::' + m.group(1) + r'::)', text):
            lines[i] = ''
            count += 1
    return lines, count


def _balanced_group(text, start):
    """End index (exclusive) of the parenthesised group opening at text[start] == '('."""
    depth = 0
    for i in range(start, len(text)):
        depth += (text[i] == '(') - (text[i] == ')')
        if depth == 0:
            return i + 1
    return None


def simplify_conditions(lines):
    """`not (not X)` -> X, `not (a == b)` -> `a ~= b`, `((C) and 0 or 1) ~= 0` -> `not (C)`, and an
    unread `p = T()` (pure) staging store."""
    count = 0
    structure = _structure(lines)
    for i, line in enumerate(lines):
        if line.lstrip().startswith('--'):
            continue
        new = line
        # ((C) and 0 or 1) ~= 0  /  == 0   (the decompiler's int-valued comparison)
        for m in reversed(list(re.finditer(r'\(\((?P<c>[^()]*(?:\([^()]*\))*[^()]*)\) and 0 or 1\) (?P<op>~=|==) 0', new))):
            cond = m.group('c')
            repl = f'not ({cond})' if m.group('op') == '~=' else f'({cond})'
            new = new[:m.start()] + repl + new[m.end():]
        # not (not X) / not (a == b)
        pos = 0
        while (m := re.compile(r'\bnot \(').search(new, pos)):
            end = _balanced_group(new, m.end() - 1)
            if end is None:
                break
            inner = new[m.end():end - 1]
            neg = _negate(inner)
            if neg.startswith('not '):
                pos = m.end()
                continue
            new = new[:m.start()] + neg + new[end:]
            pos = m.start()
        if new != line:
            lines[i] = new
            count += 1
    # p = T() with no readers
    for i, line in enumerate(lines):
        m = re.fullmatch(r'\s*(\w+) = \w+:IsActiveThreadTerminating\(\)\n', line)
        if m:
            defs, reads = _reads_and_defs(structure, m.group(1))
            if not reads:
                lines[i] = ''
                _remove_declaration(lines, m.group(1))
                count += 1
    return lines, count


def split_initialised_locals(lines):
    """`local x = E` -> `local x` + `x = E` so every local is a temporary candidate (re-merged at the end
    by merge_first_assignments when it survives)."""
    count = 0
    out = []
    for line in lines:
        m = re.fullmatch(r'(\s*)local (\w+) = ([^\n]+)\n', line)
        if m and m.group(2) not in ('resources', 'helpers') and not m.group(3).startswith('require('):
            out.append(f'{m.group(1)}local {m.group(2)}\n')
            out.append(f'{m.group(1)}{m.group(2)} = {m.group(3)}\n')
            count += 1
        else:
            out.append(line)
    return out, count


def merge_first_assignments(lines):
    """`local x` directly followed by `x = E` -> `local x = E` (Aeon declares at first use)."""
    count = 0
    for i in range(len(lines) - 1):
        m = re.fullmatch(r'(\s*)local (\w+)\n', lines[i])
        if m and re.fullmatch(re.escape(m.group(1)) + m.group(2) + r' = [^\n]+\n', lines[i + 1]):
            lines[i] = ''
            lines[i + 1] = m.group(1) + 'local ' + lines[i + 1].lstrip()
            count += 1
    return lines, count


def cosmetics(lines, function_indent='    '):
    count = 0
    for i, line in enumerate(lines):
        new = re.sub(r' \+ -(\d+(?:\.\d+)?)\b', r' - \1', line)
        if new != line:
            lines[i] = new
            count += 1
    # local ret = x ; return ret
    for i in range(len(lines) - 1):
        a = re.fullmatch(r'(\s*)local (\w+) = ([^\n]+)\n', lines[i])
        if a and lines[i + 1] == f'{a.group(1)}return {a.group(2)}\n':
            lines[i] = ''
            lines[i + 1] = f'{a.group(1)}return {a.group(3)}\n'
            count += 1
    # trailing bare return at the function's tail
    last = next((j for j in range(len(lines) - 1, -1, -1) if lines[j].strip()), None)
    if last is not None and lines[last] == 'end\n':
        prev = next((j for j in range(last - 1, -1, -1) if lines[j].strip()), None)
        if prev is not None and lines[prev] == function_indent + 'return\n':
            lines[prev] = ''
            count += 1
    return lines, count


def tidy_blank_lines(text):
    """Blank lines inside a function body are where statements were removed; keep one between functions."""
    out, depth = [], 0
    for line in text.splitlines(keepends=True):
        if re.match(r'^(?:local )?function ', line):
            depth = 1
        elif depth and line == 'end\n':
            depth = 0
            out.append(line)
            continue
        if depth and not line.strip():
            continue
        out.append(line)
    return re.sub(r'\n{3,}', '\n\n', ''.join(out))


# --------------------------------------------------------------------------------------- driver

FUNCTION_SPLIT = re.compile(r'(?m)^(?=function |local function )')


def _function_chunks(source):
    return FUNCTION_SPLIT.split(source)


def pure_local_functions(source):
    """Local functions whose bodies cannot advance the scheduler (fixpoint over the file)."""
    chunks = {re.match(r'(?:local )?function (\w+)', c).group(1): c for c in _function_chunks(source)
              if c.startswith(('function ', 'local function '))}
    pure = set()
    while True:
        before = len(pure)
        for name, chunk in chunks.items():
            if name in pure:
                continue
            lines = chunk.splitlines(keepends=True)
            structure = _structure(lines)
            if all(_pure_line(s, raw, pure) for s, raw in zip(structure[1:], lines[1:])):
                pure.add(name)
        if len(pure) == before:
            return pure


def style_function(chunk, pure_functions, *, frame_returns_alive=True):
    lines = chunk.splitlines(keepends=True)
    stats = {}

    def run(key, fn, *args):
        nonlocal lines
        lines, k = fn(lines, *args)
        stats[key] = stats.get(key, 0) + k
        lines = [l for l in lines if l != '']
        return k

    run('splitLocals', split_initialised_locals)

    def folds():
        total = 0
        total += run('staleLabelComments', drop_stale_label_comments)
        total += run('aliveStaging', normalize_alive)
        total += run('exitChecksInlined', inline_exit_checks)
        if frame_returns_alive:
            total += run('frameChecks', fold_frame_checks)
        total += run('inlinedTemporaries', inline_single_use)
        total += run('deadDefinitions', prune_dead_defs)
        total += run('guardWrappers', fold_guard_wrappers)
        total += run('retryLoops', fold_retry_loops)
        total += run('booleanMaterialisation', fold_boolean_materialisation)
        total += run('booleanCompares', strip_boolean_compares)
        total += run('conditions', simplify_conditions)
        total += run('literals', propagate_literals)
        total += run('constantConditions', fold_constant_conditions)
        total += run('emptyElse', prune_empty_else)
        total += run('unusedClosures', prune_unused_closures)
        return total

    for _ in range(3):
        for _ in range(20):
            if not folds():
                break
        if not run('deadTerminationChecks', prune_dead_termination_checks, pure_functions):
            break
    run('parentheses', strip_redundant_parens)
    run('cosmetics', cosmetics)
    run('firstAssignments', merge_first_assignments)
    return ''.join(lines), stats


def hoist_requires(source):
    """`require("Pkg.mod").Fn(` -> `mod.Fn(` with one `local mod = require("Pkg.mod")` per file."""
    modules = sorted(set(re.findall(r'require\("([\w.]+)"\)\.', source)))
    if not modules:
        return source, 0
    names = {}
    for module in modules:
        base = module.rsplit('.', 1)[-1]
        alias = 'helpers' if base == 'native_quest_helpers' else re.sub(r'_(\w)', lambda m: m.group(1).upper(), base)
        while alias in names.values() or re.search(r'\b' + alias + r'\b', source):
            alias += '_'
        names[module] = alias
    count = 0
    for module, alias in names.items():
        source, k = re.subn(r'require\("' + re.escape(module) + r'"\)\.', alias + '.', source)
        count += k
    header_end = re.search(r'^(?!--)', source, re.M).start()
    decls = ''.join(f'local {alias} = require("{module}")\n' for module, alias in names.items())
    source = source[:header_end] + '\n' + decls + source[header_end:] if source[:header_end].endswith('\n') else decls + source
    return source, count


def style_metrics(chunk):
    lines = [l for l in chunk.splitlines() if l.strip()]
    body = '\n'.join(lines[1:])
    temps = 0
    for m in re.finditer(r'^\s*local ([\w, ]+)$', body, re.M):
        temps += len(m.group(1).split(', '))
    return {'lines': len(lines), 'temporaries': temps, 'labels': len(re.findall(r'^\s*::\w+::', body, re.M)),
            'gotos': len(re.findall(r'\bgoto \w+', body)),
            'terminationChecks': body.count('IsActiveThreadTerminating'),
            'frameChecks': len(re.findall(r'if not \w+:NewScriptFrame', body)),
            'requires': body.count('require(')}


def style_source(source, *, frame_returns_alive=True):
    source, hoisted = hoist_requires(source)
    pure = pure_local_functions(source)
    out, report = [], {}
    for part in _function_chunks(source):
        if not part.startswith(('function ', 'local function ')):
            out.append(part)
            continue
        name = re.match(r'(?:local )?function (\w+)', part).group(1)
        before = style_metrics(part)
        styled, stats = style_function(part, pure, frame_returns_alive=frame_returns_alive)
        report[name] = {'rewrites': stats, 'before': before, 'after': style_metrics(styled)}
        out.append(styled)
    text = tidy_blank_lines(''.join(out))
    return text, {'functions': report, 'hoistedRequires': hoisted, 'pureLocalFunctions': sorted(pure)}
