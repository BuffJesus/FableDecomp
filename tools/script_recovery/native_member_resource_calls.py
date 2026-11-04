"""Recover member-resource calls only when retail receiver and operands agree."""
import json
import re


def resource_call_evidence(rdata, fn, spec, *, include_pause=False, include_member_loads=False, include_resource_table_loads=False):
    """Track register values across the CFG; reject disagreeing incoming values.

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
    for i in ins:
        if i.group(cs.CS_GRP_JUMP):
            if i.operands[0].type != x.X86_OP_IMM:
                return {}  # unknown jump-table entries could bypass a definition
    resources = {'self': spec.self_resources, 'parent': spec.parent_resources if spec.entity else {}}

    def transfer(i, state):
        regs, pushes = dict(state[0]), list(state[1])
        proof = None

        def address(op):
            if op.type != x.X86_OP_MEM or op.mem.index:
                return None
            base = regs.get(op.mem.base)
            if base and len(base) == 2 and base[0] in ('self', 'parent', 'interface'):
                return base[0], base[1] + op.mem.disp

        ops = i.operands
        if i.mnemonic == 'push':
            pushes.append(('imm', ops[0].imm) if ops[0].type == x.X86_OP_IMM
                          else regs.get(ops[0].reg) if ops[0].type == x.X86_OP_REG else None)
            return (regs, tuple(pushes[-6:])), proof
        if i.mnemonic == 'call':
            recv = regs.get(x.X86_REG_ECX)
            table = regs.get(ops[0].mem.base) if ops[0].type == x.X86_OP_MEM and not ops[0].mem.index else None
            slot = ops[0].mem.disp if ops[0].type == x.X86_OP_MEM else None
            matched = recv and len(recv) == 2 and table == ('vtable', *recv)
            hero = matched and recv == ('interface', 0) and slot == 0x118
            if matched and recv[1] in resources.get(recv[0], {}) and slot in (0x30, 0x34, 0x68):
                proof = dict(family=recv[0], offset=recv[1], slot=slot,
                             pushes=list(reversed(pushes[-6:])))
            elif include_pause and matched and recv == ('interface', 0) and slot == 0x5ec:
                proof = dict(family='interface', offset=0, slot=slot,
                             pushes=list(reversed(pushes[-1:])))
            for reg in (x.X86_REG_EAX, x.X86_REG_ECX, x.X86_REG_EDX):
                regs.pop(reg, None)
            if hero:
                regs[x.X86_REG_EAX] = ('hero', i.address)
            else:
                pushes = []
            return (regs, tuple(pushes)), proof
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
                    if (include_resource_table_loads and addr[1] in resources.get(addr[0], {})
                            and src.size == 4 and ops[0].size == 4 and not src.mem.segment):
                        proof = dict(family=addr[0], offset=addr[1], kind='resource_vtable_load',
                                     receiverRegister=i.reg_name(src.mem.base), tableRegister=i.reg_name(dest))
                    if include_member_loads and addr[0] == ('parent' if spec.entity else 'self'):
                        snapshots = getattr(spec, 'parent_string_snapshots', {})
                        if addr[1] in snapshots:
                            proof = dict(family=addr[0], offset=addr[1], kind='member_load')
        elif i.mnemonic == 'lea' and dest:
            value = address(ops[1])
        elif i.mnemonic == 'add' and dest and ops[1].type == x.X86_OP_IMM:
            old = regs.get(dest)
            if old and len(old) == 2 and old[0] in ('self', 'parent', 'interface'):
                value = old[0], old[1] + ops[1].imm
        for written in i.regs_access()[1]:
            regs.pop(aliases.get(written, written), None)
        if dest and dest != this_reg and value:
            regs[dest] = value
        if x.X86_REG_ESP in i.regs_access()[1]:
            pushes = []
        writes = [op for op in ops if op.type == x.X86_OP_MEM and op.access & cs.CS_AC_WRITE]
        if writes:
            # A stack spill cannot change a live register's member address.
            # Unknown writes can alias the interface/resource vtable; forget
            # everything except the independently anchored owner in that case.
            if any(op.mem.base != x.X86_REG_ESP or op.mem.index for op in writes):
                regs = {this_reg: regs[this_reg]} if this_reg in regs else {}
            pushes = []
        return (regs, tuple(pushes)), proof

    # Meet at each CFG join, keeping only facts shared by every reachable path.
    # In particular a retry loop must not discard its callee-saved resource
    # address, nor may a bypass/other lifetime inherit a textual earlier LEA.
    from collections import deque
    by_address = {i.address: k for k, i in enumerate(ins)}
    first = definition + 1
    if first >= len(ins):
        return {}
    states = {first: ({this_reg: ('self', 0)}, ())}
    pending = deque([first])
    visits = 0
    while pending:
        k = pending.popleft()
        visits += 1
        if visits > len(ins) * 128:
            return {}  # fail closed on an unsupported/non-converging shape
        i = ins[k]
        after, _ = transfer(i, states[k])
        successors = []
        if i.group(cs.CS_GRP_JUMP):
            target = by_address.get(i.operands[0].imm)
            if target is not None:
                successors.append(target)
        if not i.group(cs.CS_GRP_RET) and i.mnemonic != 'jmp' and k + 1 < len(ins):
            successors.append(k + 1)
        for nxt in successors:
            if nxt <= definition:
                return {}  # entry/prologue reached again; its anchor is not invariant
            old = states.get(nxt)
            merged = after if old is None else (
                {reg: value for reg, value in old[0].items() if after[0].get(reg) == value},
                old[1] if old[1] == after[1] else ())
            if merged != old:
                states[nxt] = merged
                pending.append(nxt)
    out = {}
    for k, state in states.items():
        _, proof = transfer(ins[k], state)
        if proof:
            out[ins[k].address] = proof
    return out


def _head(text, args_start):
    start = text.rfind('(**(code **)', max(0, args_start - 300), args_start)
    if start < 0:
        return None
    head = text[start:args_start]
    if ';' in head or head.count('(') != head.count(')') + 1:
        return None
    return start


def recover_member_resource_calls(text, fn, entries, spec, rdata, reviewed=None):
    if not entries or not (spec.self_resources or spec.parent_resources):
        if reviewed:
            raise ValueError('resource operand evidence: missing call entries/resources')
        return text
    if not any(virtual and ((site.get('slot') == '0x30' and not args)
                           or (site.get('slot') == '0x34' and len(args) < 6)
                           or site.get('slot') in ('0x68', '0x5ec'))
               for _, _, args, site, _, virtual in entries) and not reviewed:
        return text
    evidence = resource_call_evidence(rdata, fn, spec, include_pause=True)
    from tools.script_recovery.reviewed_resource_operands import reviewed_resource_operands
    reviewed_proofs = reviewed_resource_operands(fn, reviewed, rdata, evidence)
    evidence.update(reviewed_proofs)
    applied_reviewed = set()
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
        if proof['family'] == 'interface':
            # A saved interface may be mistyped/reused by the decompiler.
            # Require agreement of the live receiver, vtable, and bool operand.
            if proof['pushes'] in ([('imm', 0)], [('imm', 1)]):
                value = 'true' if proof['pushes'][0][1] else 'false'
                edits.append((head, b + 1, f'GSI->PauseAllNonScriptedEntities({value})'))
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
        elif proof['slot'] == 0x68 and re.search(r'\((?:\(int\))?\*\w+ \+ 0x68\)', text[head:a]):
            # A reused alias mistyped as CCharString* cannot be classified for
            # its whole source lifetime. The native CFG proves this receiver
            # at the call. Slot 0x68 takes no explicit arguments; the export
            # can append the preceding GetHero result as a phantom operand.
            edits.append((head, b + 1, f'RESOURCE_IsPerformingScriptTask({member})'))
            if site in reviewed_proofs:
                applied_reviewed.add(site)
        elif proof['slot'] == 0x34 and len(args) < 6:
            values = proof['pushes']
            if len(values) != 6 or not values[0] or values[0][0] != 'hero':
                continue
            if not values[1] or not all(v and v[0] == 'imm' for v in values[2:]):
                continue
            if values[1][0] == 'source' and len(args) > 1 and args[1].strip() == values[1][1]:
                text_argument = args[1].strip()
            elif values[1][0] == 'imm':
                literal = rdata.string_at(values[1][1])
                text_argument = json.dumps(literal) if literal is not None else None
            else:
                text_argument = None
            if text_argument is None or any(v[1] not in (0, 1) for v in values[3:]):
                continue
            getter = sites.get(values[0][1])
            if not getter or len(getter[2]) > 1:
                continue
            getter_head = _head(text, getter[0])
            assignment = re.search(r'\b(\w+) =\s*$', text[:getter_head]) if getter_head is not None else None
            if not assignment or not args or args[0].strip() != assignment[1]:
                continue
            parameters = [assignment[1], text_argument, *[str(v[1]) for v in values[2:]]]
            call = ('CScriptGameResourceObjectScriptedThingBase::_Speak_CScriptGameResourceObjectScriptedThingBase('
                    f'(CScriptGameResourceObjectScriptedThingBase *){member},' + ','.join(parameters) + ')')
            edits.append((head, b + 1, call))
            if site in reviewed_proofs:
                applied_reviewed.add(site)
    if applied_reviewed != set(reviewed_proofs):
        raise ValueError('resource operand evidence: reviewed call could not be matched')
    for start, end, replacement in sorted(edits, reverse=True):
        text = text[:start] + replacement + text[end:]
    return text


def recover_definition_vector_loads(text, fn, spec, rdata):
    """Restore an explicit member load when a reused decompiler alias obscures it.

    Require one native non-stack load and one printed assignment at that offset,
    and agreement of every incoming CFG path on the native parent register.
    This repairs only that RHS, never every occurrence of the printed alias.
    """
    snapshots = getattr(spec, 'parent_string_snapshots', {})
    address = int(fn['address'], 16)
    allowed = {off for off, e in snapshots.items()
               if address in {int(a, 16) for a in e.get('readFunctions', [])}}
    if not allowed:
        return text
    proofs = resource_call_evidence(rdata, fn, spec, include_member_loads=True)
    if not proofs:
        return text
    import capstone as cs
    from capstone import x86 as x
    md = cs.Cs(cs.CS_ARCH_X86, cs.CS_MODE_32); md.detail = True
    ins = list(md.disasm(rdata.bytes_at(address, int(fn['bodyEndExclusive'], 16) - address), address))
    for off in allowed:
        loads = [i for i in ins if i.mnemonic == 'mov' and len(i.operands) == 2
                 and i.operands[1].type == x.X86_OP_MEM
                 and i.operands[1].mem.base not in (x.X86_REG_ESP, x.X86_REG_EBP)
                 and i.operands[1].mem.disp == off]
        if len(loads) != 1 or proofs.get(loads[0].address) != dict(
                family='parent' if spec.entity else 'self', offset=off, kind='member_load'):
            continue
        pattern = re.compile(r'(?m)^([ \t]*\w+ = )\*\(int \*\)\(\w+ \+ '
                             + '(?:' + hex(off) + '|' + str(off) + r')\);')
        matches = list(pattern.finditer(text))
        if len(matches) != 1:
            continue
        base = f'*(int *)(this + {hex(spec.parent_off)})' if spec.entity else 'this'
        m = matches[0]
        text = text[:m.start()] + m[1] + f'*(int *)({base} + {hex(off)});' + text[m.end():]
    return text
