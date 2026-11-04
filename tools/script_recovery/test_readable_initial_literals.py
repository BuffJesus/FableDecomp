from pathlib import Path
import importlib.util,json,sys
import pytest
from lupa.lua54 import LuaRuntime
W=Path(__file__).resolve().parent
from tools.script_recovery.script_units import ROOT as R
from tools.script_recovery import readable_initial_literals as P

@pytest.mark.parametrize('value',['false','true','0'])
@pytest.mark.parametrize('semicolon',['',';'])
def test_initial_dead_literal(value,semicolon):
    s='function Main(q)\n    local x\n    x = '+value+semicolon+'\n    q:Observe()\n    x = q:Value()\n    return x\nend\n'
    after,proof=P.prune(s)
    assert len(proof)==1 and proof[0]['hostReturnContractRequired'] is False
    assert after==s.replace('    x = '+value+semicolon+'\n','')
    LuaRuntime().compile(after)

@pytest.mark.parametrize('source',[
    'function Main(q)\n    local x\n    x = q:Resource()\n    x = false\n    x = q:Value()\nend\n',
    'function Main(q)\n    local x\n    x = false\n    q:Observe(x)\n    x = q:Value()\nend\n',
    'function Main(q)\n    local x\n    x = false\n    if q:Choose() then\n        q:Observe(x)\n    end\n    x = q:Value()\nend\n',
    'function Main(q)\n    local x\n    ::again::\n    x = false\n    x = q:Value()\n    if q:Again() then goto again end\nend\n',
    'function Main(q)\n    local x\n    local function capture()\n        return x\n    end\n    x = false\n    x = q:Value()\nend\n',
    'function Main(q)\n    local x\n    x = false -- retained annotation\n    x = q:Value()\nend\n',
    'function Main(q)\n    local x\n    x = false\n    (q:Value())()\n    x = q:Value()\nend\n',
    'function Main(q)\n    local x\n    x = false\n    q:Observe()\n    local x\n    x = q:Value()\nend\n',
    'function Main(q)\n    local x\n    x, y = false, nil\n    x = q:Value()\nend\n',
    'function Main(q)\n    local x\n    x = false\n    debug.getlocal(1,1)\n    x = q:Value()\nend\n',
    'function Main(q)\n    local _ENV = q\n    local x\n    x = false\n    x = q:Value()\nend\n',
    'function Main(q)\n    local x\n    goto done\n    x = false\n    ::done::\n    x = q:Value()\nend\n',
    'function Main(q)\n    local x\n    if q:Choose() then\n        x = q:Resource()\n    end\n    x = false\n    x = q:Value()\nend\n',
])
def test_ambiguous_live_captured_populated_or_unreachable_preserves(source):
    assert P.prune(source)==(source,[])

def test_populated_userdata_retirement_order_preserved():
    source='''function Main(q)
    local x
    x = q:Resource()
    x = false
    q:Collect()
    x = q:Value()
end
'''
    assert P.prune(source)==(source,[])
    run=LuaRuntime().eval('''function(s)
        local events={}; local env={};setmetatable(env,{__index=_G})
        assert(load(s,"retirement","t",env))()
        local q={Resource=function() return setmetatable({},{__gc=function() events[#events+1]="retired" end}) end,
                 Collect=function() collectgarbage("collect");events[#events+1]="after collect" end,
                 Value=function() return {} end}
        env.Main(q);return table.concat(events,",")
    end''')
    assert run(source)=='retired,after collect'

def test_actual_beardy_only_initial_store_removed():
    from tools.script_recovery.build_readable_unit import FUNCTION_SPLIT
    from tools.script_recovery.build_readable_unit import readable_file
    from tools.script_recovery.readable_style import state_writers
    from tools.script_recovery.readable_compound_layout import expand
    draft=R/'refs/script_recovery/lifted/BeardyBaldy/draft'
    sources={p.relative_to(draft).as_posix():p.read_text() for p in draft.rglob('*.lua')}
    rel='FSE/V_BeardyBaldy/Entities/BB_BeardyBaldyMan.lua'
    registration=json.loads((R/'refs/script_recovery/beardy_baldy/units/V_BeardyBaldy.json').read_text())
    source,_=readable_file(sources[rel],rel=rel,writers=state_writers(sources),integer_registrations=[registration],integer_manifest_bytes=(R/'refs/fse_api_manifest.json').read_bytes())
    source,_=expand(source)
    part=next(c for c in FUNCTION_SPLIT.split(source) if c.startswith('function Main('))
    after,proof=P.prune(part)
    assert len(proof)==1 and proof[0]['variable']=='scratchValue' and proof[0]['line']==5
    assert after.count('scratchValue = false')==part.count('scratchValue = false')-1
    assert proof[0]['incoming']=='uninitialized local nil on every reachable path'

@pytest.mark.parametrize('value',['nil','1','-1','0.0','"false"','{}','q:Value()'])
def test_other_initial_values_are_outside_this_pass(value):
    source='function Main(q)\n    local x\n    x = '+value+'\n    x = q:Value()\nend\n'
    assert P.prune(source)==(source,[])

def test_duplicate_nested_labels_refuse_conflated_graph():
    source='function Main(q)\n    local x\n    do\n        ::again::\n    end\n    do\n        ::again::\n    end\n    x = false\n    x = q:Value()\nend\n'
    LuaRuntime().compile(source)
    assert P.prune(source)==(source,[])

def test_host_type_and_call_failures_are_not_assumed():
    source='function Main(q)\n    local x\n    x = false\n    q:Observe()\n    x = q:Value()\n    return x\nend\n'
    candidate,proof=P.prune(source);assert len(proof)==1
    run=LuaRuntime().eval('''function(s, mode)
        local events={};local env={};setmetatable(env,{__index=_G})
        assert(load(s,"initial","t",env))()
        local resource={}
        local q={Observe=function() events[#events+1]="observe" end,
                 Value=function()
                    events[#events+1]="value"
                    if mode=="error" then error("expected host error",0) end
                    if mode=="object" then return resource end
                    if mode=="false" then return false end
                    return nil
                 end}
        local ok,v=pcall(env.Main,q)
        return tostring(ok)..":"..(v==resource and "resource" or tostring(v))..":"..table.concat(events,",")
    end''')
    for mode in ['error','object','false','nil']:assert run(source,mode)==run(candidate,mode)
