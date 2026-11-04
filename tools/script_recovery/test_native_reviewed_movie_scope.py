"""HeroTricking movie scope needs exact native lifetime evidence."""
from copy import deepcopy
import json
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_movie_scope import restore

ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    fn = converter.native('0x00e40e80')
    row = unit['reviewedMovieScopes']['0x00e40e80']
    source = '\n'.join([
        'function Main(quest, me)',
        '    local resources = quest:RetailResources()',
        '    ' + row['luaStart'],
        *['    ' + row['luaSharedCleanup']] * row['luaSharedCount'],
        '    ::LAB_00e437b3::',
        '    ' + row['luaNormalCleanup'],
        'end',
    ])
    return fn, row, source, converter.rdata


def test_reviewed_movie_scope(evidence):
    fn, row, source, rdata = evidence
    result = restore(source, fn, row, rdata)
    assert result.count('resources:DestroyMovie(trickingMovie)') == 17
    assert 'trickingMovie = resources:StartMovie("")' in result
    assert 'local trickingMovie' in result
    assert restore(source, fn, None, rdata) == source


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('nativeSha256', '0' * 64),
    lambda row: row.__setitem__('sourceSha256', '0' * 64),
    lambda row: row['nativeInstructions'].__setitem__('0x00E42E9F', '00'),
    lambda row: row['cleanupSites'][1].__setitem__('stack', -1),
    lambda row: row.__setitem__('rawMember', 'iStack_354'),
    lambda row: row.__setitem__('luaSharedCount', 15),
])
def test_stale_movie_scope_fails(evidence, mutation):
    fn, row, source, rdata = evidence
    changed = deepcopy(row)
    mutation(changed)
    with pytest.raises(ValueError, match='reviewed movie scope'):
        restore(source, fn, changed, rdata)
