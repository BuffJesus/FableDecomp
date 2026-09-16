import itertools
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.readable_barrel_phase import HELPER


class BarrelPhaseReadabilityTests(unittest.TestCase):
    def test_phase_actions_and_cancellation_match_previous_lua(self):
        run=LuaRuntime(unpack_returned_tuples=True).execute('''return function(source,firstPhase,phase,thank,cancelAt,stopHelper)
            local events={};local terms=0;local reads=0
            local function event(name,...)
                local args={...};for i=1,#args do args[i]=tostring(args[i]) end
                events[#events+1]=name..':'..table.concat(args,',')
            end
            local function api(prefix)
                return setmetatable({}, {__index=function(_,name)return function(_,...)event(prefix..name,...);return true end end})
            end
            local quest=api('quest.');local state=api('state.');local resources=api('resources.')
            function quest:GetStateBool(name)assert(name=='BarrelBrokenPersistent');return false end
            function quest:IsActiveThreadTerminating()event('term');terms=terms+1;return terms==cancelAt end
            function quest:GetStateInt(name)event('watch',name);return 73 end
            function state:GetStateInt(name)event('phase',name);reads=reads+1;return reads==1 and firstPhase or phase end
            function resources:ShouldBarrelManThankHero(actor)event('thank',actor);return thank end
            local env={quest=quest,resources=resources,__native_entity_state=state,me='actor',warehouseStartMarker=9}
            for _,name in ipairs({'acquireBarrelControl','walkOffFromWarehouse','teleportWalkOff','returnToWarehouse','playThanksMovie','showWarehouseFailure'}) do
                env[name]=function(...)event(name,...);return name~=stopHelper end
            end
            env.playReturnInteraction=function(movie,phase)event('playReturnInteraction',movie,phase);return true end
            setmetatable(env,{__index=_G})
            local result=assert(load(source,'phase','t',env))();return result,table.concat(events,';')
        end''')
        before=Path(__file__).with_name('readable_barrel_phase_before.lua').read_text()
        original='local alive,native_arg_switch_2\n'+before+'do return true end\n::LAB_00db6afd:: return false'
        structured=HELPER+'return advanceBarrelPhase()'
        stops=('', 'acquireBarrelControl','walkOffFromWarehouse','teleportWalkOff','returnToWarehouse','playThanksMovie','showWarehouseFailure')
        for first,phase,thank,cancel,stop in itertools.product((0,1),(0,1,2,3,4,5), (False,True),range(4),stops):
            self.assertEqual(run(original,first,phase,thank,cancel,stop),run(structured,first,phase,thank,cancel,stop))
