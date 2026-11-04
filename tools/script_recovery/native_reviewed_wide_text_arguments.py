"""Recover reviewed wide integer arguments and their explicit global-text lifetime."""
import hashlib
import re
from capstone import Cs, CS_ARCH_X86, CS_MODE_32, x86


def restore(source, todo, fn, unit, rd):
    row = unit.get('reviewedWideTextArguments', {}).get(str(fn['address']).lower())
    if row is None:
        return source, todo

    def require(ok, why):
        if not ok:
            raise ValueError('Reviewed wide text: ' + why)

    source = source.replace('\r\n', '\n').replace('\r', '\n')
    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    raw = rd.bytes_at(start, end-start)
    for key, value in [('nativeSha256', raw), ('sourceSha256', fn['decompile'].encode()),
                       ('luaSha256', source.encode())]:
        require(value is not None and hashlib.sha256(value).hexdigest() == row[key], key+' changed')
    # Pinned dependency bodies include formatting, ownership and the virtual API.
    required = {0x99aed0, 0x99ba70, 0x99b720, 0x99b510, 0x99b7d0, 0x99b800,
                0x4128a0, 0x412130, 0x412330, 0x891340}
    require(len(row['dependencies']) == len(required)
            and {d['start'] for d in row['dependencies']} == required, 'dependency inventory changed')
    for d in row['dependencies']:
        value = rd.bytes_at(d['start'], d['size'])
        require(value is not None and hashlib.sha256(value).hexdigest() == d['sha256'],
                'native dependency changed')
    require({literal['address'] for literal in row['literals']} == {0x122de44,0x122d70c,0x12e78d4}
            and len(row['literals'])==3, 'literal inventory changed')
    for literal in row['literals']:
        require(rd.bytes_at(literal['address'], len(bytes.fromhex(literal['bytes']))) ==
                bytes.fromhex(literal['bytes']), 'literal changed')
    require(rd.bytes_at(row['vtable']+row['slot'],4) == (0x891340).to_bytes(4,'little'),
            'formatted API target changed')
    require(row['slot'] == 0xb2c, 'formatted API slot changed')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32); decoder.detail = True
    ins = list(decoder.disasm(raw,start)); indexed = {i.address:i for i in ins}
    require(ins and ins[-1].address+ins[-1].size == end, 'incomplete body')
    require(set(row['windows']) == {'format','clear','normalClose','abortClose'}, 'window inventory changed')
    direct_calls={}
    for w in row['windows'].values():
        seq = [i for i in ins if w['start'] <= i.address < w['end']]
        require(seq and seq[0].address == w['start'] and seq[-1].address+seq[-1].size == w['end']
                and b''.join(i.bytes for i in seq).hex() == w['bytes'], 'native window changed')
        direct_calls.update({hex(i.address):i.operands[0].imm for i in seq
                             if i.mnemonic=='call' and i.operands[0].type==x86.X86_OP_IMM})
    require(row['calls']==direct_calls, 'call inventory changed')
    # Distinguish the lost wide operations from similarly named narrow helpers.
    for address,target in row['calls'].items():
        i=indexed.get(int(address,16))
        require(i and i.mnemonic == 'call' and len(i.operands)==1
                and i.operands[0].type == x86.X86_OP_IMM and i.operands[0].imm == target,
                'wide call target changed')
    score=indexed[row['scorePush']]
    require(score.mnemonic=='push' and score.operands[0].type==x86.X86_OP_REG
            and score.operands[0].reg==x86.X86_REG_EBX, 'integer score operand changed')
    counter = unit.get('reviewedMasterIntStores',{}).get(str(fn['address']).lower(),{})
    require(counter.get('valueRegister')=='ebx' and counter.get('valueLocal')==row['valueLocal']
            and counter.get('nativeSha256')==row['nativeSha256'], 'score provenance changed')
    require(row['globalAddress']==0x143e908, 'unreviewed global target')
    require(row['key']=='TEXT_QST_B17_SCORE', 'text key changed')
    require(row['frameBytes']==0x3b4 and row['vectorSlot']==-0x360, 'vector identity changed')
    require(row['vectorReceivers']==[row['windows']['normalClose']['start'],row['windows']['abortClose']['start']],
            'cleanup receiver inventory changed')
    for address in row['vectorReceivers']:
        i=indexed[address]
        require(i.mnemonic=='lea' and i.operands[0].type==x86.X86_OP_REG
                and i.operands[0].reg==x86.X86_REG_ECX and i.operands[1].type==x86.X86_OP_MEM
                and i.operands[1].mem.base==x86.X86_REG_ESP and not i.operands[1].mem.index
                and i.operands[1].mem.disp==row['frameBytes']+row['vectorSlot'],
                'vector cleanup receiver changed')
    alias, value = row['alias'], row['valueLocal']
    require(all(re.fullmatch(r'[A-Za-z_]\w*',s) for s in (alias,value)), 'invalid local')
    require(not re.search(r'\b'+re.escape(alias)+r'\b',source), 'alias collision')
    require(re.search(r'(?m)^    local [^\n]*\b'+re.escape(value)+r'\b',source), 'score is not local')
    old = row['formatLines']
    require(len(old)==6, 'format inventory changed')
    pattern=re.compile(r'(?m)^(?P<indent>[ \t]*)'+
                       r'\n(?P=indent)'.join(re.escape(s) for s in old)+r'$')
    require(len(pattern.findall(source))==1, 'format block changed')
    expression=f'(({value} + 2147483648) % 4294967296 - 2147483648)'
    replacement=[f'{alias} = resources:NewWideTextArguments()',
                 f'{alias}:AppendInteger({expression})',
                 f'{alias}:FormatGlobalText(0x{row["globalAddress"]:x}, "{row["key"]}")']
    source=pattern.sub(lambda m:('\n'+m['indent']).join([m['indent']+replacement[0]]+replacement[1:]),source)
    cleanup=row['cleanupLine']
    require(source.count(cleanup)==2, 'cleanup inventory changed')
    # The normal macro return clears the global before unpause/movie destruction.
    # Abort joins its own vector destructor and deliberately retains the global.
    normal=row['normalLines']
    require(normal[-1]==cleanup and len(normal)==4, 'normal cleanup shape changed')
    pattern=re.compile(r'(?m)^(?P<indent>[ \t]*)'+
                       r'\n(?P=indent)'.join(re.escape(s) for s in normal)+r'$')
    require(len(pattern.findall(source))==1, 'normal cleanup block changed')
    normal=normal[:1]+[f'{alias}:ClearGlobalText(0x{row["globalAddress"]:x})']+normal[1:]
    source=pattern.sub(lambda m:('\n'+m['indent']).join([m['indent']+normal[0]]+normal[1:]),source)
    source=source.replace(cleanup, f'{alias}:Close()')
    # Declare at entry so existing native goto structure never jumps into scope.
    anchor='    local resources = quest:RetailResources()\n'
    require(source.count(anchor)==1, 'resource scope changed')
    source=source.replace(anchor,anchor+f'    local {alias}\n')
    remaining=list(todo)
    for diagnostic,count in row['diagnostics'].items():
        require(remaining.count(diagnostic)==count, 'diagnostic inventory changed')
        remaining=[t for t in remaining if t!=diagnostic]
    return source,remaining
