"""Extract the three Guild training teacher/completion anchors from the native TU."""
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
TU = ROOT / 'refs/script_recovery/guild_training/translation_unit.json'
OUT = ROOT / 'refs/script_recovery/guild_training/training_main_evidence.json'
TARGETS = {
    'Melee': ('M_MeleeTeacherStand', 'Q_GuildTrainingMelee', 'Complete Melee'),
    'Skill': ('M_SkillTeacherStand', 'Q_GuildTrainingSkill', 'Complete Skill'),
    'Will': ('M_WillTeacherStand', 'Q_GuildTrainingWill', 'Complete Will'),
}

def extract():
    data = json.loads(TU.read_text(encoding='utf-8-sig'))
    result = {'schema': 'guild-training-main-evidence/0.1',
              'basis': 'native translation-unit anchor extraction', 'training': {}}
    for mode, (teacher, quest, label) in TARGETS.items():
        matches = [f for f in data['functions']
                   if teacher in {s.get('value') for s in f.get('strings', [])}]
        if not matches:
            raise ValueError(f'{mode}: native teacher anchor missing')
        # The full teacher body is the largest matching body; short helpers may
        # repeat the marker while the TU keeps them separate.
        f = max(matches, key=lambda x: int(x.get('size', 0)))
        result['training'][mode] = {'address': f['address'], 'teacher': teacher,
                                    'quest': quest, 'completionLabel': label}
    result['sharedNpc'] = 'TheRealGuildmaster'
    result['followup'] = ['Q_GuildTrainingSkill', 'Q_GuildTrainingWill', 'Q_GuildTrainingDeparture']
    result['liveVerification'] = 'deferred'
    OUT.write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
    return result

if __name__ == '__main__':
    print(json.dumps(extract(), indent=2))
