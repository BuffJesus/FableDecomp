import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.lift_native_lua import Lifter, load_manifest, converter_signatures
from tools.script_recovery.test_lift_native_lua import NoRData


def make():
    lifter = Lifter(converter_signatures(load_manifest()), {}, 'quest', True, 'Pkg', NoRData())
    lifter.accessor_kinds = True
    return lifter


@pytest.mark.parametrize('zero', ['0', '0x0'])
@pytest.mark.parametrize('empty', [False, True])
def test_initialized_lookup_output_is_cached_for_repeated_uses(zero, empty):
    lifter = make()
    body = '\n'.join(lifter.lift('Main', '''{
xStack_10 = ZERO;
CCharString::CCharString(xStack_20,"LookoutPointBeggar",-1);
GSI->GetThingWithScriptName(xStack_10,xStack_20);
GSI->AddPersonToConversation(7,&xStack_10);
CCharString::CCharString(xStack_30,"TEXT_QST_015_BULLY_TAUNT",-1);
GSI->AddLineToConversation(7,xStack_30,0,me,&xStack_10);
}'''.replace('ZERO', zero)))
    lua = LuaRuntime()
    actor = None if empty else lua.table()
    speaker = lua.table()
    calls = []
    def lookup(_, name):
        calls.append(('lookup', name))
        return actor
    # Lua identity matters: table proxies returned through Python need not be identical.
    equal = lua.eval('function(a,b) return a == b end')
    def person(_, conversation, value):
        assert conversation == 7 and equal(value, actor)
        calls.append(('person',))
    def line(_, conversation, text, who, listener, unknown):
        assert conversation == 7 and text == 'TEXT_QST_015_BULLY_TAUNT'
        assert equal(who, speaker) and equal(listener, actor) and unknown is False
        calls.append(('line',))
    quest = lua.table_from(dict(GetThingWithScriptName=lookup, AddPersonToConversation=person,
                               AddLineToConversation=line))
    lua.execute('return function(quest,me)\n' + body + '\nend')(quest, speaker)
    assert calls == [('lookup', 'LookoutPointBeggar'), ('person',), ('line',)]
    assert not lifter.todo


@pytest.mark.parametrize('value,args,typed', [
    ('1', 'xStack_10,xStack_20', True),
    ('false', 'xStack_10,xStack_20', True),
    ('0', 'xStack_10,xStack_20,0', True),
    ('0', 'xStack_10,unknown', True),
    ('0', 'xStack_10,xStack_20', False),
])
def test_ambiguous_or_untyped_form_does_not_gain_a_cached_result(value, args, typed):
    lifter = make()
    lifter.accessor_kinds = typed
    lifter.push_temp('xStack_10', value)
    lifter.push_temp('xStack_20', '"LookoutPointBeggar"')
    lifter.interface_call(None, 'GetThingWithScriptName', args)
    assert 'xStack_10' not in lifter.slot_results
