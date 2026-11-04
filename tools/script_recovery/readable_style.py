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
    the read in that statement, and the read is not conditionally executed)
  * boolean materialisation: `if C then v = true else v = false end` -> `v = C`,
    `if v then v = E end` -> `v = v and E`, empty then-branches inverted
  * cosmetics (step 6): redundant parentheses, `x + -1`, `v ~= false`, blank lines, one
    `local helpers = require(...)` per file, stale `-- LAB_x` comments, `local ret = x; return ret`
"""
from __future__ import annotations

import functools
import re
from collections import Counter

from tools.script_recovery.lua_local_versions import flow_graph, random_result_locals, compound_scalar_stores, nil_clear_targets
from tools.script_recovery.readable_lua import rename_identifiers, tokens

TERMINATING = r'(?P<recv>\w+):IsActiveThreadTerminating\(\)'
EXIT = r'(?:return|goto \w+|break)'
# calls that cannot advance the scheduler (terminating can only flip across a frame / blocking call)
PURE_METHOD = re.compile(r'^(?:Get|Is|Has|Can|Msg|SetState|GetState|SetMasterGameState|GetMasterGameState|NewResource|TryAcquire|ReleaseResource|RetailResources|ReadGlobalGameData)')
PURE_RECEIVER = {'__native_entity_state'}
STATE_READ = re.compile(r'^(?:\w+:GetState(?:Bool|Int|Float|String|Thing)\(|__native_entity_state:GetState\w+\(|\w+:GetStateList(?:Count|At)\()')


# ----------------------------------------------------------------------------------------- helpers

@functools.lru_cache(maxsize=None)
def _structure_line(line):
    """One line with comments and strings blanked (the emitter never spans strings across lines)."""
    return ''.join(re.sub(r'[^\n]', ' ', t[0]) if t.lastgroup in ('comment', 'longcomment', 'string', 'longstring')
                   else t[0] for t in tokens(line)).rstrip('\n')


def _structure(lines):
    """Lines with comments and strings blanked (same shape, safe for regexes on syntax)."""
    return [_structure_line(l) for l in lines]


@functools.lru_cache(maxsize=None)
def _line_reads(structure_line):
    """Identifiers a (blanked) line reads as variables: not member names after `.`/`:`, not the
    assignment target of `x = ...` / `local x = ...`."""
    stream = [t for t in tokens(structure_line) if t.lastgroup not in ('space', 'comment', 'longcomment')]
    reads, target = set(), None
    m = re.match(r'\s*(?:local )?(\w+)\s*=(?!=)', structure_line)
    if m and not re.fullmatch(r'\s*local [\w, ]+\s*', structure_line):
        target = m.group(1)
    seen_eq = False
    for k, t in enumerate(stream):
        if t[0] == '=':
            seen_eq = True
        if t.lastgroup != 'identifier' or t[0] in KEYWORDS_ALL:
            continue
        if k and stream[k - 1][0] in ('.', ':'):
            continue
        if target and not seen_eq and t[0] == target and k <= 1:
            continue
        reads.add(t[0])
    return frozenset(reads)


def _line_target(structure_line):
    if re.fullmatch(r'\s*local [\w, ]+\s*', structure_line):
        return None
    m = re.match(r'\s*(?:local )?(\w+)\s*=(?!=)', structure_line)
    return m.group(1) if m else None


KEYWORDS_ALL = {'and', 'or', 'not', 'if', 'elseif', 'while', 'until', 'return', 'then', 'do', 'in', 'end', 'else',
                'repeat', 'local', 'function', 'goto', 'break', 'true', 'false', 'nil', 'for'}


def _closure_ranges(structure):
    """Top-level `local function` blocks (cleanup helpers): opaque to the flow graph."""
    ranges, i = [], 1
    while i < len(structure):
        m = re.match(r'^(\s*)local function \w+\(', structure[i])
        if m:
            # The range finder models emitted multiline declarations only.
            # An inline body must not consume a later unrelated same-indent end.
            if not re.fullmatch(r'\s*local function \w+\([\w, .]*\)\s*', structure[i]):
                return None
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
        self.inert_nil_initializations = set()
        ranges = _closure_ranges(self.structure)
        if ranges is None:
            return
        # Keep random-result destinations and their explicit releases together.
        self.pinned = random_result_locals(self.structure)
        # Compound destinations block identifier substitution. They need not
        # block rewrites that retain stores or remove an entire dead statement.
        self.opaque_pinned = self.pinned | {'_ENV'}
        self.pinned.update(compound_scalar_stores(self.structure))
        self.pinned.add('_ENV')  # bare global accesses implicitly read this binding
        blanked = list(self.structure)
        for a, b in ranges:
            shadowed = set()          # a closure-local (`local x = ...`) is not the enclosing function's x
            for i in range(a, b + 1):
                local = re.match(r'\s*local (\w+) = ', self.structure[i])
                names = {t[0] for t in tokens(lines[i]) if t.lastgroup == 'identifier'}
                if local:
                    names.discard(local.group(1))
                    shadowed.add(local.group(1))
                self.pinned.update(names - shadowed)
                self.opaque_pinned.update(names - shadowed)
                blanked[i] = ''
        # `__cleanup_X(); return` (hoisted cleanup regions) is straight-line code before the exit
        for i, line in enumerate(blanked):
            blanked[i] = re.sub(r'^(\s*(?:if .+ then )?)[^\n;]+; (return|goto \w+|break)((?: end)?)$', r'\1\2\3', line)
        self.graph = flow_graph(blanked)
        if self.graph is None:
            return
        self.preds = {i: set() for i in self.graph}
        for i, targets in self.graph.items():
            for t in targets:
                self.preds.setdefault(t, set()).add(i)
        # A lone nil store to one uninitialized, uncaptured hoisted local has
        # no previous value to retire. Keep ordinary literal staging available;
        # every ambiguous, reused, captured or compound destination stays pinned.
        declarations = Counter(_declared_temporaries(self.structure))
        definitions = Counter(_line_target(line) for line in self.structure)
        clear_counts = Counter(name for line in self.structure for name in nil_clear_targets(line))
        other_compound = compound_scalar_stores(self.structure, include_nil_clears=False)
        for i, line in enumerate(self.structure):
            targets = nil_clear_targets(line)
            if len(targets) != 1:
                continue
            name = next(iter(targets))
            if (declarations[name] == definitions[name] == clear_counts[name] == 1
                    and name not in self.opaque_pinned and name not in other_compound
                    and not re.search(r'\b' + re.escape(name) + r'\b', self.structure[0])):
                self.inert_nil_initializations.add(i)
                self.pinned.discard(name)
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
    defs, reads = [], []
    for i, line in enumerate(structure):
        if name not in line:
            continue
        if re.fullmatch(r'\s*local [\w, ]+\s*', line):
            continue
        if _line_target(line) == name:
            defs.append(i)
        if name in _line_reads(line):
            reads.append(i)
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
        elif depth == 0 and (v in ('and', 'or', 'not', '..', '+', '-', '*', '/', '//', '%', '^',
                                   '&', '|', '~', '<<', '>>', '#', '==', '~=', '<', '>', '<=', '>=')):
            ops.append(v)
    return ops


def _atomic(expr):
    expr = expr.strip()
    if _wrapped(expr):
        return True
    return not _top_level_operators(expr)


def _can_expand_results(expr):
    """Conservative call/vararg recognition, including Lua's call sugar."""
    if _wrapped(expr.strip()) or not _atomic(expr):
        return False
    stream = [t for t in tokens(expr) if t.lastgroup not in ('space', 'comment', 'longcomment')]
    if not stream:
        return False
    last = stream[-1]
    return (last[0] in ('...', ')')
            or len(stream) > 1 and last.lastgroup in ('string', 'longstring')
            or last[0] == '}' and stream[0][0] != '{')


def _result_list_tail(previous, following, after_following=''):
    return (previous in ('(', ',', '{', 'return', '=', 'in')
            and (following in (')', '}', '', 'end', ';', 'do')
                 or following == ',' and after_following == '}'))


def _has_expression_list(expr):
    depth = 0
    for token in tokens(expr):
        if token.lastgroup in ('space', 'comment', 'longcomment', 'string', 'longstring'):
            continue
        if token[0] in ('(', '[', '{'):
            depth += 1
        elif token[0] in (')', ']', '}'):
            depth -= 1
        elif token[0] == ',' and depth == 0:
            return True
    return False


def _reference_can_expand(source, name):
    stream = [t for t in tokens(source) if t.lastgroup not in ('space', 'comment', 'longcomment')]
    for i, token in enumerate(stream):
        if token.lastgroup == 'identifier' and token[0] == name:
            if i and stream[i - 1][0] == '=' and not _has_expression_list(''.join(t[0] for t in stream[:i - 1])):
                return False  # one assignment target always consumes exactly one value
            return _result_list_tail(stream[i - 1][0] if i else '',
                                     stream[i + 1][0] if i + 1 < len(stream) else '',
                                     stream[i + 2][0] if i + 2 < len(stream) else '')
    return False


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


def _calls(line, *, completed_only=False):
    """Calls in Lua code, ignoring comments/literal contents and spacing.

    Dynamic callees are unknown effectful calls. Literal/table argument sugar
    is still a call even though it has no opening parenthesis.
    """
    stream = [t for t in tokens(line) if t.lastgroup not in ('space', 'comment', 'longcomment')]
    calls = []
    for i, token in enumerate(stream):
        if not i or not (token[0] in ('(', '{') or token.lastgroup in ('string', 'longstring')):
            continue
        if completed_only and token[0] in ('(', '{'):
            closing = {'(': ')', '[': ']', '{': '}'}
            stack = [closing[token[0]]]
            for following in stream[i + 1:]:
                value = following[0]
                if value in closing:
                    stack.append(closing[value])
                elif value in (')', ']', '}'):
                    if not stack or value != stack.pop():
                        break
                    if not stack:
                        break
            if stack:
                continue
        previous = stream[i - 1]
        if previous.lastgroup == 'identifier' and previous[0] not in KEYWORDS_ALL:
            name, kind, receiver = previous[0], '', None
            start = i - 1
            if i >= 2 and stream[i - 2][0] in (':', '.'):
                kind = stream[i - 2][0]
                start = i - 3
                if start >= 0 and stream[start].lastgroup == 'identifier':
                    end = start
                    while start >= 2 and stream[start - 1][0] == '.' and stream[start - 2].lastgroup == 'identifier':
                        start -= 2
                    receiver = ''.join(t[0] for t in stream[start:end + 1])
            if start >= 1 and stream[start - 1][0] == 'function':
                continue
            calls.append((kind, receiver, name))
        elif previous[0] in (')', ']', '}') or previous.lastgroup in ('string', 'longstring'):
            calls.append(('', None, '<dynamic>'))
    return calls



def _environment_evaluation_is_inert(flow, raw_line, definition):
    """Unknown variable reads/stores may dispatch through an _ENV metatable."""
    structure = _structure_line(raw_line)
    names = set(_line_reads(structure))
    target = _line_target(structure)
    if target and not structure.lstrip().startswith('local '):
        names.add(target)
    if '_ENV' in names:
        return False
    return not names or (flow is not None and flow.ok and
        _prefix_bindings_are_stable(flow, 'return ' + ', '.join(sorted(names)), definition))


def _pure_line(structure_line, raw_line, pure_functions, flow=None, definition=0):
    s = structure_line.strip()
    if not s or re.fullmatch(r'(?:end|else|repeat|do|break|return|goto \w+|::\w+::|local [\w, ]+)', s):
        return True
    if _prefix_has_implicit_evaluation(raw_line):
        return False  # indexing/operators can run metamethods and change scheduler state
    if not _environment_evaluation_is_inert(flow, raw_line, definition):
        return False
    for kind, recv, name in _calls(raw_line):
        if kind == ':' and name == 'IsActiveThreadTerminating':
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
    # Relational complements differ for NaN (and may invoke different Lua
    # metamethods). Preserve logical negation unless equality is involved.
    flips = {'==': '~=', '~=': '=='}
    if len(ops) == 1 and ops[0] in flips:
        depth = 0
        for token in tokens(cond):
            if token.lastgroup in ('string', 'longstring', 'comment', 'longcomment', 'space'):
                continue
            value = token[0]
            if value in ('(', '[', '{'):
                depth += 1
            elif value in (')', ']', '}'):
                depth -= 1
            elif depth == 0 and value == ops[0]:
                return cond[:token.start()] + flips[value] + cond[token.end():]
    return f'not {cond}' if _atomic(cond) else f'not ({cond})'

# ------------------------------------------------------------------------------------------ passes

def normalize_alive(lines):
    """`alive = not T()` + `p = not alive` -> `p = T()` when `alive` has no other readers; then
    remaining staging drops its unused result but preserves scheduler calls."""
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
        if re.fullmatch(r'\s*(?:local )?alive = true\n', line):
            lines[i] = ''
            count += 1
        m = re.fullmatch(r'(\s*)(?:local )?alive = not (\w+:IsActiveThreadTerminating\(\))\n', line)
        if m:
            lines[i] = f'{m.group(1)}{m.group(2)}\n'
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
        cleanup = re.fullmatch(re.escape(m.group(1)) + r'    ([^\n;]+)\n', lines[i + 1])
        if cleanup and re.match(r'(?:if |while |repeat|for |local |return|goto |break|::|--)', cleanup.group(1)):
            cleanup = None
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
            if re.search(r'(?<![\w.:])' + re.escape(name) + r'\s*[:.\[(]', _structure_line(lines[r])):
                continue                                 # a literal cannot be a prefix expression (`nil:IsAlive()`)
            value = m.group(3)
            # A negative number is a unary expression, not an atomic token:
            # bare substitution changes x^2 and can turn a preceding '-' into
            # a Lua comment opener. Preserve the original variable grouping.
            replacement = '(' + value + ')' if value.startswith('-') else value
            lines[r] = rename_identifiers(lines[r], {name: replacement})
            count += 1
    return lines, count


def propagate_copies(lines):
    """Propagate copies only from a unique, stable function-local binding."""
    count = 0
    flow = Flow(lines)
    if not flow.ok:
        return lines, 0
    structure = flow.structure
    inline_written = compound_scalar_stores(structure)
    copy = re.compile(r'(\s*)(?:local )?(\w+) = (\w+)\n')
    declared = set(_declared_temporaries(structure))
    initialised = {m.group(1) for l in structure for m in [re.match(r'\s*local (\w+) = ', l)] if m}
    bindings = {}
    for i, line in enumerate(structure):
        match = re.fullmatch(r'(\s*)local ([\w, ]+?)(?: = .+)?', line)
        if match:
            for local in match[2].split(','):
                bindings.setdefault(local.strip(), []).append((i, len(match[1])))

    def reaches(starts, target, blocked=()):
        seen, queue = set(blocked), list(starts)
        while queue:
            i = queue.pop()
            if i in seen:
                continue
            if i == target:
                return True
            seen.add(i)
            queue.extend(flow.graph.get(i, ()))
        return False

    def unique_top_binding(name):
        sites = bindings.get(name, ())
        return len(sites) == 1 and sites[0][1] == 4

    # a name a cleanup closure only reads keeps its definitions; its reads outside the closure may still be renamed
    closure_written = set()
    for a, b in _closure_ranges(structure) or []:
        for k in range(a, b + 1):
            target = _line_target(structure[k])
            if target and not re.match(r'\s*local ', structure[k]):      # (a closure-local shadow is not a write)
                closure_written.add(target)
    for name in sorted(declared | initialised):
        if name in inline_written or name in closure_written or not unique_top_binding(name):
            continue
        defs, reads = _reads_and_defs(structure, name)
        if name in initialised:
            defs = sorted(set(defs) | {i for i, l in enumerate(structure) if re.match(r'\s*local ' + re.escape(name) + r' = ', l)})
        if not defs or not reads:
            continue
        candidates = {}
        for d in defs:
            m = copy.fullmatch(lines[d])
            if not m or m.group(3) in ('true', 'false', 'nil') or m.group(3) == name:
                continue
            src = m.group(3)
            # A global/upvalue may be changed by any callee, even without a
            # visible assignment here. Shadowed or nested bindings are opaque.
            if not unique_top_binding(src) or src in flow.pinned:
                continue
            declaration = bindings[src][0][0]
            if declaration >= d or reaches([0], d, blocked={declaration}):
                continue
            src_defs = _reads_and_defs(structure, src)[0]
            if len(src_defs) > 1:
                continue
            if src_defs:
                definition = src_defs[0]
                if reaches([0], d, blocked={definition}):
                    continue  # the source is not initialized on every copy path
                if reaches(flow.graph.get(definition, ()), definition):
                    continue  # one syntactic assignment may run on later iterations
            candidates[d] = src
        if not candidates:
            continue
        def_set = set(defs)
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
            if d not in candidates or re.match(r'\s*local\b', lines[r]):
                continue
            lines[r] = rename_identifiers(lines[r], {name: candidates[d]})
            count += 1
    return lines, count


def fold_uint_fixups(lines):
    """`f = n; if n < 0 then f = f + 4294967296.0 end` is the compiler's unsigned-to-float conversion of a
    count that is never negative: the copy alone."""
    count = 0
    i = 0
    while i + 3 < len(lines):
        m = re.fullmatch(r'(\s*)(?:local )?(\w+) = (\w+)\n', lines[i])
        if m and re.fullmatch(m.group(1) + r'if ' + re.escape(m.group(3)) + r' < 0 then\n', lines[i + 1]) \
                and re.fullmatch(r'\s*' + re.escape(m.group(2)) + r' = ' + re.escape(m.group(2)) + r' \+ 4294967296(?:\.0)?\n', lines[i + 2]) \
                and re.fullmatch(m.group(1) + r'end\n', lines[i + 3]):
            del lines[i + 1:i + 4]
            count += 1
        i += 1
    return lines, count


def tidy_closures(lines):
    """A hoisted cleanup closure is straight-line code: a literal stored into an outer temporary and read
    right away (`scratchValue5 = 0; quest:Pause(scratchValue5 ~= 0)`) becomes the literal, so the closure
    stops pinning that temporary for the enclosing function."""
    count = 0
    structure = _structure(lines)
    ranges = _closure_ranges(structure) or []
    def straight_line(a, b):
        return not any(re.search(r'^\s*(?:if|elseif|else|while|for|repeat|until|do|goto|return|break|end|function|local function)\b|::',
                                 line) for line in _structure(lines[a + 1:b]))

    for a, b in reversed(ranges):
        if not straight_line(a, b):
            continue
        body = lines[a + 1:b]
        inline_written = compound_scalar_stores(_structure(body))
        literal = re.compile(r'(\s*)(\w+) = (true|false|nil|-?\d+(?:\.\d+)?)\n')
        i = 0
        while i < len(body):
            m = literal.fullmatch(body[i])
            if m and m.group(2) not in inline_written:
                name = m.group(2)
                later = ''.join(body[i + 1:])
                # the temporary is read only in this closure after the store, and never stored again here
                if not re.search(r'^\s*' + re.escape(name) + r' = ', later, re.M) and re.search(r'\b' + re.escape(name) + r'\b', later):
                    body = body[:i] + [rename_identifiers(l, {name: m.group(3)}) if not l.lstrip().startswith(name + ' =') else l for l in body[i + 1:]]
                    count += 1
                    continue
            i += 1
        lines[a + 1:b] = body
        # (line count changed: recompute for the next range)
        structure = _structure(lines)
        ranges = _closure_ranges(structure) or []
    # an outer temporary the closure assigns before reading (`scratchValue16 = quest:GetThingWithScriptName("RaceMarker");
    # quest:MiniMapRemoveMarker(scratchValue16)`) is the closure's own when no read of that value can follow a call
    flow = Flow(lines)
    if flow.ok:
        structure = flow.structure
        for a, b in _closure_ranges(structure) or []:
            if not straight_line(a, b):
                continue
            name_m = re.match(r'\s*local function (\w+)\(', structure[a])
            if not name_m:
                continue
            calls = [k for k, l in enumerate(structure) if not (a <= k <= b) and re.search(r'\b' + name_m.group(1) + r'\(\)', l)]
            exits = [s for k in calls for s in flow.graph.get(k, ())]
            seen = set()
            for k in range(a + 1, b):
                target = _line_target(structure[k])
                reads = _line_reads(structure[k])
                if target and target not in seen and target not in reads and target in flow.pinned \
                        and not re.match(r'\s*local\b', structure[k]):
                    outer_defs = {d for d in _reads_and_defs(structure, target)[0] if not (a <= d <= b)}
                    if not _read_reachable(flow, target, exits, outer_defs) and \
                            not any(target in _line_reads(structure[j]) for j in range(a + 1, k)):
                        lines[k] = re.sub(r'^(\s*)' + re.escape(target) + r' =', r'\1local ' + target + ' =', lines[k], count=1)
                        count += 1
                seen.update(reads)
                if target:
                    seen.add(target)
    # a value the enclosing function only ever passes to `scratchValue ~= 0` style tests reads as the boolean
    for i, l in enumerate(lines):
        lines[i] = re.sub(r'([(,] ?)(\d+) ~= 0(?=[,)])', lambda m: m.group(1) + ('true' if int(m.group(2)) else 'false'), l)   # a whole operand only (`x & 2 ~= 0` stays)
    return lines, count


def fold_cutscenes(lines):
    """The retail cutscene boilerplate (a control resource per actor, an actor map, a movie object, the
    pause / camera fix, the macro, then the teardown) is ForgeFSE's `quest:StartCutscene({NAME = thing, ...},
    {}, fixCamera)` / `quest:RunCutscene(name, skippable, setup)` / `quest:EndCutscene()` (the DLL does exactly
    those native calls: LuaQuestState::StartCutscene). All the cutscenes of a function fold together or not at
    all, and only when every piece is a plain statement (an entity's `while not TryAcquire` retry loop stays)."""
    text = ''.join(lines)
    maps = re.findall(r'^\s*(?:local )?(\w+) = resources:NewActorMap\(\)\n', text, re.M)
    if not maps:
        return lines, 0
    objects, pieces, replacements = set(), [], []
    for M in maps:
        actors = re.findall(r'^\s*resources:SetActor\(' + re.escape(M) + r', "(\w+)", (\w+)\)\n', text, re.M)
        if not actors:
            return lines, 0
        pieces += [re.compile(r'^\s*(?:local )?' + re.escape(M) + r' = resources:NewActorMap\(\)\n', re.M),
                   re.compile(r'^\s*resources:SetActor\(' + re.escape(M) + r', "\w+", \w+\)\n', re.M)]
        table = []
        setup = text.index(f'{M} = resources:NewActorMap()')
        for name, R in actors:
            # the handle's last acquisition before this map (a handle is re-acquired per cutscene)
            acquires = list(re.finditer(r'^\s*resources:TryAcquire\(' + re.escape(R) + r', ([^\n]+?), 4\)\n', text[:setup], re.M))
            acquire = acquires[-1] if acquires else None
            if not acquire:
                # the entity's own control handle (acquired by the retry loop at the top of Main, priority 4):
                # the actor is that thing; the handle stays (fold_control_acquires takes it once the map is gone)
                held = list(re.finditer(r'^\s*while not resources:TryAcquire\(' + re.escape(R) + r', (\w+), 4\) do\n', text[:setup], re.M))
                if not held:
                    return lines, 0
                table.append((name, held[-1].group(1)))
                continue
            table.append((name, acquire.group(1)))
            objects.add(R)
            pieces += [re.compile(r'^\s*(?:local )?' + re.escape(R) + r' = resources:NewResource\(\)\n', re.M),
                       re.compile(r'^\s*resources:TryAcquire\(' + re.escape(R) + r', [^\n]+?, 4\)\n', re.M)]
        movie = re.search(r'^\s*(?:(?:local )?(\w+) = )?resources:StartMovie\(""\)\n(\s*)quest:StartMovieSequence\(\)\n\s*quest:PauseAllNonScriptedEntities\(true\)\n(\s*quest:FixMovieSequenceCamera\(true\)\n)?',
                          text[text.index(f'{M} = resources:NewActorMap()'):], re.M)
        if not movie:
            return lines, 0
        MV, indent, fix = movie.group(1), movie.group(2), bool(movie.group(3))     # (MV None: the movie object's name was lost)
        objects |= {M} | ({MV} if MV else set())
        macros = list(re.finditer(r'^(\s*)resources:RunMacro\(("[^"]*"|\w+), ' + re.escape(M) + r', (true|false), (true|false)\)\n', text, re.M))
        if not macros:
            return lines, 0
        for m in macros:
            replacements.append((m.group(0), f'{m.group(1)}quest:RunCutscene({m.group(2)}, {m.group(4)}, {m.group(3)})\n'))
        replacements.append((movie.group(0), f'{indent}quest:StartCutscene({{' + ', '.join(f'{n} = {th}' for n, th in table) + f'}}, {{}}, {"true" if fix else "false"})\n'))
    new = text
    for old_text, repl in replacements:
        new = new.replace(old_text, repl, 1)
    for pat in pieces:
        new = pat.sub('', new)
    # teardown lines (also inside cleanup closures), in any order, become one EndCutscene per group
    names = '|'.join(re.escape(x) for x in objects)
    teardown = (r'(?:[ \t]*quest:FixMovieSequenceCamera\(false\)\n|[ \t]*quest:PauseAllNonScriptedEntities\(false\)\n|'
                r'[ \t]*resources:(?:DestroyMovie|DestroyActorMap|ReleaseResource)\((?:' + names + r')\)\n)')
    def end(m):
        # a lone camera release mid-cutscene (before a tutorial shown with the entities still paused) is not the end
        if 'PauseAllNonScriptedEntities' not in m.group(1) and 'resources:' not in m.group(1):
            return m.group(1)
        return re.match(r'[ \t]*', m.group(1)).group(0) + 'quest:EndCutscene()\n'
    new, n_end = re.subn(r'^((?:' + teardown + r')+)', end, new, flags=re.M)
    # EndCutscene is idempotent: an end that only falls through labels into another end is the same end
    new = re.sub(r'^[ \t]*quest:EndCutscene\(\)\n(?=(?:[ \t]*::\w+::\n)*[ \t]*quest:EndCutscene\(\)\n)', '', new, flags=re.M)
    n_end = new.count('quest:EndCutscene()')
    if not n_end:
        return lines, 0
    body = re.sub(r'^\s*local [\w, ]+\n', '', new, flags=re.M)
    gone = {x for x in objects if not re.search(r'\b' + re.escape(x) + r'\b', body)}

    def undeclare(line):
        m = re.fullmatch(r'(\s*)local ([\w, ]+)\n', line)
        if not m:
            return line
        kept = [n for n in m.group(2).split(', ') if n not in gone]
        return f'{m.group(1)}local {", ".join(kept)}\n' if kept else ''
    new = ''.join(undeclare(l) for l in new.splitlines(keepends=True))
    strict = objects - set(maps)          # a map's register may be reused for an unrelated value once the map is gone
    if any(re.search(r'\b' + re.escape(x) + r'\b', new) for x in strict) or \
            any(re.search(r'\b' + re.escape(M) + r'\b', l) and 'resources:' in l for M in maps for l in new.splitlines()):
        return lines, 0                               # something still reads the objects: keep the faithful form
    if 'resources:' not in new:
        new = re.sub(r'^\s*local resources = quest:RetailResources\(\)\n', '', new, flags=re.M)
    return new.splitlines(keepends=True), len(maps)


def simplify_fresh_guards(lines):
    """`x ~= nil and not x:IsNull()` on the line right after `x = quest:Get...(...)` / `CreateCreature(...)`: ForgeFSE's
    thing wrappers already return nil for a null thing (WrapScriptThingOutput), so a fresh lookup only needs the nil
    test. Things held across frames keep the IsNull (they can die meanwhile)."""
    count = 0
    for i in range(1, len(lines)):
        m = re.match(r'\s*(?:local )?(\w+) = (?:quest|resources):(?:Get\w+|Create\w+|New\w+)\(', lines[i - 1])
        if not m:
            continue
        x = re.escape(m.group(1))
        new = re.sub(x + r' ~= nil and not ' + x + r':IsNull\(\)', m.group(1) + ' ~= nil', lines[i])   # (parens stay: `not (...)`)
        new = re.sub(r'not \(' + x + r' ~= nil\)', m.group(1) + ' == nil', new)
        if new != lines[i]:
            lines[i] = new
            count += 1
    return lines, count


def hoist_hero(lines):
    """Keep each native hero lookup at its original execution point.

    GetHero returns a borrowed engine handle. Neither handle identity across
    yields nor the timing of a nullable lookup is established by readability.
    """
    return lines, 0


def _constant_condition_structure(source):
    """Blank Lua text tokens across the complete source, preserving offsets."""
    return ''.join(re.sub(r'[^\n]', ' ', t[0])
        if t.lastgroup in ('string','longstring','comment','longcomment') else t[0]
        for t in tokens(source))

def _plain_constant_else(lines, code, indent):
    """Inline a kept else only when neither text nor a lexical lifetime can move."""
    if any(raw!=masked or '::' in masked for raw,masked in zip(lines,code)):
        return None
    if any(t[0] in ('local','return','function') for t in tokens(''.join(code))):
        return None
    if any(line.strip() and not line.startswith(indent+'    ') for line in code):
        return None
    flat=[line[4:] if line.startswith('    ') else line for line in lines]
    changed=True
    while changed:
        changed=False
        for i,line in enumerate(flat):
            inline=re.fullmatch(r'( *)do (goto [A-Za-z_]\w*) end\n',line)
            if inline:
                flat[i]=inline[1]+inline[2]+'\n'
                changed=True
                break
            match=re.fullmatch(r'( *)do\n',line)
            if not match:
                continue
            prefix=match[1]
            end=next((j for j in range(i+1,len(flat)) if flat[j]==prefix+'end\n'),None)
            if end is None or any(raw.strip() and not raw.startswith(prefix+'    ') for raw in flat[i+1:end]):
                continue
            flat[i:end+1]=[raw[4:] if raw.startswith('    ') else raw for raw in flat[i+1:end]]
            changed=True
            break
    return flat


def fold_constant_conditions(lines):
    """Fold proven literals without changing values, arity, text or scope."""
    source=''.join(lines)
    code=_constant_condition_structure(source)
    changes=[]
    # Only actual unary tokens; preserve comments between tokens and precedence.
    for match in re.finditer(r'\bnot[ \t]+(true|false)\b',code):
        if source[match.start():match.end()]!=match[0]:
            continue
        if code[match.end():].lstrip().startswith('^'):
            continue
        changes.append((match.start(),match.end(),'false' if match[1]=='true' else 'true'))
    for start,end,value in reversed(changes):
        source=source[:start]+value+source[end:]
    count=len(changes)
    lines=source.splitlines(True)
    code=_constant_condition_structure(source).splitlines(True)
    # Fold only a whole literal comparison. Never eat part of an operand or
    # turn Lua operand-valued and/or into boolean algebra.
    pattern=re.compile(r'(?P<head>\s*(?:(?:if|elseif|while|until|return) |(?:local )?[A-Za-z_]\w* = ))(?P<expr>\d+ (?:~=|==) 0)(?P<tail>(?: then(?: .* end)?| do)?[ \t]*\n)')
    for i,(raw,body) in enumerate(zip(lines,code)):
        match=pattern.fullmatch(body)
        if not match or raw[match.start('expr'):match.end('expr')]!=match['expr']:
            continue
        number,op,_=match['expr'].split()
        truth=int(number)!=0 if op=='~=' else int(number)==0
        lines[i]=raw[:match.start('expr')]+str(truth).lower()+raw[match.end('expr'):]
        count+=1
    # Constant branches still need lexical scopes and Lua's return placement.
    # Keep ambiguous blocks, labels, comments/diagnostics and alternate entries.
    changed=True
    while changed:
        changed=False
        code=_constant_condition_structure(''.join(lines)).splitlines(True)
        for i,(raw,body) in enumerate(zip(lines,code)):
            match=re.fullmatch(r'( *)if (true|false) then\n',body)
            inline=re.fullmatch(r'( *)if (true|false) then (.+) end\n',body)
            if inline:
                if raw!=body:
                    continue
                indent,value,statement=inline.groups()
                # Do not discard diagnostic-bearing edges or label entries.
                if '::' in statement:
                    continue
                if any(t[0] in {'if','then','else','elseif','end','function','for','while','repeat','until','do'} for t in tokens(statement)):
                    continue
                lines[i]=indent+'do '+statement+' end\n' if value=='true' else ''
                count+=1
                changed=True
                break
            if not match or raw!=body:
                continue
            indent,value=match.groups()
            end=next((j for j in range(i+1,len(lines)) if code[j]==indent+'end\n'
                or code[j]==indent+'else\n' or code[j].startswith(indent+'elseif ')),None)
            if end is None:
                continue
            if any(line.strip() and not line.startswith(indent+'    ') for line in code[i+1:end]):
                continue
            if value=='false' and lines[end]!=indent+'end\n':
                if any(a!=b or '::' in b for a,b in zip(lines[i+1:end],code[i+1:end])):
                    continue
                if code[end].startswith(indent+'elseif '):
                    lines[i:end+1]=[indent+'if '+lines[end][len(indent+'elseif '):]]
                elif lines[end]==indent+'else\n':
                    close=next((j for j in range(end+1,len(lines)) if code[j]==indent+'end\n'),None)
                    kept=_plain_constant_else(lines[end+1:close],code[end+1:close],indent) if close is not None else None
                    if kept is None:
                        lines[i:end+1]=[indent+'do\n']
                    else:
                        lines[i:close+1]=kept
                else:
                    continue
                count+=1
                changed=True
                break
            if lines[end]!=indent+'end\n':
                continue
            if value=='true':
                lines[i]=indent+'do\n'
            else:
                if any(a!=b or '::' in b for a,b in zip(lines[i+1:end],code[i+1:end])):
                    continue
                lines[i:end+1]=[]
            count+=1
            changed=True
            break
        lines=[line for line in lines if line]
    return lines,count


def fold_frame_checks(lines):
    """`R:NewScriptFrame(..)` then `if R:IsActiveThreadTerminating() then X end`
    -> `if not R:NewScriptFrame(..) then X end`."""
    count = 0
    for i in range(len(lines) - 1):
        a = re.fullmatch(r'(\s*)(\w+):NewScriptFrame\(([^\n]*)\)\n', lines[i])
        if not a:
            continue
        b = re.fullmatch(re.escape(a.group(1)) + r'if ' + a.group(2) + r':IsActiveThreadTerminating\(\) then ((?:[^\n;]+; )?' + EXIT + r') end\n', lines[i + 1])
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
                elif lines[j].lstrip().startswith('--'):
                    pass                     # a comment an earlier fold orphaned at the guard's own indent
                elif lines[j].strip():
                    raise ValueError('guard body indentation')
            follows = next((lines[j] for j in range(end + 1, len(lines)) if lines[j].strip()), '')
            if lines[end - 1] == indent + 'return\n' and follows and _indent(follows) >= len(indent) and not follows.startswith(indent + 'end'):
                lines[end - 1] = indent + 'do return end\n'     # (a return is only legal last in its block)
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
        if name in line and name in _line_reads(line):
            return True                           # (`v = v or E` reads the value before redefining it)
        if j in defs:
            continue
        queue.extend(flow.graph.get(j, ()))
    return False


QUERY_METHOD = re.compile(r'^(?:Get(?!Random|Hero$)|Is|Has|Can|Msg)')


def _sink_target(flow, name, d, expr, reads, def_set, touched=frozenset()):
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
        if s == d or flow.preds.get(s) != {prev} or s in touched:
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
        if written and (written[1] not in _declared_temporaries(flow.structure)
                        or written[1] in flow.pinned):
            return None  # global/upvalue or captured-local stores are observable
        if not written and re.search(r'(?<![~=<>])=(?!=)', line):
            return None  # a field/index store is not a plain local assignment
        intervening_calls = _calls(flow.lines[s])
        if intervening_calls:
            return None  # query names do not establish reorderable effects/errors
        if written and not intervening_calls:
            # Indexing and operators can invoke metamethods or raise, even
            # without a syntactic call. Only inert staging can be crossed.
            rhs = flow.lines[s].split('=', 1)[1].strip()
            stream = [t for t in tokens(rhs) if t.lastgroup not in
                      ('space', 'comment', 'longcomment')]
            literal = (bool(re.fullmatch(
                r'-?(?:0[xX][\da-fA-F]+|(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][+-]?\d+)?)', rhs))
                or len(stream) == 1 and
                (stream[0].lastgroup in ('string', 'longstring')
                 or stream[0][0] in ('nil', 'true', 'false')))
            local_read = (len(stream) == 1 and stream[0].lastgroup == 'identifier'
                          and stream[0][0] in _declared_temporaries(flow.structure)
                          and stream[0][0] not in flow.pinned)
            if not (literal or local_read):
                return None
        if expr_calls and intervening_calls and any(n == 'IsActiveThreadTerminating' or n.startswith('GetRandom') for _, _, n in [*_calls(expr), *intervening_calls]):
            return None
        if any(kind != ':' or not QUERY_METHOD.match(n) for kind, _, n in intervening_calls):
            return None
        prev = s
    return None


def _prefix_has_implicit_evaluation(prefix):
    """Conservatively retain values before preceding indexing/operator work.

    An unfinished operator immediately before the use runs after its operand;
    completed operands earlier in the prefix may invoke metamethods or raise.
    """
    stream = [t for t in tokens(prefix) if t.lastgroup not in
              ('space', 'comment', 'longcomment')]
    operators = {'+', '-', '*', '/', '//', '%', '^', '..', '#', '&', '|',
                 '~', '<<', '>>', '==', '~=', '<', '>', '<=', '>='}
    pending = operators | {'(', 'not'}
    for i, token in enumerate(stream):
        value = token[0]
        if value == '[':
            return True
        if value == '.':
            # The lexer emits decimal numbers character by character.
            if (i and stream[i - 1][0].isdigit()
                    or i + 1 < len(stream) and stream[i + 1][0].isdigit()):
                continue
            return True
        if value in operators and any(t[0] not in pending for t in stream[i + 1:]):
            return True
    return False


def _prefix_bindings_are_stable(flow, prefix, definition):
    """An effectful expression cannot pass a mutable binding/method lookup."""
    stream = [t for t in tokens(prefix) if t.lastgroup not in
              ('space', 'comment', 'longcomment', 'string', 'longstring')]
    if any(t[0] == ':' for t in stream):
        return False  # method resolution can invoke __index or select a changed method
    names = _line_reads(_structure_line(prefix))
    if not names:
        return True
    if names & flow.pinned:
        return False
    head = re.fullmatch(r'(?:local )?function \w+\(([^)]*)\)', flow.structure[0])
    params = {n.strip() for n in head[1].split(',')} if head else set()
    bindings = {}
    for i, line in enumerate(flow.structure):
        match = re.fullmatch(r'(\s*)local ([\w, ]+?)(?: = .+)?', line)
        if match:
            for name in match[2].split(','):
                bindings.setdefault(name.strip(), []).append((i, len(match[1])))
    for name in names:
        sites = bindings.get(name, ())
        if name in params and not sites:
            continue
        if name in params or len(sites) != 1 or sites[0][1] != 4:
            return False
        declaration = sites[0][0]
        if declaration >= definition:
            return False
        seen, queue = {declaration}, [0]
        while queue:
            i = queue.pop()
            if i in seen:
                continue
            if i == definition:
                return False
            seen.add(i)
            queue.extend(flow.graph.get(i, ()))
    return True


def _inert_inline_value(flow, expression, definition):
    """Literal values and unexposed local reads have no reorderable effects."""
    stream = [t for t in tokens(expression) if t.lastgroup not in
              ('space', 'comment', 'longcomment')]
    if re.fullmatch(r'-?(?:0[xX][\da-fA-F]+|(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][+-]?\d+)?)', expression):
        return True
    if len(stream) != 1:
        return False
    token = stream[0]
    if token.lastgroup in ('string', 'longstring') or token[0] in ('nil', 'true', 'false'):
        return True
    return (token.lastgroup == 'identifier' and token[0] not in KEYWORDS_ALL
            and _prefix_bindings_are_stable(flow, 'return ' + expression, definition))


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
        touched = set()
        for name in _declared_temporaries(structure):
            if name in flow.opaque_pinned:
                continue
            defs, reads = _reads_and_defs(structure, name)
            if not defs or not reads:
                continue
            def_set = set(defs)
            for d in defs:
                m = re.fullmatch(r'(\s*)' + re.escape(name) + r' = ([^\n]+?)\s*\n', lines[d])
                if not m or any(t.lastgroup in ('comment', 'longcomment') for t in tokens(lines[d])):
                    continue
                if compound_scalar_stores([structure[d]]) and d not in flow.inert_nil_initializations:
                    continue
                expr = m.group(2)
                if _has_expression_list(expr):
                    continue  # a single local can discard later results while retaining their effects
                if d in touched:
                    continue
                r = _sink_target(flow, name, d, expr, reads, def_set, touched)
                if r is None or r in touched:
                    continue
                # An inline assignment destination is not a substitution site.
                # Later inline writes are conservative reads in reachability;
                # retain their binding while allowing unrelated earlier uses.
                if name in compound_scalar_stores([structure[r]]):
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
                # A line is one CFG node, but a use after a short-circuit
                # operator or inside an inline branch need not execute.
                # Keep the unconditional evaluation at its original statement.
                if any(t[0] in ('and', 'or', 'then', 'else', 'elseif', 'function')
                       for t in tokens(prefix) if t.lastgroup == 'identifier'):
                    continue
                if (not _inert_inline_value(flow, expr, d)
                        and not _prefix_bindings_are_stable(flow, prefix, d)):
                    continue
                if _prefix_has_implicit_evaluation(prefix):
                    continue
                prefix_calls, expression_calls = _calls(prefix), _calls(expr)
                if prefix_calls and expression_calls and any(n == 'IsActiveThreadTerminating' or n.startswith('GetRandom') for _, _, n in [*prefix_calls, *expression_calls]):
                    continue  # scheduler boundary queries cannot cross another call
                # Calls whose arguments already closed run before the moved
                # expression. State reads and variable reads must not cross
                # mutations; string/table argument sugar also completes calls.
                completed = _calls(prefix, completed_only=True)
                if completed:
                    continue  # preserve completed query calls and their errors
                needs_arity_guard = _can_expand_results(expr) and _reference_can_expand(marked, marker)
                replacement = expr if _atomic(expr) and not needs_arity_guard else f'({expr})'
                if re.fullmatch(r'nil|true|false|-?\d[\w.]*|"[^"]*"', expr) and re.search(r'(?<![\w.:])' + re.escape(name) + r'\s*[:.\[(]', _structure_line(read_line)):
                    replacement = f'({expr})'            # a literal prefix expression must be parenthesised
                lines[r] = rename_identifiers(marked, {marker: replacement}).replace('__lhs__ =', name + ' =', 1)
                lines[d] = ''
                if len(defs) == 1 and len(reads) == 1:
                    _remove_declaration(lines, name)
                count += 1
                done = True
                touched.update((d, r))
                if len(defs) == 1 and len(reads) == 1:
                    break                          # the variable is gone; other definitions no longer exist
        if not done:
            return lines, count


def _block_end(structure, start, indent):
    """First line after `start` that dedents out of the block holding it (exclusive)."""
    for j in range(start + 1, len(structure)):
        if structure[j].strip() and _indent(structure[j]) < indent:
            return j
    return len(structure)


def sink_hoisted_locals(lines):
    """`local a, b, c` at the top of the function, then `b = <expr>` deep inside it, is how the lifter
    spells a slot; Aeon declares the value where it is computed. Move the declaration onto its assignment
    when that cannot change what any line sees.

    Five conditions, all necessary: exactly one assignment; every read after it; every read inside the
    assignment's own block (nothing between them dedents below it); no earlier closure captures the name
    (the hoisted `ReleaseEverything()` epilogues close over `movie` / `timerId`); and no label inside the
    new scope is the target of a `goto` from before it -- Lua 5.2+ rejects a jump into a local's scope, so
    that last one is the difference between narrowing a scope and emitting a file that will not load.
    """
    structure = _structure(lines)
    ranges = _closure_ranges(structure)
    if ranges is None:
        return lines, 0
    labels = {m.group(1): i for i, l in enumerate(structure) for m in [re.fullmatch(r'\s*::(\w+)::', l)] if m}
    gotos = [(i, m.group(1)) for i, l in enumerate(structure) for m in [re.search(r'\bgoto (\w+)\b', l)] if m]
    count = 0
    pinned = random_result_locals(structure)
    for name in _declared_temporaries(structure):
        if name in pinned:
            continue  # narrowing this scope can release an owned random result early
        defs, reads = _reads_and_defs(structure, name)
        if len(defs) != 1 or not reads:
            continue
        definition = defs[0]
        if not re.fullmatch(r'\s*' + re.escape(name) + r'\s*=(?!=).*', structure[definition]):
            continue                                   # a compound target, not a plain `name = expr`
        if any(r <= definition for r in reads):
            continue
        indent = _indent(structure[definition])
        end = _block_end(structure, definition, indent)
        if max(reads) >= end:
            continue                                   # a read outside the block would lose the binding
        if any(a < definition and re.search(r'\b' + re.escape(name) + r'\b', ''.join(lines[a:b + 1]))
               for a, b in ranges):
            continue                                   # an earlier closure captured it
        inside = {n for n, i in labels.items() if definition <= i < end}
        if any(target in inside and at < definition for at, target in gotos):
            continue                                   # a goto would jump into the new scope
        _remove_declaration(lines, name)
        lines[definition] = re.sub(r'^(\s*)' + re.escape(name) + r'\s*=', r'\1local ' + name + ' =',
                                   lines[definition], count=1)
        count += 1
        if count:
            return [l for l in lines if l != ''], count        # indices shift: rerun for the next name
    return lines, count


def _call_statement(expression):
    """Whether a complete expression is a simple Lua call statement.

    A call somewhere inside an expression is not enough: `prefix .. f()` and
    `f() + g()` cannot stand alone as statements. Keep their assignment so all
    calls, operators and short-circuit evaluation retain their original order.
    """
    code = _structure_line(expression).strip()
    head = re.match(r'[A-Za-z_]\w*(?:\.\w+)*(?::\w+)?\s*\(', code)
    if not head:
        return False
    depth = 1
    for i in range(head.end(), len(code)):
        if code[i] == '(':
            depth += 1
        elif code[i] == ')':
            depth -= 1
            if depth == 0:
                return i == len(code) - 1
    return False


def prune_dead_defs(lines):
    """A temporary's assignment whose value no read can reach: gone when the value is a literal or a
    state query, kept as the bare call when it has effects (`quest:NewScriptFrame(me)`)."""
    count = 0
    flow = Flow(lines)
    if not flow.ok:
        return lines, 0
    structure = flow.structure
    for name in _declared_temporaries(structure):
        if name in flow.opaque_pinned:
            continue
        defs, reads = _reads_and_defs(structure, name)
        def_set = set(defs)
        self_only = bool(reads) and all(r in def_set for r in reads)   # `v = v or E`: the value never leaves v
        for d in defs:
            m = re.fullmatch(r'(\s*)' + re.escape(name) + r' = ([^\n]+?)\s*\n', lines[d])
            if not m or any(t.lastgroup in ('comment', 'longcomment') for t in tokens(lines[d])):
                continue
            if compound_scalar_stores([structure[d]]) and d not in flow.inert_nil_initializations:
                continue
            if d in reads or _read_reachable(flow, name, flow.graph.get(d, ()), def_set):
                continue
            if (_prefix_has_implicit_evaluation(m.group(2))
                    and not _inert_inline_value(flow, m.group(2), d)):
                continue  # unused results can still run metamethods or raise
            if not _environment_evaluation_is_inert(flow, m.group(2), d):
                continue
            calls = _calls(m.group(2))
            if not calls or all(kind == ':' and (recv in PURE_RECEIVER or QUERY_METHOD.match(n)) for kind, recv, n in calls):
                lines[d] = ''
            elif self_only:
                continue                              # `v = v and f()`: keep the statement as is
            elif _call_statement(m.group(2)):
                lines[d] = f'{m.group(1)}{m.group(2)}\n'
            else:
                continue
            count += 1
    return lines, count


NESTED_OPENER = re.compile(r'(?:if .+ then|while .+ do|for .+ do|repeat|do)')   # a multi-line block head


def _block_openers(lines):
    """`end` line -> the head it closes. A `goto` out of a LOOP is not the same as a `goto` to the end of an
    enclosing `if`: folding that jump away leaves the loop spinning where the script returned
    (TraderConflictGood WatchForKilledPeople: `if not NewScriptFrame() then goto <function end> end` became
    `if NewScriptFrame() then .. end` INSIDE the while, a frame-less loop once the thread terminates, and the
    smoke harness reported `call trace overflow (loop without frames?)`, 2026-09-22)."""
    openers, stack = {}, []
    for i, line in enumerate(lines):
        st = line.strip()
        if not st:
            continue
        if st.startswith(('function ', 'local function ')) or re.fullmatch(r'(?:local )?[\w.:]+ = function\(.*\)', st):
            stack.append(st)
        elif NESTED_OPENER.fullmatch(st):
            stack.append(st)
        elif st == 'end' and stack:
            openers[i] = stack.pop()
        elif st.startswith('until ') and stack:
            stack.pop()
    return openers


def fold_goto_else(lines):
    """`if C then X; goto L end; Y; ::L::` -> `if C then X else Y end` (the goto skips the rest of the
    enclosing branch); `if X then goto L end; Y; ::L::` -> `if not X then Y end`; an enclosing if whose
    whole body is that if merges its condition with `and`."""
    # Raw line structure inside opaque multiline text is not Lua control flow.
    if any(t.lastgroup in ('string', 'longstring', 'longcomment')
           and any(c in t[0] for c in '\r\n') for t in tokens(''.join(lines))):
        return lines, 0
    count = 0
    changed = True
    while changed:
        changed = False
        lines = [l for l in lines if l != '']
        text = ''.join(lines)
        goto_counts = Counter(re.findall(r'\bgoto (\w+)', text))
        for g, line in enumerate(lines):
            if 'goto ' not in line:
                continue
            m = re.fullmatch(r'(\s*)goto (\w+)\n', line)
            inline = re.fullmatch(r'(\s*)if (.+?) then (?:([^\n;]+); )?goto (\w+) end\n', line)
            if not m and not inline:
                continue
            label = inline.group(4) if inline else m.group(2)
            if goto_counts[label] != 1:
                continue
            if inline and inline.group(3):
                # `if C then STMT; goto L end`: a then-branch of one statement
                indent, cond, if_line, end = inline.group(1), inline.group(2), g, g
                then_body = [f'{indent}    {inline.group(3)}\n']
                inline = None
            elif inline:
                indent, cond, if_line, then_body, end = inline.group(1), inline.group(2), g, [], g
            else:
                indent = m.group(1)[:-4] if len(m.group(1)) >= 4 else None
                if indent is None or g + 1 >= len(lines) or lines[g + 1] != indent + 'end\n':
                    continue
                if_line = next((j for j in range(g - 1, -1, -1) if lines[j].startswith(indent) and not lines[j].startswith(indent + ' ') and lines[j].strip()), None)
                head = re.fullmatch(re.escape(indent) + r'if (.+) then\n', lines[if_line]) if if_line is not None else None
                if not head:
                    continue
                cond, then_body, end = head.group(1), lines[if_line + 1:g], g + 1
            # enclosing ifs whose whole body is this if: merge conditions
            while len(indent) >= 4:
                outer_indent = indent[:-4]
                nxt = next((j for j in range(end + 1, len(lines)) if lines[j].strip()), None)
                if nxt is None or lines[nxt] != outer_indent + 'end\n':
                    break
                outer = next((j for j in range(if_line - 1, -1, -1) if lines[j].strip()), None)
                head = re.fullmatch(re.escape(outer_indent) + r'if (.+) then\n', lines[outer]) if outer is not None else None
                if not head:
                    break
                a, b = head.group(1), cond
                a = f'({a})' if 'or' in _top_level_operators(a) else a
                b = f'({b})' if 'or' in _top_level_operators(b) else b
                cond, if_line, indent, end = f'{a} and {b}', outer, outer_indent, nxt
            # the rest of the branch (same block level) up to where the block closes or the label sits
            j = end + 1
            rest_end = None
            depth = 0                 # blocks opened at this indent inside the rest: their `end` is not ours
            while j < len(lines):
                l = lines[j]
                if not l.strip() or l.startswith(indent + ' '):
                    j += 1
                    continue
                if not l.startswith(indent):
                    rest_end = j
                    break
                st = l.strip()
                if depth:
                    if st == 'end' or st.startswith('until '):
                        depth -= 1
                    elif NESTED_OPENER.fullmatch(st):
                        depth += 1
                    j += 1
                    continue
                if st in ('end', 'else') or st.startswith(('elseif ', 'until ')) or st == f'::{label}::':
                    rest_end = j
                    break
                if NESTED_OPENER.fullmatch(st):
                    depth += 1
                j += 1
            if rest_end is None:
                continue
            def shift(block, delta):
                out = []
                for b in block:
                    if not b.strip():
                        out.append(b)
                    elif delta >= 0:
                        out.append(' ' * delta + b)
                    else:
                        out.append(b[-delta:] if b.startswith(' ' * -delta) else b.lstrip())
                return out
            k = rest_end
            openers = _block_openers(lines)
            while k < len(lines) and lines[k].strip() == 'end':
                if openers.get(k, '').startswith(('while ', 'for ', 'repeat')):
                    break               # the jump leaves a LOOP: the label is not this block's tail
                k += 1
            body2 = lines[end + 1:rest_end]
            outer_else = None
            if k >= len(lines) or lines[k].strip() != f'::{label}::':
                # the rest of the branch ends in an exit and the enclosing if's remainder up to the label
                # is only reached when its condition fails: that remainder is the enclosing if's else
                last = next((l.strip() for l in reversed(body2) if l.strip()), '')
                outer_indent = indent[:-4] if len(indent) >= 4 else None
                if not body2 or outer_indent is None or not re.fullmatch(r'(?:[^\n;]+; )?(?:return\b.*|goto \w+|break)', last) \
                        or rest_end >= len(lines) or lines[rest_end] != outer_indent + 'end\n':
                    continue
                outer = next((j for j in range(if_line - 1, -1, -1) if lines[j].startswith(outer_indent) and not lines[j].startswith(outer_indent + ' ') and lines[j].strip()), None)
                if outer is None or not re.fullmatch(re.escape(outer_indent) + r'if (.+) then\n', lines[outer]):
                    continue
                j2 = rest_end + 1
                d_end = None
                depth = 0
                while j2 < len(lines):
                    l = lines[j2]
                    if not l.strip() or l.startswith(outer_indent + ' '):
                        j2 += 1
                        continue
                    if not l.startswith(outer_indent):
                        d_end = j2
                        break
                    st = l.strip()
                    if depth:
                        if st == 'end' or st.startswith('until '):
                            depth -= 1
                        elif NESTED_OPENER.fullmatch(st):
                            depth += 1
                        j2 += 1
                        continue
                    if st in ('end', 'else') or st.startswith(('elseif ', 'until ')) or st == f'::{label}::':
                        d_end = j2
                        break
                    if NESTED_OPENER.fullmatch(st):
                        depth += 1
                    j2 += 1
                if d_end is None:
                    continue
                k2 = d_end
                while k2 < len(lines) and lines[k2].strip() == 'end':
                    k2 += 1
                if k2 >= len(lines) or lines[k2].strip() != f'::{label}::':
                    continue
                d_body = lines[rest_end + 1:d_end]
                if not d_body or any(l.strip().startswith('::') for l in d_body):
                    continue
                outer_else = (outer, outer_indent, rest_end, d_end, d_body)
            if any(l.strip().startswith('::') for l in body2):
                continue                                  # a label would move into a nested block
                out = []
                for b in block:
                    if not b.strip():
                        out.append(b)
                    elif delta >= 0:
                        out.append(' ' * delta + b)
                    else:
                        out.append(b[-delta:] if b.startswith(' ' * -delta) else b.lstrip())
                return out
            body2 = shift(body2, 4)
            if inline:
                new = [f'{indent}if {_negate(cond)} then\n'] + body2 + [f'{indent}end\n']
            else:
                then_indent = len(then_body[0]) - len(then_body[0].lstrip()) if then_body else len(indent) + 4
                then_body = shift(then_body, len(indent) + 4 - then_indent)
                new = [f'{indent}if {cond} then\n'] + then_body + ([f'{indent}else\n'] + body2 if body2 else []) + [f'{indent}end\n']
            if outer_else:
                outer, outer_indent, o_end, d_end, d_body = outer_else
                lines[o_end:d_end] = [f'{outer_indent}else\n'] + shift(d_body, 4) + [f'{outer_indent}end\n']
            lines[if_line:rest_end] = new
            text2 = ''.join(lines)
            if not re.search(r'\bgoto ' + label + r'\b', text2):
                for k2, l in enumerate(lines):
                    if l.strip() == f'::{label}::':
                        lines[k2] = ''
                        break
            count += 1
            changed = True
            break
    return lines, count


BOOL_VALUE = re.compile(r'(?:not |true$|false$|[\w.:]+:(?:Is|Has|GetStateBool)\w*\()')


def _boolean_valued(expr):
    expr = expr.strip()
    return bool(BOOL_VALUE.match(expr)) or bool({'==', '~=', '<', '>', '<=', '>=', 'and', 'or'} & set(_top_level_operators(expr)))


def fold_boolean_branches(lines):
    """`if C then v = false else v = Y end` -> `v = not C and Y` (and the three sibling shapes) when Y is
    boolean-valued."""
    count = 0
    changed = True
    while changed:
        changed = False
        lines = [l for l in lines if l != '']
        for i in range(len(lines) - 4):
            m = re.fullmatch(r'(\s*)if (.+) then\n', lines[i])
            if not m or lines[i + 2] != m.group(1) + 'else\n' or lines[i + 4] != m.group(1) + 'end\n':
                continue
            indent, cond = m.group(1), m.group(2)
            a = re.fullmatch(re.escape(indent) + r'    (\w+) = ([^\n]+)\n', lines[i + 1])
            b = re.fullmatch(re.escape(indent) + r'    (\w+) = ([^\n]+)\n', lines[i + 3])
            if not a or not b or a.group(1) != b.group(1):
                continue
            if any(t.lastgroup == 'comment' for t in tokens(lines[i + 1] + lines[i + 3])):
                continue
            x, y = a.group(2).strip(), b.group(2).strip()
            def wrap_or(e):
                return f'({e})' if 'or' in _top_level_operators(e) else e
            nc = _negate(cond)
            if x == 'false' and _boolean_valued(y):
                value = f'{nc} and {wrap_or(y)}'
            elif x == 'true' and _boolean_valued(y):
                value = f'{cond} or {y}'
            elif y == 'false' and _boolean_valued(x):
                value = f'{wrap_or(cond)} and {wrap_or(x)}'
            elif y == 'true' and _boolean_valued(x):
                value = f'{nc} or {x}'
            else:
                continue
            lines[i] = f'{indent}{a.group(1)} = {value}\n'
            for j in range(i + 1, i + 5):
                lines[j] = ''
            count += 1
            changed = True
            break
    return lines, count


def fold_elseif(lines):
    """`else` whose branch is exactly one `if` block -> `elseif`."""
    count = 0
    changed = True
    while changed:
        changed = False
        lines = [l for l in lines if l != '']
        for i in range(len(lines) - 2):
            m = re.fullmatch(r'(\s*)else\n', lines[i])
            if not m:
                continue
            indent = m.group(1)
            head = re.fullmatch(re.escape(indent) + r'    if (.+) then\n', lines[i + 1])
            if not head:
                continue
            # the inner block ends right before this block's `end`
            j = i + 2
            inner_end = None
            while j < len(lines):
                if lines[j] == indent + '    end\n':
                    inner_end = j
                    break
                if lines[j].startswith(indent + '    ') or not lines[j].strip():
                    j += 1
                    continue
                break
            if inner_end is None or inner_end + 1 >= len(lines) or lines[inner_end + 1] != indent + 'end\n':
                continue
            lines[i] = f'{indent}elseif {head.group(1)} then\n'
            lines[i + 1] = ''
            for j in range(i + 2, inner_end):
                if lines[j].startswith(indent + '        '):
                    lines[j] = lines[j][4:]
                elif lines[j].startswith(indent + '    ') and re.match(r'\s*(?:else\n|elseif )', lines[j]):
                    lines[j] = lines[j][4:]
            lines[inner_end] = ''
            count += 1
            changed = True
            break
    return lines, count


def prune_unused_closures(lines):
    """Remove an unused multiline cleanup, retaining references as values."""
    count = 0
    masked = _constant_condition_structure(''.join(lines))
    structure, offset = [], 0
    for line in lines:
        structure.append(masked[offset:offset + len(line)].rstrip('\n'))
        offset += len(line)
    for a, b in _closure_ranges(structure) or []:
        name = re.match(r'\s*local function (\w+)\(', structure[a]).group(1)
        if not any(re.search(r'\b' + name + r'\b', structure[i]) for i in range(len(structure)) if not a <= i <= b):
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
    # `if C then <comments only> else BODY end` -> `if not C then BODY end`
    for i in range(len(lines) - 2):
        m = re.fullmatch(r'(\s*)if (.+) then\n', lines[i])
        if not m:
            continue
        j = i + 1
        while j < len(lines) and lines[j].strip().startswith('--'):
            j += 1
        if j > i and j < len(lines) and lines[j] == m.group(1) + 'else\n':
            lines[i] = f'{m.group(1)}if {_negate(m.group(2))} then\n'
            for k in range(i + 1, j + 1):
                lines[k] = ''
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
        flow = None
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
            w = re.fullmatch(r'(\s*)while (.+) do\n', lines[i + 1]) if m else None
            if m and w and m.group(1) == w.group(1) and m.group(2) in _line_reads(_structure_line(lines[i + 1])) \
                    and len(re.findall(r'\b' + m.group(2) + r'\b', _structure_line(lines[i + 1]))) == 1:
                indent, name, expr = m.group(1), m.group(2), m.group(3)
                end = next((j for j in range(i + 2, len(lines)) if lines[j] == indent + 'end\n'), None)
                if end is None:
                    continue
                last = next((j for j in range(end - 1, i + 1, -1) if lines[j].strip()), None)
                if last is None or lines[last] != f'{indent}    {name} = {expr}\n':
                    continue
                if flow is None:
                    flow = Flow(lines)
                defs, reads = _reads_and_defs(structure, name)
                if set(defs) != {i, last} or reads != [i + 1]:
                    # the temporary serves other loops too (`scratchValue7 = me:IsPerformingScriptTask()` before each
                    # Speak): this loop folds on its own when it holds no other definition or read of the name and no
                    # read of the loop's final value is reachable from its exit
                    inside = [k for k in defs if i < k < end and k != last] + [k for k in reads if i < k < end and k != i + 1]
                    exits = [s for s in (flow.graph.get(i + 1, ()) if flow.ok else ()) if not (i + 1 < s <= end)]
                    if inside or not flow.ok or _read_reachable(flow, name, exits, set(defs)):
                        continue
                whole = w.group(2).strip() == name or w.group(2).strip() == 'not ' + name
                cond = expr if _atomic(expr) or (whole and not w.group(2).startswith('not ')) else f'({expr})'
                lines[i + 1] = f'{indent}while {rename_identifiers(w.group(2), {name: cond})} do\n'
                lines[i] = lines[last] = ''
                if set(defs) == {i, last}:
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
    check = re.compile(r'^(\s*)if ' + TERMINATING + r' then (?:[^\n;]+; )?' + EXIT + r' end$')
    frame = re.compile(r'^(\s*)if not \w+:NewScriptFrame\([^\n]*\) then (?:[^\n;]+; )?' + EXIT + r' end$')
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
        if re.fullmatch(r'\s*while not ' + TERMINATING + r' do', s):
            return succ == min(flow.graph[i])   # the body runs only while not terminating
        if not _pure_line(structure[i], lines[i], pure_functions, flow, i):
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
        elif (q := re.fullmatch(r'(\s*)return not ' + TERMINATING, s)) and state_in[i]:
            lines[i] = f'{q.group(1)}return true\n'
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
            # v = false ; if C then v = true end   ->  v = C
            pre = re.fullmatch(r'(\s*)(\w+) = (true|false)\n', lines[i])
            if pre and i + 3 < len(lines):
                head = re.fullmatch(re.escape(pre.group(1)) + r'if ([^\n]+) then\n', lines[i + 1])
                store = re.fullmatch(re.escape(pre.group(1)) + r'    ' + pre.group(2) + r' = (true|false)\n', lines[i + 2])
                if head and store and lines[i + 3] == pre.group(1) + 'end\n' and store.group(1) != pre.group(3) \
                        and not any(t.lastgroup == 'comment' for t in tokens(lines[i] + lines[i + 2])):
                    value = head.group(1) if store.group(1) == 'true' else _negate(head.group(1))
                    lines[i] = f'{pre.group(1)}{pre.group(2)} = {value}\n'
                    lines[i + 1] = lines[i + 2] = lines[i + 3] = ''
                    count += 1
                    changed = True
                    break
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
        # Calls/varargs can contribute multiple values only at the tail of an
        # expression list. Conditions and receivers still consume one value.
        multiple_assignment = prev == '=' and _has_expression_list(''.join(t[0] for t in stream[:sig[open_n] - 1]))
        after_next = stream[sig[close_n + 2]][0] if close_n + 2 < len(sig) else ''
        if _can_expand_results(content) and _result_list_tail(prev, nxt, after_next) and (prev != '=' or multiple_assignment):
            continue
        ops = set(_top_level_operators(content))
        enclosing = max((o for o, (c, _) in pairs.items() if o < open_n and c > close_n), default=None)
        if enclosing is not None and pairs[enclosing][1] and prev in ('(', ',') and nxt in (',', ')'):
            drop.add(open_n)                                            # a whole call argument
        elif prev in ('if', 'elseif', 'while', 'until', '=', 'return') and nxt in ('then', 'do', ''):
            drop.add(open_n)                                            # a whole condition / rhs
        elif not ops:
            # an atomic operand -- unless it is a literal in prefix position: `(nil):IsAlive()` is legal Lua,
            # `nil:IsAlive()` is not (the literal comes from an inlined `x = nil` staging)
            if nxt in (':', '.', '[', '(') and (not re.match(r'[A-Za-z_]', content) or content in ('nil', 'true', 'false')):
                continue
            drop.add(open_n)
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


def fold_numeric_truth_tests(line):
    """Remove a 0/1 round trip without changing a comparison's Boolean result."""
    pattern = re.compile(r' and (?P<yes>[01]) or (?P<no>[01])(?P<close>\)) (?P<op>~=|==) 0(?![\w.])')
    while True:
        masked = _structure_line(line)
        stack, opens = [], {}
        for i, char in enumerate(masked):
            if char == '(':
                stack.append(i)
            elif char == ')' and stack:
                opens[i] = stack.pop()
        changed = False
        for match in pattern.finditer(masked):
            following = masked[match.end():].lstrip()
            if following and following[0] in '+-*/%^&|~<>.':
                continue  # zero is part of a larger operand, not the comparison's RHS
            inner_end = match.start() - 1
            inner_start = opens.get(inner_end)
            if (inner_start is None or match['yes'] == match['no']
                    or opens.get(match.start('close')) != inner_start - 1):
                continue
            condition = line[inner_start + 1:inner_end]
            invert = (match['op'] == '~=') == (match['yes'] == '0')
            replacement = ('not ' if invert else 'not not ') + '(' + condition + ')'
            line = line[:inner_start - 1] + replacement + line[match.end():]
            changed = True
            break
        if not changed:
            return line


def simplify_conditions(lines):
    """`not (not X)` -> X, `not (a == b)` -> `a ~= b`, `((C) and 0 or 1) ~= 0` -> `not (C)`, and an
    unread `p = T()` (pure) staging store."""
    count = 0
    structure = _structure(lines)
    for i, line in enumerate(lines):
        if line.lstrip().startswith('--'):
            continue
        new = line
        new = fold_numeric_truth_tests(new)
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
            new = new[:m.start()] + '(' + neg + ')' + new[end:]
            pos = m.start()
        for _ in range(4):
            before = new
            new = re.sub(r'\bnot \(not (\([^()]*(?:\([^()]*\)[^()]*)*\)|[\w.:]+(?:\([^()]*\))?) or not (\([^()]*(?:\([^()]*\)[^()]*)*\)|[\w.:]+(?:\([^()]*\))?)\)', r'\1 and \2', new)
            new = re.sub(r'\bnot \(not (\([^()]*(?:\([^()]*\)[^()]*)*\)|[\w.:]+(?:\([^()]*\))?) and not (\([^()]*(?:\([^()]*\)[^()]*)*\)|[\w.:]+(?:\([^()]*\))?)\)', r'(\1 or \2)', new)
            if new == before:
                break
        if new != line:
            lines[i] = new
            count += 1
    # p = T() with no readers
    for i, line in enumerate(lines):
        m = re.fullmatch(r'(\s*)(\w+) = (\w+:IsActiveThreadTerminating\(\))\n', line)
        if m:
            defs, reads = _reads_and_defs(structure, m.group(2))
            if not reads:
                lines[i] = f'{m.group(1)}{m.group(3)}\n'
                _remove_declaration(lines, m.group(2))
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


def drop_free_suffixes(lines):
    """`guildStagBeetle2` whose base name `guildStagBeetle` no longer occurs in the function (the earlier
    holder was inlined away): the base name, only for a proven local binding."""
    count = 0
    text = ''.join(lines)
    idents = {t.group(0) for t in re.finditer(r'[A-Za-z_]\w*', text)}
    # A bare assignment can write a global/upvalue. Tokenize the whole function
    # so declarations inside multiline comments/strings cannot establish scope.
    code = ''.join(re.sub(r'[^\n]', ' ', t[0])
                   if t.lastgroup in ('comment', 'longcomment', 'string', 'longstring')
                   else t[0] for t in tokens(text))
    structure = code.splitlines(keepends=True)
    declarations = {}
    header = re.fullmatch(r'(?:local )?function [\w.:]+\(([^)]*)\)', structure[0].strip()) if structure else None
    if header:
        for name in header[1].split(','):
            name = name.strip()
            if re.fullmatch(r'[A-Za-z_]\w*', name):
                declarations.setdefault(name, []).append((0, 1, len(structure[0])))
    for i, line in enumerate(structure):
        decl = re.match(r'^( *)local (?!function\b)([\w, ]+?)(?:\s*=|\s*$)', line)
        if decl:
            for name in decl[2].split(','):
                declarations.setdefault(name.strip(), []).append((i, len(decl[1]), decl.end()))
    renames = {}
    for name in sorted(idents):
        m = re.fullmatch(r'([A-Za-z]\w*?[A-Za-z])(\d+)', name)   # `thing_38` keeps its slot suffix
        if not m or int(m.group(2)) < 2 or m.group(1) in idents or m.group(1) in renames.values():
            continue
        if re.search(r'(?m)^\s*(?:local )?function\s+' + re.escape(name) + r'\b', text):
            continue                                   # a function definition, not a local
        bindings = declarations.get(name, [])
        if len(bindings) != 1:
            continue                                   # global/upvalue or shadowed binding
        start, indent, rhs = bindings[0]
        end = next((i for i in range(start + 1, len(structure))
                    if structure[i].strip() and len(structure[i]) - len(structure[i].lstrip(' ')) < indent), len(structure))
        word = r'\b' + re.escape(name) + r'\b'
        if (re.search(word, structure[start][rhs:]) or
                any(re.search(word, line) for line in structure[:start] + structure[end:])):
            continue                                   # initializer or out-of-scope use reads an outer value
        if not any(re.match(r'\s*(?:local )?' + re.escape(name) + r'\s*(?:,[\w\s,]*)?=(?!=)', l) for l in lines):
            continue                                   # not assigned here: a bare call statement starts
                                                       # with its name too (`helper_E12F20(quest)`)
        renames[name] = m.group(1)
    if renames:
        for i, l in enumerate(lines):
            lines[i] = rename_identifiers(l, renames)
        count = len(renames)
    return lines, count


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


def _negate_all(cond):
    """De Morgan over a top-level `or` / `and` chain of comparisons; falls back to `_negate`."""
    ops = _top_level_operators(cond)
    if 'or' in ops and 'and' not in ops:
        parts = [p.strip() for p in re.split(r' or ', cond)]
        if all(_atomic(p) or len(_top_level_operators(p)) == 1 for p in parts):
            return ' and '.join(_negate(p) for p in parts)
    return _negate(cond)


def fold_flag_clears(lines):
    """`if COND then X = false end; if X then` (the compiler's flag re-materialisation: a boolean cleared under a
    condition and tested right after) -> `if X and not COND then` when the branch is X's last read."""
    count = 0
    structure = _structure(lines)
    flow = None
    for i in range(len(lines) - 3):
        m = re.fullmatch(r'(\s*)if (.+) then\n', lines[i])
        if not m:
            continue
        indent, cond = m.group(1), m.group(2)
        c = re.fullmatch(re.escape(indent) + r'    (\w+) = (false|0)\n', lines[i + 1])
        if not c or lines[i + 2] != indent + 'end\n':
            continue
        x = c.group(1)
        test = f'{indent}if {x} then\n' if c.group(2) == 'false' else f'{indent}if {x} ~= 0 then\n'
        if lines[i + 3] != test:
            continue
        defs, reads = _reads_and_defs(structure, x)
        if i + 1 not in defs or any(i < r < i + 3 for r in reads) or i + 3 not in reads:
            continue
        if reads not in ([i + 3], [i, i + 3]):
            # the flag serves elsewhere too: the test must be the only read the cleared value can reach
            if flow is None:
                flow = Flow(lines)
            if not flow.ok or _read_reachable(flow, x, flow.graph.get(i + 3, ()), set(defs)):
                continue
        if re.search(r'\b' + x + r'\b', cond):
            # `if not X or C then X = false end`: with X true the `not X` term is gone
            ops = _top_level_operators(cond)
            parts = [p.strip() for p in re.split(r' or ', cond)] if 'or' in ops and 'and' not in ops else [cond]
            parts = [p for p in parts if p != f'not {x}']
            if not parts or any(re.search(r'\b' + x + r'\b', p) for p in parts):
                continue
            cond = ' or '.join(parts)
        lines[i] = lines[i + 1] = lines[i + 2] = ''
        lines[i + 3] = f'{indent}if {x}{"" if c.group(2) == "false" else " ~= 0"} and {_negate_all(cond)} then\n'
        count += 1
        break
    return lines, count


def prune_dead_bitmask_staging(lines):
    """Drop a private, unused integer mask and its self-contained AND guards.

    Unlike general dead-store removal this proves every operand is an integer:
    initialization and masks are uint32 literals, and each subsequent value is
    their bitwise AND. No call, arithmetic fault, diagnostic, label, captured
    value, or global store may disappear with the group.
    """
    # Whole-chunk tokenization prevents multiline strings/comments from
    # inventing declarations or hiding uses on their interior lines.
    structure = ''.join(re.sub(r'[^\n]', ' ', t[0])
                        if t.lastgroup in ('comment', 'longcomment', 'string', 'longstring')
                        else t[0] for t in tokens(''.join(lines))).splitlines()
    declarations = Counter(_declared_temporaries(structure))
    number = r'(?:0x[0-9a-fA-F]+|\d+)'

    def integer(value):
        return int(value, 16 if value.startswith('0x') else 10) <= 0xffffffff

    flow = None
    for start, line in enumerate(lines):
        if structure[start] != line.rstrip('\n'):
            continue                                    # text inside a string/comment is not code
        first = re.fullmatch(r'([ \t]*)(\w+) = (' + number + r')\n', line)
        if not first or first[2] == '_ENV' or declarations[first[2]] != 1 or not integer(first[3]):
            continue
        indent, name = first[1], first[2]
        declaration = next(i for i, text in enumerate(structure)
                           if re.fullmatch(r'\s*local [\w, ]+\s*', text)
                           and name in re.findall(r'\w+', text)[1:])
        if declaration >= start:
            continue
        # The declaring block must still enclose the assignment. Count lexical
        # block tokens, not indentation; elseif replaces its preceding then.
        depth, enclosed = 0, True
        for token in tokens('\n'.join(structure[declaration + 1:start])):
            word = token[0]
            if word in ('end', 'until', 'else', 'elseif') and depth == 0:
                enclosed = False
                break
            if word in ('function', 'then', 'do', 'repeat'):
                depth += 1
            elif word in ('end', 'until', 'elseif'):
                depth -= 1
        if not enclosed:
            continue
        operand = '(?:' + re.escape(name) + '|' + number + ')'
        condition = re.compile(re.escape(indent) + r'if \((' + operand + r') & (' + number + r')\) ~= 0 then\n')
        assignment = re.compile(re.escape(indent + '    ') + r'(\w+) = (' + operand + r') & (' + number + r')\n')
        aliases = {name}
        end, groups = start + 1, 0
        while end + 1 < len(lines):
            test = condition.fullmatch(lines[end])
            if not test:
                break
            # Earlier dead-store pruning may empty a guard. The enclosing
            # group still proves its local is a private uint32, so evaluating
            # this bitwise test cannot call user code or raise a type error.
            if lines[end + 1] == indent + 'end\n':
                if any(value != name and not integer(value) for value in (test[1], test[2])):
                    break
                end += 2
                groups += 1
                continue
            if end + 2 >= len(lines):
                break
            update = assignment.fullmatch(lines[end + 1])
            if not update or lines[end + 2] != indent + 'end\n':
                break
            operands = [test[1], test[2], update[2], update[3]]
            if any(value != name and not integer(value) for value in operands):
                break
            target = update[1]
            if target == '_ENV':
                break
            if target != name:
                if flow is None:
                    flow = Flow(lines)
                if not flow.ok or declarations[target] != 1 or not _prefix_bindings_are_stable(flow, 'return ' + target, start):
                    break
            aliases.add(target)
            end += 3
            groups += 1
        if not groups:
            continue
        outside = [text for i, text in enumerate(structure) if not start <= i < end
                   and not re.fullmatch(r'\s*local [\w, ]+\s*', text)]
        if any(re.search(r'\b' + re.escape(alias) + r'\b', '\n'.join(outside)) for alias in aliases):
            continue
        lines[start:end] = []
        for alias in sorted(aliases):
            _remove_declaration(lines, alias)
        return lines, groups + 1
    return lines, 0


def fold_blocking_speech(lines):
    """ForgeFSE's `thing:Speak(...)` is Speak_Blocking: it runs the retail wait loop itself (NewScriptFrame until
    IsPerformingScriptTask clears) and returns false when the thread terminates meanwhile. The draft's own
    `while R:IsPerformingScriptTask() do <frame check> end` after the call is therefore that call's result:
    `if not R:Speak(...) then EXIT end`."""
    count = 0
    i = 0
    while i < len(lines) - 2:
        m = re.fullmatch(r'(\s*)(\w+):Speak\((.*)\)\n', lines[i])
        w = re.fullmatch(r'(\s*)(\w+):IsPerformingScriptTask\(\) do\n', lines[i + 1].replace('while ', '', 1)) if m else None
        if not m or not w or w.group(1) != m.group(1) or w.group(2) != m.group(2):
            i += 1
            continue
        indent = m.group(1)
        end = next((j for j in range(i + 2, len(lines)) if lines[j] == indent + 'end\n'), None)
        if end is None:
            i += 1
            continue
        body = lines[i + 2:end]
        call = f'{m.group(2)}:Speak({m.group(3)})'
        one = re.fullmatch(re.escape(indent) + r'    if not quest:NewScriptFrame\((?:me)?\) then (' + EXIT + r') end\n', body[0]) if len(body) == 1 else None
        if one:
            new = [f'{indent}if not {call} then {one.group(1)} end\n']
        elif len(body) == 1 and re.fullmatch(re.escape(indent) + r'    quest:NewScriptFrame\((?:me)?\)\n', body[0]):
            new = [f'{indent}{call}\n']
        elif len(body) >= 3 and re.fullmatch(re.escape(indent) + r'    quest:NewScriptFrame\((?:me)?\)\n', body[0]) \
                and body[1] == f'{indent}    if quest:IsActiveThreadTerminating() then\n' and body[-1] == f'{indent}    end\n' \
                and all(l.startswith(indent + '        ') for l in body[2:-1]):
            new = [f'{indent}if not {call} then\n'] + [l[4:] for l in body[2:-1]] + [f'{indent}end\n']
        else:
            i += 1
            continue
        lines[i:end + 1] = new
        count += 1
        i += len(new)
    return lines, count


def _indent(line):
    return len(line) - len(line.lstrip(' '))


PLAIN_TAIL = re.compile(r'::\w+::|[\w.:]+\([^\n]*\)|\w+ = [^\n]+|return|__cleanup_\w+\(\)')
BLOCK_OPENER = re.compile(r'(?:if .+ then|elseif .+ then|else|while .+ do|for .+ do|repeat|do)$')


def _exit_path(lines, end_idx, level):
    """The statements run after the `end` at end_idx (indent `level`) until the function returns, when that
    path only falls out of if-blocks through plain statements / labels (never around a loop). None otherwise."""
    j, tail = end_idx + 1, []
    while True:
        while j < len(lines) and lines[j].strip() and _indent(lines[j]) >= level:
            if _indent(lines[j]) > level or not PLAIN_TAIL.fullmatch(lines[j].strip()):
                return None
            tail.append(lines[j].strip())
            j += 1
            if tail[-1] == 'return':
                return tail
        if j >= len(lines) or not lines[j].strip():
            return None
        closer, ci = lines[j].strip(), _indent(lines[j])
        if ci != level - 4:
            return None
        if ci == 0:
            return tail if closer == 'end' else None          # the function's own end
        if closer == 'end' or closer.startswith('until '):
            k = j - 1
            while k >= 0 and not (_indent(lines[k]) == ci and lines[k].strip()):
                k -= 1
            if k < 0 or not BLOCK_OPENER.fullmatch(lines[k].strip()):
                return None
            if closer.startswith('until ') or lines[k].strip().startswith(('while ', 'for ')):
                return ('continue', j, tail)                  # falling out of a loop body: the next iteration
            if lines[k].strip() == 'do':
                return None
        elif closer == 'else' or closer.startswith('elseif '):
            j = next((x for x in range(j + 1, len(lines)) if _indent(lines[x]) == ci and lines[x].strip() == 'end'), None)
            if j is None:
                return None
        else:
            return None
        level = ci
        j += 1


def count_labels(lines):
    """The next free continue-label number of the function."""
    used = [int(m.group(1)) for l in lines for m in [re.fullmatch(r'\s*::continue_(\d+)::\n', l)] if m]
    return max(used, default=0) + 1


def flatten_tail_guards(lines):
    """`if C then <the rest> end` whose fall-through only runs plain exit tails (`::LAB::` + cleanup + `return`,
    or out through enclosing if-blocks to the function's end) is an early exit in a quest script:
    `if not C then goto LAB end` / `if not C then <cleanup>; return end` with the body dedented. Retail's
    `if (!IsActiveThreadTerminating()) { ... }` chains (TheRealGuildmaster: 130 columns) flatten this way."""
    count = 0
    i = 0
    while i < len(lines):
        m = re.fullmatch(r'(\s*)if (.+) then\n', lines[i])
        if not m:
            i += 1
            continue
        indent, cond = m.group(1), m.group(2)
        end = next((j for j in range(i + 1, len(lines)) if lines[j] == indent + 'end\n'), None)
        if end is None or any(re.fullmatch(re.escape(indent) + r'(?:else|elseif .+)\n', lines[j]) for j in range(i + 1, end)):
            i += 1
            continue
        path = _exit_path(lines, end, len(indent))
        if path is None:
            i += 1
            continue
        body = [l[4:] if l.startswith(indent + '    ') else l for l in lines[i + 1:end]]
        if not any(l.strip() and not l.strip().startswith('--') for l in body):
            i += 1
            continue
        if isinstance(path, tuple):
            # the fall-through is the loop's next iteration: `goto continue_N` with the label at the loop's end
            _, closer_idx, tail = path
            existing = re.fullmatch(r'\s*::(continue_\d+)::\n', lines[closer_idx - 1])
            label_name = existing.group(1) if existing else f'continue_{count_labels(lines)}'
            prefix = [x for x in tail if not x.startswith('::')]
            if len(prefix) > 3 or any(x.startswith('::') for x in tail):
                i += 1
                continue
            stmts = prefix + [f'goto {label_name}']
            pending_label = None if existing else (closer_idx, label_name)
        else:
            pending_label = None
            label = next((x for x in path if x.startswith('::')), None)
            prefix = path[:path.index(label)] if label else [x for x in path if x != 'return']
            if len(prefix) > 3:
                i += 1
                continue
            stmts = prefix + ([f'goto {label[2:-2]}'] if label else ['return'])
        head = f'{indent}if {_negate(cond)} then {"; ".join(stmts)} end\n'
        statements = sum(1 for l in body if l.strip() and not l.strip().startswith(('--', '::')))
        if statements < 2 * max(1, len([x for x in stmts if not x.startswith('goto')])) and statements < 4:
            i += 1
            continue                                   # a short exit body is the readable form already
        follows = pending_label or (end + 1 < len(lines) and lines[end + 1].strip() and _indent(lines[end + 1]) >= len(indent))
        if follows and re.fullmatch(r'(?:.*; )?return\n', body[-1].lstrip()) and body[-1].startswith(indent) and body[-1][len(indent)] != ' ':
            body[-1] = body[-1][:-len('return\n')] + 'do return end\n'      # (a return is only legal last in its block)
        if pending_label:
            closer_idx, label_name = pending_label
            lines.insert(closer_idx, ' ' * (_indent(lines[closer_idx]) + 4) + f'::{label_name}::\n')
        lines[i:end + 1] = [head] + body
        count += 1
        i += 1
    return lines, count


def prune_self_assignments(lines):
    count = 0
    for i, l in enumerate(lines):
        if re.fullmatch(r'\s*(\w+) = \1\n', l):
            lines[i] = ''
            count += 1
    return lines, count


def cosmetics(lines, function_indent='    '):
    count = 0
    source = ''.join(lines)
    masked = _constant_condition_structure(source)
    offset = 0
    for i, line in enumerate(lines):
        code = masked[offset:offset + len(line)]
        offset += len(line)
        edits = []
        for pattern, replacement in ((r' \+ -(\d+(?:\.\d+)?)\b', r' - \1'),
                                      (r'\(#(\w+) \* 12\) / 12', r'#\1')):
            for match in re.finditer(pattern, code):
                edits.append((match.start(), match.end(), match.expand(replacement)))
        new = line
        for a, b, value in sorted(edits, reverse=True):
            new = new[:a] + value + new[b:]
        if new != line:
            lines[i] = new
            count += 1
    # A local assignment consumes exactly one value, including function calls
    # and varargs. Parentheses retain that arity when removing the local.
    masked = _constant_condition_structure(''.join(lines))
    offset, structure = 0, []
    for line in lines:
        structure.append(masked[offset:offset + len(line)])
        offset += len(line)
    for i in range(len(lines) - 1):
        code = structure[i]
        a = re.fullmatch(r'(\s*)local (\w+) = ([^\n]+)\n', lines[i])
        if a and re.match(r'\s*local \w+ = ', code) and lines[i + 1] == f'{a.group(1)}return {a.group(2)}\n':
            if _has_expression_list(a.group(3)):
                continue
            if any(t.lastgroup in ('comment', 'longcomment') for t in tokens(a.group(3))):
                continue
            lines[i] = ''
            lines[i + 1] = f'{a.group(1)}return ({a.group(3)})\n'
            count += 1
    # Keep trailing-return cleanup conservative around opaque text.
    if not any(t.lastgroup in ('string', 'longstring', 'longcomment') and '\n' in t[0] for t in tokens(''.join(lines))):
        last = next((j for j in range(len(lines) - 1, -1, -1) if lines[j].strip()), None)
        if last is not None and lines[last] == 'end\n':
            prev = next((j for j in range(last - 1, -1, -1) if lines[j].strip()), None)
            if prev is not None and lines[prev] == function_indent + 'return\n':
                lines[prev] = ''
                count += 1
    return lines, count


def tidy_blank_lines(text):
    """Never treat whitespace inside opaque Lua tokens as formatting."""
    if any(t.lastgroup in ('string', 'longstring', 'longcomment') and '\n' in t[0] for t in tokens(text)):
        return text
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
            flow = Flow(lines)
            if all(_pure_line(s, raw, pure, flow, i) for i, (s, raw) in enumerate(zip(structure[1:], lines[1:]), 1)):
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
    run('closures', tidy_closures)
    run('hero', hoist_hero)
    run('cutscenes', fold_cutscenes)

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
        total += run('copies', propagate_copies)
        total += run('selfAssignments', prune_self_assignments)
        total += run('flagClears', fold_flag_clears)
        total += run('deadBitmaskStaging', prune_dead_bitmask_staging)
        total += run('tailGuards', flatten_tail_guards)
        total += run('blockingSpeech', fold_blocking_speech)
        total += run('uintFixups', fold_uint_fixups)
        total += run('freshGuards', simplify_fresh_guards)
        total += run('constantConditions', fold_constant_conditions)
        total += run('emptyElse', prune_empty_else)
        total += run('unusedClosures', prune_unused_closures)
        total += run('sharedExitTests', fold_shared_exit_tests)
        total += run('gotoElse', fold_goto_else)
        total += run('booleanBranches', fold_boolean_branches)
        total += run('booleanJumpChains', fold_boolean_jump_chains)
        total += run('elseif', fold_elseif)
        total += run('elseExit', fold_else_exit)
        total += run('gotoReturn', fold_goto_return)
        total += run('nestedReturnJumps', fold_nested_return_jumps)
        total += run('loopExitJumps', fold_loop_exit_jumps)
        total += run('cleanupExitJumps', fold_cleanup_exit_jumps)
        total += run('ifAroundWhile', fold_if_around_while)
        return total

    for _ in range(3):
        for _ in range(20):
            if not folds():
                break
        if not run('deadTerminationChecks', prune_dead_termination_checks, pure_functions):
            break
    if run('controlAcquires', fold_control_acquires):
        for _ in range(20):
            if not folds():
                break
    # after every fold that can introduce a `goto`: the jump-into-scope check must see the final jumps
    for _ in range(40):
        if not run('sunkLocals', sink_hoisted_locals):
            break
    run('cleanupClosureNames', name_cleanup_closures)
    run('parentheses', strip_redundant_parens)
    run('cosmetics', cosmetics)
    run('firstAssignments', merge_first_assignments)
    run('freeSuffixes', drop_free_suffixes)
    # Final cleanup: do not let removed dead edges trigger new executable folds.
    run('unreachableReturnJumps', prune_unreachable_return_jumps)
    return ''.join(lines), stats


RE_HOISTED_CLOSURE = re.compile(r'^(?P<ind>[ \t]*)local function (?P<name>__(?:cleanup|region)_LAB_\w+)\(\)\n')


def _cleanup_name(body):
    """A name for what the epilogue does, in the vocabulary of the script rather than of the jump target."""
    verbs = {re.sub(r'\(.*', '', line).split(':')[-1] for line in body}
    if verbs <= {'PauseAllNonScriptedEntities'}:
        return 'ResumeEntities'
    if verbs <= {'DeregisterTimer'}:
        return 'DeregisterTimers'
    if 'EndCutscene' in verbs or 'RunCutscene' in verbs:
        return 'EndCutsceneAndRelease'
    if verbs <= {'ReleaseResource'}:
        return 'ReleaseControl'
    return 'ReleaseEverything'


def name_cleanup_closures(lines):
    """`local function __region_LAB_00d555f3_c27()` says where retail jumped, not what runs. Name it after what
    it does, and merge the closures whose bodies are identical - a function hoists the same one-line epilogue
    once per jump site, and five `ResumeEntities` definitions would read worse than the labels did."""
    order = []
    for i, line in enumerate(lines):
        m = RE_HOISTED_CLOSURE.match(line)
        if not m:
            continue
        end = next((j for j in range(i + 1, len(lines)) if lines[j].rstrip('\n') == m.group('ind') + 'end'), None)
        if end is None:
            return lines, 0
        order.append((m.group('name'), i, end, tuple(l.strip() for l in lines[i + 1:end] if l.strip())))
    if not order:
        return lines, 0
    occupied = set(re.findall(r'[A-Za-z_]\w*', ''.join(lines)))
    chosen, by_body, drop = {}, {}, set()
    for name, i, end, body in order:
        if body in by_body:                       # an identical epilogue: one definition serves every site
            chosen[name] = by_body[body]
            drop.update(range(i, end + 1))
            continue
        base = _cleanup_name(body)
        pick, n = base, 2
        while pick in occupied:
            pick, n = f'{base}{n}', n + 1
        occupied.add(pick)
        by_body[body] = chosen[name] = pick
    text = ''.join(l for k, l in enumerate(lines) if k not in drop)
    for name, pick in chosen.items():
        text = re.sub(r'\b' + re.escape(name) + r'\b', pick, text)
    return text.splitlines(keepends=True), len(chosen)


RE_ACQUIRE_LOOP = re.compile(
    r'^(?P<ind>[ \t]*)while not resources:TryAcquire\((?P<R>\w+), (?P<T>\w+), (?P<P>\d+)\) do\n'
    r'(?P=ind)    if not quest:NewScriptFrame\((?:me)?\) then (?P<clean>[^\n]+?) end\n'
    r'(?P=ind)end\n', re.M)
RE_ACQUIRE_STMT = re.compile(r'^(?P<ind>[ \t]*)(?P<lhs>\w+ = )?resources:TryAcquire\((?P<R>\w+), (?P<T>\w+), (?P<P>\d+)\)\n', re.M)


def fold_control_acquires(lines):
    """`R = resources:NewResource(); while not resources:TryAcquire(R, T, P) do if not NewScriptFrame() then
    ... end end` is ForgeFSE's `T:AcquireControl(P)` (LuaEntityAPI::AcquireControl runs exactly that retry loop
    and returns false when the thread terminates meanwhile), and `resources:ReleaseResource(R)` is
    `T:ReleaseControl()`. A resource folds only when every acquisition of it names the same thing and nothing
    else reads it; a resource that is also an actor in a cutscene stays (fold_cutscenes owns those)."""
    text = ''.join(lines)
    if 'resources:TryAcquire(' not in text:
        return lines, 0
    targets = {}
    for m in RE_ACQUIRE_STMT.finditer(text):
        targets.setdefault(m.group('R'), set()).add(m.group('T'))
    for m in RE_ACQUIRE_LOOP.finditer(text):
        targets.setdefault(m.group('R'), set()).add(m.group('T'))
    foldable = set()
    for R, things in targets.items():
        if len(things) != 1 or R == '0':
            continue
        rest = RE_ACQUIRE_LOOP.sub('', text)
        rest = RE_ACQUIRE_STMT.sub('', rest)
        rest = re.sub(r'^[ \t]*(?:local )?' + re.escape(R) + r' = resources:NewResource\(\)\n', '', rest, flags=re.M)
        rest = re.sub(r'resources:ReleaseResource\(' + re.escape(R) + r'\)', '', rest)
        rest = re.sub(r'resources:ScriptThing\(' + re.escape(R) + r'\)', '', rest)     # the resource's thing is the acquired one
        rest = re.sub(r'^\s*local [\w, ]+\n', '', rest, flags=re.M)
        if not re.search(r'\b' + re.escape(R) + r'\b', rest):
            foldable.add(R)
    foldable.add('0')                                   # the lifter lost the resource operand: the thing is what matters
    if not foldable:
        return lines, 0
    count = 0

    def loop(m):
        nonlocal count
        if m.group('R') not in foldable:
            return m.group(0)
        count += 1
        clean = re.sub(r'resources:ReleaseResource\(' + re.escape(m.group('R')) + r'\); ?', '', m.group('clean')).strip()
        return f"{m.group('ind')}if not {m.group('T')}:AcquireControl({m.group('P')}) then {clean} end\n"

    def stmt(m):
        nonlocal count
        if m.group('R') not in foldable:
            return m.group(0)
        count += 1
        return f"{m.group('ind')}{m.group('lhs') or ''}{m.group('T')}:AcquireControl({m.group('P')})\n"
    new = RE_ACQUIRE_LOOP.sub(loop, text)
    new = RE_ACQUIRE_STMT.sub(stmt, new)
    gone = []
    for R in foldable - {'0'}:
        T = next(iter(targets[R]))
        new = re.sub(r'^[ \t]*(?:local )?' + re.escape(R) + r' = resources:NewResource\(\)\n', '', new, flags=re.M)
        new = re.sub(r'resources:ReleaseResource\(' + re.escape(R) + r'\)', f'{T}:ReleaseControl()', new)
        new = re.sub(r'resources:ScriptThing\(' + re.escape(R) + r'\)', T, new)
        gone.append(R)
    if 'resources:' not in new:
        new = re.sub(r'^\s*local resources = quest:RetailResources\(\)\n', '', new, flags=re.M)
    lines = new.splitlines(keepends=True)
    for R in gone:
        _remove_declaration(lines, R)
    return lines, count


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


def name_script_def_reads(source):
    """`quest:ReadGlobalGameData*(0xf10)` reads a `CScriptDef` field (the global game data, script.bin SCRIPT_DEF):
    the offsets become a `SCRIPT_DEF` table at the top of the file, each entry with the retail value as a comment
    (Aeon's ports name these constants; the table is refs/script_recovery/script_def_offsets.json)."""
    try:
        from tools.script_recovery.script_def_offsets import load
        table = load()
    except Exception:
        table = {}
    if not table:
        return source, 0
    used = {}
    def repl(m):
        off = int(m.group(2), 0)
        field = table.get(f'{off:#x}')
        if not field or field.get('verified') is False:     # (an offset in the unproven middle of the layout keeps its number)
            return m.group(0)
        used[field['name']] = (off, field.get('value'))
        return f'{m.group(1)}(SCRIPT_DEF.{field["name"]}'
    new = re.sub(r'(quest:ReadGlobalGameData(?:Float|FloatAt|String)?)\((0x[0-9a-f]+|\d+)', repl, source)
    if not used:
        return source, 0
    rows = ''.join(f'    {name} = {off},{"  -- " + repr(value) if value is not None else ""}\n' for name, (off, value) in sorted(used.items(), key=lambda kv: kv[1][0]))
    block = '-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)\nlocal SCRIPT_DEF = {\n' + rows + '}\n'
    header_end = re.search(r'^(?!--)', new, re.M).start()
    new = new[:header_end] + '\n' + block + new[header_end:] if new[:header_end].endswith('\n') else block + new
    return new, len(used)


def name_enum_operands(source):
    """`me:MoveToPosition(pos, 3.0, 1, false, true)` -> `ENTITY_MOVE_RUN`: retail typed that operand, so the
    readable output should spell it the way retail did. Values come from the Ego_r PDB
    (`tools/script_recovery/retail_enums.py`), and only a bare integer literal in the enum's range is
    touched - anything else (a variable, an out-of-range number) is left exactly as it was."""
    from tools.script_recovery.retail_enums import OPERANDS, members, split_arguments
    used = {}

    def rewrite(source):
        out, changed = [], 0
        pos = 0
        for m in re.finditer(r'(\w+):(\w+)\(', source):
            if m.start() < pos:
                continue
            rules = [(i, e) for name, i, e in OPERANDS if name == m.group(2)]
            if not rules:
                continue
            depth, j = 1, m.end()
            while j < len(source) and depth:
                depth += source[j] in '([{'
                depth -= source[j] in ')]}'
                j += 1
            if depth:
                continue
            inner = source[m.end():j - 1]
            if '\n' in inner:
                continue                         # a wrapped call: leave it to the next round
            spans = split_arguments(inner)
            replaced = inner
            for index, enum in rules:
                if index >= len(spans):
                    continue
                start, stop = spans[index]
                literal = inner[start:stop].strip()
                if not re.fullmatch(r'\d+', literal):
                    continue
                name = members(enum).get(int(literal))
                if not name:
                    continue
                used[name] = (enum, int(literal))
                replaced = replaced[:start] + replaced[start:stop].replace(literal, name, 1) + replaced[stop:]
                changed += 1
            out.append(source[pos:m.end()] + replaced + ')')
            pos = j
        out.append(source[pos:])
        return ''.join(out), changed

    source, changed = rewrite(source)
    if not used:
        return source, 0
    by_enum = {}
    for name, (enum, value) in used.items():
        by_enum.setdefault(enum, []).append((value, name))
    rows = ''.join(f'local {name} = {value}  -- {enum} (Ego_r.pdb)\n'
                   for enum in sorted(by_enum) for value, name in sorted(by_enum[enum]))
    header_end = re.search(r'^(?!--)', source, re.M).start()
    source = source[:header_end] + '\n' + rows + source[header_end:] if source[:header_end].endswith('\n') else rows + source
    return source, changed


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


# ---------------------------------------------------------------- step 3: named state, step 5: helper names

STATE_SHIM = re.compile(
    r'local __native_entity_state = \{\}\ndo\n    local fields = \{\}\n'
    r'    for _, kind in ipairs\(\{"Bool", "Int", "Float", "String", "Thing"\}\) do\n'
    r'        __native_entity_state\["GetState" \.\. kind\] = function\(_, name\) return fields\[name\] end\n'
    r'        __native_entity_state\["SetState" \.\. kind\] = function\(_, name, value\) fields\[name\] = value end\n'
    r'    end\nend\n')


def _entity_code(source):
    return [token for token in tokens(source) if token.lastgroup not in ('space', 'comment', 'longcomment')]


def _entity_shim(source, pattern):
    starts = {token.start(): token for token in _entity_code(source)}
    matches = [match for match in pattern.finditer(source)
               if match.start() in starts and starts[match.start()][0] == 'local']
    return matches[0] if len(matches) == 1 else None


def _entity_prefixes(source, receiver, shim, *, state_word):
    stream = _entity_code(source)
    pattern = re.compile(re.escape(receiver) + r':(?P<action>Get|Set)' + state_word
                         + r'(?P<kind>Bool|Int|Float|String|Thing)\(')
    result = []
    for index, token in enumerate(stream):
        if token.lastgroup != 'identifier' or token[0] != receiver:
            continue
        if shim.start() <= token.start() < shim.end():
            continue
        previous = stream[index - 1][0] if index else ''
        match = pattern.match(source, token.start())
        if token.start() < shim.end() or previous in ('.', ':', 'function') or not match:
            return None  # escaped/shadowed/qualified/unsupported receiver
        result.append((match, index))
    return stream, result


def _entity_edits(source, edits):
    for begin, end, replacement in sorted(edits, reverse=True):
        source = source[:begin] + replacement + source[end:]
    return source


def alias_entity_state(source):
    """Normalize real primitive calls; opaque text and unsupported receiver uses stay intact."""
    shim = _entity_shim(source, STATE_SHIM)
    # Retain the existing conservative name-collision policy in this task.
    if not shim or re.search(r'\bstate\b', rename_identifiers(source, {'__native_entity_state': 'x'})):
        return source, 0
    found = _entity_prefixes(source, '__native_entity_state', shim, state_word='State')
    if found is None:
        return source, 0
    _, calls = found
    replacement = ('local state = {}  -- per-entity script state (__native_entity_state)\n'
                   'do\n    local fields = {}\n'
                   '    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do\n'
                   '        state["Get" .. kind] = function(_, name) return fields[name] end\n'
                   '        state["Set" .. kind] = function(_, name, value) fields[name] = value end\n'
                   '    end\nend\n')
    edits = [(shim.start(), shim.end(), replacement)]
    edits.extend((match.start(), match.end(), 'state:' + match['action'] + match['kind'] + '(')
                 for match, _ in calls)
    return _entity_edits(source, edits), len(calls)


def _entity_literal_calls(source, shim):
    found = _entity_prefixes(source, 'state', shim, state_word='')
    if found is None:
        return None
    stream, prefixes = found
    calls = []
    for match, position in prefixes:
        opening = position + 3
        if opening >= len(stream) or stream[opening][0] != '(':
            return None
        stack, commas, closing = ['('], [], None
        for index in range(opening + 1, len(stream)):
            token = stream[index]
            value = token[0]
            if token.lastgroup in ('string', 'longstring'):
                continue
            if value == 'function':
                return None
            if value in ('(', '[', '{'):
                stack.append(value)
            elif value in (')', ']', '}'):
                if not stack or stack[-1] != {')': '(', ']': '[', '}': '{'}[value]:
                    return None
                stack.pop()
                if not stack:
                    closing = index
                    break
            elif value == ',' and len(stack) == 1:
                commas.append(index)
        if closing is None:
            return None
        action = match['action']
        expected_commas = 0 if action == 'Get' else 1
        if len(commas) != expected_commas:
            return None
        key_end = closing if not commas else commas[0]
        key_tokens = stream[opening + 1:key_end]
        if len(key_tokens) != 1 or key_tokens[0].lastgroup != 'string':
            return None
        key = re.fullmatch(r'"(\w+)"', key_tokens[0][0])
        if not key:
            return None
        previous = stream[position - 1] if position else None
        expression_prefixes = ('=', '(', '[', '{', ',', '+', '-', '*', '/', '//', '%', '^', '..',
                               '==', '~=', '<', '>', '<=', '>=', '&', '|', '~', '<<', '>>', '#',
                               'and', 'or', 'not', 'return', 'if', 'elseif', 'while', 'until', 'in')
        following = stream[closing + 1] if closing + 1 < len(stream) else None
        suffix = following is not None and (following[0] in ('(', '[', '{', '.', ':')
                                             or following.lastgroup in ('string', 'longstring'))
        if action == 'Get':
            if not suffix and (previous is None or previous[0] not in expression_prefixes):
                return None  # a discarded getter call cannot become a bare variable statement
        else:
            if commas[0] + 1 == closing:
                return None
            if suffix:
                return None  # preserve chained calls on the setter's nil result
            same_line = previous is not None and '\n' not in source[previous.end():match.start()]
            statement_start = previous is None or previous[0] in ('then', 'do', 'else', 'repeat', ';', '::')
            if not statement_start:
                statement_start = not same_line and previous[0] not in expression_prefixes
            if not statement_start:
                return None  # setter return value/argument evaluation must be preserved
        calls.append(dict(match=match, key=key[1], action=action,
                          end=stream[closing].end(), value=stream[commas[0] + 1].start() if commas else None))
    return calls


def inline_entity_fields(source):
    """Inline supported real field calls, preserving opaque text and receiver scope."""
    from tools.script_recovery.readable_lua import camel
    original = source
    pattern = re.compile(r'^local state = \{\}  -- per-entity script state \(__native_entity_state\)\n(?:.*\n)*?^end\n', re.M)
    shim = _entity_shim(source, pattern)
    if not shim:
        return source, 0
    calls = _entity_literal_calls(source, shim)
    if calls is None:
        return original, 0
    keys = list(dict.fromkeys(call['key'] for call in calls))
    if not keys:
        return source[:shim.start()] + source[shim.end():], 1
    # Keep function-local snapshots. A helper or yielded callback may write the
    # field later; replacing the snapshot with a live field read changes values.
    # Retaining every local also avoids freeing a name still bound in another scope.
    taken = {token[0] for token in tokens(source) if token.lastgroup == 'identifier'}
    names = {}
    for key in keys:
        name = camel(key)
        while name in taken or name in names.values() or name in KEYWORDS_ALL:
            name += '_'
        names[key] = name
    edits = []
    for call in calls:
        match = call['match']
        if call['action'] == 'Get':
            edits.append((match.start(), call['end'], names[call['key']]))
        else:
            edits.append((match.start(), call['value'], names[call['key']] + ' = ('))
    block = '-- per-entity fields (native class members; one Lua state per entity instance)\nlocal ' + ', '.join(names[key] for key in keys) + '\n'
    edits.append((shim.start(), shim.end(), block))
    new = _entity_edits(source, edits)
    code_starts = {token.start() for token in _entity_code(new) if token.lastgroup == 'identifier'}
    cosmetic = re.compile(r'^(?P<indent>[ \t]*)(?P<lhs>\w+ = )\((?P<value>[^()\n]*(?:\([^()\n]*\)[^()\n]*)*)\)$', re.M)
    edits = [(match.start(), match.end(), match['indent'] + match['lhs'] + match['value'])
             for match in cosmetic.finditer(new) if match.start('lhs') in code_starts]
    return _entity_edits(new, edits), len(keys)



def state_writers(sources):
    """{(receiver kind, key): {function names}} for every literal SetState*("Key") in the unit
    (receiver kind: 'entity' per file — keyed by file — or 'quest' unit-wide)."""
    writers = {}
    for rel, text in sources.items():
        for chunk in _function_chunks(text):
            fm = re.match(r'(?:local )?function (\w+)', chunk)
            if not fm:
                continue
            for recv, key in re.findall(r'\b(__native_entity_state|state|quest):SetState\w+\("(\w+)"', chunk):
                scope = ('quest', None) if recv == 'quest' else ('entity', rel)
                writers.setdefault((scope, key), set()).add(fm.group(1))
    return writers


def hoist_invariant_state(chunk, rel, writers, *, min_uses=2):
    """State keys written only by Init: a function reading one ≥ min_uses times gets
    `local <key> = <read>` after its declarations."""
    fm = re.match(r'(?:local )?function (\w+)\(([^)]*)\)', chunk)
    if not fm or fm.group(1) == 'OnPersist':
        return chunk, 0
    fname = fm.group(1)
    reads = re.findall(r'\b((?:__native_entity_state|state|quest):(?:Get|GetState)(?:Bool|Int|Float|String|Thing)\("(\w+)"\))', chunk)
    counts = {}
    for expr, key in reads:
        counts.setdefault(expr, [key, 0])[1] += 1
    occupied = {t[0] for t in tokens(chunk) if t.lastgroup == 'identifier'}
    hoists = []
    for expr, (key, n) in counts.items():
        if n < min_uses:
            continue
        scope = ('quest', None) if expr.startswith('quest:') else ('entity', rel)
        owners = writers.get((scope, key), set())
        if any(w != 'Init' for w in owners) or fname in owners:
            continue
        if re.search(r'\b(?:__native_entity_state|state|quest):SetState\w+\("' + key + r'"', chunk):
            continue
        name = key[0].lower() + key[1:]
        if name == 'iD':
            name = 'id'
        name = re.sub(r'ID$', 'Id', name)
        if name in occupied or name in KEYWORDS:
            continue
        occupied.add(name)
        hoists.append((expr, name))
    if not hoists:
        return chunk, 0
    lines = chunk.splitlines(keepends=True)
    # insertion point: after the header, declaration-only locals, the resources local and the shim closures
    at = 1
    while at < len(lines) and (re.fullmatch(r'    local [\w, ]+\n', lines[at]) or re.fullmatch(r'    local (?:resources|helpers) = [^\n]+\n', lines[at])):
        at += 1
    body = ''.join(lines[at:])
    for expr, name in hoists:
        body = body.replace(expr, name)
    decls = ''.join(f'    local {name} = {expr}\n' for expr, name in hoists)
    return ''.join(lines[:at]) + decls + body, len(hoists)


def name_helpers_by_state(source):
    """`helper_XXXX(quest, me, p)` whose body stores p into one state key -> `Set<Key>` (same file)."""
    count = 0
    for m in re.finditer(r'^function (helper_[0-9A-Fa-f]+)\(([^)]*)\)\n([\s\S]*?)^end\n', source, re.M):
        old, params, body = m.group(1), m.group(2), m.group(3)
        plist = [x.strip() for x in params.split(',')]
        if len(plist) != 3:
            continue
        p = plist[2]
        stores = re.findall(r'\b(?:state|__native_entity_state|quest):(?:SetState|Set)\w+\("(\w+)", ' + re.escape(p) + r'\)', body)
        if len(stores) != 1:
            continue
        new = 'Set' + stores[0]
        if re.search(r'\b' + new + r'\b', source):
            continue
        source = rename_identifiers(source, {old: new})
        source = source.replace(f'function {new}(', f'-- helper 0x{old[7:].upper()} (named after the state it writes)\nfunction {new}(', 1)
        count += 1
    return source, count


def name_helpers_by_shape(source):
    """`helper_XXXX` whose body is one recognisable idiom gets the idiom's name: a cutscene wrapper
    (`StartCutscene / RunCutscene / EndCutscene`) is `PlayCutscene`, `PlayHeroCutscene` when the hero is its
    only actor."""
    count = 0
    for m in re.finditer(r'^function (helper_[0-9A-Fa-f]+)\(([^)]*)\)\n([\s\S]*?)^end\n', source, re.M):
        old, body = m.group(1), m.group(3)
        stmts = [l.strip() for l in body.splitlines() if l.strip()]
        if len(stmts) == 3 and stmts[0].startswith('quest:StartCutscene(') and stmts[1].startswith('quest:RunCutscene(') and stmts[2] == 'quest:EndCutscene()':
            new = 'PlayHeroCutscene' if re.match(r'quest:StartCutscene\(\{HERO = (?:hero|quest:GetHero\(\))\}', stmts[0]) else 'PlayCutscene'
        else:
            continue
        if re.search(r'\b' + new + r'\b', source):
            continue
        source = rename_identifiers(source, {old: new})
        source = source.replace(f'function {new}(', f'-- helper 0x{old[7:].upper()} (named after its shape)\nfunction {new}(', 1)
        count += 1
    return source, count


GROUP_SELECT = ['GROUP_SELECT_FIRST', 'GROUP_SELECT_RANDOM', 'GROUP_SELECT_RANDOM_NO_REPEAT', 'GROUP_SELECT_SEQUENTIAL', 'GROUP_SELECT_NONE']


def name_speak_methods(source):
    """`thing:Speak(target, "KEY", 0, ...)`: the third operand is ETextGroupSelectionMethod (ForgeFSE
    EntityScriptingAPI.h: FIRST 0, RANDOM 1, RANDOM_NO_REPEAT 2, SEQUENTIAL 3, NONE 4) — named as in Aeon's ports,
    with the constants declared once at the top of the file."""
    used = set()

    def repl(m):
        n = int(m.group(2))
        if n >= len(GROUP_SELECT):
            return m.group(0)
        used.add(GROUP_SELECT[n])
        return f'{m.group(1)}{GROUP_SELECT[n]}'
    new = re.sub(r'(\b\w+:Speak\([^\n]*?, "[^"]*", )(\d)(?=,)', repl, source)
    if not used:
        return source, 0
    names = [n for n in GROUP_SELECT if n in used]
    block = 'local ' + ', '.join(names) + ' = ' + ', '.join(str(GROUP_SELECT.index(n)) for n in names) + '  -- ETextGroupSelectionMethod\n'
    header_end = re.search(r'^(?!--)', new, re.M).start()
    new = new[:header_end] + '\n' + block + new[header_end:] if new[:header_end].endswith('\n') else block + new
    return new, len(used)


def style_source(source, *, frame_returns_alive=True, rel=None, writers=None, frame_keep_check_indices=()):
    source, hoisted = hoist_requires(source)
    source, aliased = alias_entity_state(source)
    source, script_def = name_script_def_reads(source)
    source, enum_operands = name_enum_operands(source)
    source, speak_methods = name_speak_methods(source)
    source, named = name_helpers_by_state(source)
    source, named_shape = name_helpers_by_shape(source)
    named += named_shape
    pure = pure_local_functions(source)
    out, report = [], {}
    function_index = -1
    for part in _function_chunks(source):
        if not part.startswith(('function ', 'local function ')):
            out.append(part)
            continue
        function_index += 1
        name = re.match(r'(?:local )?function (\w+)', part).group(1)
        before = style_metrics(part)
        dispatch_evidence = []
        if rel is not None:
            from tools.script_recovery.readable_dispatch_scaffolding import prune_unused_dispatch_seeds
            path_parts = str(rel).replace('\\', '/').split('/')
            part, dispatch_evidence = prune_unused_dispatch_seeds(part, entity='Entities' in path_parts)
        styled, stats = style_function(part, pure, frame_returns_alive=frame_returns_alive and function_index not in frame_keep_check_indices)
        if dispatch_evidence:
            stats['unusedNativeDispatchSeeds'] = len(dispatch_evidence)
        if writers is not None:
            styled, k = hoist_invariant_state(styled, rel, writers)
            stats['hoistedState'] = k
        report[name] = {'rewrites': stats, 'before': before, 'after': style_metrics(styled)}
        if dispatch_evidence:
            report[name]['nativeDispatchSeeds'] = dispatch_evidence
        out.append(styled)
    text = tidy_blank_lines(''.join(out))
    text, late = name_helpers_by_shape(text)      # shapes appear once the bodies are styled (the cutscene fold)
    text, fields = inline_entity_fields(text)     # after styling: the copies of the fields are gone, their names free
    joined = 0
    while True:
        text, count = fold_literal_exit_join(text)
        if not count:
            break
        joined += count
    flags_joined = 0
    while True:
        text, count = fold_literal_flag_join(text)
        if not count:
            break
        flags_joined += count
    text, unused_labels = prune_unused_generated_labels(text)
    named += late
    return text, {'functions': report, 'hoistedRequires': hoisted, 'entityStateAliased': aliased, 'namedHelpers': named,
                  'pureLocalFunctions': sorted(pure), 'unusedGeneratedLabels': unused_labels,
                  'literalExitJoins': joined, 'literalFlagJoins': flags_joined}


def fold_literal_flag_join(source):
    """Structure a two-way literal flag join without moving its first write.

    The shared fallback store can enter the branches only when their prefix has
    no local declarations or nested control: closing a local there could observe
    the old flag. Keep the initial store before the inner predicate, and reject
    shadowed/out-of-scope bindings and alternate entries.
    """
    code = ''.join(re.sub(r'[^\n]', ' ', t[0])
                   if t.lastgroup in ('string', 'longstring', 'comment', 'longcomment')
                   else t[0] for t in tokens(source))
    pattern = re.compile(r'(?m)^(?P<ind> {4,})if (?P<outer>[^\n]+) then\n'
        r'(?P=ind)    goto (?P<label>\w+)\n(?P=ind)else\n'
        r'(?P<prefix>(?:(?P=ind)    [^\n]*\n)*?)'
        r'(?P=ind)    (?P<name>\w+) = (?P<initial>0|1|true|false)\n'
        r'(?P=ind)    if (?P<inner>[^\n]+) then goto (?P=label) end\n'
        r'(?P=ind)end\n(?P=ind)goto (?P<done>\w+)\n(?P=ind)::(?P=label)::\n'
        r'(?P=ind)(?P=name) = (?P<fallback>0|1|true|false)\n(?P=ind)::(?P=done)::\n')
    jumps = Counter(re.findall(r'\bgoto\s+(\w+)', code))
    labels = Counter(re.findall(r'::(\w+)::', code))
    declarations = list(re.finditer(r'(?m)^[ \t]*local ([\w, ]+)\n', code))
    control = {'local', 'if', 'then', 'else', 'elseif', 'end', 'function', 'for',
               'while', 'repeat', 'until', 'do', 'break', 'return', 'goto'}
    opposite = {'0': '1', '1': '0', 'true': 'false', 'false': 'true'}
    for match in pattern.finditer(source):
        indent, name = match['ind'], match['name']
        if name == '_ENV' or opposite[match['initial']] != match['fallback']:
            continue
        if code[match.start():match.start() + len(indent) + 3] != indent + 'if ':
            continue
        if match['label'] == match['done'] or jumps[match['label']] != 2 or jumps[match['done']] != 1:
            continue
        if labels[match['label']] != 1 or labels[match['done']] != 1:
            continue
        if any(code[match.start(key):match.end(key)] != match[key] for key in ('outer', 'inner')):
            continue
        straight = '\n'.join(code[match.start(key):match.end(key)] for key in ('prefix', 'outer', 'inner'))
        if '::' in straight or any(token[0] in control for token in tokens(straight)):
            continue
        bindings = [decl for decl in declarations if name in [n.strip() for n in decl[1].split(',')]]
        if len(bindings) != 1 or bindings[0].end() > match.start():
            continue
        binding = bindings[0]
        word = r'\b' + re.escape(name) + r'\b'
        # Conservatively reject other local/parameter/loop binding sites, even
        # when a local initializer merely reads this name.
        without_binding = code[:binding.start()] + code[binding.end():]
        if any(re.search(word, line) and
               (line.lstrip().startswith(('local ', 'for ')) or
                re.search(r'\bfunction[^\n(]*\([^)]*' + word, line))
               for line in without_binding.splitlines()):
            continue
        depth, enclosed = 0, True
        for token in tokens(code[binding.end():match.start()]):
            value = token[0]
            if value in ('end', 'until', 'else', 'elseif') and depth == 0:
                enclosed = False
                break
            if value in ('function', 'then', 'do', 'repeat'):
                depth += 1
            elif value in ('end', 'until', 'elseif'):
                depth -= 1
        if not enclosed:
            continue
        replacement = (indent + 'if ' + match['outer'] + ' then\n'
                       + indent + '    ' + name + ' = ' + match['fallback'] + '\n'
                       + indent + 'else\n' + match['prefix']
                       + indent + '    ' + name + ' = ' + match['initial'] + '\n'
                       + indent + '    if ' + match['inner'] + ' then ' + name + ' = ' + match['fallback'] + ' end\n'
                       + indent + 'end\n')
        return source[:match.start()] + replacement + source[match.end():], 1
    return source, 0


def fold_literal_exit_join(source):
    """Structure a closed two-jump join whose remaining path returns a literal.

    Both conditions run once, in their original order. The prefix keeps its
    scope; moving the straight-line inner body to the same branch preserves
    local close order. Only bare/nil/boolean exits may move before scope closure.
    Reject alternate entries, nested control flow and diagnostic-bearing edges.
    """
    code = ''.join(re.sub(r'[^\n]', ' ', t[0])
                   if t.lastgroup in ('string', 'longstring', 'comment', 'longcomment')
                   else t[0] for t in tokens(source))
    pattern = re.compile(r'(?m)^(?P<ind> {4,})if (?P<outer>[^\n]+) then\n'
        r'(?P=ind)    goto (?P<label>\w+)\n(?P=ind)else\n'
        r'(?P<prefix>(?:(?P=ind)    [^\n]*\n)*?)'
        r'(?P=ind)    if (?P<inner>[^\n]+) then\n'
        r'(?P<body>(?:(?P=ind)        [^\n]*\n)*?)'
        r'(?P=ind)        goto (?P=label)\n(?P=ind)    end\n(?P=ind)end\n'
        r'(?P=ind)do (?P<ret>return(?: (?:nil|true|false))?) end\n'
        r'(?P=ind)::(?P=label)::\n')
    jumps = Counter(re.findall(r'\bgoto\s+(\w+)', code))
    labels = Counter(re.findall(r'::(\w+)::', code))
    control = {'if', 'then', 'else', 'elseif', 'end', 'function', 'for', 'while',
               'repeat', 'until', 'do', 'break', 'return', 'goto'}

    def invert(condition):
        simple = re.fullmatch(r'not ([A-Za-z_]\w*)', condition)
        return simple[1] if simple else 'not (' + condition + ')'

    for match in pattern.finditer(source):
        indent = match['ind']
        if code[match.start():match.start() + len(indent) + 3] != indent + 'if ':
            continue
        if jumps[match['label']] != 2 or labels[match['label']] != 1:
            continue
        if any(code[match.start(name):match.end(name)] != match[name]
               for name in ('outer', 'inner', 'ret')):
            continue
        straight = '\n'.join(code[match.start(name):match.end(name)]
                             for name in ('prefix', 'body', 'outer', 'inner'))
        if '::' in straight or any(t[0] in control for t in tokens(straight)):
            continue
        body = ''.join(line[4:] for line in match['body'].splitlines(keepends=True))
        replacement = (indent + 'if ' + invert(match['outer']) + ' then\n' + match['prefix']
                       + indent + '    if ' + invert(match['inner']) + ' then ' + match['ret'] + ' end\n'
                       + body + indent + 'end\n')
        return source[:match.start()] + replacement + source[match.end():], 1
    return source, 0


def prune_unused_generated_labels(source):
    """Drop only unreferenced generated labels at the end of each style round.

    Scan the complete file so references outside a function chunk remain visible.
    Counting same-named jumps in other scopes is intentionally conservative.
    Keep user labels and labels carrying diagnostic comments.
    """
    code = ''.join(re.sub(r'[^\n]', ' ', t[0])
                   if t.lastgroup in ('string', 'longstring', 'comment', 'longcomment')
                   else t[0] for t in tokens(source))
    jumps = set(re.findall(r'\bgoto\s+(\w+)', code))
    lines = source.splitlines(keepends=True)
    count = 0
    for i, (raw, body) in enumerate(zip(lines, code.splitlines(keepends=True))):
        label = re.fullmatch(r'[ \t]*::((?:LAB_|FLOW_)\w+)::\n', raw)
        if label and raw == body and label[1] not in jumps:
            lines[i] = ''
            count += 1
    return ''.join(lines), count


def fold_else_exit(lines):
    """`if C then A else EXIT end` (a one-statement exit branch) -> `if not C then EXIT end` + A."""
    count = 0
    changed = True
    while changed:
        changed = False
        lines = [l for l in lines if l != '']
        for i, line in enumerate(lines):
            m = re.fullmatch(r'(\s*)if (.+) then\n', line)
            if not m:
                continue
            indent, cond = m.group(1), m.group(2)
            j = i + 1
            else_at = None
            while j < len(lines):
                l = lines[j]
                if l == indent + 'else\n':
                    else_at = j
                    break
                if l == indent + 'end\n' or l.startswith(indent + 'elseif ') or (l.strip() and not l.startswith(indent + ' ')):
                    break
                j += 1
            if else_at is None or else_at == i + 1 or else_at + 2 >= len(lines) or lines[else_at + 2] != indent + 'end\n':
                continue
            exit_line = lines[else_at + 1]
            if not re.fullmatch(re.escape(indent) + r'    (?:[^\n;]+; )?(?:return(?: [^\n]*)?|goto \w+|break)\n', exit_line):
                continue
            if any(t[0] in {'if', 'then', 'else', 'elseif', 'end', 'function', 'do', 'for', 'while', 'repeat', 'until'}
                   for t in tokens(exit_line) if t.lastgroup == 'identifier'):
                continue  # a conditional exit does not leave the else branch on every path
            body = lines[i + 1:else_at]
            if any(l.strip().startswith('::') for l in body):
                continue
            new = [f'{indent}if {_negate(cond)} then {exit_line.strip()} end\n'] + [b[4:] if b.startswith(indent + '    ') else b for b in body]
            lines[i:else_at + 3] = new
            count += 1
            changed = True
            break
    return lines, count


def fold_goto_return(lines):
    """`goto L` when `::L::` is followed only by a `return ...` at the function's tail -> that return."""
    count = 0
    lines = [l for l in lines if l != '']
    for i, line in enumerate(lines):
        m = re.fullmatch(r'    ::(\w+)::\n', line)
        if not m:
            continue
        rest = [l for l in lines[i + 1:] if l.strip()]
        if len(rest) != 2 or not re.fullmatch(r'    return(?: [^\n]*)?\n', rest[0]) or rest[1] != 'end\n':
            continue
        # A return expression evaluated at a jump site may bind to a different
        # local there. Only scope-independent returns can move without a lexical
        # binding proof (a shadowed result in a nested block is not the tail's result).
        if _line_reads(_structure_line(rest[0])):
            continue
        ret = rest[0].strip()
        label = m.group(1)
        for j, l in enumerate(lines):
            g = re.fullmatch(r'(\s*)goto ' + label + r'\n', l)
            if g:
                # (a return is only legal last in its block: a goto followed by more of its block -- a label,
                # TraderEscort MakeTraderComment's `goto FLOW_past_lab_00e01afb` before `::LAB_00e01afb::` -- keeps it)
                follows = next((x.strip() for x in lines[j + 1:] if x.strip()), 'end')
                last = re.match(r'(?:end|else|elseif |until )', follows) or follows == 'else'
                lines[j] = f'{g.group(1)}{ret}\n' if last else f'{g.group(1)}do {ret} end\n'
                count += 1
            else:
                new = re.sub(r'\bthen goto ' + label + r' end$', f'then {ret} end', l.rstrip('\n'))
                if new != l.rstrip('\n'):
                    lines[j] = new + '\n'
                    count += 1
        if not re.search(r'\bgoto ' + label + r'\b', ''.join(lines)):
            lines[i] = ''
    return lines, count


def fold_shared_exit_tests(lines):
    """Combine empty alternative or nested guards leading to the same jump.

    Short-circuit OR preserves the order and number of condition evaluations.
    This does not move the destination or duplicate any branch body.
    """
    # The emitter uses one statement per line. Decline text with multiline
    # literals/comments rather than interpreting their contents as branches.
    text = ''.join(lines)
    if re.search(r'\[=*\[|\\(?:\r?\n|z)', text):
        if any(t.lastgroup in ('longstring', 'longcomment', 'string') and '\n' in t[0] for t in tokens(text)):
            return lines, 0
    count, index = 0, 0
    while index + 2 < len(lines):
        first = re.fullmatch(r'([ \t]*)if (.+) then\n', lines[index])
        if first:
            indent = first[1]
            nested = re.fullmatch(re.escape(indent) + r'    if (.+) then goto (\w+) end\n', lines[index + 1])
            if nested and lines[index + 2] == indent + 'end\n':
                lines[index:index + 3] = [f'{indent}if ({first[2]}) and ({nested[1]}) then goto {nested[2]} end\n']
                count += 1
                index += 1
                continue
            if index + 4 >= len(lines):
                break
            jump = re.fullmatch(re.escape(indent) + r'    goto (\w+)\n', lines[index + 1])
            second = re.fullmatch(re.escape(indent) + r'    if (.+) then goto (\w+) end\n', lines[index + 3])
            if (jump and second and jump[1] == second[2]
                    and lines[index + 2] == indent + 'else\n'
                    and lines[index + 4] == indent + 'end\n'):
                lines[index:index + 5] = [f'{indent}if ({first[2]}) or ({second[1]}) then goto {jump[1]} end\n']
                count += 1
        index += 1
    return lines, count


def fold_boolean_jump_chains(lines):
    """A private boolean or 0/1 flag built through a jump becomes short-circuit Lua.

    Every incoming edge must belong to the chain. The temporary must be a unique
    local, uncaptured and unread by its conditions, so delaying its intermediate
    assignment cannot be observed by a condition's calls.
    """
    flow = Flow(lines)
    if not flow.ok:
        return lines, 0
    declared = Counter(_declared_temporaries(flow.structure))
    code = '\n'.join(flow.structure)
    labels = Counter(re.findall(r'::(\w+)::', code))
    jumps = Counter(re.findall(r'\bgoto (\w+)\b', code))
    for start, raw in enumerate(lines):
        first = re.fullmatch(r'([ \t]*)(?:if .+ then goto \w+ end|\w+ = (?:true|false|0|1))\n', raw)
        if not first:
            continue
        indent = first[1]
        conditions, target, store = [], None, None
        i = start
        while i < len(lines):
            guard = re.fullmatch(re.escape(indent) + r'if (.+) then goto (\w+) end\n', lines[i])
            assignment = re.fullmatch(re.escape(indent) + r'(\w+) = (true|false|0|1)\n', lines[i])
            if guard and (target is None or target == guard[2]):
                target = guard[2]
                conditions.append(guard[1])
            elif assignment and store is None:
                store = (assignment[1], assignment[2])
            else:
                break
            i += 1
        if not conditions or store is None or i + 3 >= len(lines):
            continue
        name, initial = store
        if declared[name] != 1 or name in flow.opaque_pinned:
            continue
        if any(name in _line_reads(_structure_line(c)) for c in conditions):
            continue
        done = re.fullmatch(re.escape(indent) + r'goto (\w+)\n', lines[i])
        if not done or done[1] == target:
            continue
        final = {'true': 'false', 'false': 'true', '0': '1', '1': '0'}[initial]
        expected = [f'{indent}::{target}::\n', f'{indent}{name} = {final}\n', f'{indent}::{done[1]}::\n']
        if lines[i + 1:i + 4] != expected:
            continue
        if labels[target] != 1 or labels[done[1]] != 1 or jumps[target] != len(conditions) or jumps[done[1]] != 1:
            continue
        expression = ' or '.join('(' + c + ')' for c in conditions)
        rhs = ('not ' if initial == 'true' else 'not not ') + '(' + expression + ')'
        if initial in ('0', '1'):
            rhs = f'({expression}) and {final} or {initial}'
        lines[start:i + 4] = [f'{indent}{name} = {rhs}\n']
        return lines, len(conditions) + 1
    return lines, 0


def fold_nested_return_jumps(lines):
    """Replace jumps to an immediate bare or literal return, including nested exits.

    Only nil/boolean literals can move across scopes; a variable or call could
    change meaning through shadowing or cleanup. Duplicate label names and named
    closure bodies are deliberately excluded.
    A block around a non-tail return keeps Lua's last-statement grammar intact.
    """
    structure = _structure(lines)
    ranges = _closure_ranges(structure)
    if ranges is None:
        return lines, 0
    excluded = {i for a, b in ranges for i in range(a, b + 1)}
    labels = Counter(re.findall(r'::(\w+)::', '\n'.join(structure)))
    count = 0
    for i, raw in enumerate(structure):
        match = re.fullmatch(r'\s*::(\w+)::\s*', raw)
        if not match or i in excluded or labels[match[1]] != 1:
            continue
        nxt = next((j for j in range(i + 1, len(structure)) if structure[j].strip()), None)
        implicit_tail = (raw.startswith('    ::') and not raw.startswith('     ')
                         and nxt is not None and structure[nxt] == 'end'
                         and not any(s.strip() for s in structure[nxt + 1:]))
        if nxt is None:
            continue
        # Structure blanks string literals as well as comments. Inspect the
        # actual return tokens so `return "text"` cannot look like a bare return.
        return_code = ''.join(t[0] for t in tokens(lines[nxt])
                              if t.lastgroup not in ('comment', 'longcomment')).strip()
        literal_return = re.fullmatch(r'return(?: (?:nil|true|false))?', return_code)
        if not literal_return and not implicit_tail:
            continue
        ret = literal_return[0] if literal_return else 'return'
        label = match[1]
        for j, line in enumerate(lines):
            if j in excluded:
                continue
            jump = re.fullmatch(r'([ \t]*)goto ' + re.escape(label) + r'\s*\n', line)
            if jump:
                following = next((s.strip() for s in structure[j + 1:] if s.strip()), 'end')
                tail = following == 'end' or following == 'else' or following.startswith(('elseif ', 'until '))
                lines[j] = jump[1] + (ret if tail else f'do {ret} end') + '\n'
                count += 1
            elif re.fullmatch(r'[ \t]*if .+ then goto ' + re.escape(label) + r' end\n', line):
                lines[j] = re.sub(r'\bthen goto ' + re.escape(label) + r' end\n$', f'then {ret} end\n', line)
                count += 1
        remaining = '\n'.join(_structure(lines))
        if not re.search(r'\bgoto ' + re.escape(label) + r'\b', remaining):
            lines[i] = ''
    return lines, count


def prune_unreachable_return_jumps(lines):
    """Remove an immediate goto after an unconditional literal-return block.

    The enclosing do block contains only the return. No intervening label can
    provide another entry to the jump; cleanup errors also cannot reach it.
    Keep calls, arbitrary expressions and diagnostic comments outside this rule.
    """
    code = ''.join(re.sub(r'[^\n]', ' ', t[0])
                   if t.lastgroup in ('string', 'longstring', 'comment', 'longcomment')
                   else t[0] for t in tokens(''.join(lines))).splitlines()
    count = 0
    for i, line in enumerate(lines[:-1]):
        match = re.fullmatch(r'([ \t]*)do return(?: (?:nil|true|false))? end\n', line)
        if not match or code[i] != line.rstrip('\n'):
            continue
        if code[i + 1] != lines[i + 1].rstrip('\n'):
            continue
        if re.fullmatch(re.escape(match[1]) + r'goto \w+\n', lines[i + 1]):
            lines[i + 1] = ''
            count += 1
    return lines, count


def fold_loop_exit_jumps(lines):
    """A jump to immediately after the innermost loop is an ordinary break.

    Retain jumps out of multiple loops and across intervening cleanup. Parsing
    lexical loop ownership matters: replacing an outer-loop jump inside an inner
    loop with break would silently change quest behavior.
    """
    structure = _structure(lines)
    stack, loops, owners = [], {}, {}
    for i, raw in enumerate(structure):
        line = raw.strip()
        if re.fullmatch(r'(?:local )?function \w+\([^)]*\)', line):
            stack.append(('function', i))
        elif re.fullmatch(r'(?:while .+ do|for .+ do|repeat)', line):
            stack.append(('loop', i))
        elif re.fullmatch(r'(?:if .+ then|do)', line):
            stack.append(('block', i))
        elif line == 'end' or line.startswith('until '):
            if not stack:
                return lines, 0
            kind, start = stack.pop()
            if kind == 'loop':
                loops[start] = i
        elif 'function' in line:
            # Anonymous/multiline function syntax is outside the emitter shape.
            return lines, 0
        if re.search(r'\bgoto \w+\b', line):
            for kind, start in reversed(stack):
                if kind == 'function':
                    break
                if kind == 'loop':
                    owners[i] = start
                    break
    if stack:
        return lines, 0
    label_counts = Counter(re.findall(r'::(\w+)::', '\n'.join(structure)))
    exits = {}
    for start, end in loops.items():
        nxt = next((j for j in range(end + 1, len(structure)) if structure[j].strip()), None)
        label = re.fullmatch(r'\s*::(\w+)::\s*', structure[nxt]) if nxt is not None else None
        if label and label_counts[label[1]] == 1:
            exits[start] = (label[1], nxt)
    count = 0
    for i, owner in owners.items():
        if owner not in exits:
            continue
        label, _ = exits[owner]
        raw = lines[i]
        standalone = re.fullmatch(r'([ \t]*)goto ' + re.escape(label) + r'\n', raw)
        if standalone:
            lines[i] = standalone[1] + 'break\n'
            count += 1
        elif re.fullmatch(r'[ \t]*if .+ then goto ' + re.escape(label) + r' end\n', raw):
            lines[i] = re.sub(r'\bthen goto ' + re.escape(label) + r' end\n$', 'then break end\n', raw)
            count += 1
    remaining = '\n'.join(_structure(lines))
    for label, i in exits.values():
        if not re.search(r'\bgoto ' + re.escape(label) + r'\b', remaining):
            lines[i] = ''
    return lines, count


def fold_cleanup_exit_jumps(lines):
    """Inline a short, straight cleanup tail at its early exits.

    Only one or two calls and a void return are moved. Every local they read must
    be a unique outer binding declared before all jump sites. This rejects shadowed
    movie/resource handles, loop variables and Lua to-be-closed locals.
    """
    structure = _structure(lines)
    if any('<close>' in s or '<const>' in s for s in structure):
        return lines, 0
    ranges = _closure_ranges(structure)
    if ranges is None:
        return lines, 0
    excluded = {i for a, b in ranges for i in range(a, b + 1)}
    label_counts = Counter(re.findall(r'::(\w+)::', '\n'.join(structure)))
    declarations = {}
    for i, line in enumerate(structure):
        decl = re.match(r'^( *)local (?!function\b)([\w, ]+?)(?:\s*=|\s*$)', line)
        if decl:
            for name in decl[2].split(','):
                declarations.setdefault(name.strip(), []).append((i, len(decl[1]) == 4))
        local_function = re.match(r'^( *)local function (\w+)\(', line)
        if local_function:
            declarations.setdefault(local_function[2], []).append((i, len(local_function[1]) == 4))
        params = re.search(r'\bfunction(?:\s+[\w.:]+)?\s*\(([^)]*)\)', line)
        if params:
            for name in params[1].split(','):
                declarations.setdefault(name.strip(), []).append((i, i == 0))
        loop = re.match(r'\s*for ([\w, ]+?)(?:\s*=|\s+in\s)', line)
        if loop:
            for name in loop[1].split(','):
                declarations.setdefault(name.strip(), []).append((i, False))
    for i, line in enumerate(structure):
        label = re.fullmatch(r'    ::(\w+)::', line)
        if not label or i in excluded or label_counts[label[1]] != 1:
            continue
        tail = [j for j in range(i + 1, len(lines)) if structure[j].strip()]
        if not tail or structure[tail[-1]] != 'end':
            continue
        body = tail[:-1]
        if body and structure[body[-1]] == '    return':
            body = body[:-1]
        if not 1 <= len(body) <= 2 or any(not re.fullmatch(r'    [\w.:]+\(.*\)', structure[j]) for j in body):
            continue
        # Preserve diagnostic comments rather than copying or discarding them.
        if any(t.lastgroup in ('comment', 'longcomment')
               for t in tokens(''.join(lines[i + 1:tail[-1]]))):
            continue
        reads = set().union(*(_line_reads(structure[j]) for j in body))
        sites = [j for j, s in enumerate(structure) if re.search(r'\bgoto ' + re.escape(label[1]) + r'\b', s)]
        if not sites or any(j in excluded for j in sites):
            continue
        if any(len(declarations[n]) != 1 or not declarations[n][0][1]
               or declarations[n][0][0] >= min(sites) for n in reads if n in declarations):
            continue
        calls = [lines[j].strip() for j in body]
        edits = {}
        for j in sites:
            standalone = re.fullmatch(r'([ \t]*)goto ' + re.escape(label[1]) + r'\n', lines[j])
            inline = re.fullmatch(r'([ \t]*if .+ then )(?:goto ' + re.escape(label[1]) + r') end\n', lines[j])
            if standalone:
                following = next((s.strip() for s in structure[j + 1:] if s.strip()), 'end')
                final = following in ('end', 'else') or following.startswith(('elseif ', 'until '))
                edits[j] = ''.join(standalone[1] + call + '\n' for call in calls)
                edits[j] += standalone[1] + ('return' if final else 'do return end') + '\n'
            elif inline:
                edits[j] = inline[1] + '; '.join([*calls, 'return']) + ' end\n'
            else:
                break
        if len(edits) == len(sites):
            for j, text in edits.items():
                lines[j] = text
            lines[i] = ''
            return ''.join(lines).splitlines(keepends=True), len(edits)
    return lines, 0


def fold_if_around_while(lines):
    """`if C then while C do BODY end end` -> `while C do BODY end` (the loop tests C first anyway)."""
    count = 0
    changed = True
    while changed:
        changed = False
        lines = [l for l in lines if l != '']
        for i in range(len(lines) - 2):
            m = re.fullmatch(r'(\s*)if (.+) then\n', lines[i])
            if not m or lines[i + 1] != f'{m.group(1)}    while {m.group(2)} do\n':
                continue
            indent = m.group(1)
            inner_end = next((j for j in range(i + 2, len(lines)) if lines[j] == indent + '    end\n'), None)
            if inner_end is None or inner_end + 1 >= len(lines) or lines[inner_end + 1] != indent + 'end\n':
                continue
            if any(lines[j].strip() and not lines[j].startswith(indent + '    ') for j in range(i + 2, inner_end)):
                continue
            lines[i] = ''
            for j in range(i + 1, inner_end + 1):
                lines[j] = lines[j][4:]
            lines[inner_end + 1] = ''
            count += 1
            changed = True
            break
    return lines, count
