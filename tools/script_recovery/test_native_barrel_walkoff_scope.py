import itertools
import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_barrel_walkoff_scope import HELPER


class BarrelWalkoffScopeTests(unittest.TestCase):
    def test_snapshot_and_marker_lifetime_through_moves_and_cancellation(self):
        run=LuaRuntime(unpack_returned_tuples=True).execute('''return function(helper,moves,cancelAt,waitResult)
            local events={};local live=false;local term=0;local queries=0;local phase=1;local snapshot={x=11,y=22,z=33}
            local function event(value)events[#events+1]=value end
            local resources={}
            function resources:NewThingFromScriptName(name)assert(name=='M_BarrelManWalkOff');live=true;event('marker');return 7 end
            function resources:ThingPosition(id)assert(live and id==7);event('position');return snapshot end
            function resources:MoveToPosition(id,pos,speed,kind,a,b)
                assert(live and id==1 and pos==snapshot and speed==0 and kind==1 and not a and not b);event('move')
            end
            function resources:DestroyThing(id)assert(live and id==7);live=false;event('destroy')end
            local quest={}
            function quest:NewScriptFrame(actor)assert(live and actor=='actor');event('frame')end
            function quest:IsActiveThreadTerminating()assert(live);term=term+1;event('term');return term==cancelAt end
            local env={resources=resources,quest=quest,me='actor',barrel_resource=1,
                controlled_distance=function(pos,limit)assert(live and pos==snapshot and limit==2);queries=queries+1;event('distance');return queries<=moves end,
                waitForBarrelSpeech=function()assert(live);event('wait');return waitResult end,
                __native_entity_state={SetStateInt=function(_,name,value)assert(live and name=='MyPhase' and value==2);phase=value;event('phase')end}}
            local invoke=assert(load(helper..'\\nreturn walkOffFromWarehouse','walkoff','t',env))()
            local continued=invoke();assert(not live)
            return continued,table.concat(events,','),phase
        end''')
        for moves,cancel,wait in itertools.product((0,1,3),range(6),(False,True)):
            events=['marker','position'];cancelled=False;terms=0
            for move in range(moves):
                events+=['distance','frame','term'];terms+=1
                if terms==cancel:cancelled=True;break
                events+=['move','wait']
                if not wait:cancelled=True;break
            if not cancelled:
                events+=['distance','term'];terms+=1;cancelled=terms==cancel
            if not cancelled:events.append('phase')
            events.append('destroy')
            self.assertEqual(run(HELPER,moves,cancel,wait),(not cancelled,','.join(events),1 if cancelled else 2))
