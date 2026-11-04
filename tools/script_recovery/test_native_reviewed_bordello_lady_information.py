"""Native evidence for BordelloLady clearing her information flag."""
from copy import deepcopy
import json
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_bordello_lady_information import NEW, OLD, restore


ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    fn = converter.native('0x00e3eb10')
    row = unit['reviewedBordelloLadyInformation']['0x00e3eb10']
    return fn, row, converter.rdata


def test_restores_both_lifted_copies_of_one_native_call(evidence):
    fn, row, rdata = evidence
    source = (OLD + '\n') * 2
    todo = [row['diagnostic']] * 2
    result, diagnostics = restore(source, todo, fn, row, rdata)
    assert result == (NEW + '\n') * 2
    assert diagnostics == []
    assert restore(source, todo, fn, None, rdata) == (source, todo)


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('nativeSha256', '0' * 64),
    lambda row: row.__setitem__('sourceSha256', '0' * 64),
    lambda row: row['nativeInstructions'].__setitem__('0x00E400B1', '00'),
    lambda row: row.__setitem__('nativeCall', '0x00E400B3'),
    lambda row: row.__setitem__('liftedCallCount', 1),
])
def test_stale_information_evidence_fails(evidence, mutation):
    fn, row, rdata = evidence
    changed = deepcopy(row)
    mutation(changed)
    with pytest.raises(ValueError, match='reviewed BordelloLady information operand'):
        restore((OLD + '\n') * 2, [row['diagnostic']] * 2, fn, changed, rdata)


def test_binding_accepts_entity_thing():
    manifest = json.loads((ROOT / 'refs/fse_api_manifest.json').read_text())
    methods = [f for f in manifest['functions'] if f.get('name') == 'ClearThingHasInformation']
    assert len(methods) == 1
    assert len(methods[0]['parameters']) == 1
    assert 'CScriptThing' in methods[0]['parameters'][0]['type']
