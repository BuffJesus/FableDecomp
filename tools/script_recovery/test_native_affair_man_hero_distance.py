import unittest
from dataclasses import replace
from unittest.mock import patch
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_affair_man_animation_operands as fixtures
from tools.script_recovery.native_affair_man_conversation import recover_affair_man_animation_operands
from tools.script_recovery.native_hero_distance import recover_affair_man_hero_distances
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.lift_native_lua import Lifter


class AffairManHeroDistanceTests(unittest.TestCase):
    def inputs(self):
        fn,source,data,manifest=fixtures.AffairManAnimationOperandsTests().inputs()
        source,_=recover_affair_man_animation_operands(fn,source,data,manifest)
        return fn,source,data,manifest

    def test_distinct_hero_queries_thresholds_and_near_priority(self):
        fn,source,data,manifest=self.inputs()
        result,evidence=recover_affair_man_hero_distances(fn,source,data)
        self.assertEqual(evidence[0]['status'],'recovered')
        near,ranged=[e['new'] for e in evidence[0]['edits']]
        self.assertIn(near,result)
        self.assertIn(ranged,result)
        snippet=near+'\nif (bVar4) {\nreturn 5;\n}\n'+ranged+'\nif (bVar4) {\nreturn 13;\n}\nreturn 0;'
        lifter=Lifter(manifest,{},'quest',True,'',data)
        body='\n'.join(lifter.lift('Distances','{\n'+snippet+'\n}'))
        self.assertEqual(lifter.todo,[])
        lua=LuaRuntime()
        run=lua.execute('return function(quest,me)\n'+body+'\nend')
        for near_result,ranged_result in ((True,True),(True,False),(False,True),(False,False)):
            events=[]
            heroes=iter(('hero1','hero2'))
            def hero(q):
                value=next(heroes)
                events.append(('hero',value))
                return value
            def distance(q,actor,target,threshold):
                events.append(('distance',actor,target,threshold))
                return near_result if threshold==5 else ranged_result
            quest=lua.table_from({'GetHero':hero,'IsDistanceBetweenThingsUnder':distance})
            self.assertEqual(run(quest,'man'),5 if near_result else 13 if ranged_result else 0)
            expected=[('hero','hero1'),('distance','man','hero1',5)]
            if not near_result:
                expected.extend([('hero','hero2'),('distance','man','hero2',13)])
            self.assertEqual(events,expected)

    def test_changed_native_distance_argument_rejects(self):
        fn,source,data,manifest=self.inputs()
        def changed(*args,**kwargs):
            setup=read_call_window(*args,**kwargs)
            return replace(setup,stack_arguments=(('constant',0),)) if args[3]==0xdb1a0b else setup
        with patch('tools.script_recovery.native_hero_distance.read_call_window',side_effect=changed):
            result,evidence=recover_affair_man_hero_distances(fn,source,data)
        self.assertEqual(result,source)
        self.assertEqual(evidence[0]['status'],'rejected')
