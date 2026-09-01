import json
import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.analyze_native_helper_decompiles import analyze


class NativeHelperDecompileTests(unittest.TestCase):
    def test_reduces_helper_body_and_exact_dependencies(self):
        with tempfile.TemporaryDirectory() as temp:
            source = Path(temp) / "helpers.json"
            source.write_text(json.dumps({"helpers": [{
                "targetAddress": "0x1000", "currentName": "Helper", "helperNames": "Helper",
                "category": "engine-or-script-helper", "calls": 3, "scripts": 2,
                "roles": "Main", "status": "decompiled", "error": None,
                "directCalls": [{"site": "0x1010", "target": "0x2000",
                                 "currentName": "Dependency"}],
                "decompile": 'void Helper(void) { if (ready) Dependency(); Use("literal"); }',
            }]}))
            result = analyze(source)
            self.assertEqual(result["summary"]["targets"], 1)
            self.assertEqual(result["summary"]["directDependencyCalls"], 1)
            row = result["helpers"][0]
            self.assertEqual(row["calls"][0]["targetAddress"], "0x2000")
            self.assertEqual(row["strings"], ["literal"])
            self.assertEqual(row["controlFlow"], {"if": 1})

    def test_failed_decompile_remains_explicit(self):
        with tempfile.TemporaryDirectory() as temp:
            source = Path(temp) / "helpers.json"
            source.write_text(json.dumps({"helpers": [{
                "targetAddress": "0x1000", "currentName": "Huge", "helperNames": "Huge",
                "category": "engine-or-script-helper", "calls": 1, "scripts": 1,
                "roles": "Main", "status": "failed", "error": "timeout",
                "directCalls": [], "decompile": None,
            }]}))
            result = analyze(source)
            self.assertEqual(result["summary"]["failed"], 1)
            self.assertEqual(result["helpers"][0]["stage"], "repair-helper-decompile")


if __name__ == "__main__":
    unittest.main()
