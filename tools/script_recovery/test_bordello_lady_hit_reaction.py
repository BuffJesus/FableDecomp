"""The native hit flag gates cowering and excludes the hero's healing spell."""
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_reviewed_bordello_lady_resource_lifetime import restore
from tools.script_recovery.test_native_reviewed_bordello_lady_resource_lifetime import evidence, SOURCE


@pytest.mark.parametrize('hit,magic,heal,expected', [
    (False, False, False, 0), (True, False, False, 1),
    (False, True, False, 1), (False, True, True, 0),
])
def test_reaction_matches_selected_native_branch(evidence, hit, magic, heal, expected):
    fn, row, rdata = evidence
    source = restore(SOURCE.read_text(), fn, row, rdata)
    lua = LuaRuntime()
    lua.globals().hit, lua.globals().magic, lua.globals().heal = hit, magic, heal
    lua.execute('''
        package.preload["V_Bordello.native_quest_helpers"] = function() return {} end
        frames, cowers = 0, 0
        local noop = function() end
        resources = setmetatable({}, {__index = function() return noop end})
        function resources:NewResource() return 1 end
        function resources:TryAcquire(id, actor, priority)
            assert(id == 1 and priority == 4); return true
        end
        quest = setmetatable({}, {__index = function() return noop end})
        function quest:RetailResources() return resources end
        function quest:NewScriptFrame() frames = frames + 1; return frames < 4 end
        function quest:IsActiveThreadTerminating() return frames >= 4 end
        function quest:GetStateBool() return false end
        __native_entity_state = quest
        me = setmetatable({}, {__index = function() return noop end})
        function me:IsTalkedToByHero() return false end
        function me:MsgIsHitByHero() return hit end
        function me:MsgIsHitByAnySpecialAbilityFromHero() return magic end
        function me:MsgIsHitByHeroSpecialAbility(ability)
            assert(ability == 14); return heal
        end
        function me:PlayAnimation(name)
            assert(name == "ST_OPINION_FEAR_IDLE_COWERING")
            cowers = cowers + 1
        end
        function me:IsPerformingScriptTask() return false end
    ''')
    lua.execute(source)
    lua.globals().Main(lua.globals().quest, lua.globals().me)
    assert lua.globals().frames == 4
    assert (lua.globals().cowers > 0) == bool(expected)


def test_changed_hit_operand_is_rejected(evidence):
    fn, row, rdata = evidence
    source = SOURCE.read_text().replace('MsgIsHitByHeroSpecialAbility(0xe)',
                                      'MsgIsHitByHeroSpecialAbility(0xd)')
    with pytest.raises(ValueError, match='lifted hit flag changed'):
        restore(source, fn, row, rdata)
