import copy
import unittest
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_villager_movie_scope import verify


class VillagerMovieScopeTests(unittest.TestCase):
    def test_lifetime_covers_both_normal_exits_and_cancellation_joins(self):
        data=RData();w=verify(data)
        self.assertEqual(len(w['events']),7);self.assertEqual(len(w['pauses']),6)
        for key in ('events','pauses'):
            for index in range(len(w[key])):
                changed=copy.deepcopy(w);changed[key].pop(index)
                with self.assertRaisesRegex(ValueError,'coverage changed'):verify(data,changed)
