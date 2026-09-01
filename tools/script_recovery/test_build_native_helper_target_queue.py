import json
import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.build_native_helper_target_queue import build


class NativeHelperTargetQueueTests(unittest.TestCase):
    def test_groups_helper_calls_by_exact_target(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / "ir").mkdir()
            (root / "readiness.json").write_text(json.dumps({
                "nativeHelperBacklog": [{"name": "SharedHelper"}]}))
            for script in ("Q_A", "V_B"):
                (root / "ir" / f"{script}.json").write_text(json.dumps({
                    "script": script,
                    "lifecycle": [{"role": "Main", "calls": [{
                        "callee": "SharedHelper", "targetAddress": "0x00123456",
                        "directCallSite": "0x00001000",
                    }]}],
                }))
            rows = build(root / "readiness.json", root / "ir")
            self.assertEqual(len(rows), 1)
            self.assertEqual(rows[0]["targetAddress"], "0x00123456")
            self.assertEqual(rows[0]["calls"], 2)
            self.assertEqual(rows[0]["scripts"], ["Q_A", "V_B"])
            self.assertEqual(len(rows[0]["sites"]), 2)


if __name__ == "__main__":
    unittest.main()
