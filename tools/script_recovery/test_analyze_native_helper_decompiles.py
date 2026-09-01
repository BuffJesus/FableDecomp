import json
import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.analyze_native_helper_decompiles import analyze, semantic_patterns


class NativeHelperDecompileTests(unittest.TestCase):
    def test_archery_interface_sequences_are_complete_semantic_patterns(self):
        remove = semantic_patterns(
            "void F(X *this) { (**(code **)(**(int **)(this + 0x40) + 0x504))(0); "
            "(**(code **)(**(int **)(this + 0x40) + 0x548))(*(undefined4 *)(this + 0x58)); "
            "(**(code **)(**(int **)(this + 0x40) + 0x548))(*(undefined4 *)(this + 0x5c)); "
            "(**(code **)(**(int **)(this + 0x40) + 0x548))(*(undefined4 *)(this + 0x60)); return; }",
            [], [{"vtableOffset": offset} for offset in ("0x504", "0x548", "0x548", "0x548")],
            "NScript::CV_ArcheryCompetitionScript::RemoveArcheryQuestInfo")
        self.assertEqual(remove[0]["kind"], "quest-interface-sequence")
        self.assertEqual([row["method"] for row in remove[0]["operations"]],
                         ["DisplayQuestInfo", "RemoveQuestInfoElement",
                          "RemoveQuestInfoElement", "RemoveQuestInfoElement"])

        high_score = semantic_patterns(
            "void F(X *this,long param_1) { int iVar1; iVar1 = -1; "
            "(**(code **)(**(int **)(this + 0x40) + 0x53c))(*(undefined4 *)(this + 0x58),param_1,0xffffffff); "
            "(**(code **)(**(int **)(this + 0x40) + 0xb54))((float)iVar1); return; }",
            [], [{"vtableOffset": "0x53c"}, {"vtableOffset": "0xb54"}],
            "NScript::CV_ArcheryCompetitionScript::UpdateHighScore")
        self.assertEqual(high_score[0]["parameters"], ["param_1"])
        self.assertEqual(high_score[0]["operations"][1]["method"],
                         "UpdateOnlineScore_Archery")

    def test_conditional_native_call_and_byte_clear_is_complete(self):
        patterns = semantic_patterns(
            "void F(X *this) { if (this[0x25] != (X)0x0) { Helper(this[0x24]); "
            "this[0x25] = (X)0x0; } return; }",
            [{"site": "0x1010", "target": "0x2000"}], [],
            "CTCVillage::OnInitialActivate")
        self.assertEqual(patterns, [{
            "kind": "conditional-u8-call-clear", "complete": True,
            "conditionOffset": "0x25", "argumentOffset": "0x24",
            "callTarget": "0x2000", "callSite": "0x1010"}])

    def test_script_initializer_preserves_order_widths_and_nested_write(self):
        body = "void F(X *this) { " + " ".join([
            'CCharString::operator=((CCharString *)(this + 0x58),"A");',
            'CCharString::operator=((CCharString *)(this + 0x5c),"B");',
            'CCharString::operator=((CCharString *)(this + 0x60),"C");',
            'CCharString::operator=((CCharString *)(this + 100),"D");',
            *[f"this[0x{offset:x}] = (X)0x0;" for offset in
              (0x4a, 0x48, 0x4b, 0x4c, 0x49, 0x4d, 0x4e, 0x4f, 0x50, 0x55, 0x56, 0x57, 0x68)],
            "*(undefined4 *)(this + 0x6c) = 0;", "this[0x70] = (X)0x0;",
            "*(undefined1 *)(*(int *)(this + 0x44) + 0x11c) = 0;", "return; }"])
        direct = [{"currentName": "CCharString::operator="} for _ in range(4)]
        pattern = semantic_patterns(body, direct, [],
            "NScript::CQ_OpeningGraveyardSecretPassageScript::InitializeVariables")[0]
        self.assertEqual(pattern["kind"], "native-script-initializer")
        self.assertEqual(len(pattern["operations"]), 20)
        self.assertEqual(pattern["operations"][3],
                         {"kind": "set-string", "offset": "0x64", "value": "D"})
        self.assertEqual(pattern["operations"][-1]["kind"], "write-nested-u8")

    def test_arena_initializer_preserves_exact_native_global_argument(self):
        assignments = " ".join(
            f'CCharString::operator=((CCharString *)(this + 0x{0x48 + index * 4:x}),"S{index}");'
            for index in range(20))
        body = ("void F(X *this) { std_vector_Conversation_Assign2(DAT_0143e90c + 0x1044); "
                + assignments + " return; }")
        direct = [{"target": "0x00F25980", "currentName": "std_vector_Conversation_Assign2"},
                  *({"target": "0x0099EFE0", "currentName": "CCharString::operator="}
                    for _ in range(20))]
        pattern = semantic_patterns(body, direct, [],
            "NScript::CQ_ArenaScript::InitialiseVariables")[0]
        self.assertEqual(len(pattern["operations"]), 21)
        self.assertEqual(pattern["operations"][0], {
            "kind": "invoke-native", "target": "0x00F25980", "arguments": [0x0143F950]})

    def test_archery_setup_records_instruction_call_sites(self):
        body = ("HUD_ICON_MULTI_ARROW HUD_ICON_ARROW HUD_CLOCK_ICON "
                "*(int *)(this + 0x58) *(int *)(this + 0x5c) *(int *)(this + 0x60) "
                "this + 0x6c *(int *)(*(int *)(this + 0x44) + 0x44)")
        pattern = semantic_patterns(body, [{} for _ in range(6)],
            [{"vtableOffset": value} for value in
             ("0x51c", "0x51c", "0x518", "0x53c", "0xb54", "0x504")],
            "NScript::CV_ArcheryCompetitionScript::AddArcheryQuestInfo")[0]
        self.assertEqual(pattern["kind"], "archery-quest-info-setup")
        self.assertEqual([row["site"] for row in pattern["instructionEvidence"]],
                         ["0x00E33065", "0x00E33098", "0x00E330CD",
                          "0x00E330F5", "0x00E33108", "0x00E33115"])

    def test_conditional_strided_copy_loop_is_complete(self):
        body = ("this[0x50] this + 0x5c this + 0xa8 "
                "iVar3 = iVar3 + 4; pCVar2 = pCVar2 + 0x10; iVar3 < 0x14")
        direct = [{"target": "0x00CB7940", "site": "0x1"},
                  {"target": "0x00CB7940", "site": "0x2"}]
        pattern = semantic_patterns(body, direct, [],
            "NScript::CV_AssassinAttacksScript::InitialiseRegionSpecificInfo")[0]
        self.assertEqual(pattern["kind"], "conditional-strided-copy-loop")
        self.assertEqual((pattern["elements"], pattern["destinationStride"],
                          pattern["sourceStride"]), (5, 0x10, 4))

    def test_live_vector_removal_preserves_both_virtual_operations(self):
        body = ("for (piVar2 = begin; piVar2 != end; piVar2 = piVar2 + 3) { "
                "cVar1 = (**fn)(*piVar2 + 300)(); if (cVar1 != '\\0') { "
                "(**remove)(piVar2,_param_3,1); } }")
        indirect = [{"vtableOffset": "0x12c"}, {"vtableOffset": "0x1b0"}]
        pattern = semantic_patterns(body, [], indirect, "KillAllThingsInVector")[0]
        self.assertEqual(pattern["kind"], "remove-live-things-in-vector")
        self.assertEqual(pattern["elementDwords"], 3)
        self.assertEqual(pattern["removeThingVtableOffset"], "0x1b0")
        self.assertTrue(pattern["finalFlag"])

    def test_optional_resource_virtual_call_preserves_guard_and_slot(self):
        body = ("if (*(int **)(this + 8) != (int *)0x0) { "
                "(**(code **)(**(int **)(this + 8) + 0x58))(); return; }")
        pattern = semantic_patterns(body, [], [{"vtableOffset": "0x58"}],
            "CScriptThing::?ClearAllActionsIncludingLoopingAnimations@CScriptThing@@UAEXXZ")[0]
        self.assertEqual(pattern, {
            "kind": "optional-resource-virtual-call", "complete": True,
            "resourcePointerOffset": "0x8", "resourceVtableOffset": "0x58",
            "operation": "ClearAllActionsIncludingLoopingAnimations"})

    def test_speak_overloads_preserve_forwarding_slots(self):
        body = ("if (*(int **)(this + 8) != (int *)0x0) { "
                "(**(code **)(**(int **)(this + 8) + 0x34))(); }")
        pattern = semantic_patterns(body, [], [{"vtableOffset": "0x34"}],
            "Thing::?Speak@Thing@@UAEXABVThing@@PBD@Z")[0]
        self.assertEqual(pattern["kind"], "optional-resource-forward-virtual-call")
        self.assertEqual((pattern["operation"], pattern["resourceVtableOffset"]),
                         ("SpeakCString", "0x34"))

    def test_get_script_thing_preserves_empty_and_delegate_branches(self):
        body = ("if (*(int **)(this + 8) == (int *)0x0) { Constructor(out); "
                "*out = &PTR__scalar_deleting_destructor__01238c8c; "
                "*(undefined4 *)(in_stack_00000004 + 4) = 0; "
                "*(undefined4 *)(in_stack_00000004 + 8) = 0; } call_slot();")
        pattern = semantic_patterns(body, [{"target": "0x0099A2D0"}],
            [{"vtableOffset": "0x30"}], "Thing::?GetScriptThing@Thing@@UBE?AVThing@@XZ")[0]
        self.assertEqual(pattern["kind"], "optional-resource-script-thing-return")
        self.assertEqual(pattern["emptyTokenBytes"], 12)

    def test_destroy_and_zero_preserves_cleanup_order(self):
        body = ("Destroy((allocator *)(this + 8), edx); "
                "*(undefined4 *)(this + 8) = 0;")
        direct = [{"target": "0x007E70E0", "site": "0x00CD2776"}]
        pattern = semantic_patterns(body, direct, [], "CMemoryDataOutputStream::Clear")[0]
        self.assertEqual(pattern, {
            "kind": "destroy-and-zero-field", "complete": True,
            "fieldOffset": "0x8", "fieldWidth": 4,
            "destroyTarget": "0x007E70E0", "destroyCallSite": "0x00CD2776"})

    def test_untrusted_constructor_becomes_opaque_token_initializer(self):
        body = "*(undefined ***)this = &PTR__vector_deleting_destructor__01231710;"
        pattern = semantic_patterns(
            body, [], [], "NHeroInformationScreens::CBase::CBase")[0]
        self.assertEqual(pattern, {
            "kind": "opaque-vtable-token-initializer", "complete": True,
            "fieldOffset": "0x0", "fieldWidth": 4,
            "retailVtableAddress": "0x01231710", "donorNameTrusted": False})

    def test_reference_counted_token_destructor_preserves_full_teardown(self):
        body = ("piVar1 = *(int **)(this + 0xc); *piVar1 = *piVar1 + -1; "
                "if (**(int **)(this + 0xc) == 0) { destroy(); free(); } "
                "*(undefined4 *)(this + 8) = 0; *(undefined4 *)(this + 0xc) = 0; "
                "*(undefined ***)this = &PTR__vector_deleting_destructor__0126008c;")
        direct = [{"target": "0x00BFE9BC", "site": "0x1"},
                  {"target": "0x0099A430", "site": "0x2"}]
        pattern = semantic_patterns(body, direct, [],
            "CScriptGameResourceObjectMovieBase::~CScriptGameResourceObjectMovieBase")[0]
        self.assertEqual(pattern["kind"], "reference-counted-token-destructor")
        self.assertEqual(pattern["restoredVtableAddress"], "0x0126008C")
        self.assertEqual(pattern["callSites"], ["0x1", "0x2"])

    def test_script_token_destructor_preserves_pre_release_identity(self):
        body = ("*(undefined ***)this = &PTR__scalar_deleting_destructor__01238c8c; "
                "piVar1 = *(int **)(this + 8); *piVar1 = *piVar1 + -1; "
                "if (**(int **)(this + 8) == 0) { destroy(); free(); } "
                "*(undefined4 *)(this + 4) = 0; *(undefined4 *)(this + 8) = 0;")
        direct = [{"target": "0x00BFE9BC", "site": "0x1"},
                  {"target": "0x0099A2E0", "site": "0x2"}]
        pattern = semantic_patterns(body, direct, [],
            "C3DClothPrimitive::~C3DClothPrimitive")[0]
        self.assertEqual(pattern["kind"], "reference-counted-script-token-destructor")
        self.assertEqual(pattern["preReleaseVtableAddress"], "0x01238C8C")

    def test_aggregate_destructor_preserves_all_container_layouts(self):
        body = ("this + 0x34 this + 0x30 iVar2 = iVar2 + 0x18 iVar2 + 0x10 "
                "this + 0x1c this + 0x18 this + 0xc this + 8 this + 4")
        targets = ["0x0099EAE0", "0x00BFEA14", "0x00CBB200", "0x00BFEA14",
                   "0x00CBB1B0", "0x00BFEA14", "0x00CBB090", "0x00BFEA14",
                   "0x0099A300"]
        direct = [{"target": target, "site": f"0x{index}"}
                  for index, target in enumerate(targets)]
        pattern = semantic_patterns(body, direct, [],
            "NParticleEngine::CParticleEmitter::DeleteAllParticles")[0]
        self.assertEqual(pattern["kind"], "aggregate-owned-container-destructor")
        self.assertEqual(pattern["stridedRange"]["stride"], 0x18)
        self.assertEqual(len(pattern["ranges"]), 2)

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
