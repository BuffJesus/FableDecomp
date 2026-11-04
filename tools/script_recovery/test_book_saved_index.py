"""The generated scan must retain its selected index across request dialogue."""
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.test_book_specific_args import generated


def run_selection(source, have=(), seen=(), donated=(), last=-1, count=8,
                  health=100, interrupt=False, bounds=(3, 5, 7), speech=None):
    lua = LuaRuntime()
    lua.execute('require=function() return {} end')
    lua.execute(source)
    spoken, requests, reads, frames = [], [], [], []
    flags = {'BookOwned': set(seen), 'BookDonated': set(donated)}
    polls = []
    readable = source.startswith('-- Readable')
    def speak(_, hero, key, *args):
        spoken.append(key)
        if speech is not None:
            speech.append((hero, key, *args))
        if readable:
            # Readable folds the explicit poll loop into blocking Speak's
            # return value. Its binding yields and returns false on termination.
            frames.append(True)
            return not interrupt
        return True
    def busy(*_):
        polls.append(True)
        return not readable and len(polls) == 1
    me = lua.table_from({'Speak': speak,
                        'IsPerformingScriptTask': busy})
    resources = lua.table_from({'MemberResource': lambda *_: 'teacher',
                               'ScriptThing': lambda *_: me,
                               'IsPerformingScriptTask': busy})
    def owns(_, definition, hero):
        assert hero == 'hero'
        reads.append(definition)
        return definition in have
    quest = lua.table_from({'RetailResources': lambda _: resources,
        'ReadGlobalGameData': lambda _, off: dict(zip((1248, 1252, 1256), bounds)).get(off, 0),
        'ReadGlobalGameDataFloat': lambda *_: 0,
        'GetStateStringVectorAt': lambda _, name, i: i if name == 'BookDef' else None,
        'GetStateInt': lambda _, key: {'BooksInGame': count, 'LastBookRequested': last}.get(key, 0),
        'GetHero': lambda _: 'hero', 'GetHealth': lambda *_: health,
        'IsObjectInThingsPossession': owns,
        'GetStateFlagVectorAt': lambda _, key, i: i in flags[key],
        'SetStateFlagVectorAt': lambda _, key, i, v: flags[key].add(i),
        'NewScriptFrame': lambda *_: frames.append(True) or True,
        'IsActiveThreadTerminating': lambda _: bool(interrupt and frames)})
    def ask(_, actor, index, key):
        assert actor == me or actor is not None
        requests.append((index, key))
    lua.globals().helper_E55CE0 = ask
    lua.globals().AskForBook = ask
    lua.globals().Init(quest, me)
    (lua.globals().LookForBook or lua.globals().helper_E57530)(quest, me)
    return dict(spoken=spoken, requests=requests, reads=reads,
                seen=sorted(flags['BookOwned']), frames=len(frames))


@pytest.mark.parametrize('inputs,chosen', [
    ({}, -1), ({'have': [0, 1]}, 0), ({'have': [0, 1], 'seen': [0]}, 1),
    ({'have': [0, 1], 'seen': [0, 1]}, 1), ({'have': [1, 2]}, 1),
    ({'have': [0, 1], 'donated': [0]}, 1), ({'have': [0], 'donated': [0]}, -2),
    ({'have': [0], 'last': 0}, -2), ({'have': [4, 6]}, 4), ({'count': 0}, -1)])
def test_native_scan_cases_and_post_dialogue_reload(generated, inputs, chosen):
    for _, source in generated:
        result = run_selection(source, **inputs)
        if chosen >= 0:
            assert result['requests'] == [(chosen, 'TEXT_QST_B16_BOOK_REFUSED')]
            assert result['spoken'] == [f'TEXT_QST_B16_BOOK_REQUEST_{chosen}']
        else:
            assert result['requests'] == []
            suffix = 'NONE_FOUND' if chosen == -1 else 'NOT_FOUND'
            assert result['spoken'] == ['TEXT_QST_B16_BOOK_' + suffix]
        assert result['frames'] == 1


@pytest.mark.parametrize('health,interrupt', [(0, False), (100, True)])
def test_request_health_and_interruption_branches(generated, health, interrupt):
    for _, source in generated:
        result = run_selection(source, have=[1, 2], health=health, interrupt=interrupt)
        # Retail E578A5 bypasses speech at zero health but still calls AskForBook.
        assert result['requests'] == ([] if interrupt else [(1, 'TEXT_QST_B16_BOOK_REFUSED')])
        assert result['spoken'] == (['TEXT_QST_B16_BOOK_REQUEST_1'] if health else [])
