import copy
import unittest
from collections import Counter
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_theresa_movies import verify
from tools.script_recovery.native_theresa_control import verify as verify_control
from tools.script_recovery.native_resource_lifetime import check_resource_lifetimes
from capstone import Cs, CS_ARCH_X86, CS_MODE_32


class TheresaMovieTests(unittest.TestCase):
    def test_control_and_movie_stack_reuse_do_not_overlap(self):
        data = RData()
        control, movie = verify_control(data), verify(data)
        events = {}
        for event in control['events'] + movie['events']:
            setup = event['setup']
            identity = (setup['stack_arguments'][1] if event['name'] in ('acquire', 'start')
                        else setup['ecx'])
            operation = ('start' if event['name'] == 'construct' else
                         'end' if event['name'].startswith('destroy') else 'use')
            self.assertNotIn(event['site'], events)
            events[event['site']] = (operation, tuple(identity))
        decoder = Cs(CS_ARCH_X86, CS_MODE_32)
        decoder.detail = True
        instructions = list(decoder.disasm(data.bytes_at(control['address'], control['size']), control['address']))
        selections = {int(site): tuple(identity) for site, identity in movie['selections'].items()}
        self.assertTrue(check_resource_lifetimes(instructions, events, receiver_register='ecx', selections=selections))

    def test_all_constructions_and_shared_destructor_paths(self):
        w = verify(RData())
        self.assertEqual(Counter(e['name'] for e in w['events']), {'construct': 7, 'start': 7, 'destroy': 16})
        self.assertEqual(len(w['selections']), 4)

    def test_rejects_changed_shared_destructor_selection(self):
        data = RData()
        w = copy.deepcopy(verify(data))
        w['selections'][str(0xdba3cf)] = ['stack', 384]
        with self.assertRaisesRegex(ValueError, 'selections'): verify(data, w)
