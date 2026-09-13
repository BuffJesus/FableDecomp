import json
import unittest
from types import SimpleNamespace

from lupa.lua54 import LuaRuntime

from tools.script_recovery.native_quest_markers import recover_quest_markers
from tools.script_recovery.lift_native_lua import (
    ROOT, Lifter, RData, annotate, load_manifest, load_slots, load_thing_tables, strip_declarations,
)


class QuestMarkerTests(unittest.TestCase):
    def inputs(self):
        unit = json.loads((ROOT / 'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
        fn = next(f for f in unit['functions'] if f['address'] == '0x00DBE4E0')
        manifest, slots = load_manifest(), load_slots()
        things, returning = load_thing_tables(manifest, slots)
        return fn, annotate(fn['decompile'], slots, things, returning, entity=False), manifest

    def test_all_marker_calls_use_the_original_cached_actor_and_texture(self):
        fn, source, manifest = self.inputs()
        recovered, evidence = recover_quest_markers(fn, source, RData())
        self.assertEqual(len(evidence), 10)
        self.assertTrue(all(e['status'] == 'recovered' for e in evidence))
        # Execute the actual recovered call statements in source order. Waits
        # and cleanup remain outside this operand-focused test's claim.
        calls = [line for line in strip_declarations(recovered)
                 if any('GSI->' + name + '(' in line for name in
                        ('GetThingWithScriptName', 'MiniMapAddMarker', 'MiniMapRemoveMarker'))]
        lifter = Lifter(manifest, {}, 'quest', False, '', RData())
        body = '\n'.join(lifter.lift('Markers', '{\n' + '\n'.join(calls) + '\n}'))
        self.assertEqual(lifter.todo, [])
        lua, events = LuaRuntime(), []
        def lookup(_q, name):
            events.append(('lookup', name))
            return name
        quest = lua.table_from({'GetThingWithScriptName': lookup,
            'MiniMapAddMarker': lambda _q, actor, texture: events.append(('add', actor, texture)),
            'MiniMapRemoveMarker': lambda _q, actor: events.append(('remove', actor))})
        lua.execute('return function(quest)\n' + body + '\nend')(quest)
        father, trader, theresa, texture = 'NOVI_LiveFather', 'NOVI_BookTrader', 'NOVI_Theresa', 'HUD_ORB_QUEST_CORE'
        self.assertEqual(events, [('lookup', father), ('lookup', trader), ('lookup', theresa),
                                 ('add', father, texture), ('remove', father), ('add', trader, texture),
                                 ('remove', trader), ('add', theresa, texture), ('remove', theresa),
                                 ('add', father, texture)])

    def test_changed_native_body_strings_and_source_reject(self):
        fn, source, _ = self.inputs()
        data = RData()
        for changed in (SimpleNamespace(bytes_at=lambda *_: None, string_at=data.string_at),
                        SimpleNamespace(bytes_at=data.bytes_at, string_at=lambda *_: 'OTHER')):
            result, evidence = recover_quest_markers(fn, source, changed)
            self.assertEqual(result, source)
            self.assertEqual(evidence[0]['status'], 'rejected')
        self.assertEqual(recover_quest_markers(fn, source.replace('MiniMapRemoveMarker', 'MiniMapAddMarker'), data)[1][0]['status'], 'rejected')
