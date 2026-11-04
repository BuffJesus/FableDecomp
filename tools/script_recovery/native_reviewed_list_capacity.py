"""Recognize a reviewed capacity-word initializer after native vectors become lists."""
import hashlib
import re


def restore(text, fn, row, rdata):
    if row is None:
        return text

    def require(ok, reason):
        if not ok:
            raise ValueError(f'reviewed list capacity {fn["address"]}: {reason}')

    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    raw = rdata.bytes_at(start, end - start)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'],
            'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'exported source changed')
    normalized = text.replace('\r\n', '\n')
    require(hashlib.sha256(normalized.encode()).hexdigest() == row['loweredSha256'],
            'lowered source changed')

    import capstone
    from capstone import x86
    from tools.script_recovery.native_receiver_reaching import _cfg, _reaching
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    instructions = list(md.disasm(raw, start))
    require(instructions and instructions[-1].address + instructions[-1].size == end,
            'native disassembly incomplete')
    sites = {i.address: n for n, i in enumerate(instructions)}
    site = int(row['callSite'], 16)
    require(site in sites, 'initializer site missing')
    k = sites[site]
    require(k >= 6 and k + 8 < len(instructions), 'initializer context missing')
    call = instructions[k]
    require(call.mnemonic == 'call' and call.operands[0].type == x86.X86_OP_IMM,
            'initializer is not a direct call')
    helper = call.operands[0].imm
    # ECX destination; second stack argument value; first argument ignored.
    # EAX returns that destination; RET 8 restores the caller's balanced ESP.
    require(rdata.bytes_at(helper, 11) == bytes.fromhex('8bc18b4c24088908c20800'),
            'initializer helper changed')
    for address, encoded in row['nativeInstructions'].items():
        n = sites.get(int(address, 16))
        require(n is not None and instructions[n].bytes.hex() == encoded,
                f'native instruction changed at {address}')
    require(all(hex(i.address) in row['nativeInstructions'] for i in instructions[k-6:k+9]),
            'initializer witness incomplete')

    prefix = instructions[k-6:k]
    require([i.mnemonic for i in prefix] == ['push','lea','push','lea','mov','mov'],
            'header setup changed')
    require(prefix[0].operands[0].type == x86.X86_OP_REG, 'zero argument is not a register')
    zero = prefix[0].operands[0].reg
    require(zero in (x86.X86_REG_EBP,x86.X86_REG_EBX,x86.X86_REG_ESI,x86.X86_REG_EDI),
            'zero argument is not callee-saved')
    zero_name = md.reg_name(zero)
    require(re.fullmatch(r'ecx, \[esp \+ 0x[0-9a-f]+\]', prefix[1].op_str)
            and prefix[2].op_str == 'ecx', 'ignored allocator argument changed')
    def stack_offset(ins, destination):
        operands = ins.operands
        m = operands[1 if destination == 'ecx' else 0]
        require(m.type == x86.X86_OP_MEM and m.mem.base == x86.X86_REG_ESP
                and not m.mem.index, 'header is not a direct stack slot')
        if destination == 'ecx':
            require(operands[0].type == x86.X86_OP_REG and operands[0].reg == x86.X86_REG_ECX,
                    'header receiver changed')
        else:
            require(operands[1].type == x86.X86_OP_REG and operands[1].reg == zero
                    and m.size == 4, 'header zero store changed')
        return m.mem.disp
    capacity = stack_offset(prefix[3], 'ecx')
    begin = stack_offset(prefix[4], 'memory')
    require(stack_offset(prefix[5], 'memory') == begin + 4 and capacity == begin + 8,
            'vector header words are not adjacent')
    preds = _cfg(instructions, x86, capstone, rdata)
    require(preds is not None, 'native control flow unresolved')
    defs = _reaching(instructions, preds, k-6, zero, x86)
    require(defs and -1 not in defs and all(instructions[n].mnemonic == 'xor'
            and instructions[n].op_str == zero_name + ', ' + zero_name for n in defs),
            'initializer value is not proven zero')
    suffix = instructions[k+1:k+9]
    require(suffix[2].mnemonic == 'call' and suffix[2].op_str == 'dword ptr [edx + 0x118]'
            and suffix[3].mnemonic == 'mov' and suffix[3].op_str == 'edx, dword ptr [eax]'
            and suffix[4].mnemonic == 'lea' and stack_offset(suffix[4], 'ecx') == begin - 8
            and suffix[5].mnemonic == 'push' and suffix[5].op_str == 'ecx'
            and suffix[6].mnemonic == 'mov' and suffix[6].op_str == 'ecx, eax'
            and suffix[7].mnemonic == 'call' and suffix[7].op_str == 'dword ptr [edx + 0xdc]',
            'vector is not passed to the kill-group query')

    statement = row['statement']
    match = re.fullmatch(r'(?P<label>[\w:]+)\((?P<capacity>\w+),\(int\)&(?P<allocator>\w+),0\);', statement)
    require(match is not None and normalized.count(statement) == 1, 'initializer source changed')
    calls = [c for c in fn.get('calls', []) if int(c['target'], 16) == helper]
    require(len(calls) == 1 and calls[0]['currentName'].lstrip(':') == match['label'].lstrip(':'),
            'initializer source label is ambiguous')
    for name, declaration in ((match['capacity'], r'undefined1 {name} \[4\];'),
                              (match['allocator'], r'undefined1 {name};')):
        require(len(re.findall(r'\b' + re.escape(name) + r'\b', normalized)) == 2
                and re.search(declaration.format(name=re.escape(name)), normalized),
                'capacity or allocator escapes the lowered list')
    vec = re.escape(row['vector'])
    require(re.search(vec + r' = ENGINE_EmptyList\(\);\s*' + re.escape(statement)
                      + r'\s*\w+ = GSI->GetHero\(\);\s*' + vec
                      + r' = CScriptThing::MsgGetThingsKilledGroups\(\w+\);', normalized),
            'native vector has not been lowered to a list')
    return text.replace(statement, '', 1)
