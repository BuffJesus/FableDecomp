import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.benchmark_lifter import LuaSyntaxChecker

SOURCE = open('refs/script_recovery/lifted/GuildTraining/readable/FSE/GuildTraining/Entities/KillBird.lua', encoding='utf-8').read()

class GuildKillBirdTests(unittest.TestCase):
    def test_init_and_syntax(self):
        lua=LuaRuntime();lua.execute(SOURCE);quest=lua.table();me=lua.table();events=[]
        quest.GetHero=lambda _: 'hero'
        quest.EntitySetThingAsEnemyOfThing=lambda _,a,b: events.append((a,b))
        lua.globals().Init(quest,me)
        self.assertEqual(len(events),1)
        self.assertEqual(events[0][1],'hero')
        result=LuaSyntaxChecker().check({'KillBird.lua':SOURCE})
        self.assertTrue(result['ok'],result)

if __name__ == '__main__': unittest.main()
