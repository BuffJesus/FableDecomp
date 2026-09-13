import json
import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.audit_lua_gameflow import audit


class AuditLuaGameflowTests(unittest.TestCase):
    def test_exact_stage_and_dispatch_contract(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            lua = root / "flow.lua"
            cluster = root / "native.json"
            lua.write_text(
                "local EGP_INTRO = 0\nlocal EGP_TRAINING = 100\n"
                "function OnPersist(q,c) PostSavePosition=1 CoreQuestWaiting=1 end\n"
                "function CoreQuestReminder() end\n"
                "function RunStage_Intro(quest) quest:SetMasterGameState(\"PostSavePosition\", EGP_INTRO) return EGP_TRAINING end\n"
                "function RunStage_Training(quest) quest:SetMasterGameState(\"PostSavePosition\", EGP_TRAINING) end\n"
                "function Main() end\n"
                "local H = {[EGP_INTRO]=RunStage_Intro,[EGP_TRAINING]=RunStage_Training}\n",
                encoding="utf-8",
            )
            cluster.write_text(json.dumps({"lifecycle": [
                {"role": "Main", "decompile": "+ 4) = 0; + 4) = 100;"},
                {"role": "OnPersist", "decompile": "PostSavePosition CoreQuestWaiting SavedScriptNames SavedCardDefNames"},
            ]}), encoding="utf-8")
            report = audit(lua, cluster)
            self.assertTrue(report["ok"])
            self.assertEqual(report["stageContract"]["nativeValues"], [0, 100])
            self.assertFalse(report["persistenceContract"]["luaFields"]["SavedScriptNames"])

    def test_missing_native_stage_fails(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            lua = root / "flow.lua"
            cluster = root / "native.json"
            lua.write_text("local EGP_INTRO = 0\nfunction OnPersist() end\nfunction CoreQuestReminder() end\nfunction RunStage_Intro(quest) quest:SetMasterGameState(\"PostSavePosition\", EGP_INTRO) end\nfunction Main() end\nlocal H={[EGP_INTRO]=RunStage_Intro}\n", encoding="utf-8")
            cluster.write_text(json.dumps({"lifecycle": [
                {"role": "Main", "decompile": "+ 4) = 0; + 4) = 100;"},
                {"role": "OnPersist", "decompile": ""},
            ]}), encoding="utf-8")
            self.assertFalse(audit(lua, cluster)["ok"])


if __name__ == "__main__":
    unittest.main()
