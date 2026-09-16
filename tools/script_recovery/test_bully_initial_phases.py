import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.bully_initial_phases import recover
from tools.script_recovery.bully_health_native import execute as native_health
from tools.script_recovery.bully_home_native import execute as native_home
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.bully_initial_candidate import correspondence,DRAFT

class BullyInitialPhaseTests(unittest.TestCase):
    def test_draft_correspondence_and_nan_inverse_branch_are_guarded(self):
        edits=correspondence();self.assertEqual(len(edits),10)
        self.assertIn('if not BullyControlledHealthAboveThreshold',edits[2]['new'])
        self.assertIn('fVar18 <= fVar2',edits[2]['old'])
        self.assertEqual([e['nativeStart'] for e in edits[:4]],[0xdbbc23,0xdbbb1f,0xdbb729,0xdbb91b])
        self.assertIn('aCStack_f4',edits[-1]['old'])
        with self.assertRaises(ValueError):correspondence(DRAFT.read_text().replace('fVar18 <= fVar2','fVar18 < fVar2'))

    def test_initial_scope_reuses_failed_output_and_unwinds_cancellation_or_body_error(self):
        for cancel in (1,2,3,999):
            for error in (False,True):
                lua=LuaRuntime();lua.execute(recover()[0]);q=lua.table();me=lua.table();r=lua.table();events=[]
                state={'checks':0,'tries':0,'open':False}
                q.RegisterBoundConsciousCondition=lambda _:events.append('condition')
                q.NewScriptFrame=lambda _:events.append('frame')
                def term(_):
                    state['checks']+=1;events.append('term');return state['checks']>=cancel
                q.IsActiveThreadTerminating=term
                r.NewResource=lambda _:events.append('new') or 7
                r.PrepareResource=lambda _,c:events.append(('prepare',c))
                def acquire(_,c,actor,priority):
                    self.assertEqual((c,priority),(7,4));state['tries']+=1
                    # Failed acquire leaves populated native output; same id is
                    # reused and no release/prepare occurs between attempts.
                    state['open']=True;events.append(('acquire',c));return state['tries']>=2
                r.TryAcquire=acquire
                def scope(_,body):
                    try:body(r)
                    finally:events.append(('destroy',state['open']));state['open']=False
                q.WithRetailResources=scope
                lua.globals().BullyReturnHomePhase=lambda *args:events.append('home') or True
                def continuation(resources,control):
                    self.assertTrue(state['open']);events.append('continuation')
                    if error:raise RuntimeError('BODY')
                if cancel==999 and error:
                    with self.assertRaisesRegex(Exception,'BODY'):lua.globals().WithBullyInitialControl(q,me,continuation)
                else:lua.globals().WithBullyInitialControl(q,me,continuation)
                self.assertEqual(events[:3],['condition','frame','term'])
                self.assertFalse(state['open'])
                if cancel==1:self.assertNotIn('new',events)
                else:
                    self.assertEqual(events.count(('prepare',7)),1)
                    self.assertEqual(events[-1],('destroy',True))
                if cancel==999:self.assertEqual(events[-3:-1],['home','continuation'])

    def test_nine_native_health_consumers_empty_output_and_unordered(self):
        for site in range(9):
            for value in (-1.0,-0.0,0.0,0.0001,1.0,float('inf'),float('-inf'),float('nan')):
                for empty in (False,True):
                    lua=LuaRuntime();lua.execute(recover()[0]);resources=lua.table();events=[]
                    resources.NewThingFromResource=lambda _,control:events.append(('thing.new',empty)) or 7
                    resources.ThingHealth=lambda _,actor:events.append(('health',empty)) or value
                    resources.DestroyThing=lambda _,actor:events.append(('thing.destroy',))
                    result=lua.globals().BullyControlledHealthAboveThreshold(resources,1)
                    self.assertEqual((result,events),native_health(site,value,empty))

    def test_original_home_movement_polling_and_cancellation(self):
        for moves in (0,1,2):
            for busy in (0,2):
                for cancel in (1,2,3,4,999):
                    lua=LuaRuntime();lua.execute(recover()[0]);q=lua.table();me=lua.table();r=lua.table();events=[]
                    state={'moves':0,'busy':0,'queries':0};home=(7.0,-3.0,11.0)
                    me.GetHomePos=lambda _:events.append(('home',home)) or lua.table_from(home)
                    r.NewThingFromResource=lambda _,c:events.append(('thing.new',)) or 7
                    r.ThingIsDistanceFromPositionOver=lambda _,a,p,d:events.append(('outside',state['moves']<moves)) or state['moves']<moves
                    r.DestroyThing=lambda _,a:events.append(('thing.destroy',))
                    q.NewScriptFrame=lambda _:events.append(('frame',))
                    def term(_):
                        state['queries']+=1;value=state['queries']>=cancel;events.append(('term',value));return value
                    q.IsActiveThreadTerminating=term
                    def move(_,control,position,speed,kind,a,b):
                        self.assertEqual(tuple(position.values()),home);events.append(('move',home,speed,kind,a,b));state['moves']+=1;state['busy']=busy
                    r.MoveToPosition=move
                    def task(_,control):
                        value=state['busy']>0;state['busy']-=1;events.append(('busy',value));return value
                    r.IsPerformingScriptTask=task
                    result=lua.globals().BullyReturnHomePhase(q,me,r,1)
                    if not result:events.append(('resource.destroy',))
                    self.assertEqual((result,events),native_home(moves,busy,cancel,home))

    def test_changed_native_health_home_and_flags_rejected(self):
        class Changed(RData):
            def bytes_at(self,address,size):
                raw=super().bytes_at(address,size)
                if address<=self.changed<address+size:
                    raw=bytearray(raw);raw[self.changed-address]^=1;return bytes(raw)
                return raw
        for address in (0xdbb403,0xdbb411,0xdbb41e,0xdbb429,0xdbb489,0xdbb497,0xdbb737,0xdbb740,0xdbb74e,0x122dedc):
            d=Changed();d.changed=address
            with self.assertRaises(ValueError):recover(d)
