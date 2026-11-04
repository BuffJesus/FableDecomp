import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_cleanup_regions import hoist_cleanup_regions


@pytest.mark.parametrize('setup',[
    'local held = 7',
    'local held = held + 1',
    'local unrelated = 3\n    local held = 7',
    'local held, other = 7, 8',
])
@pytest.mark.parametrize('stop',[True,False])
def test_cleanup_binds_local_without_moving_initializer(setup,stop):
    source='''function Main(stop)
    local staging
    staging = false
    '''+setup+'''
    saved=function() return held end
    if stop then return end  -- TODO(native): goto LAB_00d00001
    do return end
    -- LAB_00d00001: (native jump target)
    held = nil
    return
end
'''
    after,report=hoist_cleanup_regions(source)
    assert 'LAB_00d00001' in report
    lua=LuaRuntime(unpack_returned_tuples=True)
    lua.execute('held=99\n'+after)
    lua.globals().Main(stop)
    assert lua.globals().held==99
    assert lua.globals().saved()==(None if stop else 100 if 'held + 1' in setup else 7)


@pytest.mark.parametrize('body',[
    '''if stop then return end  -- TODO(native): goto LAB_00d00001
    local held = 7
    do return end
    -- LAB_00d00001: (native jump target)
    held = nil
    return''',
    '''local held = 7
    if stop then
        local held = 9
        if stop then return end  -- TODO(native): goto LAB_00d00001
        -- LAB_00d00001: (native jump target)
        held = nil
        return
    end''',
])
def test_ambiguous_capture_retains_native_gap(body):
    source='function Main(stop)\n    '+body+'\nend\n'
    after,report=hoist_cleanup_regions(source)
    assert after==source and report=={}


def test_global_cleanup_remains_global():
    source='''function Main(stop)
    if stop then return end  -- TODO(native): goto LAB_00d00001
    do return end
    -- LAB_00d00001: (native jump target)
    held = nil
    return
end
'''
    after,report=hoist_cleanup_regions(source)
    assert report
    lua=LuaRuntime()
    lua.execute('held=99\n'+after)
    lua.globals().Main(True)
    assert lua.globals().held is None
