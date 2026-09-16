"""Execute original outer branches versus instrumented emitted candidate phases."""
import itertools,unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.wife_complete_candidate import generate
from tools.script_recovery.wife_dispatcher_native import execute,prove
from tools.script_recovery.lift_native_lua import RData

def instrument(source):
    a=source.index('    local function processHeroInteraction()');b=source.index('    local function runBody()',a)
    source=source[:a]+'''    local function processHeroInteraction() return outerInteraction() end
'''+source[b:]
    a=source.index('        timeRemaining = quest:GetTimer(');b=source.index('        alive = quest:NewScriptFrame(me)',a)
    source=source[:a]+'        outerIdle()\n'+source[b:]
    a=source.index('            quest:EntitySetAsUseMovementInActions(me, true)');b=source.index('            if not thingsWithinDistance then',a)
    source=source[:a]+'            thingsWithinDistance = outerRoute()\n'+source[b:]
    a=source.index('            if not __native_entity_state:GetStateBool("SaidRunningLine") then');b=source.index('            thingsWithinDistance = resources:ThingsAreWithinDistance',a)
    source=source[:a]+'            outerRunningLine()\n'+source[b:]
    a=source.index('            wifeAnimationRemainder = quest:RetailRandModulo(2)');b=source.index('        end\n\n        alive = quest:NewScriptFrame(me)',a)
    source=source[:a]+'            if not outerArgument() then return end\n'+source[b:]
    source=source.replace('__native_entity_state:GetStateBool("GoingForHusband")','outerGoing()')
    return source

def run(source,cancel=10,failures=0,going_after=0,near=False,approach=1,interaction=True,argument=True,hero_near=True,prepare=False,populated=True):
    lua=LuaRuntime();events=[];q,r,me=lua.table(),lua.table(),lua.table();state={'queries':0,'acquires':0,'going':0,'approach':0}
    def event(*args):events.append(args)
    def term(_):state['queries']+=1;v=state['queries']>=cancel;event('term',v);return v
    def acquire(_,id,actor,priority):
        assert id=='control' and actor is not None and priority==3
        v=state['acquires']>=failures;state['acquires']+=1;event('acquire',v,populated);return v
    def prep(_,id):event('prepare',prepare);event('prepare.release') if prepare else None
    def lookup(_,name):assert name=='NOVI_AffairMan';event('key.new');event('husband.new',populated);event('key.destroy');return 'husband'
    def distance(_,actor,target,value):state['approach']+=1;v=state['approach']>=approach;event('near.husband',v);return v
    def going():v=state['going']>=going_after;state['going']+=1;event('going',v);return v
    q.WithRetailResources=lambda _,body:body(r);q.RegisterBoundConsciousCondition=lambda _:event('condition')
    q.NewScriptFrame=lambda *args:event('frame');q.IsActiveThreadTerminating=term
    q.EntitySetAsUseMovementInActions=lambda _,actor,value:event('movement',value)
    q.GetHero=lambda _:event('hero') or 'hero'
    q.IsDistanceBetweenThingsUnder=lambda _,actor,target,value:event('near.hero',hero_near) or hero_near
    r.NewResource=lambda _:event('control.new') or 'control';r.PrepareResource=prep;r.TryAcquire=acquire
    r.NewThingFromScriptName=lookup;r.ThingsAreWithinDistance=distance
    r.ClearCommands=lambda _,id:event('clear.commands');r.ReleaseResource=lambda _,id:event('control.destroy')
    r.DestroyThing=lambda _,id:event('husband.destroy')
    for name,fn in {'outerGoing':going,'outerRoute':lambda:event('route',near) or near,
        'outerInteraction':lambda:event('interaction',interaction) or interaction,
        'outerIdle':lambda:event('idle'),'outerRunningLine':lambda:event('running'),
        'outerArgument':lambda:event('argument',argument) or argument}.items():lua.globals()[name]=fn
    lua.execute(source);lua.globals().Main(q,me);return events

class WifeDispatcherTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):cls.source,cls.report=generate();cls.instrumented=instrument(cls.source)

    def test_whole_outer_dispatcher_with_explicit_phase_boundaries(self):
        for cancel,failures,going_after,near,approach,interaction,argument,hero_near in itertools.product(
                range(1,17),(0,2),(0,2),(False,True),(1,3),(False,True),(False,True),(False,True)):
            args=(cancel,failures,going_after,near,approach,interaction,argument,hero_near)
            self.assertEqual(run(self.instrumented,*args),execute(*args),args)

    def test_prepare_and_failed_populated_control_output(self):
        for cancel,prepare,populated in itertools.product(range(1,12),(False,True),(False,True)):
            args=dict(cancel=cancel,failures=2,prepare=prepare,populated=populated)
            self.assertEqual(run(self.instrumented,**args),execute(**args),args)

    def test_original_first_frame_regression_and_mutated_branches_rejected(self):
        frame='''            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return false end
'''
        start=self.instrumented.index('    local function waitUntilNearHusband()')
        end=self.instrumented.index('    local function processHeroInteraction()',start)
        body=self.instrumented[start:end]
        self.assertEqual(body.count(frame),1)
        old=body.replace(frame,'').replace('            if thingsWithinDistance then break end\n','            if thingsWithinDistance then break end\n'+frame)
        original_timing=self.instrumented[:start]+old+self.instrumented[end:]
        self.assertNotEqual(run(original_timing,cancel=5),execute(cancel=5))
        events=run(self.instrumented,cancel=5)
        self.assertEqual(events[-4:],[('frame',),('term',True),('husband.destroy',),('control.destroy',)])
        self.assertNotIn(('running',),events)
        class Changed(RData):
            def bytes_at(self,address,size):
                raw=super().bytes_at(address,size)
                if address<=self.pc<address+size:raw=bytearray(raw);raw[self.pc-address]^=1;return bytes(raw)
                return raw
        for pc in (0xdb2b62,0xdb2be1,0xdb34a9,0xdb34b5,0xdb34ba,0xdb34c1,0xdb358d,0xdb35ca,0xdb3cbc,0xdb3cc6):
            d=Changed();d.pc=pc
            with self.assertRaises(ValueError):prove(d)

if __name__=='__main__':unittest.main()
