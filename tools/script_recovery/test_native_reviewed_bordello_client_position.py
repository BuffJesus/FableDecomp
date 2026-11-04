"""Retail twin call sites for BordelloClient's controlled position."""
from copy import deepcopy
import json
from pathlib import Path

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_bordello_client_position import (
    NEW_CHECK, NEW_VALUE, OLD_CHECK, OLD_VALUE, restore,
)


ROOT = Path(__file__).resolve().parents[2]
SOURCE = ('xStack_118 = resources:NewResource()\n'
          'local first = resources:ScriptThing(xStack_118):GetPos()\n'
          f'{OLD_CHECK}\n'
          '  puVar8 = {x = 0, y = 0, z = 0}\n'
          f'else\n  {OLD_VALUE}\nend')


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    return (converter.native('0x00e44ea0'),
            unit['reviewedBordelloClientPosition']['0x00e44ea0'], converter.rdata)


def test_second_position_uses_same_controlled_resource(evidence):
    fn, row, rdata = evidence
    result, todo = restore(SOURCE, [row['diagnostic']], fn, row, rdata)
    assert NEW_CHECK in result and NEW_VALUE in result and todo == []
    assert 'xStack_e4' not in result
    assert restore(SOURCE, [row['diagnostic']], fn, None, rdata) == (
        SOURCE, [row['diagnostic']])


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('nativeSha256', '0' * 64),
    lambda row: row.__setitem__('sourceSha256', '0' * 64),
    lambda row: row['nativeInstructions'].__setitem__('0x00E45724', '00'),
    lambda row: row.__setitem__('diagnostic', 'wrong'),
])
def test_stale_position_evidence_fails(evidence, mutation):
    fn, row, rdata = evidence
    changed = deepcopy(row)
    mutation(changed)
    with pytest.raises(ValueError, match='reviewed BordelloClient position'):
        restore(SOURCE, [row['diagnostic']], fn, changed, rdata)


def test_other_pointer_is_not_rewritten(evidence):
    fn, row, rdata = evidence
    with pytest.raises(ValueError, match='lifted position lookup changed'):
        restore(SOURCE.replace('xStack_e4 == nil', 'xStack_e8 == nil'),
                [row['diagnostic']], fn, row, rdata)


def test_lua_position_and_null_fallback(evidence):
    fn, row, rdata = evidence
    result, _ = restore(SOURCE, [row['diagnostic']], fn, row, rdata)
    lua = LuaRuntime(unpack_returned_tuples=True)
    for null, expected in ((False, (4, 5, 6)), (True, (0, 0, 0))):
        lua.execute('''
            resources = {}
            function resources:NewResource() return 17 end
            function resources:ScriptThing(id)
                assert(id == 17)
                return {
                    IsNull = function() return isNull end,
                    GetPos = function() return {x = 4, y = 5, z = 6} end,
                }
            end
        ''')
        lua.globals().isNull = null
        position = lua.execute(result + '\nreturn puVar8')
        assert (position['x'], position['y'], position['z']) == expected
