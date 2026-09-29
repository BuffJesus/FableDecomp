"""Recover member-resource calls only when retail receiver and operands agree."""
import json
import re


def resource_call_evidence(rdata, fn, spec):
    """Track simple register values within a basic block; reject merged values.

    A stable callee-saved `this` register anchors member addresses. Both ECX and
    the called vtable must identify the same member. Calls clobber volatile
    registers; only a proven zero-argument GetHero preserves pending pushes.
    """
    import capstone as cs
    from capstone import x86 as x
    start = int(fn['address'], 16)
    end = fn.get('bodyEndExclusive')
    if not end:
        return {}
    md = cs.Cs(cs.CS_ARCH_X86, cs.CS_MODE_32)
    md.detail = True
    ins = list(md.disasm(rdata.bytes_at(start, int(end, 16) - start), start))
    if not ins or ins[-1].address + ins[-1].size != int(end, 16):
        return {}
    aliases = {}
    for names in [('eax', 'ax', 'al', 'ah'), ('ebx', 'bx', 'bl', 'bh'),
                  ('ecx', 'cx', 'cl', 'ch'), ('edx', 'dx', 'dl', 'dh'),
                  ('esi', 'si'), ('edi', 'di'), ('ebp', 'bp'), ('esp', 'sp')]:
        full = getattr(x, 'X86_REG_' + names[0].upper())
        aliases.update({getattr(x, 'X86_REG_' + name.upper()): full for name in names})
    saved = (x.X86_REG_ESI, x.X86_REG_EDI, x.X86_REG_EBX, x.X86_REG_EBP)
    owner = next(((i.operands[0].reg, k) for k, i in enumerate(ins[:12])
                  if i.mnemonic == 'mov' and i.operands[0].type == x.X86_OP_REG
                  and i.operands[0].reg in saved and i.operands[1].type == x.X86_OP_REG
                  and i.operands[1].reg == x.X86_REG_ECX), None)
    if owner is None:
        return {}
    this_reg, definition = owner
    if any(i.group(cs.CS_GRP_JUMP) or i.group(cs.CS_GRP_CALL) or i.group(cs.CS_GRP_RET)
           for i in ins[:definition]):
        return {}
    if any(x.X86_REG_ECX in {aliases.get(r, r) for r in i.regs_access()[1]}
           for i in ins[:definition]):
        return {}
    if any(this_reg in {aliases.get(r, r) for r in i.regs_access()[1]} and i.mnemonic != 'pop'
           for k, i in enumerate(ins) if k != definition):
        return {}
    # Restoring `this` is safe only in a straight epilogue ending in RET.
    for k, i in enumerate(ins):
        if i.mnemonic == 'pop' and i.operands[0].type == x.X86_OP_REG and i.operands[0].reg == this_reg:
            tail = ins[k + 1:k + 9]
            if not tail or not any(t.group(cs.CS_GRP_RET) for t in tail):
                return {}
            for t in tail:
                if t.group(cs.CS_GRP_RET):
                    break
                if t.mnemonic not in ('pop', 'add', 'mov', 'lea', 'leave'):
                    return {}
    targets = set()
    for i in ins:
        if i.group(cs.CS_GRP_JUMP):
            if i.operands[0].type != x.X86_OP_IMM:
                return {}  # unknown jump-table entries could bypass a definition
            targets.add(i.operands[0].imm)
    regs, pushes, out = {this_reg: ('self', 0)}, [], {}
    resources = {'self': spec.self_resources, 'parent': spec.parent_resources if spec.entity else {}}

    def address(op):
        if op.type != x.X86_OP_MEM or op.mem.index:
            return None
        base = regs.get(op.mem.base)
        if base and len(base) == 2 and base[0] in ('self', 'parent', 'interface'):
            return base[0], base[1] + op.mem.disp

    anchored = True
    for i in ins[definition + 1:]:
        if i.address in targets:
            anchored = True
            regs, pushes = {this_reg: ('self', 0)}, []
        ops = i.operands
        if i.mnemonic == 'push':
            pushes.append(('imm', ops[0].imm) if ops[0].type == x.X86_OP_IMM
                          else regs.get(ops[0].reg) if ops[0].type == x.X86_OP_REG else None)
            continue
        if i.mnemonic == 'call':
            recv = regs.get(x.X86_REG_ECX)
            table = regs.get(ops[0].mem.base) if ops[0].type == x.X86_OP_MEM and not ops[0].mem.index else None
            slot = ops[0].mem.disp if ops[0].type == x.X86_OP_MEM else None
            matched = recv and len(recv) == 2 and table == ('vtable', *recv)
            hero = matched and recv == ('interface', 0) and slot == 0x118
            if matched and recv[1] in resources.get(recv[0], {}) and slot in (0x30, 0x34):
                out[i.address] = dict(family=recv[0], offset=recv[1], slot=slot,
                                      pushes=list(reversed(pushes[-6:])))
            for reg in (x.X86_REG_EAX, x.X86_REG_ECX, x.X86_REG_EDX):
                regs.pop(reg, None)
            if hero:
                regs[x.X86_REG_EAX] = ('hero', i.address)
            else:
                pushes = []
            continue
        value = None
        dest = ops[0].reg if ops and ops[0].type == x.X86_OP_REG else None
        if i.mnemonic == 'mov' and dest:
            src = ops[1]
            if src.type == x.X86_OP_REG:
                value = regs.get(src.reg)
            elif src.type == x.X86_OP_IMM:
                value = ('imm', src.imm)
            else:
                addr = address(src)
                if addr == ('self', spec.parent_off) and spec.entity:
                    value = ('parent', 0)
                elif addr == ('self', 4 if spec.entity else 0x40):
                    value = ('interface', 0)
                elif addr:
                    value = ('vtable', *addr)
        elif i.mnemonic == 'lea' and dest:
            value = address(ops[1])
        elif i.mnemonic == 'add' and dest and ops[1].type == x.X86_OP_IMM:
            old = regs.get(dest)
            if old and len(old) == 2 and old[0] in ('self', 'parent', 'interface'):
                value = old[0], old[1] + ops[1].imm
        for written in i.regs_access()[1]:
            reg = aliases.get(written, written)
            regs.pop(reg, None)
            if reg == this_reg:
                anchored = False
        if dest and dest != this_reg and value:
            regs[dest] = value
        if x.X86_REG_ESP in i.regs_access()[1]:
            pushes = []
        if any(op.type == x.X86_OP_MEM and op.access & cs.CS_AC_WRITE for op in ops):
            regs, pushes = ({this_reg: ('self', 0)} if anchored else {}), []
        if i.group(cs.CS_GRP_JUMP) or i.group(cs.CS_GRP_RET):
            regs, pushes = ({this_reg: ('self', 0)} if anchored else {}), []
    return out


def _head(text, args_start):
    start = text.rfind('(**(code **)', max(0, args_start - 300), args_start)
    if start < 0:
        return None
    head = text[start:args_start]
    if ';' in head or head.count('(') != head.count(')') + 1:
        return None
    return start


def recover_member_resource_calls(text, fn, entries, spec, rdata):
    if not entries or not (spec.self_resources or spec.parent_resources):
        return text
    if not any(virtual and ((site.get('slot') == '0x30' and not args)
                           or (site.get('slot') == '0x34' and len(args) < 6))
               for _, _, args, site, _, virtual in entries):
        return text
    evidence = resource_call_evidence(rdata, fn, spec)
    sites = {int(site['site'], 16): (a, b, args, site.get('slot'))
             for a, b, args, site, _, virtual in entries if virtual}
    edits = []
    for site, proof in evidence.items():
        if site not in sites:
            continue
        a, b, args, printed_slot = sites[site]
        if printed_slot != hex(proof['slot']):
            continue
        head = _head(text, a)
        if head is None:
            continue
        fields = spec.parent_resources if proof['family'] == 'parent' else spec.self_resources
        owner = ', (CScriptThing *)(this + 8)' if proof['family'] == 'self' and spec.entity else ''
        member = f'RESOURCE_MemberResource("{fields[proof["offset"]]}"{owner})'
        if proof['slot'] == 0x30 and not args:
            # The binding returns the CScriptThing value. Do not invent a name
            # for a hidden native result slot that the export omitted.
            cast = re.search(r'\(CScriptThing(?:_bv)? \*\)\s*$', text[:head])
            if cast:
                head = cast.start()
            edits.append((head, b + 1, f'RESOURCE_ScriptThing({member})'))
        elif proof['slot'] == 0x34 and len(args) < 6:
            values = proof['pushes']
            if len(values) != 6 or not values[0] or values[0][0] != 'hero':
                continue
            if not all(v and v[0] == 'imm' for v in values[1:]):
                continue
            literal = rdata.string_at(values[1][1])
            if literal is None or any(v[1] not in (0, 1) for v in values[3:]):
                continue
            getter = sites.get(values[0][1])
            if not getter or len(getter[2]) > 1:
                continue
            getter_head = _head(text, getter[0])
            assignment = re.search(r'\b(\w+) =\s*$', text[:getter_head]) if getter_head is not None else None
            if not assignment or not args or args[0].strip() != assignment[1]:
                continue
            parameters = [assignment[1], json.dumps(literal), *[str(v[1]) for v in values[2:]]]
            call = ('CScriptGameResourceObjectScriptedThingBase::_Speak_CScriptGameResourceObjectScriptedThingBase('
                    f'(CScriptGameResourceObjectScriptedThingBase *){member},' + ','.join(parameters) + ')')
            edits.append((head, b + 1, call))
    for start, end, replacement in sorted(edits, reverse=True):
        text = text[:start] + replacement + text[end:]
    return text
