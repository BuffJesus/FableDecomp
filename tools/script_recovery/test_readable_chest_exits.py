"""The complete readable pipeline retains Open Chest's cancellation/cleanup trace."""
from pathlib import Path

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery import readable_style
from tools.script_recovery.build_readable_unit import readable_file

ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def variants():
    draft = (ROOT / 'refs/script_recovery/lifted/Expressions/draft/FSE/Global_OpenChest/Global_OpenChest.lua').read_text()
    with pytest.MonkeyPatch.context() as patch:
        patch.setattr(readable_style, 'fold_shared_exit_tests', lambda lines: (lines, 0))
        before = readable_file(draft)[0]
    after = readable_file(draft)[0]
    assert before != after
    assert '::LAB_00eecc47::' in before and '::LAB_00eecc47::' not in after
    return before, after


def run(source, scenario):
    lua = LuaRuntime()
    trace, frame = [], 0
    quest, hero, chest, resources = (lua.table() for _ in range(4))
    def bind(obj, prefix, name, result=None):
        def call(*args):
            trace.append(prefix + name)
            return result() if callable(result) else result
        obj[name] = call
    def next_frame():
        nonlocal frame
        frame += 1
        assert frame <= 5, 'Unexpected unbounded wait'
        return not (scenario == 'shutdown' and frame >= 2)
    bind(quest, 'quest.', 'NewScriptFrame', next_frame)
    bind(quest, 'quest.', 'GetHero', lambda: hero)
    bind(quest, 'quest.', 'RetailResources', lambda: resources)
    bind(quest, 'quest.', 'GetActiveQuestName', 'Global_OpenChest')
    bind(quest, 'quest.', 'IsHeroControlledByPlayer', True)
    bind(quest, 'quest.', 'IsActiveThreadTerminating', lambda: scenario == 'shutdown' and frame >= 2)
    bind(quest, 'quest.', 'GetMostRecentValidUsedTarget', lambda: chest)
    bind(quest, 'quest.', 'GetNumberOfKeysNeededToUnlockChest', 0)
    bind(quest, 'quest.', 'IsEntityWieldingMeleeWeapon', False)
    bind(quest, 'quest.', 'IsEntityWieldingRangedWeapon', False)
    bind(quest, 'quest.', 'OpenChest', scenario != 'failed_open')
    bind(quest, 'quest.', 'IsChestOpen', lambda: frame >= 3)
    bind(quest, 'quest.', 'MsgOnChestOpeningCancelled', lambda: scenario == 'cancel' and frame >= 2)
    bind(quest, 'quest.', 'GetItemDefNamesFromContainer', lambda: lua.table())
    bind(quest, 'quest.', 'MsgOnHeroRewardedWithItemsFrom', True)
    for name in ('SetQuestAsPersistent', 'DeactivateQuestLater', 'SetToKeepHeroAbilitiesDuringCutscenes',
                 'SetToDisplayTutorialsDuringCutscenes', 'SetCutsceneMode', 'EntitySetCutsceneBehaviour',
                 'PauseAllEntities', 'CameraDefault'):
        bind(quest, 'quest.', name)
    bind(chest, 'chest.', 'IsAlive', True)
    bind(hero, 'hero.', 'MsgIsHitBy', lambda: scenario in ('hit', 'both_hits') and frame >= 2)
    bind(hero, 'hero.', 'MsgIsHitByAnySpecialAbilityFrom', lambda: scenario in ('special', 'both_hits') and frame >= 2)
    bind(resources, 'resources.', 'NewResource', 1)
    bind(resources, 'resources.', 'TryAcquire', True)
    bind(resources, 'resources.', 'StartMovie', 2)
    for name in ('ClearAllActionsIncludingLoopingAnimations', 'DestroyMovie', 'ReleaseResource'):
        bind(resources, 'resources.', name)
    lua.execute(source)
    lua.globals().Main(quest)
    return trace


@pytest.mark.parametrize('scenario', ['normal', 'failed_open', 'cancel', 'hit', 'special', 'both_hits', 'shutdown'])
def test_chest_exit_paths_preserve_calls_and_short_circuit_order(variants, scenario):
    before, after = (run(source, scenario) for source in variants)
    assert after == before
    assert 'resources.DestroyMovie' in after and 'resources.ReleaseResource' in after
    if scenario in ('hit', 'both_hits'):
        assert 'hero.MsgIsHitBy' in after
        assert 'hero.MsgIsHitByAnySpecialAbilityFrom' not in after
    if scenario == 'special':
        assert after.index('hero.MsgIsHitBy') < after.index('hero.MsgIsHitByAnySpecialAbilityFrom')
    if scenario in ('failed_open', 'cancel', 'shutdown'):
        assert 'hero.MsgIsHitBy' not in after
