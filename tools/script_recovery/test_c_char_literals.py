"""C character constants are numbers; quoted text is not a numeric operand."""
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_c_char_literals import lower_c_byte_literals
from tools.script_recovery.test_lift_native_lua import make


@pytest.mark.parametrize('literal,value', [(r"'\0'", 0), (r"'\x00'", 0),
    (r"'\x01'", 1), (r"'\x04'", 4), (r"'\x4'", 4), (r"'\x7F'", 127)])
def test_character_comparison_executes_as_integer(literal, value):
    lifter = make()
    body = '\n'.join(lifter.lift('Main', '{\nreturn count == ' + literal + ';\n}', parameters={'count': 'number'}))
    lua = LuaRuntime()
    run = lua.execute('return function(Quest,count)\n' + body + '\nend')
    assert run(lua.table(), value) is True, body
    assert run(lua.table(), value+1) is False, body
    assert not lifter.todo


@pytest.mark.parametrize('text', [r'''"literal '\x04' and '\0'"''',
    r'''"escaped quote \" '\x04'"''', r"'\x80'", r"'\xff'", r"'\x100'",
    r"L'\x04'", r"u'\x04'", r"'\\x04'", r"'AB'"])
def test_quoted_text_and_unsupported_character_forms_are_preserved(text):
    assert lower_c_byte_literals(text) == text


def test_numeric_operand_next_to_a_string_is_lowered_without_changing_string():
    assert lower_c_byte_literals(r'''call("'\x04'", '\x04')''') == r'''call("'\x04'", 4)'''
