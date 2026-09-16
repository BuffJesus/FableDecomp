import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.benchmark_lifter import LuaSyntaxChecker

SOURCE=open('refs/script_recovery/lifted/GuildTraining/readable/FSE/GuildTrainingWoodsDeparture/GuildTrainingWoodsDeparture.lua',encoding='utf-8').read()

class GuildWoodsTeleportTests(unittest.TestCase):
    def test_low_health_teleports_converses_and_restores_health(self):
        lua=LuaRuntime();lua.execute(SOURCE);q=lua.table();events=[];frames=0;hero=lua.table();marker=lua.table()
        q.IsActiveThreadTerminating=lambda _: False;q.GetHero=lambda _: hero;q.GetHealth=lambda _,thing: 5.9
        q.GetThingWithScriptName=lambda _,name: marker;q.EntityTeleportToThing=lambda _,a,b: events.append(('teleport',a,b));q.Pause=lambda _,value: events.append(('pause',value))
        q.AddNewConversation=lambda *args: events.append(('conversation',args[1:])) or 'conversation';q.AddLineToConversation=lambda *args: events.append(('line',args[1:]))
        q.ChangeHeroHealthBy=lambda *args: events.append(('health',args[1:]));
        def frame(_):
            nonlocal frames
            frames+=1;return frames < 1
        q.NewScriptFrame=frame
        lua.globals().TeleportOutHero(q)
        self.assertEqual([e[0] for e in events],['teleport','pause','conversation','line','health'])
        self.assertEqual(events[-1],('health',(1000.0,True,False)));self.assertTrue(LuaSyntaxChecker().check({'WoodsTeleport.lua':SOURCE})['ok'])

if __name__ == '__main__': unittest.main()
