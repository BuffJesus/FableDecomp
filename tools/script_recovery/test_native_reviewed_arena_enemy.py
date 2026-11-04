import copy
import json
from pathlib import Path
from unittest.mock import patch

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_arena_enemy import MAIN, ON_PREDICATE_FAIL, recover_arena_enemy


@pytest.fixture(scope='module')
def captured(tmp_path_factory):
    root = Path(__file__).resolve().parents[2]
    unit = json.loads((root / 'refs/script_recovery/arena/units/Q_Arena.json').read_text())
    converter = UnitConverter(root / 'refs/script_recovery/arena/translation_unit_typed.json')
    seen = {}

    def capture(source, fn, rdata):
        address = int(fn['address'], 16)
        if address in (MAIN, ON_PREDICATE_FAIL):
            seen[address] = (source, fn, rdata)
        return recover_arena_enemy(source, fn, rdata)

    with patch('tools.script_recovery.native_reviewed_arena_enemy.recover_arena_enemy', capture), \
            patch.object(converter, 'constructor_defaults', return_value={}):
        converter.convert(unit, tmp_path_factory.mktemp('arena_enemy'))
    assert set(seen) == {MAIN, ON_PREDICATE_FAIL}
    return seen


@pytest.mark.parametrize('newline', ['\n', '\r\n'])
def test_main_round_reads_and_health_bar(captured, newline):
    source, fn, rdata = captured[MAIN]
    result = recover_arena_enemy(source.replace('\n', newline), fn, rdata)
    assert result.count('"_NumWaveCreatures"') == 2
    assert result.count('_Creatures_" .. ctr_50 .. "_CreatureType"') == 1
    assert 'ENGINE_Colour(255, 255, 0, 255)' in result
    assert 'iVar9 + 0x28 + iVar11' not in result and 'iVar11 = iVar11 + 0x38' not in result
    assert '0xffffff00' not in result


def test_on_predicate_fail_counters(captured):
    source, fn, rdata = captured[ON_PREDICATE_FAIL]
    result = recover_arena_enemy(source, fn, rdata)
    assert 'QUESTSTATE_SetInt(__key("TotalCreatures_" .. *(int *)(this + 0x24)), ' \
           'QUESTSTATE_GetInt(__key("TotalCreatures_" .. *(int *)(this + 0x24))) - 1);' in result
    assert '_Creatures_" .. *(int *)(this + 0x24) .. "_DeathScore"' in result
    assert 'piVar1' not in result.split('{', 1)[1].split('return', 1)[0].replace('int *piVar1;', '')


@pytest.mark.parametrize('address', [MAIN, ON_PREDICATE_FAIL])
def test_native_drift_leaves_source_unchanged(captured, address):
    source, fn, rdata = captured[address]

    class Drifted:
        def bytes_at(self, at, size):
            data = bytearray(rdata.bytes_at(at, size))
            data[-1] ^= 0xff
            return bytes(data)
    assert recover_arena_enemy(source, fn, Drifted()) == source


@pytest.mark.parametrize('edit', [
    ('iVar11 = iVar11 + 0x38;', 'iVar11 = iVar11 + 0x3c;'),          # cursor no longer the group stride
    ('  iVar11 = 0;\n', '  iVar11 = 4;\n'),                           # cursor not from zero
    ('ctr_50 = (CCharString)0xffffff00;', 'ctr_50 = (CCharString)0xff0000ff;'),   # colour changed
    ('+ 0x28 + *(int *)(*(int *)(this + 0x14) + 0xac) * 0x3c)', '+ 0x30 + *(int *)(*(int *)(this + 0x14) + 0xac) * 0x3c)'),
])
def test_main_text_drift_is_all_or_nothing(captured, edit):
    source, fn, rdata = captured[MAIN]
    changed = source.replace('\r\n', '\n').replace(*edit, 1)
    assert changed != source.replace('\r\n', '\n')
    assert recover_arena_enemy(changed, fn, rdata) == changed


def test_other_functions_untouched(captured):
    source, fn, rdata = captured[MAIN]
    other = copy.deepcopy(fn)
    other['address'] = '0x00f1b9c0'
    assert recover_arena_enemy(source, other, rdata) == source
