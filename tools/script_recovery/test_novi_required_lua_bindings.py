"""A self-consistent payload still needs the binding used by generated Lua."""
import hashlib,json
import pytest
from tools.script_recovery import novi_local_test as launcher

@pytest.mark.parametrize('has_binding',[False,True])
@pytest.mark.parametrize('source,required',[
    ('resources:ResourceThingHealthAbove(r,0)',True),
    ('local f = resources.ResourceThingHealthAbove',True),
    ('resources:\n ResourceThingHealthAbove(r,0)',True),
    ('--[=[ignore]=] resources:ResourceThingHealthAbove(r,0)',True),
    ('-- ResourceThingHealthAbove\nreturn true',False),
    ('--[==[ ResourceThingHealthAbove ]==]\nreturn true',False),
    ('local s="ResourceThingHealthAbove"',False),
    ("local s='ResourceThingHealthAbove'",False),
    ('local s=[=[ResourceThingHealthAbove]=]',False),
    ('local s="escaped\\\" ResourceThingHealthAbove"',False)])
def test_executable_identifiers_require_exact_binding_while_literals_and_comments_do_not(tmp_path,source,required,has_binding):
    (tmp_path/'a.lua').write_text(source)
    (tmp_path/'NoviCompatibility.dll').write_bytes(b'NoviCompatibilityStart\0'+(b'ResourceThingHealthAbove\0' if has_binding else b''))
    manifest={'files':{'a.lua':'not-used-by-binding-check'}}
    if required and not has_binding:
        with pytest.raises(ValueError,match='required Lua binding'):launcher.validate_lua_bindings(tmp_path,manifest)
    else:launcher.validate_lua_bindings(tmp_path,manifest)

def test_unlisted_file_does_not_add_a_binding_requirement(tmp_path):
    (tmp_path/'a.lua').write_text('return true')
    (tmp_path/'unlisted.lua').write_text('resources:ResourceThingHealthAbove(r,0)')
    launcher.validate_lua_bindings(tmp_path,{'files':{'a.lua':'unused'}})

def test_longer_binding_name_does_not_satisfy_exact_literal(tmp_path):
    (tmp_path/'a.lua').write_text('resources:ResourceThingHealthAbove(r,0)')
    (tmp_path/'NoviCompatibility.dll').write_bytes(b'ResourceThingHealthAboveWrong\0')
    with pytest.raises(ValueError,match='required Lua binding'):
        launcher.validate_lua_bindings(tmp_path,{'files':{'a.lua':'unused'}})

@pytest.mark.parametrize('has_binding',[False,True])
def test_full_matching_manifest_is_checked_and_validation_is_read_only(tmp_path,monkeypatch,has_binding):
    bundle=tmp_path/'bundle';bundle.mkdir();game=tmp_path/'game';(game/'data/Levels').mkdir(parents=True)
    (game/'Fable.exe').write_bytes(b'offline executable fixture')
    (game/'data/Levels/FinalAlbion.qst').write_text('// no quest activation\n')
    (bundle/'a.lua').write_text('resources:ResourceThingHealthAbove(r,0)')
    (bundle/'NoviCompatibility.dll').write_bytes(b'NoviCompatibilityStart\0'+(b'ResourceThingHealthAbove\0' if has_binding else b''))
    sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
    manifest={'files':{n:sha(bundle/n) for n in ['a.lua','NoviCompatibility.dll']},'game_exe_sha256':sha(game/'Fable.exe')}
    (bundle/'manifest.json').write_text(json.dumps(manifest))
    monkeypatch.setattr(launcher,'validate_dll_roles',lambda bundle:None)
    before={str(p):sha(p) for p in tmp_path.rglob('*') if p.is_file()}
    if has_binding:assert launcher.validate(bundle,game)==manifest
    else:
        with pytest.raises(ValueError,match='required Lua binding'):launcher.validate(bundle,game)
    assert before=={str(p):sha(p) for p in tmp_path.rglob('*') if p.is_file()}
