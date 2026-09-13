import unittest

from tools.script_recovery.validate_reconstructed_package import (
    CALL_RE,
    UNSUPPORTED_RE,
    api_call_matches,
    call_matches_assertion,
    executable_matches,
    validate_call_assertions,
)


class ReconstructedPackageValidationTests(unittest.TestCase):
    def test_api_call_inventory_captures_arbitrary_entity_local(self):
        source = ("marker:GetAngleXY(); walkOff:GetPos(); quest:GetHero(); "
                  "value:match('x'); string.format('x')")
        calls = [(match.group(1), match.group(2)) for match in api_call_matches(source)]
        self.assertEqual(calls, [
            ("marker", "GetAngleXY"), ("walkOff", "GetPos"), ("quest", "GetHero")])

    def test_api_call_inventory_ignores_colon_call_text_inside_strings(self):
        source = "local note = 'ghost:MissingBinding(' ; me:IsAlive()"
        calls = [(match.group(1), match.group(2)) for match in api_call_matches(source)]
        self.assertEqual(calls, [("me", "IsAlive")])

    def test_unsupported_inventory_ignores_lua_line_comments(self):
        source = (
            '-- NOVI.unsupported(quest, "CommentOnly", {})\n'
            'NOVI.unsupported(quest, "Executable", {}) -- trailing note\n'
        )
        self.assertEqual([match.group(1) for match in executable_matches(UNSUPPORTED_RE, source)],
                         ["Executable"])

    def test_comment_marker_inside_lua_string_remains_executable(self):
        source = 'NOVI.unsupported(quest, "Before--After", {}) -- trailing note\n'
        self.assertEqual([match.group(1) for match in executable_matches(UNSUPPORTED_RE, source)],
                         ["Before--After"])

    def test_unsupported_inventory_ignores_lua_block_comments(self):
        source = (
            '--[[ NOVI.unsupported(quest, "BlockOnly", {})\n'
            'NOVI.unsupported(quest, "StillBlockOnly", {}) ]]\n'
            'NOVI.unsupported(quest, "Executable", {})\n'
        )
        self.assertEqual([match.group(1) for match in executable_matches(UNSUPPORTED_RE, source)],
                         ["Executable"])

    def test_argument_aware_call_assertion_matches_name_contains_and_position(self):
        event = {"name": "Speak", "arguments": [
            {"handle": "hero", "scope": "Entity"}, "TEXT_BROKEN", 0]}
        self.assertTrue(call_matches_assertion(event, {
            "name": "Speak", "containsArguments": ["TEXT_BROKEN"], "argumentAt": {"2": 0}}))
        self.assertFalse(call_matches_assertion(event, {
            "name": "Speak", "argumentAt": {"1": "TEXT_THANKS"}}))

    def test_semantic_call_validation_enforces_counts_forbids_and_order(self):
        events = [
            {"name": "DisplayGameInfo", "arguments": ["UNATTENDED"]},
            {"name": "SetStateInt", "arguments": ["BadDeedsPerformed", 1]},
            {"name": "IsTalkedToByHero", "arguments": []},
            {"name": "Speak", "arguments": [{"handle": "hero"}, "BROKEN", 0]},
        ]
        meta = {
            "expectCalls": [{"name": "Speak", "containsArguments": ["BROKEN"], "count": 1}],
            "forbidCalls": [{"name": "Speak", "containsArguments": ["THANKS"]}],
            "expectCallSequence": [
                {"name": "DisplayGameInfo", "containsArguments": ["UNATTENDED"]},
                {"name": "SetStateInt", "argumentAt": {"0": "BadDeedsPerformed"}},
                {"name": "IsTalkedToByHero"},
                {"name": "Speak", "containsArguments": ["BROKEN"]},
            ],
        }
        self.assertEqual([], validate_call_assertions(events, meta, "fixture.json"))
        meta["expectCalls"][0]["count"] = 2
        errors = validate_call_assertions(events, meta, "fixture.json")
        self.assertEqual(1, len(errors))
        self.assertIn("matched 1 event", errors[0])


if __name__ == "__main__":
    unittest.main()
