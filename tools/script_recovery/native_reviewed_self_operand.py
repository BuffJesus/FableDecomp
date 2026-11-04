"""Restore an evidenced entity-self operand without changing local lifetimes."""
import hashlib

def restore(text, fn, entries, rdata, row):
    if row is None:
        return text
    def require(ok, reason):
        if not ok:
            raise ValueError('reviewed self operand: ' + reason)
    start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
    raw=rdata.bytes_at(start,end-start)
    require(hashlib.sha256(raw).hexdigest()==row['nativeSha256'],'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest()==row['sourceSha256'],'source changed')
    from capstone import Cs,CS_ARCH_X86,CS_MODE_32
    decoder=Cs(CS_ARCH_X86,CS_MODE_32)
    instructions=list(decoder.disasm(raw,start))
    by_site={i.address:i for i in instructions}
    require(instructions[-1].address+instructions[-1].size==end,'incomplete disassembly')
    for site,encoded in row['anchors'].items():
        require(by_site[int(site,16)].bytes.hex()==encoded,'native anchor changed')
    # The full-body hash pins the proven stack save/reloads and every intervening
    # instruction. Reject any additional write to the saved handle register.
    operand=row['operand']
    writes=[i.address for i in instructions if i.mnemonic in ('lea','mov') and i.op_str.startswith(operand['register']+',')]
    require(writes==[int(a,16) for a in operand['definitions']],'saved handle definitions changed')
    matches=[e for e in entries if int(e[3]['site'],16)==int(operand['call'],16)]
    require(len(matches)==1,'call pairing missing or ambiguous')
    a,b,args,site,key,vtable=matches[0]
    require(vtable and int(key,16)==int(operand['slot'],16) and site.get('kind')=='vtable','paired call changed')
    require(args[:1]==[operand['receiver']],'printed receiver changed')
    for address,raw in operand['receiverInstructions'].items():
        require(by_site[int(address,16)].bytes.hex()==raw,'native receiver changed')
    # Source exports an explicit receiver before the three method operands.
    require(args[1:]==operand['arguments'],'printed operands changed')
    values=list(args);values[1+operand['index']]='(CScriptThing *)((int)this + 8)'
    return text[:a]+','.join(values)+text[b:]
