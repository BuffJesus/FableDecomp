import unittest
from dataclasses import replace
from unittest.mock import patch

from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_post_attack_movie_flags as fixtures
from tools.script_recovery.lift_native_lua import Lifter, RData, strip_declarations
from tools.script_recovery.native_post_attack_scalars import recover_post_attack_movie_flags
from tools.script_recovery.native_post_attack_limbo import recover_post_attack_limbo
from tools.script_recovery.native_call_setup_ir import read_call_window


class PostAttackLimboTests(unittest.TestCase):
    def inputs(self):
        fn, source, manifest = fixtures.PostAttackMovieFlagTests().inputs()
        return fn, recover_post_attack_movie_flags(fn, source, RData(), manifest)[0], manifest

    def test_both_limbo_calls_use_their_fresh_lookup_and_native_boolean(self):
        fn, source, manifest = self.inputs()
        recovered, evidence = recover_post_attack_limbo(fn, source, RData(), manifest)
        self.assertEqual([e['status'] for e in evidence], ['recovered', 'recovered'])
        selected, active = [], False
        for statement in strip_declarations(recovered):
            if 'CCharString::CCharString' in statement and '"V_OakVale"' in statement:
                active = True
                selected.append(statement)
            elif active and ('GSI->GetThingWithScriptName' in statement or 'GSI->SetVillageLimbo' in statement):
                selected.append(statement)
                if 'SetVillageLimbo' in statement:
                    active = False
        lifter = Lifter(manifest, {}, 'quest', False, '', RData())
        body = '\n'.join(lifter.lift('Limbo', '{\n' + '\n'.join(selected) + '\n}'))
        self.assertEqual(lifter.todo, [])
        lua, events, count = LuaRuntime(), [], [0]
        def lookup(_q, name):
            count[0] += 1
            events.append(('lookup', name))
            return 'village' + str(count[0])
        quest = lua.table_from({'GetThingWithScriptName': lookup,
                               'SetVillageLimbo': lambda _q, who, flag: events.append(('limbo', who, flag))})
        lua.execute('return function(quest)\n' + body + '\nend')(quest)
        self.assertEqual(events, [('lookup', 'V_OakVale'), ('limbo', 'village1', True),
                                  ('lookup', 'V_OakVale'), ('limbo', 'village2', False)])

    def test_changed_lookup_result_or_flag_rejects_both_replacements(self):
        fn, source, manifest = self.inputs()
        for site in (0xDBEC6E, 0xDBEEE7):
            for bad in ((('unknown',), ('constant', 0)), (('result', 1, 'village'), ('constant', 1))):
                def decode(*args, **kwargs):
                    result = read_call_window(*args, **kwargs)
                    return replace(result, stack_arguments=bad) if args[3] == site else result
                with patch('tools.script_recovery.native_post_attack_limbo.read_call_window', side_effect=decode):
                    result, evidence = recover_post_attack_limbo(fn, source, RData(), manifest)
                self.assertEqual(result, source)
                self.assertEqual(evidence[0]['status'], 'rejected')
        self.assertEqual(recover_post_attack_limbo(fn, source + '\n', RData(), manifest)[1][0]['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()
