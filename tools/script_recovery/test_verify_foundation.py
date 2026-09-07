import unittest
from pathlib import Path

from tools.script_recovery.verify_foundation import verify

# Gaps opened by ingesting Aeon's Lua ports. Each is tracked in
# docs/scripts/AEON_LUA_PORTS.md and must shrink to an empty set, never grow.
KNOWN_PENDING = {
    # V_StatueMaster and Q_DragonBossFight need a Ghidra anchor-discovery run.
    "anchored native decompilation proven",
    # 2026-09-07: Aeon's 12-package batch (LUAGameflow, Fisherman, WaspBoss, ...) uses 12
    # bindings added in FSE 6.9.26 that ForgeFSE-retail-shadow has not ported yet
    # (GetCurrentStateGroupType, MoveToPosition_NonBlocking, MsgIsHitBy*, IsXbox,
    # ActivateQuestWithoutLoadingResources, AddLogbookTutorialEntry, ...).
    "static API findings reconciled with live bindings",
    # 2026-09-07: the new packages' scripts do not yet correlate to native catalog names
    # (matchedSeeds stayed 8/20), so the shadow manifest covers only the first 8 packages.
    "reconstructed scripts are shadow-only",
}


class FoundationAuditTests(unittest.TestCase):
    def test_only_known_gaps_remain(self):
        result = verify(Path("refs/script_recovery"))
        failures = {row["name"] for row in result["checks"] if not row["passed"]}
        self.assertEqual(failures, KNOWN_PENDING, sorted(failures))

    def test_every_other_check_passes(self):
        result = verify(Path("refs/script_recovery"))
        self.assertEqual(result["summary"]["passed"],
                         result["summary"]["total"] - len(KNOWN_PENDING))


if __name__ == "__main__":
    unittest.main()
