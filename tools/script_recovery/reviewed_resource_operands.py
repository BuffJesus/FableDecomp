"""Reviewed call-site operands for stack spills beyond the CFG register model.

This is explicit source/native correspondence, not inferred stack alias analysis.
Whole-function hashes pin the reviewed lifetimes and helper ABI. Instruction
witnesses check the resource anchor, saved vtable, dispatch and outgoing flags.
Only Speak operands and reviewed task polls can be restored; no control flow or
arbitrary expression replacement is accepted. Stale evidence fails closed.
"""
import hashlib
import re


def reviewed_resource_operands(fn, row, rdata, inferred):
    if row is None:
        return {}

    def require(ok, message):
        if not ok:
            raise ValueError(f"resource operand evidence {fn['address']}: {message}")

    raw = rdata.bytes_at(int(fn['address'], 16), int(fn['size']))
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'],
            'native body changed')
    source = fn['decompile']
    require(hashlib.sha256(source.encode()).hexdigest() == row['sourceSha256'],
            'exported source changed')
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    from capstone.x86 import X86_OP_MEM, X86_OP_IMM, X86_OP_REG, X86_REG_ESP, X86_REG_ECX
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = {i.address: i for i in decoder.disasm(raw, int(fn['address'], 16))}
    out = {}
    for call in row['calls']:
        anchor = inferred.get(int(call['resourceAnchor'], 16))
        require(anchor is not None and anchor['slot'] == 0x30, 'resource anchor unavailable')
        require(anchor['family'] == call['family'] and anchor['offset'] == call['offset'],
                'resource anchor disagrees')
        def instruction(witness):
            ins = instructions.get(int(witness['site'], 16))
            require(ins is not None and ins.bytes.hex() == witness['bytes'], 'instruction changed')
            return ins
        lifetime = call.get('receiverLifetime')
        receiver = None
        if lifetime:
            # Explicit reviewed correspondence across an inline temporary
            # destructor, not a relaxation of unknown-write CFG invalidation.
            begin, end = int(lifetime['start'], 16), int(lifetime['end'], 16)
            region = rdata.bytes_at(begin, end - begin)
            require(region is not None and hashlib.sha256(region).hexdigest() == lifetime['sha256'],
                    'reviewed lifetime changed')
            setup = instruction(lifetime['anchorReceiver'])
            require(setup.mnemonic == 'mov' and setup.operands[0].reg == X86_REG_ECX and
                    setup.operands[1].type == X86_OP_REG and
                    setup.address + setup.size == int(call['resourceAnchor'], 16),
                    'anchor receiver mismatch')
            receiver = setup.operands[1].reg
            require(setup.reg_name(receiver) in ('esi', 'edi', 'ebx', 'ebp'),
                    'receiver is not callee saved')
            require(begin == setup.address and end in instructions,
                    'lifetime bounds mismatch')
            require(all(receiver not in i.regs_access()[1] for a, i in instructions.items()
                        if begin <= a < end), 'receiver overwritten in reviewed lifetime')
            # Pin the separately reviewed temporary cleanup and its helper ABI.
            cleanup = lifetime['cleanup']
            a, b = int(cleanup['start'], 16), int(cleanup['end'], 16)
            require(begin < a < b < end and a in instructions and b in instructions,
                    'cleanup bounds mismatch')
            require(hashlib.sha256(rdata.bytes_at(a, b-a)).hexdigest() == cleanup['sha256'],
                    'temporary cleanup changed')
        spill, reload = [instruction(w) for w in call['vtableStack']]
        for ins, witness in zip((spill, reload), call['vtableStack']):
            mem = [o for o in ins.operands if o.type == X86_OP_MEM]
            require(ins.mnemonic == 'mov' and len(mem) == 1 and mem[0].size == 4,
                    'not a dword MOV')
            require(mem[0].mem.base == X86_REG_ESP and not mem[0].mem.index and
                    mem[0].mem.disp + witness['entryEspDelta'] == call['entryOffset'],
                    'stack identity mismatch')
        require(spill.operands[0].type == X86_OP_MEM and reload.operands[1].type == X86_OP_MEM,
                'not a spill/reload pair')
        if lifetime:
            table = instruction(lifetime['tableLoad'])
            require(table.mnemonic == 'mov' and table.operands[0].type == X86_OP_REG and
                    table.operands[1].type == X86_OP_MEM and table.operands[1].mem.base == receiver and
                    table.operands[1].mem.disp == 0 and not table.operands[1].mem.index and
                    spill.operands[1].type == X86_OP_REG and spill.operands[1].reg == table.operands[0].reg and
                    begin <= table.address < spill.address < reload.address < end and
                    all(table.operands[0].reg not in i.regs_access()[1] for a, i in instructions.items()
                        if table.address < a < spill.address), 'saved vtable source mismatch')
        dispatch = instruction(call['dispatch'])
        if lifetime:
            recv = instruction(lifetime['speechReceiver'])
            require(recv.mnemonic == 'mov' and recv.operands[0].reg == X86_REG_ECX and
                    recv.operands[1].type == X86_OP_REG and recv.operands[1].reg == receiver and
                    recv.address + recv.size == dispatch.address and
                    begin <= dispatch.address < end, 'speech receiver mismatch')
        require(dispatch.mnemonic == 'call' and dispatch.operands[0].type == X86_OP_MEM and
                dispatch.operands[0].mem.disp == 0x34 and not dispatch.operands[0].mem.index and
                dispatch.operands[0].mem.base == reload.operands[0].reg,
                'not Speak through the reloaded table')
        values = []
        for witness in call['flagPushes']:
            ins = instruction(witness)
            require(ins.mnemonic == 'push', 'flag is not a PUSH')
            if ins.operands[0].type == X86_OP_IMM:
                values.append(('imm', ins.operands[0].imm))
            else:
                require(lifetime is not None and 'zeroDefinition' in witness,
                        'register flag has no reviewed zero definition')
                zero = instruction(witness['zeroDefinition'])
                reg = ins.operands[0].reg
                require(ins.operands[0].type == X86_OP_REG and zero.mnemonic == 'xor' and
                        all(o.type == X86_OP_REG and o.reg == reg for o in zero.operands) and
                        len(zero.operands) == 2 and zero.reg_name(reg) in ('esi','edi','ebx','ebp'),
                        'flag is not a callee-saved zero')
                require(begin <= zero.address < ins.address < end and
                        all(reg not in i.regs_access()[1] for a, i in instructions.items()
                            if zero.address < a < ins.address), 'zero flag overwritten')
                values.append(('imm', 0))
        require(len(values) == 4 and all(v[1] in (0, 1) for v in values[1:]),
                'invalid speech flags')
        if 'textPush' in call:
            text = instruction(call['textPush'])
            require(lifetime is not None and text.mnemonic == 'push' and
                    text.operands[0].type == X86_OP_IMM and begin <= text.address < dispatch.address,
                    'text is not a reviewed literal PUSH')
            require(rdata.string_at(text.operands[0].imm) == call['textPush']['literal'],
                    'text literal changed')
            text_value = ('imm', text.operands[0].imm)
        else:
            token = call['textToken']
            name, pos = token['name'], token['start']
            require(re.fullmatch(r'[A-Za-z_]\w*', name) is not None and
                    source[pos:pos+len(name)] == name and
                    (pos == 0 or not re.match(r'\w', source[pos-1])) and
                    not re.match(r'\w', source[pos+len(name):pos+len(name)+1]), 'text token mismatch')
            text_value = ('source', name)
        # The reviewed string helpers preserve the pending arguments and saved
        # vtable. Their full bodies are pinned, including native RET conventions.
        for helper in call['helpers']:
            body = rdata.bytes_at(int(helper['address'], 16), helper['size'])
            require(body is not None and hashlib.sha256(body).hexdigest() == helper['sha256'],
                    'helper ABI changed')
        site = dispatch.address
        require(site not in out, 'duplicate call evidence')
        out[site] = dict(family=call['family'], offset=call['offset'], slot=0x34,
                        pushes=[('hero', int(call['heroSite'], 16)), text_value, *values])
        for poll in call.get('taskPolls', []):
            require(lifetime is not None, 'task poll has no reviewed lifetime')
            table, recv, task = [instruction(poll[k]) for k in ('table', 'receiver', 'dispatch')]
            require(begin <= table.address < task.address < end and
                    table.address + table.size == recv.address and recv.address + recv.size == task.address,
                    'task poll is not contiguous')
            require(table.mnemonic == 'mov' and table.operands[0].type == X86_OP_REG and
                    table.operands[1].type == X86_OP_MEM and table.operands[1].mem.base == receiver and
                    not table.operands[1].mem.index and table.operands[1].mem.disp == 0,
                    'task vtable mismatch')
            require(recv.mnemonic == 'mov' and recv.operands[0].reg == X86_REG_ECX and
                    recv.operands[1].type == X86_OP_REG and recv.operands[1].reg == receiver,
                    'task receiver mismatch')
            require(task.mnemonic == 'call' and task.operands[0].type == X86_OP_MEM and
                    task.operands[0].mem.base == table.operands[0].reg and
                    not task.operands[0].mem.index and task.operands[0].mem.disp == 0x68,
                    'task slot mismatch')
            require(task.address not in out, 'duplicate task evidence')
            out[task.address] = dict(family=call['family'], offset=call['offset'], slot=0x68, pushes=[])
    return out
