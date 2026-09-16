import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch
from tools.script_recovery.build_readable_new_oakvale import build
from tools.script_recovery import test_generate_theresa_resource_candidate as candidate_tests


class TheresaReadableIntegrationTests(unittest.TestCase):
    def test_readable_ledger_and_actual_composed_routes(self):
        with tempfile.TemporaryDirectory() as directory:
            report=build(Path(directory))
            relative='FSE/NewOakValeIntro/Entities/NOVI_Theresa.lua'
            output=(Path(directory)/relative).read_text()
            self.assertTrue(report['syntax']['ok'])
            self.assertFalse(report['theresaCandidate']['enabled'])
            self.assertIn('Quests = {}',(Path(directory)/'FSE/quests.lua').read_text())
            for marker in ('LAB_','goto ','[[missing]]','pCVar','fVar'):
                self.assertNotIn(marker,output)
            self.assertEqual(output.count('quest:RegisterBoundConsciousCondition()'),1)
            rows={r['function']:r for r in report['functions'] if r['owner']=='NOVI_Theresa'}
            self.assertEqual(rows['Main']['implementationFunction'],'runTheresaMainAfterCondition')
            self.assertEqual(rows['Init']['implementationFunction'],'initializeTheresa')
            self.assertIsNotNone(rows['Main']['entryCondition'])
            self.assertIsNone(rows['Init']['entryCondition'])
            self.assertTrue(report['sourceMap'][relative]['structure']['validationLimits'])
            # Run the same actual-helper routes on emitted readable output.
            with patch('tools.script_recovery.test_generate_theresa_resource_candidate.generate',
                       return_value=(output,report['theresaCandidate'])):
                checks=candidate_tests.TheresaCandidateTests()
                checks.test_actual_helpers_compose_meeting_gift_and_outro()
                checks.test_assembled_init_and_entry_condition_before_first_frame()
