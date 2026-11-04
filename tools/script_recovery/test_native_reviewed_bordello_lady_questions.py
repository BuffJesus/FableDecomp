"""Execute both offer branches and verify the native question ABI."""
from copy import deepcopy
import json
from pathlib import Path

from lupa.lua54 import LuaRuntime, LuaError
import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_bordello_lady_questions import restore

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'work/codex_lua_lady_question_20261001/baseline/Bordello/draft/FSE/V_Bordello/Entities/BordelloLady.lua'


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    c = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    return c.native('0x00e3eb10'), unit['reviewedBordelloLadyQuestions']['0x00e3eb10'], c.rdata


def question_arguments(source, owned):
    lua = LuaRuntime()
    lua.globals().owned = owned
    lua.execute('''
        package.preload["V_Bordello.native_quest_helpers"] = function() return {} end
        local noop = function() end
        resources = setmetatable({}, {__index = function() return noop end})
        function resources:NewResource() return 1 end
        function resources:StartMovie() return 2 end
        function resources:TryAcquire() return true end
        quest = setmetatable({}, {__index = function() return noop end})
        function quest:RetailResources() return resources end
        function quest:NewScriptFrame() return true end
        function quest:IsActiveThreadTerminating() return false end
        function quest:GetStateBool(key) return key == "PlayerOwned" and owned end
        function quest:GetHealth() return 1 end
        function quest:GiveHeroYesNoQuestion(...)
            captured = table.pack(...); error("QUESTION_CAPTURED")
        end
        me = setmetatable({}, {__index = function() return noop end})
        function me:IsTalkedToByHero() return true end
        function me:IsPerformingScriptTask() return false end
        function me:Speak() return true end
    ''')
    lua.execute(source)
    lua.execute('function helper_E403D0(q, me, suffix) return "TEXT_QST_B13_POLLY_" .. suffix end')
    lua.execute('GetLHTSTag = helper_E403D0')
    with pytest.raises(LuaError, match='QUESTION_CAPTURED'):
        lua.globals().Main(lua.globals().quest, lua.globals().me)
    captured = lua.globals().captured
    return tuple(captured[i] for i in range(1, captured['n'] + 1))


@pytest.mark.parametrize('owned,suffix', [(False, 'PAID'), (True, 'FREE')])
def test_full_offer_branch_passes_native_question_arguments(evidence, owned, suffix):
    fn, row, rdata = evidence
    source = SOURCE.read_text()
    expected = (f'TEXT_QST_B13_POLLY_PARTY_{suffix}_QUESTION',
                'TEXT_OBJECT_HERO_ANSWER_YES', 'TEXT_OBJECT_HERO_ANSWER_NO', '', True)
    assert question_arguments(source, owned) != expected
    assert question_arguments(restore(source, fn, row, rdata), owned) == expected


@pytest.mark.parametrize('key', ['nativeSha256', 'sourceSha256'])
def test_stale_evidence_rejected(evidence, key):
    fn, row, rdata = evidence
    changed = deepcopy(row)
    changed[key] = '0' * 64
    with pytest.raises(ValueError, match='changed'):
        restore(SOURCE.read_text(), fn, changed, rdata)


def test_changed_lifted_operand_rejected(evidence):
    fn, row, rdata = evidence
    source = SOURCE.read_text().replace('uVar10, pppuVar20,', 'uVar10, different,')
    with pytest.raises(ValueError, match='lifted question call changed'):
        restore(source, fn, row, rdata)


def test_changed_argument_window_rejected(evidence):
    fn, row, rdata = evidence
    changed = deepcopy(row)
    changed['nativeArgumentWindows']['0x00e3f159'] = '90'
    with pytest.raises(ValueError, match='argument setup changed'):
        restore(SOURCE.read_text(), fn, changed, rdata)


def test_unregistered_function_unchanged(evidence):
    fn, _, rdata = evidence
    assert restore('unrelated', fn, None, rdata) == 'unrelated'
