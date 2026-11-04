"""Native health byte and Lua speech gate for Madame's beer return."""
from copy import deepcopy
import json
from pathlib import Path

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_madame_beer_health import AFTER, BEFORE, DIAGNOSTIC, restore


ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    return (converter.native('0x00e3bb70'), unit['reviewedMadameBeerHealth']['0x00e3bb70'],
            converter.rdata)


def test_health_controls_speech_not_beer_return(evidence):
    fn, row, rdata = evidence
    source = ('::LAB_00e3bf28::\n' + BEFORE + '\n'
              '                me:Speak(hero, "TEXT_QST_B13_MADAME_GIVEN_BEER")\n'
              '            end\n'
              '            quest:GiveHeroObject("OBJECT_BEER_TANKARD", -1, true)')
    result, todo = restore(source, [DIAGNOSTIC], fn, row, rdata)
    assert result.count(AFTER) == 1 and todo == []
    assert result.index('if xStack_124_b3 then') < result.index('me:Speak')
    assert result.index('            end\n            quest:GiveHeroObject') > result.index('me:Speak')
    assert restore(source, [DIAGNOSTIC], fn, None, rdata) == (source, [DIAGNOSTIC])
    lua = LuaRuntime()
    script = ('local xStack_164 = 0\nlocal xStack_124_b3\nlocal CVar2\n' + AFTER +
              '\n                speech = true\n            end\nreturn xStack_124_b3')
    for health, expect_speech in ((10.0, True), (0.0, False), (-1.0, False),
                                  (float('nan'), False)):
        lua.globals().fVar14 = health
        assert lua.execute(script) is expect_speech


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('nativeSha256', '0' * 64),
    lambda row: row.__setitem__('sourceSha256', '0' * 64),
    lambda row: row.__setitem__('zeroAddress', '0x0122DED8'),
    lambda row: row['nativeInstructions'].__setitem__('0x00E3BFD9', '00'),
])
def test_stale_health_evidence_rejected(evidence, mutation):
    fn, row, rdata = evidence
    changed = deepcopy(row)
    mutation(changed)
    with pytest.raises(ValueError, match='reviewed Madame beer health'):
        restore('::LAB_00e3bf28::\n' + BEFORE + '\nquest:GiveHeroObject("OBJECT_BEER_TANKARD", -1, true)',
                [DIAGNOSTIC], fn, changed, rdata)


def test_unrelated_health_lift_is_not_rewritten(evidence):
    fn, row, rdata = evidence
    with pytest.raises(ValueError, match='lifted health gate changed'):
        restore('::LAB_00e3bf28::\n' + BEFORE.replace('fVar14', 'fVar15') +
                '\nquest:GiveHeroObject("OBJECT_BEER_TANKARD", -1, true)',
                [DIAGNOSTIC], fn, row, rdata)
