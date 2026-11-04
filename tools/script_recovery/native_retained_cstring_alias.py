"""Fail-closed proof that a retained source CString is the pending literal object."""
import hashlib,json
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,x86 as x
from tools.script_recovery.native_receiver_reaching import _cfg
CTOR=0x99ebf0
CTOR_SHA='8c27b64a613d8319d3e866b8224ecfcedf8e5ccaf92a81314c16631d41f3f100'
def prove(fn,ctor,site,ctor_args,image):
    try:
        start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
        raw=image.bytes_at(start,end-start);md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
        ins=list(md.disasm(raw,start));by={i.address:k for k,i in enumerate(ins)}
        assert ins and sum(i.size for i in ins)==len(raw)
        c,a=int(ctor['site'],16),int(site['site'],16);assert c<a and c in by and a in by
        call=ins[by[c]];consumer=ins[by[a]]
        assert call.mnemonic=='call' and call.operands[0].type==x.X86_OP_IMM and call.operands[0].imm==CTOR and int(ctor['target'],16)==CTOR
        assert hashlib.sha256(image.bytes_at(CTOR,61)).hexdigest()==CTOR_SHA
        assert consumer.mnemonic=='call' and consumer.operands[0].type==x.X86_OP_MEM and consumer.operands[0].mem.disp==0x5b8 and not consumer.operands[0].mem.index
        assert site.get('kind')=='vtable' and int(site['slot'],16)==0x5b8
        before=ins[by[c]-3:by[c]];assert len(before)==3
        p0,p1,lea=before
        assert p0.mnemonic==p1.mnemonic=='push' and p0.operands[0].type==p1.operands[0].type==x.X86_OP_IMM and p0.operands[0].imm==-1
        assert lea.mnemonic=='lea' and lea.operands[0].type==x.X86_OP_REG and lea.operands[0].reg==x.X86_REG_ECX and lea.operands[1].type==x.X86_OP_MEM
        memory=lea.operands[1].mem;assert memory.base==x.X86_REG_ESP and not memory.index and not memory.segment
        assert isinstance(ctor['depth'],int) and isinstance(site['depth'],int) and ctor['depth']>8
        offset=ctor['ecxStack'];assert isinstance(offset,int) and offset<0 and -ctor['depth']+memory.disp==offset
        assert len(ctor_args)==3 and ctor_args[2].strip()=='-1'
        literal=image.string_at(p1.operands[0].imm);assert literal is not None and json.loads(ctor_args[1])==literal
        setup=ins[by[c]+1:by[a]];assert setup
        assert all(i.mnemonic in ['mov','lea','push'] for i in setup)
        for i in setup:
            if i.mnemonic in ['mov','lea']:assert i.operands[0].type==x.X86_OP_REG and i.operands[0].reg!=x.X86_REG_ESP
        pushes=sum(i.mnemonic=='push' for i in setup);assert pushes==5
        base_depth=ctor['depth']-8;assert site['depth']==base_depth+4*pushes
        # Outgoing argument writes must not overlap the four-byte live object.
        low,high=-site['depth'],-base_depth;assert offset+4<=low or offset>=high
        preds=_cfg(ins,x,__import__('capstone'),image);assert preds is not None
        for k in range(by[c]+1,by[a]+1):assert preds[k]==[k-1]
        return dict(constructor=hex(c),consumer=hex(a),stackOffset=offset,ctorDepth=ctor['depth'],consumerDepth=site['depth'],literal=literal,
            noInterveningCallsOrWrites=True,singleNormalEntry=True,objectBytes=4)
    except (AssertionError,KeyError,TypeError,ValueError,AttributeError):return None
