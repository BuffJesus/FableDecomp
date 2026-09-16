import itertools
import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_barrel_return_dialogue import HELPER


class BarrelReturnDialogueTests(unittest.TestCase):
    def test_reply_selection_termination_and_reset_order(self):
        run=LuaRuntime(unpack_returned_tuples=True).execute('''return function(helper,phase,letDown,broken,health,cancelAt,waitResult)
            local events={};local terms=0;local finished=false
            local function event(name)events[#events+1]=name end
            local resources={}
            function resources:Speak(id,hero,line,n,a,b,c)
                assert(id==1 and hero=='hero' and n==0 and a==false and b==true and c==false);event(line)
            end
            function resources:PrepareResource(id)assert(id==1 and not finished);event('prepare')end
            local quest={}
            function quest:IsActiveThreadTerminating()terms=terms+1;event('term');return terms==cancelAt end
            function quest:GetStateBool(name)assert(name=='BarrelBrokenPersistent');event('broken');return broken end
            function quest:GetHero()event('hero');return 'hero'end
            local state={GetStateBool=function(_,name)assert(name=='HeroLetMeDown');event('letdown');return letDown end}
            local env={resources=resources,quest=quest,me='actor',barrel_resource=1,__native_entity_state=state,
                controlled_health=function()event('health');return health end,
                waitForBarrelSpeech=function()event('wait');return waitResult end,
                finishSpeechMovie=function(id)assert(id==7 and not finished);finished=true;event('finish')end}
            local invoke=assert(load(helper..'\\nreturn playReturnInteraction','return-dialogue','t',env))()
            local result=invoke(7,phase);assert(finished)
            return result,table.concat(events,',')
        end''')
        for phase,letdown,broken,health,cancel,wait in itertools.product(
                (1,3,4,5,6),(False,True),(False,True),(1.0,0.0,float('nan')),(0,1,2,3),(False,True)):
            prefix=[];line=None
            if phase<4:prefix=['term'];line='NOT_LARKING'
            elif phase==5:
                prefix=['term','letdown','term']
                if letdown:prefix+=['broken','term'];line='LETDOWN_BROKEN' if broken else 'LETDOWN_NOT_BROKE'
                else:line='NO_TIME'
            events=[];terms=0;cancelled=False
            for item in prefix:
                events.append(item)
                if item=='term':
                    terms+=1
                    if terms==cancel:cancelled=True;break
            if not cancelled and line:
                events.append('health')
                if health>0:
                    events+=['hero','TEXT_QST_048_BARRELMAN_'+line,'wait'];cancelled=not wait
            if not cancelled:events.append('prepare')
            events.append('finish')
            with self.subTest(phase=phase,letdown=letdown,broken=broken,health=health,cancel=cancel,wait=wait):
                self.assertEqual(run(HELPER,phase,letdown,broken,health,cancel,wait),(not cancelled,','.join(events)))
