"""Only closed integer-only staging can disappear with an unused local mask."""
import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.readable_style import prune_dead_bitmask_staging
from tools.script_recovery.build_readable_unit import readable_file


SOURCE='''function Main(quest)
    local flags
    quest:Mark("before")
    flags = 31
    if (31 & 16) ~= 0 then
        flags = 31 & 0xffffffef
    end
    if (flags & 8) ~= 0 then
        flags = flags & 0xfffffff7
    end
    if (flags & 4) ~= 0 then
        flags = flags & 0xfffffffb
    end
    if (flags & 2) ~= 0 then
        flags = flags & 0xfffffffd
    end
    quest:Mark("after")
    return false
end
'''


def transform(source):
    lines,count=prune_dead_bitmask_staging(source.splitlines(True))
    return ''.join(lines),count


def run(source):
    return LuaRuntime(unpack_returned_tuples=True).execute(source+'''
local trace={}
local q={Mark=function(_,value) trace[#trace+1]=value end}
return Main(q),table.concat(trace,',')
''')


@pytest.mark.parametrize('initial',['0','31','4294967295','0xffffffff'])
def test_unused_private_integer_chain(initial):
    source=SOURCE.replace('flags = 31\n','flags = '+initial+'\n')
    result,count=transform(source)
    assert count==5 and 'flags' not in result
    assert run(result)==run(source)==(False,'before,after')


@pytest.mark.parametrize('change',[
    'global','read','capture','shadow','float','string','call','large','label','comment','close','foreign','effect',
])
def test_ambiguous_or_observable_group_is_preserved(change):
    source=SOURCE
    if change=='global':source=source.replace('    local flags\n','')
    elif change=='read':source=source.replace('return false','return flags')
    elif change=='capture':source=source.replace('    flags = 31\n','    local function observe() return flags end\n    flags = 31\n')
    elif change=='shadow':source=source.replace('    flags = 31\n','    do local flags end\n    local flags\n    flags = 31\n')
    elif change=='float':source=source.replace('flags = 31\n','flags = 31.5\n')
    elif change=='string':source=source.replace('flags = 31\n','flags = "31"\n')
    elif change=='call':source=source.replace('flags = 31\n','flags = quest:ReadFlags()\n')
    elif change=='large':source=source.replace('flags = 31\n','flags = 0x100000000\n')
    elif change=='label':source=source.replace('    if (flags & 8)', '    ::entry::\n    if (flags & 8)')
    elif change=='comment':source=source.replace('    if (flags & 8)', '    -- preserve native cleanup evidence\n    if (flags & 8)')
    elif change=='close':source=source.replace('local flags','local flags <close>')
    elif change=='foreign':source=source.replace('flags = flags & 0xfffffff7','flags = other & 0xfffffff7')
    elif change=='effect':source=source.replace('flags = flags & 0xfffffff7','quest:ReleaseResource(flags)')
    result,count=transform(source)
    assert result==source and count==0


def test_live_mask_retains_integer_result():
    source=SOURCE.replace('return false','return flags')
    result,count=transform(source)
    assert count==0 and run(result)==(1,'before,after')


def test_full_pipeline_removes_only_private_staging():
    source=SOURCE.replace('flags','scratchValue6')
    result,_=readable_file(source)
    assert 'scratchValue6' not in result
    assert run(result)==run(source)


@pytest.mark.parametrize('declaration', ['later', 'closed', 'sibling', 'comment'])
def test_out_of_scope_declaration_cannot_hide_global_store(declaration):
    source=SOURCE.replace('    local flags\n','')
    if declaration=='later':
        source=source.replace('    return false','    local flags\n    return false')
    elif declaration=='closed':
        source=source.replace('    flags = 31','    do\n    local flags\n    end\n    flags = 31',1)
    elif declaration=='sibling':
        source=source.replace('    flags = 31','    if false then\n    local flags\n    else\n    flags = 31',1).replace('    return false','    end\n    return false')
    else:
        source=source.replace('    flags = 31','    --[[\n    local flags\n    ]]\n    flags = 31',1)
    result,count=transform(source)
    assert count==0 and result==source
    assert LuaRuntime().execute(result+'Main({Mark=function() end}); return flags')==1


@pytest.mark.parametrize('opener,closer', [('[[',']]'),('[=[',']=]')])
def test_mask_shaped_multiline_string_is_not_code(opener,closer):
    source=('function Main()\n    local flags\n    return '+opener+'\n'
            '    flags = 31\n    if (flags & 8) ~= 0 then\n'
            '        flags = flags & 0xfffffff7\n    end\n'+closer+'\nend\n')
    result,count=transform(source)
    assert result==source and count==0
    assert LuaRuntime().execute(result+'return Main()')==LuaRuntime().execute(source+'return Main()')


def test_environment_local_has_implicit_uses():
    source=('function Main()\n    local _ENV\n    _ENV = 31\n'
            '    if (_ENV & 8) ~= 0 then\n        _ENV = _ENV & 0xfffffff7\n'
            '    end\n    return missingGlobal\nend\n')
    result,count=transform(source)
    assert result==source and count==0
    for text in (source,result):
        with pytest.raises(Exception,match='attempt to index a number value'):
            LuaRuntime().execute(text+'return Main()')
