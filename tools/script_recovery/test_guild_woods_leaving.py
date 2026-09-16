import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.benchmark_lifter import LuaSyntaxChecker

SOURCE=open('refs/script_recovery/lifted/GuildTraining/readable/FSE/GuildTrainingWoodsDeparture/GuildTrainingWoodsDeparture.lua',encoding='utf-8').read()

class GuildWoodsLeavingTests(unittest.TestCase):
    def test_dead_hero_sets_failure_but_success_does_not(self):
        for success in (False,True):
            lua=LuaRuntime();lua.execute(SOURCE);q=lua.table();hero=lua.table();state={'MissionFailed':False,'MissionSucceeded':success};events=[]
            hero.IsAlive=lambda _: False;q.GetHero=lambda _: hero;q.GetStateBool=lambda _,name: state[name]
            q.SetStateBool=lambda _,name,value: (state.__setitem__(name,value),events.append((name,value)))
            q.NewScriptFrame=lambda _: True;q.IsActiveThreadTerminating=lambda _: False
            lua.globals().WatchForLeaving(q)
            self.assertEqual(events,[('MissionFailed',True)] if not success else [])
        self.assertTrue(LuaSyntaxChecker().check({'WoodsLeaving.lua':SOURCE})['ok'])

if __name__ == '__main__': unittest.main()
