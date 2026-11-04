"""Restore BordelloClient's four cleanup unpause calls."""
import hashlib

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM


CALL = 'quest:PauseAllNonScriptedEntities(false)'
OLD = {
    register: f'-- TODO(native): (**(code **)(*unaff_{register} + 0x5ec))();'
    for register in ('EBX', 'EBP')
}
DIAGNOSTIC = {
    register: f'unlifted: (**(code **)(*unaff_{register} + 0x5ec))();'
    for register in ('EBX', 'EBP')
}


def restore(source, todo, fn, row, rdata):
    if row is None:
        return source, todo

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed BordelloClient pause {fn["address"]}: {why}')

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
    for address, encoded in row['nativeInstructions'].items():
        ins = indexed.get(int(address, 16))
        require(ins is not None and ins.bytes.hex() == encoded,
                f'native instruction changed at {address}')
    for push_site, call_site in row['pauseCalls']:
        calls = [call for call in fn['indirectCalls'] if call['site'].lower() == call_site.lower()]
        require(len(calls) == 1 and calls[0]['slot'] == '0x5ec',
                f'native pause call changed at {call_site}')
        push = indexed[int(push_site, 16)]
        require(push.mnemonic == 'push' and push.operands[0].type == X86_OP_IMM
                and push.operands[0].imm == 0
                and push.address + push.size == int(call_site, 16),
                f'unpause operand changed at {push_site}')
    require(len(row['pauseCalls']) == 4, 'reviewed call count changed')
    for register in ('EBX', 'EBP'):
        require(source.count(OLD[register]) == 2
                and todo.count(DIAGNOSTIC[register]) == 2,
                f'lifted {register} pause calls changed')
        source = source.replace(OLD[register], CALL)
        for _ in range(2):
            todo.remove(DIAGNOSTIC[register])
    return source, todo
