import json
import unittest
from pathlib import Path

class GuildTrainingMainEvidenceTests(unittest.TestCase):
    def test_melee_skill_will_native_main_contracts(self):
        p = Path('refs/script_recovery/guild_training/training_main_evidence.json')
        d = json.loads(p.read_text(encoding='utf-8'))
        self.assertEqual([d['training'][x]['quest'] for x in ('Melee','Skill','Will')], [
            'Q_GuildTrainingMelee','Q_GuildTrainingSkill','Q_GuildTrainingWill'])
        self.assertEqual([d['training'][x]['teacher'] for x in ('Melee','Skill','Will')], [
            'M_MeleeTeacherStand','M_SkillTeacherStand','M_WillTeacherStand'])
        self.assertEqual(d['sharedNpc'], 'TheRealGuildmaster')

if __name__ == '__main__': unittest.main()
