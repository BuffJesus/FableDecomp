import itertools
import unittest
from tools.script_recovery.native_oakvale_entity_ownership import execute,prove


class NativeOakvaleEntityOwnershipTests(unittest.TestCase):
    def test_scalar_return_abi_for_all_factories(self):
        for row in prove()['factories']:
            for flags in range(4):
                with self.subTest(name=row['name'],flags=flags):
                    result=execute(name=row['name'],scalar_flags=flags)
                    self.assertTrue(result['returnedThis'] and result['thingReleased'])
                    self.assertEqual(result['ownerFreed'],bool(flags&1))

    def test_copy_and_final_counted_release(self):
        names=[row['name'] for row in prove()['factories']]
        self.assertEqual(len(names),16)
        for name,count,has_info,shared in itertools.product(names,(1,2,7),(False,True),(False,True)):
            result=execute(count,has_info,shared,name)
            self.assertEqual(result['initialScriptRefcount'],1)
            self.assertEqual(result['retainedThingCount'],count+int(has_info))
            self.assertEqual(result['finalThingCount'],count)


if __name__=='__main__':unittest.main()
