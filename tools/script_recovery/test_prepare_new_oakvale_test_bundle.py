import json
import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.prepare_new_oakvale_test_bundle import prepare


class PrepareNewOakvaleBundleTests(unittest.TestCase):
    def test_emits_single_authority_runnable_profile(self):
        with tempfile.TemporaryDirectory() as temp:
            base = Path(temp)
            root = base / "root"
            forge = base / "forge"
            source = root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE"
            profile = root / "refs/script_recovery/new_oakvale_intro/runtime_playtest"
            source.mkdir(parents=True)
            profile.mkdir(parents=True)
            (source / "quests.lua").write_text("Quests = { Q_NewOakValeIntro = {} }\n", encoding="utf-8")
            (source / "retail_override.lua").write_text("RetailOverrides = { enabled = false }\n", encoding="utf-8")
            (profile / "quests.lua").write_text("Quests = {}\n", encoding="utf-8")
            (profile / "retail_override.lua").write_text(
                'RetailOverrides = { enabled = false, allowUnverifiedDisposable = true, '
                'disposableSaveAcknowledgement = "I UNDERSTAND THIS OVERRIDE MAY CORRUPT DISPOSABLE SAVES" }\n',
                encoding="utf-8")
            (forge / "Release").mkdir(parents=True)
            (forge / "Release/FableScriptExtender.dll").write_bytes(b"dll")

            output = base / "bundle"
            manifest = prepare(root, forge, output)

            self.assertIn("Quests = {}", (output / "FSE/quests.lua").read_text(encoding="utf-8"))
            self.assertIn("enabled = true", (output / "FSE/retail_override.lua").read_text(encoding="utf-8"))
            self.assertEqual(manifest["authority"], {
                "mode": "identity-preserving-retail-override",
                "nativeName": "Q_NewOakValeIntro",
                "overrideEnabled": True,
                "standaloneQuestRegistered": False,
            })
            saved = json.loads((output / "bundle_manifest.json").read_text(encoding="utf-8"))
            self.assertEqual(saved["schema"], "new-oakvale-test-bundle/0.2")


if __name__ == "__main__":
    unittest.main()
