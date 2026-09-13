import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.audit_forgefse_movement_abi import audit


class MovementAbiAuditTests(unittest.TestCase):
    def make_tree(self, root: Path, follow_call: str) -> tuple[Path, Path]:
        folder = root / "FableScriptExtender"
        folder.mkdir()
        (folder / "EntityScriptingAPI.h").write_text(
            "void(__thiscall* FollowThing)(X* This, const CScriptThing* pThing, float distance, bool b1);\n"
            "void(__thiscall* MoveToPosition)(X* This, const C3DVector* pPos, float radius, "
            "EScriptEntityMoveType moveType, bool b1, bool b2);\n", encoding="utf-8")
        (folder / "LuaEntityAPI.cpp").write_text(
            follow_call + "\n"
            "void LuaEntityAPI::MoveToPosition_NonBlocking() {}\n"
            "bool bAvoidObstacles = b1.value_or(true);\n"
            "bool bIgnorePathPref = b2.value_or(false);\n"
            "pVTable->MoveToPosition(pExpert, &pos, radius, eMoveType, bAvoidObstacles, bIgnorePathPref);\n",
            encoding="utf-8")
        (folder / "LuaManager.cpp").write_text(
            'cscriptThing_type["MoveToPosition"] = [entityAPI](CScriptThing* pMe, sol::table pos, float radius,\n'
            "sol::optional<int> moveType, sol::optional<bool> b1, sol::optional<bool> b2) {\n"
            "entityAPI->MoveToPosition_NonBlocking(pMe, pos, radius, moveType.value_or(ENTITY_MOVE_WALK), b1, b2);};\n",
            encoding="utf-8")
        pdb = root / "names.tsv"
        pdb.write_text(
            "?FollowThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@M_N@Z\n"
            "?MoveToPosition@CScriptGameResourceObjectScriptedThingBase@@UAEXABVC3DVector@@MW4EScriptEntityMoveType@@_N2@Z\n",
            encoding="utf-8")
        return root, pdb

    def test_accepts_exact_forwarding(self):
        with tempfile.TemporaryDirectory() as tmp:
            root, pdb = self.make_tree(Path(tmp),
                "pVTable->FollowThing(pExpert, pConstTargetThing, distance, avoidObstacles);")
            self.assertTrue(audit(root, pdb)["ok"])

    def test_rejects_inverted_follow_bool(self):
        with tempfile.TemporaryDirectory() as tmp:
            root, pdb = self.make_tree(Path(tmp),
                "pVTable->FollowThing(pExpert, pConstTargetThing, distance, !avoidObstacles);")
            result = audit(root, pdb)
            self.assertFalse(result["ok"])
            self.assertFalse(result["checks"]["followForwardsUnchanged"])

    def test_rejects_lua_binding_that_drops_move_bools(self):
        with tempfile.TemporaryDirectory() as tmp:
            root, pdb = self.make_tree(Path(tmp),
                "pVTable->FollowThing(pExpert, pConstTargetThing, distance, avoidObstacles);")
            manager = root / "FableScriptExtender" / "LuaManager.cpp"
            manager.write_text(
                'cscriptThing_type["MoveToPosition"] = [entityAPI](CScriptThing* pMe, sol::table pos, float radius) {\n'
                "entityAPI->MoveToPosition(pMe, pos, radius, ENTITY_MOVE_WALK);};\n",
                encoding="utf-8")
            result = audit(root, pdb)
            self.assertFalse(result["ok"])
            self.assertFalse(result["checks"]["luaMoveBindsOperandAwareNonBlockingPath"])


if __name__ == "__main__":
    unittest.main()
