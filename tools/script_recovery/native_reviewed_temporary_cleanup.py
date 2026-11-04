"""Drop reviewed C++ temporary destructors that Lua string lifetimes replace."""
import hashlib


def restore(text, fn, row, rdata):
    """Remove only the pinned construction flag and its matching destructor.

    The C++ compiler guards destruction of a temporary CCharString with a
    stack bit. Lua's string value needs no explicit destructor, and preserving
    the bit test can turn Ghidra's drifted cleanup word into a nil Lua local.
    A row must prove the constructor, flag set/test/clear and destructor before
    this pass can remove their C scaffolding.
    """
    if row is None:
        return text
    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed temporary cleanup {fn["address"]}: {why}')
    start = int(fn['address'], 16)
    size = int(fn['bodyEndExclusive'], 16) - start
    raw = rdata.bytes_at(start, size)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'],
            'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'exported source changed')
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    instructions = list(Cs(CS_ARCH_X86, CS_MODE_32).disasm(raw, start))
    require(instructions and instructions[-1].address + instructions[-1].size == start + size,
            'native disassembly incomplete')
    by_site = {ins.address: ins for ins in instructions}
    if 'groups' in row:
        require(bool(row['groups']), 'empty cleanup groups')
        # Each exact block is reviewed as a complete compiler cleanup guard.
        # Match both export newline styles without changing surrounding source.
        edits = []
        for group in row['groups']:
            anchors = group['nativeInstructions']
            require(len(anchors) >= 5, 'too few native instruction anchors')
            for site, encoded in anchors.items():
                ins = by_site.get(int(site, 16))
                require(ins is not None and ins.bytes.hex() == encoded,
                        f'native instruction changed at {site}')
            spans = []
            for key in ('flagSet', 'guardBlock'):
                value = group[key].replace('\r\n', '\n')
                require(bool(value), 'empty cleanup source')
                variants = {value, value.replace('\n', '\r\n')}
                matches = [(text.index(v), v) for v in variants if v in text]
                require(len(matches) == 1 and text.count(matches[0][1]) == 1,
                        'cleanup source is missing or ambiguous')
                start_at, matched = matches[0]
                spans.append((start_at, start_at + len(matched)))
            require(spans[0][1] <= spans[1][0], 'cleanup flag order changed')
            edits.extend(spans)
        edits.sort()
        require(all(a[1] <= b[0] for a, b in zip(edits, edits[1:])),
                'overlapping cleanup groups')
        for begin, end in reversed(edits):
            text = text[:begin] + text[end:]
        return text
    require(len(row['nativeInstructions']) >= 5, 'too few native instruction anchors')
    for site, encoded in row['nativeInstructions'].items():
        ins = by_site.get(int(site, 16))
        require(ins is not None and ins.bytes.hex() == encoded,
                f'native instruction changed at {site}')
    flag_set = row['flagSet']
    guard = row['guard']
    require(text.count(flag_set) == 1 and text.count(guard) == 1,
            'cleanup source is missing or ambiguous')
    guard_start = text.index(guard)
    guard_end = text.find('\n        }', guard_start)
    require(guard_end >= 0, 'cleanup guard end missing')
    guard_end += len('\n        }')
    block = text[guard_start:guard_end]
    require(all(block.count(part) == 1 for part in row['guardParts']),
            'cleanup guard contents changed')
    require(text.index(flag_set) < guard_start, 'cleanup flag order changed')
    text = text[:guard_start] + text[guard_end:]
    return text.replace(flag_set, '')
