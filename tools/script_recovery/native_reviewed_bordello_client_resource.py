"""Recover BordelloClient's parent `seh_Whore` resource assignment."""
import hashlib

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG, X86_REG_EBP, X86_REG_ECX, X86_REG_ESP


OLD_ASSIGN = ('-- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= '
              '(resources:MemberResource("seh_Whore"),&xStack_120);')
NEW_ASSIGN = 'resources:AssignResource(resources:MemberResource("seh_Whore"), xStack_118)'
OLD_THING = 'resources:ScriptThing(xStack_120)'
NEW_THING = 'resources:ScriptThing(xStack_118)'


def restore(source, todo, fn, row, rdata):
    if row is None:
        return source, todo

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed BordelloClient resource {fn["address"]}: {why}')

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
    for site, target in row['nativeCalls'].items():
        calls = [call for call in fn['calls'] if call['site'].lower() == site.lower()]
        require(len(calls) == 1 and calls[0]['target'].lower() == target.lower(),
                f'native target changed at {site}')
    ctor = indexed[0xE44EF3]
    assign = indexed[0xE45FBF]
    require(ctor.mnemonic == assign.mnemonic == 'lea'
            and ctor.operands[0].type == assign.operands[0].type == X86_OP_REG
            and ctor.operands[0].reg == assign.operands[0].reg == X86_REG_ECX
            and all(ins.operands[1].type == X86_OP_MEM
                    and ins.operands[1].mem.base == X86_REG_ESP
                    and ins.operands[1].mem.disp == 0x14 for ins in (ctor, assign)),
            'constructor and assignment no longer address the same stack slot')
    for site in (0xE46142, 0xE462A5):
        ins = indexed[site]
        require(ins.mnemonic == 'lea' and ins.operands[1].type == X86_OP_MEM
                and ins.operands[1].mem.base == X86_REG_ESP
                and ins.operands[1].mem.disp == 0x18
                and indexed[site - 1].mnemonic == 'push',
                f'GetScriptThing receiver changed at {site:x}')
    parent = indexed[0xE45FC4]
    offset = indexed[0xE45FC7]
    require(parent.mnemonic == 'mov' and parent.operands[1].type == X86_OP_MEM
            and parent.operands[1].mem.base == X86_REG_EBP
            and parent.operands[1].mem.disp == 0x14
            and offset.mnemonic == 'add' and offset.operands[1].type == X86_OP_IMM
            and offset.operands[1].imm == 0xA4,
            'parent seh_Whore member changed')
    require(source.count('xStack_118 = resources:NewResource()') == 1
            and source.count('resources:TryAcquire(xStack_118, me, 4)') >= 1
            and source.count(OLD_ASSIGN) == 1
            and source.count(OLD_THING) == 1
            and source.count('xStack_120') == 2
            and todo.count(row['diagnostic']) == 1,
            'lifted resource alias changed')
    todo.remove(row['diagnostic'])
    return source.replace(OLD_ASSIGN, NEW_ASSIGN, 1).replace(OLD_THING, NEW_THING, 1), todo
