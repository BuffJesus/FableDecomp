"""Reviewed operand maps must fail closed on changed or ambiguous evidence."""
import copy
import json
from pathlib import Path
import pytest
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_scalar_operands import restore_scalar_operands

ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture
def inputs():
    base = ROOT / 'refs/script_recovery/book_collecting'
    fn = next(f for f in json.loads((base/'translation_unit_typed.json').read_text())['functions']
              if int(f['address'], 16) == 0xe57530)
    evidence = json.loads((base/'units/V_BookCollecting.json').read_text())['nativeScalarOperands']
    return fn, evidence, RData()


def test_only_reviewed_scalar_tokens_change(inputs):
    fn, evidence, image = inputs
    output = restore_scalar_operands(fn, evidence, image)
    assert output.count('native_scalar_18') == 8
    assert 'local_18 = *pCVar12;' in output  # unrelated vtable temporary
    assert '(CCharString *)&local_18' in output  # request string lifetime
    assert 'CStack_1c._3_1_' in output  # independent mutable byte
    assert 'value = native_scalar_18;' in output
    assert restore_scalar_operands(fn, {}, image) == fn['decompile']


@pytest.mark.parametrize('fault', ['source', 'native', 'offset', 'bytes', 'token', 'site', 'overlap'])
def test_stale_or_invalid_evidence_stops_conversion(inputs, fault):
    fn, evidence, image = copy.deepcopy(inputs[:2]) + (inputs[2],)
    row = evidence['0x00e57530']; binding = row['bindings'][0]
    if fault == 'source': fn['decompile'] += '\n'
    if fault == 'native': row['nativeSha256'] = '0' * 64
    if fault == 'offset': binding['entryOffset'] -= 4
    if fault == 'bytes': binding['accesses'][0]['bytes'] = '90'
    if fault == 'token': binding['tokens'][0]['start'] += 1
    if fault == 'site': binding['tokens'][0]['site'] = '0x00e57530'
    if fault == 'overlap': binding['tokens'].append(binding['tokens'][0])
    with pytest.raises(ValueError, match='scalar operand evidence'):
        restore_scalar_operands(fn, evidence, image)
