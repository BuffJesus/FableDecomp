"""Restore the BordelloVillage thing passed to BordelloLady's crime query."""
import hashlib

from capstone import Cs, CS_ARCH_X86, CS_MODE_32


OLD = 'quest:IsReportedOrUnreportedCrimeKnown(nil --[[missing]])'
NEW = 'quest:IsReportedOrUnreportedCrimeKnown(quest:GetStateThing("BordelloVillage"))'


def restore(source, todo, fn, row, rdata, thing_fields):
    if row is None:
        return source, todo

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed BordelloLady crime operand {fn["address"]}: {why}')

    require(thing_fields.get('0x5c') == 'BordelloVillage',
            'quest thing field changed')
    start = int(fn['address'], 16)
    size = int(fn['bodyEndExclusive'], 16) - start
    raw = rdata.bytes_at(start, size)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'],
            'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'exported source changed')
    instructions = list(Cs(CS_ARCH_X86, CS_MODE_32).disasm(raw, start))
    require(instructions and instructions[-1].address + instructions[-1].size == start + size,
            'native disassembly incomplete')
    indexed = {ins.address: ins for ins in instructions}
    for site, encoded in row['nativeInstructions'].items():
        ins = indexed.get(int(site, 16))
        require(ins is not None and ins.bytes.hex() == encoded,
                f'native instruction changed at {site}')
    require(source.count(OLD) == 1 and todo.count(row['diagnostic']) == 1,
            'lifted crime call changed')
    todo.remove(row['diagnostic'])
    return source.replace(OLD, NEW, 1), todo
