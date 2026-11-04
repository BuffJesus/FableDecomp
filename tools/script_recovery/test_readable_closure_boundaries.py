"""Retained callback values and lexical boundaries in cleanup-closure pruning."""
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.readable_style import prune_unused_closures
from tools.script_recovery.build_readable_unit import readable_file


def execute(source):
    lua=LuaRuntime(unpack_returned_tuples=True)
    return lua.execute('trace={}\n'+source+'''
local result=table.pack(pcall(Main))
if result[1] and type(result[2])=='function' then result=table.pack(pcall(result[2])) end
local out={tostring(result.n)}
for i=1,result.n do out[#out+1]=type(result[i])..':'..tostring(result[i]) end
return table.concat(out,'|'),table.concat(trace,',')
''')


@pytest.mark.parametrize('use',[
    'return callback',
    'return callback ()',
    'return callback\n        ()',
    'local handlers={callback}\n    return handlers[1]()',
    'local alias=callback\n    return alias()',
    'local handlers={run=callback}\n    return handlers.run()',
    'sink=callback\n    return sink()',
    'return (callback)()',
    'return callback --[[call]] ()',
    'if callback then return callback end',
])
@pytest.mark.parametrize('pipeline',[False,True])
def test_callback_value_and_call_references(use,pipeline):
    source='''function Main()
    local function callback()
        trace[#trace+1]='called'
        return 7,nil,9
    end
    '''+use+'\nend\n'
    after=readable_file(source)[0] if pipeline else ''.join(prune_unused_closures(source.splitlines(True))[0])
    assert execute(after)==execute(source)


@pytest.mark.parametrize('body',[
    "local function callback() trace[#trace+1]='called' end\n    if callback() then\n    end\n    return 7",
    'local function unused() return 1 end\n    if true then\n        trace[#trace+1]=\'live\'\n    end\n    return 7',
    'local function callback() return 7 end\n    return callback',
])
@pytest.mark.parametrize('pipeline',[False,True])
def test_inline_closure_cannot_consume_following_block(body,pipeline):
    source='function Main()\n    '+body+'\nend\n'
    after=readable_file(source)[0] if pipeline else ''.join(prune_unused_closures(source.splitlines(True))[0])
    assert execute(after)==execute(source)


@pytest.mark.parametrize('literal',[
    '[=[\n    local function imaginary()\n        return 99\n    end\n]=]',
    '[=[\n    end\n    local function callback()\n]=]',
])
def test_multiline_text_is_opaque(literal):
    source='function Main()\n    return '+literal+'\nend\n'
    after,count=prune_unused_closures(source.splitlines(True))
    assert ''.join(after)==source and count==0


def test_empty_line_slots_keep_original_offsets():
    lines=['function Main()\n','','    local function unused()\n','        return 9\n','    end\n','','    return 7\n','end\n']
    after,count=prune_unused_closures(lines.copy())
    assert count==1
    assert execute(''.join(after))==execute(''.join(lines))


def test_unused_multiline_cleanup_can_be_removed():
    source='''function Main()
    local function cleanup()
        trace[#trace+1]='never'
        return 7
    end
    return 9
end
'''
    after,count=prune_unused_closures(source.splitlines(True))
    assert count==1 and 'local function cleanup' not in ''.join(after)
    assert execute(''.join(after))==execute(source)


def test_self_recursive_unused_closure_can_be_removed():
    source='''function Main()
    local function callback()
        return callback()
    end
    return 7
end
'''
    after,count=prune_unused_closures(source.splitlines(True))
    assert count==1 and execute(''.join(after))==execute(source)


@pytest.mark.parametrize('body',[
    "error('callback-failure')",
    "local owned <close> = setmetatable({}, {__close=function() trace[#trace+1]='closed' end})\n        return 7",
    "local owned <close> = setmetatable({}, {__close=function() trace[#trace+1]='closed' end})\n        error('callback-failure')",
])
def test_stored_callback_preserves_errors_and_close_handlers(body):
    source='''function Main()
    local function callback()
        BODY
    end
    local handlers={callback}
    return handlers[1]()
end
'''.replace('BODY',body)
    for after in (''.join(prune_unused_closures(source.splitlines(True))[0]),readable_file(source)[0]):
        # Error source line numbers may change; retain error text and close order.
        expected=execute(source);actual=execute(after)
        if 'callback-failure' in expected[0]:
            assert 'callback-failure' in actual[0] and actual[1]==expected[1]
        else:assert actual==expected


def test_nested_callback_capture_is_a_use():
    source='''function Main()
    local function callback()
        return 7
    end
    local function invoke()
        return callback
    end
    return invoke()()
end
'''
    for after in (''.join(prune_unused_closures(source.splitlines(True))[0]),readable_file(source)[0]):
        assert execute(after)==execute(source)


def test_text_mentions_do_not_keep_unused_closure():
    source='''function Main()
    local function unused()
        return 99
    end
    -- unused()
    return 'unused'
end
'''
    after,count=prune_unused_closures(source.splitlines(True))
    assert count==1 and execute(''.join(after))==execute(source)
