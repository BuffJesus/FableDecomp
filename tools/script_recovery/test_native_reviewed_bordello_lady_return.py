"""Declining returns to interaction polling, with native cleanup and hit gating."""
from copy import deepcopy
import json
from pathlib import Path

from lupa.lua54 import LuaRuntime
import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_bordello_lady_return import restore

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'work/codex_lua_lady_return_20261001/baseline/Bordello/draft/FSE/V_Bordello/Entities/BordelloLady.lua'


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    c = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    return c.native('0x00e3eb10'), unit['reviewedBordelloLadyReturn']['0x00e3eb10'], c.rdata


def run_decline(source, *, owned=False, hit=False, magic=False, heal=False, repeat=False):
    lua = LuaRuntime()
    for key, value in locals().copy().items():
        if key in {'owned', 'hit', 'magic', 'heal', 'repeat'}:
            lua.globals()[key] = value
    lua.execute('''
        package.preload["V_Bordello.native_quest_helpers"] = function() return {} end
        frames, talks, movies, destroyed, offers, cowers, released = 0, 0, 0, 0, 0, 0, 0
        pauses = {}
        local hitSeen, magicSeen, secondTalk = false, false, false
        local noop = function() end
        resources = setmetatable({}, {__index = function() return noop end})
        function resources:NewResource() return 1 end
        function resources:StartMovie() movies = movies + 1; assert(movies < 10); return 100 + movies end
        function resources:DestroyMovie(id) assert(id == 100 + movies); destroyed = destroyed + 1 end
        function resources:ReleaseResource(id) assert(id == 1); released = released + 1 end
        function resources:TryAcquire() return true end
        function resources:ScriptThing() return {IsNull = function() return false end} end
        quest = setmetatable({}, {__index = function() return noop end})
        function quest:RetailResources() return resources end
        function quest:NewScriptFrame() frames = frames + 1; assert(frames < 20); return frames < 4 end
        function quest:IsActiveThreadTerminating() return frames >= 4 end
        function quest:GetStateBool(key) return key == "PlayerOwned" and owned end
        function quest:GetHealth() return 1 end
        function quest:GiveHeroYesNoQuestion() offers = offers + 1 end
        function quest:MsgIsQuestionAnsweredYesOrNo() return 0 end
        function quest:PauseAllNonScriptedEntities(value) pauses[#pauses+1] = value end
        me = setmetatable({}, {__index = function() return noop end})
        function me:IsTalkedToByHero()
            talks = talks + 1
            if talks == 1 then return true end
            if _G["repeat"] and frames >= 3 and not secondTalk then secondTalk = true; return true end
            return false
        end
        function me:IsPerformingScriptTask() return false end
        function me:Speak() return true end
        function me:MsgIsHitByHero()
            if hit and offers > 0 and not hitSeen then hitSeen = true; return true end
            return false
        end
        function me:MsgIsHitByAnySpecialAbilityFromHero()
            if magic and offers > 0 and not magicSeen then magicSeen = true; return true end
            return false
        end
        function me:MsgIsHitByHeroSpecialAbility(ability) assert(ability == 14); return heal end
        function me:PlayAnimation() cowers = cowers + 1; return true end
    ''')
    lua.execute(source)
    lua.execute('''
        function helper_E403D0(q, me, suffix) return "TEXT_QST_B13_POLLY_" .. suffix end
        GetLHTSTag = helper_E403D0
    ''')
    lua.globals().Main(lua.globals().quest, lua.globals().me)
    result = {k: lua.globals()[k] for k in ['frames', 'talks', 'movies', 'destroyed', 'offers', 'cowers', 'released']}
    result['pauses'] = list(lua.globals().pauses.values())
    return result


@pytest.mark.parametrize('owned', [False, True])
@pytest.mark.parametrize('hit,magic,heal,cowers', [
    (False, False, False, 0), (True, False, False, 1),
    (False, True, False, 1), (False, True, True, 0),
])
def test_decline_returns_to_polling_and_preserves_hit_semantics(evidence, owned, hit, magic, heal, cowers):
    fn, row, rdata = evidence
    changed = restore(SOURCE.read_text(), fn, row, rdata)
    result = run_decline(changed, owned=owned, hit=hit, magic=magic, heal=heal)
    assert result['frames'] == 4
    assert result['talks'] == 3
    assert result['movies'] == result['destroyed'] == result['offers'] == 1
    assert result['pauses'] == [True, False]
    assert result['cowers'] == cowers


def test_fresh_talk_event_can_start_second_conversation(evidence):
    fn, row, rdata = evidence
    result = run_decline(restore(SOURCE.read_text(), fn, row, rdata), repeat=True)
    assert result['movies'] == result['destroyed'] == result['offers'] == 2
    assert result['pauses'] == [True, False, True, False]
    assert result['cowers'] == 0
    assert result['released'] == 1


def test_baseline_repeats_without_polling(evidence):
    result = run_decline(SOURCE.read_text())
    assert result['talks'] == 1 and result['offers'] == 3
    assert result['pauses'] == [True, True, True]
    assert result['cowers'] == 3


@pytest.mark.parametrize('key', ['nativeSha256', 'sourceSha256'])
def test_stale_evidence_rejected(evidence, key):
    fn, row, rdata = evidence
    changed = deepcopy(row)
    changed[key] = '0' * 64
    with pytest.raises(ValueError, match='changed'):
        restore(SOURCE.read_text(), fn, changed, rdata)


def test_different_backedge_rejected(evidence):
    fn, row, rdata = evidence
    with pytest.raises(ValueError, match='lifted return path changed'):
        restore(SOURCE.read_text().replace('goto FLOW_after_lab_00e40085', 'goto DIFFERENT'), fn, row, rdata)
