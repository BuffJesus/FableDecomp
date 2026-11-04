"""Recover nonescaping parent-state address aliases used by integer OR stores."""
from collections import Counter
import hashlib,re
from capstone import Cs,CS_ARCH_X86,CS_MODE_32

PAIR=re.compile(r'(?m)^(?P<indent>[ \t]*)(?P<alias>\w+) = \(uint \*\)\(\*\(int \*\)\((?:\(int\))?this \+ 0x14\) \+ (?P<offset>0x[0-9a-fA-F]+)\);\r?\n[ \t]*\*(?P=alias) = \*(?P=alias) \| (?P<mask>0x[0-9a-fA-F]+|\d+);')

def recover(fn, source, rdata, fields):
    matches=list(PAIR.finditer(source))
    if not matches:return source,[]
    start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
    data=rdata.bytes_at(start,end-start);cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True
    ins=list(cs.disasm(data,start))
    receiver=[i for i,x in enumerate(ins) if x.mnemonic=='mov' and x.op_str=='esi, ecx']
    if len(receiver)!=1:return source,[]
    for i,x in enumerate(ins):
        if 'esi' in [x.reg_name(r) for r in x.regs_access()[1]]:
            if i!=receiver[0] and not (x.mnemonic=='pop' and x.op_str=='esi'):return source,[]
    sites={}
    for i,x in enumerate(ins):
        m=re.fullmatch(r'dword ptr \[(\w+) \+ (0x[0-9a-f]+)\], (0x[0-9a-f]+|\d+)',x.op_str)
        if x.mnemonic!='or' or not m or i<=receiver[0]:continue
        prev=ins[i-1]
        if prev.mnemonic!='mov' or prev.op_str!=m[1]+', dword ptr [esi + 0x14]':continue
        key=(int(m[2],0),int(m[3],0));sites.setdefault(key,[]).append(dict(site=hex(x.address),load_site=hex(prev.address),bytes=(prev.bytes+x.bytes).hex()))
    edits=[];evidence=[]
    for alias in sorted({m['alias'] for m in matches}):
        group=[m for m in matches if m['alias']==alias]
        residue=source
        for m in reversed(group):residue=residue[:m.start()]+residue[m.end():]
        residue=re.sub(r'(?m)^\s*uint \*'+re.escape(alias)+r';\s*$','',residue)
        if re.search(r'\b'+re.escape(alias)+r'\b',residue):continue
        keys=[(int(m['offset'],0),int(m['mask'],0)) for m in group]
        if any(n!=1 or len(sites.get(k,[]))!=1 for k,n in Counter(keys).items()):continue
        if any(fields.get(hex(k[0]),('',None))[1]!='Int' or not 0<=k[1]<=0xffffffff for k in keys):continue
        for m,key in zip(group,keys):
            address='*(uint *)(*(int *)(this + 0x14) + '+hex(key[0])+')'
            replacement=m['indent']+address+' = '+address+' | '+str(key[1])+';'
            edits.append((m.start(),m.end(),replacement))
            evidence.append(dict(alias=alias,field=fields[hex(key[0])][0],offset=hex(key[0]),mask=key[1],native=sites[key][0],body_sha256=hashlib.sha256(data).hexdigest(),basis='Non-escaping address alias; unique matching retail parent-load/OR store; ESI receiver preserved; parent field map'))
    for a,b,value in sorted(edits,reverse=True):source=source[:a]+value+source[b:]
    return source,evidence
