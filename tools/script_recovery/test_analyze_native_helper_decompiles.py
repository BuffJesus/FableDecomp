import json
import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.analyze_native_helper_decompiles import analyze, semantic_patterns


class NativeHelperDecompileTests(unittest.TestCase):
    def test_typed_native_reads_are_complete_semantic_patterns(self):
        field = semantic_patterns(
            "bool F(X *this) { return *(int *)(this + 8) != 0; }", [], [])
        self.assertEqual(field[0], {"kind": "native-field-return", "fieldOffset": "0x8",
                                   "accessor": "read_i32", "resultTransform": "not-zero",
                                   "complete": True, "luaFieldNameResolved": False})
        global_read = semantic_patterns("long F(void) { return DAT_0143e920; }", [], [])
        self.assertEqual(global_read[0]["globalAddress"], "0x0143E920")
        constant = semantic_patterns("long F(void) { return 0; }", [], [])
        self.assertEqual(constant[0], {"kind": "constant-return", "return": 0,
                                      "complete": True})

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

    def test_complete_constant_switch_is_lua_emission_ready(self):
        with tempfile.TemporaryDirectory() as temp:
            source = Path(temp) / "helpers.json"
            source.write_text(json.dumps({"helpers": [{
                "targetAddress": "0x1000", "currentName": "Choose", "helperNames": "Choose",
                "category": "engine-or-script-helper", "calls": 1, "scripts": 1,
                "roles": "Main", "status": "decompiled", "error": None, "directCalls": [],
                "decompile": "int Choose(int param_1) { switch(param_1) { default: return 0x23; case 1: return 0x24; case 2: return 0x25; } }",
            }]}))
            row = analyze(source)["helpers"][0]
            self.assertTrue(row["luaEmissionReady"])
            self.assertEqual(row["semanticPatterns"][0]["cases"], [
                {"input": 1, "return": 0x24}, {"input": 2, "return": 0x25}])
            self.assertEqual(row["semanticPatterns"][0]["defaultReturn"], 0x23)

    def test_initializer_owner_requires_exact_main_this_call(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / "ir").mkdir()
            (root / "clusters").mkdir()
            source = root / "helpers.json"
            source.write_text(json.dumps({"helpers": [{
                "targetAddress": "0x1000", "currentName": "WrongDonor::Init",
                "helperNames": "WrongDonor::Init", "category": "engine-or-script-helper",
                "calls": 1, "scripts": 1, "roles": "Main", "status": "decompiled",
                "error": None, "directCalls": [],
                "decompile": "void Init(X *this) { this[0x48] = (X)0x0; }",
            }]}))
            body = "void Main(Q_Real *this) { WrongDonor::Init((WrongDonor *)this); }"
            offset = body.index("WrongDonor::Init")
            lifecycle = [{"role": "Main", "calls": [{
                "callee": "WrongDonor::Init", "offset": offset,
                "targetAddress": "0x1000", "directCallSite": "0x2000"}]}]
            (root / "ir" / "Q_Real.json").write_text(json.dumps({
                "script": "Q_Real", "lifecycle": lifecycle}))
            (root / "clusters" / "Q_Real.json").write_text(json.dumps({
                "lifecycle": [{"role": "Main", "decompile": body}]}))
            row = analyze(source, root / "ir", root / "clusters")["helpers"][0]
            self.assertEqual(row["parentInitializerEvidence"]["script"], "Q_Real")
            self.assertIn("this", row["consumers"][0]["statement"])

    def test_helper_interface_call_resolves_only_with_provenance(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            source = root / "helpers.json"
            source.write_text(json.dumps({"helpers": [{
                "targetAddress": "0x1000", "currentName": "Wrapper", "helperNames": "Wrapper",
                "category": "engine-or-script-helper", "calls": 1, "scripts": 1,
                "roles": "Main", "status": "decompiled", "error": None, "directCalls": [],
                "decompile": "void F(X *this) { (**(code **)(**(int **)(this + 0x40) + 0x504))(0); }",
            }]}))
            slots = root / "slots.tsv"
            slots.write_text("vtable_base\toffset\tslot_address\ttarget_address\tcurrent_name\texecutable\n"
                             "01260f0c\t0x504\t01261410\t00100000\tbad\ttrue\n")
            catalog = root / "catalog.tsv"
            catalog.write_text("00100000\tRemoveQuestCardFromHero\n")
            row = analyze(source, slots_path=slots,
                          interface_catalog_path=catalog)["helpers"][0]
            self.assertEqual(row["resolvedInterfaceCalls"][0]["name"],
                             "RemoveQuestCardFromHero")


if __name__ == "__main__":
    unittest.main()
