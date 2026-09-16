import itertools,unittest
from tools.script_recovery.guard_init import prove,SOURCE
from tools.script_recovery.guard_init_native import execute
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.guard_capabilities import prove as capabilities

class GuardInitTests(unittest.TestCase):
    def test_original_init_all_four_consumers_release_each_copy(self):
        for data_kind,info,home in itertools.product(('empty','invalid','valid'),(False,True),((7.5,-3.25,11.0),(-8.0,0.0,2.25))):
            with self.subTest(data_kind=data_kind,info=info,home=home):
                self.assertEqual(execute(data_kind,info,home),[
                    ('damage',False),('kill',False,False),('combo',False),('home',home),
                    ('centre',home),('copy.destroy',),('minimum',0.0),('copy.destroy',),
                    ('maximum',6.0),('copy.destroy',),('stateGroup',4),('copy.destroy',),('sheathe',False)])

    def test_changed_copy_increment_callee_destructor_and_dispatch_rejected(self):
        original=RData()
        class Changed:
            def __init__(self,at):self.at=at
            def bytes_at(self,at,size):
                raw=original.bytes_at(at,size)
                if at<=self.at<at+size:
                    raw=bytearray(raw);raw[self.at-at]^=1;raw=bytes(raw)
                return raw
        for at in (0xdac6af,0xdac6df,0xdac712,0xdac742,0x8a23db,0x8a2690,0x8a2950,0x8a3777,0x1260f0c+0xbec):
            with self.subTest(at=at),self.assertRaises(ValueError):prove(Changed(at))
        self.assertIn('resources:InitializeGuardActor(me)',SOURCE)
        for at in (0x7e7320,0x7e7329,0x1260f0c+0x5b8):
            with self.subTest(at=at),self.assertRaises(ValueError):capabilities(Changed(at))

if __name__=='__main__':unittest.main()
