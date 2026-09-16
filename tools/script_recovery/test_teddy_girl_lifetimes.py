import unittest
from tools.script_recovery.teddy_girl_lifetimes import inventory

class TeddyGirlLifetimeTests(unittest.TestCase):
    def test_all_native_paths_balance_control_movies_retained_things_and_presented_text(self):
        result=inventory();self.assertTrue(result['cfgLifetimeProved'])
        starts=[r['object'] for r in result['events'] if r['operation']=='start']
        self.assertEqual(starts.count(('control',20)),1)
        self.assertEqual(starts.count(('thing',40)),1)
        self.assertEqual(starts.count(('presented',16)),1)
        self.assertEqual({x for x in starts if x[0]=='movie'},{('movie',72),('movie',200),('movie',168),('movie',56),('movie',184)})
        self.assertIn(('thing',156),starts)
        self.assertTrue(result['limits'])

if __name__=='__main__':unittest.main()
