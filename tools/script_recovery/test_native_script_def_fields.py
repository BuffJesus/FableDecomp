import csv
from pathlib import Path
import unittest
from unittest.mock import patch

from lupa import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.readable_style import name_script_def_reads

R = Path(__file__).resolve().parents[2]
from tools.script_recovery import native_script_def_fields as native
from tools.script_recovery.script_def_offsets import load


class AlteredImage:
    def __init__(self, image, changed_bytes=None, changed_strings=None):
        self.image = image
        self.changed_bytes = changed_bytes or {}
        self.changed_strings = changed_strings or {}

    def bytes_at(self, address, size):
        result = self.image.bytes_at(address, size)
        if result is None:
            return result
        result = bytearray(result)
        for location, value in self.changed_bytes.items():
            if address <= location < address + size:
                result[location-address] = value
        return bytes(result)

    def string_at(self, address):
        return self.changed_strings.get(address, self.image.string_at(address))


class NativeScriptDefFieldTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.image = RData()
        with (R / 'ghidra_out/struct_layouts_egor.tsv').open() as handle:
            cls.names = {row[4] for row in csv.reader(handle, delimiter='\t')
                         if len(row) >= 5 and row[0] == 'CScriptDef' and row[4] != '_padding_'}
        cls.table = load()

    def test_metadata_matches_all_native_sites(self):
        fields = native.recover(self.image, self.names)
        self.assertEqual(len(fields), 943)
        self.assertEqual({name: row['offset'] for name, row in fields.items()},
                         {row['name']: int(offset, 0) for offset, row in self.table.items()})
        self.assertEqual(fields['SickChildsSisterAttentionAnims']['offset'], 0x74c)
        self.assertEqual(fields['ArenaRounds']['offset'], 0x1044)
        self.assertEqual(fields['DemonDoor_EatPies_FatnessLevel']['offset'], 0x1084)

    def test_retail_profile_changes_fail_closed(self):
        for address in (native.START, 0xf309a0, 0xf28fb9, 0x12fb5c8 + 18*4,
                        0x12fb5c8 + 20*4, 0xf29501):
            with self.subTest(address=hex(address)):
                previous = self.image.bytes_at(address, 1)[0]
                with self.assertRaises(ValueError):
                    native.recover(AlteredImage(self.image, {address: previous ^ 1}), self.names)

    def test_missing_or_additional_pdb_name_is_rejected(self):
        for names in (self.names - {'ArenaRounds'}, self.names | {'InventedMember'}):
            with self.subTest(count=len(names)), self.assertRaises(ValueError):
                native.recover(self.image, names)

    def test_swapped_native_strings_with_same_name_set_are_rejected(self):
        strings = {}
        for address in (0x12fb9f8, 0x12fb9e8):
            name = self.image.string_at(address)
            if name in ('ArenaRounds', 'HeroSoulsRounds'):
                strings[address] = 'HeroSoulsRounds' if name == 'ArenaRounds' else 'ArenaRounds'
        # Addresses come from the named pushes in the pinned transfer method.
        self.assertEqual(len(strings), 2)
        with self.assertRaisesRegex(ValueError, 'literals differ'):
            native.recover(AlteredImage(self.image, changed_strings=strings), self.names)

    def test_names_preserve_calls_and_runtime_tunable_values(self):
        src = ('return function(quest)\n'
               ' return quest:ReadGlobalGameData(0x270), quest:ReadGlobalGameData(0x74c), '
               'quest:ReadGlobalGameData(0x750), quest:ReadGlobalGameData(0x1044)\nend\n')
        with patch('tools.script_recovery.script_def_offsets.load', return_value=self.table):
            output, count = name_script_def_reads(src)
        self.assertEqual(count, 3)
        self.assertIn('SCRIPT_DEF.RansomVictimHaveVictimKilledCost', output)
        self.assertIn('SCRIPT_DEF.SickChildsSisterAttentionAnims', output)
        self.assertIn('SCRIPT_DEF.ArenaRounds', output)
        self.assertIn('ReadGlobalGameData(0x750)', output)  # vector end is not a named field
        lua = LuaRuntime(unpack_returned_tuples=True)
        for salt in (-987, 0, 12345):
            expected = tuple(offset + salt for offset in (0x270, 0x74c, 0x750, 0x1044))
            calls = []
            quest = lua.table()
            def read(receiver, offset):
                calls.append(offset)
                return offset + salt
            quest.ReadGlobalGameData = read
            self.assertEqual(lua.execute(output)(quest), expected)
            self.assertEqual(calls, [0x270, 0x74c, 0x750, 0x1044])


if __name__ == '__main__':
    unittest.main()
