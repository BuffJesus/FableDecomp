"""Execute generated Picklock/Steal progress, cancellation, and cleanup paths."""
import json
from pathlib import Path

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.build_readable_unit import readable_file
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_evidence_lowering import (
    fold_reused_string_arithmetic, fold_upper_byte_flags,
)

ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module', params=['Picklock', 'Steal'])
def source(request, tmp_path_factory):
    name = 'Expression_' + request.param
    evidence = ROOT / 'refs/script_recovery/expressions'
    out = tmp_path_factory.mktemp(name)
    UnitConverter(evidence / 'translation_unit_typed.json').convert(
        json.loads((evidence / 'units' / (name + '.json')).read_text()), out)
    return (out / 'FSE' / name / (name + '.lua')).read_text()


HARNESS = '''
return function(source, scenario, duration)
    local frame, rewards, destroyed, removed, deactivated = 0, 0, 0, 0, 0
    local bars = {}
    local target, hero, resources, quest = {}, {}, {}, {}
    function target:IsNull() return scenario == 'null' and frame >= 3 end
    function target:IsAlive() return not (scenario == 'dead' and frame >= 3) end
    function hero:MsgIsHitBy(s) assert(s == ''); return scenario == 'hit' and frame >= 3 end
    function resources:StartMovie(s) assert(s == ''); return 17 end
    function resources:DestroyMovie(id) assert(id == 17); destroyed = destroyed + 1 end
    function quest:RetailResources() return resources end
    function quest:GetHero() return hero end
    function quest:NewScriptFrame()
        frame = frame + 1; assert(frame < 20, 'progress loop failed to terminate')
        return not self:IsActiveThreadTerminating()
    end
    function quest:IsActiveThreadTerminating() return scenario == 'terminate' and frame >= 3 end
    function quest:GetActiveQuestName() return 'expression' end
    function quest:SetQuestAsPersistent(name, value) assert(name == 'expression' and not value) end
    function quest:GetHeroTargetedThing() return target end
    function quest:IsEntityStealable(t) assert(t == target); return scenario ~= 'ineligible' end
    quest.IsEntityPickLockable = quest.IsEntityStealable
    function quest:IsInMovieSequence() return false end
    function quest:DisplayMiniGameInfo() end
    function quest:GetStealDuration(t) assert(t == target); return duration end
    function quest:ReadGlobalGameDataFloat() return duration end
    function quest:GetHeroStatLevel(n) assert(n == 5); return 1 end
    function quest:GetHeroStatMax(n) assert(n == 5); return 3 end
    function quest:GetConstantFPS() return 2 end
    function quest:EntityPostOpinionDeedKeepSearchingForWitnesses(h,n,t)
        assert(h == hero and t == target and (n == 4 or n == 6)); return 23
    end
    function quest:StartSneaking() end
    function quest:EntitySetFacingAngleTowardsThing(h,t,b) assert(h == hero and t == target and b) end
    function quest:IsDPadButtonHeldForExpression() return scenario ~= 'release' or frame < 3 end
    function quest:IsDeedWitnessed(id) assert(id == 23); return scenario == 'witness' and frame >= 3 end
    function quest:UpdateMiniGameInfoBar(value) bars[#bars + 1] = value end
    function quest:RemoveOpinionDeedStillSearchingForWitnesses(h,id)
        assert(h == hero and id == 23); removed = removed + 1
    end
    function quest:EntitySetAsStolen(t) assert(t == target); rewards = rewards + 1 end
    quest.EntitySetAsPickLocked = quest.EntitySetAsStolen
    function quest:ChangeHeroMoralityDueToTheft() end
    quest.ChangeHeroMoralityDueToPicklock = quest.ChangeHeroMoralityDueToTheft
    function quest:DeactivateQuestLater(name,delay)
        assert(name == 'expression' and delay == 0); deactivated = deactivated + 1
    end
    math.random = function() return 0 end
    assert(load(source))(); Main(quest)
    return bars, rewards, destroyed, removed, deactivated
end
'''


@pytest.mark.parametrize('readable', [False, True])
@pytest.mark.parametrize('scenario', ['complete', 'release', 'hit', 'witness', 'dead', 'null', 'terminate', 'ineligible'])
@pytest.mark.parametrize('duration', [1, 1.125])
def test_progress_and_cancellation(source, readable, scenario, duration):
    if readable:
        source, _ = readable_file(source, style=True, frame_returns_alive=True)
    bars, rewards, destroyed, removed, deactivated = LuaRuntime().execute(HARNESS)(source, scenario, duration)
    bars = list(bars.values())
    assert rewards == (scenario == 'complete')
    assert destroyed == (scenario != 'ineligible')
    assert removed == (scenario not in ('ineligible', 'terminate'))
    assert deactivated == (scenario != 'terminate')
    if scenario == 'ineligible':
        assert bars == []
    else:
        ticks = duration * 4
        expected = [1 - remaining / ticks for remaining in range(int(ticks), -1, -1)]
        assert bars == pytest.approx(expected if scenario == 'complete' else expected[:1 if scenario == 'terminate' else 2])


@pytest.mark.parametrize('suffix', ['consume(&CStack_20);\nx = (float)CStack_20;', 'x = CStack_20;'])
def test_arithmetic_requires_numeric_read_before_string_use(suffix):
    source = 'CStack_20 = (CCharString)(iVar1 + 1);\n' + suffix
    assert fold_reused_string_arithmetic(source) == source


def test_numeric_phase_preserves_later_string_lifetime():
    source = ('CStack_20 = (CCharString)(iVar1 % 100);\n'
              'x = (float)(int)CStack_20;\n'
              'CCharString::CCharString(&CStack_20,"name",-1);\n'
              'consume(&CStack_20);')
    result = fold_reused_string_arithmetic(source)
    assert 'i_stk_20 = (iVar1 % 100);' in result
    assert 'x = (float)i_stk_20;' in result
    assert result[result.index('CCharString::'):] == source[source.index('CCharString::'):]


@pytest.mark.parametrize('extra', ['observe(uVar1);', 'x = (short)uVar1;', 'uVar1 = 0x10000;',
                                  'x = (char)(uVar1 >> 0x10) + 2;'])
def test_packed_flags_reject_unproven_word_uses(extra):
    source = ('ushort low;\nuVar1 = (uint)low;\n'
              'uVar1 = CONCAT13(1,CONCAT12(1,(short)uVar1));\n'
              'uVar1 = CONCAT13(1,(int3)uVar1);\n' + extra + '\n'
              'uVar1 = CONCAT13((char)(uVar1 >> 0x18),CONCAT12(1,(short)uVar1));\n')
    assert fold_upper_byte_flags(source) == source


@pytest.mark.parametrize('rhs,read', [('*(iVar1) + 1', '(float)CStack_20'),
                                    ('(int)CVar1 + 0x38', '(int)CStack_20'),
                                    ('call(iVar1) + 1', '(float)CStack_20')])
def test_arithmetic_does_not_guess_pointer_or_call_results(rhs, read):
    source = 'CStack_20 = (CCharString)(' + rhs + ');\nx = ' + read + ';'
    assert fold_reused_string_arithmetic(source) == source
