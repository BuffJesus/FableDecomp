"""Verify Madame's native self receiver in SetIsPushableByHero."""
from copy import deepcopy
import json
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_madame_pushable import OLD, NEW, restore


ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    return converter.native('0x00e3bb70'), unit['reviewedMadamePushable']['0x00e3bb70'], converter.rdata


def test_self_receiver_restored(evidence):
    fn, row, rdata = evidence
    assert restore(OLD, [row['diagnostic']], fn, row, rdata) == (NEW, [])
    assert restore(OLD, [row['diagnostic']], fn, None, rdata) == (OLD, [row['diagnostic']])


@pytest.mark.parametrize('field', ['nativeSha256', 'sourceSha256', 'diagnostic'])
def test_stale_evidence_fails(evidence, field):
    fn, row, rdata = evidence
    changed = deepcopy(row)
    changed[field] = 'wrong'
    with pytest.raises(ValueError, match='reviewed Madame pushability'):
        restore(OLD, [row['diagnostic']], fn, changed, rdata)


def test_wrong_lifted_source_fails(evidence):
    fn, row, rdata = evidence
    with pytest.raises(ValueError, match='lifted pushability call changed'):
        restore(OLD.replace('nil --[[missing]]', 'other'), [row['diagnostic']], fn, row, rdata)
