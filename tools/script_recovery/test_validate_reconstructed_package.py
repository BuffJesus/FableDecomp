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


if __name__ == "__main__":
    unittest.main()
