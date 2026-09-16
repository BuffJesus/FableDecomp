"""Compare the structured talk body to original x86, including cancellation joins."""
import itertools
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_villager_talk_key import verify
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_ECX, UC_X86_REG_EDX, UC_X86_REG_EDI, UC_X86_REG_EAX, UC_X86_REG_ESI


def native(data, sex, hit, failures, active_frames, cancel, held):
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for address,size in ((0xDAD000,0x2000),(0x99E000,0x2000),(0xCD2000,0x1000),
                         (0xF35000,0x1000),(0x100000,0x10000),(0x200000,0x4000)):
        uc.mem_map(address,size)
    uc.mem_write(0xDADF80,data.bytes_at(0xDADF80,2787))
    def put(address,value):uc.mem_write(address,(value&0xFFFFFFFF).to_bytes(4,'little'))
    def get(address):return int.from_bytes(uc.mem_read(address,4),'little')
    stack,actor,thread,game,table=0x108000,0x200000,0x200100,0x200200,0x201000
    put(thread+4,game);uc.mem_write(thread+0x1c,bytes([hit]));put(game,table)
    slots={0x804:'sex',0x20:'acquire',0x1c:'frame',0x118:'hero',0x76c:'face',
           0x5b0:'conversation',0x5b4:'person',0x5b8:'line',0x5c0:'active'}
    calls={0x202000+i*16:name for i,name in enumerate(slots.values())}
    for (slot,_),target in zip(slots.items(),calls):put(table+slot,target)
    calls.update({0x99E4B0:'new',0x99EFE0:'assign',0x99EBF0:'prefix',0x99F570:'concat',
                  0x99EAE0:'destroy',0xCD23B9:'prepare',0xCD2770:'reset',0xF35B30:'term'})
    for reg,value in ((UC_X86_REG_ESP,stack),(UC_X86_REG_EDI,actor),(UC_X86_REG_ESI,thread)):
        uc.reg_write(reg,value)
    events=[];counts={'term':0,'acquire':0,'active':0};live=set();finished=[]
    def hook(machine,address,size,user):
        if address in (0xDAE607,0xDAEA49):finished.append(address==0xDAE607);machine.emu_stop();return
        if address not in calls:return
        name=calls[address];esp=machine.reg_read(UC_X86_REG_ESP);receiver=machine.reg_read(UC_X86_REG_ECX)
        args=lambda n:[get(esp+4+i*4) for i in range(n)]
        pop=0;result=0xBADBAD00
        if name in slots.values():assert receiver==game
        if name=='new':assert receiver==stack+24;live.add(receiver);events.append('new');result=receiver
        elif name=='sex':assert args(1)==[actor];pop=4;result=sex;events.append('sex')
        elif name=='term':
            assert receiver==thread;counts[name]+=1;result=int(counts[name]==cancel);events.append('term')
        elif name=='assign':
            assert receiver==stack+24 and args(1)==[0x12D86E4 if sex==1 else 0x12D86DC]
            pop=4;result=receiver;events.append('male' if sex==1 else 'female')
        elif name=='prepare':assert receiver==stack+92;result=int(held);events.append('prepare')
        elif name=='reset':assert receiver==stack+92;events.append('reset')
        elif name=='acquire':
            assert args(3)==[actor,stack+92,4];pop=12;counts[name]+=1;result=int(counts[name]>failures);events.append('acquire')
        elif name=='frame':events.append('frame')
        elif name=='hero':events.append('hero');result=0x203000
        elif name=='face':assert args(3)==[actor,0x203000,0];pop=12;events.append('face')
        elif name=='conversation':assert args(3)==[actor,0,0];pop=12;result=73;events.append('conversation')
        elif name=='person':assert args(2)==[73,0x203000];pop=8;events.append('person')
        elif name=='prefix':
            assert args(2)==[0x12D86B4 if hit else 0x12D8694,0xFFFFFFFF]
            live.add(receiver);pop=8;result=receiver;events.append('prefix')
        elif name=='concat':
            assert args(1)==[stack+24] and machine.reg_read(UC_X86_REG_EDX) in live
            live.add(receiver);pop=4;result=receiver;events.append('concat')
        elif name=='line':assert args(5)==[73,stack+(52 if hit else 60),0,actor,0x203000];pop=20;events.append('line')
        elif name=='destroy':
            assert receiver in live;live.remove(receiver)
            events.append('destroy' if receiver==stack+24 else 'result.destroy' if receiver==stack+(52 if hit else 60) else 'prefix.destroy')
        elif name=='active':
            assert args(1)==[73];pop=4;counts[name]+=1;result=int(counts[name]<=active_frames);events.append('active')
        machine.reg_write(UC_X86_REG_EAX,result);machine.reg_write(UC_X86_REG_ESP,esp+4+pop);machine.reg_write(UC_X86_REG_EIP,get(esp))
    uc.hook_add(UC_HOOK_CODE,hook);uc.emu_start(0xDAE3F3,0xDAEA63,count=1000)
    assert len(finished)==1 and not live and uc.reg_read(UC_X86_REG_ESP)==stack
    return finished[0],events


def structured(sex,hit,failures,active_frames,cancel,held):
    lua=LuaRuntime(unpack_returned_tuples=True)
    lua.globals().body=lua.execute(Path(__file__).with_name('villager_talk_body.lua').read_text())
    lua.globals().config=lua.table_from(dict(sex=sex,hit=hit,failures=failures,active=active_frames,cancel=cancel,held=held))
    result,events=lua.execute('''
        local e,term,acquire,active={},0,0,0
        local function emit(v) e[#e+1]=v end
        local quest={}
        function quest:EntityGetSex(me) assert(me==9);emit('sex');return config.sex end
        function quest:IsActiveThreadTerminating() term=term+1;emit('term');return term==config.cancel end
        function quest:NewScriptFrame(me) assert(me==9);emit('frame') end
        function quest:IsConversationActive(id) assert(id==73);active=active+1;emit('active');return active<=config.active end
        local state={GetStateBool=function(_,key) assert(key=='HeroDidHitMe');return config.hit end}
        local resources={}
        function resources:NewText() emit('new');return 7 end
        function resources:AssignVillagerSuffix(id,male) assert(id==7);emit(male and 'male' or 'female') end
        function resources:PrepareResource(id) assert(id==5);emit('prepare');if config.held then emit('reset') end end
        function resources:TryAcquire(id,me,priority) assert(id==5 and me==9 and priority==4);acquire=acquire+1;emit('acquire');return acquire>config.failures end
        function resources:StartVillagerTalkConversation(me)
            assert(me==9);for _,v in ipairs({'hero','face','conversation','hero','person'}) do emit(v) end;return 73
        end
        function resources:AddVillagerTalkLine(id,suffix,me,hit)
            assert(id==73 and suffix==7 and me==9 and hit==config.hit)
            for _,v in ipairs({'hero','prefix','concat','line','result.destroy','prefix.destroy'}) do emit(v) end
        end
        function resources:DestroyText(id) assert(id==7);emit('destroy') end
        return body(quest,9,resources,5,state),e
    ''')
    return result,list(events.values())


class VillagerTalkBodyTests(unittest.TestCase):
    def test_structured_body_matches_native_joins_and_cancellation(self):
        data=RData();verify(data)
        for sex,hit,failures,frames,held in itertools.product((0,1,2),(False,True),(0,1,2),(0,1,2),(False,True)):
            for cancel in range(0,5+failures+frames):
                case=(sex,hit,failures,frames,cancel,held)
                with self.subTest(case=case):self.assertEqual(structured(*case),native(data,*case))

    def test_body_error_keeps_suffix_cleanup_and_original_error(self):
        lua=LuaRuntime(unpack_returned_tuples=True)
        lua.globals().body=lua.execute(Path(__file__).with_name('villager_talk_body.lua').read_text())
        lua.execute('''
            for _,cleanupFails in ipairs({false,true}) do
                local destroyed=0
                local resources={NewText=function() return 7 end,
                    DestroyText=function(_,id)
                        assert(id==7);destroyed=destroyed+1
                        if cleanupFails then error('CLEANUP') end
                    end}
                local quest={EntityGetSex=function() error('SEX') end}
                local ok,e=pcall(body,quest,9,resources,5,{})
                assert(not ok and string.find(e,'SEX',1,true) and destroyed==1)
            end
        ''')
