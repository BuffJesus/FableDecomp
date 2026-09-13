"""Conservative register-copy provenance over aligned x86 control flow.

Facts identify function-entry registers, never their concrete values. Call
preservation is supplied explicitly per instruction by a reviewed ABI consumer;
an unprofiled call loses every register origin. Stack saves are not restored by
this analysis: loads/POPs and unsupported computations lose the destination fact.
"""
from collections import deque

from capstone import CS_GRP_INT, CS_GRP_IRET
from capstone.x86 import X86_OP_IMM, X86_OP_REG

REGISTERS = ('eax', 'ebx', 'ecx', 'edx', 'esi', 'edi', 'ebp')


def _parent(name):
    for full, parts in (('eax', ('ax', 'al', 'ah')), ('ebx', ('bx', 'bl', 'bh')),
                        ('ecx', ('cx', 'cl', 'ch')), ('edx', ('dx', 'dl', 'dh')),
                        ('esi', ('si',)), ('edi', ('di',)), ('ebp', ('bp',))):
        if name in parts:
            return full
    return name


def register_origins(instructions, call_preserves=None):
    """Return incoming facts by address, or None for unsupported control flow.

    Unreachable instructions map to None; unknown register origins are None
    within a reachable instruction's dictionary. Input must cover the complete
    function, with instruction boundaries established from its real entry.
    """
    instructions = tuple(instructions)
    if not instructions:
        return None
    call_preserves = call_preserves or {}
    positions = {ins.address: i for i, ins in enumerate(instructions)}
    start = instructions[0].address
    end = instructions[-1].address + instructions[-1].size
    edges = []
    for index, ins in enumerate(instructions):
        if index and instructions[index - 1].address + instructions[index - 1].size != ins.address:
            return None
        if (ins.group(CS_GRP_INT) or ins.group(CS_GRP_IRET)
                or ins.mnemonic in ('syscall', 'sysenter', 'sysexit', 'sysret', 'ud2', 'hlt', 'lcall', 'ljmp')):
            return None
        following = [index + 1] if index + 1 < len(instructions) else []
        if ins.mnemonic.startswith('ret'):
            following = []
        elif ins.mnemonic.startswith(('j', 'loop')):
            if not ins.operands or ins.operands[0].type != X86_OP_IMM:
                return None
            destination = ins.operands[0].imm
            if start <= destination < end and destination not in positions:
                return None
            target = [positions[destination]] if destination in positions else []
            following = target if ins.mnemonic == 'jmp' else following + target
        edges.append(following)
    incoming = [None] * len(instructions)
    incoming[0] = {reg: reg for reg in REGISTERS}
    queue, pending = deque([0]), {0}
    while queue:
        index = queue.popleft()
        pending.remove(index)
        ins = instructions[index]
        before = incoming[index]
        after = dict(before)
        if ins.mnemonic == 'call':
            preserved = call_preserves.get(ins.address, ())
            after = {reg: before[reg] if reg in preserved else None for reg in REGISTERS}
        else:
            _, written = ins.regs_access()
            for reg in written:
                name = _parent(ins.reg_name(reg))
                if name in after:
                    after[name] = None
            args = ins.operands
            if (ins.mnemonic == 'mov' and len(args) == 2
                    and all(arg.type == X86_OP_REG and arg.size == 4 for arg in args)):
                target, source = (ins.reg_name(arg.reg) for arg in args)
                if target in after:
                    after[target] = before.get(source)
        for successor in edges[index]:
            old = incoming[successor]
            merged = (dict(after) if old is None else
                      {reg: old[reg] if old[reg] == after[reg] else None for reg in REGISTERS})
            if old != merged:
                incoming[successor] = merged
                if successor not in pending:
                    queue.append(successor)
                    pending.add(successor)
    return {ins.address: incoming[i] for i, ins in enumerate(instructions)}
