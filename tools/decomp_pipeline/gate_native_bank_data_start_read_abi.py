"""Original/rebuilt private StartRead ABI and actual file-wrapper differential gate."""
import hashlib, itertools, json, re, shutil, struct, sys
from datetime import datetime, timezone
from pathlib import Path
R=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *

BASE=R/'rebuild/build/native-bank-data-start-read-abi/00/9d/009d5650'
OUT=BASE/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ')
OUT.mkdir(parents=True)
INPUTS={Path(__file__),EXE}
def closure(p):
    if p in INPUTS:return
    INPUTS.add(p)
    for name in re.findall(r'^\s*#include\s*"([^"]+)"',p.read_text(),re.M):
        q=next(q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file())
        closure(q)
root=R/'rebuild/src/compiled/00/9d/CBankFileAsyncData_StartRead_009d5650.cpp'
provider=R/'rebuild/src/compiled/00/98/CThreadedFile_AddObject_0098f350.cpp'
closure(root);closure(provider)
for name in ('check_one.py','gate_game_component_abi.py','pe_oracle.py'):
    INPUTS.add(R/'tools/decomp_pipeline'/name)
for name in ('egor_pdb_names.tsv','fablewin_pdb_names.tsv'):
    donor=R/'ghidra_out'/name
    assert '?StartRead@CBankFileAsyncData@@AAEXXZ' in donor.read_text(encoding='utf-8')
    INPUTS.add(donor)
for folder in ('bin','include','lib'):
    INPUTS.update(p for p in (V.VC/folder).rglob('*') if p.is_file())
pins={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in INPUTS}
(OUT/'input-pins.json').write_text(json.dumps(pins,indent=2))
(OUT/'include').mkdir()
for p in INPUTS:
    if p.is_relative_to(R/'rebuild/include'):shutil.copy2(p,OUT/'include'/p.name)
shutil.copy2(root,OUT/root.name);shutil.copy2(provider,OUT/provider.name)
ROOT,FILE,MANAGER,GLOBAL=0x9d5650,0x98f350,0x98f0f0,0x13bc870
rs='?StartRead@CBankFileAsyncData@@AAEXXZ'
fs='?AddObject@CThreadedFile@@QAEKABVCThreadedFileObjectDesc@@@Z'
ms='?AddObject@CThreadedFileManager@@QAEKABVCThreadedFileObjectDesc@@AAVCThreadedFile@@@Z'
addresses={fs:FILE,ms:MANAGER,'?g_FableThreadedFileManager_013BC870@@3VCThreadedFileManager@@A':GLOBAL}
codes={};objects={};relocs={}
env=V.env()
for src,symbol,address in ((root,rs,ROOT),(provider,fs,FILE)):
    obj=OUT/(src.stem+'.obj')
    result=V.cl(['/nologo','/c','/O2','/MT','/W3','/I'+str(OUT/'include'),'/Fo'+str(obj),str(OUT/src.name)],env,OUT)
    (OUT/(src.stem+'-compile.json')).write_text(json.dumps(dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr),indent=2))
    assert result.returncode==0
    coff=Coff(obj);section,offset=coff.symbols[symbol];assert offset==0
    data,relocations=coff.sections[section];body=bytearray(data)
    for site,(name,kind) in relocations.items():
        assert kind in (6,20)
        target=addresses[name]+struct.unpack_from('<i',body,site)[0]
        if kind==20:target-=address+site+4
        struct.pack_into('<I',body,site,target&0xffffffff)
    codes[address]=bytes(body);objects[str(obj)]=hashlib.sha256(obj.read_bytes()).hexdigest();relocs[symbol]=relocations
raw=EXE.read_bytes()
original={ROOT:read_virtual_data(raw,ROOT,26),FILE:read_virtual_data(raw,FILE,19)}
assert codes==original

def execute(bodies,seed,uid,ordinal):
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    ARENA,STACK,STOP=0x2000000,0x3000000,0x4000000
    for a,n in ((0x9d5000,0x1000),(0x98f000,0x1000),(0x13bc000,0x1000),(ARENA,0x4000),(STACK,0x10000),(STOP,0x1000)):
        u.mem_map(a,n);u.mem_write(a,b'\xcc'*n)
    for a,b in bodies.items():u.mem_write(a,b)
    def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    obj=ARENA+0x100+ordinal*0x80;bank=ARENA+0x1000+ordinal*0x200;file=ARENA+0x2800+ordinal*0x100
    u.mem_write(ARENA,bytes((i*37+seed*19)&255 for i in range(0x4000)))
    u.mem_write(0x13bc000,bytes((i*11+seed*23)&255 for i in range(0x1000)))
    put(obj+0x28,bank);put(bank+0x110,file)
    before=bytes(u.mem_read(ARENA,0x4000));expected=bytearray(before)
    gbefore=bytes(u.mem_read(0x13bc000,0x1000));gexpected=bytearray(gbefore)
    trace=[];sp=STACK+0x8000;put(sp,STOP)
    def hook(uc,a,size,_):
        if a!=MANAGER:return
        esp=uc.reg_read(UC_X86_REG_ESP)
        assert uc.reg_read(UC_X86_REG_ECX)==GLOBAL
        assert get(esp+4)==obj+0x2c and get(esp+8)==file
        assert get(obj+0x18)==struct.unpack_from('<I',before,obj-ARENA+0x18)[0]
        assert bytes(uc.mem_read(ARENA,0x4000))==before
        trace.append((GLOBAL,obj+0x2c,file,bytes(uc.mem_read(obj+0x2c,24)).hex()))
        # Independent boundary side effect verifies provider order and preservation.
        put(GLOBAL+0x100,uid^0xa5a55a5a)
        struct.pack_into('<I',gexpected,GLOBAL-0x13bc000+0x100,uid^0xa5a55a5a)
        uc.reg_write(UC_X86_REG_EAX,uid);uc.reg_write(UC_X86_REG_ECX,0xdeadbeef);uc.reg_write(UC_X86_REG_EDX,0x87654321)
        uc.reg_write(UC_X86_REG_EIP,get(esp));uc.reg_write(UC_X86_REG_ESP,esp+12)
    u.hook_add(UC_HOOK_CODE,hook)
    saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
    for i,r in enumerate(saved):u.reg_write(r,0x55660000+seed+i)
    u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,obj)
    u.emu_start(ROOT,STOP,count=128)
    assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4
    assert all(u.reg_read(r)==0x55660000+seed+i for i,r in enumerate(saved))
    assert len(trace)==1
    struct.pack_into('<I',expected,obj-ARENA+0x18,uid)
    state=bytes(u.mem_read(ARENA,0x4000));gs=bytes(u.mem_read(0x13bc000,0x1000))
    assert state==bytes(expected) and gs==bytes(gexpected)
    return state,gs,trace

cases=0
for args in itertools.product((0,1,7,255),(0,1,0x7fffffff,0x80000000,0xfffffffe,0xffffffff),range(4)):
    assert execute(codes,*args)==execute(original,*args)
    cases+=1
controls={}
mutants={
 'omit-root':{FILE:codes[FILE]},
 'omit-file':{ROOT:codes[ROOT]},
 'wrong-uid-field':dict(codes,**{}),
 'wrong-bank-field':dict(codes),
 'wrong-desc-field':dict(codes),
 'skip-store':dict(codes),
}
for name,index,value in (('wrong-uid-field',23,0x1c),('wrong-bank-field',5,0x24),('wrong-desc-field',14,0x30)):
    b=bytearray(codes[ROOT]);b[index]=value;mutants[name][ROOT]=bytes(b)
b=bytearray(codes[ROOT]);b[21:24]=b'\x90'*3;mutants['skip-store'][ROOT]=bytes(b)
for name,bodies in mutants.items():
    try:execute(bodies,7,0,2)
    except Exception as e:controls[name]=type(e).__name__
    else:raise AssertionError('undetected '+name)
assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==h for p,h in pins.items())
assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==h for p,h in objects.items())
report=dict(status='PASS',cases=cases,root_bytes=26,file_bytes=19,controls=controls,input_hashes=len(pins),object_hashes=objects,relocations=relocs,boundary='Manager AddObject controlled; no native worker or constructor/lifetime claim')
(OUT/'receipt.json').write_text(json.dumps(report,indent=2))
(R/'rebuild/compile-gate/native-bank-data-start-read-abi.json').write_text(json.dumps(dict(report,build=str(OUT)),indent=2))
print('NATIVE_BANK_DATA_START_READ_ABI_PASS cases=%d controls=%d root_bytes=26 file_bytes=19'%(cases,len(controls)))
print('RECEIPT',OUT/'receipt.json')
