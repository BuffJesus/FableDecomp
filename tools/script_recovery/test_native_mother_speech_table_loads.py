import copy
import json
from pathlib import Path
from unittest.mock import patch

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery import native_reviewed_member_speech as speech


ROOT = Path(__file__).resolve().parents[2]
UNIT = ROOT / 'refs/script_recovery/sick_child/units/V_SickChild.json'
TYPED = ROOT / 'refs/script_recovery/sick_child/translation_unit_typed.json'


@pytest.fixture(scope='module')
def evidence(tmp_path_factory):
    unit = json.loads(UNIT.read_text())
    row = unit['reviewedMemberSpeech']['0x00ecd9e0']
    unit['entities'] = {'SickChildsMother': unit['entities']['SickChildsMother']}
    unit['quest']['functions'] = {}
    unit['quest']['helpers'] = {}
    # This reduced helper inventory isolates the earlier speech review.
    unit.get('reviewedParentMemberHealthLifetimes', {}).pop('0x00ecd9e0', None)
    captured = {}
    restore = speech.restore

    def capture(text, fn, review, rd, native_spec):
        if fn['address'].lower() == '0x00ecd9e0':
            captured.update(text=text, fn=fn, rd=rd, spec=native_spec)
        return restore(text, fn, review, rd, native_spec)

    converter = UnitConverter(TYPED)
    with patch.object(speech, 'restore', capture), patch.object(converter, 'constructor_defaults', return_value={}):
        converter.convert(unit, tmp_path_factory.mktemp('mother_table'))
    assert captured
    return captured, row


def test_review_removes_only_three_paired_table_loads(evidence):
    data, row = evidence
    result = speech.restore(data['text'], data['fn'], row, data['rd'], data['spec'])
    assert result.count(row['deadTableLoads']['old']) == 0
    assert data['text'].count(row['deadTableLoads']['old']) == 3
    assert result.count('RESOURCE_MemberResource("seh_Mother")') == data['text'].count('RESOURCE_MemberResource("seh_Mother")') + 1
    assert speech.restore(data['text'], data['fn'], None, data['rd'], data['spec']) == data['text']


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
        dead['sites'][0]['load'] = '0xecdf32'
    elif fault == 'call':
        dead['sites'][0]['call'] = '0xecdf51'
    elif fault == 'offset':
        dead['offset'] = 88
    elif fault == 'resource':
        native_spec.parent_resources[72] = 'wrong'
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
