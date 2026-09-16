"""Original late acquisition instructions and full candidate differential traces."""
import itertools,unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.bully_main_structure import generate,lower,ACQUIRE
from tools.script_recovery.bully_item_structure import generate as original
from tools.script_recovery.test_bully_item_structure import run
from tools.script_recovery.test_bully_readable_integration import run as complete
from tools.script_recovery.bully_runoff_controls_native import execute as native_controls
from tools.script_recovery.bully_subdual_retry_native import execute as native_self


class BullyMainStructureTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):cls.before,_=original();cls.after,cls.report=generate()

    def test_whole_item_and_question_composition(self):
        for mode,health,cancel,answer,delay in itertools.product(('teddy','other','question'),
                (1.0,0.0,float('nan')),range(1,36),(-2,0,1,2),(0,2)):
            args=(mode,health,cancel,answer,delay,'')
            self.assertEqual(run(self.before,*args),run(self.after,*args),args)

    def test_whole_four_hits_and_runoff_cancellation_and_macro_errors(self):
        for mode,cancel in itertools.product(('normal','macro_error'),range(1,100)):
            self.assertEqual(complete(self.before,mode,1.0,cancel,True),complete(self.after,mode,1.0,cancel,True),(mode,cancel))

    def test_scoped_callback_errors_preserve_primary_fault_and_cleanup(self):
        for mode,fault in itertools.product(('teddy','other','question'),
                ('Speak','ThingHealth','GetHero','GiveBullyTeddyQuestion','ClearThingHasInformation','PollPresentedItem','PresentedItemMatches')):
            args=(mode,1.0,999,1,2,fault)
            self.assertEqual(run(self.before,*args),run(self.after,*args),args)

    def test_original_self_retry_and_late_control_cleanup(self):
        for hero_fail,victim_fail,cancel in itertools.product((0,1,3),(0,1,3),range(1,12)):
            lua=LuaRuntime();q,r=lua.table(),lua.table();events=[]
            state={'queries':0,'heroes':0,'new':0,'hero':0,'victim':0};live=set()
            def term(_):
                state['queries']+=1;value=state['queries']>=cancel;events.append(('term',value));return value
            q.IsActiveThreadTerminating=term;q.NewScriptFrame=lambda _,me:events.append(('frame',))
            def hero(_):state['heroes']+=1;events.append(('hero',state['heroes']));return state['heroes']
            q.GetHero=hero
            def new(_):
                name='hero' if state['new']==0 else 'victim';state['new']+=1;live.add(name);events.append(('new',name));return name
            r.NewResource=new;r.PrepareResource=lambda _,name:events.append(('prepare',name))
            def acquire(_,name,target,priority):
                self.assertIn(name,live);self.assertEqual(priority,4)
                self.assertEqual(target,state['heroes'] if name=='hero' else 'retained')
                state[name]+=1;value=state[name]>(hero_fail if name=='hero' else victim_fail)
                events.append(('acquire',name,value));return value
            r.TryAcquire=acquire;r.TryAcquireThing=acquire
            def release(_,name):live.remove(name);events.append(('destroy',name))
            r.ReleaseResource=release
            lua.execute(ACQUIRE)
            lua.globals().BullyRunoffMovie=lambda *args:events.append(('continuation',)) or False
            lua.globals().BullyRunoffControls(q,r,'me','self','retained')
            self.assertEqual(events,native_controls(hero_fail,victim_fail,cancel))
            self.assertFalse(live)
        for failures,cancel in itertools.product((0,1,3),range(1,7)):
            lua=LuaRuntime();q=lua.table();events=[('prepare','self')];state={'queries':0,'tries':0}
            def term(_):state['queries']+=1;v=state['queries']>=cancel;events.append(('term',v));return v
            def acquire():state['tries']+=1;v=state['tries']>failures;events.append(('acquire','self',v));return v
            q.IsActiveThreadTerminating=term;q.NewScriptFrame=lambda _,me:events.append(('frame',))
            success=lua.execute(ACQUIRE+'return BullyAcquirePrepared(...)',q,'me',acquire)
            self.assertEqual((success,events),native_self(failures,cancel))

    def test_no_remaining_gotos_todos_and_fail_closed_source(self):
        self.assertNotIn('goto ',self.after)
        self.assertNotIn('TODO(native)',self.after)
        self.assertNotIn('::LAB_',self.after)
        self.assertEqual(self.after.count('quest:RegisterBoundConsciousCondition()'),1)
        with self.assertRaises(ValueError):lower(self.before.replace('iVar8 + 1','iVar8 + 2'))


if __name__=='__main__':unittest.main()
