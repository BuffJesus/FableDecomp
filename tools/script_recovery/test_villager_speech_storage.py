"""Execute quest Init appends, including actual retail CString copy construction.

Literal construction/destruction and heap allocation are boundary doubles. The
real_growth cases execute the original native vector insertion helper as well.
"""
import hashlib
import unittest
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_speech_vectors import recover_vectors
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_ECX,UC_X86_REG_EAX


def run(data,capacity,repetitions,real_growth=False,teardown=False):
    expected=recover_vectors(data.bytes_at)
    copy=data.bytes_at(0x99EC30,61)
    assert hashlib.sha256(copy).hexdigest()=='a3acb13754a768da17ddb398c1ed55dcaeabd496848d6fd57bd892aedf2ccc86'
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for address,size in ((0xDAA000,0x3000),(0xDBE000,0x2000),(0xCBD000,0x1000),(0x99E000,0x2000),(0x433000,0x1000),(0x44B000,0x1000),
                         (0x143E000,0x1000),(0x13BD000,0x1000),(0x100000,0x10000),
                         (0x200000,0x10000),(0x300000,0x10000),(0x400000,0x10000),
                         (0x500000,0x10000),(0xBFE000,0x1000)):
        uc.mem_map(address,size)
    uc.mem_write(0xDAADD0,data.bytes_at(0xDAADD0,3306));uc.mem_write(0x99EC30,copy)
    wrapper=data.bytes_at(0x44B110,11)
    assert wrapper==bytes.fromhex('85c9740652e8163b5500c3')
    uc.mem_write(0x44B110,wrapper)
    if real_growth:
        growth=data.bytes_at(0x433530,316)
        assert hashlib.sha256(growth).hexdigest()=='1c48ffbb40875ecde275517336150895f5379a47d32e5e8ddf16499dab3a489c'
        uc.mem_write(0x433530,growth)
    if teardown:
        body=data.bytes_at(0xDBEFC0,450)
        assert hashlib.sha256(body).hexdigest()=='232d98f62015f85c1aeb9aef94203cc85704d4ef4db67fa9e45aa8a65b9454c9'
        uc.mem_write(0xDBEFC0,body)
    def put(address,value):uc.mem_write(address,(value&0xFFFFFFFF).to_bytes(4,'little'))
    def get(address):return int.from_bytes(uc.mem_read(address,4),'little')
    parent,timer,table,timer_call,sentinel,stack=0x200000,0x201000,0x202000,0x203000,0x203100,0x108000
    put(0x143E8F8,timer);put(timer,table);put(table+0x164,timer_call);put(parent+0x104,73)
    put(table+0x160,timer_call+16);put(parent+0x108,91)
    buffers={int(offset,0):0x400000+i*0x1000 for i,offset in enumerate(expected)}
    for offset,buffer in buffers.items():
        put(parent+offset,buffer if capacity else 0);put(parent+offset+4,buffer if capacity else 0)
        put(parent+offset+8,buffer+capacity*4 if capacity else 0)
    texts={};events=[];live=[];completed=[];allocated=set(buffers.values()) if capacity else set()
    heap=[0x500000]
    tearing_down=[False]
    def hook(machine,address,size,user):
        if address==0xCBD510:events.append(('base',parent));machine.emu_stop();return
        if address==sentinel:completed.append(1);machine.emu_stop();return
        if address not in (timer_call,timer_call+16,0x99EBF0,0x99EAE0,0x99EC30,0x433530,0xBFEA0E,0xBFEA14):return
        esp=machine.reg_read(UC_X86_REG_ESP);receiver=machine.reg_read(UC_X86_REG_ECX)
        args=lambda n:[get(esp+4+i*4) for i in range(n)]
        result=0xBADBAD00;pop=0
        if address==timer_call:assert receiver==timer and args(2)==[73,0];pop=8;events.append(('timer',0))
        elif address==timer_call+16:assert receiver==timer;events.append(('deregister',args(1)[0]));pop=4
        elif address==0x99EBF0:
            literal,length=args(2);assert length==0xFFFFFFFF and not live
            text=data.string_at(literal);assert text.startswith('TEXT_QST_048_VILLAGER_DONE_')
            allocation=0x300000+len(texts)*64;texts[allocation]=text
            put(allocation+13,1);put(receiver,allocation);live.append(receiver)
            events.append(('construct',text));pop=8;result=receiver
        elif address==0x99EC30:
            if not (real_growth and 0x433530<=get(esp)<0x43366c):
                assert args(1)==live
                offset=next(o for o in buffers if get(parent+o+4)==receiver)
                events.append(('copy',offset,texts[get(live[0])]))
            return  # Run the actual helper: output assignment and source refcount increment.
        elif address==0x433530:
            position,source,allocator,one,again=args(5);offset=receiver-parent
            assert offset in buffers and source==live[0] and one==again==1
            assert position==get(receiver+4) and position==get(receiver+8)
            if real_growth:
                events.append(('insert',offset,texts[get(source)]))
                return
            buffer=buffers[offset];old_start=get(receiver);count=(position-old_start)//4
            destination=buffer+count*4;allocation=get(source);put(destination,allocation)
            put(allocation+13,get(allocation+13)+1)
            put(receiver,buffer);put(receiver+4,destination+4);put(receiver+8,destination+4)
            events.append(('insert',offset,texts[allocation]));pop=20
        elif address==0x99EAE0:
            allocation=get(receiver)
            if tearing_down[0]:
                assert get(allocation+13)==1;put(allocation+13,0);events.append(('final.destroy',texts[allocation]))
            else:
                assert get(allocation+13)==2;put(allocation+13,1)
                if live==[receiver]:live.clear();events.append(('destroy',texts[allocation]))
                else:assert real_growth and 0x433530<=get(esp)<0x43366c
        elif address==0xBFEA0E:
            assert real_growth;size=args(1)[0];assert size>0 and size%4==0
            result=heap[0];heap[0]+=(size+15)&~15;assert heap[0]<0x510000;allocated.add(result)
        elif address==0xBFEA14:
            assert real_growth and args(1)[0] in allocated;allocated.remove(args(1)[0])
            if tearing_down[0]:events.append(('free',next(offset for offset in buffers if get(parent+offset)==args(1)[0])))
        machine.reg_write(UC_X86_REG_EAX,result);machine.reg_write(UC_X86_REG_ESP,esp+4+pop);machine.reg_write(UC_X86_REG_EIP,get(esp))
    uc.hook_add(UC_HOOK_CODE,hook)
    for _ in range(repetitions):
        put(stack,sentinel);uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,parent)
        try:uc.emu_start(0xDAADD0,0xDAC000,count=10000)
        except Exception as error:raise AssertionError((hex(uc.reg_read(UC_X86_REG_EIP)),events[-4:])) from error
        assert uc.reg_read(UC_X86_REG_ESP)==stack+4 and not live
    assert len(completed)==repetitions
    actual={hex(offset):[texts[get(p)] for p in range(get(parent+offset),get(parent+offset+4),4)] for offset in buffers}
    assert actual=={offset:keys*repetitions for offset,keys in expected.items()}
    assert all(get(allocation+13)==1 for allocation in texts)
    if real_growth:assert allocated=={get(parent+offset) for offset in buffers}
    if teardown:
        tearing_down[0]=True
        put(stack,sentinel);uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,parent)
        uc.emu_start(0xDBEFC0,0xDBF200,count=2000)
        assert uc.reg_read(UC_X86_REG_ESP)==stack and not allocated
        assert all(get(allocation+13)==0 for allocation in texts)
    return expected,events,get(0x13BD800)


class VillagerSpeechStorageTests(unittest.TestCase):
    def test_native_init_appends_and_retains_every_literal(self):
        data=RData()
        for capacity in (0,128):
            for repetitions in (1,2):
                vectors,events,copies=run(data,capacity,repetitions)
                expected=[]
                for _ in range(repetitions):
                    expected.append(('timer',0))
                    for offset,keys in vectors.items():
                        for key in keys:
                            expected.extend([('construct',key),('copy' if capacity else 'insert',int(offset,0),key),('destroy',key)])
                self.assertEqual(events,expected)
                self.assertEqual(copies,sum(map(len,vectors.values()))*repetitions if capacity else 0)

    def test_actual_growth_helper_relocates_owned_strings_and_frees_old_buffers(self):
        data=RData()
        for repetitions in (1,2):
            vectors,events,copies=run(data,0,repetitions,real_growth=True)
            self.assertEqual([e[1] for e in events if e[0]=='construct'],
                             [key for keys in vectors.values() for key in keys]*repetitions)
            self.assertEqual([e[1] for e in events if e[0]=='destroy'],
                             [key for keys in vectors.values() for key in keys]*repetitions)
            self.assertEqual({e[0] for e in events},{'timer','construct','copy','insert','destroy'})
            self.assertGreater(copies,42*repetitions)

    def test_complete_native_teardown_reverses_vectors_but_not_entries(self):
        vectors,events,_=run(RData(),0,2,real_growth=True,teardown=True)
        tail=events[events.index(('deregister',91)):]
        expected=[('deregister',91),('deregister',73)]
        for offset in sorted(vectors,key=lambda value:int(value,0),reverse=True):
            expected.extend(('final.destroy',text) for text in vectors[offset]*2)
            expected.append(('free',int(offset,0)))
        expected.append(('base',0x200000))
        self.assertEqual(tail,expected)
