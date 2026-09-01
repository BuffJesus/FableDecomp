import json
import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.generate_native_helper_lua import generate
from tools.script_recovery.validate_native_helper_lua import validate


class NativeHelperLuaTests(unittest.TestCase):
    def test_generated_switch_executes_native_truth_table(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            helper_ir = root / "ir.json"
            helper_ir.write_text(json.dumps({"helpers": [{
                "targetAddress": "0x00100000", "currentName": "Choose",
                "decompileSha256": "A" * 64, "luaEmissionReady": True,
                "semanticPatterns": [{"kind": "constant-return-switch", "complete": True,
                                      "parameter": "param_1",
                                      "cases": [{"input": 1, "return": 36}],
                                      "defaultReturn": 35}],
            }]}))
            manifest = generate(helper_ir, root / "generated")
            self.assertEqual(manifest["summary"], {"emitted": 1, "deploymentEligible": 0})
            result = validate(root / "generated" / "manifest.json")
            self.assertTrue(result["summary"]["complete"])
            self.assertEqual(result["summary"]["checks"], 4)

    def test_generated_initializer_preserves_offset_width_and_value(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            helper_ir = root / "ir.json"
            helper_ir.write_text(json.dumps({"helpers": [{
                "targetAddress": "0x00100010", "currentName": "WrongDonor::Init",
                "decompileSha256": "B" * 64, "luaEmissionReady": True,
                "semanticPatterns": [{"kind": "native-field-initializer", "complete": True,
                                      "luaFieldNamesResolved": False, "writes": [
                                          {"fieldOffset": "0x48", "width": 1,
                                           "valueExpression": "0x0"},
                                          {"fieldOffset": "0x50", "width": 4,
                                           "valueExpression": "0xffffffff"}]}],
            }]}))
            generate(helper_ir, root / "generated")
            result = validate(root / "generated" / "manifest.json")
            self.assertTrue(result["summary"]["complete"])
            self.assertEqual(result["summary"]["checks"], 2)

    def test_generated_live_vector_removal_executes_both_flag_cases(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            helper_ir = root / "ir.json"
            helper_ir.write_text(json.dumps({"helpers": [{
                "targetAddress": "0x00CBED82", "currentName": "KillAllThingsInVector",
                "decompileSha256": "C" * 64, "luaEmissionReady": True,
                "semanticPatterns": [{"kind": "remove-live-things-in-vector",
                                      "complete": True, "elementDwords": 3,
                                      "isAliveVtableOffset": "0x12c",
                                      "removeThingVtableOffset": "0x1b0",
                                      "removeFlagParameter": "param_3",
                                      "finalFlag": True}],
            }]}))
            generate(helper_ir, root / "generated")
            result = validate(root / "generated" / "manifest.json")
            self.assertTrue(result["summary"]["complete"])
            self.assertEqual(result["summary"]["checks"], 2)

    def test_generated_optional_resource_call_executes_both_guard_branches(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            helper_ir = root / "ir.json"
            helper_ir.write_text(json.dumps({"helpers": [{
                "targetAddress": "0x007E7410", "currentName": "ClearActions",
                "decompileSha256": "D" * 64, "luaEmissionReady": True,
                "semanticPatterns": [{"kind": "optional-resource-virtual-call",
                                      "complete": True,
                                      "resourcePointerOffset": "0x8",
                                      "resourceVtableOffset": "0x58",
                                      "operation":
                                      "ClearAllActionsIncludingLoopingAnimations"}],
            }]}))
            generate(helper_ir, root / "generated")
            result = validate(root / "generated" / "manifest.json")
            self.assertTrue(result["summary"]["complete"])
            self.assertEqual(result["summary"]["checks"], 2)


if __name__ == "__main__":
    unittest.main()
