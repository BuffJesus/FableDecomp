import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_readable_book_trader as book_tests
from tools.script_recovery.readable_book_trader_conditions import readable_book_conditions,CONDITION,DIRECT

class BookConditionReadabilityTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        book_tests.ReadableBookTraderTests.setUpClass()
        cls.before=book_tests.ReadableBookTraderTests.after
        cls.after,cls.report=readable_book_conditions(cls.before)

    def test_full_candidate_trace_matches_purchase_movement_cancellation_and_errors(self):
        runner=book_tests.ReadableBookTraderTests()
        for scenario in ({'hit':True},{'talk':True,'gold':2},{'talk':True,'gold':3},
                         {'talk':True,'answer':0},{'talk':True,'wait_answers':10},
                         {'move':True},{'fail_acquire':True},{'animation_byte':47}):
            for stop in range(1,31):
                case=dict(scenario,stop_check=stop,stop_frame=3)
                self.assertEqual(runner.events(self.before,**case),runner.events(self.after,**case))
        for case in ({'hit':True,'error_at':'health'},{'talk':True,'error_at':'speak'},{'move':True,'stop_frame':3,'error_at':'face'}):
            before,error1=runner.events(self.before,**case);after,error2=runner.events(self.after,**case)
            self.assertEqual(before,after);self.assertIn('injected',error1);self.assertIn('injected',error2)

    def test_timer_short_circuit_random_call_count_and_numeric_edges(self):
        for timer in (0,1,-1,float('nan')):
            for random in (0,1,199):
                results=[]
                for predicate in (CONDITION,DIRECT):
                    lua=LuaRuntime();events=[];quest=lua.table()
                    quest.GetStateInt=lambda _,name:events.append(('state',name)) or 9
                    quest.GetTimer=lambda _,identifier:events.append(('timer',identifier)) or timer
                    quest.RetailRandModulo=lambda _,limit:events.append(('random',limit)) or random
                    result=lua.execute('local quest=...; local timeRemaining,randomChoice,__native_condition_1;\n'+predicate+'return true end return false',quest)
                    results.append((result,events))
                self.assertEqual(*results)
                self.assertEqual(sum(e[0]=='random' for e in results[0][1]),int(timer==0))

    def test_additional_reader_changed_order_and_unknown_diagnostics(self):
        for bad in (self.before+'\nprint(randomChoice)',self.before.replace('timeRemaining == 0','timeRemaining ~= 0')):
            with self.assertRaises(ValueError):readable_book_conditions(bad)
        suffix='\n-- ((0x0) ~= 0)\nlocal diagnostic="((0x1) ~= 0)"\nlocal unknown=((native_unknown) ~= 0)\n'
        result,_=readable_book_conditions(self.before+suffix);self.assertTrue(result.endswith(suffix))
        self.assertNotIn('__native_condition_1',self.after)
        self.assertEqual(self.report['literalBooleanComparisons'],14)

if __name__=='__main__':unittest.main()
