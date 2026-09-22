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

import functools
import re
from collections import Counter

from tools.script_recovery.lua_local_versions import flow_graph
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
            shadowed = set()          # a closure-local (`local x = ...`) is not the enclosing function's x
            for i in range(a, b + 1):
                local = re.match(r'\s*local (\w+) = ', self.structure[i])
                names = {t[0] for t in tokens(lines[i]) if t.lastgroup == 'identifier'}
                if local:
                    names.discard(local.group(1))
                    shadowed.add(local.group(1))
                self.pinned.update(names - shadowed)
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
            lines[r] = rename_identifiers(lines[r], {name: m.group(3)})
            count += 1
    return lines, count


def propagate_copies(lines):
    """`a = b` where `b` is defined exactly once (or is a parameter / never assigned): every read of `a`
    reached only by that copy reads `b` itself; the copy then dies."""
    count = 0
    flow = Flow(lines)
    if not flow.ok:
        return lines, 0
    structure = flow.structure
    copy = re.compile(r'(\s*)(?:local )?(\w+) = (\w+)\n')
    declared = set(_declared_temporaries(structure))
    initialised = {m.group(1) for l in structure for m in [re.match(r'\s*local (\w+) = ', l)] if m}
    # a name a cleanup closure only reads keeps its definitions; its reads outside the closure may still be renamed
    closure_written = set()
    for a, b in _closure_ranges(structure) or []:
        for k in range(a, b + 1):
            target = _line_target(structure[k])
            if target and not re.match(r'\s*local ', structure[k]):      # (a closure-local shadow is not a write)
                closure_written.add(target)
    for name in list(declared | initialised):
        if name in closure_written:
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
            src_defs = _reads_and_defs(structure, src)[0]
            # a source reloaded from the alias (`timerId = scratchValue4` at a loop end) still never moves
            src_defs = [x for x in src_defs if not re.fullmatch(r'\s*' + re.escape(src) + r' = ' + re.escape(name) + r'\n', lines[x])]
            if (src in declared or src in initialised) and len(src_defs) > 1:
                continue                          # the source moves: not an alias
            if src not in declared and src not in initialised and src_defs:
                continue                          # an unknown (upvalue / global) written somewhere
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
    for a, b in ranges:
        body = lines[a + 1:b]
        literal = re.compile(r'(\s*)(\w+) = (true|false|nil|-?\d+(?:\.\d+)?)\n')
        i = 0
        while i < len(body):
            m = literal.fullmatch(body[i])
            if m:
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
    """`quest:GetHero()` asked twice or more in one function: one `hero` local at the top (the hero never
    changes), every call reads it."""
    calls = [i for i, l in enumerate(lines) if 'quest:GetHero()' in _structure_line(l)]
    if len(calls) < 2:
        return lines, 0
    declared = set(_declared_temporaries(_structure(lines)))
    head = re.match(r'function \w+\(([^)]*)\)', lines[0]) if lines else None
    params = {x.strip() for x in head.group(1).split(',')} if head else set()
    name = 'hero'
    if name in declared:
        # an existing `hero` local that only ever holds `quest:GetHero()` is the hoisted one
        structure = _structure(lines)
        defs = [i for i, l in enumerate(structure) if _line_target(l) == name]
        if defs and all(re.fullmatch(r'\s*hero = quest:GetHero\(\)\n', lines[i]) for i in defs):
            for i in defs:
                lines[i] = ''
            _remove_declaration(lines, name)
            declared.discard(name)
            calls = [i for i in calls if lines[i]]
    while name in declared or name in params:
        name += '_'
    for i in calls:
        lines[i] = lines[i].replace('quest:GetHero()', name)
    # after the parameter line and any leading `local` declarations
    at = 1
    while at < len(lines) and re.match(r'\s*local [\w, ]+\n', lines[at]):
        at += 1
    indent = re.match(r'\s*', lines[at] if at < len(lines) else '    ').group(0) or '    '
    lines.insert(at, f'{indent}local {name} = quest:GetHero()\n')
    return lines, 1


def fold_constant_conditions(lines):
    """`not true` / `not false`, `if true then BODY end` -> BODY, `if false then ... end` -> nothing."""
    count = 0
    for i, line in enumerate(lines):
        new = re.sub(r'\bnot true\b', 'false', re.sub(r'\bnot false\b', 'true', line))
        if not new.lstrip().startswith('--'):
            def literal_compare(mm):
                # only a literal that is a whole operand (`, 1 ~= 0`), not `x & 1 ~= 0` (binds as `(x & 1) ~= 0`)
                if not re.search(r'(?:[(=,]|\b(?:if|elseif|while|until|return|and|or|not))\s*$', new[:mm.start()]):
                    return mm.group(0)
                truth = bool(int(mm.group(1))) if mm.group(2) == '~=' else not int(mm.group(1))
                return 'true' if truth else 'false'
            new = re.sub(r'(?<![\w.])(\d+) (~=|==) 0\b', literal_compare, new)
        if not new.lstrip().startswith('--'):
            new = re.sub(r' and true\b', '', new)
            new = re.sub(r'\btrue and ', '', new)
            new = re.sub(r' or false\b', '', new)
            new = re.sub(r'\bfalse or ', '', new)
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


QUERY_METHOD = re.compile(r'^(?:Get|Is|Has|Can|Msg)')


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
        touched = set()
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
                if d in touched:
                    continue
                r = _sink_target(flow, name, d, expr, reads, def_set, touched)
                if r is None or r in touched:
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
    for name in _declared_temporaries(structure):
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


def simplify_conditions(lines):
    """`not (not X)` -> X, `not (a == b)` -> `a ~= b`, `((C) and 0 or 1) ~= 0` -> `not (C)`, and an
    unread `p = T()` (pure) staging store."""
    count = 0
    structure = _structure(lines)
    for i, line in enumerate(lines):
        if line.lstrip().startswith('--'):
            continue
        new = line
        # the decompiler's int-valued comparison, both ways round:
        #   ((C) and 0 or 1) ~= 0  is  not (C)      ((C) and 1 or 0) ~= 0  is  (C)
        for staged, negated in ((r'and 0 or 1', True), (r'and 1 or 0', False)):
            # one nesting level, but any number of groups with text between them:
            # `quest:GetStateListAt("L", i):GetDefName() == "X"` is two groups, not one
            pattern = r'\(\((?P<c>(?:[^()]*\([^()]*\))*[^()]*)\) ' + staged + r'\) (?P<op>~=|==) 0'
            for m in reversed(list(re.finditer(pattern, new))):
                cond = m.group('c')
                invert = (m.group('op') == '~=') == negated
                repl = f'not ({cond})' if invert else f'({cond})'
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


def drop_free_suffixes(lines):
    """`guildStagBeetle2` whose base name `guildStagBeetle` no longer occurs in the function (the earlier
    holder was inlined away): the base name."""
    count = 0
    text = ''.join(lines)
    idents = {t.group(0) for t in re.finditer(r'[A-Za-z_]\w*', text)}
    renames = {}
    for name in sorted(idents):
        m = re.fullmatch(r'([A-Za-z]\w*?[A-Za-z])(\d+)', name)   # `thing_38` keeps its slot suffix
        if not m or int(m.group(2)) < 2 or m.group(1) in idents or m.group(1) in renames.values():
            continue
        if not any(re.match(r'\s*(?:local )?' + re.escape(name) + r'\b', l) for l in lines):
            continue                                   # not a local of this function
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
    for i, line in enumerate(lines):
        new = re.sub(r' \+ -(\d+(?:\.\d+)?)\b', r' - \1', line)
        new = re.sub(r'\(#(\w+) \* 12\) / 12', r'#\1', new)         # element count via the 12-byte stride
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
        total += run('tailGuards', flatten_tail_guards)
        total += run('blockingSpeech', fold_blocking_speech)
        total += run('uintFixups', fold_uint_fixups)
        total += run('freshGuards', simplify_fresh_guards)
        total += run('constantConditions', fold_constant_conditions)
        total += run('emptyElse', prune_empty_else)
        total += run('unusedClosures', prune_unused_closures)
        total += run('gotoElse', fold_goto_else)
        total += run('booleanBranches', fold_boolean_branches)
        total += run('elseif', fold_elseif)
        total += run('elseExit', fold_else_exit)
        total += run('gotoReturn', fold_goto_return)
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
        if not field:
            return m.group(0)
        used[field['name']] = (off, field.get('value'))
        return f'{m.group(1)}(SCRIPT_DEF.{field["name"]}'
    new = re.sub(r'(quest:ReadGlobalGameData(?:Float|FloatAt)?)\((0x[0-9a-f]+|\d+)', repl, source)
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


def alias_entity_state(source):
    """The per-entity state shim becomes `state` with `GetInt`/`SetInt`... methods
    (`__native_entity_state:GetStateInt("TeamID")` -> `state:GetInt("TeamID")`)."""
    if not STATE_SHIM.search(source) or re.search(r'\bstate\b', rename_identifiers(source, {'__native_entity_state': 'x'})):
        return source, 0
    shim = ('local state = {}  -- per-entity script state (__native_entity_state)\n'
            'do\n    local fields = {}\n'
            '    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do\n'
            '        state["Get" .. kind] = function(_, name) return fields[name] end\n'
            '        state["Set" .. kind] = function(_, name, value) fields[name] = value end\n'
            '    end\nend\n')
    source = STATE_SHIM.sub(lambda m: shim, source, count=1)
    source, k = re.subn(r'\b__native_entity_state:(Get|Set)State(Bool|Int|Float|String|Thing)\(', r'state:\1\2(', source)
    return source, k


def inline_entity_fields(source):
    """The per-entity state shim (`state:GetInt("AppleMode")`) holds the native class's member variables. ForgeFSE
    creates one sol::state per entity instance (LuaManager::RegisterEntityScriptData), so file-level locals ARE
    per-entity fields: `local appleMode` at the top, `appleMode` / `appleMode = v` in the functions. The shim goes."""
    from tools.script_recovery.readable_lua import camel
    shim = re.search(r'^local state = \{\}  -- per-entity script state \(__native_entity_state\)\n(?:.*\n)*?^end\n', source, re.M)
    if not shim:
        return source, 0
    keys = []
    for k in re.findall(r'\bstate:(?:Get|Set)(?:Bool|Int|Float|String|Thing)\("(\w+)"', source):
        if k not in keys:
            keys.append(k)
    if re.search(r'\bstate:\w+\((?!")', source):                 # a non-literal key: keep the shim
        return source, 0
    if not keys:
        # this entity never touches its own fields: the shim is dead scaffolding, not state
        rest = source[:shim.start()] + source[shim.end():]
        return (rest, 1) if not re.search(r'\bstate\b', rest) else (source, 0)
    # a function-local snapshot of a field under the field's own name (`local dummyNumber = state:GetInt("DummyNumber")`)
    # is dropped when that function never writes the field or the local: its reads read the field itself
    freed = set()
    for k in keys:
        n = camel(k)
        for chunk in _function_chunks(source):
            snap = re.search(r'^[ \t]*local ' + n + r' = state:Get\w+\("' + k + r'"\)\n', chunk, re.M)
            if snap and not re.search(r'^[ \t]*' + n + r'\s*=(?!=)', chunk, re.M) and f'"{k}", ' not in chunk \
                    and len(re.findall(r'\blocal ' + n + r'\b', chunk)) == 1:
                source = source.replace(chunk, chunk.replace(snap.group(0), '', 1), 1)
                freed.add(n)
    taken = {t[0] for t in tokens(source) if t.lastgroup == 'identifier'} - freed
    names = {}
    for k in keys:
        n = camel(k)
        while n in taken or n in names.values() or n in KEYWORDS_ALL:
            n += '_'
        names[k] = n
    new = re.sub(r'\bstate:Set(?:Bool|Int|Float|String|Thing)\("(\w+)", ', lambda m: names[m.group(1)] + ' = (', source)
    new = re.sub(r'\bstate:Get(?:Bool|Int|Float|String|Thing)\("(\w+)"\)', lambda m: names[m.group(1)], new)
    # `x = (value)` -> `x = value` for a plain operand
    new = re.sub(r'^(\s*\w+ = )\(([^()\n]*(?:\([^()\n]*\)[^()\n]*)*)\)$', r'\1\2', new, flags=re.M)
    block = '-- per-entity fields (native class members; one Lua state per entity instance)\nlocal ' + ', '.join(names[k] for k in keys) + '\n'
    new = new.replace(shim.group(0), block, 1)
    return new, len(keys)


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


def style_source(source, *, frame_returns_alive=True, rel=None, writers=None):
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
    for part in _function_chunks(source):
        if not part.startswith(('function ', 'local function ')):
            out.append(part)
            continue
        name = re.match(r'(?:local )?function (\w+)', part).group(1)
        before = style_metrics(part)
        styled, stats = style_function(part, pure, frame_returns_alive=frame_returns_alive)
        if writers is not None:
            styled, k = hoist_invariant_state(styled, rel, writers)
            stats['hoistedState'] = k
        report[name] = {'rewrites': stats, 'before': before, 'after': style_metrics(styled)}
        out.append(styled)
    text = tidy_blank_lines(''.join(out))
    text, late = name_helpers_by_shape(text)      # shapes appear once the bodies are styled (the cutscene fold)
    text, fields = inline_entity_fields(text)     # after styling: the copies of the fields are gone, their names free
    named += late
    return text, {'functions': report, 'hoistedRequires': hoisted, 'entityStateAliased': aliased, 'namedHelpers': named,
                  'pureLocalFunctions': sorted(pure)}


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
        ret = rest[0].strip()
        label = m.group(1)
        for j, l in enumerate(lines):
            g = re.fullmatch(r'(\s*)goto ' + label + r'\n', l)
            if g:
                lines[j] = f'{g.group(1)}{ret}\n'
                count += 1
            else:
                new = re.sub(r'\bthen goto ' + label + r' end$', f'then {ret} end', l.rstrip('\n'))
                if new != l.rstrip('\n'):
                    lines[j] = new + '\n'
                    count += 1
        if not re.search(r'\bgoto ' + label + r'\b', ''.join(lines)):
            lines[i] = ''
    return lines, count


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
