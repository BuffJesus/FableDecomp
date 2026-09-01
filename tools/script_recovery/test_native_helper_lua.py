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
            self.assertEqual(result["summary"]["cases"], 4)


if __name__ == "__main__":
    unittest.main()
