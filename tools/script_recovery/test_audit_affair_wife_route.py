import json
import unittest
from pathlib import Path

from tools.script_recovery.audit_affair_wife_route import audit


class AffairWifeRouteAuditTests(unittest.TestCase):
    def test_native_route_and_forge_distance_abi_pass(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(
            root / "ghidra_out/script_recovery/affair_wife_route_disasm.txt",
            Path(r"D:\Code\ForgeFSE-retail-shadow"),
            root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_AffairWife.lua",
            root / "ghidra_out/egor_pdb_names.tsv",
        )
        self.assertTrue(result["ok"], result)

    def test_normal_route_uses_one_command_and_stalled_route_reissues_same_operands(self):
        root = Path(__file__).resolve().parents[2]
        traces = root / "refs/script_recovery/new_oakvale_intro/traces"
        normal = json.loads((traces / "entity_affair_wife_reaches_argument.json").read_text())
        stalled = json.loads((traces / "entity_affair_wife_stalled_route_retries.json").read_text())
        normal_moves = [event for event in normal["events"] if event.get("name") == "MoveToPosition"]
        stalled_moves = [event for event in stalled["events"] if event.get("name") == "MoveToPosition"]
        self.assertEqual(1, len(normal_moves))
        self.assertEqual(2, len(stalled_moves))
        self.assertEqual(normal_moves[0]["arguments"][1:], stalled_moves[0]["arguments"][1:])
        self.assertEqual(stalled_moves[0]["arguments"][1:], stalled_moves[1]["arguments"][1:])

        stalled_logs = [event["arguments"][0] for event in stalled["events"]
                        if event.get("name") == "Log" and event.get("arguments")]
        retry_index = next(index for index, line in enumerate(stalled_logs) if "ROUTE_RETRY count=1" in line)
        reached_index = next(index for index, line in enumerate(stalled_logs) if "ROUTE_REACHED" in line)
        self.assertLess(retry_index, reached_index)


if __name__ == "__main__":
    unittest.main()
