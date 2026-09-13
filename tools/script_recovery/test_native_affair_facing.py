import re
import unittest

from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_cached_thing_queries as fixtures
from tools.script_recovery.native_cached_thing_queries import recover_cached_thing_queries
from tools.script_recovery.native_affair_facing import recover_affair_facing
from tools.script_recovery.native_affair_pause import recover_affair_pause
from tools.script_recovery.native_affair_movies import recover_affair_movies
from tools.script_recovery.lift_native_lua import (RData, Lifter, load_slots, load_thing_tables,
    thing_signatures, known_callee_aliases)


class AffairFacingTests(unittest.TestCase):
    def inputs(self):
        fn, source, manifest = fixtures.CachedThingQueryTests().inputs()
        source, _ = recover_cached_thing_queries(fn, source, RData())
        return fn, source, manifest

    def test_actual_branch_faces_selected_actor_towards_man_or_exits_on_termination(self):
        fn, source, manifest = self.inputs()
        recovered, evidence = recover_affair_facing(fn, source, RData())
        self.assertEqual(evidence[0]['status'], 'recovered')
        recovered, pause_evidence = recover_affair_pause(fn, recovered, RData(), manifest)
        self.assertEqual(pause_evidence[0]['status'], 'recovered')
        recovered, movie_evidence = recover_affair_movies(fn, recovered, RData(), manifest)
        self.assertEqual(movie_evidence[0]['status'], 'recovered')
        self.assertEqual(recovered.count('GSI->EndMovieSequence();'), 12)
        start = recovered.index('LAB_00db1593:')
        end = recovered.index('goto LAB_00db1c71;', start) + len('goto LAB_00db1c71;')
        region = recovered[start:end]
        lookups = [re.search(r'GSI->GetThingWithScriptName\(&local_native_cached_' + who + r'[^;]+;', recovered)[0]
                   for who in ('woman', 'wife')]
        things, _ = load_thing_tables(manifest, load_slots())
        lifter = Lifter(manifest, {}, 'quest', True, '', RData(), native_gotos=True,
                        live_termination=True, thing_sigs=thing_signatures(things),
                        callee_names=known_callee_aliases(fn))
        body = '\n'.join(lifter.lift('Facing', '{\n' + '\n'.join(lookups) +
            '\npCVar1 = (CScriptThing *)(param_1 + 8);\n' + region +
            '\nLAB_00db1d7c:\nreturn false;\nLAB_00db1c71:\nreturn true;\n}'))
        self.assertIn('goto LAB_00db15e6', body)
        self.assertNotIn('TODO(native): goto LAB_00db15e6', body)
        for woman_alive, wife_alive, terminating, expected, target in (
                (True, True, False, True, 'woman'),
                (False, True, False, True, 'wife'),
                (None, True, False, True, 'wife'),
                (True, True, True, False, None),
                (False, True, True, False, None),
                (None, None, False, True, None)):
            lua, faced, unpaused, cleanup = LuaRuntime(), [], [], []
            def entity(name, alive):
                return None if alive is None else lua.table_from({'name': name, 'IsAlive': lambda *_: alive})
            handles = {'NOVI_AffairWoman': entity('woman', woman_alive), 'NOVI_AffairWife': entity('wife', wife_alive)}
            def face(_q, actor, toward, *optional):
                faced.append((actor['name'], toward, optional))
            def unpause(_q, flag):
                unpaused.append(flag)
                cleanup.append('unpause')
            quest = lua.table_from({'GetThingWithScriptName': lambda _q, name: handles[name],
                                    'IsActiveThreadTerminating': lambda *_: terminating,
                                    'PauseAllNonScriptedEntities': unpause,
                                    'EndMovieSequence': lambda *_: cleanup.append('movie_end'),
                                    'EntitySetFacingAngleTowardsThing': face})
            run = lua.execute('return function(quest,me)\n' + body + '\nend')
            self.assertEqual(run(quest, 'man'), expected)
            self.assertEqual(faced, [] if target is None else [(target, 'man', ())])
            self.assertEqual(unpaused, [False])
            self.assertEqual(cleanup, ['unpause', 'movie_end'])

    def test_changed_join_source_is_not_restructured(self):
        fn, source, _ = self.inputs()
        changed = source.replace('puVar11 = &stack0xfffffeb4;', 'puVar11 = &stack0xfffffea8;')
        result, evidence = recover_affair_facing(fn, changed, RData())
        self.assertEqual(result, changed)
        self.assertEqual(evidence[0]['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()
