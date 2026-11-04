import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.smoke_run_unit import HARNESS


def run(body):
    lua = LuaRuntime(unpack_returned_tuples=True)
    runner = lua.execute('return function(SOURCE, NAME, FUNCTIONS, FRAME_BUDGET, INSTRUCTION_LIMIT, PARAM_KINDS, ENTITY_FILE, DEFINITION_FIXTURE) ' + HARNESS + ' end')
    return runner('function Main(quest)\n'+body+'\nend', 'strings.lua', lua.table('Main'), 2, 100,
                  lua.table(), False, None)['Main']


@pytest.mark.parametrize('call', ['ReadGlobalGameDataString(3608)',
                                 'ReadGlobalGameDataStringAt(0x4d4,0)',
                                 'ReadGlobalGameDataDefStringAt(0x74c,0)'])
def test_definition_string_return_shape(call):
    result = run('local value=quest:'+call+'; assert(type(value)=="string"); assert(#value==0)')
    assert result['ok'], result['error']


def test_numeric_defaults_keep_their_types_and_values():
    result = run('assert(quest:ReadGlobalGameData(0x168)==1); assert(quest:ReadGlobalGameDataFloatAt(0xeb4,0)==0)')
    assert result['ok'], result['error']


def test_unknown_similarly_named_method_is_not_inferred_to_return_string():
    result = run('assert(type(quest:ReadGlobalGameDataStringTypo(0))=="number")')
    assert result['ok'], result['error']
    assert 'quest:ReadGlobalGameDataStringTypo' in list(result['trace'].values())
