"""Retail fixed allocation/free sequences with independent membership/state model."""
import itertools,struct
from unicorn import Uc,UcError,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_INVALID
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EAX,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
from oracle_fixed_chunk_lists import BASE,WIDTH

def collect():
    uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000);raw=EXE.read_bytes()
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000;rows=[];events=[];new_raw=0;request=0;free_raw=0;failure=False;fault=0
    def read(a):return struct.unpack('<I',bytes(uc.mem_read(a,4)))[0]
    def word(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
    heap=0x12345678
    def hook(uc,address,size,user):
        if address not in [0x200d000,0x200d100,0x200d200,0x200d300]:return
        kind={0x200d000:1,0x200d100:2,0x200d200:3,0x200d300:4}[address];sp=uc.reg_read(UC_X86_REG_ESP)
        values=[0,0,0]if kind==1 else[read(sp+4),read(sp+8),read(sp+12)]
        events.append((kind,*values,bytes(uc.mem_read(BASE,WIDTH))))
        if kind!=1:assert values==[heap,1,request if kind==2 else new_raw if kind==3 else free_raw],(kind,values,new_raw,free_raw)
        result={1:heap,2:new_raw,3:0 if failure else request+16,4:1}[kind]
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+(4 if kind==1 else 16))
    uc.hook_add(UC_HOOK_CODE,hook)
    def invalid(uc,access,address,size,value,user):
        nonlocal fault
        fault=address;return False
    uc.hook_add(UC_HOOK_MEM_INVALID,invalid)
    for slot,target in [(0x143fd3c,0x200d000),(0x143fe88,0x200d100),(0x143fe84,0x200d200),(0x143fe8c,0x200d300)]:word(slot,target)
    word(0x13caa90,0x2007b00)
    for sequence,(alignment,count,nchunks,stride)in enumerate(itertools.product([4,8,16],[1,3,7],[0,1,3],[16,64])):
        state=bytearray(b'\xa5'*WIDTH);chunks={};chain=[];new_index=0;header=(28+alignment-1)//alignment*alignment-4
        def put(a,v):struct.pack_into('<I',state,a-BASE,v&0xffffffff)
        def get(a):return struct.unpack_from('<I',state,a-BASE)[0]
        def byte(a,v):state[a-BASE]=v&255
        def area(chunk,index):return chunk+header+index*stride
        def initialize(memory):
            chunk=(memory+alignment-1)//alignment*alignment;chunks[chunk]=dict(raw=memory,free=list(reversed(range(count))),used=[])
            put(chunk,memory)
            for off in [4,8,12,16,20]:put(chunk+off,0)
            put(chunk+16,area(chunk,count-1))
            for i in range(count):
                node=area(chunk,i);byte(node,i);byte(node+1,i-1 if i else 255);byte(node+2,i+1 if i+1<count else 255);byte(node+3,0)
            return chunk
        def unlink(chunk):
            position=chain.index(chunk);previous=chain[position-1]if position else 0;next_=chain[position+1]if position+1<len(chain)else 0
            if next_:put(next_+12,previous)
            if previous:put(previous+8,next_)
            chain.remove(chunk);put(BASE+0xc1c,chain[0]if chain else 0);put(BASE+0xc20,chain[-1]if chain else 0)
        def head(chunk):
            old=chain[0]if chain else 0;put(chunk+8,old);put(chunk+12,0)
            if old:put(old+12,chunk)
            else:put(BASE+0xc20,chunk)
            chain.insert(0,chunk);put(BASE+0xc1c,chunk)
        def tail(chunk):
            old=chain[-1]if chain else 0;put(chunk+12,old);put(chunk+8,0)
            if old:put(old+8,chunk)
            else:put(BASE+0xc1c,chunk)
            chain.append(chunk);put(BASE+0xc20,chunk)
        for off,value in [(0xc14,alignment),(0xc18,{4:2,8:3,16:4}[alignment]),(0xc1c,0),(0xc20,0),(0xc24,0),(0xc2c,stride),(0xc30,header),(0xc34,count),(0xc38,header+count*stride),(0xc48,nchunks)]:put(BASE+off,value)
        for i in range(nchunks):tail(initialize(0x2005000+i*0x1000))
        if sequence%2:chain.reverse();put(BASE+0xc1c,chain[0]if chain else 0);put(BASE+0xc20,chain[-1]if chain else 0)
        for i,chunk in enumerate(chain):put(chunk+8,chain[i+1]if i+1<len(chain)else 0);put(chunk+12,chain[i-1]if i else 0)
        put(BASE+0xc24,(chain[0]if sequence%3==1 else chain[-1])if chain and sequence%3 else 0)
        def verify_membership():
            assert get(BASE+0xc1c)==(chain[0]if chain else 0)and get(BASE+0xc20)==(chain[-1]if chain else 0)
            for position,chunk in enumerate(chain):
                assert get(chunk+8)==(chain[position+1]if position+1<len(chain)else 0)and get(chunk+12)==(chain[position-1]if position else 0)
                model=chunks[chunk];assert get(chunk+4)==len(model['used'])and sorted(model['used']+model['free'])==list(range(count))
                for name,off in [('used',20),('free',16)]:
                    order=model[name];assert get(chunk+off)==(area(chunk,order[0])if order else 0)
                    for j,index in enumerate(order):
                        node=area(chunk,index);assert state[node-BASE]==index
                        assert state[node-BASE+1]==(order[j+1]if j+1<len(order)else 255)
                        # Retail leaves the free head's previous ID stale after pop/prepend.
                        if j or name=='used':assert state[node-BASE+2]==(order[j-1]if j else 255)
        def invoke(mode,param):
            nonlocal events,new_raw,request,free_raw,new_index
            initial=bytes(state);new_raw=free_raw=0;request=0;expected_calls=[];result=0
            if mode==0:
                current=get(BASE+0xc24)
                if not current or not chunks[current]['free']:
                    current=chain[0]if chain else 0;put(BASE+0xc24,current)
                    if not current or not chunks[current]['free']:
                        new_raw=0x2008003+new_index*0x800;new_index+=1;request=header+count*stride+((alignment-1)&~7)
                        current=initialize(new_raw);head(current);put(BASE+0xc48,get(BASE+0xc48)+1);put(BASE+0xc24,current);expected_calls=[1,2,1,3]
                model=chunks[current];index=model['free'].pop(0);node=area(current,index);old=model['used'][:];model['used'].insert(0,index)
                put(current+16,area(current,model['free'][0])if model['free']else 0)
                byte(node+1,old[0]if old else 255);byte(node+2,255)
                if old:byte(area(current,old[0])+2,index)
                put(current+20,node);put(current+4,len(model['used']))
                if not old:put(BASE+0xc48,get(BASE+0xc48)-1)
                if not model['free']:unlink(current);tail(current)
                put(BASE+0xc24,chain[0]);result=node+4
            else:
                node=param-4;index=state[node-BASE];chunk=node-index*stride-header;model=chunks[chunk];position=model['used'].index(index)
                previous=model['used'][position-1]if position else 255;next_=model['used'][position+1]if position+1<len(model['used'])else 255
                if previous!=255:byte(area(chunk,previous)+1,next_)
                else:put(chunk+20,area(chunk,next_)if next_!=255 else 0)
                if next_!=255:byte(area(chunk,next_)+2,previous)
                old=model['free'][:]
                if old:byte(area(chunk,old[0])+2,index)
                byte(node+1,old[0]if old else 255);model['used'].remove(index);model['free'].insert(0,index);put(chunk+16,node);put(chunk+4,len(model['used']))
                if len(model['free'])==1 and chain[0]!=chunk:unlink(chunk);head(chunk)
                if not model['used']:
                    empty=(get(BASE+0xc48)+1)&0xffffffff;put(BASE+0xc48,empty)
                    if 1<empty<0x80000000:
                        free_raw=model['raw'];expected_calls=[1,4]
                        if get(BASE+0xc24)==chunk:put(BASE+0xc24,0)
                        unlink(chunk)
            verify_membership();expected=bytes(state);uc.mem_write(BASE,initial);events=[]
            uc.mem_write(stack,struct.pack('<2I',stop,param));uc.reg_write(UC_X86_REG_ECX,BASE);uc.reg_write(UC_X86_REG_ESP,stack)
            uc.emu_start(0xa66450 if mode==0 else 0xa662a0,stop,count=10000)
            assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+8
            if mode==0:assert uc.reg_read(UC_X86_REG_EAX)==result
            actual=bytes(uc.mem_read(BASE,WIDTH));assert actual==expected,(sequence,mode,param,next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),None))
            assert [e[0]for e in events]==expected_calls
            rows.append(dict(args=(sequence,mode,param,result,new_raw,request,free_raw,len(events)),initial=initial,after=expected,events=events[:]))
            return result
        for cycle in range(2):
            live=[invoke(0,[0,1,0xffffffff][(i+cycle)%3])for i in range(max(1,nchunks)*count+count+1 if cycle==0 else count+1)]
            order=(sequence+cycle)%3
            if order==1:live.reverse()
            elif order==2:live=live[::2]+live[1::2]
            for pointer in live:invoke(1,pointer)
    # Exceptional original semantics: failed NewChunk with an absent head faults,
    # but a surviving full head returns null via its missing free-list head.
    for alignment,full_head,kind in itertools.product([4,8,16],[False,True],[0,1,2]):
        initial=bytearray(b'\xa5'*WIDTH);chunk=0x2005000
        def put_initial(a,v):struct.pack_into('<I',initial,a-BASE,v&0xffffffff)
        size=0xffffffff if kind==2 else 256;extra=(alignment-1)&~7
        # Overflow requires slack; align4/8 have zero slack, so use zero alignment
        # in these direct raw-context cases to reach the same unsigned64 guard.
        actual_alignment=alignment if kind!=2 or alignment==16 else 0
        for off,value in [(0xc14,actual_alignment),(0xc18,{4:2,8:3,16:4}[alignment]),(0xc1c,chunk if full_head else 0),(0xc20,chunk if full_head else 0),(0xc24,0),(0xc2c,32),(0xc30,28),(0xc34,1),(0xc38,size),(0xc48,0)]:put_initial(BASE+off,value)
        if full_head:
            for off,value in [(0,chunk),(4,1),(8,0),(12,0),(16,0),(20,chunk+28)]:put_initial(chunk+off,value)
            initial[chunk+28-BASE:chunk+32-BASE]=bytes([0,255,255,0])
        expected=bytearray(initial);struct.pack_into('<I',expected,0xc24,chunk if full_head else 0)
        new_raw=0x2008003 if kind==1 else 0;request=size+((actual_alignment-1)&0xffffffff&~7);free_raw=0;failure=True;fault=0;events=[]
        uc.mem_write(BASE,bytes(initial));uc.mem_write(stack,struct.pack('<2I',stop,0xffffffff));uc.reg_write(UC_X86_REG_ECX,BASE);uc.reg_write(UC_X86_REG_ESP,stack)
        try:uc.emu_start(0xa66450,stop,count=10000)
        except UcError:assert not full_head and fault==16
        if full_head:assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_EAX)==0 and uc.reg_read(UC_X86_REG_ESP)==stack+8
        else:assert fault==16
        assert bytes(uc.mem_read(BASE,WIDTH))==expected
        assert [e[0]for e in events]==([]if kind==2 else[1,2,1,3]if kind==1 else[1,2])
        rows.append(dict(args=(54+len(rows),3 if full_head else 2,0xffffffff,0 if full_head else 16,new_raw,request&0xffffffff,0,len(events)),initial=bytes(initial),after=bytes(expected),events=events[:]))
    failure=False
    for mode,empty in itertools.product([0,1],[0xfffffffd,0,1,7,0x7fffffff,0xffffffff]):
        initial=bytearray(b'\xa5'*WIDTH);chunk=0x2005000;node=chunk+28
        def put_target(state,a,v):struct.pack_into('<I',state,a-BASE,v&0xffffffff)
        for off,value in [(0xc14,16),(0xc18,4),(0xc1c,chunk),(0xc20,chunk),(0xc24,chunk),(0xc2c,32),(0xc30,28),(0xc34,1),(0xc38,60),(0xc48,empty)]:put_target(initial,BASE+off,value)
        for off,value in [(0,chunk-3),(4,mode),(8,0),(12,0),(16,0 if mode else node),(20,node if mode else 0)]:put_target(initial,chunk+off,value)
        initial[node-BASE:node-BASE+4]=bytes([0,255,255,0]);expected=bytearray(initial)
        put_target(expected,chunk+4,0 if mode else 1);put_target(expected,chunk+16,node if mode else 0);put_target(expected,chunk+20,0 if mode else node)
        changed=(empty+(1 if mode else -1))&0xffffffff;put_target(expected,BASE+0xc48,changed)
        deleted=mode and 1<changed<0x80000000
        if deleted:
            for off in [0xc1c,0xc20,0xc24]:put_target(expected,BASE+off,0)
        free_raw=chunk-3 if deleted else 0;new_raw=request=0;events=[]
        param=node+4 if mode else 0xffffffff
        uc.mem_write(BASE,bytes(initial));uc.mem_write(stack,struct.pack('<2I',stop,param));uc.reg_write(UC_X86_REG_ECX,BASE);uc.reg_write(UC_X86_REG_ESP,stack)
        uc.emu_start(0xa662a0 if mode else 0xa66450,stop,count=10000)
        assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+8
        if not mode:assert uc.reg_read(UC_X86_REG_EAX)==node+4
        assert bytes(uc.mem_read(BASE,WIDTH))==expected
        assert [e[0]for e in events]==([1,4]if deleted else[])
        rows.append(dict(args=(54+len(rows),mode,param,0 if mode else node+4,0,0,free_raw,len(events)),initial=bytes(initial),after=bytes(expected),events=events[:]))
    return rows
