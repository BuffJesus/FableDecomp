import json
import unittest
from unittest.mock import patch

from tools.script_recovery.guild_training_inventory import EVIDENCE, recover
from tools.script_recovery.export_native_threads import Image


class GuildInventoryTests(unittest.TestCase):
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
