"""Actual composed Main exercises resource ownership and callback failures."""
import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.guard_candidate import generate

HARNESS='''return function(source, mode, cancel, fault)
    local events, entries, fields = {}, {}, {BadDeedsPerformed=mode=="lecture" and 1 or 0,GuardsDealtWithBadDeeds=0,GoodDeedsPerformed=1}
    local function rec(name) events[#events+1]=name; if name==fault then error("PRIMARY_CALLBACK_FAULT") end end
    local quest, resources, me = {}, {}, {}
    local queries, attempts = 0, 0
    local function get(id,kind) assert(entries[id] and entries[id].live and entries[id].kind==kind);return entries[id] end
    local function new(kind) local id=#entries+1;entries[id]={live=true,kind=kind};rec('new:'..kind);return id end
    local function destroy(id) local e=entries[id];assert(e.live);e.live=false;rec('destroy:'..e.kind) end
    function quest:WithRetailResources(body)
        local ok,err=pcall(body,resources)
        local liveMovie=false
        for _,e in ipairs(entries) do if e.live and e.kind=='movie' then liveMovie=true end end
        if liveMovie then rec('pause:false') end
        for i=#entries,1,-1 do if entries[i].live then destroy(i) end end
        if not ok then error(err,0) end
    end
    function quest:IsActiveThreadTerminating() queries=queries+1;rec('query');return queries>=cancel end
    function quest:NewScriptFrame() rec('frame') end
    function quest:GetStateInt(key) return assert(fields[key]) end
    function quest:SetStateInt(key,value) fields[key]=value;rec('set:'..key) end
    function quest:GetStateBool(key) return fields[key] or false end
    function quest:SetStateBool(key,value) fields[key]=value;rec('set:'..key) end
    function quest:GetHero() rec('hero');return 'hero' end
    function quest:IsHeroControlledByPlayer() rec('player');return true end
    function quest:IsConversationActive(id) assert(id==-7);return false end
    function me:IsTalkedToByHero() return mode=='talk' end
    function resources:NewResource() return new('control') end
    function resources:InitializeGuardActor(actor) assert(actor==me);rec('init.actor') end
    function resources:PrepareResource(id) get(id,'control');rec('prepare') end
    function resources:TryAcquire(id,actor,priority)
        assert(actor==me and priority==4);get(id,'control');attempts=attempts+1
        -- Failed output remains populated and belongs to the same resource.
        entries[id].populated=true;rec('acquire');return attempts>2
    end
    function resources:ReadGuardAlertRange(index) assert(index==1);return 9.25 end
    function resources:ReadGuardLectureRange() return 2.5 end
    function resources:IsDistanceBetweenThingsUnder(actor,hero,range) assert(actor==me and hero=='hero');return true end
    function resources:SetThingAsAlly(a,b) assert((a==me and b=='hero') or (a=='hero' and b==me));rec('ally') end
    function resources:GuardFaceHero(actor,snap,reverse) assert(actor==me);rec('face') end
    function resources:SetRawCutsceneBehaviour(actor,value) assert(actor==me);rec('cutscene:'..value) end
    function resources:NewConversation(actor,a,b) assert(actor==me and not a and not b);rec('conversation');return -7 end
    function resources:AddRawConversationPerson(id,hero) assert(id==-7 and hero=='hero');rec('person') end
    function resources:GuardAddHeroConversationLine(id,key,actor) assert(id==-7 and actor==me);rec('line') end
    function resources:StartMovie(key) assert(key=='');return new('movie') end
    function resources:Pause(flag) rec('pause:'..tostring(flag)) end
    function resources:DestroyMovie(id) get(id,'movie');destroy(id) end
    function resources:NewThingFromResource(id) get(id,'control');return new('thing') end
    function resources:ThingHealth(id) get(id,'thing');rec('health');return 1.0 end
    function resources:DestroyThing(id) get(id,'thing');destroy(id) end
    function resources:Speak(id,hero,key,selection,listen,sound,fade)
        get(id,'control');assert(hero=='hero' and selection==0 and not listen and sound and not fade);rec('speak:'..key)
    end
    function resources:IsPerformingScriptTask(id) get(id,'control');return false end
    function resources:IsHitByHeroExceptAbility(actor,ability) assert(actor==me and ability==14);return mode=='hit' end
    assert(load(source,'guard','t',_G))()
    local ok,err=pcall(Main,quest,me)
    for _,e in ipairs(entries) do assert(not e.live) end
    return events,ok,tostring(err),fields
end'''

class GuardCandidateTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):cls.source,cls.report=generate()

    def run_case(self,mode,cancel=80,fault=''):
        lua=LuaRuntime(unpack_returned_tuples=True)
        events,ok,error,fields=lua.execute(HARNESS)(self.source,mode,cancel,fault)
        return list(events.values()),ok,error,dict(fields.items())

    def test_first_lecture_claims_then_marks_spoken_and_reuses_owned_control(self):
        events,ok,error,fields=self.run_case('lecture')
        self.assertTrue(ok,error);self.assertEqual(fields['GuardsDealtWithBadDeeds'],1)
        self.assertTrue(fields['GuardsSpokenOnce'])
        self.assertEqual(events.count('new:control'),1);self.assertEqual(events.count('destroy:control'),1)
        self.assertEqual(events.count('new:movie'),1)
        self.assertLess(events.index('set:GuardsDealtWithBadDeeds'),events.index('new:movie'))
        self.assertLess(events.index('set:GuardsSpokenOnce'),events.index('destroy:movie'))

    def test_every_cancellation_position_closes_the_actual_live_scopes(self):
        for mode in ('lecture','talk','hit'):
            for cancel in range(1,51):
                with self.subTest(mode=mode,cancel=cancel):
                    events,ok,error,_=self.run_case(mode,cancel)
                    self.assertTrue(ok,error)
                    self.assertEqual(events.count('new:control'),1)
                    self.assertEqual(events.count('destroy:control'),1)
                    self.assertEqual(events[-1],'destroy:control')
                    self.assertEqual(events.count('new:movie'),events.count('destroy:movie'))
                    self.assertEqual(events.count('new:thing'),events.count('destroy:thing'))

    def test_callback_errors_keep_primary_error_and_reverse_cleanup(self):
        for mode,fault in (('lecture','health'),('lecture','speak:TEXT_QST_048_GUARD_CAUGHT_YOU_10'),('hit','speak:TEXT_QST_048_GUARD_ON_HIT'),('talk','line')):
            with self.subTest(mode=mode,fault=fault):
                events,ok,error,_=self.run_case(mode,fault=fault)
                self.assertFalse(ok);self.assertIn('PRIMARY_CALLBACK_FAULT',error)
                self.assertEqual(events[-1],'destroy:control')
                if mode!='talk':
                    self.assertLess(events.index('pause:false'),events.index('destroy:movie'))
                    self.assertLess(events.index('destroy:movie'),events.index('destroy:control'))

    def test_review_status_keeps_verified_init_and_pending_host_gates_explicit(self):
        self.assertFalse(self.report['enabled']);self.assertFalse(self.report['gameplayComplete'])
        self.assertNotIn('goto ',self.source);self.assertNotIn('nil --[[missing]]',self.source)
        self.assertNotIn('RegisterBound',self.source)
        self.assertTrue(any('Init' in x for x in self.report['limits']))
        self.assertEqual(len(self.report['init']['consumers']),4)
        self.assertIn('resources:InitializeGuardActor(me)',self.source)

if __name__=='__main__':unittest.main()
