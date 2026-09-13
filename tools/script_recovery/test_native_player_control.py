import json
import unittest

from lupa.lua54 import LuaRuntime

from tools.script_recovery.lift_native_lua import (
    ROOT, Lifter, RData, annotate, converter_signatures, load_manifest,
    load_slots, load_thing_tables, strip_declarations,
)


class NativePlayerControlTests(unittest.TestCase):
    def test_signature_overlay_preserves_existing_contract_and_input(self):
        original = {}
        corrected = converter_signatures(original)
        self.assertEqual(original, {})
        self.assertEqual(corrected['IsHeroControlledByPlayer']['returnType'], 'bool')
        self.assertEqual(corrected['IsHeroControlledByPlayer']['parameters'], [])
        alternate = {'IsHeroControlledByPlayer': {'scope': 'Entity', 'returnType': 'int'}}
        self.assertEqual(converter_signatures(alternate)['IsHeroControlledByPlayer'], alternate['IsHeroControlledByPlayer'])

    def test_actual_marker_wait_observes_boolean_query_and_cancellation(self):
        unit = json.loads((ROOT / 'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
        fn = next(f for f in unit['functions'] if f['address'] == '0x00DBE4E0')
        manifest, slots = load_manifest(), load_slots()
        things, returning = load_thing_tables(manifest, slots)
        statements = strip_declarations(annotate(fn['decompile'], slots, things, returning, entity=False))
        start = next(i for i, line in enumerate(statements) if 'cVar3 = GSI->IsHeroControlledByPlayer();' in line)
        end = next(i for i in range(start, len(statements)) if statements[i].strip() == '}')
        native = ('{\n' + '\n'.join(statements[start:end + 1]) +
                  '\nreturn true;\nLAB_00dbe7b4:\nreturn false;\n}')
        lifter = Lifter(manifest, {}, 'quest', False, '', RData(), live_termination=True, native_gotos=True)
        body = '\n'.join(lifter.lift('ControlWait', native))
        self.assertEqual(lifter.todo, [])
        for queries, cancel, expected in (((True,), False, True), ((False, False, True), False, True),
                                          ((False,), True, False)):
            lua, events = LuaRuntime(), []
            values = iter(queries)
            def query(_q):
                value = next(values)
                events.append(('control', value))
                return value
            def frame(_q):
                events.append(('frame',))
                return not cancel
            def terminating(_q):
                events.append(('terminating', cancel))
                return cancel
            quest = lua.table_from({'IsHeroControlledByPlayer': query, 'NewScriptFrame': frame,
                                   'IsActiveThreadTerminating': terminating})
            self.assertEqual(lua.execute('return function(quest)\n' + body + '\nend')(quest), expected)
            expected_events = [('control', queries[0])]
            if cancel:
                expected_events += [('frame',), ('terminating', True)]
            else:
                for value in queries[1:]:
                    expected_events += [('frame',), ('terminating', False), ('control', value)]
            self.assertEqual(events, expected_events)
