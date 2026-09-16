import unittest
from tools.script_recovery.maze_teardown_native import execute, validate
from tools.script_recovery.lift_native_lua import RData


class MazeTeardownNativeTests(unittest.TestCase):
    def test_termination_drives_active_process_until_it_returns(self):
        for steps in (1,2,5):
            with self.subTest(steps=steps):
                result=execute(resumes=steps)
                self.assertTrue(result['returned'])
                self.assertFalse(result['active'])
                self.assertEqual(result['callbacks'],[{'terminationFlag':1}]*steps)

    def test_inactive_process_is_flagged_without_dispatch(self):
        result=execute(active=False)
        self.assertTrue(result['returned'] and result['terminationFlag'])
        self.assertEqual(result['callbacks'],[])

    def test_noncooperating_process_has_no_proven_finite_drain(self):
        result=execute(resumes=None)
        self.assertFalse(result['returned'])
        self.assertTrue(result['active'] and result['terminationFlag'])
        self.assertGreater(len(result['callbacks']),1)

    def test_named_kill_only_flags_matching_process_without_removal(self):
        for match in (False,True):
            result=execute(kind='named',id_matches=match)
            self.assertTrue(result['returned'] and result['stillLinked'] and result['active'])
            self.assertEqual(result['terminationFlag'],match)
            self.assertEqual(result['callbacks'],[])

    def test_changed_native_cancellation_operand_rejects(self):
        data=RData()
        class Changed:
            def bytes_at(self,address,size):
                value=data.bytes_at(address,size)
                if address==0xa4b200:
                    value=bytearray(value);value[8]^=1;value=bytes(value)
                return value
        with self.assertRaisesRegex(ValueError,'TerminateProcess'):
            validate(Changed())


if __name__=='__main__':unittest.main()
