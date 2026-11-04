import copy
import json
import struct
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch

import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_master_accessor_reads import restore

ROOT = Path(__file__).resolve().parents[2]
TARGET, SITE = 0x2000, 0x1000
SOURCE = ('    pointer = WrongClass::WrongName(this);\n'
          '    if (pointer[0x74] == (WrongType)0x1) {\n        work();\n    }\n')


def setup():
    spec = SimpleNamespace(entity=False, master_off=0x44,
                           master_fields={0x74: ('WhisperKilledByHero', 'Bool')},
                           call_labels={'WrongClass::WrongName': TARGET})
    fn = {'calls': [{'site': hex(SITE), 'target': hex(TARGET)}]}
    data = {TARGET: b'\x8b\x41\x44\xc3', SITE: b'\xe8'+struct.pack('<i', TARGET-SITE-5),
            SITE+5: b'\x80\x78\x74\x01'}
    rdata = SimpleNamespace(bytes_at=lambda address, size: data.get(address))
    return spec, fn, data, rdata


def test_trivial_master_getter_read_uses_byte_offset():
    spec, fn, _, rdata = setup()
    output = restore(SOURCE, fn, spec, rdata)
    assert output == ('    if (*(byte *)(*(int *)(this + 0x44) + 0x74) == 1) {\n'
                      '        work();\n    }\n')


@pytest.mark.parametrize('case', ['entity', 'wrong-master', 'unknown-field', 'int-field',
    'getter-drift', 'call-drift', 'target-drift', 'cmp-drift', 'negative-displacement',
    'noncanonical-bool', 'duplicate-call', 'duplicate-source', 'escaping-pointer',
    'other-receiver', 'field-drift', 'value-drift'])
def test_ambiguous_master_reads_remain_unresolved(case):
    spec, fn, data, rdata = setup(); source = SOURCE
    if case == 'entity': spec.entity = True
    elif case == 'wrong-master': spec.master_off = 0x18
    elif case == 'unknown-field': spec.master_fields = {}
    elif case == 'int-field': spec.master_fields[0x74] = ('Other', 'Int')
    elif case == 'getter-drift': data[TARGET] = b'\x8b\x41\x40\xc3'
    elif case == 'call-drift': data[SITE] = b'\xe9'+data[SITE][1:]
    elif case == 'target-drift': data[SITE] = b'\xe8'+struct.pack('<i', TARGET-SITE-4)
    elif case == 'cmp-drift': data[SITE+5] = b'\x83\x78\x74\x01'
    elif case == 'negative-displacement': data[SITE+5] = b'\x80\x78\x94\x01'
    elif case == 'noncanonical-bool': data[SITE+5] = b'\x80\x78\x74\x02'
    elif case == 'duplicate-call': fn['calls'].append(copy.deepcopy(fn['calls'][0]))
    elif case == 'duplicate-source': source += SOURCE
    elif case == 'escaping-pointer': source += '    consume(pointer);\n'
    elif case == 'other-receiver': source = source.replace('(this)', '(other)')
    elif case == 'field-drift': source = source.replace('[0x74]', '[0x75]')
    elif case == 'value-drift': source = source.replace(')0x1', ')0x0')
    assert restore(source, fn, spec, rdata) == source


def test_pointer_slot_can_be_reused_after_comparison():
    spec, fn, _, rdata = setup()
    source = SOURCE + '    pointer = nextValue;\n    consume(pointer);\n'
    assert restore(source, fn, spec, rdata) != source


@pytest.fixture(scope='module')
def arena(tmp_path_factory):
    from tools.script_recovery.convert_quest_unit import UnitConverter
    from tools.script_recovery.build_readable_unit import readable_file
    unit = json.loads((ROOT/'refs/script_recovery/arena/units/Q_Arena.json').read_text())
    converter = UnitConverter(ROOT/'refs/script_recovery/arena/translation_unit_typed.json')
    out = tmp_path_factory.mktemp('master-accessor')
    with patch.object(converter, 'constructor_defaults', return_value={}):
        converter.convert(unit, out)
    draft = (out/'FSE/Arena/Arena.lua').read_text()
    return draft, readable_file(draft)[0]


HARNESS = r'''return function(killed, cancelled)
 local state={ArenaState=6,PlayerWon=true}
 local actor={IsAlive=function() return true end,IsNull=function()return false end,
              GetPos=function()return{x=0,y=0,z=0}end,GetAngleXY=function()return 0 end}
 local reads,macros,afterRead,serial=0,{},false,0
 local q,r={},{}
 function q:GetHero()return actor end
 function q:RetailResources()return r end
 function q:GetStateInt(k)return state[k] or 0 end
 function q:GetStateBool(k)return state[k] or false end
 function q:SetStateInt(k,v)state[k]=v end
 function q:SetStateBool(k,v)state[k]=v end
 function q:GetStateThing(k)return actor end
 function q:GetMasterGameState(k)
  assert(k=='WhisperKilledByHero');reads=reads+1;afterRead=true;return killed
 end
 function q:GetThingWithScriptName(k)return actor end
 function q:GetHeroTitle()return 1 end
 function q:IsQuestActive(k)return true end
 function q:IsLevelLoaded(k)return true end
 function q:IsActiveThreadTerminating()return cancelled and afterRead end
 function q:GetActiveQuestName()return 'Q_Arena' end
 function q:RegisterTimer()return 1 end
 function q:NewScriptFrame()error('unexpected frame')end
 setmetatable(q,{__index=function(_,k)return function(...)end end})
 function r:NewActorMap()return{}end
 function r:NewResource()serial=serial+1;return serial end
 function r:PrepareResource(x)assert(x~=nil)end
 function r:TryAcquire(resource,who,mode)assert(who==actor and mode==4);return true end
 function r:SetActor(map,name,resource)assert(map~=nil and resource~=nil)end
 function r:StartMovie(name)return 100 end
 function r:RunMacro(name,map,a,b)
  assert(map~=nil and a==false and b==true)
  assert(name==(killed and 'CS_ARENA_CELLS_LADYGREY_TWO' or 'CS_ARENA_CELLS_LADYGREY_ONE'))
  macros[#macros+1]=name;error('CUTSCENE_SELECTED')
 end
 setmetatable(r,{__index=function(_,k)return function(...)end end})
 -- Isolate Main's final encounter from initialization and the separate crowd thread.
 InitialiseVariables=function()end;AnimateCrowd=function()end
 local ok,err=pcall(Main,q)
 assert(reads==1,'master read count')
 if cancelled then assert(ok and #macros==0,err)
 else assert(not ok and tostring(err):find('CUTSCENE_SELECTED',1,true) and #macros==1,err)end
 return reads..':'..#macros
end'''


@pytest.mark.parametrize('style', [0, 1])
@pytest.mark.parametrize('killed', [False, True])
@pytest.mark.parametrize('cancelled', [False, True])
def test_actual_main_selects_dialogue_and_respects_cancellation(arena, style, killed, cancelled):
    lua = LuaRuntime(); lua.execute(arena[style]); check = lua.execute(HARNESS)
    assert check(killed, cancelled) == ('1:0' if cancelled else '1:1')
