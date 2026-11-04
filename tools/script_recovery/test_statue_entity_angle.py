"""Native x87 angle must reach the rotating statue's facing call."""
import math
import os
from pathlib import Path
import struct
from types import SimpleNamespace

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_evidence_lowering import lower_cifmod

ROOT=Path(__file__).resolve().parents[2]


@pytest.mark.parametrize('printed,pushed', [(0,0),(1,1),(0,1),(1,0),(1,2)])
def test_phantom_bool_requires_matching_adjacent_push(printed,pushed):
    raw=bytes((0x6a,pushed))+bytes.fromhex('d9442428dd0500200000')
    memory={0xff4+i:b for i,b in enumerate(raw)}
    memory.update({0x2000+i:b for i,b in enumerate(struct.pack('<d',1.0))})
    spec=SimpleNamespace(calls=[dict(site='0x1000',target='0x00BFEB28')],byte_at=memory.__getitem__)
    text=f'  f_stk_10 = angle + 0.5;\n  fret_0 = _CIfmod({printed},unaff_EDI,unaff_ESI,unaff_EBP);'
    out=lower_cifmod(text,spec)
    assert ('ENGINE_Fmod(f_stk_10, 1.0)' in out)==(printed==pushed)
    if printed!=pushed: assert out==text
    memory[0xff4]=0x90
    assert lower_cifmod(text,spec)==text


def test_retail_statue_push_loads_and_result_store():
    image=RData()
    assert image.bytes_at(0xed4249,16)==bytes.fromhex('d95c2424 6a01 d9442428 dd05007f2301')
    assert image.bytes_at(0xed4259,18)==bytes.fromhex('e8caa8d2ff 51 d91c24 57 8bcb ff9568070000')
    assert struct.unpack('<d',image.bytes_at(0x1237f00,8))[0]==1.0


@pytest.mark.parametrize('stage',['draft','readable'])
@pytest.mark.parametrize('fraction',[0.0,0.25,0.5,0.75,0.999])
@pytest.mark.parametrize('pointing',[0,1,3])
def test_statue_entity_facing_and_text(stage,fraction,pointing):
    root=Path(os.environ.get('STATUE_ENTITY_ARTIFACTS',ROOT/'refs/script_recovery/lifted/StatueMaster'))
    lua=LuaRuntime()
    lua.globals().fraction=fraction;lua.globals().pointing=pointing
    lua.execute('''
        package.preload['V_StatueMaster.native_quest_helpers']=function()
            return {HasCameraMode=function() return fraction end,
                    helper_ED43D0=function() return fraction end,
                    GetStatuePointingPosition=function() return pointing end,
                    helper_ED4420=function() return pointing end}
        end
    ''')
    lua.execute((root/stage/'FSE/V_StatueMaster/Entities/StatueMasterStatue.lua').read_text())
    lua.execute('''
        quest,me={},{}
        local stop={}
        local faced,tagged=0,0
        function quest:EntitySetCutsceneBehaviour(thing,mode) assert(thing==me and mode==2) end
        function quest:GetThingWithScriptName(name) assert(name=='StatueAltar');return {} end
        function quest:IsActiveThreadTerminating() return false end
        function quest:EntitySetFacingAngle(thing,angle,flag)
            assert(thing==me and type(angle)=='number' and flag==true)
            actualAngle=angle;faced=faced+1
        end
        function quest:SetReadableObjectTextTag(thing,key)
            assert(thing==me and key==(pointing==1 and 'TEXT_QST_061_STATUE_BOWERSTONE' or 'TEXT_QST_061_STATUE_GREATWOOD'))
            tagged=tagged+1
        end
        function quest:NewScriptFrame(thing) assert(thing==me);error(stop) end
        local ok,err=pcall(Main,quest,me)
        assert(not ok and err==stop,tostring(err))
        assert(faced==1 and tagged==(pointing==0 and 0 or 1))
    ''')
    assert lua.globals().actualAngle==pytest.approx(math.fmod(fraction+0.5,1.0),abs=1e-7)
