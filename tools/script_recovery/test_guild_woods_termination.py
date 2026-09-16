import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.benchmark_lifter import LuaSyntaxChecker

SOURCE=open('refs/script_recovery/lifted/GuildTraining/readable/FSE/GuildTrainingWoodsDeparture/GuildTrainingWoodsDeparture.lua',encoding='utf-8').read()

class GuildWoodsTerminationTests(unittest.TestCase):
    def test_success_and_failure_paths(self):
        for failed in (False,True):
            lua=LuaRuntime();lua.execute(SOURCE);q=lua.table();events=[];state={'MissionFailed':failed,'MissionSucceeded':not failed}
            q.GetStateBool=lambda _,name: state[name];q.NewScriptFrame=lambda _: True;q.IsActiveThreadTerminating=lambda _: False
            q.SetExperienceSpendingAsEnabled=lambda _,value: events.append(('xp',value));q.GetActiveQuestName=lambda _: 'woods'
            q.SetQuestAsFailed=lambda *args: events.append(('failed',args[1:]));q.SetQuestAsCompleted=lambda *args: events.append(('completed',args[1:]))
            q.DeactivateQuestLater=lambda *args: events.append(('deactivate',args[1:]))
            lua.globals().WatchForTermination(q)
            self.assertEqual(events[0],('xp',True));self.assertEqual(events[1][0],'failed' if failed else 'completed');self.assertEqual(events[-1],('deactivate',('Q_GuildTrainingWoodsDeparture',0)))
        self.assertTrue(LuaSyntaxChecker().check({'WoodsDeparture.lua':SOURCE})['ok'])

if __name__ == '__main__': unittest.main()
