"""Restore BordelloLady's Name field reload before the Hedwig cutscene branch."""
import hashlib

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_MEM, X86_OP_REG, X86_REG_EBP, X86_REG_ESI


OLD = ('-- TODO(native): CVar3 = *__native_entity_state:GetStateString("Name")\n'
       '                        CVar3 = nil --[[unresolved native value]]')
NEW = 'CVar3 = __native_entity_state:GetStateString("Name")'


def restore(source, todo, fn, row, rdata, fields, native):
    if row is None:
        return source, todo

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed BordelloLady Name load {fn["address"]}: {why}')

    require(fields.get('0x28') == ['Name', 'String'], 'Name field mapping changed')
    require(rdata.bytes_at(0x12E3AE4, 7) == b'HEDWIG\0', 'comparison literal changed')
    for function, native_hash, source_hash in ((fn, row['nativeSha256'], row['sourceSha256']),
                                               (native('0x00e3ac70'), row['initNativeSha256'], row['initSourceSha256'])):
        start = int(function['address'], 16)
        size = int(function['bodyEndExclusive'], 16) - start
        raw = rdata.bytes_at(start, size)
        require(raw is not None and hashlib.sha256(raw).hexdigest() == native_hash,
                f'native body changed at {start:x}')
        require(hashlib.sha256(function['decompile'].encode()).hexdigest() == source_hash,
                f'exported source changed at {start:x}')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    start = int(fn['address'], 16)
    end = int(fn['bodyEndExclusive'], 16)
    main_ins = list(decoder.disasm(rdata.bytes_at(start, end-start), start))
    require(main_ins and main_ins[-1].address + main_ins[-1].size == end,
            'Main disassembly incomplete')
    indexed = {ins.address: ins for ins in main_ins}
    field = indexed[0xE3FB9F]
    read = indexed[0xE3FD12]
    require(field.bytes.hex() == '8d7528' and field.mnemonic == 'lea'
            and field.operands[0].reg == X86_REG_ESI
            and field.operands[1].type == X86_OP_MEM
            and field.operands[1].mem.base == X86_REG_EBP
            and field.operands[1].mem.disp == 0x28
            and read.bytes.hex() == '8b36' and read.operands[0].reg == X86_REG_ESI
            and read.operands[1].type == X86_OP_MEM
            and read.operands[1].mem.base == X86_REG_ESI,
            'Main Name field load changed')
    require(all(X86_REG_ESI not in ins.regs_access()[1]
                for ins in main_ins if 0xE3FB9F < ins.address < 0xE3FD12
                and not (0xE3FC7B <= ins.address < 0xE3FD05)),
            'Name pointer changed on target branch')
    init = native('0x00e3ac70')
    init_start = int(init['address'], 16)
    init_end = int(init['bodyEndExclusive'], 16)
    init_ins = {ins.address: ins for ins in decoder.disasm(rdata.bytes_at(init_start, init_end-init_start), init_start)}
    require(init_ins[0xE3AC96].bytes.hex() == '8d5d28'
            and init_ins[0xE3AC99].bytes.hex() == 'ff500c'
            and init_ins[0xE3AC9F].bytes.hex() == 'e80c43b6ff',
            'Init Name assignment changed')
    require(source.count(OLD) == 1 and todo.count(row['diagnostic']) == 1,
            'lifted Name load changed')
    todo.remove(row['diagnostic'])
    return source.replace(OLD, NEW, 1), todo
