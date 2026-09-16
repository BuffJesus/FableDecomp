import copy
import re
import unittest
from dataclasses import replace
from unittest.mock import patch

from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_book_trader_home as fixtures
from tools.script_recovery.native_book_trader_home import recover_book_trader_home
from tools.script_recovery.native_book_trader_health_values import recover_book_trader_health_values
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.lift_native_lua import Lifter


class BookTraderHealthValuesTests(unittest.TestCase):
    def inputs(self):
        fn, source, data, manifest = fixtures.BookTraderHomeTests().inputs()
        source, evidence = recover_book_trader_home(fn, source, data, manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        return fn, source, data, manifest

    def test_all_seven_native_gates_accept_only_ordered_positive_health(self):
        fn, source, data, manifest = self.inputs()
        recovered, evidence = recover_book_trader_health_values(fn, source, data, manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertIn('unresolved', evidence[0]['resourceLifetime'])
        self.assertNotIn('_DAT_0122dedc', recovered)
        self.assertNotIn('_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__', recovered)
        destructors = re.findall(r'C3DClothPrimitive::~C3DClothPrimitive[^;]+;', source)
        self.assertEqual(re.findall(r'C3DClothPrimitive::~C3DClothPrimitive[^;]+;', recovered), destructors)
        slices = re.findall(r'native_arg_book_health_actor = me;.*?if \(native_arg_book_has_health\)',
                            recovered, re.S)
        self.assertEqual(len(slices), 7)
        for index, snippet in enumerate(slices):
            body = '{\n' + snippet + ' {\nreturn 1;\n}\nreturn 0;\n}'
            lifter = Lifter(manifest, {}, 'quest', True, '', data)
            emitted = '\n'.join(lifter.lift('HealthGate', body))
            self.assertEqual(lifter.todo, [])
            lua = LuaRuntime()
            run = lua.execute('return function(quest,me)\n' + emitted + '\nend')
            for health in (-float('inf'), -1.0, -0.0, 0.0, 0.00001, 1.0, float('inf'), float('nan')):
                with self.subTest(index=index, health=health):
                    actors = []
                    def query(_quest, actor):
                        actors.append(actor)
                        return health
                    quest = lua.table_from({'GetHealth': query})
                    self.assertEqual(run(quest, 'book-trader'), int(health > 0.0))
                    self.assertEqual(actors, ['book-trader'])

    def test_changed_literal_comparison_or_native_callee_rejects(self):
        fn, source, data, manifest = self.inputs()
        _, evidence = recover_book_trader_health_values(fn, source, data, manifest)
        original = data.bytes_at
        sites = [0x122DEDC, 0x7E7490, 0x1260F0C + 0x420]
        for comparison in evidence[0]['comparisonSites']:
            site = int(comparison, 16)
            sites += [site, site + 2, site + 12]
        for site in sites:
            def changed(address, size):
                raw = original(address, size)
                if raw is not None and address <= site < address + size:
                    raw = bytearray(raw)
                    raw[site - address] ^= 1
                    return bytes(raw)
                return raw
            with self.subTest(site=hex(site)), patch.object(data, 'bytes_at', side_effect=changed):
                result, status = recover_book_trader_health_values(fn, source, data, manifest)
                self.assertEqual(result, source)
                self.assertEqual(status[0]['status'], 'rejected')

    def test_changed_acquisition_getter_or_health_actor_rejects(self):
        fn, source, data, manifest = self.inputs()
        _, evidence = recover_book_trader_health_values(fn, source, data, manifest)
        changes = [(site, {'ecx': ('register', 'eax')}) for site, _ in evidence[0]['acquisitions']]
        for getter, health, _ in evidence[0]['queries']:
            changes += [(getter, {'ecx': ('stack', 24)}),
                        (getter, {'stack_arguments': (('stack', 999),)}),
                        (health, {'stack_arguments': (('register', 'ebp'),)}),
                        (health, {'target': ('constant', 0)})]
        for site, change in changes:
            def decode(*args, **kwargs):
                setup = read_call_window(*args, **kwargs)
                return replace(setup, **change) if args[3] == site else setup
            with self.subTest(site=hex(site), change=change), patch(
                    'tools.script_recovery.native_book_trader_health_values.read_call_window', side_effect=decode):
                result, status = recover_book_trader_health_values(fn, source, data, manifest)
                self.assertEqual(result, source)
                self.assertEqual(status[0]['status'], 'rejected')

    def test_source_and_contract_guards(self):
        fn, source, data, manifest = self.inputs()
        for function, text in ((dict(fn, decompile=fn['decompile'] + '\n'), source),
                               (fn, source.replace('if (fVar3 < fVar18)', 'if (fVar3 <= fVar18)', 1))):
            result, status = recover_book_trader_health_values(function, text, data, manifest)
            self.assertEqual(result, text)
            self.assertEqual(status[0]['status'], 'rejected')
        altered = copy.deepcopy(manifest)
        altered['GetHealth']['returnType'] = 'int'
        self.assertEqual(recover_book_trader_health_values(fn, source, data, altered)[1][0]['status'], 'rejected')
        self.assertEqual(recover_book_trader_health_values(dict(fn, address='0x0'), source, data, manifest), (source, []))


if __name__ == '__main__':
    unittest.main()
