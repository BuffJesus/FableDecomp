import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.audit_forgefse_hero_ability_abi import audit


class HeroAbilityAbiAuditTests(unittest.TestCase):
    def make_tree(self, root: Path, operand: str) -> Path:
        folder = root / "FableScriptExtender"
        folder.mkdir()
        (folder / "EntityScriptingAPI.h").write_text(
            "enum EHeroAbility { HERO_ABILITY_HEAL_LIFE_SPELL = 0xE };", encoding="utf-8")
        (folder / "LuaEntityAPI.cpp").write_text(
            "bool LuaEntityAPI::MsgIsHitByHealLifeFromHero(CScriptThing* pMe) {\n"
            f" return pVTable->MsgIsHitBySpecialAbilityFrom(pMe, {operand}, heroName.get());\n"
            "}\nbool LuaEntityAPI::Next() { return false; }\n", encoding="utf-8")
        return root

    def test_accepts_named_retail_ability(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = self.make_tree(Path(tmp), "HERO_ABILITY_HEAL_LIFE_SPELL")
            self.assertTrue(audit(root)["ok"])

    def test_rejects_stale_decimal_30(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = self.make_tree(Path(tmp), "static_cast<EHeroAbility>(30)")
            result = audit(root)
            self.assertFalse(result["ok"])
            self.assertFalse(result["checks"]["convenienceUsesNamedEnum"])
            self.assertFalse(result["checks"]["staleDecimal30Absent"])


if __name__ == "__main__":
    unittest.main()
