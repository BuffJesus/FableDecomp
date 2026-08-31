import json
import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.audit_forgefse_runtime import audit, extract_bindings


class RuntimeAuditTests(unittest.TestCase):
    def test_extract_and_scope_audit(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            manager = root / "LuaManager.cpp"
            manager.write_text('questState_type["SetFlag"] = x;\ncscriptThing_type["Speak"] = y;', encoding="utf-8")
            self.assertIn(("Entity", "Speak"), extract_bindings(manager))
            compatibility = root / "compat.json"
            compatibility.write_text(json.dumps({"calls": [
                {"scope": "Quest", "name": "SetFlag", "count": 1, "consumers": ["A"]},
                {"scope": "Entity", "name": "SetFlag", "count": 1, "consumers": ["B"]},
                {"scope": "Quest", "name": "Unknown", "count": 1, "consumers": ["C"]},
            ]}), encoding="utf-8")
            summary = audit(compatibility, manager, root / "out.json", root / "out.md")
            self.assertEqual(summary["callable"], 1)
            self.assertEqual(summary["wrongScope"], 1)
            self.assertEqual(summary["missing"], 1)


if __name__ == "__main__":
    unittest.main()

