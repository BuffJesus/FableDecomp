import pytest
from tools.script_recovery.lua_terminal_cleanup_alias import prove_terminal_cleanup_alias

BASE='''function Main(log, stop)
    local held, other
    local function cleanup()
        held = nil
        log("release")
    end
    held = log("another lifetime")
    log(held)
    if stop then cleanup(); return end
    cleanup()
    return
end
'''

def test_alias_other_lifetimes_do_not_make_terminal_cleanup_observable():
    result=prove_terminal_cleanup_alias(BASE,'held','cleanup')
    assert len(result['terminalCallLines'])==2 and result['nilWritesInClosure']==1

@pytest.mark.parametrize('old,new',[
 ('        held = nil','        log(held)'),
 ('        held = nil','        held = nil\n        log(held)'),
 ('        held = nil','        if stop then held = nil end'),
 ('    cleanup()\n    return','    cleanup()\n    log(held)\n    return'),
 ('    cleanup()\n    return','    other = cleanup\n    return'),
 ('    cleanup()\n    return','    cleanup = other\n    return'),
 ('    cleanup()\n    return','    cleanup(held)\n    return'),
 ('    if stop then cleanup(); return end','    if stop then cleanup(); log(held) end'),
 ('    held = log("another lifetime")','    local function capture()\n        log(held)\n    end\n    held = log("another lifetime")'),
 ('    local function cleanup()','    local function cleanup(held)'),
 ('        log("release")','        return held'),
 ('        log("release")','        cleanup()'),
 ('    if stop then cleanup(); return end','    if cleanup() then cleanup(); return end'),
 ('    held = log("another lifetime")','    local function escape()\n        log(cleanup)\n    end\n    held = log("another lifetime")'),
])
def test_observable_or_escaping_cleanup_rejects(old,new):
    with pytest.raises(ValueError):prove_terminal_cleanup_alias(BASE.replace(old,new),'held','cleanup')

def test_opaque_closure_name_is_not_an_escape():
    assert prove_terminal_cleanup_alias(BASE.replace('    held = log','    log("cleanup") -- cleanup\n    held = log'),'held','cleanup')
