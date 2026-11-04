"""Retail byte stores and Lua branch behavior for BordelloClient."""
from copy import deepcopy
import json
from pathlib import Path

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_bordello_client_flags import (
    NEW_TEST, OLD_TEST, REWRITES, restore,
)


ROOT = Path(__file__).resolve().parents[2]
SOURCE = ('xStack_e0_b3 = bVar4\n'
          'if not xStack_e0_b3 then end\n'
          + '\n'.join(old for old, _, _ in REWRITES)
          + '\n' + OLD_TEST + '\n  return "hit"\nend\nreturn "clear"')


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    return (converter.native('0x00e44ea0'),
            unit['reviewedBordelloClientFlags']['0x00e44ea0'], converter.rdata)


def test_native_zero_and_one_become_lua_booleans(evidence):
    fn, row, rdata = evidence
    todo = [diagnostic for _, _, diagnostic in REWRITES]
    result, remaining = restore(SOURCE, todo.copy(), fn, row, rdata)
    assert remaining == [] and NEW_TEST in result
    assert all(new in result for _, new, _ in REWRITES)
    assert restore(SOURCE, todo.copy(), fn, None, rdata) == (SOURCE, todo)
    lua = LuaRuntime()
    # Run the two native stores separately: the zero path must skip the hit branch.
    for stored, expected in ((REWRITES[1][1], 'clear'), (REWRITES[2][1], 'hit')):
        body = f'xStack_c4_b3 = nil\n{stored}\n{NEW_TEST}\n return "hit"\nend\nreturn "clear"'
        assert lua.execute(body) == expected


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('nativeSha256', '0' * 64),
    lambda row: row.__setitem__('sourceSha256', '0' * 64),
    lambda row: row['nativeInstructions'].__setitem__('0x00E453CB', '00'),
    lambda row: row['stores']['0x00E453CB'].__setitem__(1, 1),
])
def test_stale_flag_evidence_fails(evidence, mutation):
    fn, row, rdata = evidence
    changed = deepcopy(row)
    mutation(changed)
    with pytest.raises(ValueError, match='reviewed BordelloClient flags'):
        restore(SOURCE, [d for _, _, d in REWRITES], fn, changed, rdata)


def test_other_byte_field_is_not_rewritten(evidence):
    fn, row, rdata = evidence
    with pytest.raises(ValueError, match='lifted byte store changed'):
        restore(SOURCE.replace('xStack_c4_b3 =', 'xStack_c8_b3 =', 1),
                [d for _, _, d in REWRITES], fn, row, rdata)
