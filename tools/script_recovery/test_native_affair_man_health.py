import re
import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_affair_man_distance as fixtures
from tools.script_recovery.native_teddy_distance import recover_affair_man_distance, recover_affair_man_woman_distance
from tools.script_recovery.native_random import fold_retail_random
from tools.script_recovery.native_cached_thing_queries import recover_cached_thing_queries
from tools.script_recovery.native_affair_facing import recover_affair_facing
from tools.script_recovery.native_affair_pause import recover_affair_pause
from tools.script_recovery.native_affair_movies import recover_affair_movies
from tools.script_recovery.native_book_trader_health import recover_affair_man_health
from tools.script_recovery.lift_native_lua import RData, Lifter


class AffairManHealthTests(unittest.TestCase):
    def inputs(self):
        fn,source,manifest=fixtures.AffairManDistanceTests().inputs()
        data=RData()
        for recover in (recover_affair_man_distance,recover_affair_man_woman_distance):
            source,_=recover(fn,source,data)
        source,_=fold_retail_random(source,fn,data.bytes_at)
        for recover in (recover_cached_thing_queries,recover_affair_facing):
            source,_=recover(fn,source,data)
        for recover in (recover_affair_pause,recover_affair_movies):
            source,_=recover(fn,source,data,manifest)
        return fn,source,data,manifest

    def test_all_eight_queries_use_husband(self):
        fn,source,data,manifest=self.inputs()
        result,evidence=recover_affair_man_health(fn,source,data,manifest)
        self.assertEqual(evidence[0]['status'],'recovered')
        statements=re.findall(r'(fVar18 = \(float10\)GSI->GetHealth\(me\);\s*'
                              r'fVar3 = \(float10\)0\.0;)\s*'
                              r'C3DClothPrimitive::~C3DClothPrimitive\([^;]+;\s*'
                              r'if \((fVar3 < fVar18|fVar18 <= fVar3)\)',result)
        self.assertEqual(len(statements),8)
        self.assertNotIn('_DAT_0122dedc',result)
        self.assertEqual(result.count('fVar3 = (float10)0.0;'),8)
        lua,actors=LuaRuntime(),[]
        self.assertEqual(sum(condition=='fVar18 <= fVar3' for _,condition in statements),1)
        for statement,condition in statements:
            lifter=Lifter(manifest,{},'quest',True,'',data)
            body='\n'.join(lifter.lift('Health','{\nuVar8 = GSI->GetHero();\n'+statement+'\nreturn '+condition+';\n}'))
            self.assertEqual(lifter.todo,[])
            run=lua.execute('return function(quest,me)\n'+body+'\nend')
            for health in (-1,0,0.25,100):
                quest=lua.table_from({'GetHero':lambda q:'unrelated actor',
                    'GetHealth':lambda q,actor:actors.append(actor) or health})
                self.assertEqual(run(quest,'husband'),health<=0 if condition=='fVar18 <= fVar3' else health>0)
        self.assertEqual(actors,['husband']*32)

    def test_changed_threshold_rejects_all_edits(self):
        fn,source,data,manifest=self.inputs()
        class Changed:
            def bytes_at(self,address,size):
                return bytes.fromhex('0000803f') if address==0x122dedc else data.bytes_at(address,size)
        result,evidence=recover_affair_man_health(fn,source,Changed(),manifest)
        self.assertEqual(result,source)
        self.assertEqual(evidence[0]['status'],'rejected')

    def test_changed_source_rejects(self):
        fn,source,data,manifest=self.inputs()
        source+='\n// changed'
        result,evidence=recover_affair_man_health(fn,source,data,manifest)
        self.assertEqual(result,source)
        self.assertEqual(evidence[0]['status'],'rejected')
