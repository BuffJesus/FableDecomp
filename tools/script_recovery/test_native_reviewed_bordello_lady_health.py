"""Retail call site evidence for BordelloLady's health receivers."""
from copy import deepcopy
import json
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_bordello_lady_health import NEW, OLD, restore


ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    fn = converter.native('0x00e3eb10')
    row = unit['reviewedBordelloLadyHealth']['0x00e3eb10']
    return fn, row, converter.rdata


def test_health_receiver_is_entity(evidence):
    fn, row, rdata = evidence
    old = '\n'.join([OLD] * 11)
    todo = [row['diagnostic']] * 11
    assert restore(old, todo, fn, row, rdata) == ('\n'.join([NEW] * 11), [])
    assert restore(old, todo, fn, None, rdata) == (old, todo)


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('nativeSha256', '0' * 64),
    lambda row: row.__setitem__('sourceSha256', '0' * 64),
    lambda row: row['nativeCalls'].__setitem__(0, '0x00E3EE1A'),
    lambda row: row.__setitem__('diagnostic', 'wrong'),
])
def test_stale_evidence_fails(evidence, mutation):
    fn, row, rdata = evidence
    changed = deepcopy(row)
    mutation(changed)
    with pytest.raises(ValueError, match='reviewed BordelloLady health'):
        restore('\n'.join([OLD] * 11), [row['diagnostic']] * 11,
                fn, changed, rdata)


def test_other_calls_remain_unresolved(evidence):
    fn, row, rdata = evidence
    with pytest.raises(ValueError, match='lifted health calls changed'):
        restore('\n'.join([OLD] * 10 + ['quest:GetHealth(hero)']),
                [row['diagnostic']] * 11, fn, row, rdata)


def test_binding_requires_script_thing():
    manifest = json.loads((ROOT / 'refs/fse_api_manifest.json').read_text())
    methods = [f for f in manifest['functions'] if f.get('name') == 'GetHealth']
    assert len(methods) == 1
    assert [p['type'] for p in methods[0]['parameters']] == ['CScriptThing*']
