"""Declare the temporaries a lifted function reads but never declares.

The lifter builds a function's `local` line from the slots it assigns. A slot the decompiler only ever
*read* — a value Ghidra lost, a residue name left by a fold — is missing from that line, so at runtime it
resolves to a global: nil on read (the same value), but a write escapes the function and is visible to every
other script in the Lua state. `smoke_run_unit.free_globals` reports these; this pass declares them.

Only names that look like a lifted temporary are declared. Anything else free in the file is a real gap (a
missing helper, a host global the package forgot to require) and must stay visible.
"""
from __future__ import annotations

import re

# the spellings the lifter and the converter give a native slot or register
TEMPORARY = re.compile(
    r'(?:[A-Za-z]{1,4}Var\d+\w*|[A-Za-z]*Stack_[0-9a-f]+\w*|\w*_stk_[0-9a-f]+\w*|ctr_[0-9a-f]+\w*'
    r'|local_[0-9a-f]+\w*|this_\d+|fret_\d+\w*|p\d+\w*|p[A-Z]\w*|r\d+\w*|scratchValue\d*|uVar\d+\w*)\Z')
# `this`, `unaff_EBP`, `__unknown_push` are deliberately NOT temporaries: each is a decompiler or lifter gap,
# and declaring it would hide the evidence behind a silent nil.
FUNCTION_SPLIT = re.compile(r'(?m)^(?=(?:local )?function )')
LOCAL_LINE = re.compile(r'^(?P<ind>[ \t]*)local (?P<names>[A-Za-z_]\w*(?:, [A-Za-z_]\w*)*)\s*$')


def _blanked(text):
    """Comments and string bodies removed, so an identifier inside them is not a use."""
    text = re.sub(r'--\[\[.*?\]\]|--[^\n]*', '', text, flags=re.S)
    return re.sub(r'"(?:[^"\\]|\\.)*"|\'(?:[^\'\\]|\\.)*\'', '""', text)


def _declared(text):
    names = set(re.findall(r'^(?:local )?function (\w+)\(', text, re.M))
    for m in re.finditer(r'\blocal\s+(?:function\s+(\w+)|([\w\s,]+?))(?=\s*(?:=|\n|$))', text):
        names.update(re.findall(r'\w+', m.group(1) or m.group(2) or ''))
    for m in re.finditer(r'\bfunction\s*[\w.:]*\s*\(([^)]*)\)', text):
        names.update(re.findall(r'\w+', m.group(1)))
    for m in re.finditer(r'\bfor\s+([\w\s,]+?)\s*(?:=|\bin\b)', text):
        names.update(re.findall(r'\w+', m.group(1)))
    return names


def _used(text):
    return {m.group(1) for m in re.finditer(r'(?<![\w.:])([A-Za-z_]\w*)\b(?!\s*[:(])', text)}


def declare_free_locals(source: str) -> tuple[str, dict]:
    """Add every undeclared lifted temporary to its function's `local` line. Returns (source, {fn: [names]})."""
    parts = FUNCTION_SPLIT.split(source)
    file_declared = _declared(_blanked(parts[0])) if parts and not parts[0].startswith(('function ', 'local function ')) else set()
    out, added = [], {}
    for part in parts:
        if not part.startswith(('function ', 'local function ')):
            out.append(part)
            continue
        body = _blanked(part)
        free = sorted(n for n in _used(body) - _declared(body) - file_declared if TEMPORARY.fullmatch(n))
        if not free:
            out.append(part)
            continue
        lines = part.split('\n')
        name = re.match(r'(?:local )?function (\w+)', part).group(1)
        for i, line in enumerate(lines[:6]):
            m = LOCAL_LINE.fullmatch(line)
            if m:
                lines[i] = f"{m.group('ind')}local " + ', '.join(sorted(set(m.group('names').split(', ')) | set(free)))
                break
        else:
            lines.insert(1, '    local ' + ', '.join(free))
        added[name] = free
        out.append('\n'.join(lines))
    return ''.join(out), added
