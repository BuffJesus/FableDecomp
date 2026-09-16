import itertools
import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_barrel_initial_interaction import HELPER


class BarrelInitialInteractionTests(unittest.TestCase):
    def test_timer_outlives_movement_speech_departure_and_closes_before_movie(self):
        run=LuaRuntime(unpack_returned_tuples=True).execute('''return function(helper,moves,busy,health,cancelAt,remaining)
            local events={};local timerLive=false;local movieLive=true;local phase=0;local terms=0;local queries=0
            local taskBusy=busy;local timer={}
            local function event(name)events[#events+1]=name end
            function timer:Set(value)assert(timerLive and value==2);event('timer.set');taskBusy=busy end
            function timer:Get()assert(timerLive);event('timer.get');return remaining end
            local resources={}
            function resources:WithTimer(callback)
                assert(not timerLive);timerLive=true;event('timer.new')
                local result=callback(timer);timerLive=false;event('timer.close');return result
            end
            function resources:IsBarrelManFarFromHero(actor)assert(timerLive and actor=='actor');queries=queries+1;event('distance');return queries<=moves end
            function resources:MoveBarrelManToHero(id)assert(timerLive and id==1);event('move')end
            function resources:IsPerformingScriptTask(id)assert(timerLive and id==1);event('task');local result=taskBusy;taskBusy=false;return result end
            function resources:Speak(id,hero,text,n,a,b,c)
                assert(timerLive and id==1 and hero=='hero' and text=='TEXT_QST_048_BARRELMAN_FAVOUR' and n==0 and not a and b and not c);event('speak')
            end
            function resources:SetBarrelWatchTimer(id)assert(timerLive and id==73);event('watch')end
            function resources:PrepareResource(id)assert(not timerLive and movieLive and id==1);event('prepare')end
            local quest={}
            function quest:IsActiveThreadTerminating()terms=terms+1;event('term');return terms==cancelAt end
            function quest:NewScriptFrame(actor)assert(actor=='actor');event('frame')end
            function quest:GetHero()assert(timerLive);event('hero');return 'hero'end
            function quest:FadeScreenOut(a,b)assert(timerLive and a==1 and b==1);event('fade.out')end
            function quest:Pause(value)assert(timerLive and value==2);event('pause')end
            function quest:GetThingWithScriptName(name)assert(timerLive);event(name);return name end
            function quest:EntityTeleportToThing(actor,marker,flag)assert(timerLive and not flag);event('teleport.'..actor)end
            function quest:ClearThingHasInformation(actor)assert(timerLive and actor=='actor');event('information')end
            function quest:FadeScreenIn()assert(timerLive);event('fade.in')end
            function quest:GetStateInt(name)assert(name=='WatchTimer');return 73 end
            function quest:SetStateBool(name,value)assert(timerLive and name=='BarrelManLeftHeroInCharge' and value);event('left')end
            local env={resources=resources,quest=quest,me='actor',barrel_resource=1,
                __native_entity_state={SetStateInt=function(_,name,value)assert(timerLive and name=='MyPhase' and value==2);phase=value;event('phase')end},
                controlled_health=function()assert(timerLive);event('health');return health end,
                waitForBarrelSpeech=function()assert(timerLive);event('wait');return not quest:IsActiveThreadTerminating()end,
                finishSpeechMovie=function(id)assert(id==7 and movieLive and not timerLive);movieLive=false;event('movie.close')end}
            local invoke=assert(load(helper..'\\nreturn playInitialInteraction','initial','t',env))()
            local result=invoke(7);assert(not timerLive and not movieLive)
            return result,table.concat(events,','),phase
        end''')
        for moves,busy,health,cancel,remaining in itertools.product((0,1,2),(False,True),(0.0,1.0),range(10),(0,2)):
            result,trace,phase=run(HELPER,moves,busy,health,cancel,remaining)
            events=trace.split(',')
            with self.subTest(moves=moves,busy=busy,health=health,cancel=cancel):
                self.assertEqual(events.count('movie.close'),1)
                self.assertEqual(events.count('timer.new'),0 if cancel==1 else 1)
                self.assertEqual(events.count('timer.close'),events.count('timer.new'))
                if 'timer.close' in events:self.assertLess(events.index('timer.close'),events.index('movie.close'))
                self.assertEqual(events.count('prepare'),int(result))
                self.assertEqual(phase,2 if result else 0)
                if result:
                    self.assertEqual(events.count('move'),moves)
                    self.assertEqual(events.count('timer.set'),moves)
                    self.assertEqual(events.count('timer.get'),moves if busy else 0)
                    self.assertEqual(events.count('speak'),int(health>0))
                    self.assertLess(events.index('left'),events.index('timer.close'))
                    self.assertEqual(events[-3:],['timer.close','prepare','movie.close'])
