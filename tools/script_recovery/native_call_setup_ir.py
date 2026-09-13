"""Symbolic x86 argument setup within one straight-line instruction window.

Stack addresses are relative to this window's entry ESP, not global frame slots.
The caller must establish any relationship between windows separately.
"""
from dataclasses import dataclass

from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG


def read_call_window(rdata, function_start, function_size, call_site, known_calls=None, *, argument_count=None):
    """Find a straight-line setup window from aligned function disassembly."""
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    raw = rdata.bytes_at(function_start, function_size)
    if raw is None or len(raw) != function_size:
        return None
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw, function_start))
    if not instructions:
        return None
    decoded_end = instructions[-1].address + instructions[-1].size
    if decoded_end != function_start + function_size:
        # Some recovery exports count through only part of the final ADD ESP.
        # Accept an observed stack-restoration/RET tail only; arbitrary missing
        # instructions could contain unseen branches into our argument window.
        tail = rdata.bytes_at(decoded_end, 8)
        length = (7 if tail and tail[:2] == b'\x81\xc4' and tail[6:7] == b'\xc3' else
                  4 if tail and tail[:2] == b'\x83\xc4' and tail[3:4] == b'\xc3' else 0)
        if not length or decoded_end + length < function_start + function_size:
            return None
        instructions.extend(decoder.disasm(tail[:length], decoded_end))
    at = next((i for i, ins in enumerate(instructions) if ins.address == call_site and ins.mnemonic == 'call'), None)
    if at is None:
        return None
    first = at
    while first > 0:
        previous = instructions[first - 1]
        if (previous.mnemonic.startswith(('j', 'ret', 'loop'))
                or previous.mnemonic == 'call' and previous.address not in (known_calls or {})):
            break
        first -= 1
    positions = {ins.address: i for i, ins in enumerate(instructions)}
    branch_targets = []
    guarded_entries = []
    for index, ins in enumerate(instructions):
        if ins.mnemonic.startswith(('j', 'loop')):
            if ins.operands and ins.operands[0].type == X86_OP_IMM:
                branch_targets.append(ins.operands[0].imm)
                continue
            # Recognize only a contiguous unsigned CMP/JA/JMP table guard.
            # No other branch may enter after the CMP and bypass its bound.
            if index < 2 or ins.mnemonic != 'jmp' or not ins.operands:
                return None
            compare, above = instructions[index - 2:index]
            operand = ins.operands[0]
            if (operand.type != X86_OP_MEM or operand.mem.base or operand.mem.segment
                    or not operand.mem.index or operand.mem.scale != 4
                    or compare.mnemonic != 'cmp' or len(compare.operands) != 2
                    or compare.operands[0].type != X86_OP_REG or compare.operands[0].size != 4
                    or compare.operands[0].reg != operand.mem.index
                    or compare.operands[1].type != X86_OP_IMM
                    or not 0 <= compare.operands[1].imm < 1024
                    or above.mnemonic != 'ja' or above.operands[0].type != X86_OP_IMM):
                return None
            count = compare.operands[1].imm + 1
            if any(code.address < operand.mem.disp + count * 4
                   and operand.mem.disp < code.address + code.size for code in instructions):
                return None
            table = rdata.bytes_at(operand.mem.disp, count * 4)
            if table is None or len(table) != count * 4:
                return None
            targets = [int.from_bytes(table[i:i + 4], 'little') for i in range(0, len(table), 4)]
            if any(target not in positions for target in targets):
                return None
            branch_targets.extend(targets)
            guarded_entries.extend((above.address, ins.address))
    if any(entry in branch_targets for entry in guarded_entries):
        return None
    code_end = instructions[-1].address + instructions[-1].size
    for target in branch_targets:
        destination = positions.get(target)
        if function_start <= target < code_end and destination is None:
            # Overlapping instruction streams invalidate both the guard proof
            # and our list of incoming edges. Do not silently ignore such jumps.
            return None
        if destination is not None and first < destination <= at:
            first = destination
    return call_setup(instructions[first:at + 1], known_calls, argument_count=argument_count)


def read_call_setup(rdata, start, end, known_calls=None, *, argument_count=None):
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    raw = rdata.bytes_at(start, end - start)
    if raw is None or len(raw) != end - start:
        return None
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw, start))
    if not instructions or instructions[-1].address + instructions[-1].size != end:
        return None
    return call_setup(instructions, known_calls, argument_count=argument_count)


@dataclass(frozen=True)
class CallSetup:
    address: int
    target: tuple
    ecx: tuple
    edx: tuple
    stack_arguments: tuple


def call_setup(instructions, known_calls=None, *, argument_count=None):
    """Recover setup, optionally using a reviewed ABI's stack-argument count.

    An explicit count separates arguments from earlier register saves/local-frame
    allocation. It must include hidden stack arguments; it is not inferred from
    the decompiler's potentially incomplete parameter list.
    """
    if argument_count is not None and (type(argument_count) is not int or argument_count < 0):
        return None
    known_calls = known_calls or {}
    registers, stack, sp = {}, {}, 0
    unknown = ('unknown',)
    def register(name):
        return ('stack', sp) if name == 'esp' else registers.get(name, ('register', name))
    def offset(base, displacement):
        if base[0] == 'constant':
            return ('constant', (base[1] + displacement) & 0xffffffff)
        if base[0] == 'stack':
            return ('stack', base[1] + displacement)
        if base[0] == 'address':
            return offset(base[1], base[2] + displacement)
        if base == unknown:
            return unknown
        return ('address', base, displacement)
    def address(operand, ins):
        mem = operand.mem
        if mem.index or mem.segment:
            return unknown
        base = register(ins.reg_name(mem.base)) if mem.base else ('constant', 0)
        return offset(base, mem.disp)
    def value(operand, ins):
        if operand.type == X86_OP_IMM:
            return ('constant', operand.imm & 0xffffffff)
        if operand.type == X86_OP_REG:
            return register(ins.reg_name(operand.reg))
        if operand.type == X86_OP_MEM:
            at = address(operand, ins)
            if at[0] == 'stack' and at[1] in stack:
                return stack[at[1]]
            return ('memory', at)
        return unknown
    def parent(name):
        for full, parts in (('eax', ('al','ah','ax')), ('ebx', ('bl','bh','bx')),
                            ('ecx', ('cl','ch','cx')), ('edx', ('dl','dh','dx')),
                            ('esi', ('si',)), ('edi', ('di',)), ('ebp', ('bp',)), ('esp', ('sp',))):
            if name in parts:
                return full
        return name
    for index, ins in enumerate(instructions):
        op, args = ins.mnemonic, ins.operands
        if op == 'call':
            if index == len(instructions) - 1:
                pushed = []
                end = 0 if argument_count is None else sp + 4 * argument_count
                if end > 0:
                    return None
                for at in range(sp, end, 4):
                    if at not in stack:
                        return None
                    pushed.append(stack[at])
                return CallSetup(ins.address, value(args[0], ins), register('ecx'), register('edx'), tuple(pushed))
            profile = known_calls.get(ins.address)
            if profile is None:
                return None
            cleanup, result = profile
            if cleanup < 0 or cleanup % 4 or sp + cleanup > 0:
                return None
            for at in range(sp, sp + cleanup, 4):
                stack.pop(at, None)
            sp += cleanup
            registers.update(eax=('result', ins.address, result), ecx=unknown, edx=unknown)
            continue
        if op.startswith('j') or op.startswith('ret') or op.startswith('loop'):
            return None
        if (argument_count is not None and op == 'sub' and len(args) == 2
                and args[0].type == X86_OP_REG and ins.reg_name(args[0].reg) == 'esp'
                and args[1].type == X86_OP_IMM):
            allocation = args[1].imm
            if allocation <= 0 or allocation % 4:
                return None
            sp -= allocation
            # Allocated frame bytes are not pushed arguments. Leave their
            # contents absent so a requested argument in this area rejects.
            continue
        if op == 'push' and args[0].size == 4:
            pushed = value(args[0], ins)
            sp -= 4
            stack[sp] = pushed
            continue
        if (argument_count is not None and op == 'mov' and len(args) == 2
                and args[0].type == X86_OP_MEM and args[0].size in (1, 2, 4)
                and args[1].type in (X86_OP_REG, X86_OP_IMM)):
            # A store can alias any staged argument or symbolic memory load.
            # Forget those facts rather than pretending the write is unrelated.
            # Pure register identities/constants and computed stack addresses do
            # not change when memory changes. An explicit ABI argument count is
            # required so erased argument slots cannot silently shorten the call.
            def reads_memory(value):
                return isinstance(value, tuple) and (value[0] == 'memory'
                    or any(reads_memory(part) for part in value[1:]))
            registers = {name: unknown if reads_memory(value) else value
                         for name, value in registers.items()}
            stack.clear()
            continue
        if (op == 'mov' and len(args) == 2 and args[0].type == X86_OP_REG
                and args[0].size == 1 and args[1].type == X86_OP_MEM and args[1].size == 1
                and ins.reg_name(args[0].reg) in ('al','bl','cl','dl')):
            # XOR reg,reg followed by MOV low-byte,[memory] is an unsigned
            # byte load. Preserve its width; the containing global may be mutable.
            target = parent(ins.reg_name(args[0].reg))
            if register(target) == ('constant', 0):
                registers[target] = ('unsigned_byte', value(args[1], ins))
                continue
        if op in ('mov', 'lea') and args[0].type == X86_OP_REG and args[0].size == 4:
            target = ins.reg_name(args[0].reg)
            if target == 'esp':
                return None
            registers[target] = address(args[1], ins) if op == 'lea' else value(args[1], ins)
            continue
        if (op in ('add', 'sub') and len(args) == 2 and args[0].type == X86_OP_REG
                and args[0].size == 4 and args[1].type == X86_OP_IMM):
            target = ins.reg_name(args[0].reg)
            if target == 'esp':
                return None
            displacement = args[1].imm if op == 'add' else -args[1].imm
            registers[target] = offset(register(target), displacement)
            continue
        if (op == 'xor' and len(args) == 2 and args[0].type == X86_OP_REG
                and args[1].type == X86_OP_REG and args[0].size == 4
                and args[0].reg == args[1].reg):
            target = ins.reg_name(args[0].reg)
            if target == 'esp':
                return None
            registers[target] = ('constant', 0)
            continue
        if op == 'nop':
            continue
        # Operand order and register-write metadata do not describe every
        # memory/control effect (XCHG can write its second operand; string
        # operations and traps have implicit effects). Only carry facts past
        # reviewed, register-only scalar operations or read-only comparisons.
        if op not in ('cmp', 'test'):
            scalar = ('mov', 'movzx', 'movsx', 'add', 'adc', 'sub', 'sbb',
                      'and', 'or', 'xor', 'not', 'neg', 'inc', 'dec',
                      'shl', 'sal', 'shr', 'sar', 'rol', 'ror', 'rcl', 'rcr',
                      'imul', 'mul', 'idiv', 'div', 'bswap', 'xchg')
            if op not in scalar or any(arg.type == X86_OP_MEM for arg in args):
                return None
        # Do not carry a whole-register fact through an unmodeled partial write.
        _, written = ins.regs_access()
        for reg in written:
            name = parent(ins.reg_name(reg))
            if name == 'esp':
                return None
            registers[name] = unknown
    return None
