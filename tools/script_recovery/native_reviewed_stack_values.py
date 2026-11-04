"""Restore reviewed by-value scalar uses that Ghidra printed as an object."""
import hashlib


def restore(text, fn, row, rdata, source_sites):
    """Apply exact scalar-value edits only while native/source evidence matches."""
    if row is None:
        return text
    start = int(fn['address'], 16)
    size = int(fn['bodyEndExclusive'], 16) - start
    raw = rdata.bytes_at(start, size)
    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed stack value {fn["address"]}: {why}')
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'],
            'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'exported source changed')
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    instructions = list(Cs(CS_ARCH_X86, CS_MODE_32).disasm(raw, start))
    require(instructions and instructions[-1].address + instructions[-1].size == start + size,
            'native disassembly incomplete')
    by_site = {ins.address: ins for ins in instructions}
    require(len(row['nativeInstructions']) >= 4, 'too few native instruction anchors')
    for site, encoded in row['nativeInstructions'].items():
        ins = by_site.get(int(site, 16))
        require(ins is not None and ins.bytes.hex() == encoded,
                f'native instruction changed at {site}')
    calls = {int(c['site'], 16): c for c in fn['calls']}
    printed = {int(site['site'], 16): args[1] for _, _, args, site, _, _ in source_sites or []
               if len(args) > 1 and int(site.get('target', '0'), 16) == int(row['conversionTarget'], 16)}
    require(len(row['conversionSites']) >= 2, 'too few conversion sites')
    for site, spelling in row['conversionSites'].items():
        address = int(site, 16)
        require(address in calls and int(calls[address]['target'], 16) == int(row['conversionTarget'], 16)
                and printed.get(address) == spelling,
                f'conversion site changed at {site}')
    require(len(row['edits']) >= 1, 'no reviewed edits')
    for old, new, count in row['edits']:
        require(count > 0 and old != new and '\n' not in old + new,
                'invalid reviewed edit')
        require(text.count(old) == count, f'expected {count} occurrences of {old!r}')
        text = text.replace(old, new)
    return text
