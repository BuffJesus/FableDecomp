"""Retail evidence for BordelloLady's post-control movie pause operand."""
from copy import deepcopy
import json
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_bordello_lady_pause_bool import NEW, OLD, restore


ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    fn = converter.native('0x00e3eb10')
    row = unit['reviewedBordelloLadyPauseBool']['0x00e3eb10']
    return fn, row, converter.rdata


def test_post_control_pause_uses_zeroed_register(evidence):
    fn, row, rdata = evidence
    source, todo = restore(OLD, [row['diagnostic']], fn, row, rdata)
    assert source == NEW and todo == []
    assert restore(OLD, [row['diagnostic']], fn, None, rdata) == (OLD, [row['diagnostic']])


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('nativeSha256', '0' * 64),
    lambda row: row.__setitem__('sourceSha256', '0' * 64),
    lambda row: row['nativeInstructions'].__setitem__('0x00E3EDD5', '00'),
    lambda row: row.__setitem__('nativeCall', '0x00E4035E'),
    lambda row: row.__setitem__('diagnostic', 'wrong'),
])
def test_stale_pause_evidence_fails(evidence, mutation):
    fn, row, rdata = evidence
    changed = deepcopy(row)
    mutation(changed)
    with pytest.raises(ValueError, match='reviewed BordelloLady pause boolean'):
        restore(OLD, [row['diagnostic']], fn, changed, rdata)


def test_other_pause_call_is_not_rewritten(evidence):
    fn, row, rdata = evidence
    with pytest.raises(ValueError, match='lifted pause call changed'):
        restore(NEW, [row['diagnostic']], fn, row, rdata)


def test_binding_accepts_boolean():
    manifest = json.loads((ROOT / 'refs/fse_api_manifest.json').read_text())
    methods = [f for f in manifest['functions'] if f.get('name') == 'PauseAllNonScriptedEntities']
    assert len(methods) == 1
    assert len(methods[0]['parameters']) == 1
    assert methods[0]['parameters'][0]['type'] == 'bool'
