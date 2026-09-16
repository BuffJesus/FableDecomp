import unittest
from tools.script_recovery.guard_lifetime_inventory import inventory

class GuardLifetimeInventoryTests(unittest.TestCase):
    def test_every_native_cfg_path_releases_control_and_each_live_movie(self):
        result=inventory()
        self.assertTrue(result['cfgLifetimeProved'])
        starts=[(r['address'],r['object']) for r in result['events'] if r['operation']=='start']
        self.assertEqual(starts,[(0xdac776,('resource',16)),(0xdacaca,('movie',32)),(0xdadbef,('movie',100))])
        self.assertEqual(result['selections'][0xdade2e],('movie',100))
        self.assertEqual(result['selections'][0xdadd9c],('movie',32))
        self.assertTrue(result['limits'])

if __name__=='__main__':unittest.main()
