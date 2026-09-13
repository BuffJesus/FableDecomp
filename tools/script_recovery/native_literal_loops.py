"""Recover bounded byte-comparison loops only when their reads and escaping results are proven."""
from __future__ import annotations

import re

from tools.script_recovery.native_local_liveness import dead_after_marker

_NON_CODE = re.compile(r'"(?:[^"\\]|\\.)*"|\'(?:[^\'\\]|\\.)*\'|//[^\n]*|/\*.*?\*/', re.S)

# Ghidra's inline bounded comparison. All operands and updates must refer to the same four
# distinct locals. Literal bytes are deliberately ASCII without C escapes for this first shape.
_COMPARE = re.compile(
    r'\b(?P<count>iVar\d+)\s*=\s*(?P<limit>0x[0-9a-fA-F]+|[0-9]+);\s*'
    r'(?P<flag>bVar\d+)\s*=\s*true;\s*'
    r'(?P<left>pcVar\d+)\s*=\s*"(?P<a>[\x20-\x21\x23-\x5b\x5d-\x7e]*)";\s*'
    r'(?P<right>pcVar\d+)\s*=\s*"(?P<b>[\x20-\x21\x23-\x5b\x5d-\x7e]*)";\s*'
    r'do\s*\{\s*'
    r'if\s*\(\s*(?P=count)\s*==\s*0\s*\)\s*break;\s*'
    r'(?P=count)\s*=\s*(?P=count)\s*\+\s*-1;\s*'
    r'(?P=flag)\s*=\s*\*(?P=left)\s*==\s*\*(?P=right);\s*'
    r'(?P=left)\s*=\s*(?P=left)\s*\+\s*1;\s*'
    r'(?P=right)\s*=\s*(?P=right)\s*\+\s*1;\s*'
    r'\}\s*while\s*\(\s*(?P=flag)\s*\)\s*;')


def fold_literal_comparisons(source: str, normalize=None) -> tuple[str, int]:
    """Return equivalent scalar assignments and number of proven loops.

    The count and equality flag remain observable. Pointer updates may be removed only when the
    pointers are otherwise unused or control-flow analysis proves overwrite before every later
    read, without escaped addresses. Never read beyond either
    literal's terminating NUL, even if the compiler laid more readable bytes after it in retail.
    """
    recovered = 0
    protected = [(match.start(), match.end()) for match in _NON_CODE.finditer(source)]

    def replace(match: re.Match) -> str:
        nonlocal recovered
        if any(start <= match.start() < end for start, end in protected):
            return match[0]
        if len({match['count'], match['flag'], match['left'], match['right']}) != 4:
            return match[0]
        outside = source[:match.start()] + source[match.end():]
        pointers = (match['left'], match['right'])
        otherwise_used = False
        for pointer in pointers:
            outside = re.sub(rf'(?m)^\s*char\s*\*\s*{pointer}\s*;\s*$', '', outside)
            if re.search(rf'\b{pointer}\b', outside):
                otherwise_used = True
        if otherwise_used:
            marker = '__native_literal_comparison_marker();'
            if normalize is None or marker in source:
                return match[0]
            candidate = source[:match.start()] + marker + source[match.end():]
            if not dead_after_marker(normalize(candidate), marker, pointers):
                return match[0]
        limit = int(match['limit'], 16 if match['limit'].lower().startswith('0x') else 10)
        # iVar is the native signed 32-bit loop count. Other bit patterns stay unresolved.
        if limit > 0x7fffffff:
            return match[0]
        left = match['a'].encode('ascii') + b'\0'
        right = match['b'].encode('ascii') + b'\0'
        count, equal, index = limit, True, 0
        while count and equal:
            if index >= len(left) or index >= len(right):
                return match[0]
            count -= 1
            equal = left[index] == right[index]
            index += 1
        recovered += 1
        return f"{match['count']} = {count};\n{match['flag']} = {'true' if equal else 'false'};"

    return _COMPARE.sub(replace, source), recovered
