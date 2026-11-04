from pathlib import Path
import importlib.util,sys
import pytest
from lupa.lua54 import LuaRuntime
W=Path(__file__).resolve().parent;from tools.script_recovery.script_units import ROOT as R
from tools.script_recovery import readable_guard_region as P
SOURCE='''function Main(q)
    local root <close> = q:Root()
    if q:Cond() then goto finish end
    q:First(root)
    if q:Nested() then
        local child <close> = q:Child()
        q:Use(child)
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
  local function resource(id)
   return setmetatable({id=id},{__close=function() events[#events+1]="close:"..id end,__gc=function() events[#events+1]="gc:"..id end})
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
@pytest.mark.parametrize('fail',['','root','cond','first','nested','child','use','second','result'])
def test_truthiness_call_errors_roots_close_order_and_return_arity(kind,nested,fail):
 after,proof=P.fold(SOURCE);assert proof and proof['removedGotos']==1
 assert '::finish::' in after
 assert [x.strip() for x in SOURCE.splitlines() if x.strip().startswith('local ')]==[x.strip() for x in after.splitlines() if x.strip().startswith('local ')]
 assert run(SOURCE,kind,nested,fail)==run(after,kind,nested,fail)
 assert P.fold(after)==(after,None)

@pytest.mark.parametrize('body',[
 '    local x = q:Value()\n', '    local x <close> = q:Value()\n',
 '    ::incoming::\n    q:Body()\n','    if q:Other() then\n        ::incoming::\n    end\n',
 '    return\n','    else\n','    end\n','    until false\n',
 '    while q:Other() do\n    ::finish::\n    end\n',
])
def test_outer_declarations_labels_scope_boundaries_or_invalid_return_refuse(body):
 source='function Main(q)\n    if q:Cond() then goto finish end\n'+body+'    ::finish::\nend\n'
 assert P.fold(source)==(source,None)

@pytest.mark.parametrize('replacement',[
 'if q:Cond() then goto absent end','if q:Cond() then goto finish end -- TODO preserve',
 'if q:Cond() then q:Effect(); goto finish end',
])
def test_missing_target_or_annotated_multi_statement_condition_refuses(replacement):
 source=SOURCE.replace('if q:Cond() then goto finish end',replacement)
 assert P.fold(source)==(source,None)

def test_multiline_opaque_text_refuses():
 source=SOURCE.replace('    local root <close> = q:Root()','    local text = [[\n    if q:Cond() then goto finish end\n    q:Body()\n    ::finish::\n    ]]\n    local root <close> = q:Root()')
 assert P.fold(source)==(source,None)

def test_one_line_literal_control_text_is_not_parsed_as_syntax():
 source=SOURCE.replace('q:Cond()', 'q:Cond(" then goto bogus end")')
 after,proof=P.fold(source);assert proof
 assert 'q:Cond(" then goto bogus end")' in after
 assert run(source,'false',True)==run(after,'false',True)

def test_nested_do_repeat_and_for_scopes_stay_nested():
 source=SOURCE.replace('    q:First(root)\n','    do\n        local a = 1\n        q:First(root)\n    end\n    for i=1,2 do q:Second(root) end\n    repeat q:Second(root) until true\n')
 after,proof=P.fold(source);assert proof
 assert run(source,'false',True)==run(after,'false',True)

LOOPS=[
'''function Main(q)
    local root <close> = q:Root()
    for i=1,2 do
        if q:Cond() then goto skip end
        if q:Nested() then break end
        q:First(root)
        ::skip::
        q:Second(root)
    end
    return q:Result()
end
''',
'''function Main(q)
    local root <close> = q:Root()
    repeat
        if q:Cond() then goto skip end
        q:First(root)
        ::skip::
        q:Second(root)
        break
    until false
    return q:Result()
end
''',
'''function Main(q)
    local root <close> = q:Root()
    local tries = 0
    ::again::
    tries = tries + 1
    if q:Cond() then goto skip end
    q:First(root)
    if tries < 2 then goto again end
    ::skip::
    q:Second(root)
    return q:Result()
end
''']

@pytest.mark.parametrize('source',LOOPS)
@pytest.mark.parametrize('kind',['false','nil','true','zero'])
@pytest.mark.parametrize('nested',[False,True])
@pytest.mark.parametrize('fail',['','first','second','result'])
def test_enclosing_loop_break_repeat_and_backward_goto_keep_control_and_cleanup(source,kind,nested,fail):
 after,proof=P.fold(source);assert proof
 assert run(source,kind,nested,fail)==run(after,kind,nested,fail)
