import itertools,unittest
from tools.script_recovery.test_teddy_girl_question import lua_case
from tools.script_recovery.teddy_girl_question_native import execute

class TeddyGirlTalkTests(unittest.TestCase):
    def test_native_five_branches_state_short_circuit_health_and_cancellation(self):
        for done,hit,found,ruined,health,busy,cancel in itertools.product((False,True),(False,True),(False,True),(False,True),(1.0,0.0,float('nan')),(0,2),(1,2,3,4,999)):
            with self.subTest(done=done,hit=hit,found=found,ruined=ruined,health=health,busy=busy,cancel=cancel):
                case=dict(talk=True,done=done,hit=hit,found=found,ruined=ruined,health=health,busy=busy,cancel=cancel)
                self.assertEqual(execute(**case),lua_case(**case))

if __name__=='__main__':unittest.main()
