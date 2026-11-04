"""Retail indexed client release and matching quest state behavior."""
from copy import deepcopy
import json
from pathlib import Path

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_bordello_client_release import (
    DIAGNOSTIC, NEW, OLD, restore,
)


ROOT = Path(__file__).resolve().parents[2]
SOURCE = ('quest:SetStateInt("ClientsAlive", quest:GetStateInt("ClientsAlive") + -1)\n'
          + OLD)


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    return (converter.native('0x00e3b230'),
            unit['reviewedBordelloClientRelease']['0x00e3b230'], converter.rdata,
            unit['quest'], unit['entities']['BordelloClient'])


def test_native_indexed_release(evidence):
    fn, row, rdata, quest, entity = evidence
    result, todo = restore(SOURCE, [DIAGNOSTIC], fn, row, rdata, quest, entity)
    assert NEW in result and todo == []
    assert restore(SOURCE, [DIAGNOSTIC], fn, None, rdata, quest, entity) == (
        SOURCE, [DIAGNOSTIC])


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('nativeSha256', '0' * 64),
    lambda row: row.__setitem__('sourceSha256', '0' * 64),
    lambda row: row['nativeInstructions'].__setitem__('0x00E3B23C', '00'),
])
def test_stale_release_evidence_fails(evidence, mutation):
    fn, row, rdata, quest, entity = evidence
    changed = deepcopy(row)
    mutation(changed)
    with pytest.raises(ValueError, match='reviewed BordelloClient release'):
        restore(SOURCE, [DIAGNOSTIC], fn, changed, rdata, quest, entity)


def test_wrong_field_layout_or_source_fails(evidence):
    fn, row, rdata, quest, entity = evidence
    wrong_quest = deepcopy(quest)
    wrong_quest['fields']['0x59'] = ['OtherFlag', 'Bool']
    with pytest.raises(ValueError, match='field layout changed'):
        restore(SOURCE, [DIAGNOSTIC], fn, row, rdata, wrong_quest, entity)
    with pytest.raises(ValueError, match='lifted release changed'):
        restore(SOURCE.replace('ClientsAlive', 'ClientsTotal'), [DIAGNOSTIC],
                fn, row, rdata, quest, entity)


@pytest.mark.parametrize('client_id,slot', [(0, 0), (1, 1), (2, 2), (None, 0)])
def test_releasing_one_client_preserves_other_slots(evidence, client_id, slot):
    fn, row, rdata, quest, entity = evidence
    source, _ = restore(SOURCE, [DIAGNOSTIC], fn, row, rdata, quest, entity)
    lua = LuaRuntime(unpack_returned_tuples=True)
    lua.execute('''
        flags = {ClientInUse_0=true, ClientInUse_1=true, ClientInUse_2=true}
        count = 3
        quest = {}
        function quest:GetStateInt(name) assert(name == "ClientsAlive"); return count end
        function quest:SetStateInt(name, value) assert(name == "ClientsAlive"); count = value end
        function quest:SetStateBool(name, value) flags[name] = value end
        __native_entity_state = {}
        function __native_entity_state:GetStateInt(name)
            assert(name == "ClientID")
            return clientID
        end
    ''')
    lua.globals().clientID = client_id
    lua.execute(source)
    assert lua.globals().count == 2
    for index in range(3):
        assert lua.globals().flags[f'ClientInUse_{index}'] is (index != slot)
