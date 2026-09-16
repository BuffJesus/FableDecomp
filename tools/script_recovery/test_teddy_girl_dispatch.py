import itertools,json,tempfile,unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.build_readable_new_oakvale import build
from tools.script_recovery.teddy_girl_dispatch_native import execute,BOUNDARIES
from tools.script_recovery.lift_native_lua import ROOT

FIXTURE='''function compare(schedule, stop, cancel, iterations, prepared)
    local events, live = {}, {}
    local q,r={},{}
    local frames,queries,iteration=0,0,0
    local function log(value) events[#events+1]=value end
    local function new(kind) live[#live+1]=kind; log("new:"..kind); return #live end
    local function settings() return schedule[(iteration-1)%#schedule+1] end
    local function phase(name) log(name); return name~=stop end
    function q:RegisterBoundConsciousCondition() log("condition") end
    function q:NewScriptFrame() frames=frames+1; log("frame") end
    function q:IsActiveThreadTerminating() queries=queries+1; local value=queries>=cancel or frames>iterations; log("term:"..(value and "1" or "0")); return value end
    function q:WithRetailResources(body)
        local ok,err=pcall(body,r)
        for i=#live,1,-1 do if live[i] then log("destroy:"..live[i]) end end
        if not ok then error(err,0) end
    end
    function r:NewResource() return new("control") end
    function r:NewThingFromScriptName(key) assert(key=="NOVI_Bully"); return new("bully") end
    function r:NewPresentedItemOutput(me) assert(me==7); iteration=iteration+1; return new("output") end
    function r:DestroyPresentedItemOutput(id) assert(live[id]=="output"); live[id]=false; log("destroy:output") end
    function r:TeddyGirlTalkedWithTeddy(me) assert(me==7); log("talkTeddy"); return settings()[1] end
    function r:IsTalkedToByHero(me) assert(me==7); log("talkPredicate"); return settings()[2] end
    function r:IsHitByHeroExceptAbility(me,ability) assert(me==7 and ability==14); log("hitPredicate"); return settings()[3] end
    function r:PrepareResource(control) assert(control==1); log("prepare:"..(prepared and "1" or "0")); if prepared then log("prepare.release") end end
    TeddyGirlAcquire=function() return phase("acquire") end
    TeddyGirlWithMovie=function(_,_,body) log("movie"); local complete=body(); log("movie.end"); return complete end
    TeddyGirlQuestion=function() return phase("question") end
    TeddyGirlPresentedKind=function() log("presentedKind"); return "none" end
    TeddyGirlPresentedResponse=function() return phase("presented") end
    TeddyGirlDeparture=function(_,_,me,control,bully) assert(me==7 and control==1 and bully==2); return phase("departure") end
    TeddyGirlTalk=function() return phase("talk") end
    TeddyGirlHit=function() return phase("hit") end
    Main(q,7)
    return events
end
'''

class TeddyGirlDispatcherTests(unittest.TestCase):
    def test_original_outer_branches_loop_entry_and_cleanup_vs_readable(self):
        with tempfile.TemporaryDirectory() as directory:
            report=build(Path(directory));source=(Path(directory)/'FSE/NewOakValeIntro/Entities/NOVI_TeddyGirl.lua').read_text()
            self.assertTrue(report['syntax']['ok']);cases=0
            for first,stop,cancel,iterations,prepare in itertools.product(itertools.product((False,True),repeat=3),(None,'acquire','question','presented','departure','talk','hit'),(1,2,3,4,5,7,999),(1,3),(False,True)):
                schedule=(first,tuple(not value for value in first))
                with self.subTest(schedule=schedule,stop=stop,cancel=cancel,iterations=iterations,prepare=prepare):
                    lua=LuaRuntime();lua.execute(source+FIXTURE)
                    native=execute(schedule,stop,cancel,iterations,prepare)
                    settings=lua.table_from([lua.table_from(row) for row in schedule])
                    readable=list(lua.globals().compare(settings,stop,cancel,iterations,prepare).values())
                    self.assertEqual(native,readable);cases+=1
            (ROOT/'work/teddy_girl_converter/DISPATCH_EVIDENCE.json').write_text(json.dumps({'cases':cases,'nativeMain':'DAF080+5504','boundaries':BOUNDARIES,
                'executed':'Original predicate-result branches, caller cancellation queries, loop backedge, prologue/epilogues and normal/cancel outer cleanup instructions.',
                'abstracted':'Bound-conscious registration, predicate interiors, acquisition and movie/dialogue/movement/hit phase interiors; separately verified by phase and adapter tests.',
                'limits':['Not a whole-Main engine replay: inner phase state and populated reference destruction are outside this dispatcher oracle.','Callback errors are covered separately by Lua/compiled owner tests; native dispatcher cases use normal and cancellation outcomes.']},indent=2)+'\n')

if __name__=='__main__':unittest.main()
