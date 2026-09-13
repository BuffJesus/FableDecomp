"""Check a single-active-resource contract on all aligned native CFG paths."""
from collections import deque

from capstone.x86 import X86_OP_IMM, X86_OP_REG


def check_single_resource_lifetime(instructions, events, *, receiver_register=None, selections=None):
    """Check reviewed resource events, optionally following a selected receiver.

    A resource equal to ('register', receiver_register) uses the last reviewed
    selection on that path. Selections map instruction addresses to resource
    identities; the caller must prove those identities from native operands and
    stack layout. Any other receiver write or call forgets the selection.
    """
    instructions = tuple(instructions)
    selections = selections or {}
    if receiver_register not in (None, 'ecx'):
        return False
    if not instructions:
        return False
    positions = {ins.address: index for index, ins in enumerate(instructions)}
    if not set(events).issubset(positions):
        return False
    if not set(selections).issubset(positions) or selections and not receiver_register:
        return False
    if any(a.address + a.size != b.address for a, b in zip(instructions, instructions[1:])):
        return False
    queue, seen, visited_events = deque([(0, None, None)]), set(), set()
    while queue:
        index, active, selected = queue.popleft()
        if (index, active, selected) in seen:
            continue
        seen.add((index, active, selected))
        ins = instructions[index]
        if receiver_register:
            _, written = ins.regs_access()
            aliases = {'ecx': ('ecx', 'cx', 'cl', 'ch')}.get(receiver_register, (receiver_register,))
            if any(ins.reg_name(reg) in aliases for reg in written):
                selected = None
            if ins.address in selections:
                if (ins.mnemonic != 'lea' or not ins.operands or ins.operands[0].type != X86_OP_REG
                        or ins.operands[0].size != 4 or ins.reg_name(ins.operands[0].reg) != receiver_register):
                    return False
                selected = selections[ins.address]
        if ins.address in events:
            operation, resource = events[ins.address]
            if receiver_register and resource == ('register', receiver_register):
                resource = selected
            if ins.mnemonic != 'call' or resource is None:
                return False
            visited_events.add(ins.address)
            if operation == 'start' and active is None:
                active = resource
            elif operation == 'end' and active == resource:
                active = None
            elif operation == 'use' and active == resource:
                pass
            else:
                return False
        if receiver_register and ins.mnemonic == 'call':
            selected = None
        if ins.mnemonic.startswith('ret'):
            if active is not None:
                return False
            continue
        if ins.mnemonic in ('int', 'int3', 'iret', 'iretd', 'ud2', 'hlt', 'syscall', 'sysenter', 'lcall', 'ljmp'):
            return False
        following = [index + 1] if index + 1 < len(instructions) else []
        if ins.mnemonic.startswith(('j', 'loop')):
            if not ins.operands or ins.operands[0].type != X86_OP_IMM:
                return False
            target = positions.get(ins.operands[0].imm)
            if target is None:
                return False
            following = [target] if ins.mnemonic == 'jmp' else following + [target]
        if not following:
            return False
        queue.extend((target, active, selected) for target in following)
    return visited_events == set(events)
