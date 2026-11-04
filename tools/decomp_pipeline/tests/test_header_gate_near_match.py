import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import gate_header_dependents as G

class HeaderGateNearMatchTests(unittest.TestCase):
    def test_near_match_requires_passing_fixture(self):
        self.assertTrue(G.grade_passed(('NEAR_MATCH', 'PASS')))
        for behavior in ('-', 'FAIL', 'SELF_CONTAINED_TEST', 'LINK_FAIL', 'TCC_FAIL'):
            self.assertFalse(G.grade_passed(('NEAR_MATCH', behavior)))
        self.assertFalse(G.grade_passed(('DIFFER(10v10)', 'PASS')))
        self.assertTrue(G.grade_passed(('RELOCATION_MATCH', 'PASS')))

    def test_header_regrade_requires_candidate_for_near_match(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            for part in ('src/compiled', 'tests'):
                (root/'rebuild'/part).mkdir(parents=True)
            (root/'rebuild/src/compiled/f.cpp').write_text('int f(){return 1;}')
            (root/'rebuild/tests/f_test.cpp').write_text('fixture')
            entry = {'Address':'00400000', 'Source':'f.cpp', 'TestSource':'f_test.cpp', 'PassPattern':'PASS'}
            oracle = {'00400000':{'name':'f','bytes':'c3'}}
            with patch.object(G, 'ROOT', root), patch.object(G.V, 'env', return_value={}), \
                 patch.object(G.V, 'parity_of', return_value=('NEAR_MATCH', b'\xc3', 1)), \
                 patch.object(G.V, 'behaviour_of', return_value='SELF_CONTAINED_TEST') as fixture:
                result = G.grade(entry, oracle, root, root)
            self.assertTrue(fixture.call_args.kwargs['require_candidate'])
            self.assertFalse(G.grade_passed(result))

if __name__ == '__main__':
    unittest.main()
