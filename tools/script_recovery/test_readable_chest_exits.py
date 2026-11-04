"""The complete readable pipeline retains Open Chest's cancellation/cleanup trace."""
from pathlib import Path

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery import readable_style
from tools.script_recovery.build_readable_unit import readable_file

ROOT = next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file())


@pytest.fixture(scope='module')
def variants():
    draft = (ROOT / 'refs/script_recovery/lifted/Expressions/draft/FSE/Global_OpenChest/Global_OpenChest.lua').read_text()
    # This regression targets shared-exit folding with the harness's fixed hero.
    # Make that input assumption explicit, independently of production lookup
    # preservation. The separate lookup-order regression uses the retail draft.
    # Shared-exit folding is tested on a flat function. Callback lifetime is
    # covered by complete scoped Main models, and lookup tests use the actual
    # production callback. Keep this fixture's presentation boundary explicit.
    if '    return quest:WithRetailResources(function(resources)\n' in draft:
        assert draft.count('    return quest:WithRetailResources(function(resources)\n') == 1
        assert draft.count('\n    end)\nend\n') == 1
        draft = draft.replace('    return quest:WithRetailResources(function(resources)\n',
                              '    local resources = quest:RetailResources()\n', 1)
        draft = draft.replace('\n    end)\nend\n', '\nend\n', 1)
    draft = draft.replace('quest:GetHero()', 'hero')
    draft = draft.replace('function Main(quest)\n', 'function Main(quest)\n    local hero = quest:GetHero()\n', 1)
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
    # Existing exit/lookup scenarios supply a live conscious hero. Native
    # condition evaluation is covered by the dedicated Chest condition models.
    condition_registered = False
    def register_condition(*args):
        nonlocal condition_registered
        assert frame == 0 and not condition_registered
        assert lua.eval('rawequal')(args[-1], hero)
        condition_registered = True
        trace.append('quest.RegisterQuestConsciousCondition')
    quest['RegisterQuestConsciousCondition'] = register_condition
    bind(quest, 'quest.', 'NewScriptFrame', next_frame)
    bind(quest, 'quest.', 'GetHero', lambda: hero)
    bind(quest, 'quest.', 'RetailResources', lambda: resources)
    # Scope creation and target ownership are host operations, with the same
    # fixture resource table. Dedicated scope models verify fallback retirement.
    def with_resources(*args):
        trace.append('quest.RetailResources')
        return args[-1](resources)
    quest['WithRetailResources'] = with_resources
    resources['TrackThingOutput'] = lambda *_: 1
    resources['RetireThingOutput'] = lambda *_: None
    bind(quest, 'quest.', 'GetActiveQuestName', 'Global_OpenChest')
    bind(quest, 'quest.', 'IsHeroControlledByPlayer', True)
    bind(quest, 'quest.', 'IsActiveThreadTerminating', lambda: scenario == 'shutdown' and frame >= 2)
    bind(quest, 'quest.', 'GetMostRecentValidUsedTarget', lambda: chest)
    bind(quest, 'quest.', 'GetNumberOfKeysNeededToUnlockChest', lambda: lua.table_from({'count':0}))
    bind(quest, 'quest.', 'IsEntityWieldingMeleeWeapon', False)
    bind(quest, 'quest.', 'IsEntityWieldingRangedWeapon', False)
    bind(quest, 'quest.', 'OpenChest', scenario != 'failed_open')
    bind(quest, 'quest.', 'IsChestOpen', lambda: frame >= 3)
    bind(quest, 'quest.', 'MsgOnChestOpeningCancelled', lambda: scenario == 'cancel' and frame >= 2)
    bind(quest, 'quest.', 'GetItemDefNamesFromContainer', lambda: lua.table())
    # This fixture supplies an empty native item vector. Count recovery has
    # separate null-string and delayed-reward models.
    bind(quest, 'quest.', 'GetContainerItemDefCount', 0)
    bind(quest, 'quest.', 'MsgOnHeroRewardedWithItemsFrom', True)
    for name in ('SetQuestAsPersistent', 'DeactivateQuestLater', 'SetToKeepHeroAbilitiesDuringCutscenes',
                 'SetToDisplayTutorialsDuringCutscenes', 'SetCutsceneMode', 'EntitySetCutsceneBehaviour',
                 'PauseAllEntities', 'CameraDefault'):
        bind(quest, 'quest.', name)
    # Draft handles use an explicit pure null check; readable handles are nullable.
    chest['IsNull'] = lambda *_: False
    hero['IsNull'] = lambda *_: False
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
