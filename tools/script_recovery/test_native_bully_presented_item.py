import unittest
from types import SimpleNamespace

from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_bully_teddy_offer as fixtures
from tools.script_recovery.native_bully_teddy_offer import recover_bully_teddy_offer
from tools.script_recovery.native_bully_presented_item import recover_bully_presented_item
from tools.script_recovery.lift_native_lua import RData, Lifter, load_slots, load_thing_tables, thing_signatures, strip_declarations


class BullyPresentedItemTests(unittest.TestCase):
    def inputs(self):
        fn, source, manifest = fixtures.BullyTeddyOfferTests().inputs()
        source, _ = recover_bully_teddy_offer(fn, source, RData(), manifest)
        return fn, source, manifest

    def test_teddy_other_empty_and_failed_queries_preserve_dispatch(self):
        fn, source, manifest = self.inputs()
        result, evidence = recover_bully_presented_item(fn, source, RData(), manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertNotIn('LAB_00dbbb73', result)
        self.assertNotIn('bVar3 = *pcVar21 == *pcVar17', result)
        statements = [s.strip() for s in strip_declarations(result)]
        queries = [s for s in statements if 'CScriptThing::MsgIsPresentedWithItem(' in s]
        self.assertEqual(len(queries), 2)
        positive = next(s for s in statements if s.startswith('if (g_PresentedItemName =='))
        negative = next(s for s in statements if s.startswith("if ((cVar4 == '\\0') || (g_PresentedItemName =="))
        native = ('{\n' + queries[0] + "\nif (cVar4 != '\\0') {\n" + positive +
                  '\nreturn 1;\n}\n}\n' + queries[1] + '\n' + negative +
                  '\nreturn 0;\n}\nreturn 2;\n}')
        things, _ = load_thing_tables(manifest, load_slots())
        lifter = Lifter(manifest, {}, 'quest', True, '', RData(), thing_sigs=thing_signatures(things))
        body = '\n'.join(lifter.lift('PresentedDispatch', native))
        self.assertEqual(lifter.todo, [])
        lua = LuaRuntime()
        run = lua.execute('return function(me)\n' + body + '\nend')
        teddy = 'OBJECT_TEDDY_BEAR_UNGIVEABLE'
        cases = [(teddy, None, 1), ('OTHER', 'OTHER', 2), ('', '', 2),
                 (None, None, 0), ('OTHER', None, 0), (None, teddy, 0), (None, 'OTHER', 2)]
        for first, second, expected in cases:
            events = []
            # A failed query leaves the shared host global unchanged; the result
            # must gate every read so this stale teddy cannot cause acceptance.
            lua.globals().g_PresentedItemName = teddy
            values = iter([first, second])
            def presented(_):
                value = next(values)
                events.append(value)
                if value is not None:
                    lua.globals().g_PresentedItemName = value
                return value is not None
            self.assertEqual(run(lua.table_from({'MsgIsPresentedWithItem': presented})), expected)
            self.assertEqual(events, [first] if expected == 1 else [first, second])

    def test_changed_empty_string_accessor_or_item_name_rejects_recovery(self):
        fn, source, manifest = self.inputs()
        data = RData()
        def changed(address, size):
            raw = data.bytes_at(address, size)
            return b'X' * size if address == 0x129AAF4 else raw
        for reader in [SimpleNamespace(bytes_at=changed, string_at=data.string_at),
                       SimpleNamespace(bytes_at=data.bytes_at, string_at=lambda _: 'OTHER')]:
            result, evidence = recover_bully_presented_item(fn, source, reader, manifest)
            self.assertEqual(result, source)
            self.assertEqual(evidence[0]['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()
