"""Evidence for BordelloLady's native opening setup and Lua call operands."""
from copy import deepcopy
import json
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_bordello_lady_setup import REPLACEMENTS, restore


ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    fn = converter.native('0x00e3eb10')
    row = unit['reviewedBordelloLadySetup']['0x00e3eb10']
    lines = [REPLACEMENTS[0][0], '        r1 = me:GetHomePos()']
    lines += [old for old, _ in REPLACEMENTS[1:]]
    source = '\n'.join(lines) + '\n'
    return fn, row, source, converter.rdata


def test_restores_self_and_home_position(evidence):
    fn, row, source, rdata = evidence
    result, todo = restore(source, list(row['diagnostics']), fn, row, rdata)
    for old, new in REPLACEMENTS:
        assert old not in result
        if new:
            assert new in result
    assert 'r1 = me:GetHomePos()' in result
    assert todo == []
    assert restore(source, list(row['diagnostics']), fn, None, rdata) == (source, row['diagnostics'])


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('nativeSha256', '0' * 64),
    lambda row: row.__setitem__('sourceSha256', '0' * 64),
    lambda row: row['nativeInstructions'].__setitem__('0x00E3EC41', '00'),
    lambda row: row['diagnostics'].append('wrong diagnostic'),
])
def test_stale_evidence_fails(evidence, mutation):
    fn, row, source, rdata = evidence
    changed = deepcopy(row)
    mutation(changed)
    with pytest.raises(ValueError, match='reviewed BordelloLady setup'):
        restore(source, list(row['diagnostics']), fn, changed, rdata)


def test_changed_lifted_operand_fails(evidence):
    fn, row, source, rdata = evidence
    changed = source.replace('r1 = nil', 'r1 = false')
    with pytest.raises(ValueError, match='reviewed BordelloLady setup'):
        restore(changed, list(row['diagnostics']), fn, row, rdata)


def test_forge_signatures_accept_recovered_operands():
    manifest = json.loads((ROOT / 'refs/fse_api_manifest.json').read_text())
    names = ('SetCreatureBrain', 'SetWanderCentrePoint', 'SetWanderMinDistance',
             'SetWanderMaxDistance', 'SetScriptingStateGroup')
    methods = {f['name']: f for f in manifest['functions'] if f.get('name') in names}
    assert set(methods) == set(names)
    for name in names:
        params = methods[name]['parameters']
        assert len(params) == 2
        assert 'CScriptThing' in params[0]['type']
    assert methods['SetWanderCentrePoint']['parameters'][1]['type'] == 'sol::table'
