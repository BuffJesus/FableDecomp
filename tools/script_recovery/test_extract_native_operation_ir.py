import unittest
from pathlib import Path

from tools.script_recovery.extract_native_operation_ir import calls, entity_bindings, extract, indirect_calls
from tools.script_recovery.compare_seed_native_ir import compare


class NativeOperationIRTests(unittest.TestCase):
    def test_function_declaration_is_not_counted_as_self_call(self):
        rows = calls("\nvoid FUN_00cbd4e0(void)\n\n{\n  RealHelper();\n  return;\n}\n")
        self.assertEqual([row["callee"] for row in rows], ["RealHelper"])

    def test_ghidra_pcode_operators_are_not_native_calls(self):
        rows = calls("void Main(void) { x = SUB41(value, 0); y = CONCAT44(a, b); RealHelper(); }")
        self.assertEqual([row["callee"] for row in rows], ["RealHelper"])

    def test_destructor_name_retains_scope_and_tilde(self):
        rows = calls("void Main(void) { C3DClothPrimitive::~C3DClothPrimitive(ptr); }")
        self.assertEqual([row["callee"] for row in rows],
                         ["C3DClothPrimitive::~C3DClothPrimitive"])

    def test_entity_binding_record_shape(self):
        rows = entity_bindings(r'''
            p = operator_new(0x1c);
            CCharString::CCharString(name, "Messenger", -1);
            *(undefined ***)p = &PTR_binding_01234567;
            *(code **)(p + 0x10) = MessengerAllocator;
            p[0x14] = (CEntityScriptBindingBase)0x1;
            *(undefined4 *)(p + 0x18) = 1;
            CScriptBase::AddEntityScriptBinding(parent, p);
        ''')
        self.assertEqual(len(rows), 1)
        self.assertEqual(rows[0]["entityName"], "Messenger")
        self.assertEqual(rows[0]["allocationSize"], 0x1c)
        self.assertEqual(rows[0]["allocatorExpression"], "MessengerAllocator")
        self.assertTrue(rows[0]["enabled"])
        self.assertEqual(rows[0]["trailingValue"], 1)
        self.assertTrue(rows[0]["complete"])
        self.assertTrue(rows[0]["layoutComplete"])

    def test_template_constructed_entity_binding(self):
        rows = entity_bindings(r'''
            p = operator_new(0x1c);
            CCharString::CCharString(name, "Undead", -1);
            result = CEntityScriptBinding<NScript::CQuestScript>::
                CEntityScriptBinding<NScript::CQuestScript>
                (p, name, parent, UndeadAllocator, 0);
            CScriptBase::AddEntityScriptBinding(parent, result);
        ''')
        self.assertEqual(rows[0]["constructionMode"], "template-constructor")
        self.assertEqual(rows[0]["constructorScriptType"], "NScript::CQuestScript")
        self.assertEqual(rows[0]["allocatorExpression"], "UndeadAllocator")
        self.assertTrue(rows[0]["complete"])
        self.assertFalse(rows[0]["layoutComplete"])

    def test_indirect_call_retains_vtable_offset(self):
        rows = indirect_calls("(**(code **)(*DAT_0143e8f8 + 0x168))(thing);")
        self.assertEqual(rows[0]["vtableOffset"], "0x168")
        self.assertEqual(rows[0]["targetExpression"], "*DAT_0143e8f8 + 0x168")
        self.assertEqual(rows[0]["interfaceProvenance"], "direct-gamescriptinterface-singleton")

    def test_indirect_call_tracks_local_singleton_copy(self):
        rows = indirect_calls(
            "iVar1 = *DAT_0143e8f8;\n(**(code **)(iVar1 + 0x164))(timer);"
        )
        self.assertEqual(rows[0]["interfaceProvenance"],
                         "local-copy-of-gamescriptinterface-singleton")

    def test_script_interface_field_and_decimal_offset(self):
        rows = indirect_calls(
            "iVar1 = **(int **)(this + 0x40);\n(**(code **)(iVar1 + 2000))();"
        )
        self.assertEqual(rows[0]["vtableOffset"], "0x7d0")
        self.assertEqual(rows[0]["interfaceProvenance"],
                         "local-copy-of-script-interface-vtable")

    def test_maze_research_state_and_persistence(self):
        result = extract(Path("refs/script_recovery/native_clusters/V_MazeResearch.json"))
        persisted = [row["key"] for function in result["lifecycle"] for row in function["persistenceTransfers"]]
        self.assertEqual(persisted, ["SwordTaken", "BookRead"])
        main = next(row for row in result["lifecycle"] if row["role"] == "Main")
        self.assertIn("HistoryBookcase", main["strings"])
        self.assertTrue(any(row["callee"].endswith("AddEntityScriptBinding") for row in main["calls"]))

    def test_meet_sister_entity_bindings_are_recovered(self):
        result = extract(Path("refs/script_recovery/native_clusters/QS_MeetSister.json"))
        main = next(row for row in result["lifecycle"] if row["role"] == "Main")
        self.assertEqual([row["entityName"] for row in main["entityBindings"]],
                         ["MeetSisterMessenger", "MeetSisterSister"])
        self.assertTrue(all(row["complete"] for row in main["entityBindings"]))

    def test_all_seed_bindings_are_correlated(self):
        result = compare(Path("refs/script_recovery/seed_corpus/sources"),
                         Path("refs/script_recovery/native_operation_ir"))
        self.assertEqual(len(result["scripts"]), 6)
        for row in result["scripts"]:
            self.assertFalse(row["luaBindingsMissingNativeLifecycle"], row)
            self.assertEqual(row["luaBindingsFoundNative"], row["nativeEntityBindings"])


if __name__ == "__main__":
    unittest.main()
