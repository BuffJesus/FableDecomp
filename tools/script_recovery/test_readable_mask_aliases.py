import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.readable_style import prune_dead_bitmask_staging
from tools.script_recovery.build_readable_unit import readable_file
SOURCE='''function Main(quest)
    local flags, tail
    flags = 31
    if (flags & 8) ~= 0 then
        flags = flags & 0xfffffff7
    end
    if (flags & 2) ~= 0 then
        tail = flags & 0xfffffffd
    end
    return true
end
'''
@pytest.mark.parametrize('stage',['mask','pipeline'])
def test_private_integer_tail_alias_can_be_removed(stage):
    text=SOURCE.replace('flags','scratchValue6').replace('tail','scratchValue7')
    result=''.join(prune_dead_bitmask_staging(text.splitlines(True))[0]) if stage=='mask' else readable_file(text)[0]
    assert 'scratchValue' not in result
    assert LuaRuntime().execute(result+'return Main({})') is True

@pytest.mark.parametrize('change',['global','read','capture','earlier_store','later_decl','closed_decl','shadow','environment','effectful_operand'])
def test_observable_or_ambiguous_tail_alias_preserved(change):
    text=SOURCE
    if change=='global':text=text.replace('local flags, tail','local flags')
    elif change=='read':text=text.replace('return true','return tail')
    elif change=='capture':text=text.replace('    flags = 31','    local function observe() return tail end\n    flags = 31')
    elif change=='earlier_store':text=text.replace('    flags = 31','    tail = 99\n    flags = 31')
    elif change=='later_decl':text=text.replace('local flags, tail','local flags').replace('    return true','    local tail\n    return true')
    elif change=='closed_decl':text=text.replace('local flags, tail','local flags\n    do\n        local tail\n    end')
    elif change=='shadow':text=text.replace('local flags, tail','local flags, tail\n    local tail')
    elif change=='environment':text=text.replace('tail','_ENV')
    elif change=='effectful_operand':text=text.replace('tail = flags &','tail = value &')
    result,count=prune_dead_bitmask_staging(text.splitlines(True))
    assert ''.join(result)==text and count==0
