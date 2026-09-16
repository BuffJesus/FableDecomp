import unittest
from tools.script_recovery.native_oakvale_restore_order import execute


class RestoreOrderTests(unittest.TestCase):
    def test_activation_initializes_and_registers_before_optional_restore(self):
        for saved in (False,True):
            for attached in (False,True):
                with self.subTest(saved=saved,attached=attached):execute(saved,attached)


if __name__=='__main__':unittest.main()
