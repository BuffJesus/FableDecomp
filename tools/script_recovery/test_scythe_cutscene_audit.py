import unittest
from dataclasses import replace
from unittest.mock import patch

from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.scythe_cutscene_audit import audit


class ScytheCutsceneAuditTests(unittest.TestCase):
    def test_flags_camera_and_real_adapter_difference_are_explicit(self):
        report = audit()
        self.assertTrue(report['status'].startswith('checked'))
        self.assertFalse(report['runtimeChanged'])
        self.assertTrue(report['nativeAcquisitionResultIgnored'])
        proposal = report['proposal']
        self.assertEqual((proposal['arguments']['setupcond'], proposal['arguments']['skippable']), (False, True))
        self.assertEqual(proposal['cameraBracket'], [True, False])
        self.assertTrue(proposal['callerOwnsMovie'])
        self.assertTrue(proposal['status'].startswith('not lowered'))

    def test_changed_native_macro_flags_camera_or_actor_map_reject(self):
        for site, changes in ((0xE2AA7F, {'stack_arguments': (('constant', 0), ('constant', 0), ('constant', 1), ('constant', 1))}),
                              (0xE2AA7F, {'edx': ('register', 'edi')}),
                              (0xE2A9CA, {'stack_arguments': (('constant', 0),)}),
                              (0xE2AA8E, {'stack_arguments': (('constant', 1),)})):
            def decode(*args, **kwargs):
                setup = read_call_window(*args, **kwargs)
                return replace(setup, **changes) if args[3] == site else setup
            with patch('tools.script_recovery.scythe_cutscene_audit.read_call_window', side_effect=decode):
                self.assertEqual(audit()['status'], 'rejected')

    def test_changed_acquisition_bytes_or_host_snapshot_reject(self):
        data = RData()
        original = data.bytes_at
        def changed(address, size):
            raw = original(address, size)
            if address == 0xE2A900:
                raw = bytearray(raw)
                raw[0x3E] ^= 1
                return bytes(raw)
            return raw
        with patch.object(data, 'bytes_at', side_effect=changed):
            self.assertEqual(audit(data)['status'], 'rejected')
        self.assertEqual(audit(host_source='changed adapter')['status'], 'rejected')

    def test_empty_map_is_not_reported_equivalent_to_null(self):
        report = audit()
        self.assertFalse(report['inputSubstitution']['equivalent'])
        self.assertEqual(report['inputSubstitution']['argumentCount'], 11)
        self.assertEqual([s['value'] for s in report['inputSubstitution']['strings']], ['$', 'NULL'])
        data = RData()
        original = data.bytes_at
        for address in (0xCC03B5, 0x41C830, 0x9EF360):
            def changed(base, size):
                raw = original(base, size)
                return bytes([raw[0] ^ 1]) + raw[1:] if base == address else raw
            with patch.object(data, 'bytes_at', side_effect=changed):
                self.assertEqual(audit(data)['status'], 'rejected')
        with patch.object(data, 'string_at', return_value=''):
            self.assertEqual(audit(data)['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()
