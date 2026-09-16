import json
import unittest
from types import SimpleNamespace

from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import (ROOT, RData, Lifter, annotate, load_manifest,
    load_slots, load_thing_tables, thing_signatures, fold_self_wrapper_arguments)
from tools.script_recovery.native_function_parameters import function_parameters, rename_parameters
from tools.script_recovery.native_bully_messages import recover_bully_messages


class BullyMessageTests(unittest.TestCase):
    def inputs(self):
        unit = json.loads((ROOT / 'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
        fn = next(f for f in unit['functions'] if f['address'] == '0x00DBB310')
        manifest, slots = load_manifest(), load_slots()
        things, returning = load_thing_tables(manifest, slots)
        source = fold_self_wrapper_arguments(annotate(rename_parameters(fn['decompile'],
            function_parameters(fn['decompile'], member=True)), slots, things, returning, entity=True))[0]
        return fn, source, manifest

    def test_five_queries_use_self_and_the_excluded_ability(self):
        fn, source, manifest = self.inputs()
        result, evidence = recover_bully_messages(fn, source, RData(), manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        for slot in ['0x6c))', '0x54))', '0xa8))', '0xa4))']:
            self.assertNotIn(slot, result)
        things, _ = load_thing_tables(manifest, load_slots())
        for edit, expected, wrapper in zip(evidence[0]['edits'],
                ['talk', 'talk', 'hit', 'any', ('excluded', 14)],
                ['aCStack_a0', 'aCStack_c8', 'aCStack_a4', 'aCStack_ac', 'aaStack_c0']):
            native = '{\nCCharString::CCharString(' + wrapper + ',"SCRIPT_NAME_HERO",-1);\n'
            native += 'cVar4 = ' + edit['new'] + ';\nreturn cVar4;\n}'
            lifter = Lifter(manifest, {}, 'quest', True, '', RData(), thing_sigs=thing_signatures(things))
            body = '\n'.join(lifter.lift('Query', native))
            self.assertEqual(lifter.todo, [])
            lua, events = LuaRuntime(), []
            me = lua.table_from({
                'IsTalkedToByHero': lambda _: events.append('talk') or True,
                'MsgIsHitByHero': lambda _: events.append('hit') or True,
                'MsgIsHitByAnySpecialAbilityFromHero': lambda _: events.append('any') or True,
                'MsgIsHitByHeroSpecialAbility': lambda _, ability: events.append(('excluded', ability)) or True})
            self.assertTrue(lua.execute('return function(me)\n' + body + '\nend')(me))
            self.assertEqual(events, [expected])

    def test_changed_actor_bytes_or_source_are_rejected(self):
        fn, source, manifest = self.inputs()
        data = RData()
        def changed(address, size):
            raw = data.bytes_at(address, size)
            if address == 0xDBB310:
                raw = bytearray(raw)
                raw[0xDBB31E - address] = 12
                return bytes(raw)
            return raw
        result, evidence = recover_bully_messages(fn, source,
            SimpleNamespace(bytes_at=changed, string_at=data.string_at), manifest)
        self.assertEqual(result, source)
        self.assertEqual(evidence[0]['status'], 'rejected')
        changed_source = source + '\n'
        result, evidence = recover_bully_messages(fn, changed_source, data, manifest)
        self.assertEqual(result, changed_source)
        self.assertEqual(evidence[0]['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()
