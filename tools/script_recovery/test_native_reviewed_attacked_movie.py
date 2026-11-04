"""Native evidence for Magicman's attacked movie early exits."""
from copy import deepcopy
import json
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_attacked_movie import restore

ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    fn = converter.native('0x00e40e80')
    row = unit['reviewedAttackedMovie']['0x00e40e80']
    source = '\n'.join([row['luaStart'], *[row['luaWrongCleanup']] * row['luaWrongCount'],
                        row['luaNormalCleanup']])
    return fn, row, source, converter.rdata


def test_reviewed_attacked_movie(evidence):
    fn, row, source, rdata = evidence
    result = restore(source, fn, row, rdata)
    assert result.count(row['luaNormalCleanup']) == 7
    assert row['luaWrongCleanup'] not in result
    assert restore(source, fn, None, rdata) == source


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('nativeSha256', '0' * 64),
    lambda row: row.__setitem__('sourceSha256', '0' * 64),
    lambda row: row['nativeInstructions'].__setitem__('0x00E43965', '00'),
    lambda row: row['cleanupSites'][0].__setitem__('stack', -1),
    lambda row: row.__setitem__('rawMember', 'uStack_308'),
    lambda row: row.__setitem__('luaWrongCount', 5),
])
def test_stale_attacked_movie_fails(evidence, mutation):
    fn, row, source, rdata = evidence
    changed = deepcopy(row)
    mutation(changed)
    with pytest.raises(ValueError, match='reviewed attacked movie'):
        restore(source, fn, changed, rdata)
