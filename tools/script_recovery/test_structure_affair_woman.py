"""Compare every observable call across local recovery and jump elimination."""
import unittest

from tools.script_recovery.generate_affair_woman_resource_candidate import generate
from tools.script_recovery.readable_affair_woman import readable_woman_source
from tools.script_recovery.structure_affair_woman_lua import structure_woman
from tools.script_recovery.native_new_oakvale_conditions import recover
from tools.script_recovery import test_generate_affair_woman_resource_candidate as candidate_tests


class WomanStructureTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.original, _ = generate()
        cls.readable, _ = readable_woman_source(cls.original)
        cls.structured, cls.report = structure_woman(cls.readable)

    def compare(self, **options):
        results = []
        for source in (self.original, self.readable, self.structured):
            runner = candidate_tests.AffairWomanCandidateTests()
            runner.source = source
            events, error = runner.run_case(trace_frames=True, **options)
            results.append((events, error))
        for events, error in results[1:]:
            self.assertEqual(events, results[0][0])
            self.assertEqual(error is None, results[0][1] is None)
            if error:
                self.assertIn('injected', error)
        if not options.get('error_at'):
            self.assertIsNone(results[0][1])
        return results[0]

    def test_branch_and_cancellation_traces(self):
        scenarios = ({}, {'hit': True}, {'talk': True}, {'move': True},
                     {'runoff': True}, {'runoff': True, 'runoff_moves': 2},
                     {'runoff': True, 'camera': True}, {'fail_acquire': True},
                     {'hit': True, 'busy': True}, {'talk': True, 'busy': True},
                     {'kiss': True, 'hug': True, 'animation_byte': 47},
                     {'hit': True, 'health': 0}, {'talk': True, 'health': 0})
        for scenario in scenarios:
            for check in range(1, 40):
                with self.subTest(scenario=scenario, check=check):
                    self.compare(**scenario, stop_check=check, stop_frame=8)

    def test_runoff_actually_moves_before_removal(self):
        events, _ = self.compare(runoff=True, runoff_moves=2, stop_frame=20)
        self.assertEqual(events.count(('move', 1)), 2)
        self.assertLess(events.index(('move', 1)), events.index(('remove.self',)))

    def test_exception_cleanup_traces(self):
        for scenario in ({'hit': True, 'error_at': 'health'},
                         {'hit': True, 'error_at': 'speak'},
                         {'talk': True, 'error_at': 'face'}):
            with self.subTest(scenario=scenario):
                events, error = self.compare(**scenario)
                self.assertIn('injected', error)
                self.assertEqual(events[-1], ('destroy.resource', 1))

    def test_changed_cleanup_rejected(self):
        with self.assertRaises(ValueError):
            structure_woman(self.readable.replace('    ::LAB_00db296b::', '    ::OTHER::'))

    def test_reused_locals_get_distinct_call_roles(self):
        _, mappings = readable_woman_source(self.original)
        main = next(row for row in mappings if row['function']=='__resource_main')
        self.assertEqual(len(main['expandedMovieCancellation']), 2)
        self.assertTrue(main['splitLocals']['bVar5']['versions'])
        self.assertTrue(main['splitLocals']['cVar6']['versions'])
        self.assertFalse(any(row['basis']=='reused or unresolved native temporary'
                             for row in main['locals'].values()))

    def test_changed_inline_cleanup_rejected(self):
        with self.assertRaisesRegex(ValueError, 'inline movie cancellation'):
            readable_woman_source(self.original.replace(
                'if not alive then finish_movie(); goto LAB_00db2974 end',
                'if not alive then goto LAB_00db2974 end', 1))

    def test_entry_condition_survives_helper_extraction(self):
        from lupa.lua54 import LuaRuntime
        source, _ = recover('NOVI_AffairWoman', self.original)
        source, _ = readable_woman_source(source)
        source, _ = structure_woman(source)
        lua = LuaRuntime()
        lua.execute('''events={};quest={}
            function quest:WithRetailResources(body) body({}) end
            function quest:RegisterBoundConsciousCondition() events[#events+1]='condition' end
            function quest:NewScriptFrame() events[#events+1]='frame';return true end
            function quest:IsActiveThreadTerminating() events[#events+1]='cancel';return true end
        ''')
        lua.execute(source)
        lua.globals().Main(lua.globals().quest, lua.table())
        self.assertEqual(list(lua.globals().events.values()), ['condition', 'frame', 'cancel'])
