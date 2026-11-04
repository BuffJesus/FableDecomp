"""Recover an inline by-value copy of an entity's own CScriptThing argument."""
import hashlib
import re

CALLEE = 0x008a6dd0
CALLEE_SIZE = 0xda
CALLEE_SHA256 = '8270d47d77b1cf84d679faa32498b92bfa2f6f13d05ce925b00c6a699e67fe9d'


def restore(text, fn, entries, rdata):
    if not entries or not fn.get('bodyEndExclusive'):
        return text
    candidates = [entry for entry in entries if entry[5] and entry[4] == '0xd30'
                  and entry[3].get('slot') == '0xd30'
                  and entry[3].get('ecxValue') == 4 and (not entry[2] or (
                  len(entry[2]) == 4
                  and entry[2][0] == '&PTR__scalar_deleting_destructor__01238c8c'
                  and entry[2][3] == '0'
                  and all(re.fullmatch(r'[A-Za-z_]\w*', value) for value in entry[2][1:3])))]
    if not candidates:
        return text
    body = rdata.bytes_at(CALLEE, CALLEE_SIZE)
    if (rdata.bytes_at(0x01260f0c + 0xd30, 4) != CALLEE.to_bytes(4, 'little')
            or body is None or hashlib.sha256(body).hexdigest() != CALLEE_SHA256):
        return text
    # The pinned callee destroys its 12-byte by-value CScriptThing and RET16
    # also removes the boolean. The Lua binding constructs its own retained copy.
    import capstone
    from capstone import x86
    from tools.script_recovery.native_receiver_reaching import _cfg, _reaching
    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    raw = rdata.bytes_at(start, end-start)
    if not raw:
        return text
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    ins = list(md.disasm(raw, start))
    saved = (x86.X86_REG_ESI, x86.X86_REG_EDI, x86.X86_REG_EBX, x86.X86_REG_EBP)
    owner = next(((op.operands[0].reg, k) for k, op in enumerate(ins[:12])
                  if op.mnemonic == 'mov' and len(op.operands) == 2
                  and op.operands[0].type == x86.X86_OP_REG and op.operands[0].reg in saved
                  and op.operands[1].type == x86.X86_OP_REG and op.operands[1].reg == x86.X86_REG_ECX), None)
    if owner is None:
        return text
    reg, defined = owner
    preds = _cfg(ins, x86, capstone, rdata)
    if preds is None:
        return text
    indexed = {op.address: k for k, op in enumerate(ins)}
    edits = []
    for a, e, args, site, _, _ in candidates:
        last = indexed.get(int(site['site'], 16))
        if last is None or last < 14:
            continue
        first = last-14
        if _reaching(ins, preds, first, reg, x86) != {defined}:
            continue
        push = ins[first]
        immediate_zero = (push.mnemonic == 'push' and
                          push.operands[0].type == x86.X86_OP_IMM and push.operands[0].imm == 0)
        if immediate_zero:
            # This complete native layout also recovers an entirely omitted argument list.
            if args:
                continue
            flag_reg, flag = None, '0'
        else:
            if (not args or push.mnemonic != 'push' or push.operands[0].type != x86.X86_OP_REG
                    or push.operands[0].reg not in saved):
                continue
            flag_reg = push.operands[0].reg
            flag = md.reg_name(flag_reg)
        name = md.reg_name(reg)
        expected = [
            ('push', flag), ('sub', 'esp, 0xc'), ('mov', 'eax, esp'),
            ('mov', 'dword ptr [eax], 0x1231710'),
            ('mov', 'dword ptr [eax], 0x1238c8c'),
            ('mov', f'ecx, dword ptr [{name} + 0xc]'),
            ('mov', 'dword ptr [eax + 4], ecx'),
            ('mov', f'ecx, dword ptr [{name} + 0x10]'),
            ('test', 'ecx, ecx') if immediate_zero else ('cmp', f'ecx, {flag}'),
            ('mov', 'dword ptr [eax + 8], ecx'),
            ('je', hex(ins[first+12].address)), ('inc', 'dword ptr [ecx]'),
            ('mov', f'ecx, dword ptr [{name} + 4]'),
            ('mov', 'edx, dword ptr [ecx]'), ('call', 'dword ptr [edx + 0xd30]')]
        if [(op.mnemonic, op.op_str) for op in ins[first:last+1]] != expected:
            continue
        # Only the null-info guard may branch within this stack copy.
        if any(set(preds[k]) != ({first+10, first+11} if k == first+12 else {k-1})
               for k in range(first+1, last+1)):
            continue
        if not immediate_zero:
            definitions = _reaching(ins, preds, first, flag_reg, x86)
            if (not definitions or -1 in definitions
                    or any((ins[k].mnemonic, ins[k].op_str) not in
                           (('xor', f'{flag}, {flag}'), ('mov', f'{flag}, 0')) for k in definitions)):
                continue
        edits.append((a, e, '(CScriptThing *)((int)this + 8),0'))
    for a, e, value in sorted(edits, reverse=True):
        text = text[:a] + value + text[e:]
    return text
