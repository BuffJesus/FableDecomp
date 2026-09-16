import itertools,unittest
from lupa import LuaRuntime
from tools.script_recovery.dead_father_candidate import SOURCE,prove
from tools.script_recovery.dead_father_native import execute_init
class DeadFatherInitTests(unittest.TestCase):
    def test_original_init_self_marker_and_callee_consumed_copy(self):
        prove();expected=[('text.new','HUD_ORB_QUEST_CORE'),('marker.add','self'),('text.destroy','HUD_ORB_QUEST_CORE'),('pushable',False),('copy.destroy',)]
        for args in itertools.product(('empty','invalid','valid'),(False,True)):
            with self.subTest(args=args):self.assertEqual(execute_init(*args),expected)
        lua=LuaRuntime();lua.execute(SOURCE);q=lua.table();r=lua.table();events=[];q.WithRetailResources=lambda _,fn:fn(r);r.InitializeDeadFatherActor=lambda _,actor:events.extend(expected)
        lua.globals().DeadFatherInit(q,7);self.assertEqual(events,expected)
if __name__=='__main__':unittest.main()
