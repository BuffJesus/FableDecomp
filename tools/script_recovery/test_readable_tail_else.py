from pathlib import Path
import importlib.util,sys
import pytest
from lupa.lua54 import LuaRuntime
W=Path(__file__).resolve().parent;from tools.script_recovery.script_units import ROOT as R
from tools.script_recovery import readable_tail_else as P
SOURCE='''function Main(q)
    local root <close> = q:Root()
    if q:Cond() then
        local child <close> = q:Child()
        q:Use(child)
        goto finish
    end
    q:First(root)
    if q:Nested() then
        local another <close> = q:Child()
        q:Use(another)
    end
    q:Second(root)
    ::finish::
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
@pytest.mark.parametrize('nested',[False,True])
@pytest.mark.parametrize('fail',['','root','cond','child','use','first','nested','second','result','close:child','close:root'])
def test_condition_truthiness_then_scope_close_failures_roots_and_return_arity(kind,nested,fail):
 after,proof=P.fold(SOURCE);assert proof and proof['removedGotos']==1
 assert 'else' in after and '::finish::' in after
 assert [l.strip() for l in SOURCE.splitlines() if l.strip().startswith('local ')]==[l.strip() for l in after.splitlines() if l.strip().startswith('local ')]
 assert run(SOURCE,kind,nested,fail)==run(after,kind,nested,fail)
 assert P.fold(after)==(after,None)

@pytest.mark.parametrize('kind',['nil','false','true','zero','empty','object'])
@pytest.mark.parametrize('fail',['','close:child','close:root'])
def test_end_of_block_target_closes_parent_and_then_local_in_same_order(kind,fail):
 source=SOURCE.replace('    return q:Result()\n','')
 after,proof=P.fold(source);assert proof
 assert run(source,kind,False,fail)==run(after,kind,False,fail)

def test_then_local_shadow_does_not_leak_into_else():
 source=SOURCE.replace('local child <close> = q:Child()','local root <close> = q:Child()').replace('q:Use(child)','q:Use(root)')
 after,proof=P.fold(source);assert proof
 for kind in ['false','true']:
  assert run(source,kind,True)==run(after,kind,True)

@pytest.mark.parametrize('old,new',[
 ('        goto finish','        goto absent'),
 ('        goto finish','        goto finish -- TODO preserve diagnostic'),
 ('    end\n    q:First(root)','    end -- native evidence\n    q:First(root)'),
 ('        q:Use(child)','        ::inner::\n        q:Use(child)'),
 ('    q:First(root)','    local outer = q:Root()\n    q:First(root)'),
 ('    q:First(root)','    ::incoming::\n    q:First(root)'),
 ('        q:Use(child)','        return\n        q:Use(child)'),
 ('    end\n    q:First(root)','    else\n        q:First(root)\n    end\n    q:First(root)'),
 ('        goto finish','        goto finish; q:First(root)'),
 ('        q:Use(child)','        local function nested() return child end\n        q:Use(child)'),
])
def test_labels_outer_declarations_existing_else_annotations_or_unknown_tail_refuse(old,new):
 source=SOURCE.replace(old,new,1);assert source!=SOURCE
 assert P.fold(source)==(source,None)

def test_multiline_control_text_refuses():
 source=SOURCE.replace('    local root <close> = q:Root()','    local text = [[\n    if q:Cond() then\n        goto finish\n    end\n    q:Body()\n    ::finish::\n    ]]\n    local root <close> = q:Root()')
 assert P.fold(source)==(source,None)

def test_single_line_condition_literal_and_then_comments_stay_exact():
 source=SOURCE.replace('q:Cond()', 'q:Cond(" then goto bogus end")').replace('        goto finish','        -- native ownership annotation\n        goto finish')
 after,proof=P.fold(source);assert proof
 assert 'q:Cond(" then goto bogus end")' in after and '-- native ownership annotation' in after
 assert run(source,'false',True)==run(after,'false',True)

@pytest.mark.parametrize('body',[
 '        for i=1,2 do q:Use(child) end\n',
 '        while true do q:Use(child); break end\n',
 '        repeat\n            local grand <close> = q:Child()\n            q:Use(grand)\n        until true\n',
])
@pytest.mark.parametrize('kind',['false','true'])
@pytest.mark.parametrize('fail',['','use','close:child','close:root'])
def test_nested_then_loops_and_repeat_close_boundaries(body,kind,fail):
 source=SOURCE.replace('        q:Use(child)\n',body,1)
 after,proof=P.fold(source);assert proof
 assert run(source,kind,True,fail)==run(after,kind,True,fail)
