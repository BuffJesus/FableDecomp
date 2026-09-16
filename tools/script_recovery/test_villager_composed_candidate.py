import tempfile
import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.generate_villager_resource_candidate import generate
from tools.script_recovery.lift_native_lua import ROOT


class VillagerComposedCandidateTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with tempfile.TemporaryDirectory() as output:cls.source,cls.report=generate(output)

    def check_path(self,branch,cancel,error=''):
        lua=LuaRuntime();lua.globals().config=lua.table_from(dict(branch=branch,cancel=cancel,error=error))
        lua.execute(self.source)
        return lua.execute('''
            local live,events={},{}
            local nextId,terms=0,0
            local function event(name)
                events[#events+1]=name
                if config.error==name then error('INJECTED '..name) end
            end
            local resources={}
            local function create(kind) nextId=nextId+1;live[nextId]=kind;event('new.'..kind);return nextId end
            local function destroy(id,kind) assert(live[id]==kind);live[id]=nil;event('destroy.'..kind) end
            function resources:NewResource() return create('control') end
            function resources:NewText() return create('text') end
            function resources:StartMovie(key) assert(key=='');return create('movie') end
            function resources:NewThingFromResource(id) assert(live[id]=='control');return create('thing') end
            function resources:DestroyText(id) destroy(id,'text') end
            function resources:ReleaseResource(id) destroy(id,'control') end
            function resources:DestroyMovie(id) destroy(id,'movie') end
            function resources:DestroyThing(id) destroy(id,'thing') end
            function resources:PrepareResource(id) assert(live[id]=='control');event('prepare') end
            function resources:TryAcquire(id,me,priority) assert(live[id]=='control' and me==9 and priority==4);event('acquire');return true end
            function resources:WasVillagerHit() event('hit');return config.branch=='hit' end
            function resources:WasVillagerTalkedTo() event('talk');return config.branch=='talk' end
            function resources:ShouldVillagerStartAmbientConversation() event('ambient');return config.branch=='ambient' end
            function resources:ThingHealth(id) assert(live[id]=='thing');event('health');return 1 end
            function resources:IsPerformingScriptTask() event('task');return false end
            function resources:StartVillagerTalkConversation() event('conversation');return 73 end
            resources.StartVillagerAmbientConversation=resources.StartVillagerTalkConversation
            for _,name in ipairs({'Pause','SetVillagerHeroAllies','SpeakVillagerAttacked','AssignVillagerSuffix',
                'AddVillagerTalkLine','AssignVillagerSpeechText','AddVillagerAmbientText'}) do
                resources[name]=function() event(name) end
            end
            local q={}
            function q:RegisterBoundConsciousCondition() event('entry') end
            function q:NewScriptFrame() event('frame') end
            function q:IsActiveThreadTerminating() terms=terms+1;assert(terms<100);return terms>=config.cancel end
            function q:EntityGetSex() event('sex');return 1 end
            function q:IsConversationActive() return false end
            function q:GetStateInt(name) return name=='BadDeedsPerformed' and 1 or name=='lastVillagerSpeechIdx' and -1 or 0 end
            function q:SetStateInt() end
            function q:RetailRandModulo() return 0 end
            function q:GetVillagerSpeechLists() return {Count=function() return 6 end} end
            function q:WithRetailResources(callback)
                local ok,e=pcall(callback,resources)
                -- Normal and injected body-error paths must release explicitly.
                assert(next(live)==nil,'unreleased resource')
                if not ok then error(e,0) end
            end
            require=function() return {AddBadDeed=function() event('deed') end} end
            local ok,e=pcall(Main,q,9)
            assert(next(live)==nil)
            if config.error=='' then assert(ok,e) else assert(not ok and string.find(e,'INJECTED',1,true),e) end
            if nextId>0 then assert(events[#events]=='destroy.control' and events[#events-1]=='destroy.text') end
            return terms
        ''')

    def test_all_branches_release_at_each_termination_check(self):
        for branch in ('idle','hit','talk','ambient'):
            for cancel in range(1,20):
                with self.subTest(branch=branch,cancel=cancel):self.check_path(branch,cancel)

    def test_composed_error_unwinds(self):
        for branch,error in (('hit','health'),('hit','SpeakVillagerAttacked'),('talk','AddVillagerTalkLine'),('ambient','AddVillagerAmbientText')):
            with self.subTest(branch=branch,error=error):self.check_path(branch,50,error)


class ReadableVillagerComposedTests(VillagerComposedCandidateTests):
    @classmethod
    def setUpClass(cls):
        cls.source=(ROOT/'refs/script_recovery/lifted/NewOakValeIntro/readable/FSE/NewOakValeIntro/Entities/NOVI_Villager.lua').read_text()
