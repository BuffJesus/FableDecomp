import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.audit_forgefse_colour_abi import audit


class ForgeFseColourAbiAuditTests(unittest.TestCase):
    def make_tree(self, header_fields: str, parser_fields: str):
        temp = tempfile.TemporaryDirectory()
        root = Path(temp.name)
        forge = root / "forge" / "FableScriptExtender"
        forge.mkdir(parents=True)
        layouts = root / "layouts.tsv"
        layouts.write_text(
            "CRGBColour_u_0_s_0\t4\t0\tuchar\tB\n"
            "CRGBColour_u_0_s_0\t4\t1\tuchar\tG\n"
            "CRGBColour_u_0_s_0\t4\t2\tuchar\tR\n"
            "CRGBColour_u_0_s_0\t4\t3\tuchar\tA\n",
            encoding="utf-8",
        )
        (forge / "GameInterface.h").write_text(
            f"struct CRGBColour {{ unsigned char {header_fields}; }};\n", encoding="utf-8"
        )
        (forge / "LuaQuestState.cpp").write_text(
            "static CRGBColour ParseColorTable(sol::table value) {\n"
            f"  return {{ {parser_fields} }};\n"
            "}\n",
            encoding="utf-8",
        )
        return temp, layouts, forge.parent

    def test_accepts_bgra_layout_and_conversion(self):
        temp, layouts, forge_root = self.make_tree("B, G, R, A", "b, g, r, a")
        with temp:
            self.assertTrue(audit(layouts, forge_root)["ok"])

    def test_rejects_rgba_declaration_even_with_bgra_conversion(self):
        temp, layouts, forge_root = self.make_tree("R, G, B, A", "b, g, r, a")
        with temp:
            result = audit(layouts, forge_root)
            self.assertFalse(result["ok"])
            self.assertFalse(result["checks"]["forgeLayoutIsBGRA"])


if __name__ == "__main__":
    unittest.main()
