"""Generated initialization and persistence dispatch for quest flag vectors.

Native serialization/ownership is checked separately; this mock tests Lua's
state wiring and exact transfer order, not a successful donation or real save.
"""
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.test_book_reaction_lowering import generated


@pytest.mark.parametrize('count', [0, 1, 40])
def test_generated_init_resets_each_flag_and_persist_dispatches_both_vectors(generated, count):
    _, roots, _ = generated
    for source in roots:
        lua = LuaRuntime()
        lua.execute('package.preload["V_BookCollecting.native_quest_helpers"] = function() return {} end')
        lua.execute(source)
        state, vectors, transfers = {}, {}, []
        strings = {}
        def resize(_, name, size):
            assert size == count
            old = vectors.get(name, [])
            vectors[name] = (old + [False] * size)[:size]
        def write(_, name, index, value):
            assert type(value) is bool and 0 <= index < count
            vectors[name][index] = value
        q = lua.table_from({
            'SnapshotGlobalConversations': lambda *_: None,  # This test isolates flag persistence.
            'ReadGlobalGameData': lambda _, offset: count if offset == 0x4ec else 0,
            'SetStateInt': lambda _, key, value: state.update({key: value}),
            'SetStateBool': lambda _, key, value: state.update({key: value}),
            'GetStateInt': lambda _, key: state[key],
            'GetStateBool': lambda _, key: state[key],
            'StateStringVectorResize': lambda _, k, n: strings.__setitem__(k, (strings.get(k, []) + [''] * n)[:n]),
            'ReadGlobalGameDataStringAt': lambda _, off, i: 'BOOK_' + str(i),
            'SetStateStringVectorAt': lambda _, k, i, v: strings[k].__setitem__(i, v),
            'StateFlagVectorResize': resize, 'SetStateFlagVectorAt': write,
            'PersistTransferBool': lambda _, ctx, key, value: value,
            'PersistTransferInt': lambda _, ctx, key, value: value,
            'PersistStateFlagVector': lambda _, ctx, key: transfers.append((ctx, key, vectors[key][:])),
        })
        lua.globals().Init(q)
        assert vectors == {'BookDonated': [False] * count, 'BookOwned': [False] * count}
        if count:
            vectors['BookDonated'][0] = True
            vectors['BookOwned'][-1] = True
        lua.globals().OnPersist(q, 'native-context')
        assert transfers == [('native-context', name, vectors[name]) for name in ('BookDonated', 'BookOwned')]
        # resize retains existing entries: the native zero-store loop, rather
        # than a conveniently-clearing mock resize, must reset these flags.
        lua.globals().Init(q)
        assert vectors == {'BookDonated': [False] * count, 'BookOwned': [False] * count}
