import unittest
from dataclasses import replace
from types import SimpleNamespace
from unittest.mock import patch

from tools.script_recovery import test_native_post_attack_cleanup as fixtures
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_post_attack_resources import map_post_attack_resources
from tools.script_recovery.native_call_setup_ir import read_call_window


class PostAttackResourceTests(unittest.TestCase):
    def test_actual_actor_map_movie_and_cleanup_relationships(self):
        fn, _, _manifest, _things = fixtures.PostAttackCleanupTests().inputs()
        graph = map_post_attack_resources(fn, RData())[0]
        self.assertEqual((graph['status'], graph['loweringStatus']), ('mapped', 'unresolved'))
        events = {e['name']: e['setup'] for e in graph['events']}
        actor = events['acquire']['stack_arguments'][1]
        self.assertEqual(events['actor_construct']['ecx'], actor)
        self.assertEqual(events['map_assign']['stack_arguments'], [actor])
        self.assertEqual(events['actor_destroy']['ecx'], actor)
        actor_map = events['map_construct']['ecx']
        self.assertEqual(events['map_entry']['ecx'], actor_map)
        self.assertEqual(events['cutscene']['edx'], actor_map)
        self.assertEqual(events['map_destroy']['ecx'], actor_map)
        self.assertEqual(events['map_assign']['ecx'], ['result', 0xDBEDF2, 'entry'])
        movie = events['movie_construct']['ecx']
        self.assertEqual(events['movie_start']['stack_arguments'][1], movie)
        self.assertEqual(events['movie_destroy']['ecx'], movie)
        self.assertEqual(events['cutscene']['stack_arguments'], [['constant', n] for n in (0, 0, 0, 1)])
        self.assertEqual(graph['resources']['actors']['binding']['key'], 'HERO')
        self.assertTrue(graph['limitations'])

    def test_changed_event_target_prevents_a_mapped_graph(self):
        fn, _, _, _ = fixtures.PostAttackCleanupTests().inputs()
        graph = map_post_attack_resources(fn, RData())[0]
        for event in graph['events']:
            def decode(*args, **kwargs):
                result = read_call_window(*args, **kwargs)
                return replace(result, target=('constant', 0)) if args[3] == int(event['site'], 16) else result
            with patch('tools.script_recovery.native_post_attack_resources.read_call_window', side_effect=decode):
                evidence = map_post_attack_resources(fn, RData())
            self.assertEqual(evidence[0]['status'], 'rejected')
            self.assertIn(event['name'], evidence[0]['reason'])

    def test_changed_native_profile_or_string_rejects_graph(self):
        fn, _, _, _ = fixtures.PostAttackCleanupTests().inputs()
        data = RData()
        for changed in (0xDBEB20, 0x891CA0, 0xCD3D2E):
            def read(address, size):
                raw = data.bytes_at(address, size)
                return bytes([raw[0] ^ 1]) + raw[1:] if address == changed else raw
            self.assertEqual(map_post_attack_resources(fn, SimpleNamespace(bytes_at=read))[0]['status'], 'rejected')
        changed = SimpleNamespace(bytes_at=data.bytes_at, string_at=lambda _a: 'different')
        self.assertEqual(map_post_attack_resources(fn, changed)[0]['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()
