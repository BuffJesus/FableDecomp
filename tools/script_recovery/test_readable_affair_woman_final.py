import unittest
from tools.script_recovery import test_structure_affair_woman as structure_tests
from tools.script_recovery import test_generate_affair_woman_resource_candidate as candidate_tests
from tools.script_recovery.readable_affair_woman_final import readable_woman_final

class WomanFinalReadabilityTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        structure_tests.WomanStructureTests.setUpClass()
        cls.before=structure_tests.WomanStructureTests.structured
        cls.after,cls.report=readable_woman_final(cls.before)

    def test_all_branch_cancellation_runoff_and_error_traces(self):
        scenarios=({},{'hit':True},{'talk':True},{'move':True},{'runoff':True},
            {'runoff':True,'runoff_moves':2},{'runoff':True,'camera':True},{'fail_acquire':True},
            {'hit':True,'busy':True},{'talk':True,'busy':True},
            {'kiss':True,'hug':True,'animation_byte':47},{'hit':True,'health':0},{'talk':True,'health':0})
        for scenario in scenarios:
            for stop in range(1,40):
                results=[]
                for source in (self.before,self.after):
                    runner=candidate_tests.AffairWomanCandidateTests();runner.source=source
                    results.append(runner.run_case(**scenario,stop_check=stop,stop_frame=8,trace_frames=True))
                self.assertEqual(*results)
        for scenario in ({'hit':True,'error_at':'health'},{'hit':True,'error_at':'speak'},{'talk':True,'error_at':'face'}):
            results=[]
            for source in (self.before,self.after):
                runner=candidate_tests.AffairWomanCandidateTests();runner.source=source
                events,error=runner.run_case(**scenario);self.assertIn('injected',error);results.append(events)
            self.assertEqual(*results)

    def test_changed_operands_reject_and_all_other_lines_remain_identical(self):
        for changed in (self.before.replace('((0x1) ~= 0)','((0x0) ~= 0)',1),self.before.replace('TEXT_QST_048_AFFAIRWOMAN_BUSY','UNKNOWN')):
            with self.assertRaises(ValueError):readable_woman_final(changed)
        before=self.before.splitlines();after=self.after.splitlines()
        differences=[(a,b) for a,b in zip(before,after) if a!=b]
        self.assertEqual(len(before),len(after));self.assertEqual(len(differences),2)
        self.assertTrue(all('resources:Speak' in a and 'resources:Speak' in b for a,b in differences))
        self.assertEqual(self.report['literalBooleanComparisons'],4)

if __name__=='__main__':unittest.main()
