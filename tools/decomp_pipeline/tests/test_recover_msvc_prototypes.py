import os
import sys
import unittest

TOOLS = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
if TOOLS not in sys.path:
    sys.path.insert(0, TOOLS)

from recover_msvc_prototypes import (  # noqa: E402
    parse_signature,
    recovered_module,
)


class RecoverMsvcPrototypeTests(unittest.TestCase):
    def test_ctable_virtual_signature_recovers_owner_and_this(self):
        signature = (
            "protected: virtual void __thiscall "
            "NUISystem::CTable::ConstructSpritesToDraw(void)"
        )
        self.assertEqual(
            (
                "NUISystem::CTable::ConstructSpritesToDraw",
                "__thiscall",
                "void",
                ["NUISystem::CTable *"],
            ),
            parse_signature(signature),
        )

    def test_qualified_owner_wins_over_stale_global_module(self):
        self.assertEqual(
            "NUISystem::CTable",
            recovered_module(
                "NUISystem::CTable::ConstructHorizontalLine",
                "_global",
            ),
        )

    def test_unqualified_name_preserves_fallback(self):
        self.assertEqual(
            "_global",
            recovered_module("GFInitialise", "_global"),
        )


if __name__ == "__main__":
    unittest.main()
