import unittest

from tools.script_recovery.validate_reconstructed_package import (
    UNSUPPORTED_RE,
    executable_matches,
)


class ReconstructedPackageValidationTests(unittest.TestCase):
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


if __name__ == "__main__":
    unittest.main()
