import unittest

from tools.script_recovery.generate_affair_wife_resource_candidate import generate
from tools.script_recovery.structure_affair_wife_lua import structure_wife
from tools.script_recovery.readable_affair_wife import readable_wife_source,assigned_before_reads
from tools.script_recovery import test_generate_affair_wife_resource_candidate as candidate_tests


class WifeStructureTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.original,_=generate()
        cls.structured,cls.report=structure_wife(cls.original)
        cls.readable,cls.readability=readable_wife_source(cls.structured)

    def test_branch_and_cancellation_traces(self):
        for scenario in ({},{'hit':True},{'talk':True},{'talk':True,'discovered':True},
                         {'talk':True,'discovered':True,'answer':0},{'fail_acquire':True},
                         {'going':True,'busy':True},{'going':True,'busy':True,'talk':True},
                         {'going':True,'busy':True,'hit':True},
                         {'going':True,'approach_checks':3},
                         {'going':True,'approach_checks':3,'running_line':True},
                         {'going':True,'busy':True,'text_limit':20}):
            for check in range(1,65):
                results=[]
                for source in (self.original,self.structured,self.readable):
                    runner=candidate_tests.WifeCandidateTests();runner.source=source
                    results.append(runner.run_case(**scenario,stop_check=check,stop_frame=8,trace_frames=True))
                with self.subTest(scenario=scenario,check=check):
                    self.assertIsNone(results[0][1]);self.assertIsNone(results[1][1])
                    self.assertEqual(results[0][0],results[1][0])
                    self.assertIsNone(results[2][1])
                    self.assertEqual(results[0][0],results[2][0])

    def test_changed_phase_anchor_rejects(self):
        with self.assertRaises(ValueError):
            structure_wife(self.original.replace('    ::LAB_00db3593::','    ::changed::'))

    def test_error_cleanup_traces(self):
        for scenario, location, message in (
                ({'hit': True}, 'health', 'injected health failure'),
                ({'hit': True}, 'speak', 'injected speech failure'),
                ({'going': True, 'busy': True}, 'reply', 'injected reply failure')):
            results=[]
            for source in (self.original,self.structured,self.readable):
                runner=candidate_tests.WifeCandidateTests();runner.source=source
                events,error=runner.run_case(**scenario,error_at=location,stop_frame=8,trace_frames=True)
                self.assertIsNotNone(error)
                self.assertIn(message,error)
                results.append(events)
            with self.subTest(location=location):
                self.assertEqual(results[0],results[1])
                self.assertEqual(results[0],results[2])

    def test_localization_rejects_values_carried_from_prior_helper_call(self):
        self.assertFalse(assigned_before_reads('''local function helper()
    if ready then
        bVar1 = query()
    end
    consume(bVar1)
end
''','bVar1'))
        self.assertTrue(assigned_before_reads('''local function helper()
    if ready then
        bVar1 = query()
    else
        return false
    end
    consume(bVar1)
end
''','bVar1'))

    def test_cross_phase_values_stay_in_outer_scope(self):
        localized={v for values in self.readability['localizedHelpers'].values() for v in values}
        self.assertNotIn('r1',localized)
        self.assertNotIn('native_arg_wife_line_counter',localized)
        self.assertNotIn('ppVar15',localized)
        self.assertIn('bVar4',localized)
        self.assertIn('pCVar13',localized)

    def test_external_reads_and_callback_captures_prevent_localization(self):
        for changed in (
                self.structured.replace('    runBody()\n','    consume(bVar4)\n    runBody()\n'),
                self.structured.replace('                            if not argumentKey:Exists() then',
                                        '                            consume(bVar4)\n                            if not argumentKey:Exists() then'),
                self.structured.replace('    local function waitUntilNearHusband()\n',
                                        '    local function waitUntilNearHusband()\n        consume(bVar4)\n')):
            _,report=readable_wife_source(changed)
            self.assertTrue(all('bVar4' not in values for values in report['localizedHelpers'].values()))
