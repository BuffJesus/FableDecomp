"""Recover Euclidean Thing-distance returns and proven caller float stores."""
import hashlib
import re
import struct

TARGET = 0xCBE512
DIGEST = '43379ac1f04039392dcd78b5a40e06052c5ccf32332bf0e0220bd46a9d105645'


def restore(text, function, rdata):
    if not function or rdata is None:
        return text
    calls = [c for c in function.get('calls', []) if int(c.get('target', '0'), 16) == TARGET]
    if not calls:
        return text
    if hashlib.sha256(rdata.bytes_at(TARGET, 63) or b'').hexdigest() != DIGEST:
        return text
    from tools.script_recovery.native_string_returns import _target_sites
    from tools.script_recovery.native_evidence_lowering import _strip_addr
    entries = _target_sites(text, function, calls)
    if len(entries) != len(calls):
        return text
    edits = []
    for a, e, args, site, label, _ in entries:
        address = int(site['site'], 16)
        code = rdata.bytes_at(address, 32) or b''
        if len(args) != 2 or len(code) != 32 or code[0] != 0xE8:
            continue
        if address + 5 + struct.unpack('<i', code[1:5])[0] != TARGET:
            continue
        # x87 FSTP m32 through ESP, FCOMP m32, FMUL m32 or discarded ST(0).
        following = code[5:]
        stored = following.startswith(b'\xd9\x5c\x24') or following.startswith(b'\xd9\x9c\x24')
        raw = following.startswith(b'\xd8\x5c\x24') or following.startswith(b'\xd8\x9c\x24') or following.startswith(b'\xd8\x0d') or following.startswith(b'\xdd\xd8')
        # Retained comparison followed by a conditional store into the same
        # stack float. Pin the instruction shape and source data flow together.
        conditional = re.fullmatch(rb'\xd8\x54\x24(.)\x8b\x74\x24.\xdf\xe0\xf6\xc4\x05\x7a.\x8b\x4b.\xd9\x5c\x24\1', following[:22], re.S)
        head = re.search(re.escape(label).replace('::', r'\s*::\s*') + r'\s*\($', text[:a])
        if head is None:
            continue
        if conditional:
            lhs = re.search(r'\b(\w+)\s*=\s*$', text[:head.start()])
            if lhs is None:
                continue
            value = lhs[1]
            branch = re.match(r'\);\s*if\s*\(' + re.escape(value) + r'\s*<\s*(\w+)\)\s*\{', text[e:])
            if branch is None:
                continue
            destination = branch[1]
            slot = int(site['depth']) - following[3]
            if not re.fullmatch(r'(?:\w*Stack_|local_)' + format(slot, 'x'), destination):
                continue
            tail_start = e + branch.end()
            tail_end = text.find('}', tail_start)
            assignment = re.compile(r'\b' + re.escape(destination) + r'\s*=\s*' + re.escape(value) + r';')
            matches = list(assignment.finditer(text, tail_start, tail_end))
            if len(matches) != 1:
                continue
            m = matches[0]
            edits.append((m.start(), m.end(), f'{destination} = ENGINE_DistanceFloatStore({value});'))
        elif not stored and not raw:
            continue
        name = 'ENGINE_RetailDistanceFloat32' if stored else 'ENGINE_RetailDistance'
        edits.append((head.start(), e + 1, f'{name}({", ".join(_strip_addr(v) for v in args)})'))
    for a, e, replacement in sorted(edits, reverse=True):
        text = text[:a] + replacement + text[e:]
    return text
