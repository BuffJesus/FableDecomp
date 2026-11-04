"""A reviewed temporary string cleanup is removed only with exact evidence."""
from copy import deepcopy
import json
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_temporary_cleanup import restore

ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    fn = converter.native('0x00e40e80')
    row = unit['reviewedTemporaryCleanup']['0x00e40e80']
    source = ('construct("OBJECT_HERO_HAT_PIMP");\n'
              '          ' + row['flagSet'] + '\n'
              '          live_call();\n'
              '        ' + row['guard'] + '\n'
              '          ' + row['guardParts'][0] + '\n'
              '          ' + row['guardParts'][1] + 'type>\n'
              '                    ' + row['guardParts'][2] + '\n'
              '        }\n'
              '        continue_game();\n')
    return fn, row, source, converter.rdata


def test_native_reviewed_string_cleanup(evidence):
    fn, row, source, rdata = evidence
    result = restore(source, fn, row, rdata)
    assert 'construct("OBJECT_HERO_HAT_PIMP")' in result
    assert 'live_call()' in result and 'continue_game()' in result
    assert row['flagSet'] not in result and row['guard'] not in result
    assert restore(source, fn, None, rdata) == source


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('nativeSha256', '0' * 64),
    lambda row: row.__setitem__('sourceSha256', '0' * 64),
    lambda row: row['nativeInstructions'].__setitem__('0x00E418E8', '00'),
    lambda row: row.__setitem__('flagSet', 'missing flag;'),
    lambda row: row['guardParts'].__setitem__(2, '(other);'),
])
def test_stale_cleanup_evidence_fails(evidence, mutation):
    fn, row, source, rdata = evidence
    changed = deepcopy(row)
    mutation(changed)
    with pytest.raises(ValueError, match='reviewed temporary cleanup'):
        restore(source, fn, changed, rdata)
