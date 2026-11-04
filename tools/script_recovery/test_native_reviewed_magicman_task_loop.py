"""Native evidence for Magicman's first beer movie task loop."""
from copy import deepcopy
import json
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_magicman_task_loop import OLD_LOOP, OLD_TAIL, restore

ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    fn = converter.native('0x00e40e80')
    row = unit['reviewedMagicmanTaskLoop']['0x00e40e80']
    return fn, row, OLD_LOOP + OLD_TAIL, converter.rdata


def test_reviewed_task_loop(evidence):
    fn, row, source, rdata = evidence
    result, todo = restore(source, [row['diagnostic']], fn, row, rdata)
    assert 'while me:IsPerformingScriptTask() do' in result
    assert 'resources:DestroyMovie(xStack_330)' in result
    assert 'LAB_00e428c7' not in result
    assert todo == []
    assert restore(source, [row['diagnostic']], fn, None, rdata) == (source, [row['diagnostic']])


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('nativeSha256', '0' * 64),
    lambda row: row.__setitem__('sourceSha256', '0' * 64),
    lambda row: row['nativeInstructions'].__setitem__('0x00E428D2', '00'),
    lambda row: row.__setitem__('memberRelease', '0x00E4484D'),
    lambda row: row.__setitem__('diagnostic', 'wrong'),
])
def test_stale_task_loop_fails(evidence, mutation):
    fn, row, source, rdata = evidence
    changed = deepcopy(row)
    mutation(changed)
    with pytest.raises(ValueError, match='reviewed Magicman task loop'):
        restore(source, [row['diagnostic']], fn, changed, rdata)
