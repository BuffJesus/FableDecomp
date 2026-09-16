import itertools,unittest
from lupa import LuaRuntime
from tools.script_recovery.victim_hit import SOURCE,generate,recover
from tools.script_recovery.victim_hit_native import execute
from tools.script_recovery.lift_native_lua import RData

def lua_case(answers=(True,False,False),given=False,cancel=999,self_failures=0,hero_failures=0,prepare=False,populated=True,mask=256,fault=None,cleanup_fault=False):
    lua=LuaRuntime();lua.execute(SOURCE);q=lua.table();r=lua.table();events=[];state={'queries':0,'selfAttempts':0,'heroAttempts':0,'GivenHeroTeddy':given,'HeroAttackedVictim':False}
    def name(id):assert id in (1,3);return 'self' if id==1 else 'hero'
    def predicate(_,actor,excluded):
        assert (actor,excluded)==(7,14);opened=[]
        def test(i):events.extend([('text.new','SCRIPT_NAME_HERO'),('predicate',i,answers[i])]);opened.append(i);return answers[i]
        value=test(0) or (test(1) and not test(2))
        for _ in reversed(opened):events.append(('text.destroy','SCRIPT_NAME_HERO'))
        return value
    def term(_):state['queries']+=1;value=state['queries']>=cancel;events.append(('term',value));return value
    def put(_,key,value):state[key]=value;events.append(('set',key,value))
    def prepare_fn(_,id):
        which=name(id);events.append(('prepare',which,prepare))
        if prepare:events.append(('prepare.release',which))
    def acquire(_,id,actor,priority):
        which=name(id);assert actor==(7 if which=='self' else 8) and priority==4;key=which+'Attempts';value=state[key]>=(self_failures if which=='self' else hero_failures);state[key]+=1;events.append(('acquire',which,value,populated));return value
    def set_actor(_,actors,key,id):assert actors==4;events.extend([('text.new',key),('map.set',key,name(id),populated),('text.destroy',key)])
    def macro(_,key,actors,setup,skip):assert (key,actors,setup,skip)==('CS_OAKVALEINTRO_BRATHIT',4,False,True);events.extend([('text.new',key),('macro',),('text.destroy',key)])
    q.IsActiveThreadTerminating=term;q.GetHero=lambda _:events.append(('hero',)) or 8;q.NewScriptFrame=lambda _:events.append(('frame',));q.SetStateBool=put;q.GetStateBool=lambda _,key:events.append(('get',key,state[key])) or state[key]
    q.PauseAllNonScriptedEntities=lambda _,v:events.append(('pause',v));q.FixMovieSequenceCamera=lambda _,v:events.append(('camera',v))
    r.IsHitByHeroExceptAbility=predicate;r.SetThingAsAlly=lambda _,a,b:events.append(('ally','victim' if a==7 else 'hero','victim' if b==7 else 'hero'))
    r.PrepareResource=prepare_fn;r.TryAcquire=acquire;r.NewResource=lambda _:events.append(('resource.new','hero')) or 3;r.ReleaseResource=lambda _,id:events.append(('resource.destroy',name(id)))
    r.NewActorMap=lambda _:events.append(('map.new',)) or 4;r.SetActor=set_actor;r.DestroyActorMap=lambda _,id:events.append(('map.destroy',))
    r.NewMovie=lambda _:events.append(('movie.new',)) or 5;r.StartOwnedMovie=lambda _,id,key:events.extend([('text.new',key),('movie.start',),('text.destroy',key)]);r.DestroyMovie=lambda _,id:events.append(('movie.destroy',));r.RunMacro=macro
    r.ClearRawInformation=lambda _,actor:events.append(('clear',));r.FaceTowardsRetainedThing=lambda _,actor,bully,snap:events.append(('face','bully',snap))
    wrap=lua.eval('function(f,key) return function(...) f(...); error(key,0) end end')
    if fault:r[fault]=wrap(r[fault],'BODY ERROR')
    if cleanup_fault:r.DestroyMovie=wrap(r.DestroyMovie,'CLEANUP ERROR')
    try:result=lua.globals().VictimBeginHit(q,7,r,1,2,lambda amount:events.append(('badDeed',amount)))
    except Exception as error:result=str(error)
    return result,events,state['GivenHeroTeddy'],state['HeroAttackedVictim']

class VictimHitTests(unittest.TestCase):
    def test_original_hit_flags_macro_args_and_owned_resource_cancel_paths(self):
        generate();cases=0
        for args in itertools.product(tuple(itertools.product((False,True),repeat=3)),(False,True),(1,2,3,4,5,6,7,999),(0,2),(0,2),(False,True),(False,True)):
            with self.subTest(args=args):self.assertEqual(execute(*args),lua_case(*args));cases+=1
        self.assertEqual(cases,2048)
    def test_callback_errors_close_camera_movie_map_hero_and_preserve_flags(self):
        for fault,cleanup in itertools.product(('RunMacro','ClearRawInformation','StartOwnedMovie'),(False,True)):
            result,events,given,attacked=lua_case(fault=fault,cleanup_fault=cleanup)
            self.assertIn('BODY ERROR',result);self.assertTrue(given and attacked)
            self.assertEqual(events[-3:],[('movie.destroy',),('map.destroy',),('resource.destroy','hero')])
            if fault=='RunMacro':self.assertEqual(events[-5:-3],[('camera',False),('pause',False)])
            if fault=='StartOwnedMovie':self.assertFalse(any(e[0]=='pause' for e in events))
    def test_changed_flags_state_mask_or_macro_literal_rejected(self):
        original=RData()
        class Changed:
            def __init__(self,at):self.at=at
            def bytes_at(self,at,size):
                raw=bytearray(original.bytes_at(at,size))
                if at<=self.at<at+size:raw[self.at-at]^=1
                return bytes(raw)
            def string_at(self,at):return 'CHANGED' if at==self.at else original.string_at(at)
        for at in (0xdbd684,0xdbd735,0xdbd75d,0xdbd94b,0xdbd94d,0xdbd947,0x12d9bf0,0x12d99e8,0x122d70e,0x1260f0c+0x95c):
            with self.subTest(at=at),self.assertRaises(ValueError):recover(Changed(at))

if __name__=='__main__':unittest.main()
