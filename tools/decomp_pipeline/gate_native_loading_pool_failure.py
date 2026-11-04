"""Connect real loading handler, bank eviction, pool policy and async-data movement."""
import hashlib
import itertools
import json
import tempfile
from pathlib import Path
import gate_native_pool_partial_defragment as partial
import gate_native_bank_failed_allocation as bank
from movable_pool_construct_oracle import compile_codes
from near_match import compare,retail_relocations
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESP,UC_X86_REG_EIP

R,POOL,BUFFER,API=partial.R,partial.POOL,partial.BUFFER,partial.API
BANK,TREE,NODES,OBJECTS,HANDLER,BANKS=0x2008000,0x2009000,0x2009100,0x200a000,0x200b000,0x200b100


def providers():
    roots,addresses=partial.providers()
    roots=[r for r in roots if r['address'] not in (0xa026f0,0xa02710,0xa03780,0xa038c0)]
    roots.extend(r for r in partial.policy.ROOTS if r['name']=='scoped_critical_dtor')
    roots.extend(r for r in bank.lifetime.ROOTS if r['name']=='free_buffer')
    roots.append(dict(source=R/'rebuild/src/compiled/00/9d/CBankFileAsyncData_Lifetime_009d5410.cpp',symbol=bank.SYMBOL,address=bank.ENTRY))
    roots.extend(r for r in bank.loading.ROOTS if r['name']=='handler_failed_allocation')
    for name,symbol,address in (
        ('CCriticalSectionLock_Constructor','??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z',0xa62920),
        ('CCriticalSectionLock_Destructor','??1CCriticalSectionLock@@QAE@XZ',0xa629a0)):
        roots.append(dict(source=R/'rebuild/src/compiled/00/a6'/('%s_%08x.cpp'%(name,address)),symbol=symbol,address=address))
    for name,symbol,address in (
        ('CanRelocateData','?CanRelocateData@CBankFileAsyncData@@UBE_NJ@Z',0x9d55d0),
        ('RelocateData','?RelocateData@CBankFileAsyncData@@UAEXJPAX0J@Z',0x9d5670)):
        roots.append(dict(source=R/'rebuild/src/compiled/00/9d'/('CBankFileAsyncData_%s_%08x.cpp'%(name,address)),symbol=symbol,address=address))
    spec=next(s for s in partial.relocation.movable.SPECS if s['address']=='00a62ed0')
    roots.append(dict(source=R/'rebuild/src/compiled/00/a6'/spec['file'],symbol=spec['symbol'],address=0xa62ed0))
    for mapping in (bank.lifetime.ADDRESS,bank.buffer.ADDRESS,bank.loading.ADDRESS):addresses.update(mapping)
    addresses.update({'__imp__EnterCriticalSection@4':0x143fe0c,'__imp__TryEnterCriticalSection@4':0x143fd30,'__imp__LeaveCriticalSection@4':0x143fe10})
    return roots,addresses


class LoadingMachine(partial.PartialMachine):
    def __init__(self,codes,multi,threshold,registered):
        super().__init__({a:data for a,data in codes.items() if a>>16!=0x9d},512,multi)
        self.u.mem_map(0x9d0000,0x10000);self.u.mem_write(0x9d0000,b'\xcc'*0x10000)
        for a,data in codes.items():
            if a>>16==0x9d:self.u.mem_write(a,data)
        self.u.mem_map(0x129b000,0x1000)
        self.u.mem_write(0x129b800,read_virtual_data(EXE.read_bytes(),0x129b800,0x100))
        self.u.mem_write(bank.INSERT,b'\xc3')
        self.held=[];self.max_depth=0;self.handler_calls=0;self.bank_calls=0;self.restarted=[]
        self.put(POOL+0x28,threshold);self.put(POOL+0x2c,threshold);self.put(POOL+0x30,16)
        self.put(BANK+0x128,TREE);self.put(BANK+0x12c,8)
        self.put(BANK+0x134,TREE+0x200);self.put(BANK+0x138,0);self.put(BANK+0x140,0)
        self.put(HANDLER,0x129b8c4);self.put(HANDLER+4,BANKS)
        self.put(HANDLER+8,BANKS+(4 if registered else 0));self.put(HANDLER+12,BANKS+4);self.put(BANKS,BANK)
        def build(first,last,parent):
            if first==last:return 0
            at=(first+last)//2;node=NODES+at*20
            self.put(node,1);self.put(node+4,parent)
            self.put(node+8,build(first,at,node));self.put(node+12,build(at+1,last,node));self.put(node+16,OBJECTS+at*0x48)
            return node
        self.put(TREE,0);self.put(TREE+4,build(0,8,TREE));self.put(TREE+8,NODES);self.put(TREE+12,NODES+7*20)

    def hook(self,u,address,size,user):
        self.executed.add(address)
        if address in (API,API+16,API+32):
            sp=u.reg_read(UC_X86_REG_ESP);section=self.get(sp+4)
            assert section in (POOL+0x44c,BANK+0x14c,HANDLER+0x10)
            if address==API+16:
                assert self.held and self.held.pop()==section;name='leave'
            else:
                self.held.append(section);name='try' if address==API+32 else 'enter'
            self.depth=len(self.held);assert self.depth<=5;self.max_depth=max(self.max_depth,self.depth)
            self.trace.append((name,section,self.depth))
            u.reg_write(UC_X86_REG_EAX,1);u.reg_write(UC_X86_REG_ECX,0xdeadbeef);u.reg_write(UC_X86_REG_EDX,0x87654321)
            u.reg_write(UC_X86_REG_EIP,self.get(sp));u.reg_write(UC_X86_REG_ESP,sp+8)
            return
        if address==0x9d5f10:
            assert u.reg_read(UC_X86_REG_ECX)==HANDLER
            self.handler_calls+=1;self.trace.append(('loading_handler',self.get(u.reg_read(UC_X86_REG_ESP)+4)))
        if address==bank.ENTRY:
            assert u.reg_read(UC_X86_REG_ECX)==BANK
            self.bank_calls+=1;self.trace.append(('bank_failure',))
        if address==0x9d5670:
            obj=u.reg_read(UC_X86_REG_ECX)-4
            sp=u.reg_read(UC_X86_REG_ESP)
            handle,old,new,length=(self.get(sp+i*4) for i in (1,2,3,4))
            assert obj==OBJECTS+handle*0x48
            assert bytes(u.mem_read(new,length))==self.expected[handle]
            assert self.get(obj+12)==old and self.get(obj+8)==old+(handle%3)*4
            self.moved.append(handle);self.trace.append(('data_relocate',handle,old,new,length))
        if address==bank.INSERT:
            sp=u.reg_read(UC_X86_REG_ESP)
            assert u.reg_read(UC_X86_REG_ECX)==BANK+0x134
            result,value=self.get(sp+4),self.get(self.get(sp+8))
            assert OBJECTS<=value<OBJECTS+8*0x48 and (value-OBJECTS)%0x48==0
            assert value not in self.restarted
            assert self.get(value+0x20)==0xffffffff and self.get(value+8)==0 and self.get(value+12)==0
            assert u.mem_read(value+0x44,1)==b'\0'
            self.restarted.append(value);self.put(BANK+0x138,len(self.restarted))
            self.put(result,NODES+0x400);self.put(result+4,1)
            self.trace.append(('restart_insert',(value-OBJECTS)//0x48))
            u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ECX,0xdeadbeef);u.reg_write(UC_X86_REG_EDX,0x87654321)
            u.reg_write(UC_X86_REG_EIP,self.get(sp));u.reg_write(UC_X86_REG_ESP,sp+12)
            return
        super().hook(u,address,size,user)


def scenario(codes,eligible,buffer_locked,requested,flags,threshold,multi,registered,machine_type=LoadingMachine):
    m=machine_type(codes,multi,threshold,registered)
    assert m.call(0xa01d50,[BUFFER,512,0,0])==BUFFER
    m.call(0xa01da0,[BUFFER]);snapshots=[]
    for index in range(8):
        obj=OBJECTS+index*0x48
        m.put(obj+4,0x129b8a0);m.put(obj+0x28,BANK)
        assert m.call(0xa62e40,[0x2003000,obj+4,32,0])&255
        assert m.get(0x2003000)==index
        pointer=m.call(0xa026b0,[index]);data=bytes((i*17+index*31)&255 for i in range(32))
        m.u.mem_write(pointer,data);m.expected[index]=data
        m.put(obj+8,pointer+(index%3)*4);m.put(obj+12,pointer);m.put(obj+0x1c,POOL);m.put(obj+0x20,index)
        m.u.mem_write(obj+0x44,bytes([1,int(buffer_locked),(eligible>>index)&1,0]))
        snapshots.append(m.snapshot())
    m.put(POOL+0x34,HANDLER);m.put(0x2003000,0x87654321)
    result=m.call(0xa62e40,[0x2003000,0,requested,flags])&255
    assert result in (0,1) and not m.held
    if flags&2:assert not m.handler_calls and not result
    if not registered:assert not m.bank_calls and not result
    assert m.bank_calls==(m.handler_calls if registered else 0)
    assert all(eligible&(1<<((obj-OBJECTS)//0x48)) for obj in m.restarted)
    if not eligible:assert not m.restarted and not result
    live=[]
    for index in range(8):
        obj=OBJECTS+index*0x48
        if obj in m.restarted:
            assert m.get(obj+0x20)==0xffffffff and m.get(obj+8)==0 and m.get(obj+12)==0
        else:
            assert m.get(obj+0x20)==index
            pointer=m.call(0xa026b0,[index])
            assert m.call(0xa01d40,[pointer])==index
            assert m.call(0xa028d0,[index])==obj+4
            assert m.get(obj+12)==pointer and m.get(obj+8)==pointer+(index%3)*4
            assert bytes(m.u.mem_read(pointer,32))==m.expected[index]
            live.append(index)
    if result:
        fresh=m.get(0x2003000);assert fresh not in live
        assert m.call(0xa026d0,[fresh])==(requested+15)//16*16
        assert m.call(0xa028d0,[fresh])==0
    else:assert m.get(0x2003000)==0x87654321
    snapshots.append((result,m.snapshot()))
    for index in reversed(live):
        m.call(0x9d5410,[],OBJECTS+index*0x48);snapshots.append(m.snapshot())
    if result:m.call(0xa02b30,[fresh]);snapshots.append(m.snapshot())
    assert m.get(POOL+0x10)==0 and m.get(POOL+0x18)==0 and m.get(POOL+0x1c)==480
    return snapshots,m.executed,dict(result=result,handler_calls=m.handler_calls,restarted=len(m.restarted),moves=len(m.moved),depth=m.max_depth)


def main():
    roots,addresses=providers()
    inputs={Path(__file__),Path(partial.__file__),Path(bank.__file__),Path(partial.relocation.__file__),EXE,
            R/'tools/decomp_pipeline/gate_native_movable_pool_lifecycle.py',R/'tools/decomp_pipeline/movable_pool_construct_oracle.py'}
    for module in (bank.lifetime,bank.buffer,bank.loading,partial.policy,partial.queue):inputs.add(Path(module.__file__))
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
    with tempfile.TemporaryDirectory(prefix='loading-pool-failure-') as tmp:
        candidate=compile_codes(roots,addresses,Path(tmp))
        # The bank root is near, while every other included provider except
        # the previously accepted near Free must remain byte-identical.
        obj=Coff(Path(tmp)/('provider%d.obj'%list(dict.fromkeys(r['source'] for r in roots)).index(next(r['source'] for r in roots if r['address']==bank.ENTRY))))
        section,_=obj.symbols[bank.SYMBOL];relocations=obj.sections[section][1]
    raw=EXE.read_bytes()
    native={a:read_virtual_data(raw,a,314 if a==bank.ENTRY else len(data)) for a,data in candidate.items()}
    assert compare(candidate[bank.ENTRY],native[bank.ENTRY],tuple(relocations),retail_relocations('009d5bf0',314)).near_match
    for a,data in candidate.items():
        if a not in (0xa02b30,bank.ENTRY):assert data==native[a],hex(a)
    assert int.from_bytes(read_virtual_data(raw,0x129b8a4,4),'little')==0x9d5670
    assert int.from_bytes(read_virtual_data(raw,0x129b8a8,4),'little')==0x9d55d0
    assert int.from_bytes(read_virtual_data(raw,0x129b8c8,4),'little')==0x9d5f10
    cases=checkpoints=calls=restarts=moves=depth=0;witnesses={}
    for args in itertools.product((0,3,85,255),(False,True),(33,96,513),(0,2),(0,128),(False,True),(False,True)):
        try:
            actual,seen,stats=scenario(candidate,*args)
            original,_,original_stats=scenario(native,*args)
            assert actual==original and stats==original_stats
        except Exception as error:raise AssertionError(args) from error
        cases+=1;checkpoints+=len(actual);calls+=stats['handler_calls'];restarts+=stats['restarted'];moves+=stats['moves'];depth=max(depth,stats['depth'])
        for root in roots:
            if root['address'] in seen:witnesses.setdefault(root['address'],args)
        if cases%64==0:print('loading pool progress cases=%d'%cases,flush=True)
    omissions=[];unused=[]
    for root in roots:
        if root['address'] not in witnesses:unused.append(root['symbol']);continue
        reduced={a:data for a,data in candidate.items() if a!=root['address']};detected=False
        try:scenario(reduced,*witnesses[root['address']])
        except Exception:detected=True
        assert detected,root['symbol'];omissions.append(root['symbol'])
    assert not unused,unused
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==digest for p,digest in pins.items())
    report=dict(status='PASS',cases=cases,checkpoints=checkpoints,providers=len(roots),missing_provider_checks=omissions,
                handler_calls=calls,restarted_reads=restarts,moves=moves,max_lock_depth=depth,input_sha256=pins,
                boundaries=['Restart-tree insertion, CRT allocation/memmove and Win32 locks controlled.',
                            'Actual loading handler, bank selection, pool release/policy and async-data permission/relocation execute; no concurrency or actual restarted disk reads.'])
    (R/'rebuild/compile-gate/native-loading-pool-failure.json').write_text(json.dumps(report,indent=2)+'\n')
    print('NATIVE_LOADING_POOL_FAILURE_PASS cases=%d checkpoints=%d providers=%d omissions=%d calls=%d restarts=%d moves=%d depth=%d'%(cases,checkpoints,len(roots),len(omissions),calls,restarts,moves,depth))


if __name__=='__main__':main()
