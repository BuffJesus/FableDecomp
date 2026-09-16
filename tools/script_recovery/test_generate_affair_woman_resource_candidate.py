import tempfile
import unittest
from pathlib import Path

from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_generate_book_trader_resource_candidate as book_tests
from tools.script_recovery.generate_affair_woman_resource_candidate import DRAFT,generate

MOCK=book_tests.MOCK+r'''
local names={}
function resources:NewThingFromScriptName(name)
    local id=create('thing');names[id]=name;record('lookup',id,name);return id
end
function resources:ThingsAreWithinDistance(first,second,distance)
    assert(first==me and names[second]=='NOVI_AffairWife' and distance==5)
    return options.runoff or false
end
function resources:FaceThing(actor,target,snap)
    assert(names[actor]=='NOVI_AffairMan' and (target==hero or target==me) and snap==false)
    record('face',target==hero and 'hero' or 'self')
    if options.error_at=='face' then error('injected facing failure') end
end
function resources:ThingPosition(id)
    require_kind(id,'thing');assert(names[id]=='AffairWomanRunOffPoint')
    record('marker.position');return {x=30,y=40,z=50}
end
local moved=false
local runoff_moves=0
function resources:ThingIsDistanceFromPositionOver(id,position,distance)
    require_kind(id,'thing');assert(distance==2);record('distance',id)
    if position.x==30 then return runoff_moves < (options.runoff_moves or 0) end
    return not moved
end
function resources:MoveToPosition(id,position,radius,moveType,a,b)
    require_kind(id,'resource');assert(radius==0 and not a and b)
    assert(moveType==0 and position.x==-17.5 or moveType==1 and position.x==30)
    if moveType==1 then runoff_moves=runoff_moves+1 else moved=true end
    record('move',moveType)
end
function resources:ClearAllActions(id) require_kind(id,'resource');record('clear.actions') end
function resources:ClearCommands(id) require_kind(id,'resource');record('clear.commands') end
function resources:PlayAnimation(id,name,a,b,c,d,byte,e)
    require_kind(id,'resource');assert(not a and b and not c and d and not e)
    assert(name=='RECEIVE_KISS' or name=='RECEIVE_HUG');record('animation',id,byte)
end
function me:GetPos() record('self.position');return {x=1,y=2,z=3} end
function quest:IsCameraPosOnScreen(pos) assert(pos.x==1);record('camera');return options.camera or false end
function quest:RemoveThing(actor,a,b) assert(actor==me and not a and b);record('remove.self') end
function quest:GetStateBool(name)
    if name=='ReceiveKiss' then return options.kiss or false end
    if name=='ReceiveHug' then return options.hug or false end
    return false
end
local next_frame=quest.NewScriptFrame
function quest:NewScriptFrame(...)
    if options.trace_frames then record('frame') end
    return next_frame(self,...)
end
local terminating=quest.IsActiveThreadTerminating
function quest:IsActiveThreadTerminating()
    local result=terminating(self)
    if options.trace_frames then record('terminating',result) end
    return result
end
package.preload['NewOakValeIntro.native_quest_helpers']=function()
    return {AddBadDeed=function(q,actor,value) assert(q==quest and actor==me and value==2);record('bad.deed') end}
end
'''


class AffairWomanCandidateTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls): cls.source,cls.report=generate()

    def run_case(self,**options):
        lua=LuaRuntime();lua.globals().options=lua.table_from(options);lua.execute(MOCK);lua.execute(self.source)
        lua.execute('Init(quest,me)');error=None
        try:lua.execute('Main(quest,me)')
        except Exception as exc:error=str(exc)
        events=[tuple(row[i] for i in range(1,len(row)+1)) for row in lua.globals().events.values()]
        return events,error

    def test_hit_and_talk_movie_and_partner_order(self):
        for scenario,text in [({'hit':True},'TEXT_QST_048_AFFAIRWOMAN_ON_HIT'),({'talk':True},'TEXT_QST_048_AFFAIRWOMAN_BUSY')]:
            events,error=self.run_case(**scenario)
            self.assertIsNone(error)
            self.assertIn(('speak',1,text),events)
            self.assertLess(events.index(('pause',True)),events.index(('health',5)))
            self.assertLess(events.index(('destroy.thing',5)),events.index(('speak',1,text)))
            self.assertLess(events.index(('pause',False)),events.index(('destroy.movie',4)))
            self.assertEqual(events[-3:],[('destroy.thing',3),('destroy.thing',2),('destroy.resource',1)])
            if scenario.get('talk'):
                self.assertLess(events.index(('clear.actions',)),events.index(('clear.commands',)))
                self.assertLess(events.index(('face','hero')),events.index(('face','self')))

    def test_movement_runoff_and_animations(self):
        events,error=self.run_case(move=True,stop_frame=3)
        self.assertIsNone(error);self.assertIn(('move',0),events)
        events,error=self.run_case(runoff=True,stop_frame=10)
        self.assertIsNone(error);self.assertIn(('remove.self',),events)
        self.assertEqual(events[-4:],[('destroy.thing',4),('destroy.thing',3),('destroy.thing',2),('destroy.resource',1)])
        events,error=self.run_case(kiss=True,hug=True,animation_byte=47)
        self.assertIsNone(error)
        self.assertEqual(sum(e[0]=='animation' for e in events),2)

    def test_cancellation_and_injected_errors_leave_no_live_children(self):
        for scenario in ({},{'hit':True},{'talk':True},{'move':True},{'runoff':True},{'fail_acquire':True},{'hit':True,'busy':True}):
            for check in range(1,30):
                with self.subTest(scenario=scenario,check=check):
                    _,error=self.run_case(**scenario,stop_check=check,stop_frame=4)
                    self.assertIsNone(error)
        for scenario in ({'hit':True,'error_at':'health'},{'hit':True,'error_at':'speak'},{'talk':True,'error_at':'face'}):
            events,error=self.run_case(**scenario)
            self.assertIn('injected',error)
            self.assertEqual(events[-1],('destroy.resource',1))

    def test_changed_draft_rejects(self):
        with tempfile.TemporaryDirectory() as directory:
            path=Path(directory)/'woman.lua';path.write_text(DRAFT.read_text().replace('AcquireControl(4)','AcquireControl(3)'))
            with self.assertRaises(ValueError):generate(draft=path)
