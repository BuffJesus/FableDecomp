import copy
import unittest
from types import SimpleNamespace

from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_quest_vector_fields as fixtures
from tools.script_recovery.lift_native_lua import Lifter, RData, fold_self_wrapper_arguments, strip_declarations
from tools.script_recovery.native_affair_woman_position import recover_affair_woman_position


class WomanPositionTests(unittest.TestCase):
    def inputs(self):
        fn, source, manifest, _ = fixtures.QuestVectorTests().inputs('0x00DB1F00', True)
        return fn, fold_self_wrapper_arguments(source)[0], manifest

    def test_native_marker_and_actor_setup_then_movement_uses_position_snapshot(self):
        fn, source, manifest = self.inputs()
        recovered, evidence = recover_affair_woman_position(fn, source, RData(), manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        statements = strip_declarations(recovered)
        start = next(i for i, s in enumerate(statements) if '"AffairWomanRunOffPoint"' in s)
        end = next(i for i, s in enumerate(statements) if 'native_arg_runoff_position = ' in s)
        move = next(s for s in statements if '_MoveToPosition_' in s and 'native_arg_runoff_position' in s)
        selected = statements[start:end + 1] + [move]
        lifter = Lifter(manifest, {}, 'quest', True, '', RData())
        body = '\n'.join(lifter.lift('RunOffSetup', '{\n' + '\n'.join(selected) + '\n}'))
        self.assertIn('me:MoveToPosition(native_arg_runoff_position, 0.0, 1, false, true)', body)
        for marker in ('marker', None):
            lua, events = LuaRuntime(), []
            def query(actor):
                self.assertEqual(actor, marker)
                events.append('position')
                return lua.table_from({'x': 2, 'y': -3, 'z': 4})
            lua.globals().RetailThingPosition = query
            quest = lua.table_from({'GetThingWithScriptName': lambda _q, name: marker,
                'EntitySetAsUseMovementInActions': lambda _q, actor, value: events.append(('movement', actor, value)),
                'SetIsPushableByHero': lambda _q, actor, value: events.append(('pushable', actor, value))})
            lua.execute('woman = { MoveToPosition = function(self, pos, radius, mode, avoid, ignore) recordMove(pos.x,pos.y,pos.z,radius,mode,avoid,ignore) end }')
            me = lua.globals().woman
            lua.globals().recordMove = lambda *args: events.append(('move', *args))
            lua.execute('return function(quest,me)\n' + body + '\nend')(quest, me)
            # Compare actor identity within Lua; Python table wrappers are not identity-stable.
            for event in events[:2]:
                self.assertEqual(lua.eval('function(a,b) return a==b end')(event[1], me), True)
            self.assertEqual([(e[0], e[-1]) for e in events[:2]], [('movement', True), ('pushable', True)])
            self.assertEqual(events[2:], ['position', ('move', 2, -3, 4, 0.0, 1, False, True)])

    def test_changed_native_source_string_or_binding_rejects(self):
        fn, source, manifest = self.inputs()
        data = RData()
        missing = SimpleNamespace(bytes_at=lambda *_: None)
        wrong_name = SimpleNamespace(bytes_at=data.bytes_at, string_at=lambda *_: 'other')
        changed = copy.deepcopy(manifest)
        changed['SetIsPushableByHero']['parameters'][1]['type'] = 'int'
        for text, reader, api in ((source + ' ', data, manifest), (source, missing, manifest),
                                  (source, wrong_name, manifest), (source, data, changed)):
            output, evidence = recover_affair_woman_position(fn, text, reader, api)
            self.assertEqual(output, text)
            self.assertEqual(evidence[0]['status'], 'rejected')
