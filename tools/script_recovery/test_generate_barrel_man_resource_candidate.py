import tempfile
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.generate_barrel_man_resource_candidate import DRAFT, generate


HARNESS = '''
return function(source, cancelAt, failures, queryError)
    local events, live, nextId, terminationCalls = {}, {}, 0, 0
    local function event(name, id) events[#events+1] = name .. (id and ':' .. id or '') end
    local resources = {}
    function resources:SetBarrelWatchTimer(id) assert(id==1);event('watch') end
    function resources:NewResource()
        nextId=nextId+1; live[nextId]='resource'; event('construct',nextId); return nextId
    end
    function resources:PrepareResource(id) assert(live[id]=='resource');event('prepare',id) end
    function resources:TryAcquire(id, actor, priority)
        assert(live[id]=='resource' and actor.name=='barrel' and priority==4)
        event('acquire',id); failures=failures-1; return failures<0
    end
    function resources:ReleaseResource(id) assert(live[id]=='resource');live[id]=nil;event('release',id) end
    function resources:IsHitByHeroExceptAbility(actor, ability) assert(actor.name=='barrel' and ability==14);return false end
    function resources:NewThingFromResource(id)
        assert(live[id]=='resource');nextId=nextId+1;live[nextId]='thing';event('thing',nextId);return nextId
    end
    function resources:NewThingFromScriptName(name)
        assert(name=='M_BarrelManWalkOff' or name=='M_WHouse_ManStart' or name=='M_WHouse_GuardPoint');nextId=nextId+1;live[nextId]='marker';event('marker',nextId);return nextId
    end
    function resources:ThingPosition(id) assert(live[id]=='marker');return {x=11,y=12,z=13} end
    function resources:DestroyThing(id) assert(live[id]=='thing' or live[id]=='marker');live[id]=nil;event('destroy',id) end
    function resources:ThingIsDistanceFromPositionOver(id, position, distance)
        assert(live[id]=='thing' and position.x==11 and distance==2)
        event('distance',id); if queryError then error('QUERY',0) end; return false
    end
    local quest = setmetatable({}, {__index=function() return function() end end})
    function quest:WithRetailResources(callback)
        local ok, err = pcall(callback,resources)
        for id=nextId,1,-1 do
            if live[id]=='thing' or live[id]=='marker' then resources:DestroyThing(id)
            elseif live[id]=='resource' then resources:ReleaseResource(id) end
        end
        if not ok then error(err,0) end
    end
    function quest:IsActiveThreadTerminating()
        terminationCalls=terminationCalls+1
        if terminationCalls>30 then error('unbounded phase',0) end
        return terminationCalls>=cancelAt
    end
    function quest:GetThingWithScriptName(name) return {name=name} end
    function quest:GetStateInt() return 1 end
    function quest:GetTimer() return 15 end
    local me={name='barrel',GetHomePos=function() return {x=1,y=2,z=3} end,IsTalkedToByHero=function() return false end}
    local env=setmetatable({RetailThingPosition=function(marker) assert(marker.name=='M_BarrelManWalkOff');return {x=11,y=12,z=13} end}, {__index=_G})
    local api=assert(load(source .. '\\nreturn {Main=Main,setPhase=function(value) __native_entity_state:SetStateInt("MyPhase",value) end}', 'candidate', 't', env))()
    api.setPhase(1)
    local ok,err=pcall(api.Main,quest,me)
    assert(next(live)==nil)
    return table.concat(events,','),ok,err
end
'''


class BarrelResourceCandidateTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.source,cls.report=generate()

    def test_entry_and_acquisition_cancellation_keep_one_resource(self):
        run=LuaRuntime(unpack_returned_tuples=True).execute(HARNESS)
        for failures in (0,1,4):
            # Stay within entry/acquisition/first-distance phase. Later hit and
            # movie branches are deliberately still reported as incomplete.
            for cancel in range(1,6+failures):
                trace,ok,err=run(self.source,cancel,failures,False)
                with self.subTest(cancel=cancel,failures=failures):
                    self.assertTrue(ok,err)
                    events=trace.split(',') if trace else []
                    self.assertEqual(events.count('construct:1'),0 if cancel==1 else 1)
                    self.assertEqual(events.count('release:1'),0 if cancel==1 else 1)
                    self.assertFalse(any(e.startswith('acquire:') and e!='acquire:1' for e in events))
                    if 'distance:5' in events:
                        self.assertEqual(events[events.index('thing:5'):events.index('destroy:5')+1],
                                         ['thing:5','distance:5','destroy:5'])

    def test_temporary_query_error_closes_thing_before_resource(self):
        run=LuaRuntime(unpack_returned_tuples=True).execute(HARNESS)
        trace,ok,err=run(self.source,9,0,True)
        self.assertFalse(ok);self.assertEqual(err,'QUERY')
        self.assertTrue(trace.endswith('thing:5,distance:5,destroy:5,destroy:4,destroy:3,destroy:2,release:1'),trace)

    def test_changed_draft_rejects_before_writing(self):
        with tempfile.TemporaryDirectory() as directory:
            draft=Path(directory)/'draft.lua';out=Path(directory)/'candidate'
            draft.write_text(DRAFT.read_text().replace('me:AcquireControl(4)','me:AcquireControl(3)',1))
            with self.assertRaisesRegex(ValueError,'draft correspondence'):
                generate(out,draft=draft)
            self.assertFalse(out.exists())
