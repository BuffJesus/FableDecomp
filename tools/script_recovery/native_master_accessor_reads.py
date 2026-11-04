"""Recover byte comparisons reached through a proven trivial master getter.

The misleading propagated getter name is not evidence. Require retail
MOV EAX,[ECX+master_offset];RET and an adjacent caller CMP BYTE [EAX+field],0/1.
Only a mapped Bool field and a direct self receiver can use this recovery.
"""
import re
import struct


def restore(source, fn, spec, rdata):
    if spec.entity or not 0 <= spec.master_off < 128:
        return source
    getter = bytes((0x8b, 0x41, spec.master_off, 0xc3))
    for label, target in getattr(spec, 'call_labels', {}).items():
        if rdata.bytes_at(target, 4) != getter:
            continue
        calls = [c for c in fn.get('calls', []) if int(str(c['target']), 16) == target]
        # Multiple calls can associate a textual match with the wrong native site.
        if len(calls) != 1:
            continue
        site = int(str(calls[0]['site']), 16)
        call = rdata.bytes_at(site, 5)
        if not call or call[0] != 0xe8 or site + 5 + struct.unpack('<i', call[1:])[0] != target:
            continue
        compare = rdata.bytes_at(site + 5, 4)
        if not compare or compare[:2] != b'\x80\x78' or compare[3] not in (0, 1):
            continue
        offset, value = compare[2], compare[3]
        if offset >= 128:  # ModRM disp8 is signed.
            continue
        field = spec.master_fields.get(offset)
        if not field or field[1] != 'Bool':
            continue
        pattern = (r'(?P<ind>^[ \t]*)(?P<var>\w+) = ' + re.escape(label)
                   + r'\(this\);\r?\n(?P=ind)if \((?P=var)\['
                   + r'(?:' + hex(offset) + '|' + str(offset) + r')\] == '
                   + r'(?:\(\w+\))?(?:' + hex(value) + '|' + str(value) + r')\) \{')
        matches = list(re.finditer(pattern, source, re.M))
        if len(matches) != 1:
            continue
        match = matches[0]
        # The value must not escape this one comparison before the slot is reused.
        tail = source[match.end():]
        use = re.search(r'\b' + re.escape(match['var']) + r'\b', tail)
        if use and not re.match(re.escape(match['var']) + r' = ', tail[use.start():]):
            continue
        replacement = (f'{match["ind"]}if (*(byte *)(*(int *)(this + {spec.master_off:#x})'
                       f' + {offset:#x}) == {value}) {{')
        source = source[:match.start()] + replacement + source[match.end():]
    return source
