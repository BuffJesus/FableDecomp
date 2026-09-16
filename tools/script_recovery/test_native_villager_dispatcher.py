"""Original outer Main versus Lua, abstracting separately proved dialogue regions."""
import itertools
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_villager_control_resource import verify
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_ECX,UC_X86_REG_EDI,UC_X86_REG_EAX,UC_X86_REG_ESI,UC_X86_REG_EBX


def native(data,branches,cancel,failures,phase_ok):
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for address,size in ((0xDAD000,0x2000),(0x99A000,0x1000),(0x99E000,0x1000),
                         (0xCD2000,0x1000),(0x7E7000,0x1000),(0xF35000,0x1000),
                         (0x100000,0x10000),(0x200000,0x4000)):uc.mem_map(address,size)
    uc.mem_write(0xDADF80,data.bytes_at(0xDADF80,2787))
    def put(a,v):uc.mem_write(a,(v&0xFFFFFFFF).to_bytes(4,'little'))
    def get(a):return int.from_bytes(uc.mem_read(a,4),'little')
    stack,thread,actor,game,parent,table=0x108000,0x200000,0x200008,0x201000,0x201100,0x202000
    put(thread+4,game);put(thread+0x14,parent);put(game,table)
    slots={0x1c:'frame',0x20:'acquire',0x118:'hero',0x95c:'ally'}
    calls={0x203000+i*16:name for i,name in enumerate(slots.values())}
    for (slot,_),target in zip(slots.items(),calls):put(table+slot,target)
    calls.update({0xF35B30:'term',0x99A380:'resource',0x99E4B0:'key',0xCD23B9:'prepare',
                  0xCD2770:'reset',0xDAEA70:'deed',0x99EAE0:'key.destroy',0x7E74D0:'release'})
    for reg,value in ((UC_X86_REG_ESP,stack),(UC_X86_REG_ESI,thread),(UC_X86_REG_EDI,actor),(UC_X86_REG_EBX,0)):uc.reg_write(reg,value)
    events=[];counts={'term':0,'acquire':0,'iteration':-1};held=[False];finished=[]
    def branch():return branches[counts['iteration']%len(branches)]
    def jump(address):uc.reg_write(UC_X86_REG_EIP,address)
    def hook(machine,address,size,user):
        if address in (0xDAE9EE,0xDAEA5B):finished.append(1);machine.emu_stop();return
        if address==0xDAE026:
            counts['iteration']+=1;events.append('hit');uc.mem_write(stack+0x13,bytes([branch()=='hit']));jump(0xDAE0EE);return
        if address==0xDAE3B5:
            events.append('talk');machine.reg_write(UC_X86_REG_EBX,int(branch()=='talk'));jump(0xDAE3DC);return
        if address==0xDAE143:events.append('state.hit')
        if address in (0xDAE1AA,0xDAE3F3,0xDAE6BC):
            events.append({0xDAE1AA:'attacked',0xDAE3F3:'talk.body',0xDAE6BC:'ambient.body'}[address])
            jump(0xDAE98E if phase_ok else 0xDAEA49);return
        if address==0xDAE614:events.append('ambient.gate');jump(0xDAE66B if branch()=='ambient' else 0xDAE98E);return
        if address==0xDAE67A:events.append('ambient.start');jump(0xDAE6BC);return
        if address==0xDAE9AE:events.append('release');jump(0xDAE9EE);return
        if address not in calls:return
        name=calls[address];esp=machine.reg_read(UC_X86_REG_ESP);ecx=machine.reg_read(UC_X86_REG_ECX)
        args=lambda n:[get(esp+4+i*4) for i in range(n)]
        pop=0;result=0xBADBAD00
        if name=='term':counts[name]+=1;assert counts[name]<100;result=int(counts[name]>=cancel)
        elif name=='acquire':assert ecx==game and args(3)==[actor,stack+92,4];pop=12;counts[name]+=1;result=int(counts[name]>failures);held[0]=True
        elif name=='hero':assert ecx==game;result=0x203800
        elif name=='ally':assert ecx==game and args(2) in ([actor,0x203800],[0x203800,actor]);pop=8
        elif name=='deed':assert ecx==parent and args(1)==[2];pop=4
        elif name=='prepare':assert ecx==stack+92;result=int(held[0])
        elif name=='reset':assert ecx==stack+92;held[0]=False
        elif name in ('resource','release'):assert ecx==stack+92
        elif name in ('key','key.destroy'):assert ecx==stack+20
        elif name=='frame':assert ecx==game
        if name!='reset':events.append(name)
        machine.reg_write(UC_X86_REG_EAX,result);machine.reg_write(UC_X86_REG_ESP,esp+4+pop);jump(get(esp))
    uc.hook_add(UC_HOOK_CODE,hook);uc.emu_start(0xDADFBE,0xDAEFFF,count=5000)
    assert finished and uc.reg_read(UC_X86_REG_ESP)==stack
    return events


def structured(branches,cancel,failures,phase_ok):
    lua=LuaRuntime();lua.globals().config=lua.table_from(dict(branches=lua.table_from(branches),cancel=cancel,failures=failures,phase=phase_ok))
    lua.execute('''
        events={};local iteration,terms,acquires=0,0,0
        local function event(name) events[#events+1]=name end
        local function branch() return config.branches[(iteration-1)%#config.branches+1] end
        __native_entity_state={SetStateBool=function() event('state.hit') end}
        handleVillagerAttackedDialogue=function() event('attacked');return config.phase end
        handleVillagerConversation=function() event('talk.body');return config.phase end
        addVillagerAmbientLine=function() event('ambient.body');return config.phase end
        GetVillagerSpeechIndex=function() error('abstracted selection called index') end
        require=function() return {AddBadDeed=function() event('deed') end} end
        local resources={}
        function resources:NewResource() event('resource');return 5 end
        function resources:NewText() event('key');return 7 end
        function resources:PrepareResource(id) assert(id==5);event('prepare') end
        function resources:WasVillagerHit() iteration=iteration+1;event('hit');return branch()=='hit' end
        function resources:WasVillagerTalkedTo() event('talk');return branch()=='talk' end
        function resources:SetVillagerHeroAllies() event('hero');event('ally');event('hero');event('ally') end
        function resources:TryAcquire(id,me,priority) assert(id==5 and me==9 and priority==4);event('acquire');acquires=acquires+1;return acquires>config.failures end
        function resources:ShouldVillagerStartAmbientConversation() event('ambient.gate');return branch()=='ambient' end
        function resources:StartVillagerAmbientConversation() event('ambient.start');return 73 end
        function resources:DestroyText(id) assert(id==7);event('key.destroy') end
        function resources:ReleaseResource(id) assert(id==5);event('release') end
        quest={}
        function quest:RegisterBoundConsciousCondition() end
        function quest:NewScriptFrame() event('frame') end
        function quest:IsActiveThreadTerminating() event('term');terms=terms+1;assert(terms<100);return terms>=config.cancel end
        function quest:WithRetailResources(callback) callback(resources) end
        function quest:GetStateInt() return 73 end
        function quest:GetVillagerSpeechLists() return {} end
    ''')
    lua.execute(Path(__file__).with_name('villager_main_body.lua').read_text());lua.execute('Main(quest,9)')
    return list(lua.globals().events.values())


class NativeVillagerDispatcherTests(unittest.TestCase):
    def test_native_outer_control_and_cleanup_with_verified_phase_boundaries(self):
        data=RData();verify(data)
        paths=(('idle',),('hit',),('talk',),('ambient',),('idle','hit','talk','ambient'))
        for branches,cancel,failures,phase in itertools.product(paths,range(1,16),(0,2),(False,True)):
            case=(branches,cancel,failures,phase)
            with self.subTest(case=case):self.assertEqual(structured(*case),native(data,*case))
