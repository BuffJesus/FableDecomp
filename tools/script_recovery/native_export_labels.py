"""Give exported partial-return labels spellings understood by the C flow passes."""
import re

_OPAQUE = re.compile(r'//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'', re.S)
_LABEL = re.compile(r'^[ \t]*(override_prt_([0-9a-f]{6,8})_[0-9a-f]{8}):(?!:)', re.M)


def normalize(text):
    # This is a bijective rename, not a reconstruction of a branch. Only a
    # unique actual definition and its goto references qualify; quoted text
    # and comments are opaque. Existing LAB spellings must never be merged.
    masked = _OPAQUE.sub(lambda m: re.sub(r'[^\r\n]', ' ', m[0]), text)
    labels = list(_LABEL.finditer(masked))
    names = set(re.findall(r'\b\w+\b', masked))
    edits = []
    for match in labels:
        old, address = match[1], match[2]
        if sum(m[1] == old for m in labels) != 1:
            continue
        jumps = list(re.finditer(r'\bgoto\s+(' + re.escape(old) + r')\s*;', masked))
        if len(re.findall(r'\b' + re.escape(old) + r'\b', masked)) != len(jumps) + 1:
            continue
        base = f'LAB_{int(address, 16):08x}'
        new, serial = base, 0
        while new in names:
            serial += 1
            new = f'{base}_c{serial}'
        names.add(new)
        edits.append((match.start(1), match.end(1), new))
        for jump in jumps:
            edits.append((jump.start(1), jump.end(1), new))
    for start, end, value in sorted(edits, reverse=True):
        text = text[:start] + value + text[end:]
    return text
