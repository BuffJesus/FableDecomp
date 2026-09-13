import unittest
from tools.script_recovery.analyze_bully_run_variance import analyze_text

class BullyRunVarianceTests(unittest.TestCase):
 def test_classifies_complete_and_partial_runs(self):
  text='''NOVI_PROBE BullyRun before bully=(3606.745,861.278,13.666) victim=(0,0,0)
NOVI_PROBE BullyRun after1 bully=(3596.740,851.895,13.375) victim=(0,0,0)
NOVI_PROBE BullyRun before bully=(3606.752,861.396,13.679) victim=(0,0,0)
NOVI_PROBE BullyRun after1 bully=(3604.479,860.742,13.707) victim=(0,0,0)
'''
  rows=analyze_text(text)
  self.assertEqual([row['reachedRetailTolerance'] for row in rows],[True,False])
  self.assertGreater(rows[1]['markerRemaining'],10)
  self.assertAlmostEqual(rows[1]['moved'],2.365,places=2)
  self.assertEqual(rows[1]['runStartDisplacement'],0)
  self.assertTrue(rows[1]['endedAtPreRunTeleport'])
  self.assertFalse(rows[0]['endedAtPreRunTeleport'])
  self.assertGreater(rows[0]['runStartDisplacement'],11)

if __name__=='__main__':unittest.main()
