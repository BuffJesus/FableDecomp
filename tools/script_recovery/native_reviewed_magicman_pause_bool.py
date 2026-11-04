"""Recover Magicman's pause argument from the retail cleanup call sites."""
import hashlib

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_REG_ESI


WRONG = 'quest:PauseAllNonScriptedEntities(0x126008c)'
RIGHT = 'quest:PauseAllNonScriptedEntities(false)'


def restore(source, fn, row, rdata):
    if row is None:
        return source

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed Magicman pause argument {fn["address"]}: {why}')

    start = int(fn['address'], 16)
    size = int(fn['bodyEndExclusive'], 16) - start
    raw = rdata.bytes_at(start, size)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'],
            'native body changed')
    decompile = fn['decompile']
    require(hashlib.sha256(decompile.encode()).hexdigest() == row['sourceSha256'],
            'exported source changed')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw, start))
    require(instructions and instructions[-1].address + instructions[-1].size == start + size,
            'native disassembly incomplete')
    indexed = {ins.address: ins for ins in instructions}
    for site, encoded in row['nativeInstructions'].items():
        ins = indexed.get(int(site, 16))
        require(ins is not None and ins.bytes.hex() == encoded,
                f'native instruction changed at {site}')

    # Two early script-task exits converge at E44615. The first-beer movie
    # exits converge at E4483D/E4488D. Each path zeroes ESI before the
    # conditional jump and each target pushes ESI to virtual slot +0x5ec.
    for zero, branch in ((0xE417D2, 0xE41869), (0xE4195F, 0xE419F2),
                         (0xE4282D, 0xE428C1), (0xE4282D, 0xE428DD)):
        between = [ins for ins in instructions if zero < ins.address < branch]
        require(all(X86_REG_ESI not in ins.regs_access()[1] for ins in between),
                f'ESI changed before branch {branch:08x}')
    require(source.count(WRONG) == row['liftedCallCount'],
            'lifted pause calls changed')
    return source.replace(WRONG, RIGHT)
