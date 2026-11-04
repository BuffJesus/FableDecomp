"""Observe cutscene selection, map arguments and payment at the helper boundary."""
from copy import deepcopy
import json
from pathlib import Path

from lupa.lua54 import LuaRuntime, LuaError
import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_bordello_lady_cutscene_map import restore

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'work/codex_lua_lady_cutscene_map_20261001/baseline/Bordello/draft/FSE/V_Bordello/Entities/BordelloLady.lua'


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    c = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    return c.native('0x00e3eb10'), unit['reviewedBordelloLadyCutsceneMap']['0x00e3eb10'], c.rdata


def cutscene_setup(source, name, owned, had_sex):
    lua = LuaRuntime()
    lua.globals().ladyName, lua.globals().owned, lua.globals().hadSex = name, owned, had_sex
    lua.execute('''
        map, debit, frames = {}, 0, 0
        local noop = function() end
        local capture = function(q, me, name, flag)
            scene, sceneFlag = name, flag; error("CUTSCENE_CAPTURED")
        end
        package.preload["V_Bordello.native_quest_helpers"] = function()
            return {helper_E3E720 = capture, PlayCutscene = capture}
        end
        resources = setmetatable({}, {__index = function() return noop end})
        function resources:NewResource() return 1 end
        function resources:StartMovie() return 2 end
        function resources:TryAcquire() return true end
        function resources:MemberStringMap(name) assert(name == "csargs"); return map end
        function resources:SetString(id, key, value) assert(id == map); map[key] = value end
        quest = setmetatable({}, {__index = function() return noop end})
        function quest:RetailResources() return resources end
        function quest:NewScriptFrame() frames = frames + 1; assert(frames < 10); return true end
        function quest:IsActiveThreadTerminating() return false end
        function quest:GetStateBool(key)
            if key == "PlayerOwned" then return owned end
            if key == "HadSex" then return hadSex end
            return false
        end
        function quest:GetHealth() return 1 end
        function quest:GetHeroGold() return 5000 end
        function quest:MsgIsQuestionAnsweredYesOrNo() return 1 end
        function quest:GiveHeroGold(amount) debit = debit + amount end
        me = setmetatable({}, {__index = function() return noop end})
        function me:GetDataString() return ladyName end
        function me:IsTalkedToByHero() return true end
        function me:IsPerformingScriptTask() return false end
        function me:Speak() return true end
    ''')
    lua.execute(source)
    lua.execute('''
        function helper_E403D0(q, me, suffix) return "TEXT_QST_B13_" .. ladyName .. "_" .. suffix end
        GetLHTSTag = helper_E403D0
    ''')
    lua.globals().Init(lua.globals().quest, lua.globals().me)
    with pytest.raises(LuaError, match='CUTSCENE_CAPTURED'):
        lua.globals().Main(lua.globals().quest, lua.globals().me)
    return {'scene': lua.globals().scene, 'flag': lua.globals().sceneFlag,
            'map': dict(lua.globals().map.items()), 'debit': lua.globals().debit}


def expected_setup(name, owned, had_sex):
    scene = 'CS_BORDELLO_PAYINGFORSEX'
    # Native free branch E3F4A0/E3F4FD always selects the short variant;
    # only the paid branch tests the parent quest's HadSex field.
    if owned or had_sex: scene += '_QUICKIE'
    if name == 'HEDWIG': scene += '_HEDWIG'
    return {'scene': scene, 'flag': True, 'debit': 0 if owned else -({'POLLY': 50, 'HEDWIG': 2000}[name]),
            'map': {'$SEXTALK': 'TEXT_CS_B13_SEX_' + name + ('_BOSS' if owned else ''),
                    '$ENDLINE': 'TEXT_QST_B13_' + name + ('_PARTY_FREE_FINISHING_UP' if owned else '_PARTY_PAID_PLEASED')}}


@pytest.mark.parametrize('name', ['POLLY', 'HEDWIG'])
@pytest.mark.parametrize('owned', [False, True])
@pytest.mark.parametrize('had_sex', [False, True])
def test_all_cutscene_entry_variants(evidence, name, owned, had_sex):
    changed = restore(SOURCE.read_text(), *evidence)
    assert cutscene_setup(changed, name, owned, had_sex) == expected_setup(name, owned, had_sex)


def test_baseline_paid_map_has_wrong_key():
    result = cutscene_setup(SOURCE.read_text(), 'POLLY', False, False)
    assert '$SEXTALK' not in result['map']
    assert result['map']['TEXT_QST_B13_POLLY_PARTY_PAID_FOLLOW_ME'] == 'TEXT_CS_B13_SEX_POLLY'


@pytest.mark.parametrize('key', ['nativeSha256', 'sourceSha256', 'nativeSetup'])
def test_stale_evidence_rejected(evidence, key):
    fn, row, rdata = evidence
    row = deepcopy(row)
    row[key] = '00'
    with pytest.raises(ValueError, match='changed'):
        restore(SOURCE.read_text(), fn, row, rdata)


def test_different_lifted_key_rejected(evidence):
    source = SOURCE.read_text().replace('"csargs"), pCVar7, pCVar12', '"csargs"), other, pCVar12')
    with pytest.raises(ValueError, match='lifted map assignment changed'):
        restore(source, *evidence)
