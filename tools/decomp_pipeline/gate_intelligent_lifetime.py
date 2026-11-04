"""Verify intelligent-pointer construction/destruction and the compiler deleting ABI."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
from gate_intelligent_pointer import FILE,SPECS as OPERATIONS,COUNTS,TARGETS as OP_TARGETS
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ROOT=Path(__file__).resolve().parents[2]
SPECS=[dict(address='00a01b10',name='Constructor',size=16,symbol='??0CBaseIntelligentPointer@@QAE@XZ'),dict(address='00a01c10',name='Destructor',size=52,symbol='??1CBaseIntelligentPointer@@UAE@XZ'),dict(address='00a01c50',name='ScalarDeletingDestructor',size=72,symbol='??_GCBaseIntelligentPointer@@UAEPAXI@Z')]
TARGETS=dict(OP_TARGETS,**{'??_7CBaseIntelligentPointer@@6B@':0x129c95c})
def execute(code,operation,pattern,state=0,refs=0,flags=0):
    pointer,block,stack,stop=0x2000040,0x2100040,0x3008000,0x4000000
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for base,size in [(0xa01000,0x1000),(0xbfe000,0x1000),(0x2000000,0x1000),(0x2100000,0x1000),(0x3000000,0x10000),(stop,0x1000)]:u.mem_map(base,size)
    entry=int(SPECS[operation]['address'],16);u.mem_write(entry,bytes(code));before=bytearray([pattern]*0x88)
    struct.pack_into('<II',before,0x40,0x98765432,block if state else 0);u.mem_write(pointer-0x40,bytes(before))
    control=bytearray([255-pattern]*0x88);struct.pack_into('<II',control,0x40,0x12345000 if state==2 else 0,refs);u.mem_write(block-0x40,bytes(control))
    u.mem_write(stack,struct.pack('<II',stop,flags));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,pointer)
    saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x54320000+i)
    trace=[]
    def hook(uc,pc,size,user):
        if pc!=0xbfe9bc:return
        sp=uc.reg_read(UC_X86_REG_ESP);trace.append(struct.unpack('<I',uc.mem_read(sp+4,4))[0])
        uc.reg_write(UC_X86_REG_EAX,0xaaaa5555);uc.reg_write(UC_X86_REG_ECX,0xcccccccc);uc.reg_write(UC_X86_REG_EDX,0xdddddddd)
        uc.reg_write(UC_X86_REG_EIP,struct.unpack('<I',uc.mem_read(sp,4))[0]);uc.reg_write(UC_X86_REG_ESP,sp+4)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(entry,stop,count=300)
    expected=[];struct.pack_into('<II',before,0x40,0x129c95c,0)
    if operation and state:
        struct.pack_into('<I',control,0x44,(refs-1)&0xffffffff)
        if state==1 and refs==1:expected.append(block)
    if operation==2 and flags&1:expected.append(pointer)
    assert trace==expected and bytes(u.mem_read(pointer-0x40,len(before)))==before and bytes(u.mem_read(block-0x40,len(control)))==control
    if operation!=1:assert u.reg_read(UC_X86_REG_EAX)==pointer
    assert u.reg_read(UC_X86_REG_ESP)==stack+(8 if operation==2 else 4) and all(u.reg_read(r)==0x54320000+i for i,r in enumerate(saved))
    return trace,bytes(before),bytes(control)
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/a0'/FILE)
    ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/IntelligentLifetime_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/intelligent-lifetime.json');a=ap.parse_args()
    env=check_one.env();work=Path(tempfile.mkdtemp(prefix='intelligent-lifetime-'));obj=work/'source.obj'
    r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(a.source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr
    c=Coff(obj);candidate=[];native=[]
    with pefile.PE(str(EXE)) as pe:
        for s in SPECS+OPERATIONS:
            sec,off=c.symbols[s['symbol']];built,rel=c.sections[sec];assert off==0 and len(built)==s['size'];address=int(s['address'],16);patched=bytearray(built)
            for site,(name,kind) in rel.items():
                assert kind in (6,20);struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<I',built,site)[0]-(address+site+4 if kind==20 else 0))&0xffffffff)
            retail=pe.get_data(address-0x400000,len(built));assert patched==retail,s
            if s in SPECS:candidate.append(patched);native.append(retail)
        assert struct.unpack('<I',pe.get_data(0x129c95c-0x400000,4))[0]==0xa01c50
    sec,off=c.symbols['??_7CBaseIntelligentPointer@@6B@'];built,rel=c.sections[sec];weak='??_ECBaseIntelligentPointer@@UAEPAXI@Z'
    assert off==0 and built==bytes(4) and rel=={0:(weak,6)}
    index=next(i for i,n in c.indices.items() if n==weak);symptr=struct.unpack_from('<I',c.data,8)[0];at=symptr+18*index
    assert c.data[at+16:at+18]==bytes([105,1]);target,search=struct.unpack_from('<II',c.data,at+18)
    assert search==2 and c.indices[target]=='??_GCBaseIntelligentPointer@@UAEPAXI@Z'
    comparative=0
    for pattern in range(256):assert execute(candidate[0],0,pattern)==execute(native[0],0,pattern);comparative+=1
    for operation in [1,2]:
        for pattern in [0,1,127,255]:
            for state in range(3):
                for refs in COUNTS:
                    for flags in ([0,1,2,3] if operation==2 else [0]):
                        args=operation,pattern,state,refs,flags
                        assert execute(candidate[operation],*args)==execute(native[operation],*args);comparative+=1
    test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(a.test.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr
    assert not any(s['symbol'] in Coff(test).symbols for s in SPECS[:2]);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    r=subprocess.run(link+[str(test)],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
    r=subprocess.run(link+[str(obj),str(test)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
    r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'INTELLIGENT_LIFETIME_BEHAVIOR_PASS scenarios=6144' in r.stdout,r.stdout+r.stderr
    report=dict(source=str(a.source),source_sha256=hashlib.sha256(a.source.read_bytes()).hexdigest(),entries=[dict(s,grade='RELOCATION_MATCH') for s in SPECS],comparative_scenarios=comparative,missing_provider_checks=1,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),vtable=dict(address='0129c95c',slot0='00a01c50',weak_search=search,weak_default=c.indices[target]))
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('INTELLIGENT_LIFETIME_PASS methods=3 bytes=140 comparative=%d compiled=6144'%comparative)
if __name__=='__main__':main()
