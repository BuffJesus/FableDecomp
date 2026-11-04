"""Movie member alias reviews require matching raw and native evidence."""
from copy import deepcopy
import json
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_movie_members import restore

ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    fn = converter.native('0x00e40e80')
    row = unit['reviewedMovieMembers']['0x00e40e80']
    first, second = row['pairs']
    text = '\n'.join([
        first['startMarker'],
        *[first['restoredMembers'][0]] * first['memberCounts'][0],
        *[first['restoredMembers'][1]] * first['memberCounts'][1],
        first['endMarker'],
        second['startMarker'],
        *[second['restoredMembers'][0]] * second['memberCounts'][0],
        *[second['restoredMembers'][1]] * second['memberCounts'][1],
    ])
    return fn, row, text, converter.rdata


def test_reviewed_movie_members(evidence):
    fn, row, source, rdata = evidence
    result = restore(source, fn, row, rdata)
    for pair in row['pairs']:
        assert pair['restoredObject'] in result
        for old in pair['restoredMembers']:
            assert old not in result
    assert restore(source, fn, None, rdata) == source


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('nativeSha256', '0' * 64),
    lambda row: row.__setitem__('sourceSha256', '0' * 64),
    lambda row: row['nativeInstructions'].__setitem__('0x00E42023', '00'),
    lambda row: row['pairs'][0].__setitem__('rawMember', 'uStack_2f8'),
    lambda row: row['pairs'][1].__setitem__('cleanupStack', -1),
])
def test_stale_movie_evidence_fails(evidence, mutation):
    fn, row, source, rdata = evidence
    changed = deepcopy(row)
    mutation(changed)
    with pytest.raises(ValueError, match='reviewed movie members'):
        restore(source, fn, changed, rdata)
