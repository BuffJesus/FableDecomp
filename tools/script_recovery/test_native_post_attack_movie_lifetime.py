import copy
import unittest
from types import SimpleNamespace
from unittest.mock import patch

from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_post_attack_limbo as fixtures
from tools.script_recovery.lift_native_lua import Lifter, RData, strip_declarations
from tools.script_recovery.native_post_attack_limbo import recover_post_attack_limbo
from tools.script_recovery.native_post_attack_movie_lifetime import recover_post_attack_movie_lifetime
from tools.script_recovery.native_post_attack_resources import map_post_attack_resources


class PostAttackMovieLifetimeTests(unittest.TestCase):
    def inputs(self):
        fn, source, manifest = fixtures.PostAttackLimboTests().inputs()
        return fn, recover_post_attack_limbo(fn, source, RData(), manifest)[0], manifest

    def test_native_movie_lifetime_emits_one_start_and_end_in_cleanup_order(self):
        fn, source, manifest = self.inputs()
        recovered, evidence = recover_post_attack_movie_lifetime(fn, source, RData(), manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertIn('CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&stack0xffffff98)', recovered)
        self.assertLess(recovered.index('GSI->EndMovieSequence();'), recovered.index('StdMap_Destroy_API();'))
        self.assertGreater(recovered.index('GSI->EndMovieSequence();'), recovered.index('RunCutsceneMacro_Func();'))
        calls = [s.strip() for s in strip_declarations(recovered)
                 if s.strip() in ('GSI->StartMovieSequence();', 'GSI->EndMovieSequence();')]
        self.assertEqual(calls, ['GSI->StartMovieSequence();', 'GSI->EndMovieSequence();'])
        lifter = Lifter(manifest, {}, 'quest', False, '', RData())
        body = '\n'.join(lifter.lift('MovieLifetime', '{\n' + '\n'.join(calls) + '\n}'))
        self.assertEqual(lifter.todo, [])
        lua, events = LuaRuntime(), []
        quest = lua.table_from({'StartMovieSequence': lambda _q: events.append('start'),
                               'EndMovieSequence': lambda _q: events.append('end')})
        lua.execute('return function(quest)\n' + body + '\nend')(quest)
        self.assertEqual(events, ['start', 'end'])

    def test_changed_helpers_or_source_reject_cleanup_translation(self):
        fn, source, manifest = self.inputs()
        data = RData()
        for changed in (0x6E7B60, 0x6E7B80):
            def read(address, size):
                raw = data.bytes_at(address, size)
                return bytes([raw[0] ^ 1]) + raw[1:] if address == changed else raw
            result, evidence = recover_post_attack_movie_lifetime(fn, source, SimpleNamespace(bytes_at=read), manifest)
            self.assertEqual(result, source)
            self.assertEqual(evidence[0]['status'], 'rejected')
        self.assertEqual(recover_post_attack_movie_lifetime(fn, source + '\n', data, manifest)[1][0]['status'], 'rejected')

    def test_wrong_resource_or_cleanup_order_cannot_be_lowered(self):
        fn, source, manifest = self.inputs()
        original = map_post_attack_resources(fn, RData())
        for change in ('resource', 'order'):
            graph = copy.deepcopy(original)
            event = next(e for e in graph[0]['events'] if e['name'] == 'movie_destroy')
            if change == 'resource':
                event['setup']['ecx'] = ['stack', 40]
            else:
                event['setup']['address'] = 0xDBEEFF
            with patch('tools.script_recovery.native_post_attack_movie_lifetime.map_post_attack_resources', return_value=graph):
                result, evidence = recover_post_attack_movie_lifetime(fn, source, RData(), manifest)
            self.assertEqual(result, source)
            self.assertEqual(evidence[0]['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()
