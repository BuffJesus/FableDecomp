import json
import unittest

from lupa.lua54 import LuaRuntime

from tools.script_recovery.lift_native_lua import (
    ROOT, Lifter, RData, annotate, load_manifest, load_slots, load_thing_tables,
    strip_declarations, thing_signatures,
)


class NullableAliveTests(unittest.TestCase):
    def test_actual_post_attack_lookup_accepts_missing_dead_and_live_results(self):
        unit = json.loads((ROOT / 'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
        fn = next(f for f in unit['functions'] if f['address'] == '0x00DBEB20')
        manifest, slots = load_manifest(), load_slots()
        things, returning = load_thing_tables(manifest, slots)
        statements = strip_declarations(annotate(fn['decompile'], slots, things, returning, entity=False))
        constructor = next(s for s in statements if 'CCharString::CCharString' in s and 'M_PostAttackStart' in s)
        lookup = next(s for s in statements if 'piVar5 = ' in s and 'GSI->GetThingWithScriptName' in s)
        predicate = next(s for s in statements if 'CScriptThing::IsAlive(piVar5)' in s)
        native = '{\n' + '\n'.join((constructor, lookup, predicate)) + '\nreturn cVar2;\n}'
        lifter = Lifter(manifest, {}, 'quest', False, '', RData(), thing_sigs=thing_signatures(things))
        body = '\n'.join(lifter.lift('Lookup', native))
        self.assertEqual(lifter.todo, [])
        for present, alive in ((False, False), (True, False), (True, True)):
            lua, events = LuaRuntime(), []
            def predicate(_thing):
                events.append(('alive',))
                return alive
            thing = lua.table_from({'IsAlive': predicate}) if present else None
            def lookup(_q, name):
                events.append(('lookup', name))
                return thing
            quest = lua.table_from({'GetThingWithScriptName': lookup})
            result = lua.execute('return function(quest)\n' + body + '\nend')(quest)
            self.assertIs(result, present and alive)
            self.assertEqual(events, [('lookup', 'M_PostAttackStart')] + ([('alive',)] if present else []))
