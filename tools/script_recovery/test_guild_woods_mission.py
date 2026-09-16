import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.benchmark_lifter import LuaSyntaxChecker

SOURCE=open('refs/script_recovery/lifted/GuildTraining/readable/FSE/GuildTrainingWoodsDeparture/GuildTrainingWoodsDeparture.lua',encoding='utf-8').read()

class GuildWoodsMissionTests(unittest.TestCase):
    def test_objective_workers_wait_and_complete(self):
        lua=LuaRuntime();lua.execute(SOURCE);q=lua.table();events=[];loaded=[False,True];state={'MissionFailed':False,'MissionOver':False,'MissionSucceeded':False};frames=0
        q.GiveHeroNewQuestObjective=lambda *args: events.append(('objective',args[1:]))
        q.IsLevelLoaded=lambda _,name: loaded.pop(0)
        def frame(_):
            nonlocal frames
            frames += 1; events.append('frame')
            if frames >= 3: state['MissionOver']=True
            return True
        q.NewScriptFrame=frame
        q.IsActiveThreadTerminating=lambda _: False;q.CreateThread=lambda _,name: events.append(('thread',name))
        q.GetStateBool=lambda _,name: state[name]
        def completion(*args): events.append(('complete',args[1:]));state['MissionOver']=True
        q.SetQuestAsCompleted=completion;q.GetActiveQuestName=lambda _: 'woods';q.SetStateBool=lambda _,name,value: state.__setitem__(name,value)
        lua.globals().DoMission(q)
        self.assertEqual(events[0],('objective',('first objective',0)));self.assertEqual(events[-1],('complete',('woods',True,True,False)));self.assertTrue(state['MissionSucceeded']);self.assertTrue(LuaSyntaxChecker().check({'WoodsMission.lua':SOURCE})['ok'])

if __name__ == '__main__': unittest.main()
