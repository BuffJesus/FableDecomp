import unittest
from unittest.mock import patch
from types import SimpleNamespace

from lupa.lua54 import LuaRuntime
from tools.script_recovery import lift_native_lua as module
from tools.script_recovery import test_native_quest_vector_fields as fixtures
from tools.script_recovery.native_affair_wife_position import recover_affair_wife_position


class WifePositionTests(unittest.TestCase):
    def build(self):
        fn, source, manifest, sigs = fixtures.QuestVectorTests().inputs('0x00DB2B10', True)
        inputs = []
        def capture(*args):
            inputs.append(args)
            return recover_affair_wife_position(*args)
        lifter = module.Lifter(manifest, module.load_entity_state('NOVI_AffairWife'),
                               'quest', True, '', module.RData(), thing_sigs=sigs)
        with patch.object(module, 'recover_affair_wife_position', capture):
            body = '\n'.join(lifter.lift('Main', source, native_function=fn))
        return body, lifter, inputs[0]

    def test_cached_actor_position_and_five_movement_operands(self):
        body, lifter, _ = self.build()
        self.assertEqual(lifter.affair_wife_position_evidence[0]['status'], 'recovered')
        self.assertIn('me:MoveToPosition(pCVar9, 2.0, 1, false, true)', body)
        self.assertNotIn('(**(*', body)
        lines = body.splitlines()
        start = next(i for i, s in enumerate(lines) if 'r1 = quest:GetThingWithScriptName("NOVI_AffairMan")' in s)
        end = next(i for i, s in enumerate(lines) if 'me:MoveToPosition(' in s)
        for position in ((10.0, -5.0, 2.0), (4.5, 6.25, -8.0)):
            lua, events = LuaRuntime(), []
            pos = lua.table_from(dict(zip('xyz', position)))
            def query(actor):
                self.assertEqual(actor, 'cached_husband')
                events.append('position')
                return pos
            lua.globals().RetailThingPosition = query
            quest = lua.table_from({'GetThingWithScriptName': lambda _q, name: 'cached_husband',
                'EntitySetAsUseMovementInActions': lambda _q, _me, value: events.append(('movement', value))})
            def move(_me, actual, *args):
                events.append(('move', tuple(actual[a] for a in 'xyz'), *args))
            me = lua.table_from({'MoveToPosition': move})
            lua.execute('return function(quest,me)\n' + '\n'.join(lines[start:end + 1]) + '\nend')(quest, me)
            self.assertEqual(events, [('movement', True), 'position', ('move', position, 2.0, 1, False, True)])

    def test_changed_source_or_native_query_rejects(self):
        _, _, (fn, source, data) = self.build()
        for candidate, reader in ((source + ' ', data), (source, SimpleNamespace(bytes_at=lambda *_: None))):
            result, evidence = recover_affair_wife_position(fn, candidate, reader)
            self.assertEqual(result, candidate)
            self.assertEqual(evidence[0]['status'], 'rejected')

    def test_query_rejects_unknown_actors(self):
        lifter = module.Lifter(module.load_manifest(), {}, 'quest', True, '', module.RData())
        with self.assertRaisesRegex(ValueError, 'Thing provenance'):
            lifter.interface_call('pCVar9', 'RetailThingPosition', 'unknown')
