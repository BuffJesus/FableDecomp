import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_affair_man_affection_setup as fixtures
from tools.script_recovery.native_affair_man_conversation import recover_affair_man_affection_setup,recover_affair_man_animation_operands
from tools.script_recovery.lift_native_lua import Lifter


class AffairManAnimationOperandsTests(unittest.TestCase):
    def inputs(self):
        fn,source,data,manifest=fixtures.AffairManAffectionSetupTests().inputs()
        source,_=recover_affair_man_affection_setup(fn,source,data,manifest)
        return fn,source,data,manifest

    def test_all_three_animations_preserve_seven_flags(self):
        fn,source,data,manifest=self.inputs()
        result,evidence=recover_affair_man_animation_operands(fn,source,data,manifest)
        self.assertEqual(evidence[0]['status'],'recovered')
        lua=LuaRuntime()
        keys=('ST_OPINION_FEAR_IDLE_COWERING','GIVE_KISS','GIVE_HUG')
        for index,(edit,key) in enumerate(zip(evidence[0]['edits'],keys)):
            self.assertIn(edit['new'],result)
            lifter=Lifter(manifest,{},'quest',True,'',data)
            body='\n'.join(lifter.lift('Animation','{\n'+edit['new']+'\n}',parameters={'DAT_01375748':'bool'}))
            self.assertEqual(lifter.todo,[])
            run=lua.execute('return function(quest,me,DAT_01375748)\n'+body+'\nend')
            for dynamic in (False,True):
                events=[]
                me=lua.table_from({'PlayAnimation':lambda actor,name,*flags:events.append((name,flags))})
                run(lua.table(),me,dynamic)
                self.assertEqual(events,[(key,(False,index!=0,False,True,dynamic,False,False))])

    def test_changed_native_final_flag_rejects(self):
        fn,source,data,manifest=self.inputs()
        class Changed:
            def bytes_at(self,address,size):
                raw=data.bytes_at(address,size)
                if address==0xdb09e0:
                    raw=bytearray(raw)
                    raw[0xdb1bf2-address]=1
                    return bytes(raw)
                return raw
        result,evidence=recover_affair_man_animation_operands(fn,source,Changed(),manifest)
        self.assertEqual(result,source)
        self.assertEqual(evidence[0]['status'],'rejected')
