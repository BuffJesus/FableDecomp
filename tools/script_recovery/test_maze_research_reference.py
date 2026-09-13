"""Reference Lua compared with native-instruction traces, engine operations doubled."""
import json
from pathlib import Path
import unittest
from lupa import LuaRuntime

ROOT = Path(__file__).resolve().parents[2]
PORT = ROOT / 'refs/script_recovery/maze_research'
STAGED = ROOT / 'refs/script_recovery/reconstructed/MazeResearch/FSE'

def staged_bridge(lua, h, entry):
    """Drive the actual entry script and adapter against the same native oracle."""
    quest, resources, me, flags = (lua.table() for _ in range(4))
    direct = {'GetStateBool':'get_bool','SetStateBool':'set_bool','GetActiveQuestName':'active_quest',
              'SetQuestCardObjective':'objective','IsActiveThreadTerminating':'terminating',
              'SetThingAsUsable':'usable','MiniMapAddMarker':'add_marker','MiniMapRemoveMarker':'remove_marker',
              'GetThingWithScriptName':'lookup','DisplayGameInfo':'display','MsgIsGameInfoClickedPast':'clicked',
              'GetMasterGameState':'master','EntitySetInLimbo':'limbo','EntitySetAlpha':'alpha',
              'GetHero':'hero','SetCutsceneSkippable':'skippable','DeactivateQuestLater':'deactivate'}
    for api, name in direct.items():
        def forward(_, *args, name=name):
            args=tuple('me' if lua.eval('rawequal')(value,me) else value for value in args)
            return h[name](h,*args)
        quest[api]=forward
    for api,name in {'Frame':'frame','NewResource':'new_resource','NewActorMap':'new_map',
                     'SetActor':'map_actor','StartMovie':'start_movie','Pause':'pause',
                     'DestroyMovie':'destroy_movie','DestroyActorMap':'destroy_map','ReleaseResource':'release'}.items():
        resources[api]=lambda _,*args,name=name:h[name](h,*args)
    resources.TryAcquire=lambda _,resource,actor,priority:h.acquire(h,actor,resource,priority)
    resources.RunMacroWithFlags=lambda _,name,actors,passed,setup,skip:h.run_macro(h,name,actors,setup,skip)
    quest.RetainRetailThing=lambda _,key,sword:h.retain_sword(h,sword)
    quest.RetailFlags=lambda _,key:flags
    quest.GetRetainedRetailThing=lambda _,key:'sword'
    flags.Get=lambda _,name:h.flag(h,'flags',name)
    def spawn(_,name,args):
        assert name=='UnLimboSword' and args['region']=='' and args['args'] is None
        h.spawn_unlimbo(h)
    quest.CreateThread=spawn
    quest.WithRetailResources=lambda _,callback:callback(resources)
    me.MsgIsUsedByHero=lambda _:h.used(h,'me')
    flags.Set=lambda _,name,value:None
    lua.globals().package.path=STAGED.as_posix()+'/?.lua;'+lua.globals().package.path
    if entry=='unlimbo':
        lua.execute((STAGED/'MazeResearch/MazeResearch.lua').read_text())
        lua.globals().UnLimboSword(quest)
    else:
        lua.execute((STAGED/'MazeResearch/Entities'/('HistoryBookcase.lua' if entry=='history' else 'EmptyGrave.lua')).read_text())
        lua.globals().Init(quest,me)
        lua.globals().Main(quest,me)

class MazeResearchReferenceTests(unittest.TestCase):
    def test_history_native_traces(self):
        proof = json.loads((PORT/'runtime_evidence/native-history-cases-20260912.json').read_text())
        for case in proof['cases']:
            with self.subTest(read=case['book_read'], used=case['used'], clicked=case['clicked'], stop=case['stop_at_query']):
                lua = LuaRuntime(unpack_returned_tuples=True)
                q = lua.execute((PORT/'reference/quest.lua').read_text())
                events = []
                state = {'BookRead':case['book_read'], 'queries':0}
                h = lua.table()
                h.active_quest = lambda _: 'MazeResearch'
                h.get_bool = lambda _, key: state[key]
                h.set_bool = lambda _, key, value: state.__setitem__(key,value)
                def terminating(_):
                    state['queries'] += 1
                    value = state['queries'] >= case['stop_at_query']
                    events.append(['term',value])
                    return value
                h.terminating = terminating
                def operation(name, result=None):
                    def call(_, *args):
                        events.append([name,*args] if result is None else [name,result])
                        return result
                    return call
                for name in ('usable','add_marker','remove_marker','objective','display','frame'):
                    h[name] = operation(name)
                h.used = lambda _, me: (events.append(['used',case['used']]),case['used'])[1]
                h.clicked = operation('clicked',case['clicked'])
                h.lookup = lambda _, name: (events.append(['lookup',name]),name)[1]
                if getattr(self,'staged',False): staged_bridge(lua,h,'history')
                else: q.history(h,'me')
                self.assertEqual(events,case['events'])
                self.assertEqual(state['BookRead'],case['final_book_read'])

    def test_grave_native_traces(self):
        proof=json.loads((PORT/'runtime_evidence/native-grave-cases-20260912.json').read_text())
        for case in proof['cases']:
            with self.subTest(options=case['options']):
                lua=LuaRuntime(unpack_returned_tuples=True)
                q=lua.execute((PORT/'reference/quest.lua').read_text())
                opt=case['options']; events=[]
                state={'BookRead':opt['read'],'SwordTaken':opt['taken'],'queries':0,'macro':False}
                h=lua.table()
                h.get_bool=lambda _,key:state[key]
                h.set_bool=lambda _,key,val:state.__setitem__(key,val)
                h.active_quest=lambda _:'MazeResearch'
                h.master=lambda _,key:{'PostSavePosition':opt['position'],'JackBossBattleResult':opt['result'],'JackBossBattleHeroGoodAtEnd':opt['good']}[key]
                def term(_):
                    state['queries']+=1
                    value=state['queries']>=opt['stop_at'] or (state['macro'] and opt['macro_cancel'])
                    events.append(['term',value]);return value
                h.terminating=term
                def emit(name,*args): events.append([name,*args])
                h.add_marker=lambda _,me,marker:emit('marker.add',marker)
                h.remove_marker=lambda _,me:emit('marker.remove')
                h.lookup=lambda _,name:(emit('lookup',name),'sword')[1]
                h.retain_sword=lambda *_:None
                h.limbo=lambda _,sword,a,b:emit('limbo',a,b)
                h.used=lambda _,me:(emit('used',opt['used']),opt['used'])[1]
                h.frame=lambda _:emit('frame')
                h.new_resource=lambda _:(emit('resource.new'),'resource')[1]
                h.hero=lambda _:(emit('hero'),'hero')[1]
                h.acquire=lambda _,hero,res,pri:emit('acquire',pri)
                h.new_map=lambda _:(emit('map.new'),'map')[1]
                h.map_actor=lambda _,actors,name,res:emit('map.actor',name)
                def start(_,name):emit('movie.new');emit('movie.start',name);return 'movie'
                h.start_movie=start
                h.pause=lambda _,value:emit('pause',value)
                h.skippable=lambda _,value:emit('skippable',value)
                h.spawn_unlimbo=lambda _:emit('spawn','UnLimboSword','')
                def macro(_,name,actors,setup,skip):emit('macro',name,setup,skip);state['macro']=True
                h.run_macro=macro
                h.destroy_movie=lambda _,movie:emit('movie.destroy')
                h.destroy_map=lambda _,actors:emit('map.destroy')
                h.release=lambda _,res:emit('resource.release')
                h.deactivate=lambda _,name,delay:emit('deactivate',name,delay)
                if getattr(self,'staged',False): staged_bridge(lua,h,'grave')
                else: q.grave(h,'me')
                self.assertEqual(events,case['events'])
                self.assertEqual(state['SwordTaken'],case['final_sword_taken'])

    def test_unlimbo_native_outcomes(self):
        proof = json.loads((PORT/'runtime_evidence/native-unlimbo-task-20260912.json').read_text())
        for case in proof['cases'][1:]:
            with self.subTest(name=case['name']):
                lua = LuaRuntime(unpack_returned_tuples=True)
                q = lua.execute((PORT/'reference/quest.lua').read_text())
                state = dict(flag=case['initial_flag'],term=case['initial_terminated'],frames=0)
                events=[]
                h=lua.table()
                h.flag=lambda *_:bool(state['flag'])
                h.terminating=lambda *_:bool(state['term'])
                def frame(_):
                    flag,term=case['changes'][state['frames']]
                    state.update(flag=flag,term=term,frames=state['frames']+1)
                    events.append(['frame',flag,term])
                h.frame=frame
                h.limbo=lambda _,sword,value,extra:events.append(['limbo',int(value),extra])
                h.alpha=lambda _,sword,value,extra:events.append(['alpha',value,extra])
                if getattr(self,'staged',False): staged_bridge(lua,h,'unlimbo')
                else: q.unlimbo(h,'flags','sword')
                self.assertEqual(events,case['events'])

class MazeResearchStagedTests(MazeResearchReferenceTests):
    staged=True

    def test_reference_copies(self):
        for name in ('quest.lua','runtime_adapter.lua'):
            self.assertEqual((PORT/'reference'/name).read_bytes(),(STAGED/'MazeResearch'/name).read_bytes())

    def test_activation_off(self):
        lua=LuaRuntime()
        lua.execute((STAGED/'retail_override.lua').read_text())
        config=lua.globals().RetailOverrides
        self.assertFalse(config.enabled)
        self.assertFalse(config.allowUnverifiedDisposable)
        self.assertEqual(config.entries[1].nativeName,'V_MazeResearch')

if __name__ == '__main__':
    unittest.main()
