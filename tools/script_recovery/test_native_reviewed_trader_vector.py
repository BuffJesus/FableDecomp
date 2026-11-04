import copy
import hashlib
import json
from pathlib import Path

import pytest

from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_reviewed_trader_vector import DIAGNOSTIC, restore

ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/trader_escort/units/Q_TraderEscort.json').read_text(encoding='utf-8-sig'))
    exported = json.loads((ROOT / 'refs/script_recovery/trader_escort/translation_unit_typed.json').read_text(encoding='utf-8-sig'))
    fn = next(f for f in exported['functions'] if int(f['address'], 16) == 0xe06050)
    source = (ROOT / 'tools/script_recovery/fixtures/trader_vector/lowering_input.lua').read_text(encoding='utf-8')
    return fn, unit['reviewedTraderVectors']['0x00e06050'], source, RData()


def test_reviewed_list_retains_refresh_and_scope_cleanup(evidence):
    fn, row, source, data = evidence
    result, todo, calls = restore(source, ['other', DIAGNOSTIC], ['Pause'], fn, row, data)
    assert hashlib.sha256(result.encode()).hexdigest() == row['recoveredSha256']
    assert todo == ['other'] and calls[0] == 'Pause'
    assert result.index('surprisePositions:Clear()') < result.index('surprisePositions:FillScriptName')
    assert 'xStack_30' not in result
    assert result.count('surprisePositions:GetPos(') == 2
    assert result.count('surprisePositions:Count()') == 2
    assert 'quest:WithRetailResources(function(resources)' in result


def test_unreviewed_function_is_untouched(evidence):
    fn, _, source, _ = evidence
    class NoReads:
        def bytes_at(self, *args):
            raise AssertionError('unreviewed native read')
    assert restore(source, ['todo'], ['call'], fn, None, NoReads()) == (source, ['todo'], ['call'])


@pytest.mark.parametrize('change', ['extent', 'export', 'native', 'intermediate', 'output', 'clear', 'destroy',
                                   'dependency', 'sidecar', 'diagnostic', 'fill_shape', 'position_shape'])
def test_changed_evidence_fails_closed(evidence, change):
    fn, row, source, data = evidence
    fn, row = copy.deepcopy(fn), copy.deepcopy(row)
    todo = [DIAGNOSTIC]
    if change == 'extent':
        fn['bodyEndExclusive'] = '0xe06440'
    elif change == 'export':
        fn['decompile'] += '\n'
    elif change == 'native':
        row['nativeSha256'] = '0' * 64
    elif change == 'intermediate':
        source += '\n'
    elif change == 'output':
        row['recoveredSha256'] = '0' * 64
    elif change in ('clear', 'destroy'):
        site = '0x00e060ac' if change == 'clear' else '0x00e063fe'
        next(c for c in fn['calls'] if c['site'].lower() == site)['target'] = '0x00400000'
    elif change == 'dependency':
        next(iter(row['dependencies'].values()))['sha256'] = '0' * 64
    elif change == 'sidecar':
        next(iter(row['sidecar'].values()))['sha256'] = '0' * 64
    elif change == 'diagnostic':
        todo = []
    else:
        source = source.replace('M_BalverineSurprise', 'Other') if change == 'fill_shape' else source.replace('uVar10 + 1', 'uVar10 + 2')
        row['intermediateSha256'] = hashlib.sha256(source.encode()).hexdigest()
    with pytest.raises(ValueError, match='reviewed trader vector'):
        restore(source, todo, [], fn, row, data)
