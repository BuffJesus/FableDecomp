import unittest
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_region_message_native import verify
from tools.script_recovery.rock_region_message_prepare import prepare


class RegionMessageTests(unittest.TestCase):
    def test_native_constructor_destructor_and_loop_are_pinned(self):
        self.assertEqual(len(verify()['scope']['regions']),3)
        class Changed(RData):
            def bytes_at(self,address,size):
                raw=super().bytes_at(address,size)
                return bytes([raw[0]^1])+raw[1:] if address==self.changed else raw
        for address in (0x99e4b0,0x99eae0,0x99e9b0,0xec3d40):
            data=Changed();data.changed=address
            with self.subTest(address=address),self.assertRaises(ValueError): verify(data)

    def test_actual_x86_binding_lifetime_and_raw_boolean_policies(self):
        self.assertIn('8 lifetime/error policies passed',prepare()['compiledTest'])


if __name__=='__main__':unittest.main()
