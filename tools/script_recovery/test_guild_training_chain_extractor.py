import unittest
from tools.script_recovery.extract_guild_training_chain import extract

class GuildTrainingChainExtractorTests(unittest.TestCase):
    def test_native_teacher_anchors_extract_uniquely(self):
        d = extract()
        self.assertTrue(all(x['address'].startswith('0x00D') for x in d['training'].values()))
        self.assertEqual({x['quest'] for x in d['training'].values()}, {
            'Q_GuildTrainingMelee', 'Q_GuildTrainingSkill', 'Q_GuildTrainingWill'})
        self.assertEqual(d['followup'][-1], 'Q_GuildTrainingDeparture')

if __name__ == '__main__': unittest.main()
