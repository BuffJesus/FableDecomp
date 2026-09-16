import tempfile
import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.generate_theresa_resource_candidate import generate


class TheresaCandidateTests(unittest.TestCase):
    def test_actual_helpers_compose_meeting_gift_and_outro(self):
        with tempfile.TemporaryDirectory() as folder:source,_=generate(folder)
        for route in ('meeting','offer','presented'):
            for cancel in (0,*range(1,24)):
                with self.subTest(route=route,cancel=cancel):
                    lua=LuaRuntime();lua.execute(source)
                    lua.globals().route=route;lua.globals().cancelAt=cancel
                    lua.execute(r'''
local events,live={},{}
local nextId,frames,terms=0,0,0
local gifted,attacked=false,false
local function record(name) events[#events+1]=name end
local function new(kind)
    nextId=nextId+1;live[nextId]=kind;record('new:'..kind);return nextId
end
local function expect(id,kind) assert(live[id]==kind,tostring(id)..':'..kind) end
local function close(id,kind) expect(id,kind);live[id]=nil;record('close:'..kind) end
local resources={
    InitializeTheresaActor=function(_,me) assert(me==17) end,
    NewResource=function() return new('control') end,
    NewTheresaDepartureTrigger=function() return new('trigger') end,
    PrepareResource=function(_,id) expect(id,'control') end,
    TryAcquire=function(_,id,me,priority) expect(id,'control');assert(me==17 and priority==4);return true end,
    TryAcquireTheresaHero=function(_,id,priority) expect(id,'control');assert(priority==4);return false end,
    ReleaseResource=function(_,id) close(id,'control') end,
    PlayTheresaSkip=function(_,id) expect(id,'control') end,
    IsTheresaNearHero=function(_,me,radius) assert(me==17 and radius==5);return true end,
    NewActorMap=function() return new('actors') end,
    SetActor=function(_,map,role,id) expect(map,'actors');expect(id,'control') end,
    DestroyActorMap=function(_,id) close(id,'actors') end,
    StartMovie=function(_,name) assert(name=='');return new('movie') end,
    Pause=function(_,on) record(on and 'pause' or 'unpause') end,
    DestroyMovie=function(_,id) close(id,'movie') end,
    RunMacro=function(_,name,map,setup,skippable)
        expect(map,'actors');assert(not setup and skippable);record(name)
    end,
    DoesTheresaHeroHaveChocolates=function() return route=='meeting' end,
    ShowTheresaChocolateQuestion=function() record('question') end,
    NewTheresaGuardVector=function()
        local id=new('guards')
        return {RemoveLivingGuards=function() expect(id,'guards');record('remove-guards') end,
                Close=function() close(id,'guards') end}
    end,
    TakeTheresaChocolatesAndUpdateObjective=function() assert(gifted);record('take-objective') end,
    ClearTheresaInformation=function(_,me) assert(me==17 and gifted);record('clear-info') end,
    NewPresentedItemOutput=function(_,me) assert(me==17);return new('presented') end,
    DestroyPresentedItemOutput=function(_,id) close(id,'presented') end,
    TheresaTalkOffersChocolates=function() return route=='offer' and not gifted end,
    PollPresentedItem=function(_,id) expect(id,'presented');return route=='presented' and not gifted end,
    PresentedItemMatches=function(_,id,name) expect(id,'presented');assert(name=='OBJECT_CHOCOLATE_BOX_UNGIVEABLE');return true end,
    WasVillagerTalkedTo=function() return false end,
    IsHitByHeroExceptAbility=function(_,me,ability) assert(me==17 and ability==14);return false end,
    NewThingFromResource=function(_,id) expect(id,'control');return new('health') end,
    ThingHealth=function(_,id) expect(id,'health');return 1 end,
    DestroyThing=function(_,id)
        if live[id]=='trigger' then close(id,'trigger') else close(id,'health') end
    end,
    SpeakTheresa=function(_,id,line) expect(id,'control');assert(line=='TEXT_QST_048_THERESA_HELLO');record('hello') end,
    IsPerformingScriptTask=function(_,id) expect(id,'control');return false end,
    IsTheresaHeroNearTrigger=function(_,id) expect(id,'trigger');assert(gifted);record('distance');return true end,
}
local fields={GUIBullyHealthCounter=101,GUIGoodDeedCounter=202,GUIBarrelCounter=303}
local q={
    RegisterBoundConsciousCondition=function() assert(frames==0);record('condition') end,
    WithRetailResources=function(_,callback) callback(resources);assert(next(live)==nil) end,
    NewScriptFrame=function() frames=frames+1;assert(frames<10) end,
    IsActiveThreadTerminating=function() terms=terms+1;return terms==cancelAt end,
    FixMovieSequenceCamera=function(_,on) record(on and 'camera-on' or 'camera-off') end,
    MsgIsQuestionAnsweredYesOrNo=function() return 1 end,
    SetStateBool=function(_,key,value)
        assert(value)
        if key=='GivenTheresaChocs' then assert(not gifted);gifted=true;record('gift-state')
        else assert(key=='AttackOver');attacked=true;record('attack-state') end
    end,
    GetStateInt=function(_,key) assert(fields[key]);return fields[key] end,
    DisplayQuestInfo=function(_,on) assert(not on);record('display-off') end,
    RemoveQuestInfoElement=function(_,id) record('remove:'..id) end,
    PlayAVIMovie=function(_,name) assert(name==[[Data\Video\1_raid_on_oak_vale_comp.xmv]]);record('avi') end,
    FadeScreenOut=function(_,duration,hold) assert(duration==0.5 and hold==0);record('fade') end,
    OverrideMusic=function(_,kind,a,b) assert(kind==25 and not a and not b);record('music') end,
}
Init(q,17);Main(q,17)
assert(next(live)==nil)
if cancelAt==0 then
    assert(gifted and attacked)
    local text=table.concat(events,',')
    assert(string.find(text,'remove:101,remove:202,remove:303',1,true))
    assert(string.find(text,'avi,fade,music,attack-state,close:actors,close:control,unpause,close:movie,close:presented,close:trigger,close:control',1,true))
    assert(string.find(text,'gift-state,take-objective,clear-info',1,true))
end
''')

    def test_assembled_init_and_entry_condition_before_first_frame(self):
        with tempfile.TemporaryDirectory() as folder:source,report=generate(folder)
        lua=LuaRuntime();lua.execute(source)
        lua.execute('''
local events={}
local resource={InitializeTheresaActor=function(_,me) assert(me==17);events[#events+1]='init' end}
local q={
    RegisterBoundConsciousCondition=function() events[#events+1]='condition' end,
    WithRetailResources=function(_,callback) callback(resource) end,
    NewScriptFrame=function(_,me) assert(me==17);events[#events+1]='frame' end,
    IsActiveThreadTerminating=function() events[#events+1]='term';return true end,
}
Init(q,17); Main(q,17)
assert(table.concat(events,',')=='init,condition,frame,term')
''')
        self.assertFalse(report['enabled'])
        self.assertEqual(report['entryCondition']['method'],'RegisterBoundConsciousCondition')
        self.assertNotIn('LAB_',source)
        self.assertNotIn('goto ',source)
