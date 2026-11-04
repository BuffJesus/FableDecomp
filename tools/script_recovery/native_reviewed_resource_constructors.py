"""Recover an inlined resource's actual storage from reviewed native witnesses."""
import hashlib
import re

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG, X86_REG_ECX, X86_REG_EDI, X86_REG_ESP


def restore(source, fn, row, rdata):
    if row is None:
        return source

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed resource constructor {fn["address"]}: {why}')

    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    raw = rdata.bytes_at(start, end - start)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'], 'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'], 'export changed')
    normalized = source.replace('\r\n', '\n').replace('\r', '\n')
    require(hashlib.sha256(normalized.encode()).hexdigest() == row['loweredSha256'], 'lowered source changed')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw, start))
    require(instructions and instructions[-1].address + instructions[-1].size == end, 'incomplete disassembly')
    indexed = {i.address: i for i in instructions}
    for address, encoded in row['nativeInstructions'].items():
        ins = indexed.get(int(address, 16))
        require(ins is not None and ins.bytes.hex() == encoded, 'instruction changed')
    for address, encoded in row['helperBytes'].items():
        data = rdata.bytes_at(int(address, 16), len(encoded) // 2)
        require(data is not None and data.hex() == encoded, 'helper changed')

    slot, frame = row['entrySlot'], row['frameBytes']
    old, handle = row['sourceHandle'], row['resourceHandle']
    require(isinstance(slot, int) and slot < 0 and slot % 4 == 0 and frame > 0, 'invalid stack identity')
    require(re.fullmatch(r'[A-Za-z_]\w*', old) and handle == f'xStack_{-slot:x}', 'invalid resource handle')
    require(old != handle, 'resource alias unchanged')
    calls = {int(c['site'], 16): c for c in fn['calls']}

    def witness(address):
        require(address in row['nativeInstructions'], 'missing instruction witness')
        ins = indexed.get(int(address, 16))
        require(ins is not None, 'missing instruction')
        return ins

    def receiver(part, target, depth=None):
        depth = frame if depth is None else depth
        lea, call = witness(part['lea']), witness(part['call'])
        require(lea.mnemonic == 'lea' and len(lea.operands) == 2, 'missing receiver address')
        dest, operand = lea.operands
        require(dest.type == X86_OP_REG and dest.reg == X86_REG_ECX and operand.type == X86_OP_MEM
                and operand.mem.base == X86_REG_ESP and operand.mem.index == 0
                and operand.mem.disp - depth == slot, 'receiver stack changed')
        require(call.mnemonic == 'call' and call.operands[0].type == X86_OP_IMM
                and call.operands[0].imm == target and lea.address < call.address
                and call.address - lea.address <= 24, 'helper target changed')
        evidence = calls.get(call.address)
        require(evidence is not None and int(evidence['target'], 16) == target
                and evidence.get('ecxStack') == slot and evidence.get('depth') == depth, 'call metadata changed')
        require(hex(target) in row['helperBytes'], 'missing helper witness')

    def stores(part, depth, vtable):
        zero_register = part.get('zeroRegisterName', 'edi')
        require(zero_register in ('edi', 'ebx'), 'unsupported zero register')
        zero_reg = decoder.reg_name(X86_REG_EDI) if zero_register == 'edi' else 'ebx'
        ins = witness(part['vtable'])
        require(ins.mnemonic == 'mov' and len(ins.operands) == 2, 'missing vtable store')
        dest, value = ins.operands
        require(dest.type == X86_OP_MEM and dest.mem.base == X86_REG_ESP and dest.mem.index == 0
                and dest.mem.disp - depth == slot and value.type == X86_OP_IMM
                and value.imm == vtable, 'vtable identity changed')
        offsets = []
        for address in part['zeros']:
            zero = witness(address)
            require(zero.mnemonic == 'mov' and len(zero.operands) == 2, 'missing zero store')
            dest, value = zero.operands
            require(dest.type == X86_OP_MEM and dest.mem.base == X86_REG_ESP and dest.mem.index == 0
                    and value.type == X86_OP_REG and decoder.reg_name(value.reg) == zero_reg, 'zero store changed')
            if 'zeroDefinitions' in part:
                import capstone
                from capstone import x86
                from tools.script_recovery.native_receiver_reaching import _cfg, _reaching
                predecessors = _cfg(instructions, x86, capstone, rdata)
                require(predecessors is not None, 'unbounded resource cleanup CFG')
                indices = {i.address: k for k, i in enumerate(instructions)}
                definitions = _reaching(instructions, predecessors, indices[zero.address], value.reg, x86)
                require(definitions and -1 not in definitions, 'undefined cleanup zero register')
                expected = part['zeroDefinitions'].get(address)
                require(expected == [hex(instructions[k].address) for k in sorted(definitions)], 'cleanup zero definitions changed')
                for definition in expected:
                    writer = witness(definition)
                    require(writer.mnemonic == 'xor' and len(writer.operands) == 2
                            and all(o.type == X86_OP_REG and o.reg == value.reg for o in writer.operands),
                            'cleanup register is not zero')
            elif zero_register != 'edi':
                require(False, 'missing cleanup zero proof')
            offsets.append(dest.mem.disp - depth - slot)
        require(offsets == [8, 12], 'resource fields changed')

    constructor, destructor = row['constructor'], row['earlyDestructor']
    receiver(constructor, 0x99a380)
    receiver(destructor, 0x99a430)
    between = [i for i in instructions if int(constructor['call'], 16) < i.address < int(constructor['vtable'], 16)]
    require(all(i.mnemonic in ('push', 'lea', 'mov') and
                (i.mnemonic == 'push' or not i.operands or i.operands[0].type != X86_OP_REG
                 or i.operands[0].reg != X86_REG_ESP) for i in between), 'constructor stack adjustment ambiguous')
    pushes = [hex(i.address) for i in between if i.mnemonic == 'push']
    require(pushes and all(a in row['nativeInstructions'] for a in pushes), 'missing constructor push witness')
    if 'stackPushes' in constructor:
        require(constructor['stackPushes'] == pushes, 'constructor push inventory changed')
    require(constructor['vtableDepth'] == frame + 4 * len(pushes), 'constructor stack depth changed')
    stores(constructor, constructor['vtableDepth'], 0x127094c)
    stores(destructor, frame, 0x126008c)
    zero = witness(constructor['zeroRegister'])
    require(zero.mnemonic == 'xor' and len(zero.operands) == 2
            and all(o.type == X86_OP_REG and o.reg == X86_REG_EDI for o in zero.operands), 'zero register changed')
    acquire = [c for c in fn['indirectCalls'] if c.get('slot') == '0x20'
               and len(c.get('pushedStack', [])) == 3 and c['pushedStack'][1] == slot]
    require([int(c['site'], 16) for c in acquire] == row['acquireSites'], 'acquisition inventory changed')
    require(all(c.get('depth') == frame + 12 and hex(int(c['site'], 16)) in row['nativeInstructions']
                for c in acquire), 'acquisition stack changed')
    require([w['call'] for w in row['acquireReceivers']] == row['acquireSites'], 'acquisition witnesses missing')
    for part in row['acquireReceivers']:
        lea, call = witness(hex(part['lea'])), witness(hex(part['call']))
        require(lea.mnemonic == 'lea' and len(lea.operands) == 2, 'missing acquisition receiver')
        dest, operand = lea.operands
        require(dest.type == X86_OP_REG and operand.type == X86_OP_MEM and operand.mem.base == X86_REG_ESP
                and operand.mem.index == 0 and operand.mem.disp - frame - 4 == slot,
                'acquisition receiver changed')
        require(call.mnemonic == 'call' and call.operands[0].type == X86_OP_MEM
                and call.operands[0].mem.disp == 0x20 and 0 < call.address - lea.address <= 12,
                'acquisition target changed')
    base_destructors = [destructor] + row.get('additionalBaseDestructors', [])
    native_destructors = [c for c in fn['calls'] if int(c['target'], 16) == 0x99a430 and c.get('ecxStack') == slot]
    require([int(c['site'], 16) for c in native_destructors] == [int(p['call'], 16) for p in base_destructors],
            'destructor inventory changed')
    for extra in base_destructors[1:]:
        depth = extra['frameBytes']
        require(depth == frame + 4, 'unsupported late frame phase')
        saved, restored = witness(extra['stackSave']), witness(extra['stackPop'])
        require(saved.mnemonic == 'push' and restored.mnemonic == 'pop'
                and saved.operands[0].type == X86_OP_REG and restored.operands[0].type == X86_OP_REG
                and saved.operands[0].reg == restored.operands[0].reg, 'late saved register changed')
        import capstone
        from capstone import x86
        from tools.script_recovery.native_receiver_reaching import _cfg
        predecessors = _cfg(instructions, x86, capstone, rdata)
        require(predecessors is not None, 'unbounded late-frame CFG')
        indices = {i.address: k for k, i in enumerate(instructions)}
        successors = {k: [] for k in predecessors}
        for k, previous in predecessors.items():
            for p in previous:
                successors[p].append(k)
        # Removing the saved-register instruction must make the late cleanup
        # unreachable from entry. A lexical predecessor alone is insufficient.
        pending, seen = [0], set()
        stop, target = indices[saved.address], indices[int(extra['call'], 16)]
        while pending:
            k = pending.pop()
            if k == stop or k in seen:
                continue
            seen.add(k)
            require(k != target, 'late frame save does not dominate cleanup')
            pending.extend(successors[k])
        call_index = indices[int(extra['call'], 16)]
        require(call_index + 1 < len(instructions) and instructions[call_index + 1].address == restored.address,
                'late register is not restored after cleanup')
        receiver(extra, 0x99a430, depth)
        stores(extra, depth, 0x126008c)

    pattern = re.compile(r'^(?P<indent>[ \t]*)' + re.escape(old) + r' = RESOURCE_NewResource\(\);$', re.M)
    require(len(pattern.findall(normalized)) == 1, 'constructor source inventory changed')
    require(not re.search(r'\b' + re.escape(handle) + r'\s*=', normalized), 'resource already initialized')
    require('RESOURCE_TryAcquire(' + handle + ',' in normalized, 'resource acquisition missing')
    source_calls = {}
    for part, native in zip(base_destructors, native_destructors):
        source_call = part['sourceCall']
        native_name = native['currentName']
        require('::~' in native_name, 'destructor identity missing')
        type_name = native_name.split('::~')[0]
        require(source_call == f'{native_name}(({type_name} *)&{handle});', 'destructor spelling changed')
        source_calls[source_call] = source_calls.get(source_call, 0) + 1
    require(all(normalized.count(call) == count for call, count in source_calls.items()), 'destructor source changed')
    result = pattern.sub(lambda m: f'{m["indent"]}{handle} = RESOURCE_NewResource();\n'
                         f'{m["indent"]}{old} = {handle};', normalized)
    for source_call in source_calls:
        result = result.replace(source_call, f'RESOURCE_ReleaseResource({handle});')
    if 'resourceDestructorSites' in row:
        destructors = [c for c in fn['calls'] if int(c['target'], 16) == 0x7e74d0 and c.get('ecxStack') == slot]
        require([int(c['site'], 16) for c in destructors] == row['resourceDestructorSites'], 'resource destructor inventory changed')
        encoded = row['helperBytes'].get('0x7e74d0')
        require(encoded is not None, 'missing resource destructor helper')
        helper = list(decoder.disasm(bytes.fromhex(encoded), 0x7e74d0))
        require(helper[-1].mnemonic == 'jmp' and helper[-1].operands[0].type == X86_OP_IMM
                and helper[-1].operands[0].imm == 0x99a430, 'resource destructor base changed')
        fields = [(i.operands[0].mem.disp, i.operands[1].imm) for i in helper
                  if i.mnemonic == 'mov' and i.operands[0].type == X86_OP_MEM
                  and decoder.reg_name(i.operands[0].mem.base) == 'esi'
                  and i.operands[1].type == X86_OP_IMM]
        require(fields == [(8, 0), (12, 0), (0, 0x126008c)], 'resource destructor layout changed')
        rewrites = row['resourceDestructorRewrites']
        require(len(rewrites) == 1, 'ambiguous resource destructor rewrite')
        part = rewrites[0]
        receiver(part, 0x7e74d0)
        require(int(part['call'], 16) in row['resourceDestructorSites'], 'unpaired resource destructor rewrite')
        # (address-order label pairing printed this resource dtor as DestroyMovie on the slot; callOrder pairing
        # prints ReleaseResource on the constructor's alias -- the native ECX is the slot either way)
        # (the alias spelling can recur at other releases: the row then names which printed occurrence is this site)
        occurrence, count = part.get('sourceOccurrence', 0), part.get('sourceCallCount', 1)
        require(part['sourceCall'] in (f'RESOURCE_DestroyMovie({handle});', f'RESOURCE_ReleaseResource({old});')
                and result.count(part['sourceCall']) == count and 0 <= occurrence < count, 'resource destructor source changed')
        at = -1
        for _ in range(occurrence + 1):
            at = result.index(part['sourceCall'], at + 1)
        result = result[:at] + f'RESOURCE_ReleaseResource({handle});' + result[at + len(part['sourceCall']):]
    # An actor map entry takes the resource object by address (Roth F21CBD pushes ESP-relative -0x144 into the
    # map setter); once the object is the recovered handle, its address is that handle.
    for part in row.get('actorMapResourceSites', []):
        native = calls.get(int(part['call'], 16))
        require(native is not None and int(native['target'], 16) == ACTOR_MAP_SET
                and (native.get('pushedStack') or [None])[0] == slot, 'actor map resource operand changed')
        require(re.fullmatch(r'ACTORMAP_Set\(\w+, "\w+", &' + re.escape(handle) + r'\);', part['sourceCall'])
                and result.count(part['sourceCall']) == 1, 'actor map source changed')
        result = result.replace(part['sourceCall'], part['sourceCall'].replace('&' + handle, handle))
    return result


ACTOR_MAP_SET = 0xCD3D2E
