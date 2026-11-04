import copy
import hashlib
import json
from pathlib import Path
import sys
import pytest

W = Path(__file__).resolve().parent
R = W.parents[1]
sys.path.insert(0, str(R))
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_cleanup_operands import restore

SOURCE = (W / 'testdata/magicman_abort_cleanup.lua').read_text()
assert hashlib.sha256(SOURCE.encode()).hexdigest() == '86db0c247958215b521f45fdba2ba188d88e221924438c7ff869333c387af61b'
ROW = json.loads((R / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())['reviewedCleanupOperands']['0x00e40e80']
assert len(ROW['replacements']) >= 5
ROW = copy.deepcopy(ROW)
ROW['replacements'] = ROW['replacements'][:5]
PRIOR = copy.deepcopy(ROW)
PRIOR['replacements'] = PRIOR['replacements'][:4]

@pytest.fixture(scope='module')
def evidence():
    converter = UnitConverter(R / 'refs/script_recovery/bordello/translation_unit_typed.json')
    return converter.native('0x00e40e80'), converter.rdata

def test_exact_chatter_cleanup_preserves_four_prior_replacements(evidence):
    fn, rd = evidence
    assert len(ROW['replacements']) == 5 and ROW['replacements'][:4] == PRIOR['replacements']
    baseline = restore(SOURCE, fn, PRIOR, rd)
    edit = ROW['replacements'][4]
    assert edit['count'] == 1 and baseline.count(edit['old']) == 1
    expected = baseline.replace(edit['old'], edit['new'], 1)
    assert restore(SOURCE, fn, ROW, rd) == expected
    assert restore(SOURCE, fn, None, rd) == SOURCE
    assert expected.count('resources:DestroyMovie(xStack_340)') == baseline.count('resources:DestroyMovie(xStack_340)') + 1
    for operation in ['resources:ReleaseResource(xStack_364)', 'quest:DeregisterTimer(', 'quest:PauseAllNonScriptedEntities(false)']:
        assert expected.count(operation) == baseline.count(operation)
    assert expected.count('this_00 = xStack_340') == baseline.count('this_00 = xStack_340') - 1
    with pytest.raises(ValueError, match='source phase'):
        restore(expected, fn, ROW, rd)

@pytest.mark.parametrize('field', ['nativeSha256', 'sourceSha256', 'luaSha256'])
def test_changed_inputs_rejected(evidence, field):
    fn, rd = evidence
    row = copy.deepcopy(ROW)
    row[field] = '0' * 64
    with pytest.raises(ValueError):
        restore(SOURCE, fn, row, rd)

@pytest.mark.parametrize('mutation', ['witness', 'begin', 'end', 'count', 'movie', 'tail', 'duplicate'])
def test_ambiguous_chatter_cleanup_rejected(evidence, mutation):
    fn, rd = evidence
    row = copy.deepcopy(ROW)
    source = SOURCE
    old = row['replacements'][4]['old']
    if mutation == 'witness':
        row['nativeWitnesses'][-1]['sha256'] = '0' * 64
    elif mutation == 'begin':
        row['spanBegin'] = 'MISSING_BEGIN'
    elif mutation == 'end':
        row['spanEnd'] = 'MISSING_END'
    elif mutation == 'count':
        row['replacements'][4]['count'] = 2
    else:
        changed = old.replace('xStack_340', 'xStack_330') if mutation == 'movie' else old.replace('LAB_00e44862', 'LAB_00e448a1') if mutation == 'tail' else old + '\n' + old
        source = source.replace(old, changed, 1)
        row['luaSha256'] = hashlib.sha256(source.encode()).hexdigest()
    with pytest.raises(ValueError):
        restore(source, fn, row, rd)
