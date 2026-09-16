import itertools
import unittest
from tools.script_recovery.native_oakvale_lifecycle import execute,prove
from tools.script_recovery.lift_native_lua import RData


class OakvaleLifecycleNativeTests(unittest.TestCase):
    def test_scalar_flags_timer_vector_and_base_destruction(self):
        for flags,lengths,allocated,pair in itertools.product((0,1,2,3,255),((0,)*8,(2,)*8,(0,1,2,0,2,1,0,1)),(False,True),((-1,0),(0,-1),(7,7))):
            expected=[('timer',pair[0]),('timer',pair[1])]
            for index,length in enumerate(lengths):
                expected.extend(('speech.string',index,i) for i in range(length))
                if length or allocated:expected.append(('speech.free',index))
            expected += [('base.string',0),('base.string',1),('base.free',0x30),
                         ('base.range',0x18),('base.free',0x18),('base.range',8),('base.free',8),
                         ('base.map',),('base.free',4),('base.object',)]
            if flags&1:expected.append(('owner.free',))
            self.assertEqual(execute(flags,lengths,allocated,*pair),expected,(flags,lengths,allocated,pair))

    def test_changed_destructor_rejected(self):
        d=RData()
        class Changed:
            def bytes_at(self,a,n):
                raw=d.bytes_at(a,n);return bytes([raw[0]^1])+raw[1:] if a==0xcbd510 else raw
        with self.assertRaisesRegex(ValueError,'lifecycle native bytes changed'):prove(Changed())


if __name__=='__main__':unittest.main()
