"""Manager destruction with actual shutdown and both actual tree erasers."""
import hashlib
import itertools
import struct
import tempfile
from pathlib import Path
import gate_native_threaded_file_manager_remove as manager
import gate_native_threaded_manager_shutdown as shutdown
from movable_pool_construct_oracle import compile_codes
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE,read_virtual_data
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESP,UC_X86_REG_EIP

R=Path(__file__).resolve().parents[2]
ENTRY,OBJECT,STACK,STOP,API=0x98e690,0x2000100,0x3000000,0x4000000,0x5000000
DELETING=0x991330
SYMBOL='??1CThreadedFileManager@@UAE@XZ'
SOURCE=R/'rebuild/src/compiled/00/98/CThreadedFileManager_Destructor_0098e690.cpp'


def providers(source=SOURCE,deleting=False):
    roots=[dict(source=source,symbol=SYMBOL,address=ENTRY)]
    roots.extend(shutdown.ROOTS)
    roots.extend(r for r in manager.ROOTS if r['name'] in ('uid_node_erase','pending_node_erase'))
    roots.append(dict(source=R/'rebuild/src/compiled/00/99/CBaseClass_CBaseClass_0099a2d0.cpp',
                      symbol='??1CBaseClassNonCopyable@@UAE@XZ',address=0x99a300))
    addresses=dict(manager.ADDRESS);addresses.update(shutdown.ADDRESS)
    addresses.update({'??_7CThreadedFileManager@@6B@':0x129a164,'??1CBaseClassNonCopyable@@UAE@XZ':0x99a300,
                      '?Uninitialise@CThreadedFileManager@@QAEXXZ':0x98e0d0,
                      '??_7CBaseClass@@6B@':0x1231710,'_free':0xbfea14})
    if deleting:
        roots.append(dict(source=source,symbol='??_GCThreadedFileManager@@UAEPAXI@Z',address=DELETING))
        addresses.update({SYMBOL:ENTRY,'??3@YAXPAX@Z':0xbfe9bc})
    return roots,addresses


def execute(codes,args,deleting_flags=None):
    mode,null_heads,count,shape,initialised,finished,delay,event=args
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for a,size in ((0x98e000,0x2000),(0x991000,0x1000),(0x99a000,0x1000),(0xbfe000,0x1000),(0x143f000,0x1000),
                   (0x2000000,0x10000),(STACK,0x10000),(STOP,0x1000),(API,0x1000)):u.mem_map(a,size)
    u.mem_write(0x98e000,b'\xcc'*0x2000);u.mem_write(0x99a000,b'\xcc'*0x1000)
    u.mem_write(0x991000,b'\xcc'*0x1000)
    for a,b in codes.items():u.mem_write(a,b)
    u.mem_write(0xbfea14,b'\xc3');u.mem_write(0xbfe9bc,b'\xc3');u.mem_write(0x2000000,b'\xa5'*0x10000)
    def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    slots={'EnterCriticalSection':0x143fe0c,'LeaveCriticalSection':0x143fe10,
           'SetEvent':0x143fde0,'Sleep':0x143fe1c,'CloseHandle':0x143fe30,'DeleteCriticalSection':0x143fde4}
    hooks={API+i*16:name for i,name in enumerate(slots)}
    for a,name in hooks.items():put(slots[name],a);u.mem_write(a,b'\xc3')
    u.mem_write(OBJECT+0x148,bytes([int(initialised),0,int(finished)]));put(OBJECT+0x164,event)
    blocks={};maps={};nodes=set();free_order=[]
    for index in range(15):
        at=OBJECT+0x13c if index==14 else OBJECT+4+index*16
        populated=mode=='full' or mode=='uid' and index==14 or mode=='one' and index==13 or mode=='striped' and index%3==0
        n=count if populated else 0
        null=not n and (null_heads=='all' or null_heads=='even' and index%2==0)
        header=0x2001000+index*0x200 if not null else 0
        put(at,header);put(at+4,n);maps[index]=(at,header,n)
        if not header:continue
        blocks[header]=24
        def build(indices,parent):
            if not indices:return 0
            split=0 if shape=='right' else len(indices)-1 if shape=='left' else len(indices)//2
            i=indices[split];node=header+0x40+i*24
            blocks[node]=24;nodes.add(node);put(node,1);put(node+4,parent)
            put(node+8,build(indices[:split],node));put(node+12,build(indices[split+1:],node))
            put(node+16,i+1);put(node+20,0x2008000+(index*count+i)*8)
            return node
        put(header,0);put(header+4,build(list(range(n)),header))
        put(header+8,header+0x40 if n else header);put(header+12,header+0x40+(n-1)*24 if n else header)
    initial=bytes(u.mem_read(0x2000000,0x10000));expected=bytearray(initial)
    struct.pack_into('<I',expected,OBJECT-0x2000000,0x1231710)
    if initialised:
        expected[OBJECT+0x148-0x2000000:OBJECT+0x14b-0x2000000]=b'\0\1\1'
        struct.pack_into('<I',expected,OBJECT+0x164-0x2000000,0)
    for at,header,n in maps.values():
        if n:struct.pack_into('<I',expected,at+4-0x2000000,0)
    for pointer,size in blocks.items():expected[pointer-0x2000000:pointer-0x2000000+size]=b'\xdd'*size
    should_delete=deleting_flags is not None and bool(deleting_flags&1)
    if should_delete:expected[OBJECT-0x2000000:OBJECT-0x2000000+0x178]=b'\xdd'*0x178
    trace=[];depth=0;sleeps=0;deleted=False;seen=set()
    def hook(uc,a,size,user):
        nonlocal depth,sleeps,deleted
        seen.add(a)
        if a==0x98e0d0:
            assert get(OBJECT)==0x129a164 and uc.reg_read(UC_X86_REG_ECX)==OBJECT
        if a not in hooks and a not in (0xbfea14,0xbfe9bc):return
        sp=uc.reg_read(UC_X86_REG_ESP);arg=get(sp+4);pop=4;result=1
        if a==0xbfe9bc:
            assert should_delete and not blocks and arg==OBJECT and depth==0
            assert get(OBJECT)==0x1231710
            u.mem_write(OBJECT,b'\xdd'*0x178);name='delete_manager';pop=0
        elif a==0xbfea14:
            assert depth==0 and arg in blocks,('free',hex(arg))
            if initialised:assert deleted
            u.mem_write(arg,b'\xdd'*blocks.pop(arg));free_order.append(arg);pop=0;name='free'
        else:
            name=hooks[a]
            if name=='EnterCriticalSection':assert arg==OBJECT+0x14c and depth==0;depth=1
            elif name=='LeaveCriticalSection':assert arg==OBJECT+0x14c and depth==1;depth=0
            elif name=='SetEvent':
                assert depth==1 and arg==event and uc.mem_read(OBJECT+0x149,1)==b'\1'
                if not delay:uc.mem_write(OBJECT+0x14a,b'\1')
            elif name=='Sleep':
                assert depth==0 and arg==0;sleeps+=1;assert sleeps<=delay
                if sleeps==delay:uc.mem_write(OBJECT+0x14a,b'\1')
            elif name=='CloseHandle':assert depth==1 and event and arg==event
            else:assert name=='DeleteCriticalSection' and depth==0 and arg==OBJECT+0x14c;deleted=True
        trace.append((name,arg))
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xdeadbeef);uc.reg_write(UC_X86_REG_EDX,0x87654321)
        uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook)
    sp=STACK+0x8000;put(sp,STOP)
    if deleting_flags is not None:put(sp+4,deleting_flags)
    saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
    for i,reg in enumerate(saved):u.reg_write(reg,0x55660000+i)
    u.reg_write(UC_X86_REG_ECX,OBJECT);u.reg_write(UC_X86_REG_ESP,sp)
    u.emu_start(DELETING if deleting_flags is not None else ENTRY,STOP,count=20000)
    assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4+(4 if deleting_flags is not None else 0)
    if deleting_flags is not None:assert u.reg_read(UC_X86_REG_EAX)==OBJECT
    assert all(u.reg_read(reg)==0x55660000+i for i,reg in enumerate(saved))
    assert not blocks and depth==0 and deleted==bool(initialised)
    assert sum(name=='delete_manager' for name,_ in trace)==int(should_delete)
    headers=[maps[i][1] for i in (14,*range(13,-1,-1)) if maps[i][1]]
    assert [pointer for pointer in free_order if pointer not in nodes]==headers
    state=bytes(u.mem_read(0x2000000,0x10000));assert state==bytes(expected)
    return (state,tuple(trace)),seen


def compare_destroy(source=SOURCE,deleting=False):
    source=source.resolve();roots,addresses=providers(source,deleting)
    inputs={Path(__file__),EXE,R/'tools/decomp_pipeline/movable_pool_construct_oracle.py'};visited=set()
    def closure(path):
        if path in visited:return
        visited.add(path);inputs.add(path)
        import re
        for name in re.findall(r'^\s*#include\s*"([^"]+)"',path.read_text(),re.M):
            q=next((q for q in (path.parent/name,R/'rebuild/include'/name) if q.is_file()),None)
            assert q,name
            closure(q)
    for root in roots:closure(root['source'])
    pins={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    with tempfile.TemporaryDirectory(prefix='threaded-manager-destroy-') as tmp:
        codes=compile_codes(roots,addresses,Path(tmp));coff=Coff(Path(tmp)/'provider0.obj')
        section,_=coff.symbols[SYMBOL];relocations=coff.sections[section][1]
    raw=EXE.read_bytes();native={a:read_virtual_data(raw,a,214 if a==ENTRY else len(b)) for a,b in codes.items()}
    grade=compare(codes[ENTRY],native[ENTRY],tuple(relocations),retail_relocations('0098e690',214)).to_dict()
    assert grade['near_match']
    for a in codes:
        if a!=ENTRY:assert codes[a]==native[a],hex(a)
    cases=0;witnesses={}
    for args in itertools.product(('empty','uid','one','striped','full'),('none','even','all'),(1,3),('balanced','left','right'),(False,True),(False,True),(0,2),(0,0x12345678)):
        for flags in ((0,1,2,0xffffffff) if deleting else (None,)):
            try:
                actual,seen=execute(codes,args,flags);original,_=execute(native,args,flags);assert actual==original
            except Exception as error:raise AssertionError((args,flags)) from error
            cases+=1
            for root in roots:
                if root['address'] in seen:witnesses.setdefault(root['address'],(args,flags))
    for root in roots:
        assert root['address'] in witnesses
        reduced={a:b for a,b in codes.items() if a!=root['address']};detected=False
        try:execute(reduced,*witnesses[root['address']])
        except Exception:detected=True
        assert detected,root['symbol']
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==h for p,h in pins.items())
    return dict(status='PASS',cases=cases,providers=len(roots),missing_provider_checks=len(roots),grade=grade,input_sha256=pins,
                boundaries=['Actual shutdown, UID/pending recursive erasers and base destruction execute; CRT/Win32 and worker completion controlled.',
                            '15 map headers and all nodes released; pointed-to file objects remain untouched. No actual worker scheduling or IO.'])


if __name__=='__main__':
    import json,sys
    print(json.dumps(compare_destroy(Path(sys.argv[1]) if len(sys.argv)>1 else SOURCE),indent=2))
