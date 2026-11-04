import contextlib
import io
import json
import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import verify_and_land as V
from reclassify_near_matches import audit_candidates
from tools.write_decomp_dashboard import verified_near_matches


class VerifierTests(unittest.TestCase):
    def test_audit_includes_new_near_landings_without_duplicate_or_exact_rows(self):
        prior = [{'address': '00000001', 'status': 'DIFFER'},
                 {'address': '00000002', 'status': 'MATCH'},
                 {'address': '00000003', 'status': 'OBJECT_MISSING'}]
        catalog = {a: {'Grade': 'near_match'} for a in ('00000001', '00000002', '00000003', '00000004')}
        rows = audit_candidates(prior, catalog)
        self.assertEqual([r['address'] for r in rows], ['00000001', '00000003', '00000004'])
        self.assertEqual(rows[-1]['status'], 'CATALOG_NEAR_MATCH')

    def run_candidate(self, behavior='PASS', source='int f() { return 1; }'):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            catalog = root/'rebuild/build_candidates.ps1'
            catalog.parent.mkdir(parents=True)
            catalog.write_text('$catalog = @(\n)\n')
            oracle = root/'rebuild/oracles/auto-re-candidates.tsv'
            oracle.parent.mkdir()
            oracle.write_text('address\tname\tlength\tbytes\n00400000\tf\t1\tc3\n')
            payload = root/'input.json'
            payload.write_text(json.dumps({'result': {'authored': [{
                'address': '00400000', 'name': 'f', 'source_cpp': source,
                'test_cpp': 'test', 'pass_pattern': 'PASS'}]}}))
            with patch.object(V, 'ROOT', root), patch.object(V, 'WORK_ROOT', root/'work'), \
                 patch.object(V, 'manifest_addresses', return_value={'00400000'}), \
                 patch.object(V, 'parity_of', return_value=('NEAR_MATCH', b'\xc3', 0)) as parity, \
                 patch.object(V, 'behaviour_of', return_value=behavior) as fixture, \
                 patch.object(sys, 'argv', ['verify', str(payload), str(oracle), '--land']), \
                 contextlib.redirect_stdout(io.StringIO()):
                V.main()
            return catalog.read_text(), parity.call_count, fixture.call_args

    def test_near_match_stops_sweep_and_lands_with_grade(self):
        catalog, calls, fixture = self.run_candidate()
        self.assertIn("Grade = 'near_match'", catalog)
        self.assertEqual(calls, 2)  # base and final object; no pragma sweep
        self.assertTrue(fixture.kwargs['require_candidate'])

    def test_behavior_failure_cannot_land(self):
        for status in ('FAIL', 'SELF_CONTAINED_TEST', 'LINK_FAIL'):
            catalog, _, _ = self.run_candidate(status)
            self.assertNotIn('Address', catalog)

    def test_assembly_still_rejected(self):
        catalog, calls, _ = self.run_candidate(source='void f() { __asm ret }')
        self.assertNotIn('Address', catalog)
        self.assertEqual(calls, 0)

    def test_dashboard_rejects_stale_and_duplicate_promotions(self):
        import hashlib
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            gate = root/'rebuild/compile-gate'
            gate.mkdir(parents=True)
            (gate/'retail-parity.tsv').write_text('address\tstatus\n00400000\tDIFFER\n')
            (root/'source.cpp').write_text('source')
            (root/'test.cpp').write_text('test')
            row = {'address': '00400000', 'status': 'NEAR_MATCH', 'behavior': 'PASS',
                   'source': 'source.cpp', 'test': 'test.cpp',
                   'source_sha256': hashlib.sha256(b'source').hexdigest(),
                   'test_sha256': hashlib.sha256(b'test').hexdigest()}
            (gate/'near-match.json').write_text(json.dumps({'rows': [row, row]}))
            self.assertEqual(verified_near_matches(root), 1)
            (root/'source.cpp').write_text('changed')
            self.assertEqual(verified_near_matches(root), 0)
            (root/'source.cpp').write_text('source')
            (gate/'retail-parity.tsv').write_text('address\tstatus\n00400000\tMATCH\n')
            self.assertEqual(verified_near_matches(root), 0)


if __name__ == '__main__':
    unittest.main()
