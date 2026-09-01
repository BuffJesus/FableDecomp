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

    def test_interface_mapping_requires_provenance(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / "ir").mkdir()
            (root / "catalog.json").write_text(json.dumps({"scripts": [{"name": "Q_X", "kind": "quest"}]}))
            (root / "manifest.json").write_text(json.dumps({"functions": [{"name": "GetHero"}]}))
            (root / "methods.tsv").write_text("00891ca0\tGetHero\n")
            (root / "slots.tsv").write_text(
                "vtable_base\toffset\tslot_address\ttarget_address\tcurrent_name\texecutable\n"
                "01260f0c\t0x118\t01261024\t00891ca0\tdecorated\ttrue\n")
            life = [{"role": role, "address": "0x1", "calls": [], "indirectCalls": []}
                    for role in ("destructor", "RegisterMain", "Main", "Init", "OnPersist")]
            life[2]["indirectCalls"] = [
                {"vtableOffset": "0x118", "interfaceProvenance": "direct-gamescriptinterface-singleton"},
                {"vtableOffset": "0x118", "interfaceProvenance": None},
            ]
            (root / "ir" / "Q_X.json").write_text(json.dumps({
                "script": "Q_X", "allocatorAddress": "0x1", "vtableAddress": "0x2",
                "evidenceAnchors": [], "lifecycle": life}))
            result = analyze(root / "catalog.json", root / "ir", root / "manifest.json",
                             root / "slots.tsv", root / "methods.tsv")
            self.assertEqual(result["summary"]["resolvedInterfaceCalls"], 1)
            self.assertEqual(result["scripts"][0]["resolvedInterfaceMethods"], ["GetHero"])

    def test_script_field_mapping_requires_allocator_evidence(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / "ir").mkdir()
            (root / "catalog.json").write_text(json.dumps({"scripts": [{"name": "Q_X", "kind": "quest"}]}))
            (root / "manifest.json").write_text(json.dumps({"functions": [{"name": "GetHero"}]}))
            (root / "methods.tsv").write_text("00891ca0\tGetHero\n")
            (root / "slots.tsv").write_text(
                "vtable_base\toffset\tslot_address\ttarget_address\tcurrent_name\texecutable\n"
                "01260f0c\t0x118\t01261024\t00891ca0\tdecorated\ttrue\n")
            (root / "fields.tsv").write_text(
                "script\tallocator_address\tinterface_store\tmethod\tverified\n"
                "Q_X\t00100000\t00100020\tallocator-register-flow\ttrue\n")
            life = [{"role": role, "address": "0x1", "calls": [], "indirectCalls": []}
                    for role in ("destructor", "RegisterMain", "Main", "Init", "OnPersist")]
            life[2]["indirectCalls"] = [{
                "vtableOffset": "0x118",
                "interfaceProvenance": "script-instance-gamescriptinterface-field",
            }]
            (root / "ir" / "Q_X.json").write_text(json.dumps({
                "script": "Q_X", "allocatorAddress": "0x1", "vtableAddress": "0x2",
                "evidenceAnchors": [], "lifecycle": life}))
            without = analyze(root / "catalog.json", root / "ir", root / "manifest.json",
                              root / "slots.tsv", root / "methods.tsv")
            with_evidence = analyze(root / "catalog.json", root / "ir", root / "manifest.json",
                                    root / "slots.tsv", root / "methods.tsv", root / "fields.tsv")
            self.assertEqual(without["summary"]["resolvedInterfaceCalls"], 0)
            self.assertEqual(with_evidence["summary"]["resolvedInterfaceCalls"], 1)


if __name__ == "__main__":
    unittest.main()
