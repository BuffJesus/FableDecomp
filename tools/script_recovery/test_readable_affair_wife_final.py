import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_structure_affair_wife as structure_tests
from tools.script_recovery import test_generate_affair_wife_resource_candidate as candidate_tests
from tools.script_recovery.readable_affair_wife_final import readable_wife_final

class WifeFinalReadabilityTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        structure_tests.WifeStructureTests.setUpClass()
        cls.before=structure_tests.WifeStructureTests.readable
        cls.after,cls.report=readable_wife_final(cls.before)

    def test_whole_wife_cancellation_argument_callback_and_errors(self):
        scenarios=({},{'hit':True},{'talk':True},{'talk':True,'discovered':True},
            {'talk':True,'discovered':True,'answer':0},{'fail_acquire':True},
            {'going':True,'busy':True},{'going':True,'busy':True,'talk':True},
            {'going':True,'busy':True,'hit':True},{'going':True,'approach_checks':3},
            {'going':True,'approach_checks':3,'running_line':True},{'going':True,'busy':True,'text_limit':20})
        for scenario in scenarios:
            for stop in range(1,65):
                results=[]
                for source in (self.before,self.after):
                    runner=candidate_tests.WifeCandidateTests();runner.source=source
                    results.append(runner.run_case(**scenario,stop_check=stop,stop_frame=8,trace_frames=True))
                self.assertEqual(results[0],results[1])
        for scenario,location in (({'hit':True},'health'),({'hit':True},'speak'),({'going':True,'busy':True},'reply')):
            results=[]
            for source in (self.before,self.after):
                runner=candidate_tests.WifeCandidateTests();runner.source=source
                events,error=runner.run_case(**scenario,error_at=location,stop_frame=8,trace_frames=True)
                self.assertIn('injected',error);results.append(events)
            self.assertEqual(*results)

    def test_changed_consumers_callback_capture_and_unknowns_rejected_or_preserved(self):
        for changed in (self.before+'\nconsume(scratchValue)',self.before.replace('scratchValue = wifeAnimationRemainder == 0','scratchValue = wifeAnimationRemainder ~= 0'),
                self.before.replace('if not argumentKey:Exists() then','consume(scratchValue)\n if not argumentKey:Exists() then')):
            with self.assertRaises(ValueError):readable_wife_final(changed)
        suffix='\n-- ((0x0) ~= 0) unresolved\nlocal diagnostic="((0x1) ~= 0)"\nlocal unknown=((native_unknown) ~= 0)\n'
        result,_=readable_wife_final(self.before+suffix);self.assertTrue(result.endswith(suffix))
        self.assertTrue(self.report['callbackPreserved'])
        self.assertNotIn('scratchValue = wifeAnimationRemainder',self.after)
        self.assertIn('if wifeAnimationRemainder == 0 then',self.after)
        self.assertGreater(self.report['literalBooleanComparisons'],10)

    def test_animation_choice_keeps_numeric_equality_for_both_outcomes(self):
        lua=LuaRuntime()
        for value in (0,1,-1,float('nan')):
            before=lua.execute('local wifeAnimationRemainder=...; local scratchValue=wifeAnimationRemainder==0; if scratchValue then return "POINT_AWAY" else return "POINT_AT" end',value)
            after=lua.execute('local wifeAnimationRemainder=...; if wifeAnimationRemainder==0 then return "POINT_AWAY" else return "POINT_AT" end',value)
            self.assertEqual(before,after)

if __name__=='__main__':unittest.main()
