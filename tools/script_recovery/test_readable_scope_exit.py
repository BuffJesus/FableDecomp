from pathlib import Path
import importlib.util,sys
import pytest
from lupa.lua54 import LuaRuntime
W=Path(__file__).resolve().parent;from tools.script_recovery.script_units import ROOT as R
from tools.script_recovery import readable_scope_exit as P
SOURCE='''function Main(q)
    local root <close> = q:Root()
    if q:Cond() then
        do
            local child <close> = q:Child()
            q:Use(child)
            goto done
        end
    end
    ::done::
    return q:Result()
end
'''
def run(source,kind,nested,fail=''):
 vm=LuaRuntime(unpack_returned_tuples=True)
 return vm.eval('''function(source,kind,nested,fail)
  local env=setmetatable({},{__index=_G});assert(load(source,"@guard_region","t",env))()
  local events={};local q={}
  local function event(name)
   collectgarbage("collect");events[#events+1]=name
   if name==fail then error("failure:"..name,0) end
  end
  local serial=0
  local function resource(id)
   serial=serial+1;local n=serial;events[#events+1]="created:"..id..":"..n
   return setmetatable({id=id,serial=n},{__close=function() events[#events+1]="closing:"..id..":"..n;event("close:"..id) end,__gc=function() events[#events+1]="gc:"..id..":"..n end})
  end
  function q:Root() event("root");return resource("root") end
  function q:Cond()
   event("cond")
   if kind=="nil" then return nil,"ignored" end
   if kind=="false" then return false,"ignored" end
   if kind=="zero" then return 0,"ignored" end
   if kind=="empty" then return "","ignored" end
   if kind=="object" then return {},"ignored" end
   return true,"ignored"
  end
  function q:First(r) assert(r.id=="root");event("first") end
  function q:Nested() event("nested");return nested end
  function q:Child() event("child");return resource("child") end
  function q:Use(r) assert(r.id=="child");event("use") end
  function q:Second(r) assert(r.id=="root");event("second") end
  function q:Result() event("result");return "selected",nil,42 end
  local result=table.pack(pcall(env.Main,q));collectgarbage("collect")
  return result.n,tostring(result[1]),tostring(result[2]),tostring(result[3]),tostring(result[4]),table.concat(events,",")
 end''')(source,kind,nested,fail)



@pytest.mark.parametrize('kind',['nil','false','true','zero','empty','object'])
@pytest.mark.parametrize('fail',['','root','cond','child','use','result','close:child','close:root'])
@pytest.mark.parametrize('end_target',[False,True])
def test_nested_if_do_exit_preserves_parent_child_close_errors_gc_and_returns(kind,fail,end_target):
 source=SOURCE.replace('    return q:Result()\n','') if end_target else SOURCE
 after,proof=P.fold(source);assert proof and proof['removedGotos']==1
 assert '::done::' in after and 'goto done' not in after
 assert run(source,kind,True,fail)==run(after,kind,True,fail)
 assert P.fold(after)==(after,None)

@pytest.mark.parametrize('wrapper,ending',[
 ('while q:Cond() do','end'),('for i=1,2 do','end'),('repeat','until q:Cond()'),
])
def test_loop_exit_or_repeat_condition_is_not_fallthrough(wrapper,ending):
 source='function Main(q)\n    '+wrapper+'\n        goto done\n    '+ending+'\n    ::done::\nend\n'
 assert P.fold(source)==(source,None)

def test_shadowed_label_can_target_inner_backward_jump_and_must_refuse():
 source='''function Main(q)
    do
        ::done::
        q:First()
        goto done
    end
    ::done::
end
'''
 LuaRuntime().compile(source)
 assert P.fold(source)==(source,None)

@pytest.mark.parametrize('old,new',[
 ('            goto done','            goto done -- TODO keep native evidence'),
 ('            goto done','            goto absent'),
 ('        end\n    end','        end\n        q:First(root)\n    end'),
 ('    end\n    ::done::','    else\n        q:First(root)\n    end\n    ::done::'),
 ('            q:Use(child)','            local function nested() return child end\n            q:Use(child)'),
])
def test_annotations_live_statements_missing_targets_else_and_closures_refuse(old,new):
 source=SOURCE.replace(old,new,1);assert source!=SOURCE
 assert P.fold(source)==(source,None)

def test_comments_and_all_closers_are_retained_exactly():
 source=SOURCE.replace('        end\n    end','        -- native cleanup edge\n        end -- do boundary\n    end -- if boundary')
 after,proof=P.fold(source);assert proof
 assert '-- native cleanup edge' in after and 'end -- do boundary' in after and 'end -- if boundary' in after
 assert run(source,'true',True)==run(after,'true',True)

def test_adjacent_unique_target_with_no_scope_exit_is_safe():
 source='function Main(q)\n    goto done\n    ::done::\n    return q:Result()\nend\n'
 after,proof=P.fold(source);assert proof and proof['regions'][0]['exits']==[]
 assert run(source,'false',False)==run(after,'false',False)

def test_opaque_multiline_text_refuses():
 source=SOURCE.replace('    local root <close> = q:Root()','    local text = [[\ngoto done\nend\n::done::\n]]\n    local root <close> = q:Root()')
 assert P.fold(source)==(source,None)
