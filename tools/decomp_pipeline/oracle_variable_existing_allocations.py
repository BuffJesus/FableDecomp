"""Retail best-fit allocation with real splitting, lists and tree mutations."""
import random,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EAX,UC_X86_REG_EIP,UC_X86_REG_FPCW,UC_X86_REG_FPSW,UC_X86_REG_FPTAG
from pe_oracle import EXE,pe_sections
from oracle_variable_tree_insertion import validate,BASE,WIDTH,OWNER
CHUNK=0x2008000;AREAS=0x2009000

def collect(free_cycles=False,size_queries=False,largest_free=False,fragmentation=False,enumeration=False,validation=False):
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000;rows=[];coverage=set()
    uc.hook_add(UC_HOOK_CODE,lambda uc,address,size,user:coverage.add(address))
    def word(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
    def read(a):return struct.unpack('<I',bytes(uc.mem_read(a,4)))[0]
    def invoke(entry,this,args):
        uc.mem_write(stack,struct.pack('<'+str(len(args)+1)+'I',stop,*(v&0xffffffff for v in args)))
        uc.reg_write(UC_X86_REG_ECX,this);uc.reg_write(UC_X86_REG_ESP,stack);uc.emu_start(entry,stop,count=30000)
        assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+4*(len(args)+1)
        return uc.reg_read(UC_X86_REG_EAX)
    def fragmentation_query(free,after,alignment,shift):
        if not fragmentation:return 0
        uc.reg_write(UC_X86_REG_FPCW,0x27f);uc.reg_write(UC_X86_REG_FPSW,0);uc.reg_write(UC_X86_REG_FPTAG,0xffff)
        invoke(0xa64590,OWNER,[])
        # Test-only float32 store of the retail x87 return value.
        uc.mem_write(stop,b'\xd9\x1d'+struct.pack('<I',0x200e000))
        uc.emu_start(stop,stop+6,count=1)
        bits=read(0x200e000)
        assert bits==(0 if len(free)<=1 else 0x7f800000)
        assert bytes(uc.mem_read(BASE,WIDTH))==after
        assert (read(0x13d28d4),read(0x13d28d8))==(alignment,shift)
        return bits
    def enumeration_query(allocated,after,alignment,shift):
        if not enumeration:return []
        pointers=[area+32 for area in allocated]
        output=0x200d000;buffer=bytearray(b'\xcc'*256)
        assert len(pointers)<64
        uc.mem_write(output,bytes(buffer))
        for i,pointer in enumerate(pointers):struct.pack_into('<I',buffer,i*4,pointer)
        assert invoke(0xa65260,OWNER,[output,0])==len(pointers)
        assert bytes(uc.mem_read(output,256))==buffer
        assert bytes(uc.mem_read(BASE,WIDTH))==after
        for index in list(range(len(pointers)))+[-1,len(pointers),len(pointers)+1,0x7fffffff]:
            assert invoke(0xa652d0,OWNER,[index])==(pointers[index]if 0<=index<len(pointers)else 0)
            assert bytes(uc.mem_read(BASE,WIDTH))==after
            assert (read(0x13d28d4),read(0x13d28d8))==(alignment,shift)
        return pointers
    def validation_query(after,alignment,shift):
        if not validation:return
        invoke(0xa65330,OWNER,[])
        assert bytes(uc.mem_read(BASE,WIDTH))==after
        assert (read(0x13d28d4),read(0x13d28d8),read(0x13caa90))==(alignment,shift,0x200d800)
    def lists():
        def chain(head):
            result=[];prev=0;a=read(head)
            while a:
                assert a not in result and BASE<=a<BASE+WIDTH-32 and read(a+8)==prev and read(a+0x14)==CHUNK
                result.append(a);prev=a;a=read(a)
            return result
        free=chain(CHUNK+0x28);allocated=chain(CHUNK+0x30)
        assert not set(free)&set(allocated)
        sizes=[read(a+0x18)for a in free];assert sizes==sorted(sizes)
        assert read(CHUNK+4)==len(free)and read(CHUNK+8)==len(allocated)
        assert read(CHUNK+0xc)==sum(sizes)
        assert read(CHUNK+0x2c)==(free[-1]if free else 0)
        assert read(CHUNK+0x10)==(sizes[-1]if sizes else 0)and read(CHUNK+0x14)==(sizes[0]if sizes else 0)
        if free:assert validate(uc.mem_read)==len(free)
        else:assert read(OWNER+0xc34)==0
        pending=[read(OWNER+0xc34)];tree=set()
        while pending:
            a=pending.pop()
            if not a:continue
            assert a not in tree;tree.add(a);pending.extend([read(a+4),read(a+0x10)])
        assert tree==set(free)
        return free,allocated
    requests=[
        [0,1,31,32,33,63,64,65,95,96,127,128,129,255,256,511,512,1024],
        [768,512,256,128,64,32,16,1,0,4096],
        [0x7fffffff,0x80000000,0xffffffff,32,64,128,256,512,768,2048],
        [96,480,224,736,32,64,128,256,512,1,0,4096]]
    for alignment in [4,8,16,32]:
      for pattern in [0x5a,0xa5]:
       for order_mode in [0,1,2]:
        for sizes in [[32,64,128,256,512,768,128],[64,64,64,128,128,256,512]]:
         for sequence in requests:
            uc.mem_write(BASE,bytes([pattern])*WIDTH);shift={4:2,8:3,16:4,32:5}[alignment]
            word(0x13d28d4,alignment);word(0x13d28d8,shift);word(OWNER+0xc14,alignment);word(OWNER+0xc18,shift)
            word(OWNER+0xc34,0);word(OWNER+0xc1c,CHUNK)
            if validation:word(0x13caa90,0x200d800)
            if fragmentation:
                word(OWNER,0x200b000);word(0x200b020,0xa64530);word(0x200b024,0xa64560)
            for off in [4,8,0xc,0x10,0x14,0x20,0x24,0x28,0x2c,0x30]:word(CHUNK+off,0)
            word(CHUNK+0x18,OWNER);word(CHUNK,sum(sizes)+32*len(sizes))
            for index,size in enumerate(sizes):
                area=AREAS+index*0x400
                for off in [0,4,8,0xc,0x10]:word(area+off,0)
                word(area+0x14,CHUNK);word(area+0x18,size);uc.mem_write(area+0x1c,b'\0')
            order=list(range(len(sizes)))
            if order_mode==1:order.reverse()
            elif order_mode==2:random.Random(97).shuffle(order)
            for index in order:invoke(0xa650f0,CHUNK,[AREAS+index*0x400])
            lists();initial=bytes(uc.mem_read(BASE,WIDTH));steps=[]
            for request in sequence:
                free,allocated=lists();before=bytes(uc.mem_read(BASE,WIDTH))
                aligned=((request+alignment-1)&0xffffffff)>>shift<<shift
                signed=aligned if aligned<0x80000000 else aligned-0x100000000
                eligible=[a for a in free if read(a+0x18)>=signed]
                smallest=min((read(a+0x18)for a in eligible),default=0)
                choices=[a for a in eligible if read(a+0x18)==smallest]
                result=invoke(0xa656e0,OWNER,[request])
                after=bytes(uc.mem_read(BASE,WIDTH));new_free,new_allocated=lists()
                overflow=((aligned+32)&0xffffffff)>=0x80000000
                if not eligible or overflow:assert result==0 and after==before
                else:
                    expected=[a+smallest-aligned if smallest>aligned+32 else a for a in choices]
                    assert result in expected and new_allocated==[result]+allocated,(alignment,order_mode,sizes,request,aligned,hex(result),[(hex(a),read(a+0x18))for a in choices],list(map(hex,expected)))
                    assert read(result+0x18)==(aligned if smallest>aligned+32 else smallest)
                    assert len(new_free)==len(free)-(smallest<=aligned+32)
                    assert after[result-BASE+0x1d]==before[result-BASE+0x1d]
                # Every split exchanges 32 data bytes for one additional header.
                nodes=new_free+new_allocated
                assert sum(read(a+0x18)for a in nodes)+32*(len(nodes)-len(sizes))==sum(sizes)
                intervals=sorted((a,a+32+read(a+0x18))for a in nodes)
                assert all(end<=next_start for (start,end),(next_start,next_end)in zip(intervals,intervals[1:]))
                queries=(0,0)
                if size_queries and result:
                    data=result+32
                    queries=(invoke(0xa651b0,OWNER,[data]),invoke(0xa651e0,OWNER,[data]))
                    assert queries==(read(result+0x18),(read(result+0x18)+32)&0xffffffff)
                    assert bytes(uc.mem_read(BASE,WIDTH))==after
                    assert (read(0x13d28d4),read(0x13d28d8))==(alignment,shift)
                largest=0
                if largest_free:
                    largest=invoke(0xa64500,OWNER,[])
                    assert largest==max((read(a+0x18)for a in new_free),default=0)
                    assert bytes(uc.mem_read(BASE,WIDTH))==after
                bits=fragmentation_query(new_free,after,alignment,shift)
                enumeration_pointers=enumeration_query(new_allocated,after,alignment,shift)
                validation_query(after,alignment,shift)
                steps.append(dict(mode=0,request=request,result=result,after=after,queries=queries,largest=largest,fragmentation=bits,enumeration=enumeration_pointers))
            if free_cycles:
                free,allocated=lists();release_order=sorted(allocated,reverse=order_mode==1)
                if order_mode==2:random.Random(13).shuffle(release_order)
                for area in release_order:
                    free,allocated=lists();assert area in allocated
                    invoke(0xa658e0,CHUNK,[area]);new_free,new_allocated=lists()
                    assert new_allocated==[a for a in allocated if a!=area]
                    nodes=new_free+new_allocated
                    assert sum(read(a+0x18)for a in nodes)+32*(len(nodes)-len(sizes))==sum(sizes)
                    intervals=sorted((a,a+32+read(a+0x18))for a in nodes)
                    assert all(end<=next_start for (start,end),(next_start,next_end)in zip(intervals,intervals[1:]))
                    after=bytes(uc.mem_read(BASE,WIDTH));largest=0
                    if largest_free:
                        largest=invoke(0xa64500,OWNER,[])
                        assert largest==max((read(a+0x18)for a in new_free),default=0)
                        assert bytes(uc.mem_read(BASE,WIDTH))==after
                    bits=fragmentation_query(new_free,after,alignment,shift)
                    enumeration_pointers=enumeration_query(new_allocated,after,alignment,shift)
                    validation_query(after,alignment,shift)
                    steps.append(dict(mode=1,request=area,result=0,after=after,largest=largest,fragmentation=bits,enumeration=enumeration_pointers))
                final_free,final_allocated=lists();assert not final_allocated
                assert {a:read(a+0x18)for a in final_free}=={AREAS+i*0x400:size for i,size in enumerate(sizes)}
            rows.append(dict(alignment=alignment,initial=initial,steps=steps))
    assert all(a in coverage for a in [0xa6497a,0xa64aa5,0xa654e0,0xa65560]),'missing real deletion/split/whole-area path'
    return rows

if __name__=='__main__':
    rows=collect();print('VARIABLE_EXISTING_ALLOCATIONS_RETAIL sequences=%d calls=%d'%(len(rows),sum(len(r['steps'])for r in rows)))
