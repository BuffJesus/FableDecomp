import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.readable_label_alias import fold
from tools.script_recovery.readable_style import prune_unused_generated_labels

SOURCE='function Main(q)\n    local root <close> = q:Root()\n    do\n        local child <close> = q:Child()\n        q:Use(child)\n        local choice = q:Route()\n        if choice == 1 then goto LAB_a end\n        if choice == 2 then goto FLOW_b end\n        goto LAB_c\n        ::LAB_a::\n        ::FLOW_b::\n        ::LAB_c::\n    end\n    return q:Result()\nend\n'

def observed(source,route,failure):
 vm=LuaRuntime(unpack_returned_tuples=True)
 return vm.eval('function(source,route,fail)\n  local env=setmetatable({},{__index=_G});assert(load(source,"@alias_test","t",env))()\n  local events={};local serial=0;local turns=0;local q={}\n  local function event(name)\n    collectgarbage("collect");events[#events+1]=name\n    if name==fail then error("failure:"..name,0) end\n  end\n  local function own(id)\n    serial=serial+1;local n=serial;events[#events+1]="created:"..id..":"..n\n    return setmetatable({id=id,serial=n},{__close=function(self,err)\n      events[#events+1]="closing:"..id..":"..n..":"..tostring(err);event("close:"..id)\n    end,__gc=function() events[#events+1]="gc:"..id..":"..n end})\n  end\n  function q:Root() event("root");return own("root") end\n  function q:Child() event("child");return own("child") end\n  function q:Use(r) assert(r.id=="child");event("use") end\n  function q:Route() event("route");return route,"ignored" end\n  function q:Loop() event("loop");turns=turns+1;return turns<=3 end\n  function q:Done(r) assert(r.id=="child");event("done");turns=turns+1;return turns>=3 end\n  function q:Values()\n    event("values");local function next_value(state,control) event("next");control=control+1;if control<=3 then return control end end\n    return next_value,{},0,own("iterator")\n  end\n  function q:Result() event("result");return "selected",nil,42 end\n  local result=table.pack(pcall(env.Main,q));collectgarbage("collect")\n  return result.n,tostring(result[1]),tostring(result[2]),tostring(result[3]),tostring(result[4]),table.concat(events,",")\nend')(source,route,failure)

def bytecode(source):
 vm=LuaRuntime(encoding=None)
 return vm.eval(b'function(s) local f,e=load(s,"@alias_proof","t",{}); assert(f,e); return string.dump(f,true) end')(source.encode())

@pytest.mark.parametrize('route',[None,False,0,1,2,3,'text'])
@pytest.mark.parametrize('failure',['','root','child','use','route','result','close:child','close:root'])
@pytest.mark.parametrize('end_target',[False,True])
def test_alias_retarget_and_pruning_preserve_gc_close_errors_and_return_arity(route,failure,end_target):
 source=SOURCE.replace('    return q:Result()\n','') if end_target else SOURCE
 middle,proof=fold(source);assert proof['retargetedGotos']==2 and len(proof['aliases'])==2
 assert middle.count('::')==source.count('::') and bytecode(source)==bytecode(middle)
 after,count=prune_unused_generated_labels(middle);assert count==2
 assert observed(source,route,failure)==observed(middle,route,failure)==observed(after,route,failure)

LOOP_BODY='        local child <close> = q:Child()\n        q:Use(child)\n        local choice = q:Route()\n        if choice == 1 then goto LAB_a end\n        if choice == 2 then goto FLOW_b end\n        goto LAB_c\n        ::LAB_a::\n        ::FLOW_b::\n        ::LAB_c::\n'
@pytest.mark.parametrize('kind',['while','numeric_for','generic_for','repeat','backward'])
@pytest.mark.parametrize('route',[1,2,3])
@pytest.mark.parametrize('failure',['','use','route','loop','done','close:child','close:iterator','result'])
def test_loop_condition_iterator_closing_and_backward_targets_preserve_traces(kind,route,failure):
 if kind=='while':body='    while q:Loop() do\n'+LOOP_BODY+'    end\n'
 elif kind=='numeric_for':body='    for i = 1,3 do\n'+LOOP_BODY+'    end\n'
 elif kind=='generic_for':body='    for i in q:Values() do\n'+LOOP_BODY+'    end\n'
 elif kind=='repeat':body='    repeat\n'+LOOP_BODY+'    until q:Done(child)\n'
 else:body='    local child <close> = q:Child()\n    ::LAB_a::\n    ::FLOW_b::\n    ::LAB_c::\n    q:Use(child)\n    if q:Loop() then\n        local choice = q:Route()\n        if choice == 1 then goto LAB_a end\n        if choice == 2 then goto FLOW_b end\n        goto LAB_c\n    end\n'
 source='function Main(q)\n    local root <close> = q:Root()\n'+body+'    return q:Result()\nend\n'
 middle,proof=fold(source);assert proof['retargetedGotos']==2 and bytecode(source)==bytecode(middle)
 after,count=prune_unused_generated_labels(middle);assert count==2
 assert observed(source,route,failure)==observed(middle,route,failure)==observed(after,route,failure)

@pytest.mark.parametrize('separator',[
 '        q:Use(child)\n','        local extra = 1\n','        -- native diagnostic boundary\n',
 '        do end\n','        ;\n',
])
def test_executable_declarations_diagnostics_and_nonwhitespace_break_the_alias_group(separator):
 # Only two labels here,so the single boundary blocks all aliasing.
 source=SOURCE.replace('        if choice == 2 then goto FLOW_b end\n','').replace('        ::FLOW_b::\n','').replace('        ::LAB_c::',''+separator+'        ::LAB_c::')
 assert fold(source)==(source,None)

def test_shadowed_canonical_label_cannot_capture_a_retargeted_outer_jump():
 source='function Main(q)\n    do\n        ::LAB_a::\n        if q:Loop() then goto LAB_b end\n    end\n    goto LAB_a\n    ::LAB_a::\n    ::LAB_b::\n    return q:Result()\nend\n'
 LuaRuntime().compile(source);assert fold(source)==(source,None)

@pytest.mark.parametrize('replacement',[
 '        ::UserLabel::','        ::FLOW_b:: -- diagnostic',
])
def test_user_or_diagnostic_labels_are_retained_as_boundaries(replacement):
 source=SOURCE.replace('        ::FLOW_b::',replacement)
 if 'UserLabel' in replacement:source=source.replace('goto FLOW_b','goto UserLabel')
 LuaRuntime().compile(source)
 # A valid live group cannot cross either protected line.
 assert fold(source)==(source,None)

@pytest.mark.parametrize('text',[
 '    q:Use([=[goto LAB_c\n::LAB_a::]=])\n',
 '    --[=[goto LAB_c\n::LAB_a::]=]\n',
 '    local function capture() return 1 end\n',
 '    q:Use(_ENV)\n',
])
def test_opaque_or_environment_sensitive_functions_refuse(text):
 source=SOURCE.replace('    return q:Result()',text+'    return q:Result()');assert fold(source)==(source,None)

def test_cross_line_goto_comments_are_retained_while_only_the_target_changes():
 source=SOURCE.replace('goto LAB_c','goto -- keep this exact text\n            LAB_c')
 middle,proof=fold(source);assert proof['retargetedGotos']==2 and 'goto -- keep this exact text\n            LAB_a' in middle
 assert bytecode(source)==bytecode(middle)

def test_label_looking_string_literals_are_untouched():
 source=SOURCE.replace('        q:Use(child)','        q:Use(child)\n        local text = "goto LAB_c ::FLOW_b::"')
 middle,proof=fold(source);assert proof and '"goto LAB_c ::FLOW_b::"' in middle and bytecode(source)==bytecode(middle)

def test_crlf_line_endings_are_retained():
 source=SOURCE.replace('\n','\r\n');middle,proof=fold(source);assert proof and middle.count('\r\n')==source.count('\r\n') and bytecode(source)==bytecode(middle)
