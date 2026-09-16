import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.rock_actor_candidate import generate
from tools.script_recovery.lift_native_lua import ROOT


def run(added=False,played=False,cancel=100,hero_failures=0,macro_cancel=False,error=False):
    report=generate();out=ROOT/'work/rock_trigger_converter/actor_candidate'
    lua=LuaRuntime();lua.execute((out/'RockTrollFirstEncounter/Entities/RTFE_RockTroll.lua').read_text())
    q,r,me=lua.table(),lua.table(),lua.table();events=[];entries={};counter=0
    state={'AddedItemsToRockTroll':added,'PlayedExhumeCutScene':played,'queries':0,'heroQueries':0,'heroAttempts':0,'frames':0,'targetFrame':None,'paused':False,'macroDone':False}
    def thread(_,name,actor,flag,region):assert lua.eval('rawequal')(actor,me) and flag is False and region=='';events.append(('thread',name))
    q.CreateRetainedThingThread=thread
    q.GetStateBool=lambda _,name:state[name]
    def setter(_,name,value):state[name]=value;events.append(('state',name,value))
    q.SetStateBool=setter
    def term(_):
        state['queries']+=1;value=state['queries']>=cancel or (macro_cancel and state['macroDone']) or (state['targetFrame'] is not None and state['frames']>state['targetFrame'])
        events.append(('term',bool(value)));return bool(value)
    q.IsActiveThreadTerminating=term
    def frame(_):state['frames']+=1;events.append(('frame',))
    q.NewScriptFrame=frame
    def hero(_):state['heroQueries']+=1;events.append(('hero',state['heroQueries']));return 'hero'+str(state['heroQueries'])
    q.GetHero=hero
    def add(_,actor,index):assert entries[1]['control'] and lua.eval('rawequal')(actor,me);events.append(('reward',index))
    q.AddRockTrollReward=add
    def create(kind):
        nonlocal counter
        counter+=1;entries[counter]={'kind':kind,'control':False};events.append(('new',kind,counter));return counter
    r.NewResource=lambda _:create('resource')
    r.NewActorMap=lambda _:create('map')
    def prepare(_,id):
        assert id in entries
        if entries[id]['control']:entries[id]['control']=False;events.append(('release.control',id))
    r.PrepareResource=prepare
    def acquire(_,id,actor,priority):
        assert priority==4
        if lua.eval('rawequal')(actor,me):value=True
        else:state['heroAttempts']+=1;value=state['heroAttempts']>hero_failures
        entries[id]['control']=True;events.append(('acquire',id,value));return value
    r.TryAcquire=acquire
    def destroy(_,id):
        value=entries.pop(id);events.append(('destroy',value['kind'],id,value['control']))
    r.ReleaseResource=destroy;r.DestroyMovie=destroy;r.DestroyActorMap=destroy
    r.SetActor=lambda _,map,name,resource:events.append(('map.actor',name,resource))
    def movie(_,name):assert name=='';return create('movie')
    r.StartMovie=movie
    def pause(_,flag):state['paused']=flag;events.append(('pause',flag))
    r.Pause=pause
    def macro(_,name,map,setup,skip):
        assert name=='CS_ROCKTROLL_EXHUME' and not setup and skip and state['paused']
        events.append(('macro',));state['macroDone']=True
        if error:raise RuntimeError('macro failure')
    r.RunMacro=macro
    def animation(_,id,name):assert entries[id]['control'];events.append(('animation',name))
    r.PlayRockTrollAnimation=animation
    r.IsPerformingScriptTask=lambda _,id:False
    def targeting(_,actor):
        assert lua.eval('rawequal')(actor,me) and len(entries)==1 and not entries[1]['control']
        first=hero(None);events.append(('look',first));second=hero(None);events.append(('enemy',second));state['targetFrame']=state['frames']
    q.TargetRockTrollAtHero=targeting
    def scope(_,callback):
        try:callback(r)
        finally:
            if state['paused']:pause(None,False)
            for id in list(reversed(entries)):destroy(None,id)
    q.WithRetailResources=scope
    try:lua.globals().Main(q,me)
    except RuntimeError:
        if not error:raise
    assert not entries and not state['paused'];return events,state,report


class ActorCandidateTests(unittest.TestCase):
    def test_full_native_phase_order_and_distinct_hero_queries(self):
        events,state,report=run(hero_failures=1)
        self.assertEqual(events[0],('thread','WatchForRockTrollKilled'))
        self.assertEqual([e for e in events if e[0]=='reward'],[('reward',1),('reward',2)])
        self.assertEqual([e for e in events if e[0]=='thread'],[('thread','WatchForRockTrollKilled'),('thread','WatchForRockTrollHit')])
        self.assertEqual([e for e in events if e[0]=='animation'],[('animation','SPECIAL_BOAST'),('animation','SPECIAL_IDLE')])
        self.assertEqual(events[-7:],[('hero',3),('look','hero3'),('hero',4),('enemy','hero4'),('frame',),('term',True),('destroy','resource',1,False)])
        self.assertTrue(state['AddedItemsToRockTroll'] and state['PlayedExhumeCutScene'])
        self.assertEqual(report['remainingMainFunctions'],[])
        self.assertEqual(report['missingFunctions'],[]);self.assertFalse(report['gameplayComplete'])

    def test_saved_flags_skip_only_their_native_phases(self):
        for added,played in ((True,False),(False,True),(True,True)):
            events,state,_=run(added,played)
            self.assertEqual(any(e[0]=='reward' for e in events),not added)
            self.assertEqual(any(e[0]=='macro' for e in events),not played)
            self.assertEqual(sum(e[0]=='animation' for e in events),2)

    def test_cancellation_boundaries_failed_output_and_macro_cancel(self):
        for stop in (1,2,3,4,5):
            events,_,_=run(cancel=stop,hero_failures=1)
            self.assertFalse(any(e[0]=='enemy' for e in events))
            self.assertEqual(events[-1][0:3],('destroy','resource',1))
        events,state,_=run(macro_cancel=True)
        self.assertTrue(state['PlayedExhumeCutScene'])
        self.assertEqual(sum(e[0]=='animation' for e in events),2)
        self.assertFalse(any(e[0]=='look' for e in events))
        events,state,_=run(error=True)
        self.assertFalse(state['PlayedExhumeCutScene'])
        self.assertEqual([e[1] for e in events[-4:]],['movie','map','resource','resource'])

    def test_isolated_embedded_phases_and_explicit_runtime_lifecycle_gaps(self):
        report=generate();self.assertFalse(report['enabled'])
        text=(ROOT/'work/rock_trigger_converter/actor_candidate/RockTrollFirstEncounter/Entities/RTFE_RockTroll.lua').read_text()
        self.assertIn('function WithRockTrollHeroPhase',text);self.assertIn('function Main(quest, me)',text)
        self.assertTrue(any('destructor' in gap for gap in report['converterGaps']))
        self.assertTrue(any('TrollAwake' in gap for gap in report['converterGaps']))
        with self.assertRaises(ValueError):generate(ROOT/'refs/no-actor-output')


if __name__=='__main__':unittest.main()
