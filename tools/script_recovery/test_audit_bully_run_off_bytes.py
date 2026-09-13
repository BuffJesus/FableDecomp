import json
import unittest
from pathlib import Path

from tools.script_recovery.audit_bully_run_off_bytes import audit
from tools.script_recovery.export_bully_run_off import export


class BullyRunOffByteAuditTests(unittest.TestCase):
    def test_actual_run_off_observes_termination_raised_inside_run1(self):
        from lupa import LuaRuntime
        root = Path(__file__).resolve().parents[2]
        fse = root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE"
        for given in (False, True):
            with self.subTest(teddy_given=given):
                lua = LuaRuntime(unpack_returned_tuples=True)
                lua.globals().fse_root = fse.as_posix()
                lua.globals().given = given
                lua.execute('package.path = fse_root .. "/?.lua;" .. package.path')
                lua.execute((fse / "NewOakValeIntro/Entities/NOVI_Bully.lua").read_text())
                lua.execute(r'''
                    local function up(fn, name)
                        for i = 1, 100 do
                            local key, value = debug.getupvalue(fn, i)
                            if key == name then return value end
                            if not key then break end
                        end
                        error("missing closure: " .. name)
                    end
                    local run_off = up(up(Main, "handle_hit"), "run_off")
                    local terminating = false
                    local events = {}
                    local function record(s) events[#events + 1] = s end
                    local function actor(name)
                        return {
                            AcquireControl = function() assert(not terminating); return true end,
                            ReleaseControl = function() record("release:" .. name) end,
                            GetPos = function() assert(not terminating); return {x=0,y=0,z=0} end,
                        }
                    end
                    local hero, bully, victim = actor("hero"), actor("bully"), actor("victim")
                    local quest = {
                        GetHero = function() return hero end,
                        GetStateBool = function(_, key) return key == "GivenHeroTeddy" and given end,
                        Log = function() end,
                        StartMovieSequence = function() record("movie:start") end,
                        PauseAllNonScriptedEntities = function(_, b) record("pause:" .. tostring(b)) end,
                        FixMovieSequenceCamera = function(_, b) record("camera:" .. tostring(b)) end,
                        EndMovieSequence = function() record("movie:end") end,
                        RunCutsceneWithSetup = function(_, macro)
                            assert(not terminating, "a second macro was attempted after cancellation")
                            assert(macro == "CS_OAKVALEINTRO_BULLYRUN1")
                            record("run1")
                            terminating = true
                        end,
                        IsActiveThreadTerminating = function() return terminating end,
                    }
                    assert(run_off(quest, bully, victim) == false)
                    assert(table.concat(events, ",") ==
                        "movie:start,pause:true,camera:true,run1,camera:false,pause:false,movie:end,release:victim,release:hero,release:bully")
                ''')

    def test_run1_termination_unwinds_without_success_for_both_teddy_states(self):
        from tools.script_recovery.run_lua_trace import run_trace
        from tools.script_recovery.validate_reconstructed_package import validate_call_assertions
        root = Path(__file__).resolve().parents[2]
        fse = root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE"
        for state in ("teddy_missing", "teddy_given"):
            with self.subTest(state=state):
                name = "entity_bully_run1_terminated_" + state
                spec = json.loads((root / "refs/script_recovery/new_oakvale_intro/fixtures" / (name + ".json")).read_text())
                meta = spec.pop("__meta__")
                trace = run_trace(fse / meta["source"], "Main", "entity", spec, 200_000, fse)
                events = trace["events"]
                self.assertEqual(validate_call_assertions(events, meta, name), [])
                first = next(i for i, e in enumerate(events) if e["name"] == "RunCutsceneWithSetup")
                tail = events[first + 1:]
                self.assertEqual([e["name"] for e in tail], [
                    "IsActiveThreadTerminating", "FixMovieSequenceCamera",
                    "PauseAllNonScriptedEntities", "EndMovieSequence",
                    "ReleaseControl", "ReleaseControl", "ReleaseControl",
                ])
                self.assertEqual([e["receiver"] for e in tail[-3:]],
                                 ["victim", "hero", "me"])
                self.assertEqual(tail[1]["arguments"], [False])
                self.assertEqual(tail[2]["arguments"], [False])

    def test_retail_lua_and_inventory_agree(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(root)
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["retailBytes"], 1161)

    def test_checked_in_snapshot_reproduces_from_installed_exe(self):
        root = Path(__file__).resolve().parents[2]
        exe = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")
        if not exe.exists():
            self.skipTest("installed retail executable is unavailable")
        actual = export(exe)
        expected = json.loads((root / "ghidra_out/script_recovery/bully_run_off_retail_bytes.json").read_text(encoding="utf-8"))
        self.assertEqual(actual, expected)

    def test_normal_and_skipped_macros_are_not_replayed_by_lua(self):
        from tools.script_recovery.run_lua_trace import run_trace
        from tools.script_recovery.validate_reconstructed_package import validate_call_assertions
        root = Path(__file__).resolve().parents[2]
        fse = root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE"
        for name in ("entity_bully_four_hits_run_off", "entity_bully_four_hits_skipped_run_off"):
            with self.subTest(path=name):
                spec = json.loads((root / "refs/script_recovery/new_oakvale_intro/fixtures" / (name + ".json")).read_text())
                meta = spec.pop("__meta__")
                trace = run_trace(fse / meta["source"], "Main", "entity", spec, 200_000, fse)
                events = trace["events"]
                self.assertEqual(validate_call_assertions(events, meta, name), [])
                first_macro = next(i for i, e in enumerate(events) if e["name"] == "RunCutsceneWithSetup")
                self.assertFalse(any(e["name"] in ("MoveToPosition", "EntitySetAsDrawable") for e in events[first_macro:]))
                macros = [e["arguments"][0] for e in events if e["name"] == "RunCutsceneWithSetup"]
                self.assertEqual(macros, ["CS_OAKVALEINTRO_BULLYRUN1", "CS_OAKVALEINTRO_BULLYRUN2"])
                removed = [e for e in events if e["name"] == "RemoveThing"]
                self.assertEqual(len(removed), 1)
                self.assertEqual(removed[0]["arguments"][1:], [False, True])


if __name__ == "__main__":
    unittest.main()
