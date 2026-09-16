import itertools,unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.affair_man_complete import generate
from tools.script_recovery.affair_man_complete_dispatcher_native import execute,prove
from tools.script_recovery.native_new_oakvale_conditions import recover as condition
from tools.script_recovery.lift_native_lua import RData

def instrument(source):
    source,evidence=condition('NOVI_AffairMan',source)
    if not evidence:raise ValueError('AffairMan dispatcher requires builder-owned native condition')
    a=source.index('                                man_movie = resources:StartMovie("")',source.index('local function processInteraction()'))
    b=source.index('                            talkedToByHero = me:IsTalkedToByHero()',a)
    source=source[:a]+'''                                return phaseHit()
                            end
'''+source[b:]
    a=source.index('                                man_movie = resources:StartMovie("")',a)
    b=source.index('                            taskRunning17 = resources:IsPerformingScriptTask(man_resource)',a)
    source=source[:a]+'''                                return phaseTalk()
                            end
'''+source[b:]
    a=source.index('                            thingsWithinDistance2 = resources:ThingsAreWithinDistance(me, affairWife, 5.0)')
    b=source.index('                        end\n                        while true do',a)
    return source[:a]+'                            return phaseIdle()\n'+source[b:]

def run(source,cancel=8,failures=0,hit=False,talk=False,busy=False,phases=(True,True,True),prepare=False,populated=True,actors=(True,True)):
    lua=LuaRuntime();q,r,me=lua.table(),lua.table(),lua.table();events=[];state={'queries':0,'acquires':0}
    def event(*args):events.append(args)
    def term(_):state['queries']+=1;v=state['queries']>=cancel;event('term',v);return v
    def acquire(_,control,actor,priority):
        assert control=='control' and actor is not None and priority==4
        v=state['acquires']>=failures;state['acquires']+=1;event('acquire',v,populated);return v
    def lookup(_,key):
        index=('NOVI_AffairWoman','NOVI_AffairWife').index(key);event('key.new',key);event('lookup',key,actors[index]);event('key.destroy',key);return ('woman','wife')[index]
    def prep(_,id):event('prepare',prepare);event('prepare.release') if prepare else None
    q.WithRetailResources=lambda _,body:body(r);q.RegisterBoundConsciousCondition=lambda _:event('condition')
    q.NewScriptFrame=lambda *args:event('frame');q.IsActiveThreadTerminating=term
    r.NewResource=lambda _:event('control.new') or 'control';r.PrepareResource=prep;r.TryAcquire=acquire
    r.NewThingFromScriptName=lookup;r.ReleaseResource=lambda _,id:event('control.destroy');r.DestroyThing=lambda _,id:event('destroy',id)
    r.IsHitByHeroExceptAbility=lambda _,actor,ability:event('hit',hit) or hit
    r.IsPerformingScriptTask=lambda _,id:event('task',busy) or busy
    me.IsTalkedToByHero=lambda _:event('talk',talk) or talk
    for name,event_name,value in zip(('phaseHit','phaseTalk','phaseIdle'),('hit.body','talk.body','idle.body'),phases):
        lua.globals()[name]=lambda name=event_name,value=value:event(name,value) or value
    lua.execute(source);lua.globals().Main(q,me);return events

class AffairManDispatcherTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):cls.source,cls.report=generate();cls.body=instrument(cls.source)

    def test_original_outer_branches_and_cleanup_joins(self):
        for cancel,failures,hit,talk,busy,phases in itertools.product(range(1,16),(0,2),(False,True),(False,True),(False,True),itertools.product((False,True),repeat=3)):
            args=(cancel,failures,hit,talk,busy,phases)
            self.assertEqual(run(self.body,*args),execute(*args),args)

    def test_empty_outputs_prepare_release_and_actor_cleanup_order(self):
        for cancel,prepare,populated,actors in itertools.product((1,2,3,4,6,9),(False,True),(False,True),itertools.product((False,True),repeat=2)):
            args=dict(cancel=cancel,failures=2,prepare=prepare,populated=populated,actors=actors)
            self.assertEqual(run(self.body,**args),execute(**args),args)

    def test_changed_entry_branch_and_destructor_targets_rejected(self):
        class Changed(RData):
            def bytes_at(self,address,size):
                raw=super().bytes_at(address,size)
                if address<=self.pc<address+size:raw=bytearray(raw);raw[self.pc-address]^=1;return bytes(raw)
                return raw
        for pc in (0xdb0a03,0xdb0a32,0xdb0a91,0xdb0b07,0xdb0b23,0xdb0c1c,0xdb0dfa,0xdb160d,0xdb1c82,0xdb1d80,0xdb1d89):
            d=Changed();d.pc=pc
            with self.assertRaises(ValueError):prove(d)
        banner=self.source.split('\n\n',1)[0]
        self.assertIn('InitializeAffairManActor',banner);self.assertIn('PlayAffairManAnimation',banner)
        self.assertNotIn('ReadAnimationArgument5',banner)
        self.assertEqual(self.body.count('RegisterBoundConsciousCondition()'),1)

if __name__=='__main__':unittest.main()
