"""Pinned Madame beer presentation evidence and no stale item names."""
from copy import deepcopy
import json
from pathlib import Path

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_madame_presented_item import (
    AFTER, BEFORE, DIAGNOSTIC, restore,
)


ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    return (converter.native('0x00e3bb70'),
            unit['reviewedMadamePresentedItem']['0x00e3bb70'], converter.rdata)


def test_presented_tankard_is_only_beer_branch(evidence):
    fn, row, rdata = evidence
    changed, todo = restore(BEFORE, [DIAGNOSTIC], fn, row, rdata)
    assert changed == AFTER and todo == []
    assert restore(BEFORE, [DIAGNOSTIC], fn, None, rdata) == (BEFORE, [DIAGNOSTIC])
    lua = LuaRuntime()
    for presented, expected in ((None, False), ('OBJECT_APPLE', False),
                                ('OBJECT_BEER_TANKARD', True)):
        def message():
            if presented is not None:
                lua.globals().g_PresentedItemName = presented
            return presented is not None
        lua.globals().me = {'MsgIsPresentedWithItem': lambda _self: message()}
        lua.globals().g_PresentedItemName = 'OBJECT_BEER_TANKARD'  # stale previous poll
        script = (AFTER + '\n                hit = true\n'
                  '            ::FLOW_past_lab_00e3bf28::\n'
                  '        end\n        return hit == true')
        lua.globals().hit = None
        assert lua.execute(script) is expected


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('nativeSha256', '0' * 64),
    lambda row: row.__setitem__('sourceSha256', '0' * 64),
    lambda row: row.__setitem__('sidecarSha256', '0' * 64),
    lambda row: row['nativeInstructions'].__setitem__('0x00E3BF13', '00'),
    lambda row: row.__setitem__('itemName', 'OBJECT_APPLE'),
])
def test_stale_evidence_rejected(evidence, mutation):
    fn, row, rdata = evidence
    changed = deepcopy(row)
    mutation(changed)
    with pytest.raises(ValueError, match='reviewed Madame presented item'):
        restore(BEFORE, [DIAGNOSTIC], fn, changed, rdata)


def test_unrelated_lifted_branch_rejected(evidence):
    fn, row, rdata = evidence
    with pytest.raises(ValueError, match='lifted comparison changed'):
        restore(BEFORE.replace('xStack_120', 'xStack_121'), [DIAGNOSTIC], fn, row, rdata)
