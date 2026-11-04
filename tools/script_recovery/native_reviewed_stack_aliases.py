"""Keep reviewed scalar stack lifetimes apart from overlapping resource objects."""
import hashlib


def restore_lua(text, fn, row, rdata):
    """Apply a reviewed late scalar separation without changing earlier phases."""
    if row is None:
        return text
    if (not isinstance(row.get('textSha256'), str)
            or hashlib.sha256(text.encode()).hexdigest() != row['textSha256']):
        raise ValueError(f'reviewed Lua stack alias {fn["address"]}: source phase changed')
    dependencies = row.get('nativeDependencies')
    if not isinstance(dependencies, list) or not dependencies:
        raise ValueError(f'reviewed Lua stack alias {fn["address"]}: native dependencies missing')
    seen = set()
    for dependency in dependencies:
        address, raw = int(dependency['address'], 16), bytes.fromhex(dependency['bytes'])
        if address in seen or not raw or rdata.bytes_at(address, len(raw)) != raw:
            raise ValueError(f'reviewed Lua stack alias {fn["address"]}: native dependency changed')
        seen.add(address)
    return restore(text, fn, row, rdata)


def restore(text, fn, row, rdata):
    """Apply exact C spelling edits after proving the exported and native inputs.

    Ghidra may name the same native scalar differently across control flow and
    place a later spelling inside a resource's apparent extent. The ordinary
    object canonicalizer then merges it into the resource. Rows pin the native
    body, source, instructions, and each exact textual use before separating
    that scalar lifetime; a stale or ambiguous row must stop conversion.
    """
    if row is None:
        return text
    start = int(fn['address'], 16)
    size = int(fn['bodyEndExclusive'], 16) - start
    raw = rdata.bytes_at(start, size)
    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed stack alias {fn["address"]}: {why}')
    if 'textSha256' in row:
        require(isinstance(row['textSha256'], str)
                and hashlib.sha256(text.encode()).hexdigest() == row['textSha256'],
                'source phase changed')
    if 'nativeDependencies' in row:
        dependencies = row['nativeDependencies']
        require(isinstance(dependencies, list) and bool(dependencies),
                'native dependencies missing')
        seen = set()
        for dependency in dependencies:
            require(isinstance(dependency, dict), 'invalid native dependency')
            try:
                address = int(dependency['address'], 16)
                encoded = bytes.fromhex(dependency['bytes'])
            except (KeyError, TypeError, ValueError):
                require(False, 'invalid native dependency')
            require(address >= 0 and address not in seen and bool(encoded)
                    and rdata.bytes_at(address, len(encoded)) == encoded,
                    'native dependency changed')
            seen.add(address)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'],
            'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'exported source changed')
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    decoder = Cs(CS_ARCH_X86, CS_MODE_32); decoder.detail = True
    instructions = list(decoder.disasm(raw, start))
    require(instructions and instructions[-1].address + instructions[-1].size == start + size,
            'native disassembly incomplete')
    by_site = {ins.address: ins for ins in instructions}
    require(len(row['nativeInstructions']) >= 3, 'too few native instruction anchors')
    for site, encoded in row['nativeInstructions'].items():
        ins = by_site.get(int(site, 16))
        require(ins is not None and ins.bytes.hex() == encoded,
                f'native instruction changed at {site}')
    aliases = [row['alias'], *row.get('additionalAliases', [])]
    require(len(aliases) == len(set(aliases)) and
            all(alias.isidentifier() and alias not in text for alias in aliases),
            'alias already used or duplicated')
    require(len(row['edits']) >= 2 and len({old for old, _ in row['edits']}) == len(row['edits']),
            'edits are missing or duplicated')
    counts = row.get('editCounts', [1] * len(row['edits']))
    require(isinstance(counts, list) and len(counts) == len(row['edits']) and
            all(isinstance(n, int) and not isinstance(n, bool) and n > 0 for n in counts),
            'invalid reviewed occurrence counts')
    for (old, new), count in zip(row['edits'], counts):
        require(any(alias in new for alias in aliases) and old not in new and '\n' not in old + new,
                'edit does not isolate the reviewed scalar')
        require(text.count(old) == count, f'expected {count} occurrences of {old!r}')
        text = text.replace(old, new)
    # A DWORD copied unchanged into a float-only interface argument carries
    # IEEE32 bits. Explicit reviewed sites restore that read's pointee type.
    from capstone import x86
    import re
    reads = row.get('globalFloatArguments', [])
    require(isinstance(reads, list), 'invalid global float arguments')
    seen_offsets = set()
    for read in reads:
        offset = read.get('offset') if isinstance(read, dict) else None
        require(isinstance(offset, str) and re.fullmatch(r'0x[0-9a-f]+', offset)
                and offset not in seen_offsets, 'invalid or duplicated global float offset')
        seen_offsets.add(offset)
        load = by_site.get(int(read['readSite'], 16))
        require(load is not None and load.mnemonic == 'mov' and len(load.operands) == 2
                and load.operands[0].type == x86.X86_OP_REG
                and load.operands[0].size == 4
                and load.operands[1].type == x86.X86_OP_MEM
                and load.operands[1].size == 4
                and load.operands[1].mem.disp == int(offset, 16)
                and not load.operands[1].mem.index, 'global float load changed')
        index = instructions.index(load); pointer = instructions[index-1] if index else None
        require(pointer is not None and pointer.mnemonic == 'mov'
                and pointer.operands[0].type == x86.X86_OP_REG
                and pointer.operands[0].reg == load.operands[1].mem.base
                and pointer.operands[1].type == x86.X86_OP_MEM
                and not pointer.operands[1].mem.base and not pointer.operands[1].mem.index
                and pointer.operands[1].mem.disp == 0x0143e90c, 'global model pointer load changed')
        call = by_site.get(int(read['callSite'], 16)); slot = int(read['slot'], 16)
        require(call is not None and call.mnemonic == 'call' and call.operands[0].type == x86.X86_OP_MEM
                and call.operands[0].mem.disp == slot, 'global float call slot changed')
        tail = instructions[index+1:instructions.index(call)]
        require(len(tail) == 2 and tail[0].mnemonic == 'mov' and tail[1].mnemonic == 'push'
                and tail[1].operands[0].type == x86.X86_OP_REG
                and tail[1].operands[0].reg == load.operands[0].reg
                and load.operands[0].reg not in tail[0].regs_access()[1],
                'global float bits not passed unchanged')
        from tools.script_recovery.annotate_interface_slots import load_slots
        from tools.script_recovery.lift_native_lua import load_manifest
        from tools.script_recovery.convert_quest_unit import callee_stack_words
        target = int(read['target'], 16)
        require(load_slots().get(slot) == read['method']
                and rdata.bytes_at(0x01260f0c+slot, 4) == target.to_bytes(4, 'little'),
                'global float interface target changed')
        parameters = load_manifest().get(read['method'], {}).get('parameters', [])
        require(len(parameters) == 1 and parameters[0]['type'] == 'float'
                and callee_stack_words(target) == 1, 'global float argument ABI changed')
        floating = bytes.fromhex(read['calleeFloatBytes'])
        require(rdata.bytes_at(int(read['calleeFloatUse'], 16), len(floating)) == floating,
                'callee float use changed')
        decoded = list(decoder.disasm(floating, int(read['calleeFloatUse'], 16)))
        require(len(decoded) == 1 and decoded[0].mnemonic in ('fld', 'fmul', 'fadd', 'fsub', 'fdiv')
                and decoded[0].operands[0].type == x86.X86_OP_MEM
                and decoded[0].operands[0].size == 4, 'callee does not use IEEE32 argument')
        old = '*(undefined4 *)(DAT_0143e90c + ' + offset + ')'
        require(text.count(old) == 1, 'global float read spelling missing or ambiguous')
        text = text.replace(old, '*(float *)(DAT_0143e90c + ' + offset + ')')
    return text


def materialize_string_values(text, row):
    # Reviewed CString values require a variable when a branch can retain the
    # constructed default instead of reaching a shared pointer assignment.
    # Materialize only explicitly reviewed aliases; native/source/call-site
    # witnesses above remain mandatory, and default rows retain their behavior.
    def require(ok, why):
        if not ok:
            raise ValueError("reviewed stack alias string values: " + why)
    import re
    if row is None:
        return text
    aliases = [row['alias'], *row.get('additionalAliases', [])]
    values = row.get('stringValueAliases', [])
    require(isinstance(values, list), 'invalid string value aliases')
    seen = set()
    for value in values:
        require(isinstance(value, dict), 'invalid string value alias')
        alias = value.get('alias')
        require(alias in aliases and alias not in seen, 'unreviewed or duplicated string value alias')
        seen.add(alias)
        counts = [value.get('constructorCount'), value.get('assignmentCount')]
        require(all(isinstance(n, int) and not isinstance(n, bool) and n > 0 for n in counts),
                'invalid string value occurrence counts')
        escaped = re.escape(alias)
        receiver = r'(?:\(CCharString \*\)\s*)?&?' + escaped
        constructor = re.compile(r'CCharString::CCharString\s*\(\s*' + receiver +
                                 r'\s*,\s*("(?:[^"\\]|\\.)*")\s*,\s*-1\s*\);')
        assignment = re.compile(r'CCharString::operator=\s*\(\s*' + receiver +
                                r'\s*,\s*(\w+)\s*\);')
        require(len(constructor.findall(text)) == counts[0], 'string value constructor changed')
        require(len(assignment.findall(text)) == counts[1], 'string value assignment changed')
        text = constructor.sub(lambda m: alias + ' = ' + m[1] + ';', text)
        text = assignment.sub(lambda m: alias + ' = ' + m[1] + ';', text)
    return text
