"""Retail getter-to-setter evidence for BordelloClient's paid encounter."""
from copy import deepcopy
import json
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_bordello_client_sex_count import OLD, NEW, restore


ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    return (converter.native('0x00e44ea0'),
            unit['reviewedBordelloClientSexCount']['0x00e44ea0'], converter.rdata)


def test_paid_encounter_increments_count(evidence):
    fn, row, rdata = evidence
    assert restore(OLD, [], fn, row, rdata) == (NEW, [])
    assert restore(OLD, [], fn, None, rdata) == (OLD, [])


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('nativeSha256', '0' * 64),
    lambda row: row.__setitem__('sourceSha256', '0' * 64),
    lambda row: row['nativeInstructions'].__setitem__('0x00E45FAA', '00'),
])
def test_stale_evidence_fails(evidence, mutation):
    fn, row, rdata = evidence
    changed = deepcopy(row)
    mutation(changed)
    with pytest.raises(ValueError, match='reviewed BordelloClient sex count'):
        restore(OLD, [], fn, changed, rdata)


def test_other_setter_is_not_rewritten(evidence):
    fn, row, rdata = evidence
    with pytest.raises(ValueError, match='lifted count call changed'):
        restore(OLD.replace('__push4', '__push5'), [], fn, row, rdata)
