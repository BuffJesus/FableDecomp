"""Recover reviewed resource speech operands by replaying their native witnesses.

Native and lowered hashes gate the complete function. Lexical call pairing,
CFG receiver definitions, pushed constants and listener origins must reproduce
the reviewed inventory exactly before any source call is replaced.
"""
import hashlib
import json
import re
import capstone
from capstone import x86
from tools.script_recovery.native_receiver_reaching import _cfg, _reaching

def _recover(source, fn, rdata):
    from tools.script_recovery.convert_quest_unit import _text_order_sites, _split_top
    original = source
    start = int(fn['address'], 16)
    raw = rdata.bytes_at(start, int(fn['bodyEndExclusive'], 16) - start)
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    ins = list(decoder.disasm(raw, start))
    if not ins or ins[-1].address + ins[-1].size != int(fn['bodyEndExclusive'], 16):
        raise ValueError('incomplete resource speech disassembly')
    indexed = {i.address: n for n, i in enumerate(ins)}
    preds = _cfg(ins, x86, capstone, rdata)
    if not preds is not None:
        raise ValueError('native resource speech witness mismatch')
    ordered = _text_order_sites(fn['decompile'], fn)
    if not ordered is not None:
        raise ValueError('native resource speech witness mismatch')
    methods = {0x7e7390: 'Speak', 0x7e7450: 'IsPerformingScriptTask', 0x7e7360: 'ClearCommands'}
    native = {m: [] for m in methods.values()}
    for _, _, _, call, _, _ in ordered:
        if int(call.get('target', '0'), 16) in methods:
            native[methods[int(call['target'], 16)]].append(call)
    pattern = re.compile('(?:CScriptGameResourceObjectScriptedThingBase|CScriptThing)::_(Speak|IsPerformingScriptTask|ClearCommands)_CScriptGameResourceObjectScriptedThingBase\\w*\\(')
    counts = {m: 0 for m in methods.values()}
    changes = []
    rows = []
    witnessed = set()
    for match in pattern.finditer(source):
        method = match[1]
        call = native[method][counts[method]]
        counts[method] += 1
        site = int(call['site'], 16)
        k = indexed[site]
        instruction = ins[k]
        if (instruction.mnemonic != 'call' or instruction.operands[0].type != x86.X86_OP_IMM
                or methods.get(instruction.operands[0].imm) != method):
            raise ValueError('resource speech native call pairing changed')
        receiver_defs = _reaching(ins, preds, k, x86.X86_REG_ECX, x86)
        if not (len(receiver_defs) == 1 and -1 not in receiver_defs):
            raise ValueError((method, hex(site), receiver_defs))
        lea = ins[next(iter(receiver_defs))]
        if not (lea.mnemonic == 'lea' and lea.operands[0].reg == x86.X86_REG_ECX):
            raise ValueError((method, hex(site), lea.op_str))
        slot = call['ecxStack']
        depth = call['depth']
        if not (lea.operands[1].mem.base == x86.X86_REG_ESP and lea.operands[1].mem.disp - depth == slot):
            raise ValueError('native resource speech witness mismatch')
        j = match.end()
        nest = 1
        while nest:
            if source[j] == '(':
                nest += 1
            elif source[j] == ')':
                nest -= 1
            j += 1
        args = _split_top(source[match.end():j - 1])
        handle = 'xStack_' + format(-slot, 'x')
        if not re.search('\\b' + handle + '\\b', args[0]):
            raise ValueError((method, site, args))
        row = dict(method=method, site=site, receiver=slot, depth=depth, lea=lea.address, sourceArguments=args)
        witnessed.update((site, lea.address))
        if method == 'Speak':
            pushes = [i for i in reversed(ins[max(0, k - 35):k]) if i.mnemonic == 'push'][:6]
            if not len(pushes) == 6:
                raise ValueError('native resource speech witness mismatch')
            row['pushes'] = [i.address for i in pushes]
            witnessed.update(row['pushes'])
            expected = [None, None, 0, 0, 1, 0]
            for push, value in zip(pushes[2:], expected[2:]):
                operand = push.operands[0]
                if operand.type == x86.X86_OP_IMM:
                    if not operand.imm == value:
                        raise ValueError('native resource speech witness mismatch')
                else:
                    if not (value == 0 and operand.type == x86.X86_OP_REG and (operand.reg == x86.X86_REG_EDI)):
                        raise ValueError('native resource speech witness mismatch')
                    defs = _reaching(ins, preds, indexed[push.address], x86.X86_REG_EDI, x86)
                    if not (defs and -1 not in defs):
                        raise ValueError('native resource speech witness mismatch')
                    if not all((ins[n].mnemonic == 'xor' and ins[n].op_str == 'edi, edi' for n in defs)):
                        raise ValueError('native resource speech witness mismatch')
                    witnessed.update((ins[n].address for n in defs))
                    row.setdefault('zeroDefinitions', {})[hex(push.address)] = [ins[n].address for n in sorted(defs)]
            key_operand = pushes[1].operands[0]
            if not key_operand.type == x86.X86_OP_IMM:
                raise ValueError('native resource speech witness mismatch')
            key = rdata.string_at(key_operand.imm)
            if not (key and key.startswith('TEXT_QST_005_')):
                raise ValueError('native resource speech witness mismatch')
            row['keyAddress'] = key_operand.imm
            row['key'] = key
            if args[2].startswith('"'):
                if not json.loads(args[2]) == key:
                    raise ValueError('native resource speech witness mismatch')
            row['listener'] = args[1]
            p = indexed[pushes[0].address]
            origin = ins[p - 1]
            witnessed.add(origin.address)
            row['listenerOrigin'] = origin.address
            if origin.mnemonic == 'call':
                if pushes[0].operands[0].type != x86.X86_OP_REG or pushes[0].operands[0].reg != x86.X86_REG_EAX:
                    raise ValueError('hero speech listener is not the return register')
                if not (origin.operands[0].type == x86.X86_OP_MEM and origin.operands[0].mem.disp == 280):
                    raise ValueError('native resource speech witness mismatch')
                # Ghidra names the hero temporary per function; the source variable
                # must be last assigned from GetHero before this call.
                hero = re.fullmatch(r'(?:\(CScriptThing \*\))?(\w+)', args[1].strip())
                if not hero:
                    raise ValueError('native resource speech witness mismatch')
                assigns = re.findall(r'\b' + hero[1] + r'\s*=(?!=)\s*([^;]*);', source[:match.start()])
                if not (assigns and re.fullmatch(r'(?:\([\w ]+\*+\))?GSI->GetHero\(\)', assigns[-1].strip())):
                    raise ValueError('native resource speech witness mismatch')
                row['listenerKind'] = 'hero'
            else:
                if not (origin.mnemonic == 'lea' and origin.operands[0].reg == pushes[0].operands[0].reg):
                    raise ValueError('native resource speech witness mismatch')
                address = origin.operands[1].mem
                if address.base == x86.X86_REG_ESI:
                    if not (address.disp == 8 and '(this + 8)' in args[1]):
                        raise ValueError('native resource speech witness mismatch')
                    row['listenerKind'] = 'self'
                else:
                    if not address.base == x86.X86_REG_ESP:
                        raise ValueError('native resource speech witness mismatch')
                    listener_slot = address.disp - (depth - 4)
                    if not 'xStack_' + format(-listener_slot, 'x') in args[1]:
                        raise ValueError('native resource speech witness mismatch')
                    row['listenerKind'] = 'stack'
                    row['listenerSlot'] = listener_slot
            replacement = f'RESOURCE_Speak({handle}, {args[1]}, {json.dumps(key)}, 0, false, true, false)'
        else:
            replacement = f'RESOURCE_{method}({handle})'
        changes.append((match.start(), j, replacement))
        rows.append(row)
    if not counts == {m: len(v) for m, v in native.items()}:
        raise ValueError('native resource speech witness mismatch')
    for a, b, text in reversed(changes):
        source = source[:a] + text + source[b:]
    helpers = {}
    for target in methods:
        data = rdata.bytes_at(target, 128)
        decoded = list(decoder.disasm(data, target))
        end = next((i.address + i.size for i in decoded if i.mnemonic == 'ret'))
        helpers[hex(target)] = rdata.bytes_at(target, end - target).hex()
    row = dict(nativeSha256=hashlib.sha256(raw).hexdigest(), sourceSha256=hashlib.sha256(fn['decompile'].encode()).hexdigest(), loweredSha256=hashlib.sha256(original.encode()).hexdigest(), calls=rows, nativeInstructions={hex(s): ins[indexed[s]].bytes.hex() for s in sorted(witnessed)}, helperBytes=helpers)
    return (source, row)

def restore(source, fn, row, rdata):
    if row is None:
        return source
    normalized = source.replace('\r\n', '\n').replace('\r', '\n')
    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    raw = rdata.bytes_at(start, end-start)
    checks = (('nativeSha256', raw), ('sourceSha256', fn['decompile'].encode()),
              ('loweredSha256', normalized.encode()))
    for key, data in checks:
        if data is None or hashlib.sha256(data).hexdigest() != row.get(key):
            raise ValueError('reviewed resource speech: ' + key + ' changed')
    try:
        recovered, observed = _recover(normalized, fn, rdata)
    except (KeyError, IndexError, TypeError, AssertionError) as exc:
        raise ValueError('reviewed resource speech: incomplete witness') from exc
    if observed != row:
        raise ValueError('reviewed resource speech: witness inventory changed')
    return recovered
