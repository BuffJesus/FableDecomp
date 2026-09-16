import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.benchmark_lifter import LuaSyntaxChecker

SOURCE=open('refs/script_recovery/lifted/GuildTraining/readable/FSE/GuildTrainingWoodsMelee/GuildTrainingWoodsMelee.lua',encoding='utf-8').read()

class GuildWoodsMeleeSetupTests(unittest.TestCase):
    def test_state_initialization_binding_workers_and_finish(self):
        lua=LuaRuntime();lua.execute(SOURCE);q=lua.table();events=[];loaded=[False,True];state={'ScorpionsAlive':True,'MissionSucceeded':False,'MissionFailed':False,'MissionOver':False};frames=0
        q.SetStateBool=lambda _,name,value: (state.__setitem__(name,value),events.append((name,value)))
        q.GetStateBool=lambda _,name: state[name];q.IsLevelLoaded=lambda _,name: loaded.pop(0)
        def frame(_):
            nonlocal frames
            frames+=1
            if frames==2: state['ScorpionsAlive']=False
            return True
        q.NewScriptFrame=frame;q.IsActiveThreadTerminating=lambda _: False
        q.AddEntityBinding=lambda _,name,path: events.append(('bind',name,path));q.FinalizeEntityBindings=lambda _: events.append('finalize');q.CreateThread=lambda _,name: events.append(('thread',name))
        lua.globals().Main(q)
        self.assertEqual(state['MissionSucceeded'],True);self.assertIn(('bind','ScorpionHome','GuildTrainingWoodsMelee/Entities/ScorpionHome'),events);self.assertTrue(LuaSyntaxChecker().check({'WoodsMelee.lua':SOURCE})['ok'])

    def test_native_worker_names_and_objective(self):
        self.assertIn('Quest:GiveHeroNewQuestObjective("first objective", 0)', SOURCE)
        self.assertIn('Quest:CreateThread("WatchForLeaving")', SOURCE)
        self.assertIn('Quest:CreateThread("TeleportOutHero")', SOURCE)
        self.assertIn('Quest:EndMission()', SOURCE)
        self.assertIn('TEXT_QST_028_GUILDMASTER_WOODS_DEPARTURE_TELEPORT_OUT_FIRST', SOURCE)

if __name__ == '__main__': unittest.main()
