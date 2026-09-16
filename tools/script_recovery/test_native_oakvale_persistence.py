import itertools
import unittest
from tools.script_recovery.native_oakvale_persistence import execute


class NativeOakvalePersistenceTests(unittest.TestCase):
    def test_original_single_byte_transfer_and_default(self):
        for initial,reading,saved in itertools.product((False,True),(False,True),(None,False,True)):
            result=execute(initial,reading,saved)
            self.assertEqual(result['attackOver'],bool(saved) if reading else initial)
            self.assertEqual(result['saved'],saved if reading else initial)
            self.assertEqual(result['transfers'],[('AttackOver',initial,False)])


if __name__=='__main__':unittest.main()
