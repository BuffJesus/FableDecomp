"""Native and binding evidence for Magicman's conversation lines."""
from copy import deepcopy
import json
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_magicman_conversation import restore

ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    fn = converter.native('0x00e40e80')
    row = unit['reviewedMagicmanConversation']['0x00e40e80']
    source = '\n'.join('                    ' + statement for entry in row['patterns']
                       for statement in entry['assignments'] + entry['remove'] + [entry['call']]) + '\n'
    todo = ['unlifted: ' + statement.removeprefix('-- TODO(native): ')
            for entry in row['patterns'] for statement in entry['remove'] + [entry['call']]]
    return fn, row, source, todo, converter.rdata


def test_reviewed_conversation(evidence):
    fn, row, source, todo, rdata = evidence
    result, remaining, calls = restore(source, todo.copy(), [], fn, row, rdata)
    assert result.count('quest:AddLineToConversation(') == 2
    assert '-- TODO(native):' not in result
    assert remaining == [] and calls == ['AddLineToConversation'] * 2
    assert restore(source, todo, [], fn, None, rdata) == (source, todo, [])


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('nativeSha256', '0' * 64),
    lambda row: row.__setitem__('sourceSha256', '0' * 64),
    lambda row: row['nativeInstructions'].__setitem__('0x00E414DA', '00'),
    lambda row: row['sites'].__setitem__(0, '0x00E414D9'),
    lambda row: row.__setitem__('bindingSha256', '0' * 64),
    lambda row: row['patterns'][0]['assignments'].__setitem__(0, 'bad'),
])
def test_stale_conversation_fails(evidence, mutation):
    fn, row, source, todo, rdata = evidence
    changed = deepcopy(row)
    mutation(changed)
    with pytest.raises(ValueError, match='reviewed Magicman conversation'):
        restore(source, todo.copy(), [], fn, changed, rdata)
