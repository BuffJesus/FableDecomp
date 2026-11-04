from pathlib import Path
import hashlib,importlib.util,json
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import readable_scope_exit as S
from tools.script_recovery.readable_style import prune_unused_generated_labels
BASE='function Main(log, own, stop, entry)\n    do\n        local a <close> = own("a")\n        do\n            local b <close> = own("b")\n            if stop then\n                local c <close> = own("c")\n                goto LAB_done\n                ::LAB_middle::\n            end\n            ::LAB_second::\n        end\n        ::LAB_third::\n    end\n    ::LAB_done::\n    log("after")\n    return 17\nend\n'
HELPERS='events = {}\nfunction log(value) events[#events+1] = value end\nfunction own(name)\n    return setmetatable({}, {__close=function(self, err)\n        log("close:" .. name .. ":" .. tostring(err))\n        if fail == name then error("close-error:" .. name, 0) end\n    end})\nend\n'
def fold(source=BASE):return S.fold(source,allow_intervening_labels=True)
def observed(source,stop,entry,failure):
 vm=LuaRuntime(unpack_returned_tuples=True);vm.execute(HELPERS);vm.globals().fail=failure;vm.execute(source)
 result=vm.eval('function(stop, entry) return pcall(Main, log, own, stop, entry) end')(stop,entry)
 events=vm.globals().events;return result,[events[i] for i in range(1,len(events)+1)]
@pytest.mark.parametrize('stop',[False,True,0,1,None])
@pytest.mark.parametrize('failure',[None,'a','b','c'])
def test_close_order_and_errors_match_across_intervening_labels(stop,failure):
 out,proof=fold();assert proof['removedGotos']==1
 assert observed(BASE,stop,False,failure)==observed(out,stop,False,failure)
 pruned,count=prune_unused_generated_labels(out);assert count==4
 assert observed(BASE,stop,False,failure)==observed(pruned,stop,False,failure)

def test_every_physical_line_except_jump_is_retained():
 out,proof=fold();old=BASE.splitlines(True);removed=proof['regions'][0]['line']-1
 assert out==''.join(old[:removed]+old[removed+1:])
 assert [r['label'] for r in proof['regions'][0]['interveningLabelsRetained']]==['LAB_middle','LAB_second','LAB_third']
 assert [r['kind'] for r in proof['regions'][0]['exits']]==['if','do','do']

def test_default_conservative_api_stays_exact():
 assert S.fold(BASE)==(BASE,None)

ENTRY='function Main(log, own, stop, entry)\n    if entry then goto LAB_middle end\n    goto LAB_done\n    ::LAB_middle::\n    ::LAB_done::\n    log("after")\n    return 23\nend\n'
@pytest.mark.parametrize('entry',[False,True,0,None])
def test_another_jump_to_an_intervening_label_is_retained(entry):
 out,proof=fold(ENTRY);assert proof['removedGotos']==1
 pruned,count=prune_unused_generated_labels(out);assert count==1 and '::LAB_middle::' in pruned
 assert observed(ENTRY,False,entry,None)==observed(out,False,entry,None)==observed(pruned,False,entry,None)

@pytest.mark.parametrize('interposed',[
 '    log("side-effect")\n','    local x = 1\n','    return 3\n',
 '    if stop then log("branch") end\n','    goto LAB_elsewhere\n',
 '    while stop do\n    end\n','    repeat\n    until stop\n',
])
def test_labels_do_not_hide_intervening_executable_or_declaration(interposed):
 source='function Main(log, own, stop, entry)\n    goto LAB_done\n    ::LAB_middle::\n'+interposed+'    ::LAB_done::\n    return 1\nend\n'
 out,proof=fold(source)
 assert out==source and proof is None

@pytest.mark.parametrize('loop',[
 'while stop do','for i = 1, 3 do','for i in values() do',
])
def test_loop_exits_remain_gotos(loop):
 source='function Main(log, own, stop, entry)\n    '+loop+'\n        goto LAB_done\n        ::LAB_middle::\n    end\n    ::LAB_done::\n    return 1\nend\n'
 assert fold(source)==(source,None)

def test_duplicate_target_names_refuse_even_with_separate_scopes():
 source=BASE.replace('    log("after")','    do\n        ::LAB_done::\n    end\n    log("after")')
 assert fold(source)==(source,None)

@pytest.mark.parametrize('opaque',[
 '    log([=[goto LAB_done\n::LAB_middle::]=])\n',
 '    --[=[goto LAB_done\n::LAB_middle::]=]\n',
 '    local function capture() return 1 end\n',
 '    log(_ENV)\n',
])
def test_opaque_text_closures_and_environment_introspection_refuse(opaque):
 source=BASE.replace('    log("after")',opaque+'    log("after")')
 assert fold(source)==(source,None)

def test_labels_inside_else_arm_do_not_make_branch_transition_transparent():
 source=BASE.replace('                ::LAB_middle::','                ::LAB_middle::\n            else\n                log("else")')
 assert fold(source)==(source,None)

from tools.script_recovery.test_readable_scope_exit import SOURCE as GC_SOURCE,run as gc_observed
@pytest.mark.parametrize('kind',['nil','false','true','zero','empty','object'])
@pytest.mark.parametrize('failure',['','root','cond','child','use','result','close:child','close:root'])
@pytest.mark.parametrize('end_target',[False,True])
def test_intervening_label_then_pruning_preserves_gc_errors_arity_and_closing(kind,failure,end_target):
 source=GC_SOURCE.replace('done','FLOW_target').replace('            goto FLOW_target\n','            goto FLOW_target\n            ::FLOW_intermediate::\n')
 if end_target:source=source.replace('    return q:Result()\n','')
 assert S.fold(source)==(source,None)
 middle,proof=fold(source);assert proof['removedGotos']==1 and proof['regions'][0]['interveningLabelsRetained'][0]['label']=='FLOW_intermediate'
 after,count=prune_unused_generated_labels(middle);assert count==2
 assert gc_observed(source,kind,True,failure)==gc_observed(middle,kind,True,failure)==gc_observed(after,kind,True,failure)
