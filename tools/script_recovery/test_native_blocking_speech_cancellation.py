import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_blocking_speech_cancellation import recover

MANIFEST={'Speak':{'scope':'Entity','returnType':'bool','blocking':True}}
SOURCE='''function Main(quest, me)
    local alive,busy,flag,cancel
    me:Speak(hero,"LINE",0,false,true,false)
    busy = me:IsPerformingScriptTask()
    flag = busy
    while flag do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        cancel = not alive
        if cancel then
            quest:Cleanup()
            do return end
        end
        busy = me:IsPerformingScriptTask()
        flag = busy
    end
end
'''

@pytest.mark.parametrize('spoken,task',[(False,False),(False,True),(True,False),(True,True)])
def test_native_abort_exit_runs_before_any_duplicate_frame(spoken,task):
 out,evidence=recover(SOURCE,MANIFEST);assert len(evidence)==1
 lua=LuaRuntime();calls=[];pending=[task,False]
 me=lua.table_from({'Speak':lambda _,*args:spoken,'IsPerformingScriptTask':lambda _:pending.pop(0)})
 q=lua.table_from({'Cleanup':lambda _:calls.append('cleanup'),'NewScriptFrame':lambda _,actor:(calls.append('frame') or True),'IsActiveThreadTerminating':lambda _:False})
 lua.execute(out);lua.globals().Main(q,me)
 assert calls==(['cleanup'] if not spoken else ['frame'] if task else [])

@pytest.mark.parametrize('change',[
 ('me:IsPerformingScriptTask()','other:IsPerformingScriptTask()'),
 ('    busy = me:IsPerformingScriptTask()','    quest:OtherWork()\n    busy = me:IsPerformingScriptTask()'),
 ('    while flag do','    while not flag do'),
 ('        alive = quest:NewScriptFrame(me)','        quest:OtherWork()\n        alive = quest:NewScriptFrame(me)'),
 ('alive = not quest:IsActiveThreadTerminating()','alive = quest:OtherPredicate()'),
 ('cancel = not alive','cancel = alive'),
 ('            do return end','            quest:Continue()'),
 ('            quest:Cleanup()','            -- TODO(native): unresolved cleanup'),
 ('            quest:Cleanup()','            local value = 3'),
])
def test_ambiguous_wait_or_exit_keeps_original(change):
 source=SOURCE.replace(*change);assert recover(source,MANIFEST)==(source,[])

@pytest.mark.parametrize('spec',[
 {'scope':'Entity','returnType':'bool','blocking':False},
 {'scope':'Entity','returnType':'bool'},
 {'scope':'Entity','returnType':'void','blocking':True},
 {'scope':'Quest','returnType':'bool','blocking':True},
])
def test_missing_binding_contract_keeps_original(spec):
 assert recover(SOURCE,{'Speak':spec})==(SOURCE,[])

def test_assigned_speech_result_and_inline_exit_are_retained():
 source=SOURCE.replace('    me:Speak(', '    result = me:Speak(').replace('        if cancel then\n            quest:Cleanup()\n            do return end\n        end','        if cancel then quest:Cleanup(); return end')
 out,evidence=recover(source,MANIFEST);assert len(evidence)==1
 assert 'result = me:Speak(' in out and 'if not result then\n        quest:Cleanup(); return\n    end' in out
