"""Keep a reviewed local control resource separate from reused stack objects.

Rows pin native/source evidence and each receiver's entry-relative stack slot.
Only resource receiver operands are rewritten; object lookups remain independent.
"""
import hashlib
import re


def restore(source, fn, row, rdata):
    if row is None:
        return source
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG, X86_REG_ECX, X86_REG_ESP

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed resource lifetime {fn["address"]}: {why}')

    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    raw = rdata.bytes_at(start, end - start)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'],
            'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'exported source changed')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw, start))
    require(instructions and instructions[-1].address + instructions[-1].size == end,
            'incomplete disassembly')
    indexed = {i.address: i for i in instructions}
    for address, expected in row['nativeInstructions'].items():
        ins = indexed.get(int(address, 16))
        require(ins is not None and ins.bytes.hex() == expected, 'native witness changed')

    slot = row['entrySlot']
    require(isinstance(slot, int) and slot < 0 and slot % 4 == 0, 'invalid resource slot')
    allowed = {'NewResource', 'PrepareResource', 'TryAcquire', 'ScriptThing', 'ReleaseResource'}
    targets = {'NewResource': 0x99a380, 'PrepareResource': 0xcd23b9,
               'ScriptThing': 0x7e7490, 'ReleaseResource': 0x7e74d0}
    require(set(row['receivers']) == allowed, 'resource operation inventory missing')
    for method, witnesses in row['receivers'].items():
        require(bool(witnesses), 'empty receiver inventory')
        if method in {'ScriptThing', 'PrepareResource', 'ReleaseResource'}:
            native_sites = [int(c['site'], 16) for c in fn['calls']
                            if int(c['target'], 16) == targets[method]]
            require(native_sites == [int(w['call'], 16) for w in witnesses],
                    f'{method} native inventory changed')
        for witness in witnesses:
            lea = indexed.get(int(witness['lea'], 16))
            call = indexed.get(int(witness['call'], 16))
            require(witness['lea'] in row['nativeInstructions'] and
                    witness['call'] in row['nativeInstructions'], 'receiver lacks byte witness')
            require(call is not None and call.mnemonic == 'call' and lea is not None and
                    lea.mnemonic == 'lea' and len(lea.operands) == 2, 'receiver/call missing')
            if method == 'TryAcquire':
                require(call.operands[0].type == X86_OP_MEM and
                        call.operands[0].mem.disp == 0x20, 'acquisition call changed')
            else:
                require(call.operands[0].type == X86_OP_IMM and
                        call.operands[0].imm == targets[method], 'resource API target changed')
            dst, src = lea.operands
            require(dst.type == X86_OP_REG and src.type == X86_OP_MEM and
                    src.mem.base == X86_REG_ESP and not src.mem.index and
                    src.mem.disp + witness['entryEspDelta'] == slot,
                    'receiver stack identity changed')
            require(method == 'TryAcquire' or dst.reg == X86_REG_ECX,
                    'member receiver register changed')
            require(lea.address < call.address and call.address - lea.address <= 40,
                    'receiver too far from call')

    alias, original = row['alias'], row['constructorHandle']
    require(re.fullmatch(r'[A-Za-z_]\w*', alias) is not None and
            re.fullmatch(r'[A-Za-z_]\w*', original) is not None, 'invalid handle name')
    require(re.search(r'\b' + re.escape(alias) + r'\b', source) is None, 'alias collision')
    constructor = re.compile(r'^(?P<indent>[ \t]*)(?:local )?' + re.escape(original) +
                             r' = resources:NewResource\(\)$', re.M)
    require(len(constructor.findall(source)) == len(row['receivers']['NewResource']) == 1,
            'constructor inventory changed')
    changes = []
    guard_pattern = re.compile(r'not resources:ScriptThing\((?P<handle>\w+)\):IsNull\(\)')
    guards = list(guard_pattern.finditer(source))
    reviewed_guards = row.get('prepareGuards', [])
    require([m['handle'] for m in guards] == [g['handle'] for g in reviewed_guards],
            'prepare guard inventory changed')
    prepares = list(re.finditer(r'resources:PrepareResource\((\w+)\)', source))
    for guard, witness in zip(guards, reviewed_guards):
        index = witness['prepareIndex']
        require(isinstance(index, int) and 0 <= index < len(prepares), 'prepare guard index missing')
        prepare = prepares[index]
        require(guard.end() < prepare.start() and prepare[1] == witness['handle'] and
                'resources:' not in source[guard.end():prepare.start()],
                'prepare guard correspondence changed')
        changes.append((guard.start('handle'), guard.end('handle'), alias))
    for method in sorted(allowed - {'NewResource'}):
        pattern = re.compile(r'resources:' + method + r'\((?P<handle>\w+)(?=[,)])')
        matches = list(pattern.finditer(source))
        if method == 'ScriptThing':
            matches = [m for m in matches if not any(g.start() <= m.start() < g.end() for g in guards)]
        expected = row['sourceHandles'][method]
        require([m['handle'] for m in matches] == expected and
                len(matches) == len(row['receivers'][method]),
                f'{method} source inventory changed')
        changes.extend((m.start('handle'), m.end('handle'), alias) for m in matches)
    for a, b, value in sorted(changes, reverse=True):
        source = source[:a] + value + source[b:]
    return constructor.sub(lambda m: m[0] + '\n' + m['indent'] +
                           f'local {alias} = {original}', source)
