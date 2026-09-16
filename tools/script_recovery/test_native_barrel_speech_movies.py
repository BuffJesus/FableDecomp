import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_barrel_speech_movies import HELPERS


class BarrelSpeechMovieTests(unittest.TestCase):
    def test_health_speech_polling_reward_and_cancellation_order(self):
        run=LuaRuntime(unpack_returned_tuples=True).execute('''return function(helper, thanks, health, busyCount, cancelAt)
            local events={};local movieLive=false;local paused=false;local terms=0
            local function record(value) events[#events+1]=value end
            local resources={}
            function resources:StartMovie(name) assert(name=="" and not movieLive);movieLive=true;record('movie');return 7 end
            function resources:Pause(value) assert(movieLive);paused=value;record(value and 'pause' or 'unpause') end
            function resources:DestroyMovie(id) assert(id==7 and movieLive and not paused);movieLive=false;record('destroy') end
            function resources:Speak(id,hero,text,zero,a,b,c)
                assert(id==1 and hero=='hero' and zero==0 and a==false and b==true and c==false)
                assert(text==(thanks and 'TEXT_QST_048_BARRELMAN_THANKS' or 'TEXT_QST_048_BARRELMAN_CAREFUL'))
                record('speak')
            end
            function resources:IsPerformingScriptTask(id) assert(id==1);record('task');busyCount=busyCount-1;return busyCount>=0 end
            local quest={}
            function quest:GetHero() record('hero');return 'hero' end
            function quest:NewScriptFrame(actor) assert(actor=='actor');record('frame') end
            function quest:IsActiveThreadTerminating() terms=terms+1;record('term');return terms==cancelAt end
            local env={resources=resources,quest=quest,me='actor',barrel_resource=1,
                controlled_health=function() record('health');return health end,
                require=function(name) assert(name=='NewOakValeIntro.native_quest_helpers');return {
                    AddGoodDeed=function(q,actor) assert(thanks and q==quest and actor=='actor' and movieLive and paused);record('deed') end} end}
            local invoke=assert(load(helper .. '\\nreturn '..(thanks and 'playThanksMovie' or 'playCarefulMovie'),'movies','t',env))()
            local result=invoke();assert(not movieLive and not paused)
            return result,table.concat(events,',')
        end''')
        for thanks in (False,True):
            for health in (1.0,0.0,float('nan')):
                for busy in (0,1,3):
                    for cancel in (1,2,4,5):
                        events=['movie','pause','health'];cancelled=False
                        if health>0:
                            events+=['hero','speak','task'];terms=0
                            for frame in range(busy):
                                events+=['frame','term'];terms+=1
                                if terms==cancel:cancelled=True;break
                                events.append('task')
                            if not cancelled:
                                events.append('term');terms+=1;cancelled=terms==cancel
                        if thanks and not cancelled:events.append('deed')
                        events+=['unpause','destroy']
                        with self.subTest(thanks=thanks,health=health,busy=busy,cancel=cancel):
                            self.assertEqual(run(HELPERS,thanks,health,busy,cancel),(not cancelled,','.join(events)))
