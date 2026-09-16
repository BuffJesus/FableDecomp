import itertools,unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.teddy_girl_candidate import generate

FIXTURE='''function fixture(question, talked, hit, stop, fault, cancel)
    local events, live = {}, {}
    local q, r = {}, {}
    local frames, queries = 0, 0
    local function log(value) events[#events+1]=value end
    local function new(kind) live[#live+1]=kind; log("new:"..kind); return #live end
    local function phase(name) log(name); if fault==name then error("FAULT "..name,0) end; return stop~=name end
    function q:RegisterBoundConsciousCondition() log("condition") end
    function q:NewScriptFrame() frames=frames+1; log("frame") end
    function q:IsActiveThreadTerminating() queries=queries+1; return queries>=cancel or frames>=2 end
    function q:WithRetailResources(body)
        local ok, err = pcall(body,r)
        for i=#live,1,-1 do if live[i] then log("destroy:"..live[i]) end end
        if not ok then error(err,0) end
    end
    function r:NewResource() return new("control") end
    function r:NewThingFromScriptName(name) assert(name=="NOVI_Bully"); return new("bully") end
    function r:NewPresentedItemOutput(me) assert(me==7); return new("output") end
    function r:DestroyPresentedItemOutput(id) assert(live[id]=="output"); live[id]=false; log("destroy:output") end
    function r:TeddyGirlTalkedWithTeddy(me) assert(me==7); log("talkTeddy"); return question end
    function r:IsTalkedToByHero(me) assert(me==7); log("talkPredicate"); return talked end
    function r:IsHitByHeroExceptAbility(me, ability) assert(me==7 and ability==14); log("hitPredicate"); return hit end
    function r:PrepareResource(control) assert(control==1); log("prepare") end
    TeddyGirlAcquire=function() return phase("acquire") end
    TeddyGirlWithMovie=function(_,_,body) log("movie"); local value=body(); log("movie.end"); return value end
    TeddyGirlQuestion=function() return phase("question") end
    TeddyGirlPresentedKind=function() log("presentedKind"); return "none" end
    TeddyGirlPresentedResponse=function() return phase("presented") end
    TeddyGirlDeparture=function(_,_,me,control,bully) assert(me==7 and control==1 and bully==2); return phase("departure") end
    TeddyGirlTalk=function() return phase("talk") end
    TeddyGirlHit=function() return phase("hit") end
    local ok,err=pcall(Main,q,7)
    return ok, tostring(err), table.concat(events,",")
end
'''

class TeddyGirlCandidateTests(unittest.TestCase):
    def test_composed_dispatch_cancellation_and_reverse_outer_ownership(self):
        source,report=generate();self.assertFalse(report['enabled']);self.assertEqual(source.count('quest:RegisterBoundConsciousCondition()'),1)
        for question,talked,hit,stop,fault in itertools.product((False,True),(False,True),(False,True),(None,'question','presented','departure','talk','hit'),(None,'departure')):
            with self.subTest(question=question,talked=talked,hit=hit,stop=stop,fault=fault):
                lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(source+FIXTURE)
                ok,error,trace=lua.globals().fixture(question,talked,hit,stop,fault,999);events=trace.split(',')
                self.assertEqual(events[:5],['condition','frame','new:control','new:bully','new:output'])
                self.assertEqual(events[-2:],['destroy:bully','destroy:control'])
                self.assertEqual(events.count('destroy:output'),1)
                self.assertEqual('presentedKind' in events,not question)
                if not ok:self.assertIn('FAULT departure',error)
                if 'prepare' in events:self.assertEqual(events[events.index('prepare'):events.index('prepare')+3],['prepare','destroy:output','frame'])
                if 'talkPredicate' in events:self.assertLess(events.index('departure'),events.index('talkPredicate'))
                if 'hitPredicate' in events:self.assertLess(events.index('talkPredicate'),events.index('hitPredicate'))
        for cancel,expected in ((1,'condition,frame'),(2,'condition,frame,new:control,new:bully,destroy:bully,destroy:control')):
            lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(source+FIXTURE)
            self.assertEqual(lua.globals().fixture(False,False,False,None,None,cancel)[2],expected)

if __name__=='__main__':unittest.main()
