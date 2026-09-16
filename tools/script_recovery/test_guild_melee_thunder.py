import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.benchmark_lifter import LuaSyntaxChecker

SOURCE = open('refs/script_recovery/lifted/GuildTraining/readable/FSE/GuildTrainingMelee/Entities/MeleeThunder.lua', encoding='utf-8').read()

class GuildMeleeThunderTests(unittest.TestCase):
    def test_state_gates_and_controls(self):
        lua=LuaRuntime();lua.execute(SOURCE);q=lua.table();me=lua.table();state={'TutorialState':4};events=[];frames=0;controls=iter([False,True,True])
        q.GetStateInt=lambda _,name: state[name]
        def frame(_,thing):
            nonlocal frames
            frames += 1; events.append('frame'); return frames < 8
        q.NewScriptFrame=frame
        q.IsActiveThreadTerminating=lambda _: False
        def acquire(_,priority):
            events.append(('acquire',priority)); value=next(controls)
            if value and len([x for x in events if isinstance(x,tuple)])==2: state['TutorialState']=6
            elif value and len([x for x in events if isinstance(x,tuple)])==3: state['TutorialState']=0
            return value
        me.AcquireControl=acquire
        lua.globals().Main(q,me)
        self.assertGreaterEqual(events.count('frame'),3)
        self.assertEqual([x for x in events if isinstance(x,tuple)], [('acquire',4),('acquire',4),('acquire',4)])

    def test_lua_syntax(self):
        self.assertTrue(LuaSyntaxChecker().check({'MeleeThunder.lua':SOURCE})['ok'])

if __name__ == '__main__': unittest.main()
