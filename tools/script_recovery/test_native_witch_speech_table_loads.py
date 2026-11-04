import copy
import json
from pathlib import Path
from unittest.mock import patch

import pytest

R = next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').exists())
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery import native_reviewed_member_speech as speech


@pytest.fixture(scope='module')
def evidence(tmp_path_factory):
    unit = json.loads((R / 'refs/script_recovery/sick_child/units/V_SickChild.json').read_text())
    row = unit['reviewedMemberSpeech']['0x00ece7f0']
    unit['entities'] = {'Witch': unit['entities']['Witch']}
    captured = {}
    restore = speech.restore

    def capture(text, fn, review, rd, native_spec):
        if fn['address'].lower() == '0x00ece7f0':
            captured.update(text=text, fn=fn, rd=rd, spec=native_spec)
        return restore(text, fn, review, rd, native_spec)

    converter = UnitConverter(R / 'refs/script_recovery/sick_child/translation_unit_typed.json')
    with patch.object(speech, 'restore', capture), patch.object(converter, 'constructor_defaults', return_value={}):
        converter.convert(unit, tmp_path_factory.mktemp('witch_table'))
    assert captured
    return captured, row


def test_five_dead_table_loads_are_removed(evidence):
    data, row = evidence
    result = speech.restore(data['text'], data['fn'], row, data['rd'], data['spec'])
    assert data['text'].count(row['deadTableLoads']['old']) == 5
    assert result.count(row['deadTableLoads']['old']) == 0
    assert result.count('RESOURCE_MemberResource("seh_Witch")') >= 3


@pytest.mark.parametrize('fault', ('window', 'load', 'call', 'offset', 'resource', 'source', 'duplicate', 'native', 'old'))
def test_stale_dead_table_load_review_is_rejected(evidence, fault):
    data, original = evidence
    row = copy.deepcopy(original)
    source = data['text']
    native_spec = copy.copy(data['spec'])
    native_spec.parent_resources = dict(native_spec.parent_resources)
    dead = row['deadTableLoads']
    if fault == 'window':
        dead['sites'][0]['windowSha256'] = '0' * 64
    elif fault == 'load':
        dead['sites'][0]['load'] = '0xececc7'
    elif fault == 'call':
        dead['sites'][0]['call'] = '0xecece6'
    elif fault == 'offset':
        dead['offset'] = 89
    elif fault == 'resource':
        native_spec.parent_resources[88] = 'wrong'
    elif fault == 'source':
        source = source.replace(dead['old'], 'changed;', 1)
    elif fault == 'duplicate':
        dead['sites'].append(copy.deepcopy(dead['sites'][0]))
    elif fault == 'native':
        row['nativeSha256'] = '0' * 64
    else:
        dead['old'] = 'missing assignment;'
    with pytest.raises(ValueError, match='reviewed member speech'):
        speech.restore(source, data['fn'], row, data['rd'], native_spec)
