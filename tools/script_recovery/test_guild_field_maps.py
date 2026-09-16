import json
import unittest
from pathlib import Path

class GuildFieldMapTests(unittest.TestCase):
    def test_cross_checked_state_names_are_present(self):
        p=Path('refs/script_recovery/guild_training/native_field_maps.json')
        data=json.loads(p.read_text(encoding='utf-8'))
        self.assertEqual(data['maps']['Q_GuildTrainingMelee']['0x4C'],'TutorialState')
        self.assertEqual(data['maps']['Q_GuildTrainingPreMelee']['0x50'],'DummyHits')
        self.assertEqual(data['maps']['Q_GuildTrainingWoodsMelee']['0x4B'],'ScorpionsAlive')
        self.assertNotIn('0x9F', data['maps'].get('Q_GuildTrainingWill',{}))

if __name__ == '__main__': unittest.main()
