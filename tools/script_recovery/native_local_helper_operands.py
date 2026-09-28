"""Operands a local helper call lost, rebuilt from the pushes before the call.

V_BookCollecting BS_Teacher AskForBook 0x00E55CE0 is `AskForBook(long, class CCharString)` (ego_r, RET 8). Its
callers push both operands, but Ghidra printed `AskForBook(this)` at 0x00E54E90's site and `AskForBook(this,value)`
at 0x00E57530's, dropping the by-value refusal key and (at the first) the book index:

    push ecx; mov ecx, esp; push -1; push "TEXT_QST_B16_BOOK_REFUSE_AGAIN"; call CCharString::CCharString
    mov eax, [esi+0x14]; mov ecx, [eax+0x8c]; push ecx; mov ecx, esi; call AskForBook

Walking back from the call, each operand (first parameter nearest the call) is decoded from:
- the in-place by-value CCharString construction (`push r; mov r, esp; push -1; push LIT; call 0x99EBF0`) -> "LIT";
- `push imm` -> the immediate;
- `push reg` with reg last written by `mov reg, [esi+OFF]` (esi = this) -> `*(int *)(this + OFF)`, or by
  `mov reg, [r2+OFF]` with r2 = `[esi+0x14]` (the parent) -> `*(int *)(*(int *)(this + 0x14) + OFF)`.
Only when every missing operand decodes, the printed call gets them appended; otherwise nothing changes.
"""
import re

CCHARSTRING_CTOR = 0x0099EBF0


def _decode(ins, k, rdata, x86):
    """Operand pushed at ins[k] ('push'), or None."""
    op = ins[k].operands[0]
    if op.type == x86.X86_OP_IMM:
        return str(op.imm)
    if op.type != x86.X86_OP_REG:
        return None
    reg = op.reg

    def last_write(r, before):
        for j in range(before - 1, max(before - 12, -1), -1):
            i = ins[j]
            if i.mnemonic == 'call':
                return None
            _, written = i.regs_access()
            if r in written:
                return j
        return None

    j = last_write(reg, k)
    if j is None or ins[j].mnemonic != 'mov' or ins[j].operands[1].type != x86.X86_OP_MEM:
        return None
    mem = ins[j].operands[1].mem
    if mem.index:
        return None
    if mem.base == x86.X86_REG_ESI:
        return f'*(int *)(this + {mem.disp:#x})'
    jj = last_write(mem.base, j)
    if jj is not None and ins[jj].mnemonic == 'mov' and ins[jj].operands[1].type == x86.X86_OP_MEM:
        inner = ins[jj].operands[1].mem
        if inner.base == x86.X86_REG_ESI and not inner.index and inner.disp == 0x14:
            return f'*(int *)(*(int *)(this + 0x14) + {mem.disp:#x})'
    return None


def pushed_operands(rdata, entry, site, count):
    """The `count` operands pushed for the call at `site` (first parameter first; None for one that does not
    decode), or None when the pushes cannot be walked."""
    import capstone
    from capstone import x86
    code = rdata.bytes_at(entry, site - entry)
    if not code:
        return None
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    ins = list(md.disasm(code, entry))
    if not ins or ins[-1].address + ins[-1].size != site:
        return None
    # ecx must be `this` (esi) for a member call
    if not any(i.mnemonic == 'mov' and i.op_str == 'ecx, esi' for i in ins[-3:]):
        return None
    values, k = [], len(ins) - 1
    while len(values) < count and k >= 0:
        i = ins[k]
        if i.mnemonic == 'call':
            target = i.operands[0].imm if i.operands[0].type == x86.X86_OP_IMM else None
            # the by-value CCharString built in place: push r; mov r, esp; push -1; push LIT; call ctor
            if (target == CCHARSTRING_CTOR and k >= 4 and ins[k - 1].mnemonic == 'push' and ins[k - 2].mnemonic == 'push'
                    and ins[k - 3].mnemonic == 'mov' and ins[k - 3].op_str.endswith(', esp') and ins[k - 4].mnemonic == 'push'
                    and ins[k - 1].operands[0].type == x86.X86_OP_IMM):
                literal = rdata.string_at(ins[k - 1].operands[0].imm)
                if literal is None:
                    return None
                values.append('"' + literal.replace('\\', '\\\\').replace('"', '\\"') + '"')
                k -= 5
                continue
            return None
        if i.mnemonic == 'push':
            values.append(_decode(ins, k, rdata, x86))     # None: not decodable here (the printed call may have it)
        k -= 1
    return values if len(values) == count else None


def restore_local_helper_operands(text, fn, entries, arity_of, rdata):
    """`entries`: convert_quest_unit._text_order_sites output; `arity_of(target)`: the helper's recovered
    parameter count (None when not a local helper)."""
    if not entries:
        return text
    targets = {c['site'].lower(): str(c.get('target', '')).lower() for c in fn.get('calls', [])}
    entry = int(fn['address'], 16)
    edits = []
    for args_start, args_end, args, site, key, vtable in entries:
        if vtable:
            continue
        site = site.get('site') if isinstance(site, dict) else site
        target = targets.get(str(site).lower())
        n = arity_of(target) if target else None
        printed = [a.strip() for a in re.split(r',(?![^()]*\))', text[args_start:args_end])] if text[args_start:args_end].strip() else []
        if n is None or not printed or printed[0] != 'this' or len(printed) - 1 >= n:
            continue
        operands = pushed_operands(rdata, entry, int(site, 16), n)
        if operands is None or None in operands[len(printed) - 1:]:
            continue
        edits.append((args_start, args_end, ','.join(['this'] + printed[1:] + operands[len(printed) - 1:])))
    for start, end, new in sorted(edits, reverse=True):
        text = text[:start] + new + text[end:]
    return text
