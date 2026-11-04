"""Restore Madame's self receiver for her initial pushability setting."""
import hashlib

from capstone import Cs, CS_ARCH_X86, CS_MODE_32


OLD = 'quest:SetIsPushableByHero(nil --[[missing]], (uVar20 ~= 0))'
NEW = 'quest:SetIsPushableByHero(me, (uVar20 ~= 0))'


def restore(source, todo, fn, row, rdata):
    if row is None:
        return source, todo

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed Madame pushability {fn["address"]}: {why}')

    start = int(fn['address'], 16)
    size = int(fn['bodyEndExclusive'], 16) - start
    raw = rdata.bytes_at(start, size)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'],
            'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'exported source changed')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    instructions = list(decoder.disasm(raw, start))
    require(instructions and instructions[-1].address + instructions[-1].size == start + size,
            'native disassembly incomplete')
    indexed = {ins.address: ins for ins in instructions}
    for address, encoded in row['nativeInstructions'].items():
        ins = indexed.get(int(address, 16))
        require(ins is not None and ins.bytes.hex() == encoded,
                f'native instruction changed at {address}')
    calls = [call['site'].lower() for call in fn['indirectCalls'] if call['slot'] == '0xd30']
    require(calls == ['0x00e3bc28'], 'pushability call inventory changed')
    require(source.count(OLD) == 1 and todo.count(row['diagnostic']) == 1,
            'lifted pushability call changed')
    todo.remove(row['diagnostic'])
    return source.replace(OLD, NEW, 1), todo
