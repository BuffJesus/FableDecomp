import itertools
import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_barrel_teleport_scope import HELPER, recover


class BarrelTeleportScopeTests(unittest.TestCase):
    def test_timer_camera_and_reverse_cleanup_order(self):
        run = LuaRuntime(unpack_returned_tuples=True).execute('''return function(helper, polls, cancelAt, visible)
            local events={}; local live={}; local terms=0; local reads=0; local ids=0
            local function event(s) events[#events+1]=s end
            local resources={}
            function resources:GetBarrelWatchTimer(id)
                reads=reads+1; assert(id==100+reads); event('timer')
                if reads>polls then return 15 end
                return reads%2==0 and 14 or 16
            end
            function resources:NewThingFromScriptName(name)
                local id=name=='M_BarrelManWalkOff' and 1 or 2
                assert(name=='M_BarrelManWalkOff' or name=='M_BarrelManWalkOffAlt')
                assert(not live[id]); live[id]=true; event('new'..id); return id
            end
            function resources:IsOwnedThingPositionOnScreen(id)
                assert(id==1 and live[1] and live[2]); event('camera'); return visible
            end
            function resources:TeleportActorToOwnedThing(actor,id)
                assert(actor=='actor' and live[1] and live[2]); event('teleport'..id)
            end
            function resources:DestroyThing(id) assert(live[id]); live[id]=nil; event('destroy'..id) end
            local quest={}
            function quest:GetStateInt(name) assert(name=='WatchTimer'); ids=ids+1; event('id'); return 100+ids end
            function quest:NewScriptFrame(actor) assert(actor=='actor'); event('frame') end
            function quest:IsActiveThreadTerminating() terms=terms+1; event('term'); return terms==cancelAt end
            local env={resources=resources,quest=quest,me='actor',__native_entity_state={
                SetStateInt=function(_,name,value) assert(name=='MyPhase' and value==3 and live[1] and live[2]); event('phase') end}}
            local invoke=assert(load(helper..'\\nreturn teleportWalkOff','teleport','t',env))()
            local result=invoke(); assert(not live[1] and not live[2]); return result,table.concat(events,',')
        end''')
        for polls, cancel, visible in itertools.product((0, 1, 3), range(7), (False, True)):
            events=[]; cancelled=False; terms=0
            for _ in range(polls):
                events += ['id','timer','frame','term']; terms+=1
                if terms==cancel:
                    cancelled=True
                    break
            if not cancelled:
                events += ['id','timer','term']; terms+=1
                cancelled=terms==cancel
            if not cancelled:
                events += ['new1','new2','camera','term']; terms+=1
                cancelled=terms==cancel
                if not cancelled: events += ['teleport2' if visible else 'teleport1','phase']
                events += ['destroy2','destroy1']
            self.assertEqual(run(HELPER,polls,cancel,visible),(not cancelled,','.join(events)))

    def test_changed_native_bytes_rejected(self):
        class Changed:
            def bytes_at(self,address,size): return bytes(size)
        with self.assertRaisesRegex(ValueError,'native instructions changed'):
            recover('',Changed())
