from pathlib import Path
import importlib.util,sys
import pytest
from lupa.lua54 import LuaRuntime
W=Path(__file__).resolve().parent;from tools.script_recovery.script_units import ROOT as R
from tools.script_recovery import readable_empty_then as P
SOURCE='''function Main(q)
    local root <close> = q:Root()
    if CONDITION then
    else
        local child <close> = q:Child()
        q:Use(child)
        q:First(root)
        q:Second(root)
    end
    return q:Result()
end
'''
CONDITIONS=['q:Cond()','not q:Cond()','not (q:Cond())','not not q:Cond()','not q:Cond() and q:Nested()','q:Cond() or q:Nested()']
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
   if kind=="object" then return setmetatable({},{__gc=function() events[#events+1]="condition_gc" end}),"ignored" end
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



@pytest.mark.parametrize('condition',CONDITIONS)
@pytest.mark.parametrize('kind',['nil','false','true','zero','empty','object'])
@pytest.mark.parametrize('nested',[False,True])
@pytest.mark.parametrize('fail',['','root','cond','child','use','first','second','result','close:child','close:root'])
def test_truthiness_short_circuit_finalizable_condition_and_branch_scope(condition,kind,nested,fail):
 source=SOURCE.replace('CONDITION',condition);after,proof=P.fold(source);assert proof and proof['normalizedGuards']==1
 assert '    else' not in after
 assert [l.strip() for l in source.splitlines() if l.strip().startswith('local ')]==[l.strip() for l in after.splitlines() if l.strip().startswith('local ')]
 assert run(source,kind,nested,fail)==run(after,kind,nested,fail)
 assert P.fold(after)==(after,None)

@pytest.mark.parametrize('condition',CONDITIONS)
@pytest.mark.parametrize('kind',['false','true','object'])
@pytest.mark.parametrize('fail',['','close:child','close:root'])
def test_parent_close_at_function_end_keeps_temporary_condition_lifetime(condition,kind,fail):
 source=SOURCE.replace('CONDITION',condition).replace('    return q:Result()\n','')
 after,proof=P.fold(source);assert proof
 assert run(source,kind,True,fail)==run(after,kind,True,fail)

@pytest.mark.parametrize('condition,cancelled',[
 ('not q:Cond()',True),('not (q:Cond())',True),('not not q:Cond()',True),
 ('not q:Cond() and q:Nested()',False),('not q:Cond() or q:Nested()',False),
 ('not q:Cond() == false',False),('q:Cond()',False),
])
def test_only_whole_unary_not_is_cancelled(condition,cancelled):
 source=SOURCE.replace('CONDITION',condition);after,proof=P.fold(source);assert proof
 assert proof['regions'][0]['cancelledWholeUnaryNot'] is cancelled
 for kind in ['false','true']:assert run(source,kind,True)==run(after,kind,True)

@pytest.mark.parametrize('old,new',[
 ('    else','        -- TODO retain empty-then evidence\n    else'),
 ('    else','        q:First(root)\n    else'),
 ('    else','    else -- native evidence'),
 ('    if CONDITION then','    if CONDITION then -- native condition'),
 ('        local child','        local function nested() return 1 end\n        local child'),
])
def test_nonempty_then_comments_and_closures_refuse(old,new):
 source=SOURCE.replace(old,new,1).replace('CONDITION','q:Cond()')
 assert P.fold(source)==(source,None)

def test_labels_and_nested_body_scopes_remain_inside_same_branch():
 source=SOURCE.replace('CONDITION','q:Cond()').replace('        q:First(root)','        for i=1,2 do q:First(root) end\n        repeat q:Second(root) until true\n        ::inside::')
 after,proof=P.fold(source);assert proof and '::inside::' in after
 for kind in ['false','true']:assert run(source,kind,True)==run(after,kind,True)

def test_multiline_opaque_text_refuses():
 source=SOURCE.replace('CONDITION','q:Cond()').replace('    local root <close> = q:Root()','    local text = [[\nif x then\nelse\nend\n]]\n    local root <close> = q:Root()')
 assert P.fold(source)==(source,None)
