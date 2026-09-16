import itertools
import unittest
from types import SimpleNamespace

from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_bully_messages as fixtures
from tools.script_recovery.native_bully_messages import recover_bully_messages
from tools.script_recovery.native_bully_teddy_offer import recover_bully_teddy_offer
from tools.script_recovery.lift_native_lua import RData, Lifter, converter_signatures


class BullyTeddyOfferTests(unittest.TestCase):
    def inputs(self):
        fn, source, manifest = fixtures.BullyMessageTests().inputs()
        source, _ = recover_bully_messages(fn, source, RData(), manifest)
        return fn, source, manifest

    def test_inventory_binding_overlay_preserves_newer_contracts(self):
        manifest = {}
        updated = converter_signatures(manifest)
        self.assertEqual(manifest, {})
        self.assertEqual(updated['IsObjectInThingsPossession']['returnType'], 'bool')
        newer = {'IsObjectInThingsPossession': {'reviewed': 'newer'}}
        self.assertEqual(converter_signatures(newer)['IsObjectInThingsPossession'], newer['IsObjectInThingsPossession'])

    def test_talk_and_possession_predicate_short_circuits_inventory_query(self):
        fn, source, manifest = self.inputs()
        result, evidence = recover_bully_teddy_offer(fn, source, RData(), manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertNotIn('LAB_00dbb684', result)
        lifter = Lifter(manifest, {}, 'quest', True, '', RData())
        body = '\n'.join(lifter.lift('OfferPredicate', '{\n' + evidence[0]['edits'][0]['new'] +
            '\nreturn native_arg_bully_talked_with_teddy;\n}', parameters={'cVar4': 'bool', 'uVar12': 'number'}))
        self.assertEqual(lifter.todo, [])
        lua = LuaRuntime()
        run = lua.execute('return function(quest,cVar4,uVar12)\n' + body + '\nend')
        for talked, has_teddy in itertools.product((False, True), repeat=2):
            events = []
            quest = lua.table_from({
                'GetHero': lambda _: events.append('hero') or 'hero_actor',
                'IsObjectInThingsPossession': lambda _, item, actor: events.append(('inventory', item, actor)) or has_teddy})
            self.assertEqual(run(quest, talked, 0), talked and has_teddy)
            self.assertEqual(events, ['hero', ('inventory', 'OBJECT_TEDDY_BEAR_UNGIVEABLE', 'hero_actor')] if talked else [])

    def test_changed_native_branch_rejects_predicate(self):
        fn, source, manifest = self.inputs()
        data = RData()
        def changed(address, size):
            raw = data.bytes_at(address, size)
            if address == 0xDBB613:
                raw = bytearray(raw)
                raw[0xDBB682 - address] = 0x74
                return bytes(raw)
            return raw
        result, evidence = recover_bully_teddy_offer(fn, source,
            SimpleNamespace(bytes_at=changed, string_at=data.string_at), manifest)
        self.assertEqual(result, source)
        self.assertEqual(evidence[0]['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()
