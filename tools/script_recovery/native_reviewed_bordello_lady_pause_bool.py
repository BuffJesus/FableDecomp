"""Recover BordelloLady's post-control movie pause boolean."""
import hashlib

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_REG_EDI


OLD = 'quest:PauseAllNonScriptedEntities(nil --[[missing]])'
NEW = 'quest:PauseAllNonScriptedEntities(false)'


def restore(source, todo, fn, row, rdata):
    if row is None:
        return source, todo

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed BordelloLady pause boolean {fn["address"]}: {why}')

    start = int(fn['address'], 16)
    size = int(fn['bodyEndExclusive'], 16) - start
    raw = rdata.bytes_at(start, size)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'],
            'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
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
    # The branch after AcquireControl reaches E40392. The earlier loop
    # termination reaches E40359 and already has a correctly lifted false.
    branch = indexed[0xE3EDD5]
    require(branch.operands[0].type == X86_OP_IMM
            and branch.operands[0].imm == 0xE40392
            and row['nativeCall'].lower() == '0x00e40397',
            'post-control branch changed')
    native_calls = [call for call in fn['indirectCalls'] if call['site'].lower() == row['nativeCall'].lower()]
    require(len(native_calls) == 1 and native_calls[0]['slot'] == '0x5ec',
            'native pause call changed')
    require(all(X86_REG_EDI not in ins.regs_access()[1] for ins in instructions
                if 0xE3EB1F < ins.address < 0xE3EDD5),
            'pause argument register changed before branch')
    require(source.count(OLD) == 1 and todo.count(row['diagnostic']) == 1,
            'lifted pause call changed')
    todo.remove(row['diagnostic'])
    return source.replace(OLD, NEW, 1), todo
