import itertools
import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.guard_interactions import recover
from tools.script_recovery.guard_talk_native import execute as native_talk
from tools.script_recovery.guard_hit_native import execute as native_hit
SOURCE=recover()[0]

def lua_talk(talked=True,bad=0,good=1,conversation=-7,busy=0,cancel=999):
    lua=LuaRuntime();lua.execute(SOURCE);q=lua.table();r=lua.table();me=lua.table(name='guard');events=[];queries=0;remaining=busy
    def talked_to(_):
        events.extend([('text.new','SCRIPT_NAME_HERO'),('talk',talked),('text.destroy','SCRIPT_NAME_HERO')]);return talked
    def term(_):
        nonlocal queries
        queries+=1;value=queries>=cancel;events.append(('term',value));return value
    def hero(_):events.append(('hero',));return 7
    def face(_,actor,snap,reverse):assert actor.name=='guard' and not snap and not reverse;hero(q);events.append(('face',False))
    def conversation_(_,actor,a,b):assert actor.name=='guard' and not a and not b;events.append(('conversation',conversation));return conversation
    def person(_,index,actor):assert (index,actor)==(conversation,7);events.append(('person',index))
    def get(_,key):events.append(('get',key));return bad if key=='BadDeedsPerformed' else good
    def line(_,index,key,actor):
        assert index==conversation and actor.name=='guard'
        events.append(('text.new',key));hero(q);events.append(('line',index,key));events.append(('text.destroy',key))
    def active(_,index):
        nonlocal remaining
        assert index==conversation;value=remaining>0;remaining-=1;events.append(('busy',index,value));return value
    me.IsTalkedToByHero=talked_to;q.IsActiveThreadTerminating=term;q.GetHero=hero;q.GetStateInt=get
    q.IsConversationActive=active;q.NewScriptFrame=lambda _:events.append(('frame',))
    r.GuardFaceHero=face;r.NewConversation=conversation_;r.AddRawConversationPerson=person;r.GuardAddHeroConversationLine=line
    r.PrepareResource=lambda _,control:events.append(('prepare',))
    return lua.globals().GuardTalk(q,me,r,9),events

class GuardInteractionTests(unittest.TestCase):
    def test_original_talk_states_ids_waits_and_cancellation(self):
        for talked,(bad,good),conversation,busy,cancel in itertools.product((False,True),((0,1),(0,0),(-1,5),(1,9)),(-7,0,29),(0,2),(1,2,3,4,999)):
            with self.subTest(talked=talked,bad=bad,conversation=conversation,busy=busy,cancel=cancel):
                case=dict(talked=talked,bad=bad,good=good,conversation=conversation,busy=busy,cancel=cancel)
                self.assertEqual(native_talk(**case),lua_talk(**case))

    def test_original_hit_mask_short_circuit_and_reverse_text_lifetime(self):
        for answers,mask in itertools.product(itertools.product((False,True),repeat=3),(0,256)):
            with self.subTest(answers=answers,mask=mask):
                result,remaining,events=native_hit('hit',answers,mask)
                count=1 if answers[0] else (3 if answers[1] else 2)
                expected=[]
                for i in range(count):expected.extend([('string.new','SCRIPT_NAME_HERO'),('query',i,answers[i])])
                expected.extend([('string.destroy','SCRIPT_NAME_HERO')]*count)
                self.assertEqual(events,expected)
                self.assertEqual(result,answers[0] or (answers[1] and not answers[2]));self.assertEqual(remaining,mask)

if __name__=='__main__':unittest.main()
