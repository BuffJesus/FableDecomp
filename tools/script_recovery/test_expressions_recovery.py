"""Execute the recovered teleport branches, including resource/list and vector operands."""
import json
from pathlib import Path

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.lift_native_lua import converter_signatures
from tools.script_recovery.native_evidence_lowering import fold_stack_vector_builds

ROOT = Path(__file__).resolve().parents[2]


def test_recall_vector_tag_preserves_other_position_api_contracts():
    position = {'scope': 'Entity', 'returnType': 'sol::table', 'parameters': []}
    recall = {'scope': 'Quest', 'returnType': 'sol::table', 'parameters': []}
    result = converter_signatures({'GetPos': position, 'GetGuildSealRecallPos': recall})
    assert result['GetPos'] == position
    assert result['GetGuildSealRecallPos']['returnNativeKind'] == 'vector'


@pytest.fixture(scope='module', params=['draft', 'readable'])
def teleport_source(tmp_path_factory, request):
    if request.param == 'readable':
        return (ROOT / 'refs/script_recovery/lifted/Expressions/readable/FSE'
                / 'Global_TeleportToHeroGuild/Global_TeleportToHeroGuild.lua').read_text()
    evidence = ROOT / 'refs/script_recovery/expressions'
    out = tmp_path_factory.mktemp('expressions')
    converter = UnitConverter(evidence / 'translation_unit_typed.json')
    converter.convert(json.loads((evidence / 'units/Global_TeleportToHeroGuild.json').read_text()), out)
    return (out / 'FSE/Global_TeleportToHeroGuild/Global_TeleportToHeroGuild.lua').read_text()


HARNESS = '''
return function(source, near, inside, count, recallX, stopAtFollower)
    local trace, followers, checks = {}, {}, 0
    local function record(...) trace[#trace + 1] = {...} end
    for i = 1, count do followers[i] = {name = 'follower' .. i} end
    local hero = {}
    function hero:GetPos() return {x = 12, y = 34, z = 56} end
    function hero:GetAngleXY() return 0.75 end
    local marker = {}
    function marker:IsNull() return false end
    function marker:IsAlive() return true end
    local resource = {}
    function resource:NewResource() return 17 end
    function resource:TryAcquire(id, actor, priority)
        assert(id == 17 and actor == hero and priority == 4)
        record('acquire')
    end
    function resource:PerformExpression(id, actor, expression)
        assert(id == 17 and expression == 'EXPRESSION_WAIT')
        record('expression', actor.name)
        checks = checks + 1
    end
    function resource:ReleaseResource(id) assert(id == 17); record('release') end
    local quest = {}
    function quest:RetailResources() return resource end
    function quest:GetHero() return hero end
    function quest:NewScriptFrame() return true end
    function quest:IsActiveThreadTerminating() return stopAtFollower > 0 and checks == stopAtFollower end
    function quest:IsTeleportingActive() return true end
    function quest:GetThingWithScriptName(name) assert(name == 'HERO_GUILD_TELEPORT_MARKER'); return marker end
    function quest:IsDistanceBetweenThingsUnder(a,b,d) assert(a == marker and b == hero and d == 5); return near end
    function quest:IsRegionLoaded(name) assert(name == 'HeroGuildComplexInside'); return inside end
    function quest:GetFollowingEntityList(actor) assert(actor == hero); return followers end
    function quest:SetGuildSealRecallLocation(pos,angle) record('recall',pos.x,pos.y,pos.z,angle) end
    function quest:EntityTeleportToThing(actor,target,flag)
        assert(actor == hero and target == marker and flag == true); record('guild')
    end
    function quest:GetGuildSealRecallPos() return {x = recallX, y = 0, z = 0} end
    function quest:GetGuildSealRecallAngleXY() return 1.5 end
    function quest:EntityTeleportToPosition(actor,pos,angle,b1,b2)
        assert(actor == hero and angle == 0 and b1 and b2); record('position',pos.x,pos.y,pos.z)
    end
    function quest:EntitySetFacingAngle(actor,angle,flag)
        assert(actor == hero and flag); record('angle',angle)
    end
    function quest:DeactivateQuestLater(name,delay)
        assert(name == 'Global_TeleportToHeroGuild' and delay == 0); record('deactivate')
    end
    assert(load(source))()
    Main(quest)
    return trace
end
'''


def run(source, near=False, inside=False, followers=0, recall=0, stop=0):
    lua = LuaRuntime()
    result = lua.execute(HARNESS)(source, near, inside, followers, recall, stop)
    return [tuple(row.values()) for row in result.values()]


@pytest.mark.parametrize('followers', [0, 1, 3])
def test_departure_preserves_each_follower_and_hero_recall(teleport_source, followers):
    assert run(teleport_source, followers=followers) == (
        [('acquire',)] + [('expression', f'follower{i}') for i in range(1, followers + 1)]
        + [('recall', 12, 34, 56, 0.75), ('guild',), ('release',), ('deactivate',)])


@pytest.mark.parametrize('near,inside', [(True, False), (False, True)])
def test_return_teleport_restores_position_and_angle_then_clears_recall(teleport_source, near, inside):
    assert run(teleport_source, near=near, inside=inside, recall=9) == [
        ('position', 9, 0, 0), ('angle', 1.5), ('recall', 0, 0, 0, 0), ('deactivate',)]


@pytest.mark.parametrize('recall', [0, 0.00001])
def test_empty_or_negligible_recall_does_not_teleport(teleport_source, recall):
    assert run(teleport_source, near=True, recall=recall) == [
        ('recall', 0, 0, 0, 0), ('deactivate',)]


def test_termination_releases_resource_without_teleporting(teleport_source):
    assert run(teleport_source, followers=3, stop=1) == [
        ('acquire',), ('expression', 'follower1'), ('release',)]


@pytest.mark.parametrize('decl,intervening', [('CScriptThing', ''), ('C3DVector_bv', '    iVar = 2;\n')])
def test_zero_vector_requires_typed_immediate_consumption(decl, intervening):
    source = (f'{decl} CStack_1c;\n'
              '    CStack_1c._0_4_ = 0;\n    CStack_1c._4_4_ = 0;\n    CStack_1c._8_4_ = 0;\n'
              + intervening + '    f(&CStack_1c, 0.0);\n')
    assert fold_stack_vector_builds(source) == source
