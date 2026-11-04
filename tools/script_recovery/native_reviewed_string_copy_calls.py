"""Recover an adjacent outgoing CCharString copy and parent helper call."""
import hashlib
import re

from capstone import Cs, CS_ARCH_X86, CS_MODE_32, x86


def restore(source, todo, fn, unit, rd):
    row = unit.get('reviewedStringCopyCalls', {}).get(str(fn['address']).lower())
    if row is None:
        return source, todo

    def require(ok, why):
        if not ok:
            raise ValueError('Reviewed string copy call: ' + why)

    source = source.replace('\r\n', '\n').replace('\r', '\n')
    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    raw = rd.bytes_at(start, end-start)
    for key, value in (('nativeSha256', raw), ('sourceSha256', fn['decompile'].encode()),
                       ('luaSha256', source.encode())):
        require(value is not None and hashlib.sha256(value).hexdigest() == row[key], key+' changed')
    required = {row['callee'],row['copyConstructor'],row['stringDestructor'],row['stringRelease']}
    require(len(row['dependencies'])==len(required)
            and {d['start'] for d in row['dependencies']}==required, 'missing native dependency')
    for dependency in row['dependencies']:
        value = rd.bytes_at(dependency['start'], dependency['size'])
        require(value is not None and hashlib.sha256(value).hexdigest() == dependency['sha256'],
                'native string/callee dependency changed')
    helper = row['helper']
    entry = unit.get('quest', {}).get('functions', {}).get(helper)
    require(entry and int(entry['address'], 16) == row['callee'], 'registered parent helper changed')
    require(row['module'] == unit['script']+'.native_quest_helpers', 'helper module changed')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32); decoder.detail = True
    constructor = next(d for d in row['dependencies'] if d['start']==row['copyConstructor'])
    copy_ins = list(decoder.disasm(rd.bytes_at(constructor['start'],constructor['size']),constructor['start']))
    require(copy_ins and copy_ins[-1].mnemonic=='ret' and len(copy_ins[-1].operands)==1
            and copy_ins[-1].operands[0].type==x86.X86_OP_IMM and copy_ins[-1].operands[0].imm==4,
            'copy constructor stack purge changed')
    ins = list(decoder.disasm(raw, start))
    require(ins and ins[-1].address+ins[-1].size == end, 'incomplete caller body')

    def reg(op, expected):
        return op.type == x86.X86_OP_REG and op.reg == expected

    def mem(op, base, displacement):
        return (op.type == x86.X86_OP_MEM and op.size == 4 and op.mem.base == base
                and not op.mem.index and op.mem.disp == displacement)

    # Fixed caller frame determines the incoming-frame source slot; the PUSH
    # reserves precisely the copied four-byte outgoing CCharString.
    require(ins[0].mnemonic == 'sub' and reg(ins[0].operands[0], x86.X86_REG_ESP)
            and ins[0].operands[1].type == x86.X86_OP_IMM, 'unsupported frame')
    frame = ins[0].operands[1].imm
    for i in ins[1:]:
        if i.mnemonic == 'call': break
        if i.mnemonic == 'push':
            require(i.operands[0].type == x86.X86_OP_REG, 'ambiguous frame push')
            frame += 4
        else:
            require(i.mnemonic in ('mov','lea','xor') and x86.X86_REG_ESP not in i.regs_access()[1],
                    'ambiguous frame setup')
    require(frame == row['frameBytes'], 'frame extent changed')
    sites = set()
    for window in row['calls']:
        seq = [i for i in ins if window['start'] <= i.address < window['end']]
        require(len(seq) == 7 and seq[0].address == window['start']
                and seq[-1].address+seq[-1].size == window['end'], 'call window changed')
        require(''.join(i.bytes.hex() for i in seq) == window['bytes'], 'call instructions changed')
        reserve, address, dest, push, copy, receiver, call = seq
        require([i.mnemonic for i in seq] == ['push','lea','mov','push','call','mov','call'],
                'unsupported outgoing copy sequence')
        require(reserve.operands[0].type == x86.X86_OP_REG, 'outgoing allocation changed')
        require(address.operands[0].type == x86.X86_OP_REG
                and mem(address.operands[1],x86.X86_REG_ESP,frame+row['entrySlot']+4),
                'source stack identity changed')
        require(reg(dest.operands[0],x86.X86_REG_ECX) and reg(dest.operands[1],x86.X86_REG_ESP),
                'copy destination is not outgoing stack')
        require(reg(push.operands[0],address.operands[0].reg), 'copy source changed')
        require(copy.operands[0].type == x86.X86_OP_IMM and copy.operands[0].imm == row['copyConstructor'],
                'copy constructor changed')
        require(reg(receiver.operands[0],x86.X86_REG_ECX)
                and mem(receiver.operands[1],getattr(x86,'X86_REG_'+row['ownerRegister'].upper()),row['parentOffset']),
                'parent receiver changed')
        require(call.operands[0].type == x86.X86_OP_IMM and call.operands[0].imm == row['callee'],
                'callee changed')
        require(call.address not in sites, 'duplicate call site')
        sites.add(call.address)
    # Reviewed helper exit destroys the incoming argument and consumes it.
    tail = list(decoder.disasm(rd.bytes_at(row['cleanupStart'],row['cleanupEnd']-row['cleanupStart']),row['cleanupStart']))
    require(len(tail)==8 and [i.mnemonic for i in tail]==['lea','call','pop','pop','pop','pop','add','ret'],
            'helper argument cleanup changed')
    require(reg(tail[0].operands[0],x86.X86_REG_ECX)
            and mem(tail[0].operands[1],x86.X86_REG_ESP,row['calleeFrameBytes']+4), 'argument cleanup slot changed')
    require(tail[1].operands[0].type==x86.X86_OP_IMM and tail[1].operands[0].imm==row['stringDestructor'],
            'argument destructor changed')
    require(tail[-1].operands[0].type==x86.X86_OP_IMM and tail[-1].operands[0].imm==4, 'callee purge changed')
    value, temporary, label = row['valueLocal'], row['copyLocal'], row['calleeLabel']
    require(all(re.fullmatch(r'[A-Za-z_]\w*', s) for s in (value,temporary,label,helper)), 'invalid source identity')
    require(value == f'xStack_{-row["entrySlot"]:x}', 'Lua source slot changed')
    require(re.search(r'\blocal\s+[^\n]*\b'+re.escape(value)+r'\b',source), 'source is not local')
    old_copy = f'CCharString::CCharString(&{temporary},&{value});'
    old_call = f'{label}(*(undefined4 *)(this + {row["parentOffset"]:#x}));'
    pattern = re.compile(r'(?m)^(?P<indent>[ \t]*)-- TODO\(native\): '+re.escape(old_copy)
                         +r'\n(?P=indent)-- TODO\(native\): '+re.escape(old_call)+r'$')
    require(len(pattern.findall(source)) == len(sites) and sites, 'adjacent copy/call pairs changed')
    diagnostic = label+': unproven parent helper receiver or arity'
    require(todo.count('unlifted: '+old_copy)==len(sites) and todo.count(diagnostic)==len(sites),
            'copy/call diagnostics changed')
    # Immediate Lua argument evaluation snapshots the immutable string value.
    # No source assignment is crossed, and the existing shared helper retains
    # responsibility for its resources and yields.
    text = f'require("{row["module"]}").{helper}(quest, me, {value})'
    source = pattern.sub(lambda m:m['indent']+text,source)
    return source,[t for t in todo if t not in ('unlifted: '+old_copy,diagnostic)]
