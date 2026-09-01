import json
import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.analyze_native_conversion_readiness import (
    LUA_QUEST_BINDING_RE, analyze, api_match, native_helper_category,
)


class NativeConversionReadinessTests(unittest.TestCase):
    def test_native_helper_categories_preserve_lifetime_and_runtime_support(self):
        self.assertEqual(native_helper_category("C3DClothPrimitive::~C3DClothPrimitive"),
                         "object-lifetime")
        self.assertEqual(native_helper_category("CBaseIntelligentPointer::CBaseIntelligentPointer"),
                         "object-lifetime")
        self.assertEqual(native_helper_category("StdMap_Destroy_API"), "runtime-support")
        self.assertEqual(native_helper_category("RunCutsceneMacro_Func"),
                         "engine-or-script-helper")
        self.assertEqual(native_helper_category("BadDonorName", {"destructor"}),
                         "lifecycle-cleanup")

    def test_runtime_binding_extraction(self):
        text = 'questState_type["CloseDoor"] = &LuaQuestState::CloseDoor;'
        self.assertEqual(LUA_QUEST_BINDING_RE.findall(text), ["CloseDoor"])

    def test_runtime_alias_and_host_managed_calls_are_not_reported_missing(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / "ir").mkdir()
            (root / "catalog.json").write_text(json.dumps({"scripts": [{"name": "Q_X", "kind": "quest"}]}))
            (root / "manifest.json").write_text(json.dumps({"functions": []}))
            (root / "methods.tsv").write_text(
                "00100000\tPostAddScriptedEntities\n00100010\tStartScriptingEntity\n")
            (root / "slots.tsv").write_text(
                "vtable_base\toffset\tslot_address\ttarget_address\tcurrent_name\texecutable\n"
                "01260f0c\t0x100\t0126100c\t00100000\tdecorated\ttrue\n"
                "01260f0c\t0x104\t01261010\t00100010\tdecorated\ttrue\n")
            (root / "LuaManager.cpp").write_text(
                'questState_type["FinalizeEntityBindings"] = &LuaQuestState::FinalizeEntityBindings;')
            life = [{"role": role, "address": "0x1", "calls": [], "indirectCalls": []}
                    for role in ("destructor", "RegisterMain", "Main", "Init", "OnPersist")]
            life[2]["indirectCalls"] = [
                {"vtableOffset": "0x100", "interfaceProvenance": "direct-gamescriptinterface-singleton"},
                {"vtableOffset": "0x104", "interfaceProvenance": "direct-gamescriptinterface-singleton"},
            ]
            (root / "ir" / "Q_X.json").write_text(json.dumps({
                "script": "Q_X", "allocatorAddress": "0x1", "vtableAddress": "0x2",
                "evidenceAnchors": [], "lifecycle": life}))
            result = analyze(root / "catalog.json", root / "ir", root / "manifest.json",
                             root / "slots.tsv", root / "methods.tsv",
                             lua_manager_path=root / "LuaManager.cpp")
            self.assertEqual(result["summary"]["missingForgeRuntimeMethods"], [])
            self.assertEqual(result["summary"]["hostManagedInterfaceMethods"],
                             ["StartScriptingEntity"])
            calls = result["scripts"][0]["resolvedInterfaceCalls"]
            self.assertEqual(calls[0]["forgeRuntimeName"], "FinalizeEntityBindings")

    def test_stl_output_method_records_runtime_abi_blocker(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / "ir").mkdir()
            (root / "catalog.json").write_text(json.dumps({"scripts": [{"name": "Q_X", "kind": "quest"}]}))
            (root / "manifest.json").write_text(json.dumps({"functions": []}))
            (root / "methods.tsv").write_text("00100000\tMsgOnLevelLoaded\n")
            (root / "slots.tsv").write_text(
                "vtable_base\toffset\tslot_address\ttarget_address\tcurrent_name\texecutable\n"
                "01260f0c\t0x48\t01260f54\t00100000\tdecorated\ttrue\n")
            life = [{"role": role, "address": "0x1", "calls": [], "indirectCalls": []}
                    for role in ("destructor", "RegisterMain", "Main", "Init", "OnPersist")]
            life[2]["indirectCalls"] = [{
                "vtableOffset": "0x48", "interfaceProvenance": "direct-gamescriptinterface-singleton"}]
            (root / "ir" / "Q_X.json").write_text(json.dumps({
                "script": "Q_X", "allocatorAddress": "0x1", "vtableAddress": "0x2",
                "evidenceAnchors": [], "lifecycle": life}))
            result = analyze(root / "catalog.json", root / "ir", root / "manifest.json",
                             root / "slots.tsv", root / "methods.tsv")
            blockers = result["summary"]["abiBlockedInterfaceMethods"]
            self.assertIn("std::list<CCharString>", blockers["MsgOnLevelLoaded"])

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

    def test_native_helper_backlog_counts_calls_and_consumers(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / "ir").mkdir()
            (root / "catalog.json").write_text(json.dumps({"scripts": [
                {"name": "Q_A", "kind": "quest"}, {"name": "V_B", "kind": "village"}]}))
            (root / "manifest.json").write_text(json.dumps({"functions": []}))
            for script in ("Q_A", "V_B"):
                life = [{"role": role, "address": "0x1", "calls": [], "indirectCalls": []}
                        for role in ("destructor", "RegisterMain", "Main", "Init", "OnPersist")]
                life[2]["calls"] = [{"callee": "SharedHelper"}, {"callee": "SharedHelper"}]
                (root / "ir" / f"{script}.json").write_text(json.dumps({
                    "script": script, "allocatorAddress": "0x1", "vtableAddress": "0x2",
                    "evidenceAnchors": [], "lifecycle": life}))
            result = analyze(root / "catalog.json", root / "ir", root / "manifest.json")
            self.assertEqual(result["summary"]["unresolvedNativeHelperMethods"], 1)
            self.assertEqual(result["summary"]["unresolvedNativeHelperCalls"], 4)
            self.assertEqual(result["nativeHelperBacklog"][0], {
                "name": "SharedHelper", "calls": 4,
                "category": "engine-or-script-helper", "scripts": 2, "roles": ["Main"],
                "kinds": ["quest", "village"], "consumers": ["Q_A", "V_B"]})

    def test_entity_binding_registration_is_structural_evidence_not_helper_backlog(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / "ir").mkdir()
            (root / "catalog.json").write_text(json.dumps({"scripts": [
                {"name": "Q_A", "kind": "quest"}]}))
            (root / "manifest.json").write_text(json.dumps({"functions": []}))
            life = [{"role": role, "address": "0x1", "calls": [], "indirectCalls": []}
                    for role in ("destructor", "RegisterMain", "Main", "Init", "OnPersist")]
            life[2]["calls"] = [{"callee": "CScriptBase::AddEntityScriptBinding"}]
            life[2]["entityBindings"] = [{"entityName": "Messenger", "complete": True}]
            (root / "ir" / "Q_A.json").write_text(json.dumps({
                "script": "Q_A", "allocatorAddress": "0x1", "vtableAddress": "0x2",
                "evidenceAnchors": [], "lifecycle": life}))
            result = analyze(root / "catalog.json", root / "ir", root / "manifest.json")
            self.assertEqual(result["summary"]["entityBindings"], 1)
            self.assertEqual(result["summary"]["completeEntityBindings"], 1)
            self.assertEqual(result["summary"]["unresolvedNativeHelperMethods"], 0)

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
