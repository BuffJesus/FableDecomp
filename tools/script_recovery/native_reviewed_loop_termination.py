"""Restore a reviewed native loop-head poll exported at the loop's tail."""
import hashlib
import re


def restore(text, fn, row, rdata):
    if row is None:
        return text

    def require(ok, reason):
        if not ok:
            raise ValueError(f'reviewed loop termination {fn["address"]}: {reason}')

    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    raw = rdata.bytes_at(start, end - start)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'],
            'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'exported source changed')
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    disassembler = Cs(CS_ARCH_X86, CS_MODE_32)
    disassembler.detail = True
    instructions = list(disassembler.disasm(raw, start))
    by_site = {i.address: i for i in instructions}
    require(instructions and instructions[-1].address + instructions[-1].size == end,
            'native disassembly incomplete')
    for site, encoded in row['nativeInstructions'].items():
        ins = by_site.get(int(site, 16))
        require(ins is not None and ins.bytes.hex() == encoded, f'instruction changed at {site}')

    def at(key):
        ins = by_site.get(int(row[key], 16))
        require(ins is not None, f'{key} is not an instruction')
        return ins

    entry, poll, empty, back = [at(k) for k in ('loopEntry', 'pollSite', 'emptyBranch', 'backedge')]
    owner = row['ownerRegister']
    require(owner in ('esi', 'edi', 'ebx', 'ebp'), 'owner is not callee-saved')
    import capstone
    from capstone import x86
    from tools.script_recovery.native_receiver_reaching import _cfg, _reaching
    definitions = [k for k, i in enumerate(instructions[:12])
                   if i.mnemonic == 'mov' and i.op_str == owner + ', ecx']
    require(len(definitions) == 1, 'incoming owner definition changed')
    predecessors = _cfg(instructions, x86, capstone, rdata)
    require(predecessors is not None, 'native control flow is unresolved')
    successors = {k: [] for k in predecessors}
    for child, parents in predecessors.items():
        for parent in parents:
            successors[parent].append(child)
    reachable, pending = set(), [0]
    while pending:
        k = pending.pop()
        if k not in reachable:
            reachable.add(k)
            pending.extend(successors[k])
    predecessors = {k: [p for p in parents if p in reachable]
                    for k, parents in predecessors.items()}
    owner_id = getattr(x86, 'X86_REG_' + owner.upper())
    require(instructions.index(entry) in reachable and _reaching(instructions, predecessors,
            instructions.index(entry), owner_id, x86) == set(definitions),
            'loop owner is not incoming this on every path')
    require(entry.mnemonic == 'mov' and entry.op_str == 'ecx, ' + owner
            and entry.address + entry.size == poll.address, 'loop receiver setup changed')
    require(poll.mnemonic == 'call' and poll.op_str == '0xcb7940', 'termination target changed')
    test = by_site.get(poll.address + poll.size)
    require(test is not None and test.mnemonic == 'test' and test.op_str == 'al, al',
            'termination AL test changed')
    branch = by_site.get(test.address + test.size)
    require(branch is not None and branch.mnemonic == 'jne'
            and branch.op_str == hex(int(row['cleanupTarget'], 16)), 'termination exit changed')
    require(empty.mnemonic == 'je' and empty.op_str == hex(int(row['normalExit'], 16))
            and empty.address + empty.size == entry.address, 'initial loop entry changed')
    require(back.mnemonic == 'jb' and back.op_str == hex(entry.address)
            and branch.address < back.address < int(row['normalExit'], 16), 'loop backedge changed')
    require(int(row['cleanupTarget'], 16) > back.address, 'cleanup falls inside loop')
    require(int(row['cleanupTarget'], 16) in by_site and int(row['normalExit'], 16) in by_site,
            'loop exits leave function')
    anchors = {int(site, 16) for site in row['nativeInstructions']}
    require(all(i.address in anchors for i in instructions
                if empty.address <= i.address <= back.address), 'loop instruction witness incomplete')

    loop = row['sourceLoop'].replace('\r\n', '\n')
    guard = row['sourceGuard'].replace('\r\n', '\n')
    label = f'LAB_{int(row["cleanupTarget"], 16):08x}'
    require(re.fullmatch(r'        alive = not quest:IsActiveThreadTerminating\(\)\n'
                         r'        (\w+) = not alive\n'
                         r'        if \1 then goto ' + label + r' end\n', guard), 'source poll/test changed')
    header, footer = '    while true do\n', '    end'
    require(loop.startswith(header) and loop.endswith(guard + footer)
            and loop.count(guard) == 1 and loop.count(header) == 1,
            'source loop shape changed')
    variants = {loop, loop.replace('\n', '\r\n')}
    matches = [v for v in variants if text.count(v) == 1]
    require(len(matches) == 1, 'source loop is missing or ambiguous')
    matched = matches[0]
    replacement = header + guard + loop[len(header):].removesuffix(guard + footer) + footer
    if '\r\n' in matched:
        replacement = replacement.replace('\n', '\r\n')
    return text.replace(matched, replacement, 1)
