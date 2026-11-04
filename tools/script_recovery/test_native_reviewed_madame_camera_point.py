"""Verify Madame's six literal camera point calls from retail operands."""
from copy import deepcopy
import json
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_madame_camera_point import DIAGNOSTICS, NEW, OLD, restore


ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    return converter.native('0x00e3bb70'), unit['reviewedMadameCameraPoint']['0x00e3bb70'], converter.rdata


def test_all_six_calls_use_native_name_and_target(evidence):
    fn, row, rdata = evidence
    source = '\n'.join(OLD)
    assert restore(source, list(DIAGNOSTICS), fn, row, rdata) == ('\n'.join([NEW] * 6), [])
    assert restore(source, list(DIAGNOSTICS), fn, None, rdata) == (source, DIAGNOSTICS)


@pytest.mark.parametrize('field', ['nativeSha256', 'sourceSha256'])
def test_stale_body_or_source_fails(evidence, field):
    fn, row, rdata = evidence
    changed = deepcopy(row)
    changed[field] = '0' * 64
    with pytest.raises(ValueError, match='reviewed Madame camera point'):
        restore('\n'.join(OLD), list(DIAGNOSTICS), fn, changed, rdata)


def test_changed_instruction_or_lifted_call_fails(evidence):
    fn, row, rdata = evidence
    changed = deepcopy(row)
    changed['sites'][2]['callBytes'] = '00'
    with pytest.raises(ValueError, match='camera point call changed'):
        restore('\n'.join(OLD), list(DIAGNOSTICS), fn, changed, rdata)
    with pytest.raises(ValueError, match='lifted camera point calls changed'):
        restore('\n'.join(OLD).replace('pCVar8', 'other'), list(DIAGNOSTICS), fn, row, rdata)
