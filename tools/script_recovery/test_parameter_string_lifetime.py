"""Reused parameter slots must retain path-dependent runtime values."""
from lupa.lua54 import LuaRuntime
import pytest

from tools.script_recovery.test_lift_native_lua import make


@pytest.mark.parametrize('value', [15, 17, 19, 22, 23, 99])
def test_string_construction_in_returning_branch_does_not_replace_other_paths(value):
    source = '''{
if (native_arg_param_1 == 15) {
CCharString::CCharString((CCharString *)&native_arg_param_1,"$ARG1",-1);
return native_arg_param_1;
}
if (native_arg_param_1 == 17) {
return 170;
}
return native_arg_param_1;
}'''
    body='\n'.join(make().lift('Helper',source,parameters={'native_arg_param_1':'number'}))
    run=LuaRuntime().execute('return function(native_arg_param_1)\n'+body+'\nend')
    assert run(value)==('$ARG1' if value==15 else 170 if value==17 else value)


@pytest.mark.parametrize('change', [False, True])
def test_parameter_write_is_visible_only_if_branch_executes(change):
    source='''{
if (change) {
CCharString::CCharString((CCharString *)&native_arg_param_1,"replacement",-1);
}
return native_arg_param_1;
}'''
    body='\n'.join(make().lift('Helper',source,parameters={'native_arg_param_1':'string','change':'bool'}))
    run=LuaRuntime().execute('return function(native_arg_param_1,change)\n'+body+'\nend')
    assert run('incoming',change)==('replacement' if change else 'incoming')
