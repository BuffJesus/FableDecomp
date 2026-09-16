import itertools
import unittest
from tools.script_recovery.native_post_attack_world import execute,prove
from tools.script_recovery.lift_native_lua import RData


class PostAttackWorldNativeTests(unittest.TestCase):
    def test_atomic_calls_own_output_but_forward_returned_pointer(self):
        for kind,alias,empty,count,result,mutate in itertools.product(
                ('alive','teleport','limbo.on','limbo.off','near'),(False,True),(False,True),
                (0,1,2),(False,True),(False,True)):
            with self.subTest(kind=kind,alias=alias,empty=empty,count=count,result=result,mutate=mutate):
                if kind=='near':expected=[('hero',empty),('near',empty,result)]
                else:
                    key='V_OakVale' if kind.startswith('limbo') else 'M_PostAttackStart'
                    expected=[('key.new',key),('lookup',alias,empty)]
                    if kind=='alive':expected += [('alive',alias,result)]
                    elif kind=='teleport':expected += [('hero',empty),('teleport',alias,empty,mutate)]
                    else:expected += [('limbo',alias,kind=='limbo.on',mutate)]
                    if count==1:expected += [('object.destroy',),('info.free',)]
                    expected += [('output.destroy',),('key.destroy',key)]
                self.assertEqual(execute(kind,alias,empty,count,result,mutate),expected)

    def test_changed_native_evidence_rejected(self):
        d=RData()
        class Changed:
            def bytes_at(self,a,n):
                raw=d.bytes_at(a,n)
                return bytes([raw[0]^1])+raw[1:] if a==0xdbeb20 else raw
        with self.assertRaisesRegex(ValueError,'native bytes changed'):prove(Changed())


if __name__=='__main__':unittest.main()
