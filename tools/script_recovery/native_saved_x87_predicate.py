"""Prove a bounded ordered x87 predicate saved across one normal cleanup call.

The cleanup must preserve caller locals and return normally without reentry.
Its identity/footprint, incoming CFG edges and the producer ABI are caller gates.
No decompiler comparison is used as evidence of floating-point ordering.
"""
import hashlib
import math
import struct

from capstone import Cs, CS_ARCH_X86, CS_MODE_32, x86 as x


def prove(raw, start, read_bytes, cleanup_target):
    def require(ok, why):
        if not ok:
            raise ValueError('saved x87 predicate: ' + why)

    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    code = list(decoder.disasm(raw, start))
    require(len(code) == 11 and sum(i.size for i in code) == len(raw), 'instruction extent')
    require([i.mnemonic for i in code] == [
        'fcomp', 'mov', 'fnstsw', 'test', 'je', 'mov', 'lea', 'call', 'mov', 'test', 'je'],
        'instruction sequence')

    def reg(op, value):
        return op.type == x.X86_OP_REG and op.reg == value

    def imm(op, value):
        return op.type == x.X86_OP_IMM and op.imm == value

    def local(op, size):
        return (op.type == x.X86_OP_MEM and op.size == size
                and op.mem.base == x.X86_REG_ESP and not op.mem.index
                and op.mem.segment == 0 and 0 <= op.mem.disp < 4096)

    compare, yes, status, mask, join, no, receiver, cleanup, reload, test, branch = code
    operand = compare.operands[0]
    require(operand.type == x.X86_OP_MEM and operand.size == 4
            and operand.mem.base == 0 and operand.mem.index == 0
            and operand.mem.segment == 0, 'threshold operand')
    constant = read_bytes(operand.mem.disp, 4)
    require(constant is not None and len(constant) == 4, 'threshold unavailable')
    threshold = struct.unpack('<f', constant)[0]
    require(math.isfinite(threshold), 'nonfinite threshold')
    require(local(yes.operands[0], 1) and imm(yes.operands[1], 1), 'positive flag store')
    require(reg(status.operands[0], x.X86_REG_AX), 'status register')
    require(reg(mask.operands[0], x.X86_REG_AH) and imm(mask.operands[1], 0x41), 'ordered status mask')
    # C0/C3: less=01, equal=40, greater=00, unordered=45. Only greater
    # leaves the initialized byte set. In particular unordered must clear it.
    require(imm(join.operands[0], receiver.address), 'positive join')
    require(local(no.operands[0], 1) and no.operands[0].mem.disp == yes.operands[0].mem.disp
            and imm(no.operands[1], 0), 'negative flag store')
    require(reg(receiver.operands[0], x.X86_REG_ECX) and local(receiver.operands[1], 4), 'cleanup receiver')
    require(imm(cleanup.operands[0], cleanup_target), 'cleanup identity')
    require(reg(reload.operands[0], x.X86_REG_AL) and local(reload.operands[1], 1)
            and reload.operands[1].mem.disp == yes.operands[0].mem.disp, 'saved flag reload')
    require(reg(test.operands[0], x.X86_REG_AL) and reg(test.operands[1], x.X86_REG_AL), 'saved flag test')
    require(branch.operands[0].type == x.X86_OP_IMM, 'false exit')
    end = start + len(raw)
    false_exit = branch.operands[0].imm
    require(not start <= false_exit <= end, 'non-disjoint false exit')
    flag = yes.operands[0].mem.disp
    thing = receiver.operands[1].mem.disp
    require(not thing <= flag < thing + 12, 'flag overlaps cleanup object')
    return dict(start=hex(start), endExclusive=hex(end), nativeSha256=hashlib.sha256(raw).hexdigest(),
                thresholdAddress=hex(operand.mem.disp), thresholdBytes=constant.hex(),
                threshold=threshold, positiveExit=hex(end), falseExit=hex(false_exit),
                flagOffset=flag, cleanupOffset=thing, cleanupTarget=hex(cleanup_target),
                relation='ordered-greater', unordered=False)
