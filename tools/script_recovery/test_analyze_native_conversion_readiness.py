import json
import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.analyze_native_conversion_readiness import analyze, api_match


class NativeConversionReadinessTests(unittest.TestCase):
    def test_api_match_respects_symbol_boundaries(self):
        self.assertEqual(api_match("global_GetHero_CGameScriptInterface", ["GetHero"]), "GetHero")
        self.assertIsNone(api_match("global_GetHeroic", ["GetHero"]))

    def test_opaque_call_blocks_reconstruction_stage(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / "ir").mkdir()
            (root / "catalog.json").write_text(json.dumps({"scripts": [{"name": "Q_X", "kind": "quest"}]}))
            (root / "manifest.json").write_text(json.dumps({"functions": [{"name": "GetHero"}]}))
            life = [{"role": role, "address": "0x1", "calls": []}
                    for role in ("destructor", "RegisterMain", "Main", "Init", "OnPersist")]
            life[2]["calls"] = [{"callee": "FUN_00123456"},
                                  {"callee": "global_GetHero_CGameScriptInterface"}]
            (root / "ir" / "Q_X.json").write_text(json.dumps({
                "script": "Q_X", "allocatorAddress": "0x1", "vtableAddress": "0x2",
                "evidenceAnchors": [], "lifecycle": life}))
            result = analyze(root / "catalog.json", root / "ir", root / "manifest.json")
            self.assertEqual(result["summary"]["scripts"], 1)
            self.assertEqual(result["scripts"][0]["stage"], "resolve-opaque-callees")
            self.assertEqual(result["scripts"][0]["mappedForgeApis"], ["GetHero"])


if __name__ == "__main__":
    unittest.main()
