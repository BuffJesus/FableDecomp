import pytest
from tools.script_recovery.native_state_pointer_cleanup import remove_mapped_state_cursor
SOURCE='''function Main(quest)
    local cursor, value
    cursor = (this + 0x6c)
    repeat
        value = quest:GetStateInt("Notes_0")
        cursor = cursor + 1
    until true
    return value
end
'''
def test_proven_cursor_operations_removed_after_reads_recovered():
    result=remove_mapped_state_cursor(SOURCE,'cursor',0x6c)
    assert 'this' not in result and 'cursor =' not in result
    assert 'GetStateInt("Notes_0")' in result

@pytest.mark.parametrize('extra',['    return cursor\n','    consume(cursor)\n','    local cursor\n','    cursor = cursor + 1\n','    local function use() return cursor end\n','    value = cursor[1]\n'])
def test_unknown_cursor_use_fails_closed(extra):
    with pytest.raises(ValueError):remove_mapped_state_cursor(SOURCE.replace('    return value',extra+'    return value'),'cursor',0x6c)

@pytest.mark.parametrize('change',['base','increment','missing','reversed'])
def test_changed_cursor_shape_fails_closed(change):
    s=SOURCE
    if change=='base':s=s.replace('0x6c','0x70')
    elif change=='increment':s=s.replace('cursor + 1','cursor + 2')
    elif change=='missing':s=s.replace('    cursor = (this + 0x6c)\n','')
    else:s=s.replace('cursor = (this + 0x6c)','cursor = cursor + 1').replace('        cursor = cursor + 1','        cursor = (this + 0x6c)')
    with pytest.raises(ValueError):remove_mapped_state_cursor(s,'cursor',0x6c)

def test_opaque_cursor_names_do_not_invent_remaining_uses():
    s=SOURCE.replace('    return value','    -- cursor is native-only\n    local text = [=[cursor + 1]=]\n    return value')
    result=remove_mapped_state_cursor(s,'cursor',0x6c)
    assert '-- cursor is native-only' in result and '[=[cursor + 1]=]' in result
