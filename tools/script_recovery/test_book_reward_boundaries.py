"""Threshold and precedence checks against freshly generated Teacher Lua."""
import pytest

from tools.script_recovery.test_book_specific_args import generated
from tools.script_recovery.test_book_reward_maps import run_reward


@pytest.mark.parametrize('index,total,good,hat,key,prizes', [
    (7, 7, 7, False, False, []),
    (8, 8, 8, False, False, ['OBJECT_HERO_HAT_WIZARD_GOOD']),
    (9, 9, 9, False, False, ['OBJECT_HERO_HAT_WIZARD_GOOD']),
    (18, 9, 2, False, False, []),
    (18, 10, 2, False, False, ['OBJECT_HERO_HAT_WIZARD_EVIL']),
    (18, 11, 2, False, False, ['OBJECT_HERO_HAT_WIZARD_EVIL']),
    (18, 18, 9, False, False, ['OBJECT_HERO_HAT_WIZARD_GOOD']),
    (24, 24, 9, False, False, ['OBJECT_HERO_HAT_WIZARD_GOOD']),
    (24, 23, 9, True, False, []),
    (24, 24, 9, True, False, ['OBJECT_SILVER_KEY']),
    (24, 25, 9, True, False, []),
    (24, 24, 9, True, True, []),
])
def test_thresholds_precedence_and_exact_key_total(generated, index, total, good, hat, key, prizes):
    # run_reward supplies synthetic thresholds: 9 good/evil, 25 accepted.
    # The native donation path increments total, and good for indices below10.
    for _, source in generated:
        events, state, result = run_reward(source, index, total, good, hat, key)
        awards = [e[2] for e in events if e[0] == 'macro' and 'TEACHER_GIVES' in e[1]]
        assert awards == [{'$PRIZE': prize} for prize in prizes]
        assert sum(e[0] == 'clear' for e in events) == len(prizes)
        assert events[-1] == ('destroy',)
        assert state['BooksDonated'] == total + 1
        assert state['GoodBooksDonated'] == good + (index < 10)
        assert state['HatRewarded'] == (hat or any('HAT' in p for p in prizes))
        assert state['KeyRewarded'] == (key or 'OBJECT_SILVER_KEY' in prizes)
        assert result and not state['ReadingBook']
