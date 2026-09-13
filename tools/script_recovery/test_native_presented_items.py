import json
import unittest
from types import SimpleNamespace

from lupa.lua54 import LuaRuntime

from tools.script_recovery.native_presented_items import recover_presented_item_comparison
from tools.script_recovery.native_self_wrapper import fold_self_wrapper_arguments
from tools.script_recovery.lift_native_lua import (
    ROOT, Lifter, RData, annotate, load_manifest, load_slots, load_thing_tables,
    strip_declarations, thing_signatures,
)


class PresentedItemComparisonTests(unittest.TestCase):
    def inputs(self, address):
        unit = json.loads((ROOT / 'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
        fn = next(f for f in unit['functions'] if f['address'] == address)
        manifest, slots = load_manifest(), load_slots()
        things, returning = load_thing_tables(manifest, slots)
        source = fold_self_wrapper_arguments(annotate(fn['decompile'], slots, things, returning, entity=True))[0]
        return fn, source, manifest, things

    def test_actual_conditions_compare_returned_name_and_preserve_short_circuit(self):
        for address, key, is_theresa in (
                ('0x00DB97A0', 'OBJECT_CHOCOLATE_BOX_UNGIVEABLE', True),
                ('0x00DAF080', 'OBJECT_TEDDY_BEAR_UNGIVEABLE', False)):
            fn, source, manifest, things = self.inputs(address)
            recovered, evidence = recover_presented_item_comparison(fn, source, RData())
            self.assertEqual(evidence[0]['status'], 'recovered')
            header = next(line for line in strip_declarations(recovered) if 'CCharString::NotEqual(' in line)
            lifter = Lifter(manifest, {}, 'quest', True, '', RData(), thing_sigs=thing_signatures(things))
            body = '\n'.join(lifter.lift('Main', '{\n'
                'cVar5 = CScriptThing::MsgIsPresentedWithItem((CScriptThing *)(this + 8));\n' + header +
                '\nreturn true,cVar5;\n}\nreturn false,cVar5;\n}'))
            self.assertEqual(lifter.todo, [])
            for presented in (False, True):
                for item in (key, 'OTHER_ITEM', key.lower(), '', None):
                    with self.subTest(address=address, presented=presented, item=item):
                        lua, calls = LuaRuntime(), []
                        lua.globals().g_PresentedItemName = 'STALE_ITEM'
                        def query(_me):
                            calls.append('presentation')
                            if presented:
                                lua.globals().g_PresentedItemName = item
                            return presented
                        me = lua.table_from({'MsgIsPresentedWithItem': query})
                        branch, saved = lua.execute('return function(quest,me)\n' + body + '\nend')(None, me)
                        unequal = presented and item != key
                        self.assertEqual(branch, not unequal if is_theresa else unequal)
                        self.assertEqual(saved, unequal)
                        self.assertEqual(calls, ['presentation'])

    def test_changed_source_native_bytes_and_literals_reject_recovery(self):
        fn, source, _, _ = self.inputs('0x00DB97A0')
        data = RData()
        for candidate in (source + '\nchanged();', source.replace('CCharString__NotEqual()', 'other()')):
            unchanged, evidence = recover_presented_item_comparison(fn, candidate, data)
            self.assertEqual(unchanged, candidate)
            self.assertEqual(evidence[0]['status'], 'rejected')
        for changed_data in (
                SimpleNamespace(bytes_at=lambda address, size: b'\x90' * size, string_at=data.string_at),
                SimpleNamespace(bytes_at=data.bytes_at, string_at=lambda address: 'WRONG_ITEM')):
            unchanged, evidence = recover_presented_item_comparison(fn, source, changed_data)
            self.assertEqual(unchanged, source)
            self.assertEqual(evidence[0]['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()
