import json
import unittest
from pathlib import Path

from tools.script_recovery.guild_state_access_report import build, OUT


class GuildStateAccessReportTests(unittest.TestCase):
    def test_report_contains_all_stateful_lifecycle_evidence(self):
        result = build()
        self.assertEqual(result['schema'], 'guild-native-state-access/0.1')
        self.assertGreaterEqual(result['lifecycleFunctions'], 9)
        pre_melee = [f for f in result['functions'] if f['script'] == 'Q_GuildTrainingPreMelee' and f['role'] == 'Main'][0]
        self.assertIn('0x4C', pre_melee['offsets'])
        self.assertTrue(any('0x4C' in f['offsets'] for f in result['functions']))
        self.assertEqual(result, json.loads(OUT.read_text(encoding='utf-8')))


if __name__ == '__main__':
    unittest.main()
