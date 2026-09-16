import copy
import unittest
from collections import Counter
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_theresa_control import verify


class TheresaControlTests(unittest.TestCase):
    def test_complete_main_control_inventory_and_lifetimes(self):
        w = verify(RData())
        self.assertEqual(len(w['events']), 50)
        counts = Counter(e['name'] for e in w['events'])
        self.assertEqual(counts['construct'], 3)
        self.assertEqual(counts['acquire'], 4)
        self.assertEqual(counts['speak'], 8)
        self.assertEqual(counts['animation'], 2)

    def test_rejects_missing_cleanup_and_changed_resource(self):
        data = RData()
        w = verify(data)
        omitted = copy.deepcopy(w)
        omitted['events'].pop()
        with self.assertRaisesRegex(ValueError, 'coverage'): verify(data, omitted)
        changed = copy.deepcopy(w)
        changed['events'][0]['setup']['ecx'] = ['stack', 328]
        with self.assertRaisesRegex(ValueError, 'operands'): verify(data, changed)
