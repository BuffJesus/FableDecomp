import unittest

from tools.script_recovery.build_reconstruction_manifests import (
    aggregate_operation_status,
    api_gap_is_blocking,
    binding_scope,
    build_lua_call_surface,
    fixture_covers,
    include_in_api_inventory,
    native_function_name,
    parent_field_names,
    normalize_forge_binding,
    split_forge_bindings,
    status_for,
)


class ReconstructionCoverageTests(unittest.TestCase):
    def test_annotated_parent_field_declarations_normalize_for_manifest_ownership(self):
        self.assertEqual(
            parent_field_names("BadDeedsPerformed/WhichBadDeedsPerformed[2] (via AddBadDeed)"),
            {"BadDeedsPerformed", "WhichBadDeedsPerformed"})

    def test_split_lua_helper_maps_to_its_native_owner(self):
        self.assertEqual(native_function_name({"function": "Main/judge_hero"}), "Main")
        self.assertEqual(native_function_name({"function": "Init"}), "Init")

    def test_entity_fixture_is_source_scoped(self):
        meta = {"function": "Main", "source": "NewOakValeIntro/Entities/NOVI_Barrel.lua"}
        passed = {"status": "ran", "trace": "match"}
        self.assertTrue(fixture_covers("entity", meta["source"], "Main", meta, passed))
        self.assertFalse(fixture_covers(
            "entity", "NewOakValeIntro/Entities/NOVI_Bully.lua", "Main", meta, passed))

    def test_failed_or_different_function_is_not_coverage(self):
        meta = {"function": "Init", "source": "NewOakValeIntro/Entities/NOVI_Barrel.lua"}
        self.assertFalse(fixture_covers("entity", meta["source"], "Main", meta,
                                       {"status": "ran", "trace": "match"}))
        self.assertFalse(fixture_covers("entity", meta["source"], "Init", meta,
                                       {"status": "expectation-failed", "trace": "match"}))

    def test_secondary_function_coverage_requires_expected_trace_evidence(self):
        source = "NewOakValeIntro/Entities/NOVI_Bully.lua"
        passed = {"status": "ran", "trace": "match"}
        meta = {"function": "Main", "source": source, "covers": ["Main", "GivenTeddy"]}
        self.assertFalse(fixture_covers("entity", source, "GivenTeddy", meta, passed))
        meta["coverEvidence"] = {"GivenTeddy": ["TakeObjectFromHero"]}
        self.assertFalse(fixture_covers("entity", source, "GivenTeddy", meta, passed))
        meta["expect"] = ["TakeObjectFromHero"]
        self.assertTrue(fixture_covers("entity", source, "GivenTeddy", meta, passed))

    def test_default_source_only_covers_quest(self):
        meta = {"function": "Main", "source": None}
        passed = {"status": "ran", "trace": "written"}
        self.assertTrue(fixture_covers("quest", "NewOakValeIntro/NewOakValeIntro.lua",
                                      "Main", meta, passed))
        self.assertFalse(fixture_covers("entity", "NewOakValeIntro/Entities/NOVI_Barrel.lua",
                                       "Main", meta, passed))

    def test_untraced_status_is_not_mistaken_for_traced(self):
        operation = {"function": "GivenTeddy", "forgeBinding": "GiveHeroGold",
                     "evidence": "native-decompile", "argsKnown": True}
        self.assertEqual(status_for(operation, set(), False), "implemented-but-untraced")

    def test_public_lifecycle_can_name_executed_private_helper(self):
        meta = {"function": "Main", "covers": ["Main", "GivenTeddy"],
                "coverEvidence": {"GivenTeddy": ["TakeObjectFromHero"]},
                "expect": ["TakeObjectFromHero"],
                "source": "NewOakValeIntro/Entities/NOVI_Bully.lua"}
        passed = {"status": "ran", "trace": "match"}
        self.assertTrue(fixture_covers("entity", meta["source"], "GivenTeddy", meta, passed))

    def test_traced_operation_with_uncertain_args_remains_uncertain(self):
        self.assertEqual(aggregate_operation_status([
            "implemented-and-traced", "implemented-and-traced (uncertain args)"
        ]), "implemented (uncertain args)")

    def test_untraced_operation_prevents_function_trace_claim(self):
        self.assertEqual(aggregate_operation_status([
            "implemented-and-traced", "implemented-but-untraced"
        ]), "implemented-but-untraced")

    def test_binding_splitter_preserves_sentinels_and_splits_composites(self):
        self.assertEqual(split_forge_bindings("n/a"), ["n/a"])
        self.assertEqual(split_forge_bindings(
            "GetPos|MoveToPosition + IsPerformingScriptTask / NewScriptFrame"),
            ["GetPos", "MoveToPosition", "IsPerformingScriptTask", "NewScriptFrame"])
        self.assertEqual(split_forge_bindings("Speak(hero, key, 0), EndMovieSequence"),
                         ["Speak(hero, key, 0)", "EndMovieSequence"])

    def test_binding_normalizer_rejects_inventory_annotations(self):
        self.assertEqual(normalize_forge_binding("RunCutsceneWithSetup actors table"),
                         "RunCutsceneWithSetup")
        self.assertEqual(normalize_forge_binding("quest helper AddBadDeed"), "n/a")
        self.assertEqual(normalize_forge_binding("host-managed control release"), "host-managed")
        self.assertEqual(normalize_forge_binding("LuaEntityHost vtable slot 5 dispatch"),
                         "host-managed")
        self.assertIsNone(normalize_forge_binding("see seq 24"))
        self.assertIsNone(normalize_forge_binding("27"))
        self.assertEqual(normalize_forge_binding("Speak(hero, key, 0)"), "Speak")

    def test_api_inventory_excludes_nonbinding_sentinels(self):
        for sentinel in (None, "n/a", "n/a (data)", "none", "unknown", "missing"):
            self.assertFalse(include_in_api_inventory(sentinel))
        self.assertTrue(include_in_api_inventory("host-managed"))
        self.assertTrue(include_in_api_inventory("Speak"))

    def test_binding_scope_preserves_dual_registration(self):
        quest = {"SetIsPushableByHero", "QuestOnly"}
        entity = {"SetIsPushableByHero", "EntityOnly"}
        self.assertEqual(binding_scope("SetIsPushableByHero", quest, entity), "Quest|Entity")
        self.assertEqual(binding_scope("QuestOnly", quest, entity), "Quest")
        self.assertEqual(binding_scope("EntityOnly", quest, entity), "Entity")
        self.assertIsNone(binding_scope("Missing", quest, entity))

    def test_lua_call_surface_preserves_scope_and_helper_only_calls(self):
        validation = {"apiCalls": {
            "Quest.GetHero": ["b.lua", "a.lua", "a.lua"],
            "Entity.Speak": "actor.lua",
        }}
        rows = build_lua_call_surface(
            validation, {("Quest", "GetHero"), ("Entity", "Speak")}, {"Speak": {"Entity"}})
        by_name = {row["scopedName"]: row for row in rows}
        self.assertEqual(by_name["Quest.GetHero"]["files"], ["a.lua", "b.lua"])
        self.assertTrue(by_name["Quest.GetHero"]["bindingExists"])
        self.assertFalse(by_name["Quest.GetHero"]["mappedByOperationInventory"])
        self.assertTrue(by_name["Entity.Speak"]["mappedByOperationInventory"])

    def test_lua_call_surface_requires_matching_inventory_scope(self):
        validation = {"apiCalls": {"Entity.Speak": ["actor.lua"]}}
        rows = build_lua_call_surface(
            validation, {("Entity", "Speak")}, {"Speak": {"Quest"}})
        self.assertFalse(rows[0]["mappedByOperationInventory"])

    def test_nonblocking_documented_gap_requires_explicit_evidence_flag(self):
        self.assertTrue(api_gap_is_blocking({"retail": "missing call"}))
        self.assertFalse(api_gap_is_blocking({"retail": "host lifecycle", "blocking": False}))


if __name__ == "__main__":
    unittest.main()
