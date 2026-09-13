import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.audit_forgefse_speech_termination import audit


class SpeechTerminationAuditTests(unittest.TestCase):
    def make_tree(self, root: Path, termination_result: str) -> Path:
        folder = root / "FableScriptExtender"
        folder.mkdir()
        (folder / "LuaEntityAPI.h").write_text("bool Speak_Blocking(...);", encoding="utf-8")
        (root / "ALL-INTERFACE-FUNCTIONS-FOR-FSE.h").write_text(
            "bool Speak_Blocking(...);", encoding="utf-8")
        (folder / "ManifestFunctionRegistry.generated.h").write_text(
            '{"Speak_Blocking", "Entity", "bool"}', encoding="utf-8")
        (folder / "LuaManager.cpp").write_text(
            "return entityAPI->Speak_Blocking(a);\n" * 3, encoding="utf-8")
        (folder / "LuaEntityAPI.cpp").write_text(
            "bool LuaEntityAPI::Speak_Blocking(...) {\n"
            " while (task) { frame(); if (IsThreadTerminating()) {\n"
            "  Log(\"Thread terminating during speech wait\");\n"
            f"  {termination_result};\n"
            " } }\n return true;\n}\n"
            "void LuaEntityAPI::Converse_NonBlocking() {}\n", encoding="utf-8")
        return root

    def test_accepts_false_termination_result(self):
        with tempfile.TemporaryDirectory() as tmp:
            self.assertTrue(audit(self.make_tree(Path(tmp), "return false"))["ok"])

    def test_rejects_hidden_termination_result(self):
        with tempfile.TemporaryDirectory() as tmp:
            result = audit(self.make_tree(Path(tmp), "break"))
            self.assertFalse(result["ok"])
            self.assertFalse(result["checks"]["terminationReturnsFalse"])


if __name__ == "__main__":
    unittest.main()
