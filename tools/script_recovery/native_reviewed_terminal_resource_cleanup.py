"""Recover a reviewed native local-resource retirement at a terminal poll exit."""
import hashlib
import re
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from capstone.x86 import X86_OP_IMM,X86_OP_MEM,X86_OP_REG,X86_REG_ECX,X86_REG_ESP

def restore(source,fn,row,rd):
    if row is None:return source
    def require(ok,why):
        if not ok:raise ValueError(f'reviewed terminal resource cleanup {fn["address"]}: {why}')
    start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
    raw=rd.bytes_at(start,end-start)
    require(raw is not None and hashlib.sha256(raw).hexdigest()==row['nativeSha256'],'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest()==row['sourceSha256'],'exported source changed')
    require(hashlib.sha256(source.encode()).hexdigest()==row['luaSha256'],'lifted source phase changed')
    cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True
    instructions=list(cs.disasm(raw,start))
    require(instructions and instructions[-1].address+instructions[-1].size==end,'native disassembly incomplete')
    indexed={i.address:i for i in instructions}
    for address,encoded in row['nativeInstructions'].items():
        i=indexed.get(int(address,16));require(i is not None and i.bytes.hex()==encoded,f'native instruction changed at {address}')
    calls={int(c['site'],16):int(c['target'],16) for c in fn['calls']}
    for site,target in row['nativeCalls'].items():
        require(calls.get(int(site,16))==int(target,16),f'native call changed at {site}')
    require(sorted(int(t,16) for t in row['nativeCalls'].values())==[0x99a380,0x99a430,0xf35b30],
            'native lifecycle targets changed')
    branch=indexed.get(int(row['terminationBranch'],16))
    require(branch is not None and branch.mnemonic=='jne' and branch.operands[0].type==X86_OP_IMM
            and branch.operands[0].imm==int(row['cleanupEntry'],16),'termination edge changed')
    require(int(row['cleanupEntry'],16)==int(row['inlineCleanupStart'],16),'cleanup entry mismatch')
    for key in ['constructorReceiver','cleanupReceiver']:
        i=indexed.get(int(row[key],16))
        require(i is not None and i.mnemonic=='lea' and i.operands[0].type==X86_OP_REG and i.operands[0].reg==X86_REG_ECX
                and i.operands[1].type==X86_OP_MEM and i.operands[1].mem.base==X86_REG_ESP
                and not i.operands[1].mem.index and i.operands[1].mem.disp==row['balancedStackSlot'],'local resource receiver changed')
    for witness in row['equivalentDestructorBodies']:
        body=rd.bytes_at(int(witness['address'],16),witness['size'])
        require(body is not None and hashlib.sha256(body).hexdigest()==witness['sha256'],'equivalent destructor changed')
    global_bodies=[w for w in row['equivalentDestructorBodies'] if int(w['address'],16)!=int(row['inlineCleanupStart'],16)]
    require(global_bodies==[
        {'address':'0x7e74d0','size':66,'sha256':'63d35277586994402fc860945a37ce38ac2c40da11870d375fe4034f4c2df9ba'},
        {'address':'0x99a430','size':56,'sha256':'1d75d831d3f6e008489a4847244a3e123b663f8f7de361ca3f15fc938a470796'}],
        'retail destructor equivalence certificate changed')
    first=indexed.get(int(row['inlineCleanupStart'],16))
    require(first is not None and first.mnemonic=='mov' and first.operands[1].type==X86_OP_MEM
            and first.operands[1].mem.base==X86_REG_ESP and first.operands[1].mem.disp==row['balancedStackSlot']+12,
            'inline cleanup imp slot changed')
    alias,handle,label,flag=(row[k] for k in ['alias','constructorHandle','pollLabel','terminationFlag'])
    require(all(re.fullmatch(r'[A-Za-z_]\w*',n) for n in [alias,handle,label,flag]),'invalid local name')
    constructor=f'{handle} = resources:NewResource()\n        local {alias} = {handle}'
    require(source.count(constructor)==1,'constructed local alias changed')
    stores=re.findall(r'^\s*(?:local )?'+re.escape(alias)+r'\s*=',source,re.M)
    require(len(stores)==1,'resource alias reassigned')
    old=f'::{label}::\n        while true do\n            if {flag} then\n                return\n            end'
    require(source.count(old)==1 and source.index(constructor)<source.index(old),'terminal poll shape changed')
    new=old.replace('                return',f'                resources:ReleaseResource({alias})\n                return')
    source=source.replace(old,new,1)
    if 'splitCleanup' not in row:return source
    split=row['splitCleanup']
    require(split['pairCount']==2,'split cleanup source inventory changed')
    require(sorted(int(t,16) for t in split['nativeCalls'].values())==[0x6e7ab0,0x7e74d0,0x99a430],
            'split cleanup lifecycle targets changed')
    for site,target in split['nativeCalls'].items():
        require(calls.get(int(site,16))==int(target,16),f'split cleanup call changed at {site}')
    for address,encoded in split['nativeInstructions'].items():
        i=indexed.get(int(address,16));require(i is not None and i.bytes.hex()==encoded,f'split cleanup instruction changed at {address}')
    for witness in split['bodies']:
        body=rd.bytes_at(int(witness['address'],16),witness['size'])
        require(body is not None and hashlib.sha256(body).hexdigest()==witness['sha256'],'split cleanup destructor changed')
    require([int(w['address'],16) for w in split['bodies']]==[int(split['entry'],16),0x6e7ab0,0x6e7b80],
            'split cleanup destructor certificate changed')
    movie=split['movieAlias']
    require(re.fullmatch(r'[A-Za-z_]\w*',movie) is not None,'invalid movie handle')
    require(source.count(f'{movie} = resources:StartMovie("")')==1,'constructed movie handle changed')
    pattern=re.compile(r'(?P<indent>^[ \t]*)resources:DestroyMovie\('+re.escape(movie)+r'\)\n(?P=indent)resources:DestroyMovie\('+re.escape(movie)+r'\)',re.M)
    require(len(list(pattern.finditer(source)))==split['pairCount'],'split movie/resource cleanup shape changed')
    return pattern.sub(lambda m:m['indent']+f'resources:DestroyMovie({movie})\n'+m['indent']+f'resources:ReleaseResource({alias})',source)
