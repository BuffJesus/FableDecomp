"""Registry of native script translation units (address range + quest scripts) for the converter.

A unit is one contiguous retail code range holding a family of quest script classes. The range
bounds come from the native clusters (constructor of the first class .. destructor of the last).
"""
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]

UNITS = {
    'guild_training': {
        'evidence': ROOT / 'refs/script_recovery/guild_training',
        'lo': 0xD3B390, 'hi': 0xD68F00,
        'ir_glob': 'Q_GuildTraining*.json',
        'scripts': ['Q_GuildTraining', 'Q_GuildTrainingDeparture', 'Q_GuildTrainingMelee',
                    'Q_GuildTrainingPreMelee', 'Q_GuildTrainingSkill', 'Q_GuildTrainingWill',
                    'Q_GuildTrainingWoodsDeparture', 'Q_GuildTrainingWoodsMelee', 'Q_GuildTrainingWoodsWill'],
        'pdb_pattern': '*CQ_GuildTraining*',
        'schema': 'guild-training-native-inventory/0.1',
        'package': 'GuildTraining',
    },
    'orchard_farm': {
        # Q_OrchardFarmRaid ctor 0x00DCC040 .. Q_OrchardFarmRaidGood dtor 0x00DD26C0 (+ tail)
        # Q_OrchardFarm_Barricade has no script class: it is a resource section quest activated by the raid.
        'evidence': ROOT / 'refs/script_recovery/orchard_farm',
        'lo': 0xDCC040, 'hi': 0xDD2700,
        'ir_glob': 'Q_OrchardFarmRaid*.json',
        'scripts': ['Q_OrchardFarmRaid', 'Q_OrchardFarmRaidEvil', 'Q_OrchardFarmRaidGood'],
        'pdb_pattern': '*CQ_OrchardFarm*',
        'schema': 'orchard-farm-native-inventory/0.1',
        'package': 'OrchardFarm',
    },
}


def unit(name):
    if name not in UNITS:
        raise KeyError(f'unknown script unit {name!r}; known: {sorted(UNITS)}')
    return UNITS[name]
