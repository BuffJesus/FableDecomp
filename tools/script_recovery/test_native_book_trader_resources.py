import unittest
from collections import Counter
from dataclasses import replace
from unittest.mock import patch

from tools.script_recovery import test_native_book_trader_termination as fixtures
from tools.script_recovery.native_post_attack_resources import map_book_trader_resources
from tools.script_recovery.native_call_setup_ir import read_call_window


class BookTraderResourceTests(unittest.TestCase):
    def test_native_resource_events_share_one_slot_across_priorities(self):
        fn, source, data, manifest = fixtures.BookTraderTerminationTests().inputs()
        evidence = map_book_trader_resources(fn, data)
        self.assertEqual(evidence[0]['status'], 'mapped')
        self.assertEqual(evidence[0]['loweringStatus'], 'unresolved')
        events = evidence[0]['events']
        self.assertEqual(Counter(e['name'] for e in events),
                         {'acquire': 8, 'has_resource': 4, 'reset': 4, 'destroy': 2})
        acquisitions = [e['setup']['stack_arguments'] for e in events if e['name'] == 'acquire']
        self.assertEqual([a[2][1] for a in acquisitions], [3, 3, 4, 4, 4, 4, 4, 4])
        for args in acquisitions:
            self.assertEqual(args[:2], [['register', 'ebp'], ['stack', 20]])
        for event in events:
            if event['name'] != 'acquire':
                self.assertEqual(event['setup']['ecx'], ['register', 'ecx'] if event['site'] in ('0xdb4f5a', '0xdb455f')
                                 else ['stack', 20])
        # The shared destructor has three separately reviewed receiver selections.
        for site in evidence[0]['sharedDestructorReceiver']['selectionSites']:
            self.assertEqual(data.bytes_at(int(site, 16), 4), bytes.fromhex('8d4c2414'))

    def test_wrong_reset_resource_rejects_mapping(self):
        fn, source, data, manifest = fixtures.BookTraderTerminationTests().inputs()
        def changed(*args, **kwargs):
            setup = read_call_window(*args, **kwargs)
            return replace(setup, ecx=('stack', 24)) if args[3] == 0xDB403B else setup
        with patch('tools.script_recovery.native_post_attack_resources.read_call_window', side_effect=changed):
            evidence = map_book_trader_resources(fn, data)
        self.assertEqual(evidence[0]['status'], 'rejected')
