import unittest

from tools.script_recovery.generate_book_trader_resource_candidate import generate
from tools.script_recovery.readable_book_trader import HELPER, readable_book_source
from tools.script_recovery.structure_book_trader_lua import structure_book
from tools.script_recovery import test_generate_book_trader_resource_candidate as book_tests


class ReadableBookTraderTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.before, _ = generate()
        readable, cls.mapping = readable_book_source(cls.before)
        cls.after, cls.structure = structure_book(readable)

    def events(self, source, **scenario):
        runner = book_tests.BookTraderCandidateTests()
        runner.source = source
        return runner.run_case(**scenario)

    def test_readable_trace_matches_for_purchase_hits_movement_and_cancellation(self):
        scenarios = [{'hit': True}, {'talk': True, 'gold': 2}, {'talk': True, 'gold': 3},
                     {'talk': True, 'gold': 3, 'answer': 0}, {'talk': True, 'wait_answers': 10},
                     {'move': True}, {'fail_acquire': True}, {'animation_byte': 47}]
        for scenario in scenarios:
            for check in range(1, 31):
                options = dict(scenario, stop_check=check, stop_frame=3)
                with self.subTest(options=options):
                    self.assertEqual(self.events(self.after, **options), self.events(self.before, **options))
        for scenario in ({'hit': True,'error_at':'health'}, {'talk':True,'error_at':'speak'},
                         {'move':True,'stop_frame':3,'error_at':'face'}):
            before, before_error=self.events(self.before, **scenario)
            after, after_error=self.events(self.after, **scenario)
            self.assertEqual(after,before)
            self.assertIn('injected',before_error)
            self.assertIn('injected',after_error)

    def test_helper_is_unchanged_and_changed_captures_reject(self):
        self.assertIn(HELPER,self.after)
        self.assertNotIn('goto ',self.after)
        self.assertIn('local function finishTrade()',self.after)
        main=next(f for f in self.mapping if f['function']=='__resource_main')
        self.assertTrue(main['splitLocals'])
        self.assertTrue(main['inlinedLiteralLocals'])
        with self.assertRaisesRegex(ValueError,'capture review changed'):
            readable_book_source(self.before.replace('book_resource = nil','book_resource = uVar9'))


if __name__=='__main__':
    unittest.main()
