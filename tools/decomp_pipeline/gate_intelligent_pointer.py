"""Validate base intelligent-pointer ownership, expired targets, sharing, and allocation."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ROOT=Path(__file__).resolve().parents[2]
SPECS=[dict(address='00a01b20',name='ClearData',size=46,symbol='?ClearData@CBaseIntelligentPointer@@ABEXXZ'),dict(address='00a01b50',name='GetPItem',size=54,symbol='?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ'),dict(address='00a01b90',name='SetPItem',size=123,symbol='?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z')]
FILE='CBaseIntelligentPointer_Operations_00a01b20.cpp'
for s in SPECS:s['file']=FILE
TARGETS={'??3@YAXPAX@Z':0xbfe9bc,'??2@YAPAXI@Z':0xbfea1a}
COUNTS=[0,1,2,0xffffffff]
def execute(code,operation,old_state,old_count,incoming,shared,new_count):
    pointer,a,b,old,new,fresh,stack,stop=0x2000040,0x2100040,0x2100140,0x2200040,0x2200140,0x2200240,0x3008000,0x4000000
    item=[0,a,b][incoming];old_target=[0,0,a,b][old_state];old_refs=COUNTS[old_count];new_refs=COUNTS[new_count]
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for base,size in [(0xa01000,0x1000),(0xbfe000,0x1000),(0x2000000,0x1000),(0x2100000,0x1000),(0x2200000,0x1000),(0x3000000,0x10000),(stop,0x1000)]:u.mem_map(base,size)
    entry=int(SPECS[operation]['address'],16);u.mem_write(entry,bytes(code))
    regions={0x2000000:bytearray([0xa9]*0x300),0x2100000:bytearray([0xb7]*0x300),0x2200000:bytearray([0xc5]*0x300)}
    def put(addr,value):
        base=addr&~0xffff;struct.pack_into('<I',regions[base],addr-base,value&0xffffffff)
    put(pointer+4,old if old_state else 0);put(a+4,new if item==a and shared else 0);put(b+4,new if item==b and shared else 0)
    put(old,old_target);put(old+4,old_refs);put(new,item);put(new+4,new_refs)
    for base,data in regions.items():u.mem_write(base,bytes(data))
    u.mem_write(stack,struct.pack('<II',stop,item));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,pointer)
    saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x43210000+i)
    trace=[]
    def hook(uc,pc,size,user):
        if pc not in TARGETS.values():return
        sp=uc.reg_read(UC_X86_REG_ESP);arg=struct.unpack('<I',uc.mem_read(sp+4,4))[0]
        if pc==0xbfea1a:assert arg==8;trace.append(('new',arg));result=fresh
        else:trace.append(('delete',arg));result=0xaaaa5555
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xcccccccc);uc.reg_write(UC_X86_REG_EDX,0xdddddddd)
        uc.reg_write(UC_X86_REG_EIP,struct.unpack('<I',uc.mem_read(sp,4))[0]);uc.reg_write(UC_X86_REG_ESP,sp+4)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(entry,stop,count=300)
    same=bool(old_state and item and old_target==item)
    clear=bool(old_state and (operation==0 or (operation==1 and not old_target) or (operation==2 and not same)))
    freed=bool(clear and old_refs==1 and not old_target);expected=[]
    if clear:put(old+4,old_refs-1);put(pointer+4,0)
    if freed:expected.append(('delete',old))
    if operation==2 and not same and item:
        if shared:put(pointer+4,new);put(new+4,new_refs+1)
        else:put(pointer+4,fresh);put(fresh,item);put(fresh+4,1);put(item+4,fresh);expected.append(('new',8))
    assert trace==expected,(operation,old_state,old_count,incoming,shared,new_count,trace,expected)
    if operation==1:assert u.reg_read(UC_X86_REG_EAX)==old_target
    assert all(bytes(u.mem_read(base,len(data)))==data for base,data in regions.items())
    assert u.reg_read(UC_X86_REG_ESP)==stack+(8 if operation==2 else 4) and all(u.reg_read(r)==0x43210000+i for i,r in enumerate(saved))
    return trace,[bytes(data) for data in regions.values()]
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--stage',type=Path)
    ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/IntelligentPointer_test.cpp')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/intelligent-pointer.json');a=ap.parse_args()
    env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='intelligent-pointer-'));objects=[];providers=[];candidate=[];native=[]
    def compile(source,index):
        obj=work/('source%d.obj'%index);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;return obj
    source=a.stage/FILE if a.stage else ROOT/'rebuild/src/compiled/00/a0'/FILE
    obj=compile(source,0);objects.append(obj);c=Coff(obj)
    providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    with pefile.PE(str(EXE)) as pe:
        for i,s in enumerate(SPECS):
            sec,off=c.symbols[s['symbol']];built,rel=c.sections[sec];assert off==0 and len(built)==s['size'];address=int(s['address'],16);patched=bytearray(built)
            for site,(name,kind) in rel.items():
                assert kind==20;struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<I',built,site)[0]-address-site-4)&0xffffffff)
            retail=pe.get_data(address-0x400000,len(built));assert patched==retail,s
            candidate.append(patched);native.append(retail)
    comparative=0
    for operation in range(3):
        for old_state in range(4):
            for old_count in range(4):
                for incoming in range(3):
                    for shared in range(2):
                        for new_count in range(4):
                            args=operation,old_state,old_count,incoming,shared,new_count
                            assert execute(candidate[operation],*args)==execute(native[operation],*args);comparative+=1
    test=compile(a.test,3);assert not any(s['symbol'] in Coff(test).symbols for s in SPECS);objects.append(test);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(1):
        r=subprocess.run(link+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
    r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'INTELLIGENT_POINTER_BEHAVIOR_PASS scenarios=1152' in r.stdout,r.stdout+r.stderr
    report=dict(providers=providers,entries=[dict(s,grade='RELOCATION_MATCH') for s in SPECS],comparative_scenarios=comparative,missing_provider_checks=1,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),header_sha256=hashlib.sha256((a.include/'fable_intelligent_pointer.h').read_bytes()).hexdigest())
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('INTELLIGENT_POINTER_PASS methods=3 providers=1 bytes=223 compiled=1152 comparative=%d'%comparative)
if __name__=='__main__':main()
