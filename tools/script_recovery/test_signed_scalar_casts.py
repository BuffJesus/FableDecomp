"""Explicit x86 signed casts must not erase high-bit sentinel semantics."""
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.test_lift_native_lua import make


@pytest.mark.parametrize('bits,signed', [(0xffffffff, -1), (0xfffffffe, -2),
    (0x80000000, -2147483648), (0x7fffffff, 2147483647), (0, 0)])
@pytest.mark.parametrize('replace', [False, True])
def test_signed_read_preserves_raw_equality_and_unsigned_reads(bits, signed, replace):
    source = '''{
iVar1 = (CCharString)BITS;
if (replace) {
iVar1 = 7;
}
GSI->SetTimer(1,(int)iVar1);
GSI->SetTimer(2,(uint)iVar1);
if (iVar1 == BITS) {
GSI->SetTimer(3,1);
}
return (int)iVar1 < 0;
}'''.replace('BITS', hex(bits) if bits else '0')
    lifter = make()
    lifter.accessor_kinds = True
    body = '\n'.join(lifter.lift('Main', source, parameters={'replace': 'bool'}))
    lua, writes = LuaRuntime(), []
    run = lua.execute('return function(Quest,replace)\n' + body + '\nend')
    result = run(lua.table_from({'SetTimer': lambda _, key, value: writes.append((key,value))}), replace)
    expected = 7 if replace else signed
    assert result == (expected < 0), body
    assert writes == [(1, expected), (2, 7 if replace else bits)] + ([] if replace else [(3,1)]), body
    assert not lifter.todo
