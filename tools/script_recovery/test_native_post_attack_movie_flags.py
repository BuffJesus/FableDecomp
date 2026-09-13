import unittest
from dataclasses import replace
from unittest.mock import patch

from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_post_attack_distance as fixtures
from tools.script_recovery.lift_native_lua import Lifter, RData, strip_declarations
from tools.script_recovery.native_teddy_distance import recover_post_attack_distance
from tools.script_recovery.native_post_attack_scalars import recover_post_attack_movie_flags
from tools.script_recovery.native_call_setup_ir import read_call_window


class PostAttackMovieFlagTests(unittest.TestCase):
    def inputs(self):
        fn, source, manifest = fixtures.PostAttackDistanceTests().inputs()
        return fn, recover_post_attack_distance(fn, source, RData())[0], manifest

    def test_native_flags_override_stale_scalars_and_emit_booleans(self):
        fn, source, manifest = self.inputs()
        recovered, evidence = recover_post_attack_movie_flags(fn, source, RData(), manifest)
        self.assertEqual([e['word'] for e in evidence], [1, 1, 0])
        calls = [s.strip() for s in strip_declarations(recovered)
                 if s.strip().startswith(('GSI->PauseAllNonScriptedEntities(', 'GSI->FixMovieSequenceCamera('))][:3]
        lifter = Lifter(manifest, {}, 'quest', False, '', RData())
        body = '\n'.join(lifter.lift('MovieFlags', '{\nfVar11 = 0;\n' + '\n'.join(calls) + '\n}'))
        self.assertEqual(lifter.todo, [])
        lua, events = LuaRuntime(), []
        quest = lua.table_from({'PauseAllNonScriptedEntities': lambda _q, value: events.append(('pause', value)),
                               'FixMovieSequenceCamera': lambda _q, value: events.append(('camera', value))})
        lua.execute('return function(quest)\n' + body + '\nend')(quest)
        self.assertEqual(events, [('pause', True), ('camera', True), ('camera', False)])
        self.assertTrue(all(type(value) is bool for _, value in events))

    def test_each_reviewed_native_flag_must_match_before_any_replacement(self):
        fn, source, manifest = self.inputs()
        for site in (0xDBEE47, 0xDBEE54, 0xDBEE8F):
            def decode(*args, **kwargs):
                result = read_call_window(*args, **kwargs)
                return replace(result, stack_arguments=(('constant', 99),)) if args[3] == site else result
            with patch('tools.script_recovery.native_post_attack_scalars.read_call_window', side_effect=decode):
                result, evidence = recover_post_attack_movie_flags(fn, source, RData(), manifest)
            self.assertEqual(result, source)
            self.assertEqual(evidence[0]['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()
