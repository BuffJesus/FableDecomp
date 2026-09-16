import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.bully_runoff_movie import SOURCE,evidence
from tools.script_recovery.bully_runoff_movie_native import execute
from tools.script_recovery.bully_full_resource_candidate import generate

HARNESS='''return function(source,attacked,given,cancel,fault)
    local trace,live,resources,quest={}, {}, {}, {}
    local function record(s) trace[#trace+1]=s end
    local function new(kind) assert(not live[kind]);live[kind]=true;record('new:'..kind);return kind end
    local function destroy(kind) assert(live[kind]);live[kind]=false;record('destroy:'..kind) end
    function resources:NewActorMap() return new('actors') end
    function resources:SetActor(map,key,resource) assert(live[map]);record('actor:'..key..':'..resource) end
    function resources:NewStringMap() return new('strings') end
    function resources:SetString(map,key,value) assert(live[map]);record('string:'..key..':'..value) end
    function resources:StartMovie(key) assert(key=='');return new('movie') end
    function resources:Pause(flag) assert(live.movie);record('pause:'..tostring(flag)) end
    function resources:RunMacroWithStrings(name,actors,inputs,setup,skip)
        assert(live[actors] and live[inputs] and live.movie and not setup and skip)
        record('macro:'..name..':strings');if fault==1 then error('MACRO_FAULT') end
    end
    function resources:RunMacro(name,actors,setup,skip)
        assert(live[actors] and live.strings and live.movie and not setup and skip)
        record('macro:'..name..':null');if fault==2 then error('MACRO_FAULT') end
    end
    function resources:ClearThingHasInformation(id) assert(id=='victimThing');record('clear:'..id) end
    function resources:DestroyMovie(id) destroy(id) end
    function resources:DestroyStringMap(id) destroy(id) end
    function resources:DestroyActorMap(id) destroy(id) end
    function quest:GetStateBool(name) if name=='HeroAttackedVictim' then return attacked end;assert(name=='GivenHeroTeddy');return given end
    function quest:SetStateBool(name,value) assert(value);record('state:'..name) end
    function quest:FixMovieSequenceCamera(value) record('camera:'..tostring(value)) end
    local queries=0
    function quest:IsActiveThreadTerminating() queries=queries+1;local term=queries>=cancel;record('term:'..tostring(term));return term end
    assert(load(source))()
    local ok,result=pcall(BullyRunoffMovie,quest,resources,'hero','victim','bully','victimThing')
    if not ok then
        -- Current outer resource Close: unpause movie then reverse-entry cleanup.
        if live.movie then resources:Pause(false);destroy('movie') end
        if live.strings then destroy('strings') end
        if live.actors then destroy('actors') end
    end
    record('destroy:victim');record('destroy:hero')
    if ok and result then record('state:BullyRanOff');record('good.deed');record('remove:me') end
    for _,value in pairs(live) do assert(not value) end
    return trace,ok,tostring(result)
end'''

class BullyRunoffMovieTests(unittest.TestCase):
    def lua(self,attacked,given,cancel,fault=0):
        events,ok,result=LuaRuntime(unpack_returned_tuples=True).execute(HARNESS)(SOURCE,attacked,given,cancel,fault)
        return list(events.values()),ok,result

    def test_native_actor_maps_inputs_and_cancellation_match_lua(self):
        for attacked in (False,True):
            for given in (False,True):
                for cancel in (1,2,999):
                    with self.subTest(attacked=attacked,given=given,cancel=cancel):
                        actual,ok,_=self.lua(attacked,given,cancel)
                        self.assertTrue(ok);self.assertEqual(execute(attacked,given,cancel),actual)

    def test_macro_errors_close_movie_before_maps_and_borrowed_controls(self):
        for fault in (1,2):
            for given in (False,True):
                trace,ok,error=self.lua(True,given,999,fault)
                self.assertFalse(ok);self.assertIn('MACRO_FAULT',error)
                self.assertEqual(trace[-6:],['pause:false','destroy:movie','destroy:strings','destroy:actors','destroy:victim','destroy:hero'])
                self.assertNotIn('state:BullyRanOff',trace);self.assertNotIn('camera:false',trace)

    def test_phase_is_composed_and_old_targets_removed(self):
        source,report=generate()
        self.assertIn('if BullyRunoffMovie(quest, resources, bully_hero_control, bully_victim_control, bully_control, r1)',source)
        self.assertNotIn('quest:ClearThingHasInformation(r14)',source)
        self.assertNotIn('quest:RemoveThing(r13',source)
        self.assertNotIn('RunCutsceneMacro_Func',source)
        self.assertFalse(report['enabled']);self.assertEqual(evidence()['actorResources']['BRAT'],116)

if __name__=='__main__':unittest.main()
