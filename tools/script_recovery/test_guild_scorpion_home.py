import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.benchmark_lifter import LuaSyntaxChecker

SOURCE = Path('refs/script_recovery/lifted/GuildTraining/readable/FSE/GuildTrainingWoodsMelee/Entities/ScorpionHome.lua').read_text(encoding='utf-8')

class GuildScorpionHomeTests(unittest.TestCase):
    def test_init_contract_and_native_anchors(self):
        lua = LuaRuntime(); lua.execute(SOURCE)
        entity = lua.table()
        lua.globals().Init(entity)
        for anchor in ('HUD_BEETLE_ICON', 'GuildScorpions', 'CREATURE_GUILD_STAG_BEETLE', 'ScorpionSpawn'):
            self.assertIn(anchor, SOURCE)
        self.assertTrue(LuaSyntaxChecker().check({'ScorpionHome.lua': SOURCE})['ok'])

if __name__ == '__main__': unittest.main()
