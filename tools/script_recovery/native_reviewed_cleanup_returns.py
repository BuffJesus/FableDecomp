"""Restore a reviewed movie/resource return tail misnested by the decompiler."""
import hashlib
import re

from capstone import Cs, CS_ARCH_X86, CS_MODE_32, x86


def restore(source, fn, row, rdata):
    if row is None:
        return source

    def require(ok, reason):
        if not ok:
            raise ValueError(f'reviewed cleanup return {fn["address"]}: {reason}')

    normalized = source.replace('\r\n', '\n')
    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    raw = rdata.bytes_at(start, end - start)
    for key, data in [('nativeSha256', raw), ('sourceSha256', fn['decompile'].encode()),
                      ('loweredSha256', normalized.encode())]:
        require(data is not None and hashlib.sha256(data).hexdigest() == row[key], key + ' changed')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw, start))
    require(instructions and instructions[-1].address + instructions[-1].size == end,
            'incomplete native body')
    movie, release, tail_end = row['movieSite'], row['releaseSite'], row['tailEnd']
    tail = [ins for ins in instructions if movie <= ins.address < tail_end]
    require(len(tail) == 9 and [ins.mnemonic for ins in tail] ==
            ['call', 'lea', 'call', 'pop', 'pop', 'pop', 'pop', 'add', 'ret'],
            'unsupported native return tail')
    require(tail[-1].address + tail[-1].size == tail_end <= end,
            'return boundary changed')
    witnesses = row['nativeInstructions']
    indexed = {ins.address: ins for ins in instructions}
    require(all(hex(ins.address) in witnesses for ins in tail), 'incomplete tail witness')
    for site, encoded in witnesses.items():
        ins = indexed.get(int(site, 16))
        require(ins is not None and ins.bytes.hex() == encoded, 'native instruction changed')
    for ins, target in [(tail[0], 0x6e7b80), (tail[2], 0x7e74d0)]:
        require(ins.operands[0].type == x86.X86_OP_IMM and ins.operands[0].imm == target,
                'cleanup helper target changed')
        encoded = row['helperBytes'].get(hex(target))
        actual = rdata.bytes_at(target, 66)
        require(encoded and len(encoded) == 132 and actual is not None and actual.hex() == encoded,
                'cleanup helper changed')
    require(tail[2].address == release, 'resource release site changed')
    registers = [ins.op_str for ins in tail[3:7]]
    require(len(set(registers)) == 4 and set(registers) == {'ebp', 'ebx', 'esi', 'edi'},
            'saved register epilogue changed')
    frame = row['frameBytes']
    adjustment = tail[7].operands
    require(adjustment[0].type == x86.X86_OP_REG and adjustment[0].reg == x86.X86_REG_ESP
            and adjustment[1].type == x86.X86_OP_IMM and adjustment[1].imm + 16 == frame,
            'stack frame changed')
    destination, storage = tail[1].operands
    require(destination.type == x86.X86_OP_REG and destination.reg == x86.X86_REG_ECX
            and storage.type == x86.X86_OP_MEM and storage.mem.base == x86.X86_REG_ESP
            and not storage.mem.index and storage.mem.disp - frame == row['resourceSlot'],
            'resource receiver changed')
    calls = {int(call['site'], 16): call for call in fn['calls']}
    for site, target in [(movie, 0x6e7b80), (release, 0x7e74d0)]:
        call = calls.get(site)
        require(call and int(call['target'], 16) == target and call.get('depth') == frame,
                'native cleanup call metadata changed')
    require(calls[release].get('ecxStack') == row['resourceSlot'], 'resource metadata changed')
    resource, phi = row['resourceAlias'], row['movieAlias']
    require(all(re.fullmatch(r'[A-Za-z_]\w*', name) for name in (resource, phi)) and resource != phi,
            'invalid cleanup aliases')
    require(normalized.count(resource + ' = RESOURCE_NewResource();') == 1,
            'resource construction ambiguous')
    # Each native edge keeps the selected movie receiver; no resource-specific
    # glue is introduced. Complete source/native hashes pin the reviewed phi.
    assignments = re.findall(r'\b' + re.escape(phi) +
                             r' = \(CScriptGameResourceObjectMovieBase \*\)(\w+);', normalized)
    require(assignments == row['movieAssignments'], 'movie phi inventory changed')
    label = f'LAB_{movie:08x}'
    pattern = re.compile(r'(?P<header>^' + label + r':\n(?P<indent>[ \t]*)' +
                         re.escape(f'RESOURCE_DestroyMovie({phi});') + r')', re.M)
    require(len(list(pattern.finditer(normalized))) == 1, 'shared source label ambiguous')
    normalized = restore_continuations(normalized, fn, row.get('continuations', []), rdata)
    return pattern.sub(lambda match: match['header'] + '\n' + match['indent'] +
                       f'RESOURCE_ReleaseResource({resource});\n' + match['indent'] + 'return;', normalized)

def restore_continuations(source, fn, rows, rdata):
    """Link a reviewed movie label to an existing timer/resource return epilogue."""
    def require(ok, reason):
        if not ok:
            raise ValueError(f'reviewed cleanup continuation {fn["address"]}: {reason}')

    require(isinstance(rows, list), 'invalid continuation inventory')
    seen = set()
    native_calls = {int(c['site'], 16): c for c in fn['calls']}
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    for row in rows:
        start, end, frame = row['movieSite'], row['tailEnd'], row['frameBytes']
        require(start not in seen, 'duplicate continuation')
        seen.add(start)
        require(int(fn['address'], 16) <= start < end <= int(fn['bodyEndExclusive'], 16),
                'continuation outside native function')
        raw = rdata.bytes_at(start, end-start)
        require(raw is not None, 'missing native tail')
        tail = list(decoder.disasm(raw, start))
        require(len(tail) == 15 and tail[-1].address + tail[-1].size == end and
                [i.mnemonic for i in tail] == ['call', 'lea', 'call', 'lea', 'call',
                'lea', 'call', 'lea', 'call', 'pop', 'pop', 'pop', 'pop', 'add', 'ret'],
                'unsupported continuation tail')
        require(row['nativeInstructions'] == {hex(i.address): i.bytes.hex() for i in tail},
                'native tail witness changed')
        calls = row['calls']
        require(len(calls) == 5 and [c['kind'] for c in calls] ==
                ['movie', 'timer', 'managedThing', 'timer', 'resource'] and
                [c['target'] for c in calls] == [0x6e7b80, 0xcd4470, 0x4aa840, 0xcd4470, 0x7e74d0],
                'unsupported cleanup operations')
        for index, call in zip((0, 2, 4, 6, 8), calls):
            ins = tail[index]
            require(ins.address == call['site'] and ins.operands[0].type == x86.X86_OP_IMM
                    and ins.operands[0].imm == call['target'], 'cleanup call changed')
            metadata = native_calls.get(ins.address)
            require(metadata and int(metadata['target'], 16) == call['target'] and
                    metadata.get('depth') == frame, 'cleanup call metadata changed')
            if index:
                dst, mem = tail[index-1].operands
                require(dst.type == x86.X86_OP_REG and dst.reg == x86.X86_REG_ECX and
                        mem.type == x86.X86_OP_MEM and mem.mem.base == x86.X86_REG_ESP and
                        not mem.mem.index and mem.mem.disp-frame == call['slot'] and
                        metadata.get('ecxStack') == call['slot'], 'cleanup receiver changed')
        registers = [i.op_str for i in tail[9:13]]
        require(len(set(registers)) == 4 and set(registers) == {'ebp', 'edi', 'esi', 'ebx'},
                'saved registers changed')
        dst, amount = tail[13].operands
        require(dst.type == x86.X86_OP_REG and dst.reg == x86.X86_REG_ESP and
                amount.type == x86.X86_OP_IMM and amount.imm+16 == frame, 'stack frame changed')
        for helper, size in ((0x6e7b80, 66), (0xcd4470, 20), (0x4aa840, 66),
                             (0x99a2e0, 7), (0x7e74d0, 66)):
            data = rdata.bytes_at(helper, size)
            require(data is not None and row['helperBytes'].get(hex(helper)) == data.hex(),
                    'cleanup helper changed')
        # The Thing helper's final jump is the native opaque-vtable initializer.
        # Lua/host ownership already covers that temporary; it is not a resource ID.
        thing = list(decoder.disasm(rdata.bytes_at(0x4aa840, 66), 0x4aa840))[-1]
        require(thing.mnemonic == 'jmp' and thing.operands[0].imm == 0x99a2e0,
                'managed Thing helper boundary changed')
        timers, resource, movie = row['timerAliases'], row['resourceAlias'], row['movieAlias']
        require(len(timers) == 2 and len(set([*timers, resource, movie])) == 4 and
                all(re.fullmatch(r'[A-Za-z_]\w*', a) for a in [*timers, resource, movie]),
                'invalid cleanup aliases')
        require(row['continuationSite'] == calls[1]['site'], 'continuation target changed')
        destination = f"LAB_{row['continuationSite']:08x}"
        epilogue = re.compile(r'^' + destination + r':\n\s*' +
            re.escape(f'GSI->DeregisterTimer({timers[0]});') + r'\nLAB_[0-9a-f]{8}:\n\s*' +
            re.escape(f'GSI->DeregisterTimer({timers[1]});') + r'\n\s*' +
            re.escape(f'RESOURCE_ReleaseResource({resource});') + r'\n\s*return;', re.M)
        require(len(list(epilogue.finditer(source))) == 1, 'existing cleanup epilogue ambiguous')
        label = f'LAB_{start:08x}'
        pattern = re.compile(r'(?P<header>^' + label + r':\n(?P<indent>[ \t]*)' +
                             re.escape(f'RESOURCE_DestroyMovie({movie});') + r')', re.M)
        require(len(list(pattern.finditer(source))) == 1, 'movie label ambiguous')
        source = pattern.sub(lambda m: m['header'] + '\n' + m['indent'] +
                             f'goto {destination};', source)
    return source
