import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import Lifter, load_manifest


class FrameArgumentTests(unittest.TestCase):
    def test_frame_supplies_execution_actor_without_consuming_another_thing(self):
        manifest = load_manifest()
        lifter = Lifter(manifest, {}, 'quest', True, '', None)
        # A staged Thing belongs to the following native call, not to the
        # bridge-only execution entity argument of NewScriptFrame.
        lifter.kinds['other_actor'] = 'thing'
        lifter.push_temp('other_actor', 'other_actor')
        lifter.interface_call(None, 'NewScriptFrame', '')
        self.assertEqual(lifter.todo, [])
        self.assertEqual(lifter.temps['other_actor'], 'other_actor')
        lifter.interface_call('health', 'GetHealth', 'other_actor')
        self.assertEqual(lifter.todo, [])
        lua, events = LuaRuntime(), []
        quest = lua.table_from({'NewScriptFrame': lambda _, actor: events.append(('frame', actor)) or True,
                               'GetHealth': lambda _, actor: events.append(('health', actor)) or 7})
        lua.execute('return function(quest,me,other_actor)\nlocal alive\n' +
                    '\n'.join(lifter.out) + '\nend')(quest, 'thread_actor', 'query_actor')
        self.assertEqual(events, [('frame', 'thread_actor'), ('health', 'query_actor')])

    def test_quest_frame_has_no_invented_entity(self):
        lifter = Lifter(load_manifest(), {}, 'quest', False, '', None, execution_entity=False)
        body = '\n'.join(lifter.lift('Frame', '{\nGSI->NewScriptFrame();\n}'))
        self.assertEqual(lifter.todo, [])
        self.assertIn('quest:NewScriptFrame()', body)
        self.assertNotIn('NewScriptFrame(me)', body)


if __name__ == '__main__':
    unittest.main()
