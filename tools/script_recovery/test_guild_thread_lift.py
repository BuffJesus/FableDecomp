import unittest
from tools.script_recovery.lift_native_lua import load_entity_parent_state

from tools.script_recovery.lift_native_lua import lift_cluster


class GuildThreadLiftTests(unittest.TestCase):
    def test_guild_parent_state_map_is_used_by_entity_lifts(self):
        state = load_entity_parent_state('Q_GuildTrainingMelee.MeleeThunder')
        self.assertEqual(state['0x4c'], ('TutorialState', 'Int'))
        state = load_entity_parent_state('Q_GuildTrainingWoodsMelee.ScorpionHome')
        self.assertEqual(state['0x4b'], ('ScorpionsAlive', 'Bool'))
    def test_woods_workers_use_exported_bodies(self):
        for script in ('Q_GuildTrainingWoodsDeparture', 'Q_GuildTrainingWoodsMelee', 'Q_GuildTrainingWoodsWill'):
            report = lift_cluster(script)
            workers = {name: data for name, data in report['functions'].items() if name.startswith('Thread:')}
            self.assertEqual(set(workers), {
                'Thread:WatchForTermination', 'Thread:DoMission',
                'Thread:WatchForLeaving', 'Thread:TeleportOutHero'})
            self.assertTrue(all(data['lines'] > 1 for data in workers.values()))
            self.assertTrue(all('body unavailable' not in str(data['todo']) for data in workers.values()))


if __name__ == '__main__':
    unittest.main()
