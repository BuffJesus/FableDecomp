import json
import struct
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

from lupa.lua54 import LuaRuntime
from tools.script_recovery.convert_new_oakvale import convert, ROOT
from tools.script_recovery.native_deeds import recover_deed_operands, morality_amount
from tools.script_recovery.lift_native_lua import RData, annotate, load_manifest, load_slots, load_thing_tables
from tools.script_recovery.native_function_parameters import function_parameters, rename_parameters
from tools.script_recovery.test_lift_native_lua import make


class NativeDeedTests(unittest.TestCase):
    def test_signed_host_parameter_preserves_native_bit_pattern(self):
        for kind, expected in (('int', '-1'), ('long', '-1'), ('uint', '0xffffffff')):
            self.assertEqual(make().place_args([{'name': 'value', 'type': kind}], ['0xffffffff']), [expected])

    def test_changed_native_source_bytes_or_data_reject_recovery(self):
        tu = json.loads((ROOT / 'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
        slots = load_slots()
        things, returns = load_thing_tables(load_manifest(), slots)
        rdata = RData()
        for address, sign in (('0x00DB0660', 1), ('0x00DAEA70', -1)):
            fn = next(f for f in tu['functions'] if f['address'] == address)
            source = annotate(rename_parameters(fn['decompile'], function_parameters(fn['decompile'], member=True)),
                              slots, things, returns, entity=False)
            result, evidence = recover_deed_operands(fn, source, rdata.bytes_at)
            self.assertEqual(evidence[0]['status'], 'recovered')
            self.assertEqual(evidence[0]['amount'], sign * morality_amount())
            self.assertNotIn('GSI->GiveHeroMorality();', result)
            for changed_fn, changed_source, reader in (
                (dict(fn, decompile=fn['decompile'] + ' '), source, rdata.bytes_at),
                (fn, source + ' ', rdata.bytes_at), (fn, source, lambda *_: b'bad bytes'),
            ):
                result, evidence = recover_deed_operands(changed_fn, changed_source, reader)
                self.assertEqual(result, changed_source)
                self.assertEqual(evidence[0]['status'], 'rejected')
            with patch('tools.script_recovery.native_deeds.morality_amount', side_effect=ValueError('changed data')):
                self.assertEqual(recover_deed_operands(fn, source, rdata.bytes_at)[0], source)

    def test_generated_deeds_match_actual_port_effects(self):
        with tempfile.TemporaryDirectory() as directory:
            out = Path(directory)
            report = convert(out)
            self.assertEqual(report['sharedHelpers']['todo']['AddGoodDeed'], [])
            self.assertEqual(report['sharedHelpers']['todo']['AddBadDeed'], [])
            generated = (out / report['sharedHelpers']['path']).read_text()
        port = ROOT / 'refs/script_recovery/reconstructed/NewOakValeIntro/FSE'

        def run(is_native, good, bad, gold, sweets, delay, cancel, which=None, seen=False):
            lua, events = LuaRuntime(), []
            lua.globals().package.path = port.as_posix() + '/?.lua;' + lua.globals().package.path
            module = lua.execute(generated) if is_native else lua.eval('require("NewOakValeIntro.deeds")')[0]
            fields = {'GoodDeedsPerformed': good, 'BadDeedsPerformed': bad,
                      'GivenSweets': sweets, 'GUIGoodDeedCounter': 17}
            fields.update({f'WhichBadDeedsPerformed_{i}': seen if i == which else False for i in range(5)})
            frame = [0]
            def write(_q, name, value):
                fields[name] = value
                events.append(('state', name, value))
            def advance(_q, me):
                self.assertEqual(me, 'entity')
                frame[0] += 1
                events.append(('frame',))
                return not cancel
            methods = {'GetStateInt': lambda _q, name: fields[name], 'GetStateBool': lambda _q, name: fields[name],
                       'SetStateInt': write, 'SetStateBool': write, 'NewScriptFrame': advance,
                       'IsActiveThreadTerminating': lambda _q: cancel and frame[0] > 0,
                       'MsgIsGameInfoClickedPast': lambda _q: frame[0] >= delay,
                       'GetHeroGold': lambda _q: gold, 'GetActiveQuestName': lambda _q: 'NewOakValeIntro',
                       # Match the actual host's float conversion, not Lua double spelling.
                       'GiveHeroMorality': lambda _q, amount: events.append(('morality', struct.pack('<f', amount)))}
            for name in ('DisplayGameInfo', 'AddLogbookTutorialEntry', 'SetQuestCardObjective', 'UpdateQuestInfoCounter'):
                methods[name] = lambda _q, *args, name=name: events.append((name, *args))
            if which is None:
                method = module.AddGoodDeed if is_native else module.add_good
                method(lua.table_from(methods), 'entity')
            else:
                method = module.AddBadDeed if is_native else module.add_bad
                method(lua.table_from(methods), 'entity', which)
            return events, fields

        for scenario in ((0, 0, 0, False, 0, False), (1, 0, 0, False, 2, False),
                         (0, 2, 3, False, 0, False), (0, 0, 0, True, 0, False),
                         (0, 0, 0, False, 2, True)):
            with self.subTest(scenario=scenario):
                self.assertEqual(run(True, *scenario), run(False, *scenario))
        for which in range(5):
            for scenario in ((0, 0, 0, False, 0, False, which, False),
                             (0, 1, 0, False, 0, False, which, True),
                             (1, 1, 0, False, 2, False, which, False),
                             (0, 0, 0, False, 2, True, which, False)):
                with self.subTest(scenario=scenario):
                    self.assertEqual(run(True, *scenario), run(False, *scenario))


if __name__ == '__main__':
    unittest.main()
