import copy
import json
from pathlib import Path

import pytest

from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_reviewed_thing_outputs import restore

ROOT = Path(__file__).resolve().parents[2]
FIXTURES = ROOT / 'tools/script_recovery/fixtures/trader_helper_owned_outputs'

@pytest.fixture(scope='module')
def evidence():
    export = json.loads((ROOT / 'refs/script_recovery/trader_escort/translation_unit_typed.json').read_text())
    fn = next(f for f in export['functions'] if int(f['address'], 16) == 0xe01900)
    unit = json.loads((ROOT / 'refs/script_recovery/trader_escort/units/Q_TraderEscort.json').read_text())
    rows = dict(master=unit['reviewedThingOutputScopes']['0x00e01900'],
                shared=unit['reviewedSharedThingOutputScopes']['0x00e01900'])
    return fn, rows, RData()

@pytest.mark.parametrize('form', ['master', 'shared'])
def test_helper_owned_locals_and_direct_equality(evidence, form):
    fn, rows, data = evidence
    source = (FIXTURES / (form + '_input.lua')).read_text()
    output, todo, calls = restore(source, ['retain diagnostic'], ['prior'], fn, rows[form], data, ROOT)
    assert output == (FIXTURES / (form + '_output.lua')).read_text()
    assert todo == ['retain diagnostic']
    assert calls == ['prior', *rows[form]['calls']]
    assert output.count(':RetailImplementationIsEqualTo(') == 2
    assert 'resources:CopyThingOutput' in output and 'resources:AssignThingOutput' in output

@pytest.mark.parametrize('form', ['master', 'shared'])
@pytest.mark.parametrize('changed', ['export', 'extent', 'body', 'dispatch', 'equality', 'getter', 'sidecar', 'intermediate', 'operation', 'output'])
def test_helper_changed_evidence_stays_rejected(evidence, form, changed):
    original_fn, rows, data = evidence
    fn, row = copy.deepcopy(original_fn), copy.deepcopy(rows[form])
    source = (FIXTURES / (form + '_input.lua')).read_text()
    if changed == 'export': fn['decompile'] += ' changed'
    elif changed == 'extent': fn['bodyEndExclusive'] = hex(int(fn['bodyEndExclusive'], 16) + 1)
    elif changed == 'body': row['nativeSha256'] = '0' * 64
    elif changed == 'dispatch': row['anchors']['0xe01a98'] = '00' * 17
    elif changed == 'equality': row['dependencies']['0x8d46a0']['sha256'] = '0' * 64
    elif changed == 'getter': row['dependencies']['0xa01b50']['sha256'] = '0' * 64
    elif changed == 'sidecar': row['sidecar']['DirectThingImplementationEquality.h']['sha256'] = '0' * 64
    elif changed == 'intermediate': source += '-- changed'
    elif changed == 'operation': row['operations'][-1]['before'] += ' changed'
    elif changed == 'output': row['recoveredSha256'] = '0' * 64
    with pytest.raises(ValueError, match='reviewed Thing outputs'):
        restore(source, [], [], fn, row, data, ROOT)
