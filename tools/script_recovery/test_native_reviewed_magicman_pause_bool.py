"""Retail evidence for Magicman's pause argument on movie exits."""
from copy import deepcopy
import json
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_magicman_pause_bool import RIGHT, WRONG, restore


ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    fn = converter.native('0x00e40e80')
    row = unit['reviewedMagicmanPauseBool']['0x00e40e80']
    source = (WRONG + '\n') * 10
    return fn, row, source, converter.rdata


def test_pause_argument_from_retail_zeroed_register(evidence):
    fn, row, source, rdata = evidence
    assert source.count(WRONG) == 10
    result = restore(source, fn, row, rdata)
    assert result.count(WRONG) == 0
    assert result.count(RIGHT) == source.count(RIGHT) + 10
    assert restore(source, fn, None, rdata) == source


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('nativeSha256', '0' * 64),
    lambda row: row.__setitem__('sourceSha256', '0' * 64),
    lambda row: row['nativeInstructions'].__setitem__('0x00E4282D', '00'),
    lambda row: row.__setitem__('liftedCallCount', 9),
])
def test_stale_pause_evidence_fails(evidence, mutation):
    fn, row, source, rdata = evidence
    changed = deepcopy(row)
    mutation(changed)
    with pytest.raises(ValueError, match='reviewed Magicman pause argument'):
        restore(source, fn, changed, rdata)


def test_other_lifted_call_is_untouched(evidence):
    fn, row, source, rdata = evidence
    with pytest.raises(ValueError, match='lifted pause calls changed'):
        restore(source.replace(WRONG, 'quest:PauseAllNonScriptedEntities(true)', 1), fn, row, rdata)
