"""Retail field and literal evidence for BordelloLady's Name reload."""
from copy import deepcopy
import json
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_bordello_lady_name_load import NEW, OLD, restore


ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    row = unit['reviewedBordelloLadyNameLoad']['0x00e3eb10']
    fields = unit['entities']['BordelloLady']['fields']
    return converter.native('0x00e3eb10'), row, converter.rdata, fields, converter.native


def test_name_reload_uses_persisted_entity_field(evidence):
    fn, row, rdata, fields, native = evidence
    assert restore(OLD, [row['diagnostic']], fn, row, rdata, fields, native) == (NEW, [])
    assert restore(OLD, [row['diagnostic']], fn, None, rdata, fields, native) == (OLD, [row['diagnostic']])


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('nativeSha256', '0' * 64),
    lambda row: row.__setitem__('sourceSha256', '0' * 64),
    lambda row: row.__setitem__('initNativeSha256', '0' * 64),
    lambda row: row.__setitem__('diagnostic', 'wrong'),
])
def test_stale_evidence_fails(evidence, mutation):
    fn, row, rdata, fields, native = evidence
    changed = deepcopy(row)
    mutation(changed)
    with pytest.raises(ValueError, match='reviewed BordelloLady Name load'):
        restore(OLD, [row['diagnostic']], fn, changed, rdata, fields, native)


def test_wrong_field_or_source_remains_unresolved(evidence):
    fn, row, rdata, fields, native = evidence
    with pytest.raises(ValueError, match='Name field mapping changed'):
        restore(OLD, [row['diagnostic']], fn, row, rdata, {'0x28': ['Other', 'String']}, native)
    with pytest.raises(ValueError, match='lifted Name load changed'):
        restore(OLD.replace('"Name"', '"Other"'), [row['diagnostic']], fn, row, rdata, fields, native)
