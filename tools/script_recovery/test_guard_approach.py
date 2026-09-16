import itertools
import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.guard_approach import recover
from tools.script_recovery.guard_approach_native import execute
SOURCE,EVIDENCE=recover()

def lua_case(bad=1,dealt=0,distances=(True,False,False,True),failures=0,cancel=999,claim_on_frame=False,conversation=-7):
    lua=LuaRuntime();lua.execute(SOURCE);q=lua.table();r=lua.table();events=[];state={'queries':0,'frames':0,'acquires':0,'distance':0,'dealt':dealt}
    def get(_,key):events.append(('get',key));return bad if key=='BadDeedsPerformed' else state['dealt']
    def term(_):
        state['queries']+=1;value=state['queries']>=cancel;events.append(('term',value));return value
    def hero(_):events.append(('hero',));return 'hero'
    def frame(_):
        state['frames']+=1;events.append(('frame',))
        if claim_on_frame:state['dealt']=bad
    def alert(_,index):
        offset=(index*4)&0xffffffff
        if offset>=0x80000000:offset-=0x100000000
        value=2.5+state['frames'] if offset==-4 else 7.25+offset//4
        events.append(('alert',index,value));return value
    def range_(_):value=2.5+state['frames'];events.append(('range',value));return value
    def distance(_,a,b,range):
        assert (a,b)==('guard','hero');value=distances[min(state['distance'],len(distances)-1)];state['distance']+=1;events.append(('distance',range,value));return value
    def acquire(_,control,actor,priority):
        assert (control,actor,priority)==(9,'guard',4);state['acquires']+=1;value=state['acquires']>failures;events.append(('acquire',value));return value
    def face(_,actor,snap,reverse):assert (actor,snap,reverse)==('guard',True,False);hero(q);events.append(('face',True))
    def new(_,actor,a,b):assert (actor,a,b)==('guard',False,False);events.append(('conversation',conversation));return conversation
    def person(_,index,actor):assert (index,actor)==(conversation,'hero');events.append(('person',index))
    def line(_,index,key,actor):
        assert (index,key,actor)==(conversation,EVIDENCE['conversationKey'],'guard')
        events.append(('text.new',key));hero(q);events.append(('line',index));events.append(('text.destroy',))
    def follow(_,control,actor,range,flag):assert (control,actor,range,flag)==(9,'hero',1.0,True);events.append(('follow',range,flag))
    q.GetStateInt=get;q.IsActiveThreadTerminating=term;q.GetHero=hero;q.NewScriptFrame=frame
    r.ReadGuardAlertRange=alert;r.ReadGuardLectureRange=range_;r.IsDistanceBetweenThingsUnder=distance
    r.SetThingAsAlly=lambda _,a,b:events.append(('ally',a));r.PrepareResource=lambda _,c:events.append(('prepare',));r.TryAcquire=acquire
    r.GuardFaceHero=face;r.NewConversation=new;r.AddRawConversationPerson=person;r.GuardAddHeroConversationLine=line;r.FollowThing=follow
    r.SetRawCutsceneBehaviour=lambda _,actor,value:events.append(('cutscene',value))
    return lua.globals().GuardApproach(q,'guard',r,9),events

class GuardApproachTests(unittest.TestCase):
    def test_signed_counter_delta_and_upper_only_index_bound(self):
        for bad,dealt in ((0,0),(1,0),(3,0),(8,0),(-1,-2),(-2,-3),(-2147483648,1),(2147483647,-1)):
            with self.subTest(bad=bad,dealt=dealt):
                case=dict(bad=bad,dealt=dealt)
                self.assertEqual(execute(**case),lua_case(**case))

    def test_chase_live_ranges_recheck_and_all_cancellation_points(self):
        for distances,failures,cancel,claim in itertools.product(((False,),(True,True,True),(True,False,False,True)),(0,2),(1,2,3,4,5,6,7,999),(False,True)):
            with self.subTest(distances=distances,failures=failures,cancel=cancel,claim=claim):
                case=dict(distances=distances,failures=failures,cancel=cancel,claim_on_frame=claim)
                self.assertEqual(execute(**case),lua_case(**case))

if __name__=='__main__':unittest.main()
