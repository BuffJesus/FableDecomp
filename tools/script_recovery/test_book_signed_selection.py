"""Empty book definitions take the native -1/no-book branch in both Lua forms."""
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.test_book_specific_args import generated


@pytest.mark.parametrize('terminate', [False, True])
@pytest.mark.parametrize('health', [0, 100])
def test_empty_inventory_definition_set_uses_none_found(generated, terminate, health):
    for _, source in generated:
        lua = LuaRuntime()
        lua.execute('package.preload["V_BookCollecting.native_quest_helpers"] = function() return {} end')
        lua.execute(source)
        spoken, health_reads = [], []
        me = lua.table_from({'Speak': lambda _, hero, key, *args: spoken.append((hero,key)) or True,
                             'IsPerformingScriptTask': lambda _: False})
        resources = lua.table_from({'MemberResource': lambda *_: 'teacher-resource',
                                    'ScriptThing': lambda *_: me,
                                    'IsPerformingScriptTask': lambda *_: False})
        quest = lua.table_from({'RetailResources': lambda _: resources,
            'ReadGlobalGameData': lambda _, offset: {1248:3,1252:5,1256:7}.get(offset,0),
            'ReadGlobalGameDataFloat': lambda *_: 0,
            'GetStateInt': lambda *_: 0, 'GetHero': lambda _: 'hero',
            'GetHealth': lambda *_: health_reads.append(True) or health,
            'IsActiveThreadTerminating': lambda _: terminate})
        lua.globals().Init(quest,me)
        (lua.globals().LookForBook or lua.globals().helper_E57530)(quest,me)
        assert spoken == ([('hero','TEXT_QST_B16_BOOK_NONE_FOUND')] if health and not terminate else [])
        assert len(health_reads) == (0 if terminate else 1)
