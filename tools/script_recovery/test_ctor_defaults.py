import json
import os
import unittest
from pathlib import Path

from tools.script_recovery.ctor_defaults import IMMEDIATE_KEY, constant_fields, constructor_constants, find_constructor
from tools.script_recovery.lift_native_lua import RData

ROOT = Path(__file__).resolve().parents[2]
ORCHARD = ROOT / 'refs/script_recovery/orchard_farm'


class ConstantFieldTests(unittest.TestCase):
    FIELDS = {'0x10': ['Count', 'Int'], '0x14': ['Handle', 'Int'], '0x18': ['Flag', 'Bool'], '0x19': ['Copied', 'Bool'],
              '0x1c': ['Untouched', 'Int']}

    def written(self):
        w = {}
        for off, raw in ((0x10, (0).to_bytes(4, 'little')),                 # a constant zero
                         (0x14, (0xDEAD0003).to_bytes(4, 'little'))):       # a call result (RegisterTimer)
            for i, b in enumerate(raw):
                w[off + i] = b
        w[0x18], w[0x19] = 1, 0
        w[IMMEDIATE_KEY] = frozenset({0x18})                                # only Flag was stored from an immediate
        return w

    def test_constants_only(self):
        self.assertEqual(constant_fields(self.written(), self.FIELDS), {'Count': ('Int', 0), 'Flag': ('Bool', True)})


class FindConstructorTests(unittest.TestCase):
    def test_constructor_allocator_and_factory_spellings(self):
        fns = [{'address': '0x100', 'decompile': '  *(undefined ***)this = &PTR__vector_deleting_destructor__012daf78;\n'},
               {'address': '0x200', 'decompile': '  this_00 = ::operator_new(0x70);\n  *(undefined ***)this_00 = &PTR__x_012daf78;\n',
                'calls': [{'target': '0x00BFEA1A', 'currentName': 'MSVCR71.DLL::operator_new'}]},
               {'address': '0x300', 'decompile': '  *(undefined ***)this = &PTR__x_012daf78;\n'}]
        self.assertEqual(find_constructor(fns, 0x012daf78, {0x300}), [(0x100, frozenset()), (0x200, frozenset({0xBFEA1A}))])
        factory = [{'address': '0x400', 'decompile': '  puVar2 = ::operator_new(0x3c);\n  *puVar2 = &PTR__x_012db364;\n',
                    'calls': [{'target': '0x00BFEA1A', 'currentName': 'MSVCR71.DLL::operator_new'}]}]
        self.assertEqual(find_constructor(factory, 0x012db364, set()), [(0x400, frozenset({0xBFEA1A}))])


@unittest.skipUnless(RData().ok and (ORCHARD / 'translation_unit_typed.json').is_file(), 'retail image not installed')
class OrchardConstructorTests(unittest.TestCase):
    def test_team_counters_are_zeroed_but_mission_state_is_not(self):
        spec = json.loads((ORCHARD / 'typing_spec.json').read_text(encoding='utf-8'))
        slots = {int(k, 16): len(v['params']) for k, v in spec['slots'].items()}
        tu = json.loads((ORCHARD / 'translation_unit_typed.json').read_text(encoding='utf-8'))
        unit = json.loads((ORCHARD / 'units' / os.listdir(ORCHARD / 'units')[0]).read_text(encoding='utf-8'))
        got = constructor_constants(tu['functions'], int(unit['vtable'], 16), set(), unit['quest']['fields'], slots.get)
        expected = {f'Teams_{t}_{m}': ('Int', 0) for t in (0, 1)
                    for m in ['MemberCount'] + [f'StateCounter_{i}' for i in range(6)]}
        self.assertEqual(got, expected)       # 0x00DCC040: timers are call results, CrateCount is left alone


if __name__ == '__main__':
    unittest.main()
