import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.benchmark_lifter import LuaSyntaxChecker

ROOT='refs/script_recovery/lifted/GuildTraining/readable/FSE/'

class GuildQuestInitTests(unittest.TestCase):
    def check(self,name,expected):
        source=Path(ROOT+name+'/'+name+'.lua').read_text(encoding='utf-8');lua=LuaRuntime();lua.execute(source);q=lua.table();writes=[]
        q.SetStateInt=lambda _,key,value: writes.append((key,value))
        q.SetStateBool=lambda _,key,value: writes.append((key,value))
        lua.globals().Init(q)
        self.assertEqual(writes,expected)
        self.assertTrue(LuaSyntaxChecker().check({name+'.lua':source})['ok'])

    def test_skill_init(self):
        self.check('GuildTrainingSkill',[('TargetsHit',0),('TutorialState',1),('GenericTutorialCounter',0),('TotalTrainingDummies',3)])

    def test_will_init(self):
        self.check('GuildTrainingWill',[('TargetsHit',0),('TutorialState',1),('TotalTrainingDummiesCounter',0),('GenericTutorialCounter',0),('TestFinished',False),('BanditsDefeated',False)])

if __name__ == '__main__': unittest.main()
