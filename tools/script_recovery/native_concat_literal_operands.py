"""Recover a concat left operand directly forwarded from a literal constructor."""
import json
import re


def restore(text, fn, ordered, rdata):
    if not ordered:
        return text
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG
    start = int(fn['address'], 16)
    end = int(fn['bodyEndExclusive'], 16)
    raw = rdata.bytes_at(start, end - start)
    if not raw:
        return text
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    insns = list(decoder.disasm(raw, start))
    index = {ins.address: i for i, ins in enumerate(insns)}
    calls = {int(site['site'], 16): (args, site) for _, _, args, site, _, indirect in ordered if not indirect}

    def immediate(ins, mnemonic, value):
        return (ins.mnemonic == mnemonic and len(ins.operands) == 1
                and ins.operands[0].type == X86_OP_IMM and ins.operands[0].imm == value)

    def stack_result(ins):
        return (ins.mnemonic == 'lea' and len(ins.operands) == 2
                and ins.operands[0].type == X86_OP_REG
                and ins.reg_name(ins.operands[0].reg) == 'ecx'
                and ins.operands[1].type == X86_OP_MEM
                and ins.reg_name(ins.operands[1].mem.base) == 'esp'
                and ins.operands[1].mem.index == 0)

    edits = []
    for a, e, args, site, key, vtable in ordered:
        if vtable or int(site.get('target', '0'), 16) != 0x99F570 or len(args) != 3:
            continue
        i = index.get(int(site['site'], 16))
        if i is None or i < 6:
            continue
        length, literal, receiver, ctor, forward, destination, concat = insns[i-6:i+1]
        # Retail Guard E40C00..E40C1C and Madame E3CD0A..E3CD26:
        # push -1; push literal; lea ecx,[esp+slot]; call ctor;
        # mov edx,eax; lea ecx,[esp+result]; call concat(dest,left,right).
        if not (immediate(length, 'push', -1)
                and literal.mnemonic == 'push' and literal.operands[0].type == X86_OP_IMM
                and stack_result(receiver) and immediate(ctor, 'call', 0x99EBF0)
                and forward.mnemonic == 'mov' and forward.op_str == 'edx, eax'
                and stack_result(destination) and immediate(concat, 'call', 0x99F570)):
            continue
        value = rdata.string_at(literal.operands[0].imm)
        if value is None:
            continue
        printed = calls.get(ctor.address)
        if not printed:
            continue
        ctor_args, ctor_site = printed
        if (int(ctor_site.get('target', '0'), 16) != 0x99EBF0 or len(ctor_args) != 3
                or ctor_args[1].strip() != json.dumps(value, ensure_ascii=False)
                or ctor_args[2].strip() != '-1'):
            continue
        slot = re.fullmatch(r'(?:\(CCharString(?:_bv)? \*\)\s*)?&?(\w+)', ctor_args[0].strip())
        if not slot:
            continue
        # Keep the constructed temporary in the intermediate representation.
        # Thread recognizers consume ParentClass concatenation before lifting
        # strings; substituting a literal here bypasses those structural rules.
        edits.append((a, e, ','.join((args[0], slot[1], args[2]))))
    for a, e, replacement in sorted(edits, reverse=True):
        text = text[:a] + replacement + text[e:]
    return text
