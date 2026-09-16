import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.benchmark_lifter import LuaSyntaxChecker

SOURCE=open('refs/script_recovery/lifted/GuildTraining/readable/FSE/GuildTrainingDeparture/GuildTrainingDeparture.lua',encoding='utf-8').read()

class GuildDepartureTests(unittest.TestCase):
    def test_registration_wait_and_deactivation(self):
        lua=LuaRuntime();lua.execute(SOURCE);q=lua.table();events=[];active=[False,True,True,False]
        q.AddEntityBinding=lambda _,name,path: events.append(('bind',name,path))
        q.FinalizeEntityBindings=lambda _: events.append('finalize')
        q.IsQuestActive=lambda _,name: active.pop(0)
        q.NewScriptFrame=lambda _: True
        q.IsActiveThreadTerminating=lambda _: False
        q.GetActiveQuestName=lambda _: 'Q_GuildTrainingDeparture'
        q.DeactivateQuestLater=lambda _,name,delay: events.append(('deactivate',name,delay))
        lua.globals().Main(q)
        self.assertEqual(events[:2],[('bind','TheRealGuildmaster','GuildTrainingDeparture/Entities/TheRealGuildmaster'),'finalize'])
        self.assertEqual(events[-1],('deactivate','Q_GuildTrainingDeparture',0))
        self.assertTrue(LuaSyntaxChecker().check({'Departure.lua':SOURCE})['ok'])

if __name__ == '__main__': unittest.main()
