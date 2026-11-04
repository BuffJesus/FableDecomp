"""Retail byte stores and Lua response branches for Madame."""
from copy import deepcopy
import json
from pathlib import Path

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_madame_hit_flags import restore


ROOT = Path(__file__).resolve().parents[2]
SOURCE = r"""-- TODO(native): xStack_124_b3 = '\0';
::LAB_00e3cab0::
-- TODO(native): xStack_124_b3 = '\0';
::LAB_00e3cb3c::
-- TODO(native): xStack_124_b3 = '\x01';
::FLOW_past_lab_00e3cb3c::
if xStack_124_b3 ~= 0 then end
::LAB_00e3dfb3::
-- TODO(native): xStack_f8_b3 = '\0';
::LAB_00e3e06a::
-- TODO(native): xStack_f8_b3 = '\x01';
::FLOW_past_lab_00e3e06a::
if xStack_f8_b3 ~= 0 then end
"""
TODO = ["unlifted: xStack_124_b3 = '\\0';",
        "unlifted: xStack_124_b3 = '\\0';",
        "unlifted: xStack_124_b3 = '\\x01';",
        "unlifted: xStack_f8_b3 = '\\0';",
        "unlifted: xStack_f8_b3 = '\\x01';"]


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    return (converter.native('0x00e3bb70'),
            unit['reviewedMadameHitFlags']['0x00e3bb70'], converter.rdata)


def test_two_hit_responses_use_native_booleans(evidence):
    fn, row, rdata = evidence
    result, todo = restore(SOURCE, TODO.copy(), fn, row, rdata)
    assert result.count('xStack_124_b3 = false') == 1
    assert result.count('xStack_124_b3 = true') == 1
    assert result.count('xStack_f8_b3 = false') == 1
    assert result.count('xStack_f8_b3 = true') == 1
    assert 'if xStack_124_b3 then' in result
    assert 'if xStack_f8_b3 then' in result
    assert result.count("-- TODO(native): xStack_124_b3 = '\\0';") == 1
    assert todo == TODO[:1]
    assert restore(SOURCE, TODO.copy(), fn, None, rdata) == (SOURCE, TODO)


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('nativeSha256', '0' * 64),
    lambda row: row.__setitem__('sourceSha256', '0' * 64),
    lambda row: row['nativeInstructions'].__setitem__('0x00E3CB35', '00'),
    lambda row: row['stores']['0x00E3E06A'].__setitem__(1, 0),
])
def test_stale_hit_evidence_fails(evidence, mutation):
    fn, row, rdata = evidence
    changed = deepcopy(row)
    mutation(changed)
    with pytest.raises(ValueError, match='reviewed Madame hit flags'):
        restore(SOURCE, TODO.copy(), fn, changed, rdata)


def test_other_hit_field_is_not_rewritten(evidence):
    fn, row, rdata = evidence
    with pytest.raises(ValueError, match='lifted branch changed'):
        restore(SOURCE.replace('xStack_f8_b3 ~= 0', 'xStack_fc_b3 ~= 0'),
                TODO.copy(), fn, row, rdata)


@pytest.mark.parametrize('field', ['xStack_124_b3', 'xStack_f8_b3'])
def test_hit_branch_skips_clear_event(field):
    lua = LuaRuntime()
    for value, expected in ((False, 'clear'), (True, 'hit')):
        script = f'{field} = {str(value).lower()}\nif {field} then return "hit" end\nreturn "clear"'
        assert lua.execute(script) == expected
