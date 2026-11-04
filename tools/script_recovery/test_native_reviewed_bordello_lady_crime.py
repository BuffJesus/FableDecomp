"""Native and field evidence for BordelloLady's village crime query."""
from copy import deepcopy
import json
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_bordello_lady_crime import NEW, OLD, restore


ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    fn = converter.native('0x00e3eb10')
    row = unit['reviewedBordelloLadyCrime']['0x00e3eb10']
    return fn, row, unit['quest']['thingFields'], converter.rdata


def test_restores_quest_village(evidence):
    fn, row, fields, rdata = evidence
    source, todo = restore(OLD, [row['diagnostic']], fn, row, rdata, fields)
    assert source == NEW and todo == []
    assert restore(OLD, [row['diagnostic']], fn, None, rdata, fields) == (OLD, [row['diagnostic']])


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('nativeSha256', '0' * 64),
    lambda row: row.__setitem__('sourceSha256', '0' * 64),
    lambda row: row['nativeInstructions'].__setitem__('0x00E3ECBD', '00'),
    lambda row: row.__setitem__('diagnostic', 'wrong'),
])
def test_stale_crime_evidence_fails(evidence, mutation):
    fn, row, fields, rdata = evidence
    changed = deepcopy(row)
    mutation(changed)
    with pytest.raises(ValueError, match='reviewed BordelloLady crime operand'):
        restore(OLD, [row['diagnostic']], fn, changed, rdata, fields)


def test_wrong_field_and_lifted_call_fail(evidence):
    fn, row, fields, rdata = evidence
    with pytest.raises(ValueError, match='quest thing field changed'):
        restore(OLD, [row['diagnostic']], fn, row, rdata, {'0x5c': 'WrongThing'})
    with pytest.raises(ValueError, match='lifted crime call changed'):
        restore(OLD.replace('nil', 'me'), [row['diagnostic']], fn, row, rdata, fields)


def test_forge_crime_signature_accepts_thing():
    manifest = json.loads((ROOT / 'refs/fse_api_manifest.json').read_text())
    functions = [f for f in manifest['functions'] if f.get('name') == 'IsReportedOrUnreportedCrimeKnown']
    assert len(functions) == 1
    assert len(functions[0]['parameters']) == 1
    assert functions[0]['parameters'][0]['name'] == 'spVillage'
    assert 'CScriptThing' in functions[0]['parameters'][0]['type']
