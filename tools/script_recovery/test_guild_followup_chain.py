import json
import unittest
from pathlib import Path

class GuildFollowupChainTests(unittest.TestCase):
    def test_melee_skill_will_chain_and_npc_boundary(self):
        p = Path('refs/script_recovery/guild_training/followup_quest_chain.json')
        d = json.loads(p.read_text(encoding='utf-8'))
        self.assertEqual([x['followupQuest'] for x in d['chains']], [
            'Q_GuildTrainingSkill', 'Q_GuildTrainingWill', 'Q_GuildTrainingDeparture'])
        self.assertTrue(d['npcEvidence']['waitAfterCompletion'])
        self.assertEqual(d['nativeCallOffsets']['isActiveQuest'], '0x470')
        self.assertEqual(d['nativeCallOffsets']['stageQuest'], '0x450')
        self.assertEqual(d['liveVerification'], 'deferred')

if __name__ == '__main__': unittest.main()
