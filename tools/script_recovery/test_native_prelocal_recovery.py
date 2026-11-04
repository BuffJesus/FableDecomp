from pathlib import Path
from copy import deepcopy
from unittest.mock import patch
import json,sys,struct
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file());sys.path.insert(0,str(ROOT))
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_thing_outputs import restore
from tools.script_recovery.build_readable_unit import readable_file
W=ROOT/'tools/script_recovery/fixtures/prelocal_native'

@pytest.fixture(scope='module',params=['arena','trophy_dealer'])
def reviewed(request):
    unit=request.param;name='Q_Arena' if unit=='arena' else 'V_TrophyDealer'
    c=UnitConverter(ROOT/'refs/script_recovery'/unit/'translation_unit_typed.json')
    data=json.loads((ROOT/'refs/script_recovery'/unit/'units'/(name+'.json')).read_text(encoding='utf-8'))
    address='0x00f1b690' if unit=='arena' else '0x00ee6e90'
    row=data['reviewedThingOutputScopes'][address]
    return unit,c,c.native(address),row,(W/(unit+'_intermediate.lua')).read_text(encoding='utf-8')

def test_exact_reviewed_recovery(reviewed):
    unit,c,fn,row,source=reviewed
    result,diagnostics,calls=restore(source,['retained'],['retained'],fn,row,c.rdata)
    assert result==(W/(unit+'_recovered.lua')).read_text(encoding='utf-8')
    assert diagnostics==calls==['retained']
    assert restore(source,[],[],fn,None,c.rdata)[0]==source

def test_converter_reaches_reviewed_intermediate(reviewed,tmp_path):
    unit,c,fn,row,source=reviewed
    name='Q_Arena' if unit=='arena' else 'V_TrophyDealer'
    data=json.loads((ROOT/'refs/script_recovery'/unit/'units'/(name+'.json')).read_text(encoding='utf-8'))
    if unit=='arena':
        data['entities']={'ArenaSpawn':data['entities']['ArenaSpawn']};data['entities']['ArenaSpawn']['functions']={'Main':data['entities']['ArenaSpawn']['functions']['Main']};data['quest']['functions']={}
    else:data['entities']={};data['quest']['functions']={'Main':data['quest']['functions']['Main']}
    data['quest']['helpers']={}
    from tools.script_recovery import native_reviewed_thing_outputs as adapter
    seen=[]
    def capture(current,todo,calls,native,review,rdata,root=None):
        if native['address'].lower()==fn['address'].lower():
            assert current==source and review==row;seen.append(True)
        return restore(current,todo,calls,native,review,rdata,root)
    with patch.object(adapter,'restore',capture),patch.object(c,'constructor_defaults',return_value={}):c.convert(data,tmp_path)
    assert seen==[True]

@pytest.mark.parametrize('fault',['source','native','extent','anchor','dependency','sidecar','intermediate','recovered','missing','duplicate','count'])
def test_changed_evidence_rejected(reviewed,fault):
    unit,c,fn,row,source=reviewed;row=deepcopy(row)
    if fault in ('source','native','intermediate','recovered'):row[fault+'Sha256']='0'*64
    elif fault=='extent':row['end']='0x0'
    elif fault=='anchor':row['anchors'][next(iter(row['anchors']))]='90'
    elif fault=='dependency':row['dependencies'][next(iter(row['dependencies']))]['sha256']='0'*64
    elif fault=='sidecar':row['sidecar'][next(iter(row['sidecar']))]['sha256']='0'*64
    else:
        if fault=='missing':row['operations'][0]['before']='missing'
        elif fault=='duplicate':row['operations'].append(row['operations'][0])
        else:row['operations'][0]['count']=2
    with pytest.raises(ValueError,match='reviewed Thing outputs'):
        restore(source,[],[],fn,row,c.rdata)

@pytest.fixture(scope='module')
def arena_forms():
    source=(W/'arena_recovered.lua').read_text(encoding='utf-8')
    return {'draft':source,'readable':readable_file(source)[0]}

@pytest.mark.parametrize('style',['draft','readable'])
@pytest.mark.parametrize('timer',[0,1,713,-4])
@pytest.mark.parametrize('round_number',[2,3,4])
@pytest.mark.parametrize('ready',[False,True])
@pytest.mark.parametrize('stop',[0,1,2])
def test_arena_effect_arguments_and_timer(arena_forms,style,timer,round_number,ready,stop):
    lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(arena_forms[style])
    lua.execute('''return function(timer,round,ready,stop)
      local q,me,pos={},{},{x=1,y=2,z=3};local frame,effects,closed=0,0,0
      function me:GetDataString()return '2' end
      function me:GetPos()return pos end
      function q:RegisterTimer()return timer end
      function q:IsActiveThreadTerminating()return frame>=stop end
      function q:NewScriptFrame(actor)assert(actor==me);frame=frame+1;return frame<stop end
      function q:GetStateBool(name)assert(name=='ArenaSpawnNeeded_2');return ready end
      function q:GetStateInt(name)assert(name=='ArenaRound');return round end
      function q:SetStateBool(name,value)assert(name=='ArenaSpawnNeeded_2' and value==false)end
      function q:CreateEffectAtPos(name,position,angle,independent,alwaysUpdate)
        assert(name=='SUMMON_ARENA' and position==pos and angle==0)
        assert(independent==false and alwaysUpdate==false);effects=effects+1;return {}
      end
      function q:DeregisterTimer(value)assert(value==timer);closed=closed+1 end
      local budget=0;debug.sethook(function()budget=budget+1;if budget>20 then error('budget')end end,'',1000)
      Main(q,me);debug.sethook()
      assert(closed==1);assert(effects==((ready and round~=3)and stop or 0))
    end''')(timer,round_number,ready,stop)

@pytest.mark.parametrize('timer',[1,713,0xffffffff])
def test_native_effect_stack_arguments(timer):
    import unicorn as u
    from unicorn import x86_const as x
    c=UnitConverter(ROOT/'refs/script_recovery/arena/translation_unit_typed.json');rd=c.rdata
    cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
    for a,n in [(0xf1b000,0x1000),(0x99e000,0x1000),(0x89f000,0x1000),(0x4aa000,0x1000),(0x200000,0x1000),(0x300000,0x20000)]:cpu.mem_map(a,n)
    cpu.mem_write(0xf1b73f,rd.bytes_at(0xf1b73f,0xf1b7a9-0xf1b73f))
    def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
    def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
    stack,owner,gsi,table,thingtable,pos=0x300800,0x310000,0x311000,0x312000,0x313000,0x314000
    put(owner+4,gsi);put(owner+8,thingtable);put(gsi,table);put(table+0x190,0x89f9e0);put(thingtable+0x18,0x200100);put(stack+0x1c,timer)
    calls=[];destroyed=[]
    def ret(purge,value=0):
        sp=cpu.reg_read(x.UC_X86_REG_ESP);cpu.reg_write(x.UC_X86_REG_EAX,value);cpu.reg_write(x.UC_X86_REG_EIP,word(sp));cpu.reg_write(x.UC_X86_REG_ESP,sp+4+purge)
    def hook(machine,address,size,user):
        sp=machine.reg_read(x.UC_X86_REG_ESP);receiver=machine.reg_read(x.UC_X86_REG_ECX)
        if address==0x99ebf0:
            put(receiver,word(sp+4));ret(8,receiver)
        elif address==0x200100:assert receiver==owner+8;ret(0,pos)
        elif address==0x89f9e0:
            args=[word(sp+4+4*i) for i in range(7)];calls.append((receiver,args));ret(28,args[0])
        elif address==0x99eae0:destroyed.append(receiver);ret(0)
        elif address==0x4aa840:destroyed.append(receiver);ret(0)
    cpu.hook_add(u.UC_HOOK_CODE,hook);cpu.reg_write(x.UC_X86_REG_ESP,stack);cpu.reg_write(x.UC_X86_REG_ESI,owner);cpu.reg_write(x.UC_X86_REG_EDI,owner+8);cpu.reg_write(x.UC_X86_REG_EBX,timer)
    cpu.emu_start(0xf1b73f,0xf1b7a9,count=100)
    assert calls==[(gsi,[stack+0x20,stack+0x10,pos,stack+0x14,0,0,0])]
    assert word(stack+0x10)==0x12f9d00 and word(stack+0x14)==0x122d70e
    assert destroyed==[stack+0x10,stack+0x14,stack+0x20]
    assert cpu.reg_read(x.UC_X86_REG_EBX)==timer and cpu.reg_read(x.UC_X86_REG_ESP)==stack

@pytest.mark.parametrize('style',['draft','readable'])
@pytest.mark.parametrize('region_after',[1,2,4])
@pytest.mark.parametrize('frames',[0,1,3,5])
def test_trophy_thread_then_region_without_global_flag(style,region_after,frames):
    source=(W/'trophy_dealer_recovered.lua').read_text(encoding='utf-8')
    if style=='readable':source=readable_file(source)[0]
    lua=LuaRuntime(unpack_returned_tuples=True)
    lua.execute('''return function(source,after,limit)
      local env=setmetatable({},{__index=function(_,key)assert(key~='bVar3','stray native guard');return _G[key]end})
      assert(load(source,'trophy','t',env))()
      local q={};local frame,polls,threads,started,objectives=0,0,0,0,0
      function q:SetCreatureGeneratorsEnabled(region,flag)assert(region=='Witchwood2' and not flag)end
      function q:AddEntityBinding(name,path)assert(path=='V_TrophyDealer/Entities/'..name)end
      function q:FinalizeEntityBindings()end
      function q:ActivateQuestWithoutLoadingResources(name)assert(name=='V_SingingStones')end
      function q:CreateThread(name)assert(name=='HilightGuildTeleporter');threads=threads+1 end
      function q:IsRegionLoaded(name)assert(name=='Witchwood1' and threads==1);polls=polls+1;return polls>=after end
      function q:NewScriptFrame()frame=frame+1;return frame<limit end
      function q:IsActiveThreadTerminating()return frame>=limit end
      function q:GetActiveQuestName()return 'V_TrophyDealer' end
      function q:SetQuestCardObjective(name,text,r1,r2)
        assert(name=='V_TrophyDealer' and text=='TEXT_QUEST_FIND_TROPHY_DEALER_OBJECTIVE_02' and r1=='Witchwood2' and r2=='');objectives=objectives+1
      end
      function q:KickOffQuestStartScreen(name,show,flag)assert(name=='V_TrophyDealer' and show and not flag);started=started+1 end
      env.Main(q)
      assert(threads==1 and started==objectives)
      assert(started==((after-1<limit)and 1 or 0))
    end''')(source,region_after,frames)

@pytest.mark.parametrize('allocated',[False,True])
@pytest.mark.parametrize('initial_flags',[0,3])
def test_native_thread_name_cleanup_ownership(allocated,initial_flags):
    import unicorn as u
    from unicorn import x86_const as x
    c=UnitConverter(ROOT/'refs/script_recovery/trophy_dealer/translation_unit_typed.json');rd=c.rdata
    cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
    for a,n in [(0xee6000,0x2000),(0xbfe000,0x1000),(0x99e000,0x1000),(0xcdd000,0x1000),(0xcb7000,0x1000),(0x300000,0x20000)]:cpu.mem_map(a,n)
    cpu.mem_write(0xee6fd4,rd.bytes_at(0xee6fd4,0xee7049-0xee6fd4))
    def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
    def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
    stack,owner,obj=0x300800,0x310000,0x315000
    destructors=[];registration=[];constructed=[]
    def ret(purge,value=0):
        sp=cpu.reg_read(x.UC_X86_REG_ESP);cpu.reg_write(x.UC_X86_REG_EAX,value);cpu.reg_write(x.UC_X86_REG_EIP,word(sp));cpu.reg_write(x.UC_X86_REG_ESP,sp+4+purge)
    def hook(machine,address,size,user):
        sp=machine.reg_read(x.UC_X86_REG_ESP);receiver=machine.reg_read(x.UC_X86_REG_ECX)
        if address==0xbfea1a:assert word(sp+4)==0x3c;ret(0,obj if allocated else 0)
        elif address==0x99ebf0:put(receiver,word(sp+4));ret(8,receiver)
        elif address==0xcdd450:
            assert receiver==obj and word(sp+4)==stack+0xc and word(sp+8)==0
            constructed.append(receiver);ret(8,receiver)
        elif address==0xcb7e50:registration.append((receiver,word(sp+4),word(sp+8)));ret(8)
        elif address==0x99eae0:destructors.append(receiver);ret(0)
    cpu.hook_add(u.UC_HOOK_CODE,hook);cpu.reg_write(x.UC_X86_REG_ESP,stack);cpu.reg_write(x.UC_X86_REG_ESI,owner);cpu.reg_write(x.UC_X86_REG_EBX,initial_flags)
    cpu.emu_start(0xee6fd4,0xee7049,count=100)
    assert registration==[(owner,obj if allocated else 0,stack+0x14)]
    assert destructors==[stack+0x14]+([stack+0xc] if allocated else [])
    assert constructed==([obj] if allocated else [])
    if allocated:assert word(obj+0x34)==0xee71a0 and word(obj+0x38)==owner and word(stack+0xc)==0x12f527c
    assert cpu.reg_read(x.UC_X86_REG_ESP)==stack
