import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_affair_wife_animation_random as fixtures
from tools.script_recovery.native_affair_wife_animation_random import recover_affair_wife_animation_random
from tools.script_recovery.native_affair_wife_animation_operands import recover_affair_wife_animation_operands
from tools.script_recovery.lift_native_lua import Lifter


class AffairWifeAnimationOperandsTests(unittest.TestCase):
    def inputs(self):
        fn,source,data,manifest=fixtures.AffairWifeAnimationRandomTests().inputs()
        source,_=recover_affair_wife_animation_random(fn,source,data)
        return fn,source,data,manifest

    def test_both_animations_preserve_all_flags_and_dynamic_value(self):
        fn,source,data,manifest=self.inputs()
        result,evidence=recover_affair_wife_animation_operands(fn,source,data)
        self.assertEqual(evidence[0]['status'],'recovered')
        self.assertIn('Runtime value of writable byte01375748',evidence[0]['unresolved'])
        lua=LuaRuntime()
        for edit,key in zip(evidence[0]['edits'],('ST_ARGUING_POINT_AWAY','ST_ARGUING_POINT_AT')):
            self.assertIn(edit['new'],result)
            lifter=Lifter(manifest,{},'quest',True,'',data)
            body='\n'.join(lifter.lift('Animation','{\n'+edit['new']+'\n}',parameters={'DAT_01375748':'bool'}))
            self.assertEqual(lifter.todo,[])
            run=lua.execute('return function(quest,me,DAT_01375748)\n'+body+'\nend')
            for dynamic in (False,True):
                events=[]
                me=lua.table_from({'PlayAnimation':lambda actor,name,*flags:events.append((name,flags))})
                run(lua.table(),me,dynamic)
                self.assertEqual(events,[(key,(False,False,False,True,dynamic,False,False))])

    def test_changed_wrapper_rejects(self):
        fn,source,data,manifest=self.inputs()
        class Changed:
            def bytes_at(self,address,size):
                raw=data.bytes_at(address,size)
                return bytes(size) if address==0x7e73d0 else raw
        result,evidence=recover_affair_wife_animation_operands(fn,source,Changed())
        self.assertEqual(result,source)
        self.assertEqual(evidence[0]['status'],'rejected')
