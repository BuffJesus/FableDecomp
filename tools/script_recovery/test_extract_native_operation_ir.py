import unittest
from pathlib import Path

from tools.script_recovery.extract_native_operation_ir import extract
from tools.script_recovery.compare_seed_native_ir import compare


class NativeOperationIRTests(unittest.TestCase):
    def test_maze_research_state_and_persistence(self):
        result = extract(Path("refs/script_recovery/native_clusters/V_MazeResearch.json"))
        persisted = [row["key"] for function in result["lifecycle"] for row in function["persistenceTransfers"]]
        self.assertEqual(persisted, ["SwordTaken", "BookRead"])
        main = next(row for row in result["lifecycle"] if row["role"] == "Main")
        self.assertIn("HistoryBookcase", main["strings"])
        self.assertTrue(any(row["callee"].endswith("AddEntityScriptBinding") for row in main["calls"]))

    def test_all_seed_bindings_are_correlated(self):
        result = compare(Path("refs/script_recovery/seed_corpus/sources"),
                         Path("refs/script_recovery/native_operation_ir"))
        self.assertEqual(len(result["scripts"]), 6)
        for row in result["scripts"]:
            self.assertFalse(row["luaBindingsMissingNativeLifecycle"], row)


if __name__ == "__main__":
    unittest.main()
