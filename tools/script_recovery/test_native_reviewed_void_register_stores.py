import copy
import hashlib
import json
from pathlib import Path

import pytest

from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_scalar_operands import restore_scalar_operands
from tools.script_recovery.native_reviewed_void_register_stores import restore

ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/trader_escort/units/Q_TraderEscort.json').read_text())
    functions = json.loads((ROOT / 'refs/script_recovery/trader_escort/translation_unit_typed.json').read_text())['functions']
    fn = next(f for f in functions if f['address'] == '0x00E0A510')
    data = RData()
    text = restore_scalar_operands(fn, unit.get('nativeScalarOperands', {}), data)
    return text, fn, unit, data


def test_only_reviewed_store_removed(evidence):
    text, fn, unit, data = evidence
    before = copy.deepcopy((fn, unit))
    result = restore(text, fn, unit, data)
    assert result == text.replace('iVar4 = extraout_EAX_00;', '')
    assert (fn, unit) == before
    assert restore(text, fn, {}, data) == text


@pytest.mark.parametrize('change', ['source', 'native', 'prototype', 'intermediate', 'output', 'effect', 'site', 'producer'])
def test_stale_or_unsafe_evidence_rejected(evidence, change):
    text, fn, unit, data = evidence
    fn, unit = copy.deepcopy((fn, unit))
    row = unit['reviewedVoidRegisterStores']['0x00e0a510']
    if change == 'source':
        fn['decompile'] += '\n'
    elif change == 'native':
        unit['reviewedFunctionPrototypes']['0x00e0a510']['nativeSha256'] = '0' * 64
    elif change == 'prototype':
        unit.pop('reviewedFunctionPrototypes')
    elif change == 'intermediate':
        text += '\n'
    elif change == 'output':
        row['recoveredSha256'] = '0' * 64
    elif change == 'effect':
        row['statement'] = 'iVar4 = side_effect();'
    elif change == 'site':
        row['producerCall'] = '0x00e0a812'
    elif change == 'producer':
        row['producerBytes'] = 'e800000000'
    with pytest.raises(ValueError, match='Reviewed'):
        restore(text, fn, unit, data)


def test_nonterminal_producer_rejected(evidence):
    text, fn, unit, data = evidence
    unit = copy.deepcopy(unit)
    row = unit['reviewedVoidRegisterStores']['0x00e0a510']
    row['producerCall'] = '0x00e0a800'
    row['producerBytes'] = 'ff9014050000'
    with pytest.raises(ValueError, match='return register may be observed'):
        restore(text, fn, unit, data)


@pytest.mark.parametrize('epilogue', [bytes.fromhex('89c1'), bytes.fromhex('58'), bytes.fromhex('eb00')])
def test_epilogue_observation_rejected_even_with_updated_body_pin(evidence, epilogue):
    text, fn, unit, data = evidence
    unit = copy.deepcopy(unit)
    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    raw = bytearray(data.bytes_at(start, end-start))
    offset = 0xE0A812 - start
    raw[offset:offset+len(epilogue)] = epilogue
    unit['reviewedFunctionPrototypes']['0x00e0a510']['nativeSha256'] = hashlib.sha256(raw).hexdigest()

    class MutatedData:
        def bytes_at(self, address, size):
            assert address == start and size == end-start
            return bytes(raw)

    with pytest.raises(ValueError, match='return register may be observed'):
        restore(text, fn, unit, MutatedData())
