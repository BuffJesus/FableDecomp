import pytest
from tools.script_recovery.lua_terminal_cleanup_alias import prove_staging_before_terminal_cleanup as prove
BASE='function Main(log, stop)\n    local value, other\n    local function cleanup()\n        log("release")\n    end\n    local function finish()\n        log("finish")\n    end\n    value = log("earlier")\n    log(value)\n    if stop then\n        value = nil -- marker\n        cleanup(); return\n    end\n    log(value)\n    finish()\n    return\nend\n'
def run(source=BASE,alias='value',cleanup='cleanup'):
 return prove(source,source.index('value = nil'),alias,cleanup)
def test_terminal_cleanup_does_not_observe_other_alias_lifetimes():
 p=run();assert p['closureAliasCaptures']==0 and p['straightLineClosures']==2 and p['terminalCalls']==2
 assert p['liveness']['copiedAliases']==[]
def test_direct_call_then_return_is_supported():
 assert run(BASE.replace('cleanup(); return','cleanup()\n        return'))
def test_opaque_alias_and_cleanup_text_is_not_a_capture():
 assert run(BASE.replace('log("release")','log("value cleanup") -- value cleanup'))
def test_inline_terminal_other_call_is_modeled():
 assert run(BASE.replace('    log(value)\n    finish()','    if stop then finish(); return end\n    log(value)\n    finish()'))
@pytest.mark.parametrize('old,new',[
 ('log("release")','log(value)'),('log("release")','value = nil'),
 ('log("release")','finish()'),('log("release")','cleanup()'),
 ('log("release")','if stop then log("release") end'),
 ('log("release")','local x = 1'),('log("release")','return'),
 ('local function cleanup()','local function cleanup(value)'),
 ('local function finish()','local function cleanup()'),
 ('cleanup(); return','cleanup(value); return'),
 ('cleanup(); return','cleanup(); log(value); return'),
 ('cleanup(); return','other = cleanup; return'),
 ('cleanup(); return','cleanup = finish; return'),
 ('cleanup(); return','finish(); return'),
 ('cleanup(); return','log(value)\n        cleanup(); return'),
 ('    finish()\n    return','    finish()\n    log(value)\n    return'),
 ('    finish()\n    return','    log(finish)\n    return'),
 ('    finish()\n    return','    finish = log\n    return'),
 ('cleanup(); return','if value then cleanup(); return end'),
 ('        value = nil -- marker','        value = nil; log(value) -- marker'),
 ('    log(value)\n    finish()','    goto target\n    ::target::\n    log(value)\n    finish()\n    local function hidden() return value end'),
])
def test_capture_nonterminal_calls_and_unknown_controls_refuse(old,new):
 with pytest.raises(ValueError):run(BASE.replace(old,new))
@pytest.mark.parametrize('offset',[True,-1,100000])
def test_invalid_assignment_coordinates_refuse(offset):
 with pytest.raises(ValueError):prove(BASE,offset,'value','cleanup')
