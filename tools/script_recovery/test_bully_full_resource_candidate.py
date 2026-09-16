import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.bully_full_resource_candidate import generate

HARNESS='''return function(source, mode, health)
    local events,entries={},{}
    local function rec(v) events[#events+1]=v end
    local resources={}
    local function add(kind) entries[#entries+1]={kind=kind,live=true};rec('new:'..kind);return #entries end
    local function destroy(id) local e=assert(entries[id]);assert(e.live);e.live=false;rec('destroy:'..e.kind) end
    function resources:NewResource() return add('control') end
    function resources:PrepareResource(id) assert(entries[id].live) end
    function resources:TryAcquire(id,actor,priority) assert(entries[id].live and priority==4);return true end
    function resources:ReleaseResource(id) destroy(id) end
    function resources:NewThingFromResource(id) assert(entries[id].live);return add('temporary') end
    function resources:ThingIsDistanceFromPositionOver(id,position,distance) assert(distance==2);return false end
    function resources:NewThingFromScriptName(name) assert(name=='NOVI_Victim');return add('victim') end
    function resources:DestroyThing(id) destroy(id) end
    function resources:ThingHealth(id) assert(entries[id].live);return health end
    function resources:BullyTalkedWithTeddy(actor) return mode=='question' end
    function resources:NewPresentedItemOutput(actor) return add('presented') end
    local polls=0
    function resources:PollPresentedItem(id) assert(entries[id].live);polls=polls+1;return true end
    function resources:PresentedItemMatches(id,name) assert(entries[id].live);return mode~='other' end
    function resources:DestroyPresentedItemOutput(id) destroy(id) end
    function resources:StartMovie(key) assert(key=='');return add('movie') end
    function resources:Pause(flag) rec('pause:'..tostring(flag)) end
    function resources:DestroyMovie(id) destroy(id) end
    function resources:Speak(control,hero,key,...) assert(entries[control].live);rec('speak:'..key) end
    function resources:IsPerformingScriptTask(id) return false end
    local quest,me={},{}
    function me:GetHomePos() return {x=7,y=-3,z=11} end
    function me:IsTalkedToByHero() error('AFTER_ITEM') end
    function quest:WithRetailResources(body)
        local ok,err=pcall(body,resources)
        -- Match current resource Close ordering for fallback only.
        local hasMovie=false
        for _,entry in ipairs(entries) do if entry.live and entry.kind=='movie' then hasMovie=true end end
        if hasMovie then resources:Pause(false) end
        for i=#entries,1,-1 do if entries[i].live then destroy(i) end end
        if not ok then error(err,0) end
    end
    local frames=0
    function quest:NewScriptFrame() frames=frames+1;if frames>10 then error('FRAME_GUARD') end end
    function quest:IsActiveThreadTerminating() return false end
    function quest:RegisterBoundConsciousCondition() rec('condition') end
    function quest:GetHero() return {name='hero'} end
    function quest:GetStateBool(name) return false end
    function quest:SetStateBool(name,value) rec('state:'..name) end
    function quest:GiveHeroYesNoQuestion(...) rec('question');error('QUESTION') end
    function resources:GiveBullyTeddyQuestion() return quest:GiveHeroYesNoQuestion('TEXT_QST_048_GIVE_TEDDY_TO_BULLY','TEXT_OBJECT_HERO_ANSWER_YES','TEXT_OBJECT_HERO_ANSWER_NO','',true) end
    setmetatable(quest,{__index=function(_,name) return function(...) rec('api:'..name);return false end end})
    package.preload['NewOakValeIntro.native_quest_helpers']=function() return {AddBadDeed=function() rec('bad.deed') end} end
    source=source..'\\nfunction fixture() __native_entity_state:SetStateBool("DoneIntro",true) end'
    local fn=assert(load(source,'candidate','t',_G));fn();fixture()
    local ok,err=pcall(Main,quest,me)
    return events,ok,tostring(err),polls
end'''

class BullyFullResourceCandidateTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):cls.source,cls.report=generate()

    def run_case(self,mode,health):
        lua=LuaRuntime(unpack_returned_tuples=True);lua.execute('package={preload={}}; function require(n) return package.preload[n]() end')
        events,ok,error,polls=lua.execute(HARNESS)(self.source,mode,health)
        return list(events.values()),ok,error,polls

    def test_presented_movies_complete_shared_cleanup_before_next_interaction(self):
        for mode in ('teddy','other'):
            events,ok,error,polls=self.run_case(mode,1.0)
            self.assertFalse(ok);self.assertIn('AFTER_ITEM',error)
            self.assertEqual(polls,1 if mode=='teddy' else 2)
            self.assertEqual(events.count('pause:true'),1);self.assertEqual(events.count('pause:false'),1)
            self.assertEqual(events.count('destroy:movie'),1);self.assertEqual(events.count('destroy:presented'),1)
            self.assertLess(events.index('destroy:movie'),events.index('destroy:presented'))
            self.assertEqual(events[-2:],['destroy:victim','destroy:control'])

    def test_greeting_then_question_join_for_positive_zero_and_nan_health(self):
        for health in (1.0,0.0,float('nan')):
            events,ok,error,_=self.run_case('question',health)
            self.assertFalse(ok);self.assertIn('QUESTION',error)
            greeting='speak:TEXT_QST_048_BULLY_FOUND_TEDDY_ONE'
            self.assertEqual(greeting in events,health>0)
            if health>0:self.assertLess(events.index(greeting),events.index('question'))
            self.assertEqual(events[-4:],['destroy:movie','destroy:presented','destroy:victim','destroy:control'])

    def test_full_owned_body_stays_disabled_with_remaining_native_join_gaps_visible(self):
        self.assertFalse(self.report['enabled']);self.assertFalse(self.report['gameplayComplete'])
        self.assertIn('function Main(quest, me)',self.source)
        self.assertNotIn('me:AcquireControl(',self.source)
        self.assertNotIn('g_PresentedItemName',self.source)
        self.assertNotIn('extraout_AL_06',self.source)
        self.assertNotIn('nil --[[missing]]',self.source)
        self.assertNotIn('return end  -- TODO(native): goto',self.source)
        self.assertTrue(self.report['limits'])
