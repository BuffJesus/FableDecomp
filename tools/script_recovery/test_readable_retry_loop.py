from pathlib import Path
import importlib.util,json,re,sys
import pytest
from lupa.lua54 import LuaRuntime
W=Path(__file__).resolve().parent
from tools.script_recovery.script_units import ROOT as R
from tools.script_recovery import readable_retry_loop as P
SOURCE=(W/'testdata/retry_loop_two_history.lua').read_text()

def test_exact_shape_and_graph_proof():
    after,proof=P.restructure(SOURCE)
    assert proof and len(proof['unreachableRemoved'])==5
    assert 'goto ' not in after and '::' not in after
    assert 'while not predicateResult do' in after
    assert '(quest:GetStateString("RandomSpeech_" .. randomChoice))' in after
    assert after.count('quest:NewScriptFrame(me)')==1
    assert after.count('quest:IsActiveThreadTerminating()')==4
    LuaRuntime().compile(after)
    assert P.restructure(after)==(after,None)

@pytest.mark.parametrize('old,new',[
    ('lastRandomSpeechIdx2 = lastRandomSpeechIdx1','lastRandomSpeechIdx1 = lastRandomSpeechIdx2'),
    ('lastRandomSpeechIdx1 = randomChoice','lastRandomSpeechIdx1 = 0'),
    ('RetailRandModulo(10)','GetRandomThing(10)'),
    ('quest:NewScriptFrame(me);','quest:NewScriptFrame(other);'),
    ('goto continue_1','goto FLOW_after_lab_00e53d52'),
    ('until false','until true'),
    ('local randomChoice','randomChoice'),
    ('local predicateResult','predicateResult'),
    ('do return "" end;','do return "different" end;'),
    ('predicateResult = quest:IsActiveThreadTerminating();','predicateResult = quest:IsActiveThreadTerminating(); q:Observe()'),
    ('quest:NewScriptFrame(me)\n','quest:Other(me)\n'),
    ('repeat\n','repeat\n        local extra = quest:Extra()\n'),
    ('local randomChoice','local predicateResult'),
    ('lastRandomSpeechIdx2)) then','lastRandomSpeechIdx1)) then'),
    ('return ""\n','return nil\n'),
    ('function GetRandomSpeech(quest, me)','function GetRandomSpeech(quest, me, lastRandomSpeechIdx1)'),
    ('::continue_1::','::FLOW_after_lab_00e53d52::'),
])
def test_changed_calls_state_scope_edges_or_default_refuse(old,new):
    assert old in SOURCE
    source=SOURCE.replace(old,new,1)
    assert P.restructure(source)==(source,None)

@pytest.mark.parametrize('hazard',sorted(P.HAZARDS))
def test_environment_and_introspection_refuse(hazard):
    source=SOURCE.replace('    repeat\n','    '+hazard+'()\n    repeat\n')
    assert P.restructure(source)==(source,None)

def test_comments_and_unknown_cleanup_refuse():
    for addition in ['        -- TODO keep native evidence\n','        resource:Release()\n','        local function captured() return randomChoice end\n']:
        source=SOURCE.replace('        local randomChoice',addition+'        local randomChoice',1)
        assert P.restructure(source)==(source,None)

def test_names_are_captured_generically():
    source=SOURCE
    for old,new in [('GetRandomSpeech','ChooseLine'),('quest','host'),('me','actor'),('predicateResult','cancelled'),('randomChoice','choice'),('lastRandomSpeechIdx1','recent'),('lastRandomSpeechIdx2','previous')]:
        source=re.sub(r'\b'+re.escape(old)+r'\b',new,source)
    after,proof=P.restructure(source)
    assert proof and proof['function']=='function ChooseLine(host, actor)'
    assert proof['history']==['recent','previous']
    assert 'goto ' not in after
    LuaRuntime().compile(after)

def runner(source):
    vm=LuaRuntime(unpack_returned_tuples=True)
    run=vm.eval('''function(source, mode, terminateAt, frameLimit)
        local events,checks,frames,index={},0,0,0
        local factory=assert(load("return function(first,second) local lastRandomSpeechIdx1,lastRandomSpeechIdx2=first,second;"..source.."; return GetRandomSpeech,function() return lastRandomSpeechIdx1,lastRandomSpeechIdx2 end end","@retry","t",_G))()
        local fn,peek=factory(1,2)
        local q={}
        function q:IsActiveThreadTerminating()
            checks=checks+1;events[#events+1]="check:"..checks
            if mode=="throw-term" and checks==2 then error("fixture term error",0) end
            if checks>=terminateAt or frames>frameLimit then return true end
            return false
        end
        function q:RetailRandModulo(n)
            assert(n==10);index=index+1;events[#events+1]="random:"..index
            if mode=="throw-rand" then error("fixture random error",0) end
            if index<3 then return index end
            return 3
        end
        function q:NewScriptFrame(actor)
            frames=frames+1;events[#events+1]="frame"
            if mode=="throw-frame" then error("fixture frame error",0) end
            return false
        end
        function q:GetStateString(key)
            local a,b=peek();events[#events+1]="string:"..key..":"..a..":"..b
            if mode=="throw-string" then error("fixture string error",0) end
            if mode=="multi" then return "first","second",71 end
            if mode=="nil" then return nil,"second" end
            if mode=="false" then return false,"second" end
            return "text:"..key
        end
        local results=table.pack(pcall(fn,q,{}))
        local a,b=peek()
        local parts={tostring(results.n),tostring(results[1]),tostring(results[2]),tostring(a),tostring(b),table.concat(events,",")}
        return table.concat(parts,"|")
    end''')
    return lambda mode,check,frame:run(source,mode,check,frame)

@pytest.mark.parametrize('mode',['normal','multi','nil','false','throw-term','throw-rand','throw-frame','throw-string'])
@pytest.mark.parametrize('cancel',[1,2,3,4,5,6,20])
def test_calls_state_errors_and_exact_return_arity(mode,cancel):
    after,proof=P.restructure(SOURCE);assert proof
    before_run,after_run=runner(SOURCE),runner(after)
    for frame in [0,1,2,8]:assert before_run(mode,cancel,frame)==after_run(mode,cancel,frame)
