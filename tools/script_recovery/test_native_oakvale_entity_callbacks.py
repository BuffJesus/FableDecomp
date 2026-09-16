import unittest
from tools.script_recovery.native_oakvale_entity_callbacks import execute


class EntityCallbackTests(unittest.TestCase):
    def test_native_virtual_forwarding_and_null_script(self):
        for interrupted in (False,True):
            for present in (False,True):
                with self.subTest(interrupted=interrupted,present=present):execute(interrupted,present)


if __name__=='__main__':unittest.main()
