from pathlib import Path
import importlib.util,sys
import pytest
from lupa.lua54 import LuaRuntime
W=Path(__file__).resolve().parent;from tools.script_recovery.script_units import ROOT as R
from tools.script_recovery import readable_unreachable_goto_tail as P
SOURCE='''function Main(q)
    do
        local resource <close> = q:Resource()
        goto finish
        q:Dead(resource)
        return
    end
    ::finish::
    return q:Result()
end
'''

def run(source,throw_resource=False,throw_result=False):
 vm=LuaRuntime(unpack_returned_tuples=True)
 return vm.eval('''function(source,throwResource,throwResult)
  local events={};local env=setmetatable({},{__index=_G})
  assert(load(source,"@goto_tail","t",env))()
  local q={}
  function q:Resource()
   events[#events+1]="new"
   if throwResource then error("resource failure",0) end
   return setmetatable({},{__close=function() events[#events+1]="close" end,__gc=function() events[#events+1]="gc" end})
  end
  function q:Dead() error("unreachable body executed",0) end
  function q:Result()
   collectgarbage("collect");events[#events+1]="result"
   if throwResult then error("result failure",0) end
   return "selected",nil,42
  end
  local result=table.pack(pcall(env.Main,q))
  collectgarbage("collect")
  return result.n,tostring(result[1]),tostring(result[2]),tostring(result[3]),tostring(result[4]),table.concat(events,",")
 end''')(source,throw_resource,throw_result)

@pytest.mark.parametrize('throw_resource',[False,True])
@pytest.mark.parametrize('throw_result',[False,True])
def test_to_be_closed_resource_root_return_arity_and_errors(throw_resource,throw_result):
 after,proof=P.prune(SOURCE)
 assert proof and len(proof['removed'])==2
 assert 'local resource <close>' in after and '::finish::' in after
 assert run(SOURCE,throw_resource,throw_result)==run(after,throw_resource,throw_result)
 assert P.prune(after)==(after,None)

@pytest.mark.parametrize('leaf',[
 'q:Dead(resource)','q.Dead(resource)','q.deep.Dead(resource)','q.deep:Dead(resource)',
 'q:Dead()','q:Dead(resource, true, false, nil, -12, 0, "plain", resource.field)',
 'return',
])
def test_only_modeled_adjacent_leaves_removed(leaf):
 source='function F(q)\n do\n goto out\n '+leaf+'\n end\n ::out::\n ::another::\nend\n'
 after,proof=P.prune(source);assert proof and len(proof['removed'])==1
 assert leaf not in after
 LuaRuntime().compile(source);LuaRuntime().compile(after)

@pytest.mark.parametrize('boundary',[
 'local x = q:Value()', 'x = q:Value()', 'if q:Value() then end', 'do q:Value() end',
 'while q:Value() do end','repeat q:Value() until true','for i=1,2 do q:Value() end',
 '::incoming::','-- preserve diagnostic','q:Unknown(q:Value())','q:Unknown({x=1})',
 'q:Unknown(resource[1])',r'q:Unknown("\\q")','q:Unknown(1 2)','q:Unknown(1 +)',
 'q:Unknown(); q:Value()','q:Unknown() q:Value()', 'q:Unknown() -- native evidence',
 'q:Unknown(١٢)','q:Unknown(ressourcé)','q:Déád(resource)',
])
def test_unknown_syntax_declarations_labels_annotations_and_controls_stop(boundary):
 source='function F(q)\n goto out\n '+boundary+'\n q:Reachable()\n ::out::\nend\n'
 assert P.prune(source)==(source,None)

@pytest.mark.parametrize('string',[
 '[[goto finish\nq:Dead()\nreturn]]','[=[goto finish\nq:Dead()\nreturn]=]',
 '"goto finish\\\nq:Dead()\\\nreturn"',
])
def test_multiline_literals_refuse(string):
 source='function F(q)\n local text = '+string+'\n goto out\n q:Dead()\n ::out::\nend\n'
 assert P.prune(source)==(source,None)

def test_long_comment_refuses():
 source=SOURCE.replace('        goto finish','        --[[goto finish\nq:Dead()\nreturn]]\n        goto finish')
 assert P.prune(source)==(source,None)

def test_conditional_goto_does_not_start_a_tail():
 source='function F(q)\n if q:Cond() then goto out end\n q:Reachable()\n ::out::\nend\n'
 assert P.prune(source)==(source,None)

def test_invalid_return_before_label_is_not_silently_repaired():
 source='function F(q)\n goto out\n return\n ::out::\nend\n'
 assert P.prune(source)==(source,None)

def test_unreachable_jump_and_unresolved_target_diagnostic_are_preserved():
 source='function F(q)\n goto out\n goto missing\n ::out::\nend\n'
 assert P.prune(source)==(source,None)

def test_label_entry_is_preserved():
 source='''function Main(q)
    if q:Cond() then goto incoming end
    goto finish
    q:Dead()
    ::incoming::
    q:Reachable()
    ::finish::
    return q:Result()
end
'''
 after,proof=P.prune(source);assert proof
 assert 'q:Dead()' not in after and 'q:Reachable()' in after
 for value in [False,True]:
  vm=LuaRuntime();f=vm.eval('''function(source,value)
   local env=setmetatable({},{__index=_G});assert(load(source,"@label","t",env))()
   local calls=0;local q={Cond=function() return value end,Reachable=function() calls=calls+1 end,Result=function() return 42 end,Dead=function() error("dead") end}
   assert(env.Main(q)==42);return calls
  end''')
  assert f(source,value)==f(after,value)==int(value)
