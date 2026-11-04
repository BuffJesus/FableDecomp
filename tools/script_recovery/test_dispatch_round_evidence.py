from lupa.lua54 import LuaRuntime

from tools.script_recovery.build_readable_unit import readable_file


SOURCE='''function Main(quest)
    local discarded
    -- TODO(native): CVar12 = *(this + 4)
    discarded = nil --[[unresolved native value]]
    if false then
        quest:Unused(discarded)
    end
    return 17
end
'''


def test_second_round_recovery_keeps_native_proof():
    output,report=readable_file(SOURCE,rel='Test/Entities/Actor.lua')
    entry=report['style']['functions']['Main']
    assert entry['rewrites']['unusedNativeDispatchSeeds']==1
    assert len(entry['nativeDispatchSeeds'])==1
    assert entry['nativeDispatchSeeds'][0]['nativeLocal']=='CVar12'
    assert '-- TODO(native)' not in output
    lua=LuaRuntime()
    for source in (SOURCE,output):
        lua.execute(source)
        assert lua.globals().Main(lua.table())==17


def test_missing_file_role_keeps_unresolved_origin():
    output,report=readable_file(SOURCE)
    assert '-- TODO(native): CVar12 = *(this + 4)' in output
    assert 'nativeDispatchSeeds' not in report['style']['functions']['Main']


def test_disabled_style_keeps_the_faithful_marker():
    output,report=readable_file(SOURCE,style=False,rel='Test/Entities/Actor.lua')
    assert '-- TODO(native): CVar12 = *(this + 4)' in output
    assert report['style']=={}
