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
"""
from __future__ import annotations

import re

LABEL_LINE = re.compile(r'^(?P<ind>[ \t]*)-- (?P<label>LAB_[0-9a-f]+): \(native jump target\)\s*$')
CONTROL = re.compile(r'^\s*(if |while |for |repeat|until |else|elseif |end\b|goto |::|function |local function )')
EARLY_RETURN = re.compile(r'^(?P<ind>[ \t]*)(?P<cond>if .+ then )?return end  -- TODO\(native\): goto (?P<label>LAB_[0-9a-f]+)\s*$')
COMMENT_GOTO = re.compile(r'^(?P<ind>[ \t]*)-- TODO\(native\): goto (?P<label>LAB_[0-9a-f]+)\s*$')
INLINE_GOTO = re.compile(r'^(?P<ind>[ \t]*)if true then return end  -- TODO\(native\): goto (?P<label>LAB_[0-9a-f]+)\s*$')


GOTO_LINE = re.compile(r'^\s*goto (LAB_[0-9a-f]+)\s*$')


def _terminal(s):
    """`return` or a plain `goto LAB_y` ends a straight-line region."""
    if s == 'return':
        return 'return'
    m = GOTO_LINE.match(s)
    return ('goto', m.group(1)) if m else None


def _region_end(lines, start):
    """Index of the `return`/`goto` that ends the straight-line region starting at label line `start`."""
    j = start + 1
    while j < len(lines):
        s = lines[j].strip()
        if _terminal(s):
            return j
        if LABEL_LINE.match(lines[j]):
            return _region_end(lines, j)
        if CONTROL.match(lines[j]) or (s.startswith('--') and not s.startswith('-- TODO(native)')):
            return None
        j += 1
    return None


def _regions(lines):
    """label -> (start index, end index exclusive of `return`, body statements) for straight-line regions."""
    regions = {}
    for i, line in enumerate(lines):
        m = LABEL_LINE.match(line)
        if not m:
            continue
        body, j, ok, terminal = [], i + 1, False, None
        while j < len(lines):
            s = lines[j].strip()
            if _terminal(s):
                ok, terminal = True, _terminal(s)
                break
            if not s or s.startswith('-- TODO(native): goto'):
                pass
            elif LABEL_LINE.match(lines[j]):
                # the nested label owns the rest of the epilogue; inline it at expansion time
                body.append(('label', LABEL_LINE.match(lines[j]).group('label')))
                inner = _region_end(lines, j)
                if inner is None:
                    break
                j, ok, terminal = inner, True, _terminal(lines[inner].strip())
                break
            elif CONTROL.match(lines[j]) or s.startswith('--'):
                if s.startswith('-- TODO(native)') or s.startswith('-- LAB_'):
                    pass
                else:
                    break
            else:
                body.append(('stmt', s))
            j += 1
        if ok and (body or terminal == 'return'):
            regions[m.group('label')] = (i, j, body, terminal)
    return regions


def _call(label, regions):
    """Call of the hoisted region, or nothing when the region is a bare `return`."""
    return '' if not regions[label][2] else f'{_name(label, regions)}(); '


def _exit(label, regions):
    """Lua statement that reproduces the region's terminal at a call site."""
    terminal = regions[label][3]
    return 'return' if terminal == 'return' else f'goto {terminal[1]}'


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

        # replace the in-place regions with a call + return, and the jumps with call + return
        new_lines, i = [], 0
        while i < len(lines):
            line = lines[i]
            lm = LABEL_LINE.match(line)
            if lm and lm.group('label') in wanted:
                ind = lm.group('ind'); label = lm.group('label')
                if regions[label][2]:
                    new_lines.append(f'{ind}{_name(label, regions)}()')
                new_lines.append(f'{ind}{_exit(label, regions)}')
                i = regions[label][1] + 1          # skip through the terminal
                continue
            m = EARLY_RETURN.match(line)
            if m and m.group('label') in wanted:
                cond = m.group('cond') or ''; label = m.group('label')
                new_lines.append(f"{m.group('ind')}{cond}{_call(label, regions)}{_exit(label, regions)}{' end' if cond else ''}")
                i += 1; continue
            m = INLINE_GOTO.match(line) or COMMENT_GOTO.match(line)
            if m and m.group('label') in wanted:
                label = m.group('label')
                new_lines.append(f"{m.group('ind')}{_call(label, regions)}{_exit(label, regions)}")
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
