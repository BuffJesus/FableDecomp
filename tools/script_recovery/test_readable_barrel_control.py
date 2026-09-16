import itertools
import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.readable_barrel_control import BODY,HELPER


class BarrelControlReadabilityTests(unittest.TestCase):
    def test_identical_call_order_and_cancellation(self):
        run=LuaRuntime(unpack_returned_tuples=True).execute('''return function(body,failures,cancelAt,failAt)
            local events={};local terms=0;local tries=0
            local function event(value)
                events[#events+1]=value
                if #events==failAt then error('BOUNDARY',0) end
            end
            local env={me='actor',barrel_resource=7,resources={
                PrepareResource=function(_,id)assert(id==7);event('prepare')end,
                TryAcquire=function(_,id,actor,priority)assert(id==7 and actor=='actor' and priority==4);event('acquire');tries=tries+1;return tries>failures end},
                quest={NewScriptFrame=function(_,actor)assert(actor=='actor');event('frame')end,
                IsActiveThreadTerminating=function()event('term');terms=terms+1;return terms==cancelAt end}}
            local f=assert(load(body,'control','t',env));local ok,result=pcall(f)
            return ok,result,table.concat(events,',')
        end''')
        original='local alive,cVar2\n'+BODY+'do return true end\n::LAB_00db6afd:: return false'
        structured=HELPER+'return acquireBarrelControl()'
        for failures,cancel,error in itertools.product((0,1,4),range(7),(0,1,2,3,6,10)):
            self.assertEqual(run(original,failures,cancel,error),run(structured,failures,cancel,error))
