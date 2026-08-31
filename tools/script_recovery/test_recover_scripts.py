import json
import tempfile
import unittest
import zipfile
from pathlib import Path

from tools.script_recovery.recover_scripts import classify, ingest, parse_lua_ir, parse_registry, validate_script


class ScriptRecoveryTests(unittest.TestCase):
    def test_classification_is_not_quest_only(self):
        self.assertEqual(classify("Q_Test"), "quest")
        self.assertEqual(classify("V_Test"), "village")
        self.assertEqual(classify("Anything", entity=True), "entity")
        self.assertEqual(classify("Gameflow"), "global")
        self.assertEqual(classify("Expression_Dig"), "expression")
        self.assertEqual(classify("CS_PlayCutscene"), "cutscene_host")
        self.assertEqual(classify("PersonalScript_Empty"), "personal")
        self.assertEqual(classify("TestQuestCard1"), "test")

    def test_registry_and_ir(self):
        registry = '''Quests = { Q_Test = { name="Test", file="Test/Test", id=1001,
          entity_scripts = { { name="Door", file="Test/Entities/Door", id=64 }, }\n }\n}'''
        rows = parse_registry(registry)
        self.assertEqual(rows[0]["entities"][0]["name"], "Door")
        ir = parse_lua_ir("FSE/Test/Test.lua", '''
function Main(quest)
 quest:SetStateBool("Done", true)
 quest:AddEntityBinding("Door", "Test/Entities/Door")
 quest:NewScriptFrame()
end
function OnPersist(quest, context)
 quest:PersistTransferBool(context, "Done", false)
end
''', "quest")
        self.assertEqual(ir["state"][0]["name"], "Done")
        self.assertTrue(ir["state"][0]["persisted"])
        self.assertEqual(ir["entityBindings"][0]["name"], "Door")

    def test_ingest_rejects_duplicate_ids(self):
        manifest = {"functions": [
            {"name": "NewScriptFrame", "scope": "Quest"},
            {"name": "SetStateBool", "scope": "Quest"},
        ]}
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            api = root / "api.json"
            api.write_text(json.dumps(manifest), encoding="utf-8")
            archives = []
            for number in (1, 2):
                archive = root / f"p{number}.zip"
                with zipfile.ZipFile(archive, "w") as zf:
                    zf.writestr("FSE/quests.lua", f'''Quests = {{ Q_T{number} = {{ name="T{number}", file="T{number}/T{number}", id=1001, entity_scripts={{}}\n }}\n}}''')
                    zf.writestr(f"FSE/T{number}/T{number}.lua", "function Main(quest) quest:NewScriptFrame() end")
                archives.append(archive)
            result = ingest(archives, root / "out", api)
            self.assertEqual(result["errors"], 1)

    def test_wrong_receiver_is_not_reported_as_missing_api(self):
        ir = parse_lua_ir("E.lua", "function Main(quest, me) me:SetFlag(false) end", "entity")
        findings = validate_script(ir, {("quest", "SetFlag"): {}})
        self.assertEqual(findings[0]["code"], "wrong-api-scope")


if __name__ == "__main__":
    unittest.main()
