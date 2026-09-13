import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_lift_native_lua as fixtures


class AcquireBooleanTests(unittest.TestCase):
    def test_false_result_retries_and_preserves_loop_effects(self):
        lifter = fixtures.make(entity=True)
        source = '''{
cVar4 = GSI->StartScriptingEntity();
while (cVar4 == '\\0') {
GSI->NewScriptFrame();
GSI->SetTimer(17,9);
cVar4 = GSI->StartScriptingEntity();
}
return cVar4;
}'''
        body = '\n'.join(lifter.lift('Main', source))
        self.assertIn('while not cVar4 do', body)
        lua, events = LuaRuntime(), []
        results = iter((False, True))
        def acquire(_me, priority):
            events.append(('acquire', priority))
            return next(results)
        me = lua.table_from({'AcquireControl': acquire})
        quest = lua.table_from({'NewScriptFrame': lambda *_: events.append(('frame',)) or True,
                               'SetTimer': lambda _q, timer, value: events.append(('timer', timer, value))})
        self.assertTrue(lua.execute('return function(quest,me)\n' + body + '\nend')(quest, me))
        self.assertEqual(events, [('acquire', 4), ('frame',), ('timer', 17, 9), ('acquire', 4)])

    def test_retry_result_used_after_loop_is_not_removed(self):
        lifter = fixtures.make(entity=True)
        body = '\n'.join(lifter.lift('Main', '''{
cVar4 = GSI->StartScriptingEntity();
while (cVar4 == '\\0') {
GSI->NewScriptFrame();
cVar4 = GSI->StartScriptingEntity();
}
return cVar4;
}'''))
        self.assertIn('while not cVar4 do', body)
        self.assertIn('return cVar4', body)


if __name__ == '__main__':
    unittest.main()
