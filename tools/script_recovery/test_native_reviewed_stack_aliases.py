"""Reviewed native counter evidence separates a scalar from its resource."""
from copy import deepcopy
import hashlib
import json
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_stack_aliases import restore

ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    fn = converter.native('0x00e40e80')
    row = unit['reviewedStackScalarAliases']['0x00e40e80']
    source = '\n'.join(old for old, _ in row['edits']) + '\n'
    return fn, row, source, converter.rdata


def test_native_reviewed_chatter_counter(evidence):
    fn, row, source, rdata = evidence
    result = restore(source, fn, row, rdata)
    assert all(new in result for _, new in row['edits'])
    assert 'iStack_358' not in result
    assert restore(source, fn, None, rdata) == source


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('nativeSha256', '0' * 64),
    lambda row: row.__setitem__('sourceSha256', '0' * 64),
    lambda row: row['nativeInstructions'].__setitem__('0x00E40FAF', '00'),
    lambda row: row['edits'][0].__setitem__(0, 'missing = 1;'),
    lambda row: row.__setitem__('alias', 'uStack_318'),
])
def test_stale_or_ambiguous_counter_evidence_fails(evidence, mutation):
    fn, row, source, rdata = evidence
    changed = deepcopy(row)
    mutation(changed)
    with pytest.raises(ValueError, match='reviewed stack alias'):
        restore(source, fn, changed, rdata)


@pytest.fixture(scope='module')
def spectator_evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/chicken_kicking/units/V_ChickenKicking.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/chicken_kicking/translation_unit_typed.json')
    fn = converter.native('0x00e63890')
    row = unit['reviewedStackScalarAliases']['0x00e63890']
    counts = row.get('editCounts', [1] * len(row['edits']))
    source = '\n'.join(old for (old, _), count in zip(row['edits'], counts) for _ in range(count))
    return fn, row, source, converter.rdata


def test_spectator_local_greeting_flag(spectator_evidence):
    fn, row, source, rdata = spectator_evidence
    result = restore(source, fn, row, rdata)
    assert result.splitlines()[:3] == ['native_greeting_done = false;',
                                   '!native_greeting_done', 'native_greeting_done = true;']
    assert restore(source, fn, None, rdata) == source


def test_spectator_changed_initial_byte_is_rejected(spectator_evidence):
    fn, row, source, rdata = spectator_evidence
    changed = deepcopy(row)
    changed['nativeInstructions']['0xe638d5'] = 'c644242201'
    with pytest.raises(ValueError, match='native instruction changed'):
        restore(source, fn, changed, rdata)


def test_spectator_ambiguous_flag_read_is_rejected(spectator_evidence):
    fn, row, source, rdata = spectator_evidence
    source += '\n' + row['edits'][1][0]
    with pytest.raises(ValueError, match='expected 1 occurrences'):
        restore(source, fn, row, rdata)


def pinned_phase(evidence):
    fn, original, source, rdata = evidence
    row = deepcopy(original)
    row['textSha256'] = hashlib.sha256(source.encode()).hexdigest()
    row['nativeDependencies'] = [{'address': '0x0099eae0',
                                  'bytes': rdata.bytes_at(0x99eae0, 5).hex()}]
    return fn, row, source, rdata


def test_opt_in_phase_and_callee_pins_preserve_output(evidence):
    fn, row, source, rdata = pinned_phase(evidence)
    assert restore(source, fn, row, rdata) == restore(source, fn, evidence[1], rdata)


def test_changed_phase_outside_exact_edits_is_rejected(evidence):
    fn, row, source, rdata = pinned_phase(evidence)
    with pytest.raises(ValueError, match='source phase changed'):
        restore(source + '\nother = 1;', fn, row, rdata)


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('textSha256', None),
    lambda row: row.__setitem__('nativeDependencies', []),
    lambda row: row.__setitem__('nativeDependencies', {}),
    lambda row: row.__setitem__('nativeDependencies', [None]),
    lambda row: row['nativeDependencies'][0].__setitem__('address', 'invalid'),
    lambda row: row['nativeDependencies'][0].__setitem__('bytes', 'invalid'),
    lambda row: row['nativeDependencies'][0].__setitem__('bytes', ''),
    lambda row: row['nativeDependencies'][0].__setitem__('bytes', '00'),
    lambda row: row['nativeDependencies'][0].pop('address'),
    lambda row: row['nativeDependencies'].append(deepcopy(row['nativeDependencies'][0])),
])
def test_stale_phase_or_dependency_is_rejected(evidence, mutation):
    fn, row, source, rdata = pinned_phase(evidence)
    mutation(row)
    with pytest.raises(ValueError, match='reviewed stack alias'):
        restore(source, fn, row, rdata)
