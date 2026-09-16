import unittest
from types import SimpleNamespace
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_barrel_failure_movie import recover,HELPER
from tools.script_recovery.generate_barrel_man_resource_candidate import DRAFT


class BarrelFailureMovieTests(unittest.TestCase):
    def test_dismissal_and_each_cancellation_cleanup(self):
        run=LuaRuntime(unpack_returned_tuples=True).execute('''return function(helper, dismissAfter, cancelAt)
            local events={};local live=false;local paused=false;local polls=0;local terms=0
            local function record(name) events[#events+1]=name end
            local resources={}
            function resources:StartMovie(name) assert(name=="" and not live);live=true;record('movie');return 1 end
            function resources:Pause(value) assert(live);paused=value;record(value and 'pause' or 'unpause') end
            function resources:DestroyMovie(id) assert(id==1 and live and not paused);live=false;record('destroy') end
            local quest={}
            function quest:DisplayGameInfo(text) assert(paused and text=='TEXT_QST_048_INSTRUCTION_LEFT_WAREHOUSE_UNATTENDED');record('display') end
            function quest:MsgIsGameInfoClickedPast() polls=polls+1;record('poll');return polls>dismissAfter end
            function quest:NewScriptFrame(actor) assert(actor=='actor');record('frame') end
            function quest:IsActiveThreadTerminating() terms=terms+1;record('term');return terms==cancelAt end
            local env={resources=resources,quest=quest,me='actor',require=function(name)
                assert(name=='NewOakValeIntro.native_quest_helpers')
                return {AddBadDeed=function(q,actor,count) assert(q==quest and actor=='actor' and count==1 and live and paused);record('deed') end}
            end}
            local phase=assert(load(helper .. '\\nreturn showWarehouseFailure','helper','t',env))()
            local result=phase();assert(not live and not paused)
            return result,table.concat(events,',')
        end''')
        for dismiss_after in range(4):
            for cancel_at in range(1,7):
                events=['movie','pause','display','poll'];cancelled=False;terms=0
                for frame in range(dismiss_after):
                    events+=['frame','term'];terms+=1
                    if terms==cancel_at:cancelled=True;break
                    events.append('poll')
                if not cancelled:
                    events.append('term');terms+=1;cancelled=terms==cancel_at
                if not cancelled:events.append('deed')
                events+=['unpause','destroy']
                with self.subTest(dismiss_after=dismiss_after,cancel_at=cancel_at):
                    self.assertEqual(run(HELPER,dismiss_after,cancel_at),(not cancelled,','.join(events)))

    def test_normal_join_is_restored_and_changed_evidence_rejects(self):
        data=RData();source=DRAFT.read_text();out,report=recover(source,data)
        self.assertIn('if not showWarehouseFailure() then goto LAB_00db6afd end\n                            quest:SetCreatureBrain(me, "BRAIN_GOOD_VILLAGER_BASE")\n                            __native_entity_state:SetStateInt("MyPhase", 5)\n                            break',out)
        with self.assertRaisesRegex(ValueError,'source correspondence'):
            recover(source.replace('pCVar22 = ""','pCVar22 = "changed"'),data)
        for site in (0xDB5BE1,0xDB5BFF,0xDB5C23,0xDB5DAF,0xDB5DE7):
            def read(address,size):
                raw=data.bytes_at(address,size)
                if raw is not None and address<=site<address+size:
                    raw=bytearray(raw);raw[site-address]^=1;raw=bytes(raw)
                return raw
            with self.assertRaisesRegex(ValueError,'native instructions'):
                recover(source,SimpleNamespace(bytes_at=read,string_at=data.string_at))
