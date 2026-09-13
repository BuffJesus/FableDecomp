import json
import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.lift_native_lua import (Lifter, RData, lift_entity, lift_persist,
                                                   known_callee_aliases, load_entity_state,
                                                   load_entity_constants, load_entity_parent_state, strip_declarations)


MANIFEST = {
    "SetQuestCardObjective": {"scope": "Quest", "returnType": "void", "parameters": [
        {"name": "questName", "type": "const std::string&"}, {"name": "objectiveText", "type": "const std::string&"},
        {"name": "region1", "type": "const std::string&"}, {"name": "region2", "type": "const std::string&"}]},
    "GetActiveQuestName": {"scope": "Quest", "returnType": "std::string", "parameters": []},
    "NewScriptFrame": {"scope": "Quest", "returnType": "bool", "parameters": []},
    "RegisterTimer": {"scope": "Quest", "returnType": "int", "parameters": []},
    "SetTimer": {"scope": "Quest", "returnType": "void", "parameters": [
        {"name": "timerId", "type": "int"}, {"name": "timeValue", "type": "int"}]},
    "GetTimer": {"scope": "Quest", "returnType": "int", "parameters": [{"name": "timerId", "type": "int"}]},
    "RemoveThing": {"scope": "Quest", "returnType": "void", "parameters": [{"name": "pThing", "type": "CScriptThing*"}]},
    "IsAlive": {"scope": "Entity", "returnType": "bool", "parameters": [{"name": "pMe", "type": "CScriptThing*"}]},
    "MsgOnRegionLoaded": {"scope": "Quest", "returnType": "sol::object", "parameters": [{"name": "s", "type": "sol::this_state"}]},
    "PlayLoopingAnimation": {"scope": "Entity", "returnType": "void", "parameters": [
        {"name": "pMe", "type": "CScriptThing*"}, {"name": "animName", "type": "const std::string&"},
        {"name": "loopCount", "type": "int"}]},
    "Speak": {"scope": "Entity", "returnType": "bool", "parameters": [
        {"name": "pMe", "type": "CScriptThing*"},
        {"name": "spTargetThing", "type": "const std::shared_ptr<CScriptThing>&"},
        {"name": "dialogueKey", "type": "const std::string&"},
        {"name": "selectionMethod", "type": "int", "optional": True},
        {"name": "makeTargetListen", "type": "bool", "optional": True},
        {"name": "soundIn2D", "type": "bool", "optional": True},
        {"name": "overScreenFade", "type": "bool", "optional": True}]},
}

ROOT = Path(__file__).resolve().parents[2]


class NoRData(RData):
    def __init__(self):
        self.ok = False


def make(entity: bool = False, state=None) -> Lifter:
    return Lifter(MANIFEST, state or {}, "quest" if entity else "Quest", entity, "Pkg", NoRData())


class StripDeclarationsTests(unittest.TestCase):
    def test_return_before_labeled_code_is_valid_lua_and_still_exits(self):
        from lupa.lua54 import LuaRuntime

        lifter = make()
        body = lifter.lift('Main', '{\nGSI->SetTimer(42,1);\nreturn;\n'
                          'LAB_00123456:\nGSI->SetTimer(42,2);\n}')
        lua = LuaRuntime()
        events = []
        quest = lua.table_from({'SetTimer': lambda _q, timer, value: events.append(value)})
        lua.execute('return function(Quest)\n' + '\n'.join(body) + '\nend')(quest)
        self.assertEqual(events, [1])
        self.assertIn('label LAB_00123456', lifter.todo)

    def test_nested_terminal_blocks_and_else_chain_execute(self):
        from lupa import LuaRuntime

        native = ("{\nif (outer) {\nif (first) {\nGSI->SetTimer(42,1);\n}\n"
                  "else if (second) {\nGSI->SetTimer(42,2);\n}\nelse {\n"
                  "GSI->SetTimer(42,3);\n}\n}\n}")
        lifter = make()
        body = "\n".join(lifter.lift("Main", native))
        lua = LuaRuntime()
        events = []
        quest = lua.table_from({"SetTimer": lambda _q, timer, v: events.append(v)})
        run = lua.execute("return function(Quest, outer, first, second)\n" + body + "\nend")
        for outer, first, second, expected in (
                (False, True, True, []), (True, True, True, [1]),
                (True, False, True, [2]), (True, False, False, [3])):
            with self.subTest(outer=outer, first=first, second=second):
                events.clear()
                run(quest, outer, first, second)
                self.assertEqual(events, expected)
        self.assertEqual(lifter.todo, [])

    def test_drops_signature_declarations_and_closing_brace(self):
        body = "\r\nvoid __fastcall FUN_1(int param_1)\r\n\r\n{\r\n  int iVar1;\r\n  bool bVar2;\r\n  \r\n" \
               "  *(undefined1 *)(param_1 + 0x48) = 0;\r\n  return;\r\n}\r\n\r\n"
        self.assertEqual(strip_declarations(body), ["  *(undefined1 *)(param_1 + 0x48) = 0;"])

    def test_joins_wrapped_statements(self):
        body = "{\n  ppVar3 = (pair *)\n           GSI->GetActiveQuestName(&uStack_4);\n}\n"
        self.assertEqual(strip_declarations(body), ["  ppVar3 = (pair *) GSI->GetActiveQuestName(&uStack_4);"])

    def test_compact_while_and_inline_break_are_structured(self):
        lifter = make()
        out = lifter.lift("Thread:Watch", "{\nwhile( true ) {\nif (cVar1 != '\\0') break;\n}\n}")
        self.assertIn("    while true do", out)
        self.assertIn("        if cVar1 ~= 0 then break end", out)
        self.assertEqual(lifter.todo, [])

    def test_terminal_destructor_goto_is_a_proven_early_return(self):
        lifter = make()
        native = ("{\nif (bVar1) goto LAB_00123456;\n"
                  "QuestWork();\nLAB_00123456:\noperator_delete(local_10);\nreturn;\n}\n")
        out = lifter.lift("Main", native)
        self.assertIn("    if bVar1 then return end  -- native destructor epilogue LAB_00123456", out)
        self.assertFalse(any("verify cleanup" in item for item in lifter.todo))

    def test_effectful_terminal_goto_remains_a_todo(self):
        lifter = make()
        native = ("{\nif (bVar1) goto LAB_00123456;\n"
                  "LAB_00123456:\nGSI->NewScriptFrame();\nreturn;\n}\n")
        out = lifter.lift("Main", native)
        self.assertTrue(any("TODO(native): goto LAB_00123456" in line for line in out))
        self.assertTrue(any("verify cleanup" in item for item in lifter.todo))


class EntityInventoryStateTests(unittest.TestCase):
    def test_unnamed_scalar_loads_share_the_storage_used_by_stores(self):
        from lupa.lua54 import LuaRuntime

        lifter = make()
        body = "\n".join(lifter.lift("Main", "{\n*(int *)(this + 0x48) = 3;\n"
                         "while (*(int *)(this + 0x48) > 0) {\n"
                         "GSI->SetTimer(42,*(int *)(this + 0x48));\n"
                         "*(int *)(this + 0x48) = *(int *)(this + 0x48) + -1;\n}\n}"))
        lua = LuaRuntime()
        state = {}
        events = []

        def record(_q, timer, value):
            events.append(value)
            if len(events) > 3:
                self.fail("field-backed counter did not terminate")

        quest = lua.table_from({"GetStateInt": lambda _q, key: state[key],
                                "SetStateInt": lambda _q, key, v: state.__setitem__(key, v),
                                "SetTimer": record})
        lua.execute("return function(Quest)\n" + body + "\nend")(quest)
        self.assertEqual(events, [3, 2, 1])
        self.assertEqual(state, {"self_0x48": 0})
        self.assertTrue(lifter.todo, "unnamed fields must remain review items")

    def test_only_typed_unmodified_ecx_input_is_a_quest_receiver(self):
        lifter = make(state={"0x48": ("Count", "Int")})
        source = "void Worker(void)\n{\n  CScriptBase *in_ECX;\n  iVar1 = *(int *)(in_ECX + 0x48);\n}"
        body = "\n".join(lifter.lift("Thread:Worker", source))
        self.assertIn('Quest:GetStateInt("Count")', body)
        for unsupported in (source.replace("CScriptBase", "CThing"),
                            source.replace("  iVar1", "  in_ECX = another;\n  iVar1")):
            body = "\n".join(lifter.lift("Thread:Worker", unsupported))
            self.assertNotIn('Quest:GetStateInt("Count")', body)

    def test_lifted_barrel_instruction_and_termination_paths_execute(self):
        from lupa.lua54 import LuaRuntime

        report = lift_entity(ROOT / "refs/script_recovery/new_oakvale_intro/translation_unit.json",
                             "NOVI_Barrel", None, "0x00DB7E10")
        for already_given, xbox, cancel_prompt in (
                (True, False, False), (False, False, False), (False, True, False),
                (False, False, True)):
            with self.subTest(already_given=already_given, xbox=xbox, cancel_prompt=cancel_prompt):
                lua = LuaRuntime()
                lua.execute(report["lua"])
                state = {"InstructionGiven_Barrels": already_given}
                texts = []
                frames = []

                def frame(_q, _me):
                    frames.append(1)
                    if len(frames) > 5:
                        self.fail("lifted barrel did not terminate")
                    return not ((state["InstructionGiven_Barrels"] and len(frames) > 1)
                                or (cancel_prompt and len(frames) >= 3))

                quest = lua.table_from({
                    "NewScriptFrame": frame,
                    "GetStateBool": lambda _q, key: state[key],
                    "SetStateBool": lambda _q, key, value: state.__setitem__(key, value),
                    "GetHero": lambda _q: lua.table(),
                    "IsDistanceBetweenThingsUnder": lambda _q, actor, hero, distance: True,
                    "IsXbox": lambda _q: xbox,
                    "DisplayGameInfo": lambda _q, key: texts.append(key),
                    "MsgIsGameInfoClickedPast": lambda _q: not cancel_prompt})
                lua.globals().Main(quest, lua.table())
                expected = [] if already_given else ["TEXT_QST_048_INSTRUCTION_BREAK_BARRELS" +
                                                      ("" if xbox else "_PC")]
                self.assertEqual(texts, expected)
                self.assertIs(state["InstructionGiven_Barrels"], not cancel_prompt)

    def test_persisted_boolean_hex_comparison_uses_boolean_semantics(self):
        from lupa.lua54 import LuaRuntime

        lifter = make(state={"0x48": ("Activated", "Bool")})
        body = "\n".join(lifter.lift("Main", "{\nif (this[0x48] == (bool)0x0) {\n"
                         "GSI->SetTimer(42,1);\n}\nif (this[0x48] == (bool)0x1) {\n"
                         "GSI->SetTimer(42,2);\n}\n}"))
        lua = LuaRuntime()
        events = []
        run = lua.execute("return function(Quest)\n" + body + "\nend")
        for state, expected in ((False, [1]), (True, [2])):
            events.clear()
            quest = lua.table_from({"GetStateBool": lambda _q, key: state,
                                    "SetTimer": lambda _q, timer, value: events.append(value)})
            run(quest)
            self.assertEqual(events, expected)

    def test_mutable_scalar_loop_does_not_reuse_initial_constant(self):
        from lupa.lua54 import LuaRuntime

        lifter = make()
        body = "\n".join(lifter.lift("Main", "{\niVar1 = 3;\nwhile (iVar1 > 0) {\n"
                         "GSI->SetTimer(42,iVar1);\niVar1 = iVar1 + -1;\n}\n"
                         "GSI->SetTimer(42,iVar1);\n}"))
        events = []

        def record(_q, timer, value):
            events.append(value)
            if len(events) > 4:
                self.fail("mutable counter did not end the loop")

        lua = LuaRuntime()
        quest = lua.table_from({"SetTimer": record})
        lua.execute("return function(Quest)\n" + body + "\nend")(quest)
        self.assertEqual(events, [3, 2, 1, 0])

    def test_mutable_scalar_survives_branch_scope_and_numeric_bool_abi(self):
        from lupa.lua54 import LuaRuntime

        lifter = make(entity=True)
        lifter.manifest = {**lifter.manifest, "SetFlag": {
            "scope": "Quest", "returnType": "void", "parameters": [{"name": "value", "type": "bool"}]}}
        body = "\n".join(lifter.lift("Main", "{\nif (choose) {\niVar1 = 1;\n}\n"
                         "else {\niVar1 = 0;\n}\nGSI->SetFlag(iVar1);\n}"))
        lua = LuaRuntime()
        events = []
        quest = lua.table_from({"SetFlag": lambda _q, value: events.append(value)})
        run = lua.execute("return function(quest, choose)\n" + body + "\nend")
        run(quest, False)
        run(quest, True)
        self.assertEqual(events, [False, True])
        self.assertTrue(all(type(value) is bool for value in events))

    def test_reassigning_call_result_invalidates_old_operand_temporary(self):
        lifter = make()
        out = lifter.lift("Main", "{\niVar1 = 7;\niVar1 = GSI->GetTimer(42);\n"
                          "GSI->SetTimer(42,iVar1);\n}")
        self.assertIn("    Quest:SetTimer(42, iVar1)", out)
        self.assertNotIn("Quest:SetTimer(42, 7)", "\n".join(out))

    def test_reviewed_float_constant_remains_a_numeric_comparison(self):
        from lupa.lua54 import LuaRuntime

        lifter = make()
        lifter.native_constants = {"_DAT_0122dedc": "0.0"}
        body = lifter.lift("Main", "{\nfVar3 = (float10)_DAT_0122dedc;\n"
                           "if (fVar3 < health) {\nGSI->SetTimer(42,1);\n}\n}")
        lua = LuaRuntime()
        events = []
        quest = lua.table_from({"SetTimer": lambda _q, timer, value: events.append(value)})
        run = lua.execute("return function(Quest, health)\n" + "\n".join(body) + "\nend")
        run(quest, 0)
        self.assertEqual(events, [])
        run(quest, 1)
        self.assertEqual(events, [1])
        self.assertEqual(load_entity_constants("NOVI_Villager"), {"_DAT_0122dedc": "0.0"})
        self.assertEqual(lifter.expr('"_DAT_0122dedc (float10) != 0"'),
                         '"_DAT_0122dedc (float10) != 0"')

    def test_constant_join_rejects_guesses_and_ambiguous_symbols(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "Actor.json"
            path.write_text(json.dumps({"constants": [
                {"value": 1, "source": "DAT_01234567", "evidence": "inferred"},
                {"value": 2, "source": "DAT_01234568 + DAT_01234569", "evidence": "retail-rdata"},
                {"value": 0.0, "source": "_DAT_01234570", "evidence": "retail-rdata (byte read)"}]}),
                encoding="utf-8")
            self.assertEqual(load_entity_constants("Actor", Path(directory)), {"_DAT_01234570": "0.0"})

    def test_exact_target_termination_result_copy_exits_generated_loop(self):
        from lupa.lua54 import LuaRuntime

        lifter = make(entity=True)
        lifter.callee_names = {"Mislabelled::Call": "IsActiveThreadTerminating"}
        native = ("{\nGSI->NewScriptFrame();\nMislabelled::Call(this);\ncVar1 = extraout_AL_00;\n"
                  "while (cVar1 == '\\0') {\nGSI->NewScriptFrame();\n"
                  "Mislabelled::Call(this);\ncVar1 = extraout_AL_01;\n}\n}")
        body = "\n".join(lifter.lift("Main", native))
        lua = LuaRuntime()
        frames = []

        def frame(_q, _me):
            frames.append(1)
            if len(frames) > 2:
                self.fail("termination result did not end the loop")
            return len(frames) == 1

        quest = lua.table_from({"NewScriptFrame": frame})
        lua.execute("return function(quest, me)\n" + body + "\nend")(quest, lua.table())
        self.assertEqual(frames, [1, 1])
        self.assertNotIn("extraout_AL", body)
        lifter.callee_names = {}
        body = "\n".join(lifter.lift("Main", native))
        self.assertIn("extraout_AL", body)
        self.assertTrue(lifter.todo)

    def test_parent_state_requires_matching_owner_and_declared_scalar_field(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            inventories = root / "entities"
            inventories.mkdir()
            inventory = {"nativeClass": "NScript::CQ_DemoScript::CActor",
                         "parentFieldsRead": ["Ready", "Flags"], "parentFieldsWritten": []}
            path = inventories / "Actor.json"
            path.write_text(json.dumps(inventory), encoding="utf-8")
            (root / "persistence_manifest.json").write_text(json.dumps({
                "script": "Q_Demo", "fields": [
                    {"name": "Ready", "retailOffset": "0x51", "type": "bool"},
                    {"name": "Unrelated", "retailOffset": "0x52", "type": "bool"},
                    {"name": "Flags", "retailOffset": "0x60..0x64", "type": "bool[5]"}]}),
                encoding="utf-8")
            expected = {"0x51": ("Ready", "Bool")}
            expected.update({hex(0x60 + i): (f'Flags_{i}', 'Bool') for i in range(5)})
            self.assertEqual(load_entity_parent_state("Actor", inventories), expected)
            inventory["nativeClass"] = "NScript::CQ_OtherScript::CActor"
            path.write_text(json.dumps(inventory), encoding="utf-8")
            self.assertEqual(load_entity_parent_state("Actor", inventories), {})

    def test_parent_boolean_wait_reloads_shared_state_and_writes_boolean(self):
        from lupa.lua54 import LuaRuntime

        lifter = make(entity=True)
        lifter.parent_state = {"0x51": ("Ready", "Bool"), "0x82": ("InstructionGiven", "Bool")}
        native = ("{\ncVar1 = *(char *)(*(int *)(this + 0x14) + 0x51);\n"
                  "while (cVar1 == '\\0') {\nGSI->NewScriptFrame();\n"
                  "cVar1 = *(char *)(*(int *)(this + 0x14) + 0x51);\n}\n"
                  "if (*(char *)(*(int *)(this + 0x14) + 0x82) == '\\0') {\n"
                  "*(undefined1 *)(*(int *)(this + 0x14) + 0x82) = 1;\n}\n}")
        body = "\n".join(lifter.lift("Main", native))
        lua = LuaRuntime()
        calls = []
        state = {"Ready": False, "InstructionGiven": False}

        def frame(_q, _me):
            calls.append("frame")
            state["Ready"] = True
            return True

        quest = lua.table_from({
            "GetStateBool": lambda _q, key: state[key],
            "SetStateBool": lambda _q, key, value: state.__setitem__(key, value),
            "NewScriptFrame": frame})
        lua.execute("return function(quest, me)\n" + body + "\nend")(quest, lua.table())
        self.assertEqual(calls, ["frame"])
        self.assertIs(state["InstructionGiven"], True)
        self.assertEqual(lifter.todo, [])

    def test_parent_field_is_not_treated_as_entity_local_or_unknown_owner(self):
        lifter = make(entity=True, state={"0x51": ("LocalReady", "Bool")})
        out = lifter.lift("Main", "{\ncVar1 = *(char *)(*(int *)(this + 0x14) + 0x51);\n}")
        self.assertNotIn('GetStateBool("LocalReady")', "\n".join(out))
        self.assertTrue(lifter.todo)

    def test_mislabelled_termination_alias_requires_exact_target(self):
        calls = {"calls": [
            {"currentName": "CCreatureAction_TrollWhackGroundBase::Initialise", "target": "0x00F35B30"},
            {"currentName": "same_label_wrong_target", "target": "0x00123456"},
        ]}
        self.assertEqual(known_callee_aliases(calls), {
            "CCreatureAction_TrollWhackGroundBase::Initialise": "IsActiveThreadTerminating"})

    def test_local_field_types_come_from_reviewed_inventory_evidence(self):
        self.assertEqual(load_entity_state("NOVI_Villager"), {"0x1c": ("HeroDidHitMe", "Bool")})
        bully = load_entity_state("NOVI_Bully")
        self.assertEqual(bully["0x1c"], ("InitialHealth", "Int"))
        self.assertEqual(bully["0x24"], ("DoneIntro", "Bool"))

    def test_entity_lift_uses_proven_field_name(self):
        report = lift_entity(
            ROOT / "refs/script_recovery/new_oakvale_intro/translation_unit.json",
            "NOVI_Villager", "0x00DADF00", "0x00DADF80")
        self.assertEqual(report["state"]["0x1c"], ("HeroDidHitMe", "Bool"))
        self.assertIn('quest:SetStateBool("HeroDidHitMe", false)', report["lua"])
        self.assertNotIn("unnamed field 0x1c", report["functions"]["Init"]["todo"])

    def test_ccharstring_field_assignment_uses_proven_persisted_name(self):
        lifter = make(state={"0x78": ("RandomSpeech00", "String")})
        out = lifter.lift(
            "Init", '{\nCCharString::operator=((CCharString *)(param_1 + 0x78),"TEXT_QST_014_RANDOM_00");\n}')
        self.assertEqual(out, ['    Quest:SetStateString("RandomSpeech00", "TEXT_QST_014_RANDOM_00")'])
        self.assertEqual(lifter.todo, [])

    def test_opaque_vtable_token_initializer_has_no_lua_effect(self):
        lifter = make()
        out = lifter.lift(
            "Main", "{\nNHeroInformationScreens::CBase::CBase((CBase *)&stack0xffffffd4);\n}")
        self.assertEqual(out, [])
        self.assertEqual(lifter.todo, [])

    def test_locally_paired_bsim_termination_call_uses_alive(self):
        lifter = make(entity=True)
        native = ("{\nGSI->NewScriptFrame();\n"
                  "CCreatureAction_TrollWhackGroundBase::Initialise(param_1);\n"
                  "if (extraout_AL_03 != '\\0') {\nreturn;\n}\n}")
        out = lifter.lift("Main", native)
        self.assertIn("    if not alive then", out)
        self.assertFalse(any("TrollWhackGroundBase" in line for line in out))
        self.assertEqual(lifter.todo, [])

    def test_unpaired_bsim_label_is_not_suppressed(self):
        lifter = make(entity=True)
        out = lifter.lift(
            "Main", "{\nCCreatureAction_TrollWhackGroundBase::Initialise(param_1);\nreturn;\n}")
        self.assertTrue(any("TrollWhackGroundBase" in line for line in out))
        self.assertTrue(any("TrollWhackGroundBase" in item for item in lifter.todo))

    def test_scripted_resource_speak_retains_explicit_target(self):
        lifter = make(entity=True)
        native = ('{\npCVar18 = (CCharString *)0x0;\npCVar19 = (CCharString *)0x0;\n'
                  'pCVar20 = (CCharString *)0x1;\nbVar5 = false;\n'
                  'CScriptGameResourceObjectScriptedThingBase::'
                  '_Speak_CScriptGameResourceObjectScriptedThingBase__UAEXABVCScriptThing__'
                  'PBDW4ETextGroupSelectionMethod___N33_Z(resource,me,"TEXT_KEY",pCVar18,pCVar19,'
                  'pCVar20,bVar5,junk);\n}')
        out = lifter.lift("Main", native)
        self.assertEqual(out, ['    local r1 = me:Speak(me, "TEXT_KEY", 0x0, false, true, false)'])
        self.assertEqual(lifter.todo, [])

    def test_manifest_typed_boolean_literals_become_lua_booleans(self):
        lifter = make(entity=True)
        speak = lifter.lift(
            "Main", '{\nCScriptGameResourceObjectScriptedThingBase::'
                    '_Speak_CScriptGameResourceObjectScriptedThingBase__UAEXABVCScriptThing__'
                    'PBDW4ETextGroupSelectionMethod___N33_Z(resource,me,"K",1,0,1,0);\n}')
        self.assertEqual(speak, ['    local r1 = me:Speak(me, "K", 1, false, true, false)'])

    def test_template_pointer_casted_null_is_exact_local_literal(self):
        lifter = make()
        self.assertEqual(
            lifter.literal('(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0'),
            "0x0")
        out = lifter.lift(
            "Main", "{\nppVar16 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0;\n"
                    "GSI->PauseAllNonScriptedEntities(ppVar16);\n}")
        self.assertEqual(out, ["    Quest:PauseAllNonScriptedEntities(false)"])
        self.assertEqual(lifter.todo, [])

    def test_assignment_in_if_condition_preserves_call_order(self):
        lifter = make()
        native = ("{\nuVar1 = GSI->RegisterTimer();\n"
                  "if ((bVar6) && (iVar10 = GSI->GetTimer(), iVar10 < 1)) {\n"
                  "GSI->NewScriptFrame();\n}\nGSI->SetTimer(uVar1,2);\n}")
        out = lifter.lift("Main", native)
        self.assertEqual(out, [
            "    local alive = true",
            "    local uVar1 = Quest:RegisterTimer()",
            "    local __native_condition_1 = bVar6",
            "    local iVar10",
            "    if __native_condition_1 then",
            "        iVar10 = Quest:GetTimer(uVar1)",
            "        __native_condition_1 = iVar10 < 1",
            "    end",
            "    if __native_condition_1 then",
            "        alive = Quest:NewScriptFrame()",
            "    end",
            "    Quest:SetTimer(uVar1, 2)",
        ])
        self.assertEqual(lifter.todo, [])

    def test_conditional_assignment_executes_only_required_calls(self):
        from lupa import LuaRuntime

        for op in ("&&", "||"):
            for left in (False, True):
                for value in (0, 2):
                    for staged in (False, True):
                        with self.subTest(op=op, left=left, value=value, staged=staged):
                            lifter = make()
                            initial = "iVar10 = 7;" if staged else "iVar10 = GSI->GetTimer(uVar1);"
                            native = ("{\nuVar1 = GSI->RegisterTimer();\n" + initial + "\n"
                                      f"if ((bVar6) {op} (iVar10 = GSI->GetTimer(uVar1), iVar10 < 1)) {{\n"
                                      "GSI->SetTimer(uVar1,10);\n}\nelse {\n"
                                      "GSI->SetTimer(uVar1,20);\n}\n"
                                      "GSI->SetTimer(uVar1,iVar10);\n}")
                            body = "\n".join(lifter.lift("Main", native))
                            lua = LuaRuntime(unpack_returned_tuples=True)
                            events = []
                            reads = []

                            def get_timer(_quest, timer):
                                reads.append(timer)
                                return 7 if not staged and len(reads) == 1 else value

                            quest = lua.table_from({
                                "RegisterTimer": lambda _quest: 42,
                                "GetTimer": get_timer,
                                "SetTimer": lambda _quest, timer, v: events.append((timer, v)),
                            })
                            run = lua.execute("return function(Quest, bVar6)\n" + body + "\nend")
                            run(quest, left)
                            called = left if op == "&&" else not left
                            branch = (left and value < 1) if op == "&&" else (left or value < 1)
                            self.assertEqual(reads, [42] * (int(not staged) + int(called)))
                            self.assertEqual(events, [(42, 10 if branch else 20),
                                                      (42, value if called else 7)])
                            self.assertEqual(lifter.todo, [])

    def test_conditional_assignment_evaluates_left_before_overwriting_target(self):
        from lupa import LuaRuntime

        lifter = make_thing()
        native = ("{\ncVar1 = GSI->IsAlive();\n"
                  "if ((cVar1 != '\\0') && (cVar1 = GSI->IsAlive(), cVar1 == '\\0')) {\n"
                  "GSI->SetTimer(42,10);\n}\nGSI->SetTimer(42,20);\n}")
        body = "\n".join(lifter.lift("Main", native))
        lua = LuaRuntime()
        events = []
        values = iter((True, False))
        me = lua.table_from({"IsAlive": lambda _me: next(values)})
        quest = lua.table_from({"SetTimer": lambda _q, timer, v: events.append(v)})
        lua.execute("return function(quest, me)\n" + body + "\nend")(quest, me)
        self.assertEqual(events, [10, 20])

    def test_condition_temporary_does_not_shadow_native_identifier(self):
        lifter = make()
        body = lifter.lift("Main", "{\nif ((__native_condition_1) && "
                           "(iVar10 = GSI->GetTimer(42), iVar10 < 1)) {\n}\n}")
        self.assertIn("    local __native_condition_2 = __native_condition_1", body)

    def test_while_assignment_rechecks_left_and_preserves_final_result(self):
        from lupa.lua54 import LuaRuntime

        for op in ("&&", "||"):
            for initial in (0, 2):
                with self.subTest(op=op, initial=initial):
                    # The left operand reads a value assigned in the body. OR skips the first two
                    # timer reads; AND stops without another read once the body changes iVar2 to 0.
                    left = "iVar2 > 0"
                    native = (f"{{\niVar2 = {initial};\niVar10 = 7;\n"
                              f"while (({left}) {op} (iVar10 = GSI->GetTimer(42), iVar10 < 1)) {{\n"
                              "GSI->SetTimer(42,iVar10);\niVar2 = iVar2 - 1;\n}\n"
                              "GSI->SetTimer(42,iVar10);\n}")
                    lifter = make()
                    body = "\n".join(lifter.lift("Main", native))
                    lua = LuaRuntime()
                    reads, values = [], []

                    def timer(_quest, number):
                        reads.append(number)
                        self.assertLess(len(reads), 5, "condition did not advance")
                        return 0 if len(reads) <= 2 else 2

                    quest = lua.table_from({"GetTimer": timer,
                                            "SetTimer": lambda _q, _timer, v: values.append(v)})
                    lua.execute("return function(Quest)\n" + body + "\nend")(quest)
                    if op == "&&":
                        self.assertEqual(reads, [42] * initial)
                        self.assertEqual(values, [0, 0, 0] if initial else [7])
                    else:
                        self.assertEqual(reads, [42, 42, 42])
                        self.assertEqual(values, [7] * initial + [0, 0, 2])
                    self.assertEqual(lifter.todo, [])

    def test_while_assignment_break_keeps_assigned_value_in_outer_scope(self):
        from lupa.lua54 import LuaRuntime

        native = ("{\nwhile ((true) && (iVar10 = GSI->GetTimer(42), iVar10 < 1)) {\n"
                  "break;\n}\nGSI->SetTimer(42,iVar10);\n}")
        for value in (0, 2):
            lifter = make()
            body = "\n".join(lifter.lift("Main", native))
            lua = LuaRuntime()
            events = []
            quest = lua.table_from({"GetTimer": lambda _q, _timer: value,
                                    "SetTimer": lambda _q, _timer, v: events.append(v)})
            lua.execute("return function(Quest)\n" + body + "\nend")(quest)
            self.assertEqual(events, [value])
            self.assertIsNone(lua.globals().iVar10)

    def test_thing_condition_preserves_flag_when_null_skips_call(self):
        from lupa.lua54 import LuaRuntime

        for initial in (False, True):
            for actor_alive in (None, False, True):
                lifter = make_thing()
                native = (f"{{\ncVar1 = {str(initial).lower()};\n"
                          "if ((!thing) || (cVar1 = CScriptThing::IsAlive (thing), cVar1 == '\\0')) {\n"
                          "GSI->SetTimer(42,10);\n}\nGSI->SetTimer(42,cVar1);\n}")
                body = '\n'.join(lifter.lift('Main', native))
                lua = LuaRuntime()
                events, calls = [], []

                def predicate(_thing):
                    calls.append('IsAlive')
                    return actor_alive

                actor = None if actor_alive is None else lua.table_from({'IsAlive': predicate})
                quest = lua.table_from({'SetTimer': lambda _q, _timer, value: events.append(value)})
                lua.execute('return function(quest, thing)\n' + body + '\nend')(quest, actor)
                self.assertEqual(calls, [] if actor_alive is None else ['IsAlive'])
                self.assertEqual(events[:-1], [] if actor_alive else [10])
                self.assertIs(events[-1], initial if actor_alive is None else actor_alive)
                self.assertEqual(lifter.todo, [])

    def test_thing_assignment_while_repeats_predicate(self):
        from lupa.lua54 import LuaRuntime

        lifter = make_thing()
        body = '\n'.join(lifter.lift('Main', "{\nwhile ((thing) && "
                                     "(cVar1 = CScriptThing::IsAlive(thing), cVar1 != '\\0')) {\n"
                                     "GSI->SetTimer(42,10);\n}\nGSI->SetTimer(42,cVar1);\n}"))
        lua = LuaRuntime()
        values, events = iter([True, True, False]), []
        actor = lua.table_from({'IsAlive': lambda _thing: next(values)})
        quest = lua.table_from({'SetTimer': lambda _q, _timer, value: events.append(value)})
        lua.execute('return function(quest, thing)\n' + body + '\nend')(quest, actor)
        self.assertEqual(events, [10, 10, False])

    def test_interface_call_accepts_whitespace_before_operands(self):
        from lupa.lua54 import LuaRuntime

        lifter = make()
        body = '\n'.join(lifter.lift('Main', '{\niVar1 = GSI->GetTimer \n(42);\n'
                                     'GSI->SetTimer \t(42,iVar1);\n}'))
        lua = LuaRuntime()
        events = []
        quest = lua.table_from({'GetTimer': lambda _q, _timer: 17,
                                'SetTimer': lambda _q, _timer, value: events.append(value)})
        lua.execute('return function(Quest)\n' + body + '\nend')(quest)
        self.assertEqual(events, [17])
        self.assertEqual(lifter.todo, [])

    def test_repeated_termination_assignment_condition_uses_alive(self):
        lifter = make()
        native = ("{\nbVar2 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);\n"
                  "if ((!bVar2) && (bVar2 = CScriptBase::IsActiveThreadTerminating("
                  "(CScriptBase *)this), !bVar2)) {\nGSI->NewScriptFrame();\n}\n"
                  "GSI->NewScriptFrame();\n}")
        out = lifter.lift("Main", native)
        self.assertEqual(out, [
            "    local alive = true",
            "    local bVar2 = not alive",
            "    if alive then",
            "        alive = Quest:NewScriptFrame()",
            "    end",
            "    alive = Quest:NewScriptFrame()",
        ])
        self.assertEqual(lifter.todo, [])


class QuestLiftTests(unittest.TestCase):
    def test_entity_binding_boilerplate_collapses(self):
        native = ('{\n  pCVar2 = operator_new(0x1c);\n  bVar4 = pCVar2 == (CEntityScriptBindingBase *)0x0;\n'
                  '  if (bVar4) {\n    pCVar2 = (CEntityScriptBindingBase *)0x0;\n  }\n  else {\n'
                  '    CCharString::CCharString(aCStack_10,"EmptyGrave",-1);\n'
                  '    *(undefined ***)pCVar2 = &PTR_LAB_012ee4ac;\n  }\n'
                  '  CScriptBase::AddEntityScriptBinding(param_1,pCVar2);\n  if (!bVar4) {\n'
                  '    std::\n    _Cons_val<x>((allocator *)aCStack_10, extraout_EDX,unaff_EDI);\n  }\n'
                  '  GSI->PostAddScriptedEntities();\n  return;\n}\n')
        lifter = make()
        out = lifter.lift("Main", native)
        self.assertEqual(out, ['    Quest:AddEntityBinding("EmptyGrave", "Pkg/Entities/EmptyGrave")',
                               "    Quest:FinalizeEntityBindings()"])
        self.assertEqual(lifter.entities, ["EmptyGrave"])
        self.assertEqual(lifter.todo, [])

    def test_thiscall_entity_binding_variant_collapses(self):
        native = ('{\npCVar1 = ::operator_new(0x1c);\nif (bVar2) {\npCVar1 = 0x0;\n}\nelse {\n'
                  'CCharString::CCharString((CCharString *)&stack0xfffffff8,"Dragon",-1);\n'
                  '*(undefined ***)pCVar1 = &PTR_LAB_012cb760;\n'
                  '*(CQ_DragonBossFightScript **)(pCVar1 + 8) = this;\n}\n'
                  'CScriptBase::AddEntityScriptBinding((CScriptBase *)this,pCVar1);\nif (!bVar2) {\n'
                  'std::_Cons_val<x>((allocator *)&stack0xfffffff8,extraout_EDX,unaff_EDI);\n}\n}')
        lifter = make()
        self.assertEqual(lifter.lift("Main", native),
                         ['    Quest:AddEntityBinding("Dragon", "Pkg/Entities/Dragon")'])
        self.assertEqual(lifter.entities, ["Dragon"])
        self.assertEqual(lifter.todo, [])

    def test_string_operands_recovered_from_temporaries_in_push_order(self):
        native = ('{\n  CCharString::CCharString(aCStack_8,"",-1);\n'
                  '  CCharString::CCharString(aCStack_c,"HeroGuildComplexInside",-1);\n'
                  '  CCharString::CCharString(aCStack_10,"TEXT_QUEST_MAZE_RESEARCH_OBJECTIVE_01",-1);\n'
                  '  ppVar3 = (pair *) GSI->GetActiveQuestName(&uStack_4,aCStack_10);\n'
                  '  GSI->SetQuestCardObjective();\n  return;\n}\n')
        lifter = make()
        out = lifter.lift("Main", native)
        self.assertEqual(out, ["    local ppVar3 = Quest:GetActiveQuestName()",
                               '    Quest:SetQuestCardObjective(ppVar3, "TEXT_QUEST_MAZE_RESEARCH_OBJECTIVE_01", '
                               '"HeroGuildComplexInside", "")'])
        self.assertEqual(lifter.todo, [])

    def test_default_and_copy_constructed_strings_remain_operand_temporaries(self):
        native = ('{\n  CCharString::CCharString(aCStack_8);\n'
                  '  CCharString::CCharString(aCStack_c,"OBJECT_APPLE",-1);\n'
                  '  CCharString::CCharString(aCStack_10,aCStack_c);\n'
                  '  CCharString::operator=(aCStack_14,aCStack_8);\n'
                  '  GSI->SetQuestCardObjective(aCStack_10,aCStack_14);\n}\n')
        lifter = make()
        out = lifter.lift("Main", native)
        self.assertEqual(out, ['    Quest:SetQuestCardObjective("OBJECT_APPLE", "", '
                               'nil --[[missing]], nil --[[missing]])'])
        self.assertEqual(lifter.todo, ["2 args for 4 params"])

    def test_char_pointer_literal_feeds_string_constructor(self):
        native = ('{\n  pcVar15 = "TEXT_EXACT";\n'
                  '  CCharString::CCharString(aCStack_8,pcVar15,-1);\n'
                  '  GSI->GetThingWithScriptName(aCStack_8);\n}\n')
        lifter = make()
        out = lifter.lift("Main", native)
        self.assertEqual(out, ['    local r1 = Quest:GetThingWithScriptName("TEXT_EXACT")'])
        self.assertEqual(lifter.todo, [])

    def test_native_allocation_cleanup_has_no_lua_statement(self):
        lifter = make()
        out = lifter.lift("Main", "{\n  operator_delete(piVar3);\n  return;\n}\n")
        self.assertEqual(out, [])
        self.assertEqual(lifter.todo, [])

    def test_recovered_script_helper_call_stays_in_generated_call_graph(self):
        lifter = make()
        lifter.helper_names = {"SpawnMinions"}
        lifter.callee_names = {"NScript::CQ_DragonBossFightScript::SpawnMinions": "SpawnMinions"}
        native = ('{\n  NScript::CQ_DragonBossFightScript::SpawnMinions('
                  '(CQ_DragonBossFightScript *)this);\n}\n')
        out = lifter.lift("Thread:RunEnemySpawning", native)
        self.assertEqual(out, ["    SpawnMinions(Quest)"])
        self.assertEqual(lifter.todo, [])

    def test_flattened_direct_cscriptthing_method_uses_manifest_binding(self):
        lifter = make(entity=True)
        lifter.thing_sigs = {"IsAlive": "?IsAlive@CScriptThing@@UBE_NXZ"}
        native = ('{\n  bVar1 = CScriptThing::_IsAlive_CScriptThing__UBE_NXZ('
                  '(CScriptThing *)me);\n}\n')
        out = lifter.lift("Main", native)
        self.assertEqual(out, ["    local bVar1 = me:IsAlive()"])
        self.assertEqual(lifter.todo, [])

    def test_sol_this_state_is_not_a_lua_operand(self):
        lifter = make()
        out = lifter.lift("Main", "{\n  GSI->MsgOnRegionLoaded();\n}\n")
        self.assertEqual(out, ["    local r1 = Quest:MsgOnRegionLoaded()"])
        self.assertEqual(lifter.todo, [])

    def test_state_writes_use_persist_names(self):
        persist = ('CPersistContext::Transfer<signed_char>(param_2,"SwordTaken",(char *)(param_1 + 0x48),(char *)0);\n'
                   'CPersistContext::Transfer<int>(this,"Hits",(int *)(param_1 + 0x4c),(int *)&local);\n')
        lines, state, calls = lift_persist(persist, "Quest")
        self.assertEqual(state, {"0x48": ("SwordTaken", "Bool"), "0x4c": ("Hits", "Int")})
        self.assertIn('    swordTaken = Quest:PersistTransferBool(context, "SwordTaken", swordTaken)', lines)
        lifter = make(state=state)
        out = lifter.lift("Init", "{\n  *(undefined1 *)(param_1 + 0x48) = 0;\n  *(undefined4 *)(param_1 + 0x4c) = 3;\n}\n")
        self.assertEqual(out, ['    Quest:SetStateBool("SwordTaken", false)', '    Quest:SetStateInt("Hits", 3)'])

    def test_unnamed_field_is_marked_not_guessed(self):
        lifter = make()
        out = lifter.lift("Init", "{\n  *(undefined1 *)(param_1 + 0x50) = 1;\n}\n")
        self.assertEqual(out, ["    -- TODO(native): name field 0x50 (undefined1)",
                               '    Quest:SetStateBool("self_0x50", true)'])
        self.assertEqual(lifter.todo, ["unnamed field 0x50 written in Init"])

    def test_thread_spawn_becomes_create_thread_with_body_address(self):
        native = ('{\n  this_00 = ::operator_new(0x3c);\n  bVar1 = this_00 == (CSpawnedFunc<X> *)0x0;\n'
                  '  if (bVar1) {\n    this_00 = (CSpawnedFunc<X> *)0x0;\n  }\n  else {\n'
                  '    CCharString::CCharString((CCharString *)&stack0xfffffff8,"WatchForBooty",-1);\n'
                  '    CSpawnedFunc<NScript::CExpression_FollowScript>::CSpawnedFunc<NScript::CExpression_FollowScript>\n'
                  '              (this_00,(CCharString *)&stack0xfffffff8,0,unaff_EDI,unaff_ESI);\n'
                  '    *(undefined ***)this_00 = &PTR__vector_deleting_destructor__012d502c;\n'
                  '    *(undefined1 **)(this_00 + 0x34) = &LAB_00cdd440;\n'
                  '    *(CGlobal_WatchForHeroDeathScript **)(this_00 + 0x38) = this;\n  }\n  return;\n}\n')
        lifter = make()
        out = lifter.lift("Main", native)
        self.assertEqual(len(out), 1)
        self.assertTrue(out[0].startswith('    Quest:CreateThread("WatchForBooty")'))
        self.assertEqual(lifter.threads, [{"name": "WatchForBooty", "body": "0x00CDD440"}])

    def test_thread_ownership_add_is_collapsed_only_for_captured_object(self):
        native = ('{\npCVar4 = operator_new(0x3c);\n'
                  'CCharString::CCharString(local_20,"Watch",-1);\n'
                  'CSpawnedFunc<X>::CSpawnedFunc<X>(pCVar4,local_20,0,this,this);\n'
                  '*(code **)(pCVar4 + 0x34) = WatchBody;\n}\n'
                  'CCharString::CCharString(&local_1c,"",-1);\n'
                  'CGuiVarTransferStruct::Add((CGuiVarTransferStruct *)this,(CGuiVarTransferBase *)pCVar4);\n'
                  'CGuiVarTransferStruct::Add((CGuiVarTransferStruct *)this,(CGuiVarTransferBase *)other);\n}')
        lifter = make()
        out = lifter.lift("Main", native)
        self.assertTrue(any('Quest:CreateThread("Watch")' in line for line in out))
        self.assertFalse(any("pCVar4" in line for line in out))
        self.assertTrue(any("CGuiVarTransferStruct::Add" in line for line in out))
        self.assertTrue(any("CGuiVarTransferStruct::Add" in item for item in lifter.todo))

    def test_fun_named_thread_body_becomes_an_address(self):
        native = ('{\npCVar8 = operator_new(0x3c);\n'
                  'CCharString::CCharString(&local_10,"Worker",-1);\n'
                  'CSpawnedFunc<X>::CSpawnedFunc<X>(pCVar8,&local_10,0,this,this);\n'
                  '*(code **)(pCVar8 + 0x34) = FUN_00d26ba0;\n}\n')
        lifter = make()
        out = lifter.lift("Main", native)
        self.assertTrue(any('native thread body 0x00D26BA0' in line for line in out))
        self.assertEqual(lifter.threads, [{"name": "Worker", "body": "0x00D26BA0"}])


class EntityLiftTests(unittest.TestCase):
    NATIVE = ('{\n  GSI->NewScriptFrame();\n'
              '  CCreatureAction_TrollWhackGroundBase::Initialise((CCreatureAction_TrollWhackGroundBase *)this);\n'
              "  if (extraout_AL == '\\0') {\n    uVar1 = GSI->RegisterTimer();\n    GSI->SetTimer(uVar1,5);\n"
              '    iVar2 = GSI->GetTimer(uVar1);\n    while (iVar2 != 0) {\n      GSI->NewScriptFrame();\n'
              '      CCreatureAction_TrollWhackGroundBase::Initialise((CCreatureAction_TrollWhackGroundBase *)this)\n'
              "      ;\n      if (extraout_AL_00 != '\\0') {\n        GSI->DeregisterTimer(uVar1);\n        return;\n      }\n"
              '      iVar2 = GSI->GetTimer();\n    }\n    GSI->RemoveThing((CScriptThing *)(this + 8),1,1);\n  }\n  return;\n}\n')

    def test_frame_terminate_timer_and_me_idioms(self):
        lifter = make(entity=True)
        lifter.callee_names = {"CCreatureAction_TrollWhackGroundBase::Initialise": "IsActiveThreadTerminating"}
        out = lifter.lift("Main", self.NATIVE)
        self.assertEqual(out, [
            "    local iVar2, uVar1",
            "    local alive = true",
            "    alive = quest:NewScriptFrame(me)",
            "    if alive then",
            "        uVar1 = quest:RegisterTimer()",
            "        quest:SetTimer(uVar1, 5)",
            "        iVar2 = quest:GetTimer(uVar1)",
            "        while iVar2 ~= 0 do",
            "            alive = quest:NewScriptFrame(me)",
            "            if not alive then",
            "                -- TODO(native): DeregisterTimer is not a ForgeFSE binding",
            "                quest:DeregisterTimer(uVar1)",
            "                return",
            "            end",
            "            iVar2 = quest:GetTimer(uVar1)",
            "        end",
            "        quest:RemoveThing(me)",
            "    end",
        ])

    def test_acquire_retry_loop_collapses(self):
        native = ('{\n  cVar4 = StartScriptingEntity(this,local_1c,4);\n  while (cVar4 == \'\\0\') {\n'
                  '    GSI->NewScriptFrame();\n    cVar4 = StartScriptingEntity(this,local_1c,4);\n  }\n  return;\n}\n')
        lifter = make(entity=True)
        out = lifter.lift("Main", native)
        self.assertEqual([l for l in out if l.strip()], ["    local alive = true",
                                                          "    if not me:AcquireControl(4) then return end"])

    def test_controlled_entity_resource_method_maps_to_me(self):
        native = ('{\n  CCharString::CCharString((CCharString *)&stack0xffffffd0,"CS_DEAD_DAD",-1);\n'
                  '  CScriptGameResourceObjectScriptedThingBase::'
                  '_PlayLoopingAnimation_CScriptGameResourceObjectScriptedThingBase__UAEXABVCCharString__J_N111111_Z'
                  '((CScriptGameResourceObjectScriptedThingBase *)local_1c, (CScriptThing *)&stack0xffffffd0,(char *)0xffffffff);\n  return;\n}\n')
        lifter = make(entity=True)
        out = lifter.lift("Main", native)
        self.assertEqual(out, ['    me:PlayLoopingAnimation("CS_DEAD_DAD", -1)'])


if __name__ == "__main__":
    unittest.main()


class ThreadBodyJobTests(unittest.TestCase):
    """export_native_threads.py resolves the recorded thread bodies from the retail bytes."""

    def setUp(self):
        from tools.script_recovery import export_native_threads as ent
        if not ent.RETAIL_EXE.is_file() or not ent.FUNCTIONS_TSV.is_file():
            self.skipTest("retail Fable.exe / functions.tsv not available")
        self.ent = ent

    def test_heros_old_house_watch_for_booty_resolves_to_manifest_address(self):
        job = self.ent.build_job([("Q_HerosOldHouse", "HerosOldHouse")])[0]
        self.assertEqual(job["problems"], [])
        self.assertEqual([(t["thread"], t["address"]) for t in job["threads"]],
                         [("WatchForBooty", "0x00D8A240")])   # functions.tsv: 00d8a240 WatchForBooty
        self.assertIn("0x00CDD440", job["sharedBodyStores"])  # RegisterMain's shared Main-thread body

    def test_lab_addresses_and_symbols_cross_check_in_beardy_baldy(self):
        job = self.ent.build_job([("V_BeardyBaldy", "BeardyBaldy")])[0]
        self.assertEqual(job["problems"], [])
        by_name = {t["thread"]: t["address"] for t in job["threads"]}
        self.assertEqual(by_name["WatchForNewHairdo"], "0x00E50150")   # recorded as LAB_00e50150
        self.assertEqual(by_name["WatchForQuestCardConditions"], "0x00E50910")

    def test_exported_thread_bodies_have_decompiles(self):
        from pathlib import Path
        out = self.ent.OUT_DIR / "Q_HerosOldHouse.json"
        if not out.is_file():
            self.skipTest("native_threads export not present (run export_native_threads.py)")
        data = json.loads(out.read_text(encoding="utf-8"))
        fn = {f["address"]: f for f in data["functions"]}["0x00D8A240"]
        self.assertIsNone(fn["error"])
        self.assertIn("thread:WatchForBooty", fn["selectedBy"])
        self.assertIn("{", fn["decompile"])

    def test_nested_dragon_workers_resolve_from_literal_and_retail_symbol(self):
        job = self.ent.build_job([("Q_DragonBossFight", "DragonBossFight")])[0]
        self.assertEqual(job["problems"], [])
        by_name = {t["thread"]: t["address"] for t in job["threads"] if t["thread"]}
        self.assertEqual(by_name["RunEnemySpawning"], "0x00D26BA0")
        self.assertEqual(by_name["JackTaunts"], "0x00D26A50")

    def test_cluster_lift_emits_exported_thread_function(self):
        from tools.script_recovery.lift_native_lua import lift_cluster
        out = self.ent.OUT_DIR / "Q_HerosOldHouse.json"
        if not out.is_file():
            self.skipTest("native_threads export not present (run export_native_threads.py)")
        report = lift_cluster("Q_HerosOldHouse")
        self.assertIn("Thread:WatchForBooty", report["functions"])
        self.assertIn("function WatchForBooty(questObject)", report["lua"])
        self.assertNotIn("native thread body unavailable", report["lua"])

    def test_cluster_lift_emits_exported_helpers_and_anchored_entity_body(self):
        from tools.script_recovery.lift_native_lua import lift_cluster
        dragon = lift_cluster("Q_DragonBossFight")
        self.assertEqual({h["name"] for h in dragon["helpers"]},
                         {"DoMission_Setup", "SpawnMinions", "SpawnSummoners"})
        self.assertIn("function SpawnMinions(questObject)", dragon["lua"])
        self.assertIn("SpawnMinions(Quest)", dragon["lua"])

        house = lift_cluster("Q_HerosOldHouse")
        self.assertIn("GhostFisherman", house["entityBodies"])
        self.assertEqual(house["entityBodies"]["GhostFisherman"]["address"], "0x00D8ABF0")
        self.assertIn("function Main(quest, me)", house["entityBodies"]["GhostFisherman"]["lua"])


THING_SIGS = {
    "MsgIsHitBy": "?MsgIsHitBy@CScriptThing@@UBE_NABVCCharString@@@Z",
    "MsgIsTalkedToBy": "?MsgIsTalkedToBy@CScriptThing@@UBE_NABVCCharString@@@Z",
    "MsgIsHitByAnySpecialAbilityFrom": "?MsgIsHitByAnySpecialAbilityFrom@CScriptThing@@UBE_NABVCCharString@@@Z",
    "MsgIsHitBySpecialAbilityFrom": "?MsgIsHitBySpecialAbilityFrom@CScriptThing@@UBE_NW4EHeroAbility@@ABVCCharString@@@Z",
    "MsgIsPresentedWithItem": "?MsgIsPresentedWithItem@CScriptThing@@UBE_NAAVCCharString@@@Z",
    "GetAngleXY": "?GetAngleXY@CScriptThing@@UBEMXZ",
    "GetHomePos": "?GetHomePos@CScriptThing@@UBE?AVC3DVector@@XZ",
    "MsgWhoHitMe": "?MsgWhoHitMe@CScriptThing@@UBE?AV1@XZ",
    "IsAlive": "?IsAlive@CScriptThing@@UBE_NXZ",
}
THING_SLOTS = {0x28: ("GetAngleXY", THING_SIGS["GetAngleXY"]), 0x1c: ("GetHomePos", THING_SIGS["GetHomePos"]),
               0x54: ("MsgIsHitBy", THING_SIGS["MsgIsHitBy"]), 0x68: ("MsgWhoHitMe", THING_SIGS["MsgWhoHitMe"]),
               0x6c: ("MsgIsTalkedToBy", THING_SIGS["MsgIsTalkedToBy"]),
               0x8c: ("MsgIsPresentedWithItem", THING_SIGS["MsgIsPresentedWithItem"]),
               0xa4: ("MsgIsHitBySpecialAbilityFrom", THING_SIGS["MsgIsHitBySpecialAbilityFrom"]),
               0xa8: ("MsgIsHitByAnySpecialAbilityFrom", THING_SIGS["MsgIsHitByAnySpecialAbilityFrom"]),
               0x12c: ("IsAlive", THING_SIGS["IsAlive"])}
GSI_SLOTS = {0x118: "GetHero", 0x120: "GetThingWithScriptName", 0x28: "IsActiveThreadTerminating",
             0x5ec: "PauseAllNonScriptedEntities", 0x768: "EntitySetFacingAngle", 0x580: "MiniMapRemoveMarker"}
THING_RETURNING = {0x118, 0x120}
MANIFEST.update({
    "MsgIsHitByHero": {"scope": "Entity", "returnType": "bool", "parameters": [{"name": "pMe", "type": "CScriptThing*"}]},
    "IsTalkedToByHero": {"scope": "Entity", "returnType": "bool", "parameters": [{"name": "pMe", "type": "CScriptThing*"}]},
    "MsgIsHitByAnySpecialAbilityFromHero": {"scope": "Entity", "returnType": "bool",
                                            "parameters": [{"name": "pMe", "type": "CScriptThing*"}]},
    "MsgIsHitByHeroSpecialAbility": {"scope": "Entity", "returnType": "bool", "parameters": [
        {"name": "pMe", "type": "CScriptThing*"}, {"name": "abilityEnum", "type": "int"}]},
    "MsgIsPresentedWithItem": {"scope": "Entity", "returnType": "bool", "parameters": [
        {"name": "pMe", "type": "CScriptThing*"}, {"name": "s", "type": "sol::this_state"}]},
    "GetAngleXY": {"scope": "Entity", "returnType": "float", "parameters": [{"name": "pMe", "type": "CScriptThing*"}]},
    "GetHomePos": {"scope": "Entity", "returnType": "sol::table", "parameters": [
        {"name": "pMe", "type": "CScriptThing*"}, {"name": "s", "type": "sol::this_state"}]},
    "IsAlive": {"scope": "Entity", "returnType": "bool", "parameters": [{"name": "pMe", "type": "CScriptThing*"}]},
    "GetHero": {"scope": "Quest", "returnType": "std::shared_ptr<CScriptThing>", "parameters": []},
    "GetThingWithScriptName": {"scope": "Quest", "returnType": "std::shared_ptr<CScriptThing>", "parameters": [
        {"name": "scriptName", "type": "const std::string&"}]},
    "EntitySetFacingAngle": {"scope": "Quest", "returnType": "void", "parameters": [
        {"name": "spThing", "type": "const std::shared_ptr<CScriptThing>&"}, {"name": "angle", "type": "float"},
        {"name": "bUnknown", "type": "sol::optional<bool>"}]},
    "MiniMapRemoveMarker": {"scope": "Quest", "returnType": "void", "parameters": [
        {"name": "spThing", "type": "const std::shared_ptr<CScriptThing>&"}]},
    "PauseAllNonScriptedEntities": {"scope": "Quest", "returnType": "void",
                                    "parameters": [{"name": "isPaused", "type": "bool"}]},
})


def make_thing(entity: bool = True) -> Lifter:
    lifter = make(entity=entity)
    lifter.thing_sigs = THING_SIGS
    return lifter


class ThingSlotAnnotationTests(unittest.TestCase):
    """annotate_interface_slots: CScriptThing vtable slots with receiver aliases."""

    def annotate(self, text: str, entity: bool = True) -> str:
        from tools.script_recovery.annotate_interface_slots import annotate
        return annotate(text, GSI_SLOTS, THING_SLOTS, THING_RETURNING, entity=entity)

    def test_me_alias_and_direct_this_plus_8(self):
        out = self.annotate("  pCVar1 = (CScriptThing *)(this + 8);\n"
                            "  cVar6 = (**(code **)(*(int *)pCVar1 + 0x54))();\n"
                            "  cVar7 = (**(code **)(*(int *)(this + 8) + 0x6c))(aCStack_50);\n")
        self.assertIn("cVar6 = CScriptThing::MsgIsHitBy((CScriptThing *)(this + 8));", out)
        self.assertIn("cVar7 = CScriptThing::MsgIsTalkedToBy((CScriptThing *)(this + 8), aCStack_50);", out)

    def test_thing_returned_by_interface_is_a_thing_receiver_until_redefined(self):
        native = ("  piVar6 = (int *)(**(code **)(**(int **)(this + 4) + 0x120))(&stack0xffffffc0,&stack0xffffffbc)\n"
                  "  ;\n  fVar7 = (float10)(**(code **)(*piVar6 + 0x28))(1);\n"
                  "  piVar6 = *(int **)(this + 4);\n  (**(code **)(*piVar6 + 0x5ec))(0);\n")
        out = self.annotate(native)
        self.assertIn("fVar7 = (float10)CScriptThing::GetAngleXY(piVar6, 1);", out)   # not GSI IsActiveThreadTerminating
        self.assertIn("GSI->PauseAllNonScriptedEntities(0);", out)                     # redefinition ends the alias

    def test_plain_pivar_stays_interface_and_unknown_receiver_untouched(self):
        native = ("  piVar2 = *(int **)(this + 4);\n  (**(code **)(*piVar2 + 0x5ec))(1);\n"
                  "  cVar1 = (**(code **)(*(int *)pCStack_108 + 0x6c))();\n")
        out = self.annotate(native)
        self.assertIn("GSI->PauseAllNonScriptedEntities(1);", out)
        self.assertIn("cVar1 = (**(code **)(*(int *)pCStack_108 + 0x6c))();", out)

    def test_quest_mode_has_no_me_receiver(self):
        native = "  cVar1 = (**(code **)(*(int *)(this + 8) + 0x54))();\n"
        self.assertEqual(self.annotate(native, entity=False), native)

    def test_decimal_interface_and_thing_offsets_are_accepted(self):
        native = ("  pCVar1 = (CScriptThing *)(this + 8);\n"
                  "  cVar1 = (**(code **)(*(int *)pCVar1 + 300))();\n"
                  "  piVar2 = *(int **)(this + 4);\n"
                  "  (**(code **)(*piVar2 + 1516))(1);\n")
        out = self.annotate(native)
        self.assertIn("cVar1 = CScriptThing::IsAlive((CScriptThing *)(this + 8));", out)
        self.assertIn("GSI->PauseAllNonScriptedEntities(1);", out)

    def test_thing_slot_returning_a_thing_chains_the_alias(self):
        native = ("  pCVar1 = (CScriptThing *)(this + 8);\n"
                  "  piVar3 = (int *)(**(code **)(*(int *)pCVar1 + 0x68))(&stack0xffffffd0);\n"
                  "  cVar2 = (**(code **)(*piVar3 + 0x12c))();\n")
        out = self.annotate(native)
        self.assertIn("piVar3 = (int *)CScriptThing::MsgWhoHitMe((CScriptThing *)(this + 8), &stack0xffffffd0);", out)
        self.assertIn("cVar2 = CScriptThing::IsAlive(piVar3);", out)

    def test_retail_table_loads_when_present(self):
        from tools.script_recovery.annotate_interface_slots import THING_SLOTS as path, load_thing_slots
        if not path.is_file():
            self.skipTest("cscriptthing_vtable_slots.tsv not present")
        table = load_thing_slots()
        self.assertEqual(len(table), 79)
        self.assertEqual(table[0x54][0], "MsgIsHitBy")
        self.assertEqual(table[0x28], ("GetAngleXY", "?GetAngleXY@CScriptThing@@UBEMXZ"))
        self.assertEqual(table[0x138][0], "IsEqualTo")


class ThingSlotLiftTests(unittest.TestCase):
    """lift_native_lua: CScriptThing slot calls lowered by their decorated signature."""

    def test_me_message_with_dropped_hero_operand(self):
        native = ('{\n  pCVar1 = (CScriptThing *)(this + 8);\n'
                  '  CCharString::CCharString(aCStack_54,"SCRIPT_NAME_HERO",-1);\n'
                  '  uVar15 = uVar4 | 1;\n  local_60 = uVar15;\n'
                  '  cVar6 = CScriptThing::MsgIsHitBy((CScriptThing *)(this + 8));\n'
                  "  if (cVar6 == '\\0') {\n    return;\n  }\n  return;\n}\n")
        lifter = make_thing()
        out = lifter.lift("Main", native)
        self.assertEqual(out, ["    local uVar15 = uVar4 | 1", "    local cVar6 = me:MsgIsHitByHero()",
                               "    if not cVar6 then", "        return", "    end"])
        self.assertEqual(lifter.todo, [])

    def test_enum_and_string_operands(self):
        native = ('{\n  CCharString::CCharString((CCharString *)&local_60,"SCRIPT_NAME_HERO",-1);\n'
                  '  cVar6 = CScriptThing::MsgIsHitBySpecialAbilityFrom((CScriptThing *)(this + 8), 0xe,&local_60);\n'
                  '  CCharString::CCharString(aCStack_5c,"SCRIPT_NAME_HERO",-1);\n'
                  '  cVar6 = CScriptThing::MsgIsHitByAnySpecialAbilityFrom((CScriptThing *)(this + 8), aCStack_5c);\n'
                  '  return;\n}\n')
        out = make_thing().lift("Main", native)
        self.assertEqual(out, ["    local cVar6 = me:MsgIsHitByHeroSpecialAbility(0xe)",
                               "    cVar6 = me:MsgIsHitByAnySpecialAbilityFromHero()"])

    def test_non_hero_name_keeps_raw_slot_with_todo(self):
        native = ('{\n  CCharString::CCharString(aCStack_54,"SCRIPT_NAME_BOB",-1);\n'
                  '  cVar6 = CScriptThing::MsgIsHitBy((CScriptThing *)(this + 8));\n  return;\n}\n')
        lifter = make_thing()
        out = lifter.lift("Main", native)
        self.assertEqual(out, ["    -- TODO(native): MsgIsHitBy is not a ForgeFSE binding",
                               '    local cVar6 = me:MsgIsHitBy("SCRIPT_NAME_BOB")'])
        self.assertEqual(lifter.todo, ["MsgIsHitBy: CScriptThing method not in FSE manifest"])

    def test_dead_father_angle_from_marker_thing(self):
        native = ('{\n  pCVar1 = (CScriptThing *)(this + 8);\n'
                  '  CCharString::CCharString((CCharString *)&stack0xffffffbc,"MK_OVID_DAD",-1);\n'
                  '  piVar6 = (int *)GSI->GetThingWithScriptName(&stack0xffffffc0,&stack0xffffffbc);\n'
                  '  iVar2 = **(int **)(this + 4);\n'
                  '  fVar7 = (float10)CScriptThing::GetAngleXY(piVar6, 1);\n'
                  '  GSI->EntitySetFacingAngle(pCVar1,(float)fVar7);\n  return;\n}\n')
        lifter = make_thing()
        out = lifter.lift("Main", native)
        self.assertEqual(out, ['    local piVar6 = quest:GetThingWithScriptName("MK_OVID_DAD")',
                               "    local fVar7 = piVar6:GetAngleXY()",
                               "    quest:EntitySetFacingAngle(me, fVar7)"])   # optional bUnknown absent, not guessed
        self.assertEqual(lifter.todo, [])

    def test_hidden_return_slot_and_out_parameter_are_dropped(self):
        native = ('{\n  pCVar12 = (C3DVector *)CScriptThing::GetHomePos((CScriptThing *)(this + 8), auStack_48);\n'
                  '  bVar2 = CScriptThing::MsgIsPresentedWithItem((CScriptThing *)(this + 8), &aCStack_30);\n'
                  '  return;\n}\n')
        out = make_thing().lift("Main", native)
        self.assertEqual(out, ["    local pCVar12 = me:GetHomePos()", "    local bVar2 = me:MsgIsPresentedWithItem()"])

    def test_refcount_release_and_interface_alias_lines_are_noise(self):
        native = ('{\n  if ((ppuStack_14 != (undefined **)0x0) &&\n'
                  '     (*ppuStack_14 = *ppuStack_14 + -1, *ppuStack_14 == (undefined *)0x0)) {\n'
                  '    (*(code *)ppuStack_14[1])();\n    operator_delete(ppuStack_14);\n  }\n'
                  '  iVar2 = **(int **)(this + 4);\n  piVar2 = *(int **)(this + 4);\n'
                  '  GSI->PauseAllNonScriptedEntities(1);\n  return;\n}\n')
        lifter = make_thing()
        self.assertEqual(lifter.lift("Main", native), ["    quest:PauseAllNonScriptedEntities(true)"])
        self.assertEqual(lifter.todo, [])

    def test_typed_pointer_refcount_release_is_native_cleanup(self):
        native = ('{\n  if ((piStack_110 != (int *)0x0) &&\n'
                  '     (*piStack_110 = *piStack_110 + -1, *piStack_110 == 0)) {\n'
                  '    (*(code *)piStack_110[1])();\n    operator_delete(piStack_110);\n  }\n'
                  '  GSI->PauseAllNonScriptedEntities(0);\n  return;\n}\n')
        lifter = make_thing()
        self.assertEqual(lifter.lift("Main", native),
                         ["    quest:PauseAllNonScriptedEntities(false)"])
        self.assertEqual(lifter.todo, [])

    def test_template_typed_refcount_release_is_native_cleanup(self):
        native = ('{\n  if ((ppVar3 != (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0) &&\n'
                  '     (*ppVar3 = *ppVar3 + -1, *ppVar3 == 0)) {\n'
                  '    (*(code *)ppVar3[1])();\n    operator_delete(ppVar3);\n  }\n'
                  '  GSI->PauseAllNonScriptedEntities(0);\n  return;\n}\n')
        lifter = make_thing()
        self.assertEqual(lifter.lift("Main", native),
                         ["    quest:PauseAllNonScriptedEntities(false)"])
        self.assertEqual(lifter.todo, [])

    def test_return_slot_alias_resolves_to_the_result(self):
        native = ('{\n  CCharString::CCharString(local_20,"BB_BeardyBaldyMan",-1);\n  ppVar7 = apStack_c;\n'
                  '  GSI->GetThingWithScriptName(ppVar7,local_20);\n'
                  '  bVar13 = CScriptThing::IsAlive((CScriptThing *)apStack_c);\n'
                  '  if (bVar13) {\n    GSI->MiniMapRemoveMarker(apStack_c);\n  }\n  return;\n}\n')
        lifter = make_thing(entity=False)
        out = lifter.lift("Main", native)
        self.assertEqual(out, ['    local r1 = Quest:GetThingWithScriptName("BB_BeardyBaldyMan")',
                               "    local bVar13 = (r1 ~= nil and r1:IsAlive())", "    if bVar13 then",
                               "        Quest:MiniMapRemoveMarker(r1)", "    end"])
        self.assertEqual(lifter.todo, [])

    def test_wrapped_scope_operator_joins(self):
        body = ("{\n  bVar5 = CScriptThing::\n"
                "          _IsPerformingScriptTask_CScriptGameResourceObjectScriptedThingBase__UBE_NXZ\n"
                "                    ((CScriptThing *)aCStack_34);\n}\n")
        self.assertEqual(strip_declarations(body),
                         ["  bVar5 = CScriptThing::_IsPerformingScriptTask_CScriptGameResourceObjectScriptedThingBase"
                          "__UBE_NXZ ((CScriptThing *)aCStack_34);"])
