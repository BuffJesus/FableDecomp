"""Transfer keys must survive exactly, even when not valid Lua identifiers."""
from types import SimpleNamespace
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import lift_persist, persist_local_name
from tools.script_recovery.convert_quest_unit import lift_persist_evidence


@pytest.mark.parametrize('name', ['ClientInUse[0]', 'ClientInUse[1]', 'end', 'Quest', 'Context', '42', 'SwordTaken'])
@pytest.mark.parametrize('typed', [True, False])
def test_transfer_uses_exact_key_without_shadowing_context(name, typed):
    receiver = 'this' if typed else 'param_1'
    source = f'CPersistContext::Transfer<int>(param_2,"{name}",(int *)({receiver} + 0x48),(int *)0);\n'
    if typed:
        spec = SimpleNamespace(call_labels={'CPersistContext::Transfer<int>': 1}, resolve_string=None)
        lines, _, _ = lift_persist_evidence(source, {'quest': {'fields': {}}}, spec, {1: 'Int'})
    else:
        lines, _, _ = lift_persist(source, 'quest')
    assert lines
    lua = LuaRuntime()
    events = []
    context = lua.table_from({'token': 'same-context'})
    def transfer(_q, ctx, key, value):
        assert ctx['token'] == 'same-context'
        events.append((key, value))
        return 11
    quest = lua.table_from({
        'GetStateInt': lambda _q, key: 7,
        'PersistTransferInt': transfer,
        'SetStateInt': lambda _q, key, value: events.append((key, value)),
    })
    fn = lua.execute('return function(quest, context)\n'+'\n'.join(lines)+'\nend')
    fn(quest, context)
    assert events == [(name, 7), (name, 11)]
    assert persist_local_name(name) not in ('quest', 'context', 'end')
