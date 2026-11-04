"""Reward macros must receive a cleared map, not stale classroom arguments."""
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_evidence_lowering import fold_resource_objects
from tools.script_recovery.test_book_specific_args import generated


@pytest.mark.parametrize('operand', ['localMap', '&localMap', '(WrongType *)localMap'])
def test_native_target_identifies_local_map_clear(operand):
    source = f'  Misnamed::OnKill({operand});\n'
    assert fold_resource_objects(source, {'Misnamed::OnKill': 0x9aace0}) == '  STRINGMAP_Clear(localMap);\n'
    assert fold_resource_objects(source, {'Misnamed::OnKill': 0x123456}) == source


@pytest.mark.parametrize('operand', ['', 'map,other', 'owner->unknown', '*map', 'map + 4'])
def test_missing_or_compound_receiver_remains_explicit(operand):
    source = f'  Misnamed::OnKill({operand});\n'
    assert fold_resource_objects(source, {'Misnamed::OnKill': 0x9aace0}) == source


def run_reward(source, index, total, good, hat, key, interrupt=False):
    lua = LuaRuntime()
    lua.execute('require=function() return {} end')
    lua.execute(source)
    state = dict(BooksDonated=total, GoodBooksDonated=good, HatRewarded=hat,
                 KeyRewarded=key, ReadingBook=False)
    events, maps, stopped = [], [], [False]
    def new_map(*_):
        m = {}; maps.append(m); return m
    def clear(_, m):
        assert m is maps[0]
        events.append(('clear', dict(m))); m.clear()
    def macro(_, name, actors, strings, *flags):
        events.append(('macro', name, dict(strings)))
        if interrupt: stopped[0] = True
    noop = lambda *_: None
    resources = lua.table_from({
        'MemberResource': lambda *_: 'teacher', 'ScriptThing': lambda *_: 'teacher',
        'NewResource': lambda *_: 'hero-resource', 'NewActorMap': lambda *_: {},
        'NewStringMap': new_map, 'ClearStringMap': clear,
        'SetString': lambda _, m, k, v: m.__setitem__(k, v),
        'RunMacroWithStrings': macro, 'TryAcquire': lambda *_: True,
        'SetActor': noop, 'DestroyStringMap': lambda *_: events.append(('destroy',)),
        'DestroyActorMap': noop, 'ReleaseResource': noop})
    quest = lua.table_from({
        'RetailResources': lambda *_: resources, 'GetHero': lambda *_: 'hero',
        'ReadGlobalGameData': lambda _, off: {1248:10,1252:25,1256:35,1264:9,1268:9}.get(off,0),
        'ReadGlobalGameDataFloat': lambda *_: 0,
        'GetStateStringVectorAt': lambda _, name, i: f'BOOK_{i}',
        'GetStateInt': lambda _, k: state.get(k,0), 'SetStateInt': lambda _, k, v: state.__setitem__(k,v),
        'GetStateBool': lambda _, k: state.get(k,False), 'SetStateBool': lambda _, k, v: state.__setitem__(k,v),
        'SetStateFlagVectorAt': noop, 'TakeObjectFromHero': noop, 'GiveHeroMorality': noop,
        'GiveHeroYesNoQuestion': noop, 'MsgIsQuestionAnsweredYesOrNo': lambda *_: 1,
        'IsActiveThreadTerminating': lambda *_: stopped[0], 'GetHealth': lambda *_: 0,
        'FixMovieSequenceCamera': noop, 'FadeScreenIn': noop, 'CreateThread': noop})
    lua.globals().helper_E56D10 = noop
    def args(_, actor, i, m): m['$ARG1'] = 'CLASSROOM_ONLY'
    lua.globals().helper_E57020 = args
    lua.globals().GetBookSpecificArgs = args
    me = lua.table()
    lua.globals().Init(quest, me)
    result = (lua.globals().AskForBook or lua.globals().helper_E55CE0)(quest, me, index, 'REFUSED')
    return events, state, result


@pytest.mark.parametrize('case', [
    (8, 8, 8, False, False, 'HAT', 'OBJECT_HERO_HAT_WIZARD_GOOD'),
    (18, 10, 2, False, False, 'HAT', 'OBJECT_HERO_HAT_WIZARD_EVIL'),
    (24, 24, 9, True, False, 'KEY', 'OBJECT_SILVER_KEY'),
    (2, 2, 2, False, False, None, None),
    (24, 24, 9, True, True, None, None),
])
def test_generated_reward_receives_only_prize(generated, case):
    index,total,good,hat,key,kind,prize = case
    for _, source in generated:
        events,state,result = run_reward(source,index,total,good,hat,key)
        expected = [('macro', f'CS_SCHOOLBOOK_{index}', {'$ARG1':'CLASSROOM_ONLY'})]
        if kind:
            expected += [('clear', {'$ARG1':'CLASSROOM_ONLY'}),
                         ('macro', f'CS_SCHOOLBOOK_TEACHER_GIVES_{kind}', {'$PRIZE':prize})]
        assert events == expected + [('destroy',)]
        assert result and state['ReadingBook'] is False
        assert state['HatRewarded'] == (hat or kind=='HAT')
        assert state['KeyRewarded'] == (key or kind=='KEY')


def test_interruption_before_reward_does_not_clear_or_award(generated):
    for _, source in generated:
        events,state,result = run_reward(source,8,8,8,False,False,interrupt=True)
        assert events == [('macro','CS_SCHOOLBOOK_8',{'$ARG1':'CLASSROOM_ONLY'}),('destroy',)]
        assert not result and not state['HatRewarded'] and not state['KeyRewarded']
