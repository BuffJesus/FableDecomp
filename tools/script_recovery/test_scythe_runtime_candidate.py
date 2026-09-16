import unittest
from unittest.mock import patch
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.scythe_runtime_candidate import build, wire


class ScytheRuntimeCandidateTests(unittest.TestCase):
    def test_actual_candidate_bracket_retains_caller_movie_and_completion_order(self):
        report = build()
        self.assertFalse(report['registrationEnabled'])
        self.assertEqual(report['runtimeProposal']['status'], 'pending integration')
        source = (ROOT / 'work/scythe_converter/runtime_proposal/draft/Entities/ScytheNearOracle.lua').read_text()
        start = source.index('quest:StartMovieSequence()')
        end = source.index('quest:EndMovieSequence()', start) + len('quest:EndMovieSequence()')
        block = source[start:end]
        self.assertNotIn('TODO(native)', block)
        lua, events = LuaRuntime(), []
        me = lua.table()
        def adapter(_quest, actor):
            self.assertTrue(lua.eval('function(a,b) return a==b end')(actor, me))
            self.assertEqual(events, [('start',), ('pause', True), ('marker',)])
            events.append(('adapter',))
        callbacks = {'RunScytheOracleCutscene': adapter}
        for method, event in [('StartMovieSequence','start'), ('EndMovieSequence','end'),
                              ('PauseAllNonScriptedEntities','pause'), ('SetStateBool','state')]:
            callbacks[method] = lambda _q, *args, event=event: events.append((event, *args))
        callbacks['MiniMapRemoveMarker'] = lambda _q, actor: events.append(('marker',))
        callbacks['RemoveThing'] = lambda _q, actor, a, b: events.append(('remove', a, b))
        lua.execute('return function(quest,me)\n' + block + '\nend')(lua.table_from(callbacks), me)
        self.assertEqual(events, [('start',), ('pause', True), ('marker',), ('adapter',),
                                 ('state','MissionSucceeded',True), ('remove',False,True), ('pause',False), ('end',)])

    def test_changed_source_or_native_audit_rejects(self):
        with self.assertRaisesRegex(ValueError, 'correspondence changed'):
            wire('changed Lua')
        with patch('tools.script_recovery.scythe_runtime_candidate.audit', return_value={'status':'rejected'}):
            with self.assertRaisesRegex(ValueError, 'audit rejected'):
                wire('anything')


if __name__ == '__main__':
    unittest.main()
