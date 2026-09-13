import unittest
from tools.script_recovery.audit_generated_init import audit


class GeneratedInitAuditTests(unittest.TestCase):
    def test_full_generated_init_matches_native_effect_order(self):
        for timer_id in (57, 913):
            report = audit(timer_id)
            self.assertTrue(report['ok'], report)
            self.assertEqual(report['timerIdAfter'], timer_id)
            self.assertEqual(report['speechKeys'], 42)
            self.assertEqual(sum(e[0] == 'timer' for e in report['nativeEffects']), 1)
            self.assertEqual(sum(e[0] == 'state' for e in report['nativeEffects']), 30)
