"""Retail and Forge evidence for BordelloLady's personality override."""
from copy import deepcopy
import json
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_bordello_lady_personality import COMMENT, NEW, OLD, restore


ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    fn = converter.native('0x00e3eb10')
    row = unit['reviewedBordelloLadyPersonality']['0x00e3eb10']
    source = ('    ' + COMMENT + '\n    ' + OLD + '\n') * 2
    todo = [row['diagnostic']] * 2
    calls = ['EntitySetPersonalityOverride'] * 2
    return fn, row, source, todo, calls, converter.rdata


def test_restores_string_personality_binding(evidence):
    fn, row, source, todo, calls, rdata = evidence
    result, diagnostics, methods = restore(source, todo, calls, fn, row, rdata)
    assert result == ('    ' + NEW + '\n') * 2
    assert diagnostics == []
    assert methods == ['EntitySetPersonalityOverrideByString'] * 2
    assert restore(source, todo, calls, fn, None, rdata) == (source, todo, calls)


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('nativeSha256', '0' * 64),
    lambda row: row.__setitem__('sourceSha256', '0' * 64),
    lambda row: row['nativeInstructions'].__setitem__('0x00E400D8', '00'),
    lambda row: row.__setitem__('nativeCall', '0x00E400DA'),
    lambda row: row.__setitem__('literalAddress', '0x012E422D'),
    lambda row: row.__setitem__('bindingSha256', '0' * 64),
    lambda row: row.__setitem__('liftedCallCount', 1),
])
def test_stale_personality_evidence_fails(evidence, mutation):
    fn, row, source, todo, calls, rdata = evidence
    changed = deepcopy(row)
    mutation(changed)
    with pytest.raises(ValueError, match='reviewed BordelloLady personality'):
        restore(source, list(todo), list(calls), fn, changed, rdata)


def test_binding_signature_is_string_and_thing():
    manifest = json.loads((ROOT / 'refs/fse_api_manifest.json').read_text())
    methods = [f for f in manifest['functions'] if f.get('name') == 'EntitySetPersonalityOverrideByString']
    assert len(methods) == 1
    assert [p['name'] for p in methods[0]['parameters']] == ['spThing', 'personalityName']
    assert 'CScriptThing' in methods[0]['parameters'][0]['type']
    assert 'string' in methods[0]['parameters'][1]['type']
