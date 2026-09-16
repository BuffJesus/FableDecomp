"""Compile actual host entry methods and exercise opt-in/default policies."""
import unittest
from tools.script_recovery.maze_prepare_entry_proposal import prepare


class MazeEntryProposalTests(unittest.TestCase):
    def test_actual_host_policy_and_sword_composition(self):
        report=prepare()
        self.assertTrue(report['compiledTest'].startswith('PASS: actual host'))
        self.assertTrue(report['swordGate'].startswith('PASS: native Info-identity'))
        self.assertFalse(report['runtimeChanged'])
        self.assertTrue(all(item['exitCode']==0 for item in report['commands']))


if __name__=='__main__':
    unittest.main()
