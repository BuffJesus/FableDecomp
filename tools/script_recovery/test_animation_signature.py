import copy
import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import Lifter, converter_signatures


class AnimationSignatureTests(unittest.TestCase):
    def manifest(self):
        return {'PlayAnimation': {'scope': 'Entity', 'returnType': 'void', 'blocking': True,
            'parameters': [{'name': 'pMe', 'type': 'CScriptThing*'},
                           {'name': 'animName', 'type': 'const std::string&'}] + [
                {'name': name, 'type': 'sol::optional<bool>', 'optional': True}
                for name in ('waitForFinish', 'stayOnLastFrame', 'allowLooking')]}}

    def test_overlay_is_narrow_idempotent_and_immutable(self):
        manifest = self.manifest()
        original = copy.deepcopy(manifest)
        updated = converter_signatures(manifest)
        self.assertEqual(manifest, original)
        self.assertFalse(updated['PlayAnimation']['blocking'])
        self.assertEqual(len(updated['PlayAnimation']['parameters']), 9)
        self.assertEqual(converter_signatures(updated), updated)
        manifest['PlayAnimation']['parameters'][2]['name'] = 'customFlag'
        self.assertEqual(converter_signatures(manifest)['PlayAnimation'], manifest['PlayAnimation'])

    def test_all_seven_flags_survive_emission_in_order(self):
        lua = LuaRuntime()
        for mask in range(128):
            flags = [bool(mask & (1 << i)) for i in range(7)]
            args = ','.join('true' if x else 'false' for x in flags)
            lifter = Lifter(converter_signatures(self.manifest()), {}, 'quest', True, '', None)
            body = '\n'.join(lifter.lift('Animation', '{\nGSI->PlayAnimation("TEST_ANIMATION",' + args + ');\n}'))
            self.assertEqual(lifter.todo, [])
            events = []
            me = lua.table_from({'PlayAnimation': lambda actor, key, *values: events.append((key, values))})
            lua.execute('return function(quest,me)\n' + body + '\nend')(lua.table(), me)
            self.assertEqual(events, [('TEST_ANIMATION', tuple(flags))])
