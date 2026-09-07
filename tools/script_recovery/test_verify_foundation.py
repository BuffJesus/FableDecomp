import unittest
from pathlib import Path

from tools.script_recovery.verify_foundation import verify

# Gaps opened by ingesting Aeon's StatueMaster and DragonBossFight ports on
# 2026-09-02.  Both are tracked in docs/AEON_FSE_UPSTREAM_2026-09-02.md and must
# shrink to an empty set, never grow.
KNOWN_PENDING = {
    # V_StatueMaster and Q_DragonBossFight need a Ghidra anchor-discovery run.
    # (The upstream-binding gap was closed by porting the 14 bindings into
    # ForgeFSE on 2026-09-02; the corpus is now 111/111 callable.)
    "anchored native decompilation proven",
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
