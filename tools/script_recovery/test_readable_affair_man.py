import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_structure_affair_man_lua as structure_tests
from tools.script_recovery.readable_affair_man import readable_man_source

class HusbandReadabilityTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        structure_tests.HusbandStructureTests.setUpClass()
        cls.before=structure_tests.HusbandStructureTests.after
        cls.after,cls.report=readable_man_source(cls.before)

    def test_full_candidate_cleanup_and_cancellation_traces_unchanged(self):
        runner=structure_tests.HusbandStructureTests()
        for scenario in ({'hit':True},{'talk':True},{'talk':True,'retryTalk':True},
                         {'talk':True,'womanDead':True,'wifeDead':True},{'idle':True},{'acquire':False}):
            for stop in range(1,31):
                case=dict(acquire=True,terminateAtFrame=5,terminateAtCheck=stop);case.update(scenario)
                with self.subTest(case=case):self.assertEqual(runner.events(self.before,case),runner.events(self.after,case))
        for failure in ('failSpeak','failHealth','failLookup'):
            for interaction in ('hit','talk'):
                case=dict(acquire=True,terminateAtFrame=5);case[failure]=case[interaction]=True
                self.assertEqual(runner.events(self.before,case),runner.events(self.after,case))

    def test_branch_equality_and_boolean_values_not_lua_numeric_truthiness(self):
        # Both random outcomes and out-of-range inputs retain numeric equality.
        lua=LuaRuntime()
        before='local scratchValue; local randomChoice3=...; scratchValue=randomChoice3==0; if scratchValue then return "kiss" else return "hug" end'
        after='local randomChoice3=...; if randomChoice3==0 then return "kiss" else return "hug" end'
        for value in (0,1,-1,2,float('nan')):self.assertEqual(lua.execute(before,value),lua.execute(after,value))
        for value in (0,1):self.assertEqual(lua.eval(f'((0x{value}) ~= 0)'),bool(value))
        self.assertGreater(self.report['literalBooleanComparisons'],10)
        self.assertNotIn('scratchValue',self.after)
        self.assertNotIn('((0x0) ~= 0)',self.after)
        self.assertNotIn('goto ',self.after)

    def test_extra_scratch_reader_or_changed_comparison_rejected(self):
        for source in (self.before+'\nprint(scratchValue)\n',self.before.replace('scratchValue = randomChoice3 == 0','scratchValue = randomChoice3 ~= 0')):
            with self.assertRaises(ValueError):readable_man_source(source)

    def test_diagnostics_strings_unknown_operands_preserved(self):
        suffix='\n-- ((0x0) ~= 0) unresolved\nlocal diagnostic="((0x1) ~= 0)"\nlocal unknown = ((native_unknown) ~= 0)\n'
        result,_=readable_man_source(self.before+suffix)
        self.assertTrue(result.endswith(suffix))

if __name__=='__main__':unittest.main()
