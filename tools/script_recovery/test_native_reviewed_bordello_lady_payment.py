"""Check debit amounts after native Init and the full paid-offer branch."""
from copy import deepcopy
import json
from pathlib import Path

from lupa.lua54 import LuaRuntime, LuaError
import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_bordello_lady_payment import restore

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'work/codex_lua_lady_payment_20261001/baseline/Bordello/draft/FSE/V_Bordello/Entities/BordelloLady.lua'


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    c = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    return (c.native('0x00e3eb10'), unit['reviewedBordelloLadyPayment']['0x00e3eb10'],
            c.rdata, unit['entities']['BordelloLady']['fields'])


def payment(source, name, gold, accept):
    lua = LuaRuntime()
    lua.globals().ladyName, lua.globals().gold, lua.globals().accept = name, gold, accept
    lua.execute('''
        package.preload["V_Bordello.native_quest_helpers"] = function() return {} end
        frames, talks = 0, 0
        local noop = function() end
        resources = setmetatable({}, {__index = function() return noop end})
        function resources:NewResource() return 1 end
        function resources:StartMovie() return 2 end
        function resources:TryAcquire() return true end
        function resources:ScriptThing() return {IsNull = function() return false end} end
        quest = setmetatable({}, {__index = function() return noop end})
        function quest:RetailResources() return resources end
        function quest:NewScriptFrame() frames = frames + 1; assert(frames < 20); return frames < 4 end
        function quest:IsActiveThreadTerminating() return frames >= 4 end
        function quest:GetStateBool() return false end
        function quest:GetHealth() return 1 end
        function quest:GetHeroGold() return gold end
        function quest:MsgIsQuestionAnsweredYesOrNo() return accept and 1 or 0 end
        function quest:GiveHeroGold(amount) debit = amount; error("PAYMENT_CAPTURED") end
        me = setmetatable({}, {__index = function() return noop end})
        function me:GetDataString() return ladyName end
        function me:IsTalkedToByHero() talks = talks + 1; return talks == 1 end
        function me:IsPerformingScriptTask() return false end
        function me:Speak() return true end
        function me:MsgIsHitByHero() return false end
        function me:MsgIsHitByAnySpecialAbilityFromHero() return false end
    ''')
    lua.execute(source)
    lua.execute('''
        function helper_E403D0(q, me, suffix) return "TEXT_QST_B13_" .. ladyName .. "_" .. suffix end
        GetLHTSTag = helper_E403D0
    ''')
    lua.globals().Init(lua.globals().quest, lua.globals().me)
    try:
        lua.globals().Main(lua.globals().quest, lua.globals().me)
    except LuaError as exc:
        assert 'PAYMENT_CAPTURED' in str(exc), str(exc)
    return lua.globals().debit


@pytest.mark.parametrize('name,cost', [('POLLY', 50), ('AMELIA', 100), ('LUCREZIA', 200), ('SOPHIA', 1000)])
def test_acceptance_debits_initialized_price(evidence, name, cost):
    source = SOURCE.read_text()
    assert payment(source, name, cost, True) == 0
    assert payment(restore(source, *evidence), name, cost, True) == -cost


@pytest.mark.parametrize('accept,gold', [(False, 1000), (True, 49)])
def test_decline_or_insufficient_funds_does_not_debit(evidence, accept, gold):
    assert payment(restore(SOURCE.read_text(), *evidence), 'POLLY', gold, accept) is None


def test_price_field_type_is_required(evidence):
    fn, row, rdata, fields = evidence
    fields = deepcopy(fields)
    fields['0x20'] = ['GoldRequired', 'Float']
    with pytest.raises(ValueError, match='entity price field changed'):
        restore(SOURCE.read_text(), fn, row, rdata, fields)


@pytest.mark.parametrize('key', ['nativeSha256', 'sourceSha256'])
def test_stale_evidence_rejected(evidence, key):
    fn, row, rdata, fields = evidence
    row = deepcopy(row)
    row[key] = '0' * 64
    with pytest.raises(ValueError, match='changed'):
        restore(SOURCE.read_text(), fn, row, rdata, fields)


def test_changed_call_operand_rejected(evidence):
    with pytest.raises(ValueError, match='lifted payment operand changed'):
        restore(SOURCE.read_text().replace('GiveHeroGold(fVar18)', 'GiveHeroGold(other)'), *evidence)
