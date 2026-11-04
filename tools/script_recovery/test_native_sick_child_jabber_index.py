import itertools
import struct
import sys
from pathlib import Path

ROOT = next(p for p in Path(__file__).resolve().parents if (p / 'tools/script_recovery/script_units.py').is_file())
sys.path.insert(0, str(ROOT))
from tools.script_recovery.lift_native_lua import RData


def test_native_jabber_index_and_missing_text_reset():
    import unicorn as u
    from unicorn import x86_const as x

    rd = RData()
    cpu = u.Uc(u.UC_ARCH_X86, u.UC_MODE_32)
    cpu.mem_map(0xec6000, 0x1000)
    cpu.mem_map(0x300000, 0x10000)
    cpu.mem_write(0xec6000, rd.bytes_at(0xec6000, 0x1000))
    layouts = 0

    def put(address, value):
        cpu.mem_write(address, struct.pack('<I', value & 0xffffffff))

    def word(address):
        return struct.unpack('<I', cpu.mem_read(address, 4))[0]

    for initial, missing_at, stack in itertools.product((10, 0x7ffffffc, 0xfffffff8), (-1, 0, 1, 2), (0x300800, 0x301800)):
        cpu.reg_write(x.UC_X86_REG_ESP, stack)
        cpu.emu_start(0xec60b3, 0xec60bb, count=1)
        assert word(stack + 0x1c) == 10
        put(stack + 0x1c, initial)
        expected = initial
        for turn in range(4):
            cpu.emu_start(0xec61a9, 0xec61af, count=2)
            assert cpu.reg_read(x.UC_X86_REG_EBX) == expected & 0xffffffff
            assert cpu.reg_read(x.UC_X86_REG_EDX) == expected & 0xffffffff
            exists = turn != missing_at
            eax = 0xabcdef00 | int(exists)
            cpu.reg_write(x.UC_X86_REG_EAX, eax)
            cpu.emu_start(0xec61f3, 0xec61fe, count=2)
            assert cpu.reg_read(x.UC_X86_REG_EIP) == (0xec6232 if exists else 0xec61f7)
            if not exists:
                cpu.emu_start(0xec61f7, 0xec61fc, count=1)
                expected = 10
                assert cpu.reg_read(x.UC_X86_REG_EBX) == 10
            cpu.reg_write(x.UC_X86_REG_ECX, 0x55667788)
            cpu.emu_start(0xec6237, 0xec6240, count=3)
            expected = (expected + 10) & 0xffffffff
            assert word(stack + 0x1c) == expected
            assert cpu.reg_read(x.UC_X86_REG_ESP) == stack
            layouts += 1
    assert layouts == 96



H = "\nreturn function(target,missing,finishAtTarget)\n local q,r,me,hero,bed={},{},{},{},{}\n local lines,events={},{}\n local frames,timer,resource,held=0,41,23,false\n local function log(x)events[#events+1]=x end\n function q:GetHero()return hero end\n function q:RetailResources()return r end\n function q:NewScriptFrame(actor)assert(actor==me);frames=frames+1;assert(frames<30);return true end\n function q:IsActiveThreadTerminating()return #lines>=target end\n function r:NewResource()assert(not held);held=true;log('new');return resource end\n function r:PrepareResource(id)assert(id==resource and held)end\n function r:TryAcquire(id,actor,kind)assert(id==resource and actor==me and kind==4);return true end\n function r:ReleaseResource(id)assert(id==resource and held);held=false;log('release')end\n function q:RegisterTimer()log('register');return timer end\n function q:DeregisterTimer(id)assert(id==timer);log('deregister')end\n function q:GetSleepingPositionAndOrientationFromBed(actor,b)assert(actor==me and b==bed);return {pos={x=1,y=2,z=3},orient={x=1,y=1,z=0}} end\n function q:GetThingWithScriptName(name)assert(name=='SickChildBed');return bed end\n function q:EntityTeleportToPosition(actor,pos,angle,a,b)assert(actor==me and pos.x==1 and type(angle)=='number' and not a and not b);log('teleport')end\n function q:ReadGlobalGameDataString(offset)assert(offset==1864);return 'ANIM' end\n function r:PlayLoopingAnimation(id,key,a,b,c,d,e,f,g,h)assert(id==resource and key=='ANIM' and a==-1 and not b and not c and not d and e and f and not g and not h);log('animate')end\n function q:SetBedAvailability(b,available)assert(b==bed and not available)end\n function q:SetThingAsUsable(b,usable)assert(b==bed and not usable)end\n function q:EntitySetTargetable(actor,value)assert(actor==me and not value)end\n function q:EntitySetAsRespondToHit(actor,value)assert(actor==me and not value)end\n function q:EntitySetAsKillable(actor,a,b)assert(actor==me and not a and not b)end\n function q:EntitySetAsToAddToComboMultiplierWhenHit(actor,value)assert(actor==me and not value)end\n function q:GetStateBool(key)assert(key=='FinishedQuest');log('state');return finishAtTarget and #lines>=target end\n function q:GetTimer(id)assert(id==timer);return 0 end\n function q:IsDistanceBetweenThingsUnder(a,b,d)assert(a==me and b==hero and d==14);return true end\n function me:IsTalkedToByHero()return false end\n function q:AddNewConversation(actor,a,b)assert(actor==me and not a and not b);log('conversation');return 100+#lines end\n function q:AddPersonToConversation(id,actor)assert(id==100+#lines and actor==hero)end\n function q:TextEntryExists(key)assert(key:find('TEXT_QST_B10_BOY_JABBERS_',1,true)==1);return key~=missing end\n function q:AddLineToConversation(id,key,speaker,listener,subtitle)\n  assert(id==100+#lines and speaker==me and listener==hero and not subtitle)\n  lines[#lines+1]=key;log('line:'..key)\n end\n function q:SetTimer(id,seconds)assert(id==timer and seconds==20);log('timer')end\n Main(q,me)\n assert(not held and frames==target+(finishAtTarget and 1 or 2))\n return table.concat(lines,'|'),table.concat(events,'|')\nend\n"


def test_generated_jabber_progression_fallback_wrap_and_cleanup():
    import re
    from lupa.lua54 import LuaRuntime
    normalize = lambda trace: re.sub(r'BOY_JABBERS_\d+', 'BOY_JABBERS_N', trace)
    traces = {}
    for style in ('draft', 'readable'):
        source = (ROOT / 'refs/script_recovery/lifted/SickChild' / style / 'FSE/V_SickChild/Entities/SickChild.lua').read_text()
        lua = LuaRuntime()
        lua.execute(source)
        for count in (1, 2, 3, 4):
            result = lua.execute(H)(count, '', False)
            expected = '|'.join('TEXT_QST_B10_BOY_JABBERS_' + str(i * 10) for i in range(1, count + 1))
            assert result[0] == expected
            key = ('ordinary', count)
            if style == 'draft': traces[key] = result
            else: assert result == traces[key]
        for count in (2, 3, 4):
            result = lua.execute(H)(count, 'TEXT_QST_B10_BOY_JABBERS_20', False)
            assert result[0] == '|'.join(['TEXT_QST_B10_BOY_JABBERS_10'] * count)
            key = ('missing', count)
            if style == 'draft': traces[key] = result
            else: assert result == traces[key]
        for count in (1, 2, 3):
            result = lua.execute(H)(count, '', True)
            assert result[0] == '|'.join('TEXT_QST_B10_BOY_JABBERS_' + str(i * 10) for i in range(1, count + 1))
            key = ('finished', count)
            if style == 'draft': traces[key] = result
            else: assert result == traces[key]
        for initial, expected in (
            (0x7ffffffc, 'TEXT_QST_B10_BOY_JABBERS_2147483644|TEXT_QST_B10_BOY_JABBERS_-2147483642'),
            (-8, 'TEXT_QST_B10_BOY_JABBERS_-8|TEXT_QST_B10_BOY_JABBERS_2'),
        ):
            assert source.count('jabberNextIndex = 10') == 1
            patched = source.replace('jabberNextIndex = 10', 'jabberNextIndex = ' + str(initial), 1)
            lua.execute(patched)
            result = lua.execute(H)(2, '', False)
            assert result[0] == expected
            key = ('wrap', initial)
            if style == 'draft': traces[key] = result
            else: assert result == traces[key]
            lua.execute(source)


def test_registered_jabber_review_preserves_later_string_slot():
    import json
    unit = json.loads((ROOT / 'refs/script_recovery/sick_child/units/V_SickChild.json').read_text())
    row = unit['reviewedStackScalarAliases']['0x00ec5de0']
    assert row['alias'] == 'jabberIndex' and row['additionalAliases'] == ['jabberNextIndex']
    assert row['editCounts'] == [1] * 6
    assert '0xffffffff' in row['edits'][-1][1]
    from tools.script_recovery.lift_native_lua import RData
    rd = RData()
    for address in (0xec60b3, 0xec61a9, 0xec61f7, 0xec6237, 0xec623c):
        raw = bytes.fromhex(row['nativeInstructions'][hex(address)])
        assert rd.bytes_at(address, len(raw)) == raw
