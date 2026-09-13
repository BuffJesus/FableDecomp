"""Exercise the reconstructed Lua against explicit native control-flow cases.

Native boundaries are archived at 0xE292C0 and 0xE295E0. These tests validate
the reference adapter contract, not FSE playback or the missing runtime adapter.
"""
from pathlib import Path
import json
import unittest
from lupa import LuaRuntime

SOURCE = Path(__file__).resolve().parents[2] / "refs/script_recovery/meet_sister/reference/quest.lua"
HOST = r'''
function make_host(region_wait, acquire_wait, stop_frame, initial_stop, hero_ok, stop_in_macro)
    local h = {events={}, frames=0, region_calls=0, acquire_calls=0, stopped=initial_stop,
               refs={}, paused=false}
    local function emit(self, name, ...)
        local row={name}
        for i=1,select('#',...) do row[#row+1]=tostring(select(i,...)) end
        self.events[#self.events+1]=row
    end
    function h:new_map() emit(self,'map.new'); return {} end
    function h:destroy_map(map)
        for _,resource in pairs(map) do self.refs[resource]=self.refs[resource]-1 end
        emit(self,'map.destroy')
    end
    function h:new_resource(name)
        self.refs[name]=1;emit(self,'resource.new',name);return name
    end
    function h:prepare(resource) emit(self,'prepare',resource) end
    function h:release(resource)
        assert(self.refs[resource]>0,'resource released twice')
        self.refs[resource]=self.refs[resource]-1;emit(self,'release',resource)
    end
    function h:region_loaded(region)
        emit(self,'region',region);self.region_calls=self.region_calls+1
        return self.region_calls>region_wait
    end
    function h:frame()
        emit(self,'frame');self.frames=self.frames+1
        if stop_frame>0 and self.frames>=stop_frame then self.stopped=true end
    end
    function h:terminating() emit(self,'terminating',self.stopped);return self.stopped end
    function h:hero() emit(self,'hero');return 'actual-hero' end
    function h:acquire(thing,resource,priority)
        emit(self,'acquire',thing,resource,priority)
        if resource=='hero' then return hero_ok end
        self.acquire_calls=self.acquire_calls+1
        return self.acquire_calls>acquire_wait
    end
    function h:map_actor(map,name,resource)
        assert(self.refs[resource]>0,'copying a destroyed resource')
        map[name]=resource;self.refs[resource]=self.refs[resource]+1
        emit(self,'map.actor',name,resource)
    end
    function h:start_movie(name) emit(self,'movie.start',name);return 'movie' end
    function h:pause(value) self.paused=value;emit(self,'pause',value) end
    function h:run_macro(name,map,flags,input,setup,skippable)
        emit(self,'macro',name,flags,input,setup,skippable)
        self.macro=name
        assert(map.Hero=='hero')
        assert(map[name=='CS_ARENA_LEAVE_THERESA_01' and 'Messenger' or 'Sister']=='bound')
        if stop_in_macro then self.stopped=true end
    end
    function h:destroy_movie(movie)
        assert(not self.paused,'movie destroyed while entities still paused')
        emit(self,'movie.destroy')
    end
    function h:remove(thing,a,b)
        if self.macro=='CS_ARENA_LEAVE_THERESA_01' then
            assert(self.paused and self.refs.hero==2 and self.refs.bound==2)
        else
            assert(not self.paused and self.refs.hero==0 and self.refs.bound==1)
        end
        emit(self,'remove',thing,a,b)
    end
    function h:active_quest() emit(self,'active');return 'QS_MeetSister' end
    function h:complete(...) emit(self,'complete',...) end
    function h:deactivate_later(...) emit(self,'deactivate',...) end
    function h:fade_in() emit(self,'fade.in') end
    function h:bind(...) emit(self,'bind',...) end
    function h:finalize_bindings() emit(self,'bindings.finalize') end
    function h:set_objective(...) emit(self,'objective',...) end
    return h
end
'''


class MeetSisterReferenceTests(unittest.TestCase):
    def run_lua(self, entry, region_wait=0, acquire_wait=0, stop_frame=0,
                initial_stop=False, hero_ok=True, stop_in_macro=False):
        lua = LuaRuntime(unpack_returned_tuples=True)
        lua.execute(HOST)
        host = lua.globals().make_host(region_wait, acquire_wait, stop_frame,
                                       initial_stop, hero_ok, stop_in_macro)
        quest = lua.execute(SOURCE.read_text())
        quest[entry](host, "bound-instance-not-a-name-lookup")
        self.assertTrue(all(value == 0 for value in host.refs.values()), "resource leak")
        return [[row[i] for i in range(1, len(row)+1)] for row in host.events.values()]

    def test_matches_archived_native_x86_scenarios(self):
        path = SOURCE.parent.parent / "runtime_evidence/native-lua-flow-comparison-20260912.json"
        proof = json.loads(path.read_text())
        self.assertEqual(len(proof["cases"]), 15)
        for case in proof["cases"]:
            with self.subTest(entry=case["entry"], scenario=case["scenario"]):
                self.assertEqual(self.run_lua(case["entry"], **case["options"]), case["events"])

    def test_root_objective_and_binding_order(self):
        self.assertEqual(self.run_lua("main"), [
            ["bind", "MeetSisterMessenger", "MeetSister/Entities/MeetSisterMessenger", "1"],
            ["bind", "MeetSisterSister", "MeetSister/Entities/MeetSisterSister", "0"],
            ["bindings.finalize"], ["active"],
            ["objective", "QS_MeetSister", "TEXT_QUEST_THERESA_MOTHER_INFO_2_SUMMARY",
             "HauntedHouse", "BarrowFields"]])
        self.assertEqual(self.run_lua("init"), [])

    def test_messenger_termination_during_region_wait_allocates_no_resources(self):
        self.assertEqual(self.run_lua("messenger", region_wait=9, stop_frame=1), [
            ["map.new"], ["region", "ArenaExterior"], ["frame"],
            ["terminating", "true"], ["map.destroy"]])

    def test_messenger_already_terminating_after_region_check(self):
        self.assertEqual(self.run_lua("messenger", initial_stop=True), [
            ["map.new"], ["region", "ArenaExterior"], ["terminating", "true"],
            ["map.destroy"]])

    def test_bound_acquisition_retry_and_cleanup(self):
        for entry in ["messenger", "sister"]:
            with self.subTest(entry=entry):
                events = self.run_lua(entry, acquire_wait=9, stop_frame=1)
                self.assertEqual([e for e in events if e[0]=="acquire"], [
                    ["acquire", "bound-instance-not-a-name-lookup", "bound", "4"]])
                self.assertEqual([e for e in events if e[0]=="release"],
                                 [["release", "hero"], ["release", "bound"]])
                self.assertFalse(any(e[0] in ["movie.start", "remove", "complete"] for e in events))

    def test_post_acquire_termination_does_not_start_movie(self):
        events = self.run_lua("sister", initial_stop=True)
        self.assertIn(["acquire", "bound-instance-not-a-name-lookup", "bound", "4"], events)
        self.assertEqual(events[-2:], [["release", "hero"], ["release", "bound"]])
        self.assertNotIn(["hero"], events)

    def test_hero_failed_acquire_is_one_attempt_and_does_not_abort(self):
        for entry in ["messenger", "sister"]:
            with self.subTest(entry=entry):
                events = self.run_lua(entry, hero_ok=False)
                self.assertEqual([e for e in events if e[:2]==["acquire", "actual-hero"]],
                                 [["acquire", "actual-hero", "hero", "4"]])
                self.assertNotIn(["frame"], events)
                self.assertIn(["remove", "bound-instance-not-a-name-lookup", "false", "true"], events)

    def test_messenger_removes_inside_movie_even_if_macro_terminates(self):
        for stop in [False, True]:
            events = self.run_lua("messenger", region_wait=2, acquire_wait=2, stop_in_macro=stop)
            tail = events[next(i for i,e in enumerate(events) if e[0]=="macro"):]
            self.assertEqual(tail, [
                ["macro", "CS_ARENA_LEAVE_THERESA_01", "nil", "nil", "false", "true"],
                ["remove", "bound-instance-not-a-name-lookup", "false", "true"],
                ["pause", "false"], ["movie.destroy"], ["release", "hero"],
                ["release", "bound"], ["map.destroy"]])

    def test_sister_completes_after_movie_and_hero_release_even_if_macro_terminates(self):
        for stop in [False, True]:
            events = self.run_lua("sister", stop_in_macro=stop)
            tail = events[next(i for i,e in enumerate(events) if e[0]=="macro"):]
            self.assertEqual(tail, [
                ["macro", "CS_ARENA_LEAVE_THERESA_02", "nil", "nil", "false", "true"],
                ["pause", "false"], ["movie.destroy"], ["map.destroy"], ["release", "hero"],
                ["remove", "bound-instance-not-a-name-lookup", "false", "true"],
                ["active"], ["complete", "QS_MeetSister", "false", "false", "false"],
                ["active"], ["deactivate", "QS_MeetSister", "0"],
                ["fade.in"], ["release", "bound"]])


if __name__ == "__main__":
    unittest.main()
