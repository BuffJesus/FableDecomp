import itertools,unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.teddy_girl_health import SOURCE as HEALTH
from tools.script_recovery.teddy_girl_question import recover
from tools.script_recovery.teddy_girl_question_native import execute
from tools.script_recovery.teddy_girl_talk import SOURCE as TALK
SOURCE,PROOF=recover()

def lua_case(done=False,hit=False,health=1.0,busy=0,answers=(-1,1),cancel=999,*,talk=False,found=False,ruined=False):
    lua=LuaRuntime();lua.execute(HEALTH+SOURCE+TALK);q=lua.table();r=lua.table();state=lua.table();events=[];queries=0;remaining=0;answer_index=0
    def get(_,key):events.append(('get',key));return {'DoneIntro':done,'HeroHitMe':hit,'FoundTeddy':found,'TeddyRuined':ruined}[key]
    def term(_):
        nonlocal queries
        queries+=1;value=queries>=cancel;events.append(('term',value));return value
    def speak(_,control,hero,key,*flags):
        nonlocal remaining
        assert (control,hero,flags)==(1,7,(0,False,True,False));events.append(('speak',key));remaining=busy
    def task(_,control):
        nonlocal remaining
        value=remaining>0;remaining-=1;events.append(('busy',value));return value
    def question(_):
        events.extend(('text.new',key) for key in PROOF['questionConstructionOrder']);events.append(('question',))
        events.extend(('text.destroy',key) for key in reversed(PROOF['questionConstructionOrder']))
    def answer(_):
        nonlocal answer_index
        value=answers[min(answer_index,len(answers)-1)];answer_index+=1;events.append(('answer',value));return value
    state.GetStateBool=get;state.SetStateBool=lambda _,key,value:events.append(('set',key,value))
    q.GetStateBool=get;q.ClearThingHasInformation=lambda _,actor:events.append(('clear',))
    q.IsActiveThreadTerminating=term;q.GetHero=lambda _:events.append(('hero',)) or 7
    q.NewScriptFrame=lambda _:events.append(('frame',));q.MsgIsQuestionAnsweredYesOrNo=answer
    r.NewThingFromResource=lambda _,c:events.append(('thing.new',)) or 9;r.ThingHealth=lambda _,a:events.append(('health',)) or health
    r.DestroyThing=lambda _,a:events.append(('thing.destroy',));r.Speak=speak;r.IsPerformingScriptTask=task;r.GiveTeddyGirlQuestion=question
    result=lua.globals().TeddyGirlTalk(q,'girl',r,1,state) if talk else lua.globals().TeddyGirlQuestion(q,r,1,state,lambda:events.append(('given',)))
    return result,events

class TeddyGirlQuestionTests(unittest.TestCase):
    def test_original_introduction_answer_health_waits_and_cancellation(self):
        for done,hit,health,busy,answers,cancel in itertools.product((False,True),(False,True),(1.0,0.0,float('nan')),(0,2),((1,),(0,),(-1,1),(-3,2)),(1,2,3,4,6,8,12,999)):
            with self.subTest(done=done,hit=hit,health=health,busy=busy,answers=answers,cancel=cancel):
                case=dict(done=done,hit=hit,health=health,busy=busy,answers=answers,cancel=cancel)
                self.assertEqual(execute(**case),lua_case(**case))

if __name__=='__main__':unittest.main()
