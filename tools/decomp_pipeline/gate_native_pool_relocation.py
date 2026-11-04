"""Compose actual pool defragmentation/MoveArea, resource copy/callback and free."""
import hashlib
import json
import tempfile
from pathlib import Path
from gate_native_movable_pool_lifecycle import Machine,providers,R,POOL,BUFFER,API
import gate_native_pool_defragment as defragment
import gate_native_pool_critical as critical
import gate_movable_pool_policy as movable
from movable_pool_construct_oracle import compile_codes
from pe_oracle import EXE,read_virtual_data
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESP,UC_X86_REG_EIP

RESOURCE,RESOURCE_VT=0x2005000,0x2005100
MEMMOVE=0xbfeae6
RELOCATE=API+0x100


class RelocationMachine(Machine):
    def __init__(self,codes,capacity,multi):
        super().__init__(codes,capacity,multi,0,0,0)
        self.expected={}
        self.moved=[]
        self.invalid_during_move=True
        self.u.mem_map(0x129d000,0x1000)
        self.put(POOL,0x129d6a0)
        self.put(0x129d6a4,0xa62f30)
        self.u.mem_write(POOL+0x26,b'\1')
        self.put(RESOURCE,RESOURCE_VT)
        self.put(RESOURCE_VT+4,RELOCATE)
        self.u.mem_write(MEMMOVE,b'\xc3')
        self.u.mem_write(RELOCATE,b'\xc3')

    def hook(self,u,address,size,user):
        if address not in (MEMMOVE,RELOCATE):
            return super().hook(u,address,size,user)
        sp=u.reg_read(UC_X86_REG_ESP)
        if address==MEMMOVE:
            to,source,length=(self.get(sp+4*i) for i in (1,2,3))
            assert BUFFER<=source<=BUFFER+self.capacity-length
            assert BUFFER<=to<=BUFFER+self.capacity-length
            if length:u.mem_write(to,bytes(u.mem_read(source,length)))
            self.trace.append(('memmove',to,source,length))
            result,pop=to,0
        else:
            assert u.reg_read(UC_X86_REG_ECX)==RESOURCE
            handle,source,to,length=(self.get(sp+4*i) for i in (1,2,3,4))
            assert bytes(u.mem_read(to,length))==self.expected[handle]
            # Defragment temporarily invalidates the handle during relocation.
            assert self.get(self.get(POOL+0x438)+handle*4)==(0 if self.invalid_during_move else source-32)
            self.moved.append(handle)
            self.trace.append(('relocate',handle,source,to,length))
            result,pop=0,16
        u.reg_write(UC_X86_REG_EAX,result)
        u.reg_write(UC_X86_REG_ECX,0xdeadbeef)
        u.reg_write(UC_X86_REG_EDX,0x87654321)
        u.reg_write(UC_X86_REG_EIP,self.get(sp))
        u.reg_write(UC_X86_REG_ESP,sp+4+pop)


def scenario(codes,capacity,sizes,present,multi,mode):
    m=RelocationMachine(codes,capacity,multi)
    snapshots=[]
    assert m.call(0xa01d50,[BUFFER,capacity,0,0])==BUFFER
    m.call(0xa01da0,[BUFFER])
    live=[]
    for index,size in enumerate(sizes):
        out=0x2003000
        m.put(out,0x87654321)
        result=m.call(0xa62e40,[out,RESOURCE if present else 0,size,0])&255
        if result:
            handle=m.get(out)
            header=m.get(m.get(POOL+0x438)+handle*4)
            length=(size+15)//16*16
            data=bytes((i*17+index*31)&255 for i in range(length))
            if length:m.u.mem_write(header+32,data)
            m.expected[handle]=data
            live.append(handle)
        snapshots.append((result,m.snapshot()))
    assert live
    m.call(0xa02b30,[live.pop(0)])
    snapshots.append(m.snapshot())
    before={h:m.get(m.get(POOL+0x438)+h*4) for h in live}
    if mode=='disabled':m.u.mem_write(POOL+0x26,b'\0')
    if mode=='shutdown':m.u.mem_write(POOL+0x25,b'\1')
    if mode=='locked' and live:m.call(0xa026f0,[live[0]])
    unchanged=m.snapshot()[0]
    m.call(0xa02730,[])
    snapshots.append(m.snapshot())
    blocked=mode in ('disabled','shutdown') or (mode=='locked' and bool(live))
    if blocked:assert m.snapshot()[0]==unchanged and not m.moved
    next_header=BUFFER
    expected_moved=[]
    for handle in live:
        pointer=m.call(0xa026b0,[handle])
        assert m.call(0xa01d40,[pointer])==handle
        assert m.call(0xa028d0,[handle])==(RESOURCE if present else 0)
        length=m.call(0xa026d0,[handle])
        assert bytes(m.u.mem_read(pointer,length))==m.expected[handle]
        if not blocked:
            assert pointer==next_header+32
            if before[handle]!=next_header:expected_moved.append(handle)
            next_header+=32+length
    if not blocked:assert m.moved==(expected_moved if present else [])
    if mode=='locked' and live:m.call(0xa02710,[live[0]])
    for handle in reversed(live):
        m.call(0xa02b30,[handle])
        snapshots.append(m.snapshot())
    assert m.get(POOL+0x10)==0 and m.get(POOL+0x18)==0
    assert m.get(POOL+0x1c)==capacity-32
    return snapshots


def move_area_scenario(codes,capacity,first_size,moved_size,present,multi):
    m=RelocationMachine(codes,capacity,multi)
    m.invalid_during_move=False
    snapshots=[]
    assert m.call(0xa01d50,[BUFFER,capacity,0,0])==BUFFER
    m.call(0xa01da0,[BUFFER])
    handles=[]
    for index,size in enumerate((first_size,moved_size,16)):
        assert m.call(0xa62e40,[0x2003000,RESOURCE if present else 0,size,0])&255
        handle=m.get(0x2003000)
        header=m.get(m.get(POOL+0x438)+handle*4)
        length=(size+15)//16*16
        data=bytes((i*7+index*31)&255 for i in range(length))
        if length:m.u.mem_write(header+32,data)
        m.expected[handle]=data
        handles.append(handle)
        snapshots.append(m.snapshot())
    m.call(0xa02b30,[handles[0]])
    snapshots.append(m.snapshot())
    source=m.get(m.get(POOL+0x438)+handles[1]*4)
    m.call(0xa028f0,[source,BUFFER])
    assert m.get(m.get(POOL+0x438)+handles[1]*4)==BUFFER
    assert m.get(BUFFER)==(RESOURCE if present else 0)
    assert bytes(m.u.mem_read(BUFFER+32,len(m.expected[handles[1]])))==m.expected[handles[1]]
    assert m.moved==([handles[1]] if present else [])
    snapshots.append(m.snapshot())
    m.put(0x2003200,source)
    m.call(0xa02470,[0x2003200])
    snapshots.append(m.snapshot())
    for handle in reversed(handles[1:]):
        pointer=m.call(0xa026b0,[handle])
        assert bytes(m.u.mem_read(pointer,len(m.expected[handle])))==m.expected[handle]
        m.call(0xa02b30,[handle])
        snapshots.append(m.snapshot())
    assert m.get(POOL+0x10)==0 and m.get(POOL+0x1c)==capacity-32
    return snapshots


def main():
    roots,addresses=providers()
    roots=[r for r in roots if r['address']!=0xa032f0]
    roots.extend(r for r in defragment.ROOTS if r['name'] in ('defragment','move_area'))
    roots.extend(r for r in critical.ROOTS if r['name']=='combine_adjacent_areas')
    addresses.update(defragment.ADDRESS)
    spec=next(s for s in movable.SPECS if s['address']=='00a62f30')
    roots.append(dict(source=R/'rebuild/src/compiled/00/a6'/spec['file'],symbol=spec['symbol'],address=0xa62f30))
    addresses.update(movable.EXTERNAL)
    inputs={Path(__file__),Path(movable.__file__),Path(defragment.__file__),Path(critical.__file__),
            R/'tools/decomp_pipeline/gate_native_movable_pool_lifecycle.py',
            R/'tools/decomp_pipeline/movable_pool_construct_oracle.py',EXE}
    def closure(path):
        if path in inputs:return
        inputs.add(path)
        import re
        for name in re.findall(r'^\s*#include\s*"([^"]+)"',path.read_text(),re.M):
            q=next((q for q in (path.parent/name,R/'rebuild/include'/name) if q.is_file()),None)
            assert q,name
            closure(q)
    for root in roots:closure(root['source'])
    pins={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    with tempfile.TemporaryDirectory(prefix='pool-relocation-') as tmp:
        candidate=compile_codes(roots,addresses,Path(tmp))
    raw=EXE.read_bytes()
    native={a:read_virtual_data(raw,a,len(data)) for a,data in candidate.items()}
    for a,data in candidate.items():
        if a!=0xa02b30:assert data==native[a],hex(a)
    cases=checkpoints=0
    for capacity in (256,512,1024):
        for sizes in ((16,32,48),(0,1,15),(64,16,32),(128,128,128)):
            for present in (False,True):
                for multi in (False,True):
                    for mode in ('normal','disabled','shutdown','locked'):
                        args=(capacity,sizes,present,multi,mode)
                        try:
                            actual=scenario(candidate,*args)
                            assert actual==scenario(native,*args)
                        except Exception as error:raise AssertionError(args) from error
                        cases+=1;checkpoints+=len(actual)
    for capacity in (512,1024):
        for first_size in (64,128):
            for moved_size in (0,1,16,32):
                for present in (False,True):
                    for multi in (False,True):
                        args=(capacity,first_size,moved_size,present,multi)
                        actual=move_area_scenario(candidate,*args)
                        assert actual==move_area_scenario(native,*args)
                        cases+=1;checkpoints+=len(actual)
    omissions=[]
    for root in roots:
        reduced={a:data for a,data in candidate.items() if a!=root['address']}
        detected=False
        for mode in ('normal','locked'):
            try:scenario(reduced,512,(16,32,48),True,True,mode)
            except Exception:detected=True;break
        if not detected:
            try:move_area_scenario(reduced,512,128,32,True,True)
            except Exception:detected=True
        assert detected,('redundant provider',root['symbol'])
        omissions.append(root['symbol'])
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==digest for p,digest in pins.items())
    report=dict(status='PASS',cases=cases,checkpoints=checkpoints,providers=len(roots),missing_provider_checks=omissions,input_sha256=pins,
                boundaries=['Controlled CRT allocation, memmove, recursive Win32 locks and resource RelocateData callback.',
                            'No concurrent threads, failure-handler or PartialDefragment claim.'])
    (R/'rebuild/compile-gate/native-pool-relocation.json').write_text(json.dumps(report,indent=2)+'\n')
    print('NATIVE_POOL_RELOCATION_PASS cases=%d checkpoints=%d providers=%d omissions=%d'%(cases,checkpoints,len(roots),len(omissions)))


if __name__=='__main__':main()
