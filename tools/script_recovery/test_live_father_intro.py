import itertools,unittest
from lupa import LuaRuntime
from tools.script_recovery.live_father_candidate import BODY
from tools.script_recovery.live_father_intro_native import execute

def lua_case(xbox=False,clicked_after=0,cancel=999,failures=0,populated=True,counter=-1,fault=None):
    lua=LuaRuntime();lua.execute(BODY);q=lua.table();r=lua.table();events=[];state={'queries':0,'acquires':0,'clicks':0};live=[]
    def term(_):state['queries']+=1;value=state['queries']>=cancel;events.append(('term',value));return value
    def acquire(_,control,actor,priority):
        assert (control,actor,priority)==(2,8,4);value=state['acquires']>=failures;state['acquires']+=1;events.append(('acquire',value,populated));return value
    def new(name,id):events.append((name+'.new',));live.append((name,id));return id
    def destroy(name,id):live.remove((name,id));events.append((name+'.destroy',))
    def actor(_,actors,key,control):assert actors==3;events.extend([('text.new',key),('map.set',key,'hero' if control==2 else 'control'),('text.destroy',key)])
    def macro(_,name,actors,setup,skip):assert (name,actors,setup,skip)==('CS_OAKVALE_INTRO_FATHER',3,False,True);events.extend([('text.new',name),('macro',),('text.destroy',name)])
    def assert_cutscene(name,actors,flags,setup,skip,fix):
        assert (name,actors,flags,setup,skip,fix)==('CS_OAKVALE_INTRO_FATHER',3,None,False,True,False)
    def clicked(_):value=state['clicks']>=clicked_after;state['clicks']+=1;events.append(('clicked',value));return value
    def text_new(_,key):assert key=='HUD_DEED_GOOD_ICON';events.append(('text.new',key));live.append(('text',5));return 5
    def text_destroy(_,key):assert key==5;live.remove(('text',5));events.append(('text.destroy','HUD_DEED_GOOD_ICON'))
    q.IsActiveThreadTerminating=term;q.NewScriptFrame=lambda _:events.append(('frame',));q.GetHero=lambda _:events.append(('hero',)) or 8
    q.PauseAllNonScriptedEntities=lambda _,v:events.append(('pause',v));q.FixMovieSequenceCamera=lambda _,v:events.append(('camera',v));q.SetAllSoundsAsMuted=lambda _,v:events.append(('mute',v))
    q.SetStateBool=lambda _,key,v:events.append(('setBool',key,v));q.SetStateInt=lambda _,key,v:events.append(('setInt',key,v))
    def reset(_,v):assert type(v) is float and v==0.0;events.append(('reset',v))
    q.Pause=lambda _,v:events.append(('seconds',v));q.CameraResetToViewBehindHero=reset;q.CameraDefault=lambda _:events.append(('default',))
    q.IsXbox=lambda _:events.append(('xbox',xbox)) or xbox;q.MsgIsGameInfoClickedPast=clicked;q.DisplayQuestInfo=lambda _,v:events.append(('displayQuest',v))
    r.NewResource=lambda _:new('hero',2);r.ReleaseResource=lambda _,id:destroy('hero',id);r.TryAcquire=acquire
    r.NewActorMap=lambda _:new('map',3);r.SetActor=actor;r.DestroyActorMap=lambda _,id:destroy('map',id)
    r.NewMovie=lambda _:new('movie',4);r.DestroyMovie=lambda _,id:destroy('movie',id);r.StartOwnedMovie=lambda _,id,key:events.extend([('text.new',key),('movie.start',),('text.destroy',key)])
    q.RunCutsceneWithSetup=lambda _,name,actors,flags,setup,skip,fix: (assert_cutscene(name,actors,flags,setup,skip,fix), events.extend([('text.new',name),('macro',),('text.destroy',name)]))[1]
    r.DisplayRawGameInfo=lambda _,key:events.extend([('text.new',key),('info',key),('text.destroy',key)])
    r.NewLiteralText=text_new;r.DestroyText=text_destroy;r.AddLiveFatherGoodDeedCounter=lambda _,id:events.append(('counter',counter)) or counter
    wrap=lua.eval('function(f) return function(...) f(...); error("BODY ERROR",0) end end')
    if fault=='state':q.SetStateInt=wrap(q.SetStateInt)
    if fault=='macro':q.RunCutsceneWithSetup=wrap(q.RunCutsceneWithSetup)
    error=None
    try:complete=lua.globals().LiveFatherIntro(q,7,r,1)
    except Exception as e:complete=False;error=str(e)
    for name,id in list(reversed(live)):destroy(name,id)
    if not complete:events.append(('control.destroy',))
    return complete,events,error

class LiveFatherIntroTests(unittest.TestCase):
    def test_original_intro_map_macro_state_counter_and_all_cancel_joins(self):
        cases=0
        for args in itertools.product((False,True),(0,2),(1,2,3,4,5,6,7,999),(0,2),(False,True),(-2147483648,-1,0,2147483647)):
            with self.subTest(args=args):self.assertEqual(execute(*args),lua_case(*args)[:2]);cases+=1
        self.assertEqual(cases,512)
    def test_error_closes_camera_and_hud_text_before_enclosing_movie(self):
        for fault in ('macro','state'):
            complete,events,error=lua_case(fault=fault)
            self.assertIn('BODY ERROR',error);self.assertFalse(complete)
            self.assertEqual(events[-5:],[('pause',False),('movie.destroy',),('map.destroy',),('hero.destroy',),('control.destroy',)])
            if fault=='macro':self.assertEqual(events[-6],('camera',False))
            else:self.assertEqual(events[-6],('text.destroy','HUD_DEED_GOOD_ICON'))

if __name__=='__main__':unittest.main()
