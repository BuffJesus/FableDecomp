import json
import unittest
from unittest.mock import patch

from tools.script_recovery.guild_training_inventory import EVIDENCE, recover
from tools.script_recovery.export_native_threads import Image
from tools.script_recovery.guild_training_inventory import spawned_thread_name
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
import struct


class GuildInventoryTests(unittest.TestCase):
    def test_parent_class_thread_name_requires_concatenation(self):
        def code(concat=True, prefix='ParentClass.'):
            data = bytearray()
            def call(address):
                data.extend(b'\xe8' + struct.pack('<i', address - (0x1000 + len(data) + 5)))
            for pointer in (0x2000, 0x2100):
                data.extend(b'\x6a\xff\x68' + struct.pack('<I', pointer))
                call(0x99EBF0)
            if concat:
                call(0x99F570)
            decoder = Cs(CS_ARCH_X86, CS_MODE_32)
            decoder.detail = True
            instructions = list(decoder.disasm(bytes(data), 0x1000))
            return spawned_thread_name(instructions, len(instructions),
                                       {0x2000: 'TurnToBalv', 0x2100: prefix}.get)
        self.assertEqual(code(), 'TurnToBalv')
        self.assertIsNone(code(concat=False))
        self.assertIsNone(code(prefix='Unrelated.'))

    def test_trader_parent_threads_keep_distinct_native_bodies(self):
        inventory = recover(unit_name='trader_escort')
        threads = {t['name']: t['body'] for t in inventory['threads']}
        self.assertEqual(threads['WatchForPickpocketing'], '0x00E04F10')
        self.assertEqual(threads['TurnToBalv'], '0x00E0A820')
        self.assertEqual(len(threads), 4)
        self.assertTrue(all(t['bodyExported'] for t in inventory['threads']))

    def test_native_ownership_and_all_lifecycle_exports(self):
        inventory = recover()
        self.assertEqual(inventory, json.loads((EVIDENCE / 'inventory.json').read_text()))
        self.assertEqual(len(inventory['quests']), 9)
        entities = [e for q in inventory['quests'] for e in q['entities']]
        self.assertEqual(len(entities), 28)
        self.assertTrue(all(len(e['functions']) == 7 and not e['missingExports'] for e in entities))
        self.assertEqual(len(inventory['threads']), 16)
        self.assertTrue(all(t['name'] and t['bodyExported'] for t in inventory['threads']))
        woods = next(q for q in inventory['quests'] if q['script'] == 'Q_GuildTrainingWoodsMelee')
        self.assertEqual([e['name'] for e in woods['entities']], ['ScorpionHome'])

    def test_corrupt_native_entity_table_rejected(self):
        class ChangedImage(Image):
            def bytes_at(self, va, size):
                value = super().bytes_at(va, size)
                if va == 0x12CD7E8:
                    value = value[:4] + b'\0' * 4 + value[8:]
                return value

        with patch('tools.script_recovery.guild_training_inventory.Image', ChangedImage):
            with self.assertRaisesRegex(ValueError, 'ambiguous entity vtable'):
                recover()


if __name__ == '__main__':
    unittest.main()
