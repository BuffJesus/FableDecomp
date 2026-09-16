import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.benchmark_lifter import LuaSyntaxChecker

SOURCE=open('refs/script_recovery/lifted/GuildTraining/readable/FSE/GuildTrainingWoodsDeparture/GuildTrainingWoodsDeparture.lua',encoding='utf-8').read()

class GuildWoodsDepartureTests(unittest.TestCase):
    def test_setup_wait_bind_threads_and_marker(self):
        lua=LuaRuntime();lua.execute(SOURCE);q=lua.table();events=[];loaded=[False,True];marker=lua.table();marker.GetPos=lambda _: 'pos'
        q.SetStateBool=lambda _,name,value: events.append(('bool',name,value));q.SetStateInt=lambda _,name,value: events.append(('int',name,value))
        q.IsLevelLoaded=lambda _,name: loaded.pop(0);q.NewScriptFrame=lambda _: True;q.IsActiveThreadTerminating=lambda _: False
        q.AddEntityBinding=lambda _,name,path: events.append(('bind',name,path));q.FinalizeEntityBindings=lambda _: events.append('finalize')
        q.CreateThread=lambda _,name: events.append(('thread',name));q.SetQuestCardObjective=lambda *args: events.append(('objective',args[1]))
        q.GetThingWithScriptName=lambda _,name: marker;q.CreateCreature=lambda _,name,pos,script: events.append(('creature',name,pos,script))
        lua.globals().Main(q)
        self.assertIn(('int','DepartureMissionPoint',0),events);self.assertEqual(sum(x[0]=='bind' for x in events if isinstance(x,tuple)),3)
        self.assertEqual(events[-1],('creature','MazeCreationMarker','pos','FinalMaze'));self.assertTrue(LuaSyntaxChecker().check({'WoodsDeparture.lua':SOURCE})['ok'])

if __name__ == '__main__': unittest.main()
