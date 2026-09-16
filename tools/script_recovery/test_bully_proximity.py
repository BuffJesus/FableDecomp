import itertools,unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.bully_proximity import SOURCE
from tools.script_recovery.bully_proximity_native import execute
from tools.script_recovery.bully_full_resource_candidate import generate

def lua_case(**kw):
    values=dict(timer=0,spoken=False,roll=0,modulus=3,near=True,hits=0,index=10,cancel=999,raw=255);values.update(kw)
    lua=LuaRuntime();lua.execute(SOURCE);r,q,state=lua.table(),lua.table(),lua.table();trace=[];live={};queries=0
    fields={'SpokenOnFirstProximity':values['spoken'],'HitsTaken':values['hits'],'IntimidateSpeechLoop':values['index']}
    state.GetStateBool=lambda _,k:fields[k];state.GetStateInt=lambda _,k:fields[k]
    def set_state(_,k,v):fields[k]=v;trace.append(('state',k,v))
    state.SetStateBool=set_state;state.SetStateInt=set_state
    r.FaceRetainedThing=lambda _,a,b,snap:trace.append(('face',)) if (a,b,snap)==('me','victim',True) else (_ for _ in ()).throw(AssertionError())
    q.GetStateInt=lambda _,key:77 if key=='TalkIntermittentTimer' else (_ for _ in ()).throw(AssertionError())
    q.GetTimer=lambda _,timer:trace.append(('timer',values['timer'])) or values['timer']
    q.SetTimer=lambda _,timer,duration:trace.append(('settimer',duration))
    q.SetStateBool=set_state;q.GetHero=lambda _:trace.append(('hero',)) or 'hero'
    lua.globals().rand=lambda:trace.append(('rand',values['roll'])) or values['roll']
    r.ReadBullyRandomModulus=lambda _:values['modulus'];r.ReadBullyProximityRange=lambda _:7.25
    r.IsDistanceBetweenThingsUnder=lambda _,a,b,d:trace.append(('distance',d,values['near'])) or values['near']
    def term(_):
        nonlocal queries
        queries+=1;result=queries>=values['cancel'];trace.append(('term',result));return result
    q.IsActiveThreadTerminating=term
    def conversation(_,actor,a,b):assert (actor,a,b)==('me',False,False);trace.append(('conversation',));return 73
    r.NewConversation=conversation;r.AddConversationPerson=lambda _,c,p:trace.append(('person',))
    def text(_):assert not live;live[1]='';trace.append(('text.new',));return 1
    def format_text(_,key,index):assert live[key]=='';live[key]='TEXT_QST_048_BULLY_SCRMSG_INTIMIDATING_%d'%index;trace.append(('text.format',index))
    def destroy(_,key):live.pop(key);trace.append(('text.destroy',))
    r.NewText=text;r.FormatBullyIntimidationText=format_text;r.DestroyText=destroy
    r.AddConversationText=lambda _,c,key,a,b,f:trace.append(('line',live[key]))
    def animation(_,control,name,*flags):
        assert control=='control' and flags==(False,False,False,True,False,False) and live
        trace.extend([('animation.string.new',name),('animation',name,values['raw']),('animation.string.destroy',name)])
    r.PlayAnimationWithNativeArgument5=animation
    result=lua.globals().BullyProximity(q,r,'me','victim','control',state)
    assert not live
    return result,trace

class BullyProximityTests(unittest.TestCase):
    def test_original_gates_rollover_cancellation_and_retained_format_match(self):
        cases=[dict(timer=3),dict(near=False),dict(hits=1),dict(spoken=True,roll=2,modulus=2),dict(spoken=True,roll=-3,modulus=-3)]
        cases += [dict(index=index,roll=roll,cancel=cancel,spoken=spoken)
            for index,roll,cancel,spoken in itertools.product((10,40,2147483640),(0,1),(1,2,3,999),(False,True))]
        for case in cases:
            with self.subTest(case=case):self.assertEqual(execute(**case),lua_case(**case))

    def test_full_candidate_has_no_missing_operand_placeholders(self):
        source,report=generate()
        self.assertNotIn('nil --[[missing]]',source)
        self.assertNotIn('DAT_013ac',source)
        self.assertIn('BullyProximity(quest, resources, me, r1, bully_control, __native_entity_state)',source)
        self.assertFalse(report['enabled'])
        self.assertTrue('-- TODO(native): goto' in source or '-- TODO(native):' in source)

if __name__=='__main__':unittest.main()
