import unittest
from unittest.mock import patch
from capstone import Cs, CS_ARCH_X86, CS_MODE_32

from tools.script_recovery import test_native_affair_wife_pause as fixtures
from tools.script_recovery.native_affair_pause import recover_affair_wife_pause
from tools.script_recovery.native_affair_movies import recover_affair_wife_movies
from tools.script_recovery.native_resource_lifetime import check_single_resource_lifetime


class AffairWifeMoviesTests(unittest.TestCase):
    def inputs(self):
        fn, source, data, manifest = fixtures.AffairWifePauseTests().inputs()
        source, _ = recover_affair_wife_pause(fn, source, data, manifest)
        return fn, source, data, manifest

    def test_shared_end_sites_balance_both_resources(self):
        fn, source, data, manifest = self.inputs()
        result, evidence = recover_affair_wife_movies(fn, source, data, manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertTrue(evidence[0]['lifetimeChecked'])
        self.assertEqual(result.count('GSI->EndMovieSequence();'), 7)
        self.assertEqual(result.count('GSI->StartMovieSequence();'), 4)
        self.assertNotIn('CCarriedReadableDef::CCarriedReadableDef', result)
        # Misnamed actor destructors are not movie ends.
        self.assertEqual(result.count('CScriptGameResourceObjectMovieBase::~CScriptGameResourceObjectMovieBase'), source.count('CScriptGameResourceObjectMovieBase::~CScriptGameResourceObjectMovieBase') - 7)
        w = evidence[0]
        decoder = Cs(CS_ARCH_X86, CS_MODE_32)
        decoder.detail = True
        instructions = list(decoder.disasm(data.bytes_at(0xDB2B10, w['size']), 0xDB2B10))
        events = {e['setup']['address']: (e['operation'], tuple(e['resource']) if isinstance(e['resource'], list) else e['resource']) for e in w['events']}
        selections = {int(a, 16): r for a, r in w['selections'].items()}
        selections[0xDB3DD0] = 64
        self.assertFalse(check_single_resource_lifetime(instructions, events, receiver_register='ecx', selections=selections))

    def test_failed_lifetime_proof_preserves_source(self):
        fn, source, data, manifest = self.inputs()
        with patch('tools.script_recovery.native_affair_movies.check_single_resource_lifetime', return_value=False):
            result, evidence = recover_affair_wife_movies(fn, source, data, manifest)
        self.assertEqual(result, source)
        self.assertEqual(evidence[0]['status'], 'rejected')
