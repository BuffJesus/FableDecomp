"""Restore BordelloLady's thing argument when clearing its information flag."""
import hashlib

from capstone import Cs, CS_ARCH_X86, CS_MODE_32


OLD = 'quest:ClearThingHasInformation(nil --[[missing]])'
NEW = 'quest:ClearThingHasInformation(me)'


def restore(source, todo, fn, row, rdata):
    if row is None:
        return source, todo

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed BordelloLady information operand {fn["address"]}: {why}')

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
    native_calls = [call for call in fn['indirectCalls'] if call['slot'] == '0x5a4']
    require(len(native_calls) == 1 and native_calls[0]['site'].lower() == row['nativeCall'].lower(),
            'native information call changed')
    count = row['liftedCallCount']
    require(source.count(OLD) == count and todo.count(row['diagnostic']) == count,
            'lifted information calls changed')
    for _ in range(count):
        todo.remove(row['diagnostic'])
    return source.replace(OLD, NEW), todo
