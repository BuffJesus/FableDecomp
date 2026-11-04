"""Restore BordelloLady's four retail cutscene skip flags."""
import hashlib
import re

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG, X86_REG_EDI


CALL = re.compile(r'quest:SetCutsceneSkippable\((?:nil --\[\[missing\]\]|true|false)\)')
BEFORE = [
    'quest:SetCutsceneSkippable(nil --[[missing]])',
    'quest:SetCutsceneSkippable(nil --[[missing]])',
    'quest:SetCutsceneSkippable(true)',
    'quest:SetCutsceneSkippable(nil --[[missing]])',
]
AFTER = [
    'quest:SetCutsceneSkippable(false)',
    'quest:SetCutsceneSkippable(true)',
    'quest:SetCutsceneSkippable(false)',
    'quest:SetCutsceneSkippable(true)',
]


def restore(source, todo, fn, row, rdata):
    if row is None:
        return source, todo

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed BordelloLady skip flags {fn["address"]}: {why}')

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
    indexed = {ins.address: i for i, ins in enumerate(instructions)}
    expected = {int(site, 16) for site in row['nativeCallsInSourceOrder']}
    actual = {int(call['site'], 16) for call in fn['indirectCalls'] if call['slot'] == '0x60c'}
    require(actual == expected and len(expected) == 4, 'native skip-call inventory changed')
    require(fn['decompile'].count('+ 0x60c') == 4, 'decompiled skip-call inventory changed')
    values = []
    for site in row['nativeCallsInSourceOrder']:
        i = indexed.get(int(site, 16))
        require(i is not None and i > 0, f'native site {site} missing')
        call, push = instructions[i], instructions[i-1]
        require(call.mnemonic == 'call' and call.operands[0].type == X86_OP_MEM
                and call.operands[0].mem.disp == 0x60C and push.mnemonic == 'push',
                f'native call shape changed at {site}')
        operand = push.operands[0]
        if operand.type == X86_OP_IMM:
            value = operand.imm
        else:
            require(operand.type == X86_OP_REG and operand.reg == X86_REG_EDI,
                    f'native skip operand changed at {site}')
            require(instructions[indexed[0xE3EB1F]].mnemonic == 'xor'
                    and all(X86_REG_EDI not in ins.regs_access()[1]
                            for ins in instructions if 0xE3EB1F < ins.address < int(site, 16)),
                    'zeroed skip register changed')
            value = 0
        require(value in (0, 1), f'nonboolean skip operand at {site}')
        values.append(bool(value))
    require(values == [False, True, False, True], 'skip flag sequence changed')
    matches = list(CALL.finditer(source))
    require([m.group() for m in matches] == BEFORE, 'lifted skip calls changed')
    require(todo.count(row['diagnostic']) == 3, 'skip diagnostics changed')
    for _ in range(3):
        todo.remove(row['diagnostic'])
    choices = iter(AFTER)
    return CALL.sub(lambda _: next(choices), source), todo
