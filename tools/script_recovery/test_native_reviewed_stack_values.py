"""Native reviewed dialogue value aliases are exact and fail closed."""
from copy import deepcopy
import json
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter, _text_order_sites
from tools.script_recovery.native_reviewed_stack_values import restore

ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    fn = converter.native('0x00e40e80')
    row = unit['reviewedStackValueAliases']['0x00e40e80']
    source = '\n'.join([old for old, _, count in row['edits'] for _ in range(count)]) + '\n'
    sites = _text_order_sites(fn['decompile'], fn)
    return fn, row, source, converter.rdata, sites


def test_native_reviewed_dialogue_values(evidence):
    fn, row, source, rdata, sites = evidence
    result = restore(source, fn, row, rdata, sites)
    assert result.count('ppuVar4 = iStack_350;') == 2
    assert result.count('iStack_350') == 4
    assert restore(source, fn, None, rdata, sites) == source


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('nativeSha256', '0' * 64),
    lambda row: row.__setitem__('sourceSha256', '0' * 64),
    lambda row: row['nativeInstructions'].__setitem__('0x00E414A6', '00'),
    lambda row: row['conversionSites'].__setitem__('0x00E414B7', 'iStack_350'),
    lambda row: row['edits'][0].__setitem__(2, 3),
])
def test_stale_dialogue_value_evidence_fails(evidence, mutation):
    fn, row, source, rdata, sites = evidence
    changed = deepcopy(row)
    mutation(changed)
    with pytest.raises(ValueError, match='reviewed stack value'):
        restore(source, fn, changed, rdata, sites)
