"""A normalized stack byte must retain conditional writes across loop iterations."""
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.test_lift_native_lua import make


@pytest.mark.parametrize('values,expected', [([], -1), ([0], -1), ([1], 0),
    ([1, 1], 0), ([0, 1, 1], 1), ([0, 0, 1], 2)])
def test_loop_keeps_first_new_index(values, expected):
    source = '''{
xStack_18_b3 = 0;
iVar1 = -1;
iVar2 = 0;
while (iVar2 < count) {
iVar3 = GSI->GetTimer(iVar2);
if (iVar3 != 0) {
if (xStack_18_b3 == 0) {
iVar1 = iVar2;
}
xStack_18_b3 = 1;
}
iVar2 = iVar2 + 1;
}
return iVar1;
}'''
    lifter = make()
    lifter.accessor_kinds = True
    body = '\n'.join(lifter.lift('Main', source, parameters={'count': 'number'}))
    lua = LuaRuntime()
    run = lua.execute('return function(Quest,count)\n' + body + '\nend')
    assert not lifter.todo
    reads = []
    def read(_, index):
        reads.append(index)
        return values[index]
    assert run(lua.table_from({'GetTimer': read}), len(values)) == expected, body
    assert reads == list(range(len(values)))


def test_single_assignment_byte_remains_a_constant():
    lifter = make()
    lifter.accessor_kinds = True
    body = '\n'.join(lifter.lift('Main', '{\nxStack_18_b3 = 1;\nreturn xStack_18_b3;\n}'))
    assert 'return 1' in body and 'local xStack_18_b3' not in body
