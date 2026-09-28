"""Thing vtable calls whose printed receiver is a merged variable, resolved by reaching definitions.

Arena ArenaCellDoorGuard Main (0x00F17C70) tests `MsgIsHitByHero` on `ebp`, written only by `lea ebp,[esi+8]`
(0x00F17D9D, `me`) and, inside a cutscene branch that leaves elsewhere, `mov ebp,[esi+4]`. The typed export prints
several unrelated values as `pCVar6`, so the Lua receiver was whichever branch assigned last (nil on the smoke path).

For a vtable call site, ecx is traced to its register source (`mov ecx, r`), then every definition of that register
reaching the site is collected over the function's control-flow graph (decoded linearly; direct jumps and fall-through
only -- a function with an indirect jump is left alone). When every reaching definition is `lea r, [esi+8]` and esi is
never written after the prologue's `mov esi, ecx`, the receiver is the entity's own thing and the printed call is
respelled on `(this + 8)`.
"""
import re


def _switch_targets(ins, k, op, x86, rdata):
    """Targets of `jmp [reg*4 + TABLE]` bounded by a preceding `cmp reg, N` (N + 1 entries), else None."""
    mem = op.mem
    if not rdata or mem.scale != 4 or mem.base or not mem.index:
        return None
    for j in range(k - 1, max(k - 8, -1), -1):
        i = ins[j]
        if i.mnemonic == 'cmp' and i.operands[0].type == x86.X86_OP_REG and i.operands[0].reg == mem.index \
                and i.operands[1].type == x86.X86_OP_IMM:
            count = i.operands[1].imm + 1
            raw = rdata.bytes_at(mem.disp & 0xffffffff, 4 * count)
            if not raw or count > 256:
                return None
            return [int.from_bytes(raw[4 * n:4 * n + 4], 'little') for n in range(count)]
    return None


def _cfg(ins, x86, capstone, rdata=None):
    index = {i.address: k for k, i in enumerate(ins)}
    preds = {k: [] for k in range(len(ins))}
    for k, i in enumerate(ins):
        jump = i.group(capstone.CS_GRP_JUMP)
        if i.group(capstone.CS_GRP_RET):
            continue
        if jump:
            op = i.operands[0] if i.operands else None
            if op is not None and op.type == x86.X86_OP_MEM and i.mnemonic == 'jmp':
                targets = _switch_targets(ins, k, op, x86, rdata)
                if targets is None or any(t not in index for t in targets):
                    return None              # an unbounded indirect jump: no claim
                for t in set(targets):
                    preds[index[t]].append(k)
                continue
            if op is None or op.type != x86.X86_OP_IMM:
                return None                  # an indirect jump through a register: no claim
            t = index.get(op.imm)
            if t is not None:
                preds[t].append(k)
            if i.mnemonic == 'jmp':
                continue
        if k + 1 < len(ins):
            preds[k + 1].append(k)
    return preds


def _reaching(ins, preds, start, reg, x86):
    """Indices of the instructions defining `reg` that reach instruction `start` (exclusive)."""
    defs, seen, stack = set(), set(), list(preds[start])
    while stack:
        k = stack.pop()
        if k in seen:
            continue
        seen.add(k)
        _, written = ins[k].regs_access()
        if reg in written:
            defs.add(k)
            continue
        if not preds[k]:
            defs.add(-1)                     # reached the entry without a definition
        stack.extend(preds[k])
    return defs


def me_receiver_sites(rdata, entry, end):
    """Addresses of calls in [entry, end) whose ecx provably holds `this + 8` (esi = this) on every path."""
    import capstone
    from capstone import x86
    code = rdata.bytes_at(entry, end - entry)
    if not code:
        return set()
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    ins = list(md.disasm(code, entry))
    if not ins:
        return set()
    # esi must be `this` for the whole body: one `mov esi, ecx` in the prologue, never written again
    esi_writes = [k for k, i in enumerate(ins) if x86.X86_REG_ESI in i.regs_access()[1] and i.mnemonic != 'push']
    if not esi_writes or ins[esi_writes[0]].op_str != 'esi, ecx' or esi_writes[0] > 12 \
            or any(ins[k].mnemonic != 'pop' for k in esi_writes[1:]):
        return set()
    preds = _cfg(ins, x86, capstone, rdata)
    if preds is None:
        return set()
    out = set()
    for k, i in enumerate(ins):
        if i.mnemonic != 'call' or not i.operands or i.operands[0].type != x86.X86_OP_MEM:
            continue
        ecx_defs = _reaching(ins, preds, k, x86.X86_REG_ECX, x86)
        if len(ecx_defs) != 1 or -1 in ecx_defs:
            continue
        d = ins[next(iter(ecx_defs))]
        if d.mnemonic == 'lea' and d.op_str == 'ecx, [esi + 8]':
            out.add(i.address)
            continue
        if d.mnemonic != 'mov' or d.operands[1].type != x86.X86_OP_REG:
            continue
        src = d.operands[1].reg
        src_defs = _reaching(ins, preds, next(iter(ecx_defs)), src, x86)
        if src_defs and -1 not in src_defs and all(ins[j].mnemonic == 'lea' and ins[j].op_str.endswith(', [esi + 8]')
                                                   for j in src_defs):
            out.add(i.address)
    return out


RE_HEAD = re.compile(r'\(\*\*\(code \*\*\)\(\*\(int \*\)(?P<recv>\w+) \+ (?P<slot>0x[0-9a-f]+|\d+)\)\)\($')


def respell_me_receivers(text, fn, entries, rdata):
    """Respell printed thing vcalls whose site `me_receiver_sites` proves to be on `this + 8`."""
    if not entries:
        return text
    lo, hi = int(fn['address'], 16), fn.get('bodyEndExclusive')
    if not hi:
        return text
    sites = me_receiver_sites(rdata, lo, int(hi, 16))
    if not sites:
        return text
    edits = []
    for args_start, args_end, args, site, key, vtable in entries:
        if not vtable:
            continue
        address = site.get('site') if isinstance(site, dict) else site
        if int(str(address), 16) not in sites:
            continue
        head = RE_HEAD.search(text[:args_start])
        if not head or head['recv'] in ('this',):
            continue
        recv = head['recv']
        inner = text[args_start:args_end]
        first = re.match(r'\s*(?:\([\w ]+\*\))?' + re.escape(recv) + r'\b', inner)
        # Ghidra prints calls on `this + 8` without the receiver argument (`(**(code **)(*(int *)(this + 8) + 0xf0))()`)
        new_inner = re.sub(r'^\s*,\s*', '', inner[first.end():]) if first else inner
        edits.append((head.start(), args_end,
                      f'(**(code **)(*(int *)(this + 8) + {head["slot"]}))(' + new_inner))
    for start, end, new in sorted(edits, reverse=True):
        text = text[:start] + new + text[end:]
    return text
