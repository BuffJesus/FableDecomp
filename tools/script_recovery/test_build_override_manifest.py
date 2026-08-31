import json
import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.build_override_manifest import build


class OverrideManifestTests(unittest.TestCase):
    def fixture(self, root: Path, evidence: str):
        catalog = root / "catalog.json"
        corpus = root / "corpus.json"
        catalog.write_text(json.dumps({"scripts": [{"name": "V_Test", "kind": "village"}], "seedCorrelations": [{
            "status": "matched", "nativeName": "V_Test", "nativeSection": "S_VT", "package": "Test"
        }]}), encoding="utf-8")
        corpus.write_text(json.dumps({"packages": [{
            "name": "Test", "evidenceLevel": evidence, "archiveSha256": "ABC",
            "registry": [{"file": "Test/Test"}],
        }]}), encoding="utf-8")
        return catalog, corpus

    def test_reconstructed_source_can_only_shadow(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            catalog, corpus = self.fixture(root, "reconstructed-source")
            result = build(catalog, corpus, root / "shadow.json", "shadow")
            self.assertEqual(result["shadow"], 1)
            with self.assertRaisesRegex(ValueError, "refusing override"):
                build(catalog, corpus, root / "override.json", "override")

    def test_lua_shadow_config_defaults_disabled_and_preserves_safety_flags(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            catalog, corpus = self.fixture(root, "reconstructed-source")
            lua_output = root / "retail_shadow.lua"
            build(catalog, corpus, root / "shadow.json", "shadow", lua_output)
            text = lua_output.read_text(encoding="utf-8")
            self.assertIn("enabled = false", text)
            self.assertIn('kind = "village"', text)
            self.assertIn("mutatingCallsAllowed = false", text)
            self.assertIn("saveWritesAllowed = false", text)

    def test_verified_port_can_override(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            catalog, corpus = self.fixture(root, "verified-port")
            result = build(catalog, corpus, root / "override.json", "override")
            self.assertEqual(result["override"], 1)


if __name__ == "__main__":
    unittest.main()
