import itertools
import unittest

from lupa.lua54 import LuaRuntime

from tools.script_recovery.benchmark_lifter import LuaSyntaxChecker


SOURCE = open('refs/script_recovery/lifted/GuildTraining/readable/FSE/GuildTraining/Entities/SpeedFriend.lua', encoding='utf-8').read()


def run_case(frames, controls, cancel):
    lua = LuaRuntime(); lua.execute(SOURCE)
    quest, me = lua.table(), lua.table(); trace=[]; fi=ci=0
    def frame(_, thing):
        nonlocal fi
        value=frames[min(fi,len(frames)-1)]; fi+=1; trace.append(('frame',value)); return value
    def terminating(_):
        nonlocal cancel
        cancel -= 1; value=cancel <= 0; trace.append(('term',value)); return value
    def acquire(_, priority):
        nonlocal ci
        assert priority == 4; value=controls[min(ci,len(controls)-1)]; ci+=1; trace.append(('acquire',value)); return value
    quest.NewScriptFrame=frame; quest.IsActiveThreadTerminating=terminating
    quest.SetIsPushableByHero=lambda _,thing,push: (assertion(push is False),trace.append(('pushable',False)))[1]
    me.AcquireControl=acquire
    lua.globals().Main(quest,me)
    return trace


def assertion(value):
    if not value: raise AssertionError()


class GuildSpeedFriendTests(unittest.TestCase):
    def test_control_and_frame_boundaries(self):
        for frames, controls, cancel in itertools.product(((True, False), (True, True, False)), ((True,), (False, True), (False, False, True)), (8, 20)):
            with self.subTest(frames=frames,controls=controls,cancel=cancel):
                trace=run_case(frames,controls,cancel)
                self.assertEqual(trace[0],('frame',True))
                if ('pushable',False) in trace:
                    self.assertIn(('acquire',True),trace)
                    self.assertIn(('frame',False),trace)

    def test_lua_syntax(self):
        result=LuaSyntaxChecker().check({'SpeedFriend.lua':SOURCE})
        self.assertTrue(result['ok'], result)


if __name__ == '__main__': unittest.main()
