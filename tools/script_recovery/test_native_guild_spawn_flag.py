from pathlib import Path
from copy import deepcopy
from unittest.mock import patch
import hashlib,json,struct,sys
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file());sys.path.insert(0,str(ROOT))
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_thing_outputs import restore
from tools.script_recovery.build_readable_unit import readable_file
W=ROOT/'tools/script_recovery/fixtures/guild_spawn_flag'

@pytest.fixture(scope='module')
def evidence():
    c=UnitConverter(ROOT/'refs/script_recovery/guild_training/translation_unit_typed.json')
    data=json.loads((ROOT/'refs/script_recovery/guild_training/units/Q_GuildTrainingWill.json').read_text(encoding='utf-8'))
    return c,c.native('0x00d5e0c0'),data['reviewedThingOutputScopes']['0x00d5e0c0'],(W/'guild_intermediate.lua').read_text(encoding='utf-8')

def test_recovery(evidence):
    c,fn,row,source=evidence
    result,todo,calls=restore(source,['keep'],['keep'],fn,row,c.rdata)
    assert result==(W/'guild_recovered.lua').read_text(encoding='utf-8') and todo==calls==['keep']
    assert source.count(row['operations'][0]['before'])==1
    assert restore(source,[],[],fn,None,c.rdata)[0]==source

def test_converter_reaches_reviewed_spawn(evidence,tmp_path):
    c,fn,row,source=evidence
    data=json.loads((ROOT/'refs/script_recovery/guild_training/units/Q_GuildTrainingWill.json').read_text(encoding='utf-8'))
    data['entities']={'TheRealGuildmaster':data['entities']['TheRealGuildmaster']}
    data['entities']['TheRealGuildmaster']['functions']={'Main':data['entities']['TheRealGuildmaster']['functions']['Main']}
    data['quest']['functions']={};data['quest']['helpers']={}
    from tools.script_recovery import native_reviewed_thing_outputs as adapter
    seen=[]
    def capture(current,todo,calls,native,review,rdata,root=None):
        if native['address'].lower()==fn['address'].lower():assert current==source and review==row;seen.append(True)
        return restore(current,todo,calls,native,review,rdata,root)
    with patch.object(adapter,'restore',capture),patch.object(c,'constructor_defaults',return_value={}):c.convert(data,tmp_path)
    assert seen==[True]

@pytest.mark.parametrize('fault',['native','source','intermediate','recovered','extent','push','call','buffer','sidecar','dependency','before','count'])
def test_stale_evidence_rejected(evidence,fault):
    c,fn,row,source=evidence;row=deepcopy(row)
    if fault in ('native','source','intermediate','recovered'):row[fault+'Sha256']='0'*64
    elif fault=='extent':row['end']='0x0'
    elif fault in ('push','call','buffer'):row['anchors'][{'push':'0xd61967','call':'0xd6198c','buffer':'0xd61981'}[fault]]='90'
    elif fault=='sidecar':next(iter(row['sidecar'].values()))['sha256']='0'*64
    elif fault=='dependency':next(iter(row['dependencies'].values()))['sha256']='0'*64
    elif fault=='before':row['operations'][0]['before']='missing'
    else:row['operations'][0]['count']=2
    with pytest.raises(ValueError,match='reviewed Thing outputs'):restore(source,[],[],fn,row,c.rdata)

@pytest.fixture(scope='module')
def forms():
    source=(W/'guild_recovered.lua').read_text(encoding='utf-8')
    point=source.index('quest:ResetPlayerCreatureCombatMultiplier()')
    start=source.rfind('quest:FadeScreenOut(',0,point);end=source.index('goto LAB_00d61ad8',point)+len('goto LAB_00d61ad8')
    block=source[start:end]
    wrapped='function Main(quest)\n local alive,bVar6,bVar8,delay,pCVar15,pCVar12,pCVar13,r9\n do\n'+block+'\n end\n ::LAB_00d61ad8::\n do return "finished" end\n ::LAB_00d61b44::\n do return "cancelled" end\nend\n'
    return {'draft':wrapped,'readable':readable_file(wrapped)[0]}

@pytest.mark.parametrize('style',['draft','readable'])
@pytest.mark.parametrize('active',[False,True])
@pytest.mark.parametrize('terminating',[False,True])
@pytest.mark.parametrize('height',[0,3,100])
def test_graduation_block(forms,style,active,terminating,height):
    lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(forms[style])
    lua.execute('''return function(active,terminating,height)
      local q={};local calls={};local pos={x=7,y=8,z=height};local marker={};local old={};local spawned=0
      function marker:GetPos()return pos end
      setmetatable(q,{__index=function(_,key)return function(_,...)calls[#calls+1]=key end end})
      function q:IsQuestActive(name)assert(name=='Q_GuildTrainingWoodsWill');return active end
      function q:IsActiveThreadTerminating()return terminating end
      function q:GetActiveQuestName()return 'Q_GuildTrainingWill' end
      function q:GetThingWithScriptName(name)if name=='MeleeApprentice' then return old end;assert(name=='MeleeApprenticeMarker');return marker end
      function q:RemoveThing(thing,a,b)assert(thing==old and a==false and b==true)end
      function q:CreateCreature(def,position,name,flag)
        assert(def=='CREATURE_RIVAL_HERO_WHISPER_APPRENTICE' and position==pos and name=='MeleeApprentice')
        assert(type(flag)=='boolean' and flag==false,'nativeFlag must be false');spawned=spawned+1;return {}
      end
      local result=Main(q)
      assert(result==((active and terminating)and 'cancelled' or 'finished'))
      assert(spawned==((active and terminating)and 0 or 1))
    end''')(active,terminating,height)

def test_native_create_flag_stack(evidence):
    import unicorn as u
    from unicorn import x86_const as x
    c,fn,row,source=evidence;cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
    for a,n in [(0xd61000,0x1000),(0x8a9000,0x1000),(0x200000,0x1000),(0x300000,0x20000)]:cpu.mem_map(a,n)
    cpu.mem_write(0xd61962,c.rdata.bytes_at(0xd61962,0xd61992-0xd61962))
    def put(a,v):cpu.mem_write(a,struct.pack('<I',v))
    def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
    stack,owner,gsi,table,marker,thingtable,pos=0x300800,0x310000,0x311000,0x312000,0x313000,0x314000,0x315000
    put(owner+4,gsi);put(gsi,table);put(table+0x16c,0x8a9100);put(marker,thingtable);put(thingtable+0x18,0x200100)
    calls=[]
    def ret(purge,value):
        sp=cpu.reg_read(x.UC_X86_REG_ESP);cpu.reg_write(x.UC_X86_REG_EAX,value);cpu.reg_write(x.UC_X86_REG_EIP,word(sp));cpu.reg_write(x.UC_X86_REG_ESP,sp+4+purge)
    def hook(machine,address,size,user):
        sp=machine.reg_read(x.UC_X86_REG_ESP)
        if address==0x200100:assert machine.reg_read(x.UC_X86_REG_ECX)==marker;ret(0,pos)
        elif address==0x8a9100:
            args=[word(sp+4+4*i) for i in range(5)];calls.append((machine.reg_read(x.UC_X86_REG_ECX),args));ret(20,args[0])
    cpu.hook_add(u.UC_HOOK_CODE,hook);cpu.reg_write(x.UC_X86_REG_ESP,stack);cpu.reg_write(x.UC_X86_REG_ESI,owner);cpu.reg_write(x.UC_X86_REG_EAX,marker)
    cpu.emu_start(0xd61962,0xd61992,count=50)
    assert calls==[(gsi,[stack+0x1f4,stack+0xe0,pos,stack+0xe8,0])]
    assert cpu.reg_read(x.UC_X86_REG_ESP)==stack
