"""Hoist native cleanup regions into local functions so early exits keep their releases.

Retail script functions share one cleanup epilogue (release controlled entities, destroy actor
maps/movies, unpause) reached by `goto` from many places. The lifter cannot always express those
jumps in Lua (labels inside nested blocks), so it lowers them to `return` with a
`-- TODO(native): goto LAB_x` marker, which would leak resources. This pass, on the lifted Lua:

  1. finds `-- LAB_x: (native jump target)` regions that run straight to a `return`,
  2. hoists them into `local function __cleanup_LAB_x() ... end` right after the locals line,
  3. turns every `... goto LAB_x` early return into `__cleanup_LAB_x(); return`.

Nested cleanup chains (LAB_a falls into LAB_b) are inlined. Regions containing control flow other
than nested labels are left alone (they are not plain epilogues).

A region is still straight-line when it *closes* enclosing `if`/`do` blocks on its way out (the label
sits inside a block and the epilogue continues after it) -- only a loop's `end`, which is a back edge,
ends the search. Such a region is hoisted but its own lines are left in place: the `end`s it crosses
belong to those blocks. A region may also finish by jumping to a label the lifter really emitted
(`goto FLOW_after_lab_x`); when that label's own block runs straight to `return`, its statements join
the region and the call sites get the whole epilogue.
"""
from __future__ import annotations

import re

LABEL_LINE = re.compile(r'^(?P<ind>[ \t]*)-- (?P<label>LAB_[0-9a-f]+(?:_c\d+)?): \(native jump target\)\s*$')
CONTROL = re.compile(r'^\s*(if |while |for |repeat|until |else|elseif |end\b|goto |::|function |local function )')
EARLY_RETURN = re.compile(r'^(?P<ind>[ \t]*)(?P<cond>if .+ then )?return end  -- TODO\(native\): goto (?P<label>LAB_[0-9a-f]+(?:_c\d+)?)\s*$')
COMMENT_GOTO = re.compile(r'^(?P<ind>[ \t]*)-- TODO\(native\): goto (?P<label>LAB_[0-9a-f]+(?:_c\d+)?)\s*$')
INLINE_GOTO = re.compile(r'^(?P<ind>[ \t]*)if true then return end  -- TODO\(native\): goto (?P<label>LAB_[0-9a-f]+(?:_c\d+)?)\s*$')


GOTO_LINE = re.compile(r'^\s*goto (LAB_[0-9a-f]+(?:_c\d+)?)\s*$')
ANY_GOTO_LINE = re.compile(r'^\s*goto (\w+)\s*$')


def _emitted_labels(lines):
    return {m.group(1) for line in lines for m in [re.fullmatch(r'\s*::(\w+)::\s*', line)] if m}


def _terminal(s, emitted=()):
    """`return`, a plain `goto LAB_y` (another native region), or a jump to a label the lifter really emitted
    (`goto FLOW_after_lab_x`) ends a straight-line region."""
    if s == 'return':
        return 'return'
    m = GOTO_LINE.match(s)
    if m:
        return ('goto', m.group(1))
    m = ANY_GOTO_LINE.match(s)
    return ('mark', m.group(1)) if m and m.group(1) in emitted else None


OPENER = re.compile(r'(?:if .+ then|while .+ do|for .+ do|repeat|do|(?:local )?function .*)')
LOOP_OPENER = re.compile(r'(?:while .+ do|for .+ do|repeat)')


def _blocks(lines):
    """index -> ('open', kind) | ('close', open_index) | ('branch', open_index), or {} when unbalanced.

    `kind` is 'loop' for while/for/repeat and 'block' for if/do/function. Single-line forms
    (`if C then return end`) open nothing.
    """
    out, stack = {}, []
    for i, raw in enumerate(lines):
        line = raw.strip()
        if not line:
            continue
        if line == 'end' or line.startswith('until '):
            if not stack:
                return {}
            out[i] = ('close', stack.pop())
        elif line == 'else' or re.fullmatch(r'elseif .+ then', line):
            if not stack:
                return {}
            out[i] = ('branch', stack[-1])
        elif OPENER.fullmatch(line):
            out[i] = ('open', 'loop' if LOOP_OPENER.fullmatch(line) else 'block')
            stack.append(i)
    return {} if stack else out


def _walk(lines, start, emitted, blocks, seen=()):
    """(body, terminal index, terminal, crossed) for the epilogue from label line `start`, or None.
    `crossed` is True when the path left an enclosing block, which makes the region unsafe to replace in place.

    The epilogue is straight-line: it opens no block of its own, and it may close enclosing if/do blocks
    (falling out of them is still one path) but never a loop, whose `end` is a back edge.
    """
    body, j, crossed = [], start + 1, False
    while j < len(lines):
        s = lines[j].strip()
        terminal = _terminal(s, emitted)
        if terminal:
            return body, j, terminal, crossed
        kind = blocks.get(j)
        if kind is not None:
            if kind[0] == 'open':
                return None
            crossed = True
            if kind[0] == 'branch':
                # the fall-through skips the rest of this if: continue after the block's own `end`
                close = next((k for k in range(j + 1, len(lines))
                              if blocks.get(k) == ('close', kind[1])), None)
                if close is None:
                    return None
                j = close + 1
                continue
            if lines[kind[1]].strip() and LOOP_OPENER.fullmatch(lines[kind[1]].strip()):
                return None                      # falling out of a loop body is the next iteration
            if kind[1] == 0 or not lines[kind[1]].strip():
                return None                      # the function's own end: no terminal on this path
            j += 1
            continue
        m = LABEL_LINE.match(lines[j])
        if m:
            if m.group('label') in seen:
                return None
            inner = _walk(lines, j, emitted, blocks, seen + (m.group('label'),))
            if inner is None:
                return None
            return body + [('label', m.group('label'))], inner[1], inner[2], crossed or inner[3]
        cm = COMMENT_GOTO.match(lines[j])
        if cm:
            # the region ends with a jump the lifter could not make: it delegates to that label's region
            # (a cleanup ladder: release A, then the region that releases B and returns). Resolved below;
            # a region whose delegate never reaches a return is dropped.
            if cm.group('label') in seen:
                return None
            return body + [('label', cm.group('label'))], j, ('todo', cm.group('label')), crossed
        if not s or s.startswith('--'):
            if s and not (s.startswith('-- TODO(native)') or s.startswith('-- LAB_')):
                return None
            j += 1
            continue
        if CONTROL.match(lines[j]):
            return None
        body.append(('stmt', s))
        j += 1
    return None


def _regions(lines):
    """label -> (start index, terminal index, body statements, terminal, crossed) for straight-line regions."""
    regions = {}
    emitted = _emitted_labels(lines)
    blocks = _blocks(lines)
    if not blocks:
        return regions
    for i, line in enumerate(lines):
        m = LABEL_LINE.match(line)
        if not m:
            continue
        walked = _walk(lines, i, emitted, blocks, (m.group('label'),))
        if walked is None:
            continue
        body, j, terminal, crossed = walked
        if body or terminal == 'return' or terminal[0] == 'goto':
            regions[m.group('label')] = (i, j, body, terminal, crossed)
    # a region that ends by jumping to an emitted label whose own block runs straight to `return` really ends
    # in that return: take the tail into the region so the call sites reproduce the whole epilogue. A region
    # that delegates to another region takes nothing - the delegate carries the tail, and adding it here too
    # would release the same resource twice.
    def delegate(body):
        return body[-1][1] if body and body[-1][0] == 'label' else None
    for label, (start, end, body, terminal, crossed) in list(regions.items()):
        if terminal == 'return' or terminal[0] != 'mark' or delegate(body) in regions:
            continue
        tail = _emitted_tail(lines, terminal[1])
        if tail is None:
            continue
        regions[label] = (start, end, body + [('stmt', t) for t in tail], 'return', crossed)
    for _ in range(len(regions)):
        for label, (start, end, body, terminal, crossed) in list(regions.items()):
            inner = delegate(body)
            if terminal != 'return' and inner in regions and regions[inner][3] == 'return':
                regions[label] = (start, end, body, 'return', crossed)
    return {label: region for label, region in regions.items() if region[3] == 'return' or region[3][0] != 'todo'}


def _emitted_tail(lines, name):
    """The statements a real `::name::` label runs before the function returns, when that block is
    straight-line. None when it is anything else (the jump is then a genuine flow gap)."""
    start = next((i for i, line in enumerate(lines) if re.fullmatch(r'\s*::' + re.escape(name) + r'::\s*', line)), None)
    if start is None:
        return None
    out, j = [], start + 1
    while j < len(lines):
        s = lines[j].strip()
        if not s or s.startswith('--') or re.fullmatch(r'::\w+::', s):
            j += 1
            continue
        if s in ('return', 'do return end'):
            return out
        if s == 'end' and not any(line.strip() for line in lines[j + 1:]):
            return out                    # the function's own end: falling off it is the same return
        if CONTROL.match(lines[j]):
            return None
        out.append(s)
        j += 1
    return None


def _scopes(lines):
    """index -> tuple of the open-block indices enclosing that line (a `::label::` is visible from every line
    whose scope extends its own)."""
    blocks = _blocks(lines)
    out, stack = [], []
    for i in range(len(lines)):
        kind = blocks.get(i)
        if kind and kind[0] == 'close':
            stack.pop()
        out.append(tuple(stack))
        if kind and kind[0] == 'open':
            stack.append(i)
    return out


def _call(label, regions):
    """Call of the hoisted region, or nothing when the region is a bare `return`."""
    return '' if not regions[label][2] else f'{_name(label, regions)}(); '


def _exit(label, regions, in_place=False):
    """Lua statement that reproduces the region's terminal.

    A region that ends by jumping to a label the lifter really emitted keeps that `goto` where the region
    itself stood, but not at a call site: the site is nested where the lifter already judged the jump
    inexpressible, which is why it wrote `return` there. Running the cleanup and returning is still what the
    native does with the resources; the flow gap stays on the line as its own TODO."""
    terminal = regions[label][3]
    if terminal == 'return':
        return 'return'
    if terminal[0] == 'mark' and not in_place:
        return 'return'
    return f'goto {terminal[1]}'


def _gap(label, regions):
    """The marker a call site keeps: the jump the region ends with that the site cannot make."""
    terminal = regions[label][3]
    return f'  -- TODO(native): goto {terminal[1]}' if terminal != 'return' and terminal[0] == 'mark' else ''


def _name(label, regions):
    return f'__cleanup_{label}' if regions[label][3] == 'return' else f'__region_{label}'


def hoist_cleanup_regions(source: str) -> tuple[str, dict]:
    functions = re.split(r'(?m)^(?=function )', source)
    out, report = [], {}
    for chunk in functions:
        if not chunk.startswith('function '):
            out.append(chunk)
            continue
        lines = chunk.split('\n')
        regions = _regions(lines)
        # only labels that some early exit jumps to are worth hoisting
        wanted = {m.group('label') for line in lines for m in [EARLY_RETURN.match(line) or COMMENT_GOTO.match(line) or INLINE_GOTO.match(line)] if m}
        wanted &= set(regions)
        if not wanted:
            out.append(chunk)
            continue

        def expand(label, seen=()):
            stmts = []
            for kind, value in regions[label][2]:
                if kind == 'stmt':
                    stmts.append(value)
                elif value in regions and value not in seen:
                    stmts.extend(expand(value, seen + (label,)))
            return stmts

        scopes = _scopes(lines)

        def reachable(label, site):
            # a region that ends in `goto Y` can only be reproduced where Y is visible: an enclosing block
            terminal = regions[label][3]
            if terminal == 'return' or terminal[0] != 'goto':
                return True
            target = next((k for k, line in enumerate(lines) if re.fullmatch(r'\s*::' + re.escape(terminal[1]) + r'::\s*', line)), None)
            return target is not None and scopes[site][:len(scopes[target])] == scopes[target]

        # replace the in-place regions with a call + return, and the jumps with call + return
        new_lines, i = [], 0
        while i < len(lines):
            line = lines[i]
            lm = LABEL_LINE.match(line)
            if lm and lm.group('label') in wanted and not regions[lm.group('label')][4]:
                ind = lm.group('ind'); label = lm.group('label')
                if regions[label][2]:
                    new_lines.append(f'{ind}{_name(label, regions)}()')
                new_lines.append(f'{ind}{_exit(label, regions, in_place=True)}')
                i = regions[label][1] + 1          # skip through the terminal
                continue
            m = EARLY_RETURN.match(line)
            if m and m.group('label') in wanted and reachable(m.group('label'), i):
                cond = m.group('cond') or ''; label = m.group('label')
                new_lines.append(f"{m.group('ind')}{cond}{_call(label, regions)}{_exit(label, regions)}"
                                 f"{' end' if cond else ''}{_gap(label, regions)}")
                i += 1; continue
            m = INLINE_GOTO.match(line) or COMMENT_GOTO.match(line)
            if m and m.group('label') in wanted and reachable(m.group('label'), i):
                label = m.group('label')
                new_lines.append(f"{m.group('ind')}{_call(label, regions)}{_exit(label, regions)}{_gap(label, regions)}")
                i += 1; continue
            new_lines.append(line)
            i += 1
        # hoist definitions after the header + local declarations
        head_end = 1
        while head_end < len(new_lines) and re.match(r'^\s*(local |--)', new_lines[head_end]):
            head_end += 1
        defs = []
        for label in sorted(wanted):
            body = expand(label)
            if not body:
                continue
            defs.append(f'    local function {_name(label, regions)}()')
            defs.extend('        ' + s for s in body)
            defs.append('    end')
            report[label] = body
        out.append('\n'.join(new_lines[:head_end] + defs + new_lines[head_end:]))
    return ''.join(out), report
