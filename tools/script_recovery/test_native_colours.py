import unittest
from types import SimpleNamespace

from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_barrel_distance as fixtures
from tools.script_recovery.native_teddy_distance import recover_barrel_distance
from tools.script_recovery.native_colours import colour_literal, recover_barrel_colours
from tools.script_recovery.lift_native_lua import ROOT, Lifter, RData


class NativeColourTests(unittest.TestCase):
    def inputs(self):
        fn, source, manifest = fixtures.BarrelDistanceTests().inputs()
        return fn, recover_barrel_distance(fn, source, RData())[0], manifest

    def test_packed_literal_preserves_all_four_distinct_channels(self):
        lua = LuaRuntime()
        colour = lua.execute('return ' + colour_literal('__native_colour_bgra32(0x12345678)'))
        self.assertEqual(dict(colour), {'r': 0x34, 'g': 0x56, 'b': 0x78, 'a': 0x12})
        for expression in ('0xff00ff00', '__native_colour_bgra32(value)',
                           '__native_colour_bgra32(0x100000000)', '__native_colour_bgra32(-1)',
                           '"__native_colour_bgra32(0xff00ff00)"'):
            self.assertIsNone(colour_literal(expression))

    def test_actual_branches_call_once_and_match_working_port_colours(self):
        fn, source, manifest = self.inputs()
        recovered, evidence = recover_barrel_colours(fn, source, RData(), manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        start = recovered.index('if (bVar2)', recovered.index('IsDistanceBetweenThingsUnder'))
        end = recovered.index('iVar3 = **', start)
        native = '{\n' + recovered[start:end] + '\n}'
        lifter = Lifter(manifest, {'0x60': ('GUIBarrelCounter', 'Int')}, 'quest', False, '', RData())
        body = '\n'.join(lifter.lift('Colours', native, parameters={'bVar2': 'bool'}))
        self.assertEqual(lifter.todo, [])
        lua = LuaRuntime()
        common = lua.execute((ROOT / 'refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/common.lua').read_text())
        for near, key in ((True, 'COLOUR_GREEN'), (False, 'COLOUR_RED')):
            events = []
            def state(_q, name):
                self.assertEqual(name, 'GUIBarrelCounter')
                return 71
            def change(_q, bar, first, second):
                events.append((bar, dict(first), dict(second)))
            quest = lua.table_from({'GetStateInt': state, 'ChangeQuestInfoBarColour': change})
            lua.execute('return function(quest,bVar2)\n' + body + '\nend')(quest, near)
            expected = dict(common[key])
            self.assertEqual(events, [(71, expected, expected)])

    def test_changed_source_bytes_and_contract_reject(self):
        fn, source, manifest = self.inputs()
        self.assertEqual(recover_barrel_colours(fn, source.replace('0xff00ff00', '0xff0000ff'), RData(), manifest)[1][0]['status'], 'rejected')
        self.assertEqual(recover_barrel_colours(fn, source, SimpleNamespace(bytes_at=lambda *_: None), manifest)[1][0]['status'], 'rejected')
        self.assertEqual(recover_barrel_colours(fn, source, RData(), {})[1][0]['status'], 'rejected')
