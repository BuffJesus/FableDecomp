"""Exercise the generated startup paths that failed in the v43 live replay."""
import math
import os
from pathlib import Path
import struct
import subprocess
import sys
import tempfile
from types import SimpleNamespace

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.native_evidence_lowering import lower_cifmod, normalise_typed_decompile

ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def artifacts():
    explicit = os.environ.get('STARTUP_OPERANDS_ARTIFACTS')
    if explicit:
        yield Path(explicit)
        return
    owned = tempfile.TemporaryDirectory(prefix='startup_operands_', dir=ROOT / 'work')
    root = Path(owned.name)
    for unit in ('statue_master', 'sick_child', 'wasp_boss', 'trader_escort', 'bandit_camp'):
        dest = root / unit
        subprocess.run([sys.executable, '-m', 'tools.script_recovery.convert_quest_unit',
                        '--unit', unit, '--out', str(dest / 'draft')], cwd=ROOT, check=True, capture_output=True)
        code = ('from pathlib import Path; import sys; '
                'from tools.script_recovery.build_readable_unit import build; '
                'p=Path(sys.argv[2]); build(sys.argv[1],draft=p/"draft",out=p/"readable")')
        subprocess.run([sys.executable, '-c', code, unit, str(dest)], cwd=ROOT, check=True, capture_output=True)
    yield root
    owned.cleanup()


def wrapped(x, y):
    angle = math.atan2(x, y) * 0.15915493667125702
    if angle < 0 or angle >= 1:
        # The native call reloads its dividend from a single-precision slot.
        angle = math.fmod(struct.unpack('<f', struct.pack('<f', angle))[0], 1)
        if angle < 0:
            angle += 1
    return angle


@pytest.mark.parametrize('stage', ['draft', 'readable'])
@pytest.mark.parametrize('x,y', [(-1, 0), (-1, -1), (-1, 1), (0, -1), (1, 0), (1, 1), (0, 1), (0, 0)])
def test_statue_all_four_angle_stores(artifacts, stage, x, y):
    lua = LuaRuntime()
    lua.execute((artifacts / 'statue_master' / stage / 'FSE/V_StatueMaster/V_StatueMaster.lua').read_text())
    lua.globals().dx, lua.globals().dy = x, y
    lua.execute('''
        angles, calls = {}, 0
        local center = {GetPos=function() return {x=10,y=20,z=30} end, IsNull=function() return false end}
        local marker = {GetPos=function() return {x=10+dx,y=20+dy,z=99} end, IsNull=center.IsNull}
        quest = {}
        function quest:AddEntityBinding() end
        function quest:FinalizeEntityBindings() end
        function quest:GetThingWithScriptName(name) return name=='SM_Center' and center or marker end
        function quest:SetStateFloat(name,value)
            assert(type(value)=='number' and value>=0 and value<1)
            angles[name]=value; calls=calls+1
            if calls==4 then error('ANGLE_BOUNDARY') end
        end
        ok, reason = pcall(Main, quest)
        assert(not ok and string.find(reason,'ANGLE_BOUNDARY',1,true), reason)
    ''')
    for i in range(4):
        assert lua.globals().angles[f'AnglesToFaceList_{i}'] == pytest.approx(wrapped(x, y), abs=6e-8)


@pytest.mark.parametrize('stage', ['draft', 'readable'])
@pytest.mark.parametrize('x,y', [(-1, 0), (-1, -1), (-1, 1), (0, -1), (1, 0), (0, 1)])
def test_child_bed_teleport_and_animation(artifacts, stage, x, y):
    lua = LuaRuntime()
    lua.execute((artifacts / 'sick_child' / stage / 'FSE/V_SickChild/Entities/SickChild.lua').read_text())
    lua.globals().dx, lua.globals().dy = x, y
    lua.execute('''
        me, bed, handle, hero = {}, {}, {}, {}
        pos={x=17,y=-31,z=6}; trace={}; quest={}; resources={}
        function quest:GetHero() return hero end
        function quest:RetailResources() return resources end
        function quest:NewScriptFrame(actor) assert(actor==me); return true end
        function quest:IsActiveThreadTerminating() return false end
        function resources:NewResource() return handle end
        function resources:PrepareResource(h) assert(h==handle) end
        function resources:TryAcquire(h,actor,priority) assert(h==handle and actor==me and priority==4); return true end
        function quest:RegisterTimer() return 77 end
        function quest:GetThingWithScriptName(name) assert(name=='SickChildBed'); return bed end
        function quest:GetSleepingPositionAndOrientationFromBed(a,b)
            -- Native EC5F08/09 pushes bed then the entity; preserve operand order.
            assert(a==me and b==bed)
            return {pos=pos,orient={x=dx,y=dy,z=0}}
        end
        function quest:EntityTeleportToPosition(...)
            assert(select('#',...)==5)
            local actor,p,a,b,c=...
            assert(actor==me and p==pos and type(a)=='number' and b==false and c==false)
            teleportAngle=a; trace[#trace+1]='teleport'
        end
        function quest:ReadGlobalGameDataString(offset) assert(offset==0x748); return 'BED_SLEEP_TEST' end
        function resources:PlayLoopingAnimation(...)
            assert(select('#',...)==10)
            local h,key,n,b1,b2,b3,b4,b5,b6,b7=...
            assert(h==handle and key=='BED_SLEEP_TEST' and n==-1)
            assert(b1==false and b2==false and b3==false and b4==true and b5==true and b6==false and b7==false)
            trace[#trace+1]='animation'
        end
        function quest:SetBedAvailability(b,flag) assert(b==bed and flag==false) end
        function quest:SetThingAsUsable(b,flag) assert(b==bed and flag==false); error('BED_BOUNDARY') end
        ok,reason=pcall(Main,quest,me)
        assert(not ok and string.find(reason,'BED_BOUNDARY',1,true),reason)
        assert(table.concat(trace,',')=='teleport,animation')
    ''')
    assert lua.globals().teleportAngle == pytest.approx(wrapped(x, y), abs=6e-8)


@pytest.mark.parametrize('args', ['', 'unaff_EDI,unaff_ESI,unaff_EBP,unaff_EBX', '(int)unaff_EBP,(int)unaff_EDI,unaff_ESI,unaff_EBX'])
def test_cifmod_register_noise_still_requires_instruction_proof(args):
    image = {0xff6+i: b for i,b in enumerate(bytes.fromhex('d9442410dd0500200000'))}
    image.update({0x2000+i: b for i,b in enumerate(struct.pack('<d',1.0))})
    spec = SimpleNamespace(calls=[dict(site='0x1000',target='0x00BFEB28')],byte_at=image.__getitem__)
    text = f'  f_stk_40 = angle;\n  fret_0 = _CIfmod({args});\n'
    assert 'fret_0 = ENGINE_Fmod(f_stk_40, 1.0);' in lower_cifmod(text,spec)
    image[0xff6] = 0x90
    assert lower_cifmod(text,spec) == text
    image[0xff6] = 0xd9
    unknown = text.replace(f'_CIfmod({args})', '_CIfmod(unknown)')
    assert lower_cifmod(unknown,spec) == unknown


def test_float_update_stops_at_string_lifetime():
    source = ('{\n  CCharString CStack_8c;\n  CStack_8c = (CCharString)(float)angle;\n'
              '  CStack_8c = (CCharString)((float)CStack_8c + 1.0);\n'
              '  fill(&CStack_8c);\n  CStack_8c = (CCharString)other;\n}\n')
    out = normalise_typed_decompile(source)
    assert 'f_stk_8c = (f_stk_8c + 1.0);' in out
    assert 'fill(&CStack_8c);' in out and 'CStack_8c = (CCharString)other;' in out


def test_retail_angle_and_bed_operand_evidence():
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    from tools.script_recovery.lift_native_lua import RData
    image = RData()
    decode = Cs(CS_ARCH_X86, CS_MODE_32)
    for site in (0xed3dd3, 0xed3e5a, 0xed3ee1, 0xed3f60, 0xec5f5e):
        ops = [(i.mnemonic, i.op_str) for i in decode.disasm(image.bytes_at(site-10,15),site-10)]
        assert ops == [('fld','dword ptr [esp + 0x10]'),
                       ('fld','qword ptr [0x1237f00]'), ('call','0xbfeb28')]
    assert struct.unpack('<d', image.bytes_at(0x1237f00,8))[0] == 1.0
    ops = [(i.mnemonic, i.op_str) for i in decode.disasm(image.bytes_at(0xec5f82,29),0xec5f82)]
    assert ops == [('mov','eax, dword ptr [esp + 0x10]'), ('mov','ecx, dword ptr [esi + 4]'),
                   ('mov','edx, dword ptr [ecx]'), ('push','0'), ('push','0'), ('push','eax'),
                   ('lea','eax, [esp + 0x9c]'), ('push','eax'), ('push','edi'),
                   ('call','dword ptr [edx + 0x764]')]
    # Out position was ESP+0x90 before the three teleport pushes (same object).
    ops = [(i.mnemonic,i.op_str) for i in decode.disasm(image.bytes_at(0xec5eed,8),0xec5eed)]
    assert ops == [('lea','eax, [esp + 0x94]'), ('push','eax')]


@pytest.mark.parametrize('stage', ['draft', 'readable'])
@pytest.mark.parametrize('unit,relative,key,flags', [
    ('wasp_boss', 'WaspBoss/Entities/HornetDrone.lua', 'ST_FEED_LOOP', [False,False,True,False,True,False,False]),
    ('trader_escort', 'TraderEscort/Entities/InfectedBalverine.lua', 'ST_MUNCH', [False,False,False,True,True,False,False]),
    ('bandit_camp', 'BanditCampBossBattle/Entities/BanditKing.lua', 'DEFEATED_POSE', [False,False,False,True,True,False,False]),
])
def test_other_recovered_animation_sites(artifacts, stage, unit, relative, key, flags):
    lua = LuaRuntime()
    lua.execute((artifacts / unit / stage / 'FSE' / relative).read_text())
    lua.globals().expectedKey = key
    lua.globals().expectedFlags = lua.table_from(flags)
    lua.execute('''
        me,quest,resources,handle,body,hero={},{},{},{},{},{}
        function quest:RetailResources() return resources end
        function quest:GetHero() return hero end
        function quest:NewScriptFrame(actor) assert(actor==me); return true end
        function quest:IsActiveThreadTerminating() return false end
        function resources:NewResource() return handle end
        function resources:PrepareResource(h) assert(h==handle) end
        function resources:TryAcquire(h,actor,priority) assert(h==handle and actor==me and priority==4); return true end
        function quest:GetNearestWithScriptName(actor,key) assert(actor==me and key=='Q_WB_DeadBody'); return body end
        function me:MoveToThing(target,radius,kind) assert(target==body and kind==1) end
        function me:IsPerformingScriptTask() return false end
        function resources:PlayAnimationWithNativeArgument5(...)
            assert(select('#',...)==8)
            local h,key,a,b,c,d,e,f=...
            assert(h==handle and key=='ST_FEED_INTO')
            assert(a==false and b==false and c==true and d==false and e==false and f==false)
        end
        function quest:ReadGlobalGameData(offset) assert(offset==0xe0c); return 10 end
        function quest:SetQuestCardObjective() end
        function quest:MiniMapAddMarker() end
        function quest:EntitySetAbleToBeEngagedInCombat() end
        function quest:GetHealth(actor) assert(actor==me); return 100 end
        function quest:RegisterTimer() return 77 end
        function quest:SetTimer() end
        function quest:GetStateBool(key) assert(key=='ItsAllOver'); return true end
        function quest:EntitySetFacingAngleTowardsThing(a,b,flag)
            assert((a==me and b==hero or a==hero and b==me) and flag==false)
        end
        function resources:PlayLoopingAnimation(...)
            assert(select('#',...)==10)
            local args={...}
            assert(args[1]==handle and args[2]==expectedKey and args[3]==-1)
            for i=1,7 do assert(args[i+3]==expectedFlags[i]) end
            error('ANIMATION_BOUNDARY')
        end
        local ok,reason=pcall(Main,quest,me)
        assert(not ok and string.find(reason,'ANIMATION_BOUNDARY',1,true),reason)
    ''')
