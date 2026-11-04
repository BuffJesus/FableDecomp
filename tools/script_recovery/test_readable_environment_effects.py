import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.readable_style import prune_dead_termination_checks
from tools.script_recovery.build_readable_unit import readable_file

@pytest.mark.parametrize('statement',['local value = exposed','exposed = 1','local value = not exposed','local value = exposed or false','local value = exposed and 8','local value = (exposed)','local a, b = exposed, 1','local value = { exposed }'])
@pytest.mark.parametrize('stage',['prune','pipeline'])
@pytest.mark.parametrize('stop',[False,True])
@pytest.mark.parametrize('fault',[False,True])
def test_environment_effects_preserved(statement,stage,stop,fault):
    source='function Main(quest)\n    if quest:IsActiveThreadTerminating() then return end\n    '+statement+'\n    if quest:IsActiveThreadTerminating() then return end\n    return true\nend\n'
    result=''.join(prune_dead_termination_checks(source.splitlines(True),set())[0]) if stage=='prune' else readable_file(source)[0]
    def run(code):
        lua=LuaRuntime(unpack_returned_tuples=True)
        runner=lua.eval('''function(source,stop,fault)
          local stopped=false;local events={}
          local function effect(kind)
            events[#events+1]=kind
            if fault then error('environment fault') end
            stopped=stop;return 7
          end
          local env=setmetatable({}, {
            __index=function(_,k) if k=='exposed' then return effect('read') end end,
            __newindex=function(t,k,v) if k=='exposed' then effect('write') else rawset(t,k,v) end end
          })
          assert(load(source,'test','t',env))()
          local q={IsActiveThreadTerminating=function()return stopped end}
          local ok,out=pcall(env.Main,q)
          return ok,ok and tostring(out) or 'fault',table.concat(events,',')
        end''')
        return runner(code,stop,fault)
    assert run(result)==run(source)

@pytest.mark.parametrize('prefix',['','    local exposed\n','    local exposed = true\n'])
@pytest.mark.parametrize('statement',['local value = exposed','exposed = 1'])
def test_proven_local_or_parameter_keeps_redundant_check_optimization(prefix,statement):
    params='quest, exposed' if not prefix else 'quest'
    source='function Main('+params+')\n'+prefix+'    if quest:IsActiveThreadTerminating() then return end\n    '+statement+'\n    if quest:IsActiveThreadTerminating() then return end\n    return true\nend\n'
    after=''.join(prune_dead_termination_checks(source.splitlines(True),set())[0])
    assert after.count('IsActiveThreadTerminating')==1

@pytest.mark.parametrize('body',['return exposed','exposed = true','local value = exposed\n    return value'])
def test_global_access_helper_not_classified_as_pure(body):
    from tools.script_recovery.readable_style import pure_local_functions
    source='function Observe()\n    '+body+'\nend\n'
    assert 'Observe' not in pure_local_functions(source)

@pytest.mark.parametrize('value',['"exposed"','[=[exposed]=]','true','nil','7'])
def test_opaque_literals_remain_optimizable(value):
    source='function Main(quest)\n    if quest:IsActiveThreadTerminating() then return end\n    local text = '+value+'\n    if quest:IsActiveThreadTerminating() then return end\n    return text\nend\n'
    after=''.join(prune_dead_termination_checks(source.splitlines(True),set())[0])
    assert after.count('IsActiveThreadTerminating')==1
