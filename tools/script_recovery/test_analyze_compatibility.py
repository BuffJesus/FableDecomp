import json
import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.analyze_compatibility import analyze


class CompatibilityTests(unittest.TestCase):
    def test_missing_api_ranking_and_trace(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / "ir").mkdir()
            (root / "ir/test.json").write_text(json.dumps({
                "kind": "entity", "functions": [{"name": "Main", "calls": [
                    {"receiver": "quest", "name": "Missing", "line": 2},
                    {"receiver": "me", "name": "Speak", "line": 3},
                ]}],
            }), encoding="utf-8")
            (root / "corpus_index.json").write_text(json.dumps({"packages": [{"name": "P", "files": [
                {"path": "FSE/P.lua", "ir": "ir/test.json"}
            ]}]}), encoding="utf-8")
            (root / "validation.json").write_text(json.dumps([{
                "package": "P", "path": "FSE/P.lua", "severity": "error", "code": "unknown-api",
                "message": "quest API 'Missing' is not present in the ForgeFSE manifest",
            }]), encoding="utf-8")
            summary = analyze(root, root / "result.json", root / "result.md")
            self.assertEqual(summary["missingApis"], 1)
            self.assertEqual(summary["totalCalls"], 2)


if __name__ == "__main__":
    unittest.main()

