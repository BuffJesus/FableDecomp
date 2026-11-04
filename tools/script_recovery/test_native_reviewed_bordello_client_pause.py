"""Retail operands for BordelloClient cleanup unpause paths."""
from copy import deepcopy
import json
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_bordello_client_pause import CALL, DIAGNOSTIC, OLD, restore


ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    return (converter.native('0x00e44ea0'),
            unit['reviewedBordelloClientPause']['0x00e44ea0'], converter.rdata)


def test_all_four_cleanup_paths_unpause(evidence):
    fn, row, rdata = evidence
    source = '\n'.join([OLD['EBX'], OLD['EBP'], OLD['EBP'], OLD['EBX']])
    todo = [DIAGNOSTIC['EBX'], DIAGNOSTIC['EBP'], DIAGNOSTIC['EBP'], DIAGNOSTIC['EBX']]
    assert restore(source, todo.copy(), fn, row, rdata) == ('\n'.join([CALL] * 4), [])
    assert restore(source, todo.copy(), fn, None, rdata) == (source, todo)


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('nativeSha256', '0' * 64),
    lambda row: row.__setitem__('sourceSha256', '0' * 64),
    lambda row: row['nativeInstructions'].__setitem__('0x00E46625', '00'),
    lambda row: row['pauseCalls'][0].__setitem__(1, '0x00E466BE'),
])
def test_stale_pause_evidence_fails(evidence, mutation):
    fn, row, rdata = evidence
    changed = deepcopy(row)
    mutation(changed)
    source = '\n'.join([OLD['EBX'], OLD['EBP'], OLD['EBP'], OLD['EBX']])
    todo = [DIAGNOSTIC['EBX'], DIAGNOSTIC['EBP'], DIAGNOSTIC['EBP'], DIAGNOSTIC['EBX']]
    with pytest.raises(ValueError, match='reviewed BordelloClient pause'):
        restore(source, todo, fn, changed, rdata)


def test_other_missing_slot_is_not_rewritten(evidence):
    fn, row, rdata = evidence
    source = '\n'.join([OLD['EBX'], OLD['EBP'], OLD['EBP'], OLD['EBX'].replace('0x5ec', '0x5e8')])
    todo = [DIAGNOSTIC['EBX'], DIAGNOSTIC['EBP'], DIAGNOSTIC['EBP'], DIAGNOSTIC['EBX']]
    with pytest.raises(ValueError, match='lifted EBX pause calls changed'):
        restore(source, todo, fn, row, rdata)
