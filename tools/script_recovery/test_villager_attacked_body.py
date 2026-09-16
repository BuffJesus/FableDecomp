"""Original movie/speech instructions versus the structured Villager helper."""
import itertools
import struct
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_villager_movie_scope import verify
from tools.script_recovery.native_villager_health_temporaries import verify as verify_health
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_ECX,UC_X86_REG_EDI,UC_X86_REG_EAX,UC_X86_REG_ESI


def native(data,sex,bits,frames,cancel):
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for address,size in ((0xDAE000,0x1000),(0x99A000,0x1000),(0x99E000,0x1000),
                         (0x6E7000,0x1000),(0x7E7000,0x1000),(0x4AA000,0x1000),
                         (0xF35000,0x1000),(0x122D000,0x1000),(0x100000,0x10000),(0x200000,0x4000)):
        uc.mem_map(address,size)
    uc.mem_write(0xDAE1AA,data.bytes_at(0xDAE1AA,0xDAEA49-0xDAE1AA))
    def put(address,value):uc.mem_write(address,(value&0xFFFFFFFF).to_bytes(4,'little'))
    def get(address):return int.from_bytes(uc.mem_read(address,4),'little')
    stack,actor,thread,game,table=0x108000,0x200000,0x200100,0x200200,0x201000
    put(thread+4,game);put(game,table);put(0x203000,bits);put(0x122DEDC,0)
    # The real health API returns ST0. Execute FLD/RET rather than replacing its value with a Python comparison.
    health=0x202F00;uc.mem_write(health,b'\xD9\x05'+struct.pack('<I',0x203000)+b'\xC2\x04\x00');put(table+0x420,health)
    slots={0x5c8:'start',0x5ec:'pause',0x804:'sex',0x118:'hero',0x1c:'frame'}
    calls={0x202000+i*16:name for i,name in enumerate(slots.values())}
    for (slot,_),target in zip(slots.items(),calls):put(table+slot,target)
    calls.update({0x99A380:'movie.new',0x99EBF0:'key.new',0x99EAE0:'key.destroy',
                  0x6E7B80:'movie.destroy',0xF35B30:'term',0x7E7490:'thing.new',
                  0x4AA840:'thing.destroy',0x7E7390:'speak',0x7E7450:'task',health:'health'})
    for reg,value in ((UC_X86_REG_ESP,stack),(UC_X86_REG_EDI,actor),(UC_X86_REG_ESI,thread)):uc.reg_write(reg,value)
    events=[];counts={'term':0,'task':0};finished=[];thing=stack+(68 if sex==1 else 80)
    def hook(machine,address,size,user):
        if address in (0xDAE3B0,0xDAEA49):finished.append(address==0xDAE3B0);machine.emu_stop();return
        if address not in calls:return
        name=calls[address];esp=machine.reg_read(UC_X86_REG_ESP);receiver=machine.reg_read(UC_X86_REG_ECX)
        args=lambda n:[get(esp+4+i*4) for i in range(n)]
        pop=0;result=0xBADBAD00
        if name in slots.values() or name=='health':assert receiver==game
        if name=='movie.new':assert receiver==stack+108;events.append(name)
        elif name=='key.new':assert receiver==stack+44 and args(2)==[0x122D70E,0xFFFFFFFF];pop=8;events.append(name)
        elif name=='key.destroy':assert receiver==stack+44;events.append(name)
        elif name=='start':assert args(2)==[stack+44,stack+108];pop=8;events.append(name)
        elif name=='pause':assert args(1)[0] in (0,1);events.append('pause' if args(1)[0] else 'unpause');pop=4
        elif name=='sex':assert args(1)==[actor];pop=4;result=sex;events.append(name)
        elif name=='term':assert receiver==thread;counts[name]+=1;result=int(counts[name]==cancel);events.append(name)
        elif name=='thing.new':assert receiver==stack+92 and args(1)==[thing];pop=4;result=thing;events.append(name)
        elif name=='health':assert args(1)==[thing];events.append(name);return
        elif name=='thing.destroy':assert receiver==thing;events.append(name)
        elif name=='hero':result=0x203100;events.append(name)
        elif name=='speak':
            assert receiver==stack+92 and args(6)==[0x203100,0x12D8714 if sex==1 else 0x12D86EC,1,0,1,0]
            pop=24;events.append('male' if sex==1 else 'female')
        elif name=='task':assert receiver==stack+92;counts[name]+=1;result=int(counts[name]<=frames);events.append(name)
        elif name=='movie.destroy':assert receiver==stack+108;events.append(name)
        elif name=='frame':events.append(name)
        machine.reg_write(UC_X86_REG_EAX,result);machine.reg_write(UC_X86_REG_ESP,esp+4+pop);machine.reg_write(UC_X86_REG_EIP,get(esp))
    uc.hook_add(UC_HOOK_CODE,hook);uc.emu_start(0xDAE1AA,0xDAEA49,count=500)
    # Unicorn's end address may stop before the hook for cancellation.
    if uc.reg_read(UC_X86_REG_EIP)==0xDAEA49 and not finished:finished.append(False)
    assert len(finished)==1 and uc.reg_read(UC_X86_REG_ESP)==stack
    return finished[0],events


def structured(sex,bits,frames,cancel):
    lua=LuaRuntime(unpack_returned_tuples=True)
    lua.globals().body=lua.execute(Path(__file__).with_name('villager_attacked_body.lua').read_text())
    lua.globals().config=lua.table_from(dict(sex=sex,health=struct.unpack('<f',struct.pack('<I',bits))[0],frames=frames,cancel=cancel))
    result,events=lua.execute('''
        local e,term,task={},0,0
        local function emit(v) e[#e+1]=v end
        local quest={EntityGetSex=function(_,me) assert(me==9);emit('sex');return config.sex end,
            IsActiveThreadTerminating=function() term=term+1;emit('term');return term==config.cancel end,
            NewScriptFrame=function(_,me) assert(me==9);emit('frame') end}
        local resources={}
        function resources:StartMovie(key)
            assert(key=='');for _,v in ipairs({'movie.new','key.new','start','key.destroy'}) do emit(v) end;return 7
        end
        function resources:Pause(value) emit(value and 'pause' or 'unpause') end
        function resources:NewThingFromResource(id) assert(id==5);emit('thing.new');return 8 end
        function resources:ThingHealth(id) assert(id==8);emit('health');return config.health end
        function resources:DestroyThing(id) assert(id==8);emit('thing.destroy') end
        function resources:SpeakVillagerAttacked(id,male) assert(id==5);emit('hero');emit(male and 'male' or 'female') end
        function resources:IsPerformingScriptTask(id) assert(id==5);task=task+1;emit('task');return task<=config.frames end
        function resources:DestroyMovie(id) assert(id==7);emit('movie.destroy') end
        return body(quest,9,resources,5),e
    ''')
    return result,list(events.values())


class VillagerAttackedBodyTests(unittest.TestCase):
    def test_original_health_speech_and_movie_exits(self):
        data=RData();verify(data);verify_health(data)
        self.assertEqual(data.string_at(0x12D8714),'TEXT_QST_048_VILLAGER_ATTACKED_MALE')
        self.assertEqual(data.string_at(0x12D86EC),'TEXT_QST_048_VILLAGER_ATTACKED_FEMALE')
        for sex,bits,frames in itertools.product((0,1,2),(0,0x80000000,0x3F800000,0xBF800000,1,0x7F800000,0xFF800000,0x7FC12345),(0,1,2)):
            for cancel in range(0,3+frames):
                case=(sex,bits,frames,cancel)
                with self.subTest(case=case):self.assertEqual(structured(*case),native(data,*case))
