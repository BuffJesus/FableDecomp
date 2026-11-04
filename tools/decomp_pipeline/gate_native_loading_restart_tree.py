"""Real restart-tree insertion/rebalancing connected to loading-pool eviction."""
import hashlib
import itertools
import json
import tempfile
from pathlib import Path
import gate_native_loading_pool_failure as loading
import gate_native_async_bank_insert_unique as unique
from movable_pool_construct_oracle import compile_codes
from near_match import compare,retail_relocations
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESP,UC_X86_REG_EIP

R,POOL,BUFFER=loading.R,loading.POOL,loading.BUFFER
HDR=loading.TREE+0x200
PRED=0x9d74e0


def providers():
    roots,addresses=loading.providers()
    roots.extend(unique.ROOTS)
    addresses.update(unique.ADDRESS)
    roots.append(dict(source=unique.F,symbol='?_M_decrement@_Rb_tree_base_iterator@_STL@@QAEXXZ',address=PRED))
    insert=next(name for name in unique.ADDRESS if name.startswith('?_M_insert@'))
    roots.append(dict(source=R/'rebuild/src/compiled/00/9d/STLAsyncBankTree_Minsert_009d7990.cpp',symbol=insert,address=0x9d7990))
    source=R/'rebuild/src/compiled/00/42/STL_Rb_global_Rebalance_0042971d.cpp'
    for method,address in (('_Rebalance',0x42971d),('_Rotate_left',0x42951b),('_Rotate_right',0x42955b)):
        symbol='?%s@?$_Rb_global@_N@_STL@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z'%method
        roots.append(dict(source=source,symbol=symbol,address=address));addresses[symbol]=address
    return roots,addresses


class TreeMachine(loading.LoadingMachine):
    def __init__(self,codes,multi,threshold,registered):
        super().__init__({a:data for a,data in codes.items() if a>>16!=0x42},multi,threshold,registered)
        self.u.mem_map(0x429000,0x1000);self.u.mem_write(0x429000,b'\xcc'*0x1000)
        for a,data in codes.items():
            if a>>16==0x42 or a==loading.bank.INSERT:self.u.mem_write(a,data)
        self.put(HDR,0);self.put(HDR+4,0);self.put(HDR+8,HDR);self.put(HDR+12,HDR)
        self.put(loading.BANK+0x140,loading.OBJECTS+3*0x48)

    def hook(self,u,address,size,user):
        if address==0xbfea0e:
            sp=u.reg_read(UC_X86_REG_ESP)
            if self.get(sp+4)==20:
                result=self.next_block;self.next_block+=0x1000
                assert self.next_block<=0x2040000
                self.blocks[result]=20;self.trace.append(('malloc',20,result))
                u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ECX,0xdeadbeef);u.reg_write(UC_X86_REG_EDX,0x87654321)
                u.reg_write(UC_X86_REG_EIP,self.get(sp));u.reg_write(UC_X86_REG_ESP,sp+4)
                return
        if address==loading.bank.INSERT:
            self.executed.add(address)
            sp=u.reg_read(UC_X86_REG_ESP)
            assert u.reg_read(UC_X86_REG_ECX)==loading.BANK+0x134
            value=self.get(self.get(sp+8))
            assert loading.OBJECTS<=value<loading.OBJECTS+8*0x48
            assert self.get(value+0x20)==0xffffffff and self.get(value+8)==0 and self.get(value+12)==0
            assert u.mem_read(value+0x44,1)==b'\0'
            fresh=value not in self.restarted
            if fresh:self.restarted.append(value)
            self.trace.append(('restart_insert',(value-loading.OBJECTS)//0x48,fresh))
            return  # Execute actual insert_unique; no insertion substitute.
        super().hook(u,address,size,user)

    def validate_tree(self):
        visited=set()
        def walk(node,parent):
            if not node:return [],1
            assert node not in visited and self.blocks.get(node)==20
            visited.add(node);assert self.get(node+4)==parent
            color=self.u.mem_read(node,1)[0];assert color in (0,1)
            left,right=self.get(node+8),self.get(node+12)
            if not color:
                assert not left or self.u.mem_read(left,1)[0]==1
                assert not right or self.u.mem_read(right,1)[0]==1
            lo,lb=walk(left,node);hi,hb=walk(right,node);assert lb==hb
            return lo+[self.get(node+16)]+hi,lb+color
        root=self.get(HDR+4)
        values,_=walk(root,HDR)
        assert values==sorted(self.restarted)
        assert self.get(loading.BANK+0x138)==len(values)
        if root:
            assert self.u.mem_read(root,1)[0]==1
            assert self.get(self.get(HDR+8)+16)==values[0]
            assert self.get(self.get(HDR+12)+16)==values[-1]
        else:assert self.get(HDR+8)==HDR and self.get(HDR+12)==HDR
        table=self.get(POOL+0x438)
        assert set(self.blocks)==visited|({table} if table in self.blocks else set())

    def call(self,address,args,this=POOL):
        # The middle-cursor composition performs multiple complete 256-bucket
        # searches. Its saved original/candidate 20k stops were finite scans.
        # Keep a fixed larger bound for this expanded composition only.
        u=self.u;sp=loading.bank.STACK+0x8000
        self.put(sp,loading.bank.STOP)
        for i,arg in enumerate(args):self.put(sp+4+4*i,arg)
        saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
        for i,reg in enumerate(saved):u.reg_write(reg,0x55660000+i)
        u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,this)
        u.emu_start(address,loading.bank.STOP,count=100000)
        assert u.reg_read(UC_X86_REG_EIP)==loading.bank.STOP,hex(address)
        assert u.reg_read(UC_X86_REG_ESP)==sp+4+4*len(args),hex(address)
        assert all(u.reg_read(reg)==0x55660000+i for i,reg in enumerate(saved))
        assert self.depth==0
        result=u.reg_read(UC_X86_REG_EAX)
        self.validate_tree()
        return result


def ordered_case(codes,multi):
    m=TreeMachine(codes,multi,128,True)
    assert m.call(0xa01d50,[BUFFER,512,0,0])==BUFFER
    m.call(0xa01da0,[BUFFER]);snapshots=[]
    for index in range(8):
        obj=loading.OBJECTS+index*0x48
        assert m.call(0xa62e40,[0x2003000,0,32,0])&255
        pointer=m.call(0xa026b0,[index])
        m.put(obj+8,pointer);m.put(obj+12,pointer);m.put(obj+0x1c,POOL);m.put(obj+0x20,index)
        m.u.mem_write(obj+0x44,b'\1\0\1\0')
    for index in (4,6,0,2,5,7,1,3):
        m.put(loading.BANK+0x140,loading.OBJECTS+index*0x48-1)
        assert m.call(loading.bank.ENTRY,[],loading.BANK)&255
        snapshots.append(m.snapshot())
    for index in (0,7,4,2,5,1,6,3):
        value,result=0x200d000,0x200d010
        m.put(value,loading.OBJECTS+index*0x48)
        before=set(m.blocks)
        assert m.call(loading.bank.INSERT,[result,value],loading.BANK+0x134)==result
        assert m.u.mem_read(result+4,1)==b'\0'
        assert m.get(m.get(result)+16)==loading.OBJECTS+index*0x48
        assert set(m.blocks)==before
        snapshots.append(m.snapshot())
    assert PRED in m.executed
    return snapshots,m.executed


def compare_insert_unique():
    roots,addresses=providers()
    inputs={Path(__file__),Path(loading.__file__),*(root['source'] for root in roots)}
    pins={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    with tempfile.TemporaryDirectory(prefix='async-bank-insert-unique-') as tmp:
        candidate=compile_codes(roots,addresses,Path(tmp))
    raw=EXE.read_bytes()
    native={a:read_virtual_data(raw,a,314 if a==loading.bank.ENTRY else 79 if a==PRED else len(data)) for a,data in candidate.items()}
    for multi in (False,True):
        actual,seen=ordered_case(candidate,multi);original,_=ordered_case(native,multi)
        assert actual==original and PRED in seen
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==digest for p,digest in pins.items())
    return dict(status='PASS',cases=2,insert_calls=32,input_sha256=pins,
                boundary='Actual node insertion/rotations/rebalance with CRT/Win32 controlled; predecessor helper behavior-only, no folded-clone grade claim.')


def main():
    roots,addresses=providers()
    inputs={Path(__file__),Path(loading.__file__),Path(unique.__file__),Path(loading.partial.__file__),
            Path(loading.bank.__file__),R/'tools/decomp_pipeline/movable_pool_construct_oracle.py',EXE}
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
    with tempfile.TemporaryDirectory(prefix='loading-restart-tree-') as tmp:
        candidate=compile_codes(roots,addresses,Path(tmp))
    raw=EXE.read_bytes()
    native={a:read_virtual_data(raw,a,314 if a==loading.bank.ENTRY else 79 if a==PRED else len(data)) for a,data in candidate.items()}
    for a,data in candidate.items():
        if a not in (0xa02b30,loading.bank.ENTRY,PRED):assert data==native[a],hex(a)
    pred_score=compare(candidate[PRED],native[PRED]).to_dict()
    cases=checkpoints=0;witnesses={}
    def run(codes,args):
        if args[0]=='ordered':
            state,seen=ordered_case(codes,args[1]);return state,seen,{}
        return loading.scenario(codes,*args,machine_type=TreeMachine)
    matrix=list(itertools.product((0,3,85,255),(False,True),(33,96,513),(0,2),(0,128),(False,True),(False,True)))
    matrix.extend([('ordered',False),('ordered',True)])
    for args in matrix:
        try:
            actual,seen,stats=run(candidate,args);original,_,original_stats=run(native,args)
            assert actual==original and stats==original_stats
        except Exception as error:raise AssertionError(args) from error
        cases+=1;checkpoints+=len(actual)
        for root in roots:
            if root['address'] in seen:witnesses.setdefault(root['address'],args)
        if cases%64==0:print('restart tree progress cases=%d'%cases,flush=True)
    omissions=[]
    for root in roots:
        assert root['address'] in witnesses,('unexercised',root['symbol'])
        reduced={a:data for a,data in candidate.items() if a!=root['address']};detected=False
        try:run(reduced,witnesses[root['address']])
        except Exception:detected=True
        assert detected,root['symbol'];omissions.append(root['symbol'])
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==digest for p,digest in pins.items())
    report=dict(status='PASS',cases=cases,checkpoints=checkpoints,providers=len(roots),missing_provider_checks=omissions,
                predecessor_grade='HELPER_BEHAVIOR_ONLY',predecessor_similarity=pred_score,input_sha256=pins,
                boundaries=['Win32 locks and CRT allocation/memmove controlled; restart-tree insertion and balancing are genuine.',
                            'Predecessor helper is behavior-only; no new grade/ownership claim for its folded clone.',
                            'No restarted disk reads or concurrent-thread claim.'])
    (R/'rebuild/compile-gate/native-loading-restart-tree.json').write_text(json.dumps(report,indent=2)+'\n')
    print('NATIVE_LOADING_RESTART_TREE_PASS cases=%d checkpoints=%d providers=%d omissions=%d'%(cases,checkpoints,len(roots),len(omissions)))


if __name__=='__main__':main()
