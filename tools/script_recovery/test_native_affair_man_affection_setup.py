import struct
import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_affair_man_ranged_lines as fixtures
from tools.script_recovery.native_affair_man_conversation import recover_affair_man_ranged_lines,recover_affair_man_affection_setup
from tools.script_recovery.lift_native_lua import Lifter
from tools.script_recovery.native_call_setup_ir import read_call_window


class AffairManAffectionSetupTests(unittest.TestCase):
    def inputs(self):
        fn,source,data,manifest=fixtures.AffairManRangedLinesTests().inputs()
        source,_=recover_affair_man_ranged_lines(fn,source,data,manifest)
        return fn,source,data,manifest

    def test_pause_and_both_snap_directions(self):
        fn,source,data,manifest=self.inputs()
        result,evidence=recover_affair_man_affection_setup(fn,source,data,manifest)
        self.assertEqual(evidence[0]['status'],'recovered')
        calls=evidence[0]['edits'][0]['new'].splitlines()
        self.assertEqual([result.index(c) for c in calls],sorted(result.index(c) for c in calls))
        lifter=Lifter(manifest,{},'quest',True,'',data)
        body='\n'.join(lifter.lift('Affection','{\nuVar9 = GSI->GetHero();\n'+'\n'.join(calls)+'\n}',parameters={'local_native_cached_woman':'thing'}))
        self.assertEqual(lifter.todo,[])
        lua,events=LuaRuntime(),[]
        quest=lua.table_from({'GetHero':lambda q:'unrelated hero',
            'Pause':lambda q,seconds:events.append(('pause',struct.pack('<f',seconds))),
            'EntitySetFacingAngleTowardsThing':lambda q,actor,target,snap:events.append(('facing',actor,target,snap))})
        lua.execute('return function(quest,me,local_native_cached_woman)\n'+body+'\nend')(quest,'man','woman')
        self.assertEqual(events,[('pause',bytes.fromhex('cdcccc3e')),('facing','man','woman',True),('facing','woman','man',True)])

    def test_changed_snap_flag_rejects(self):
        fn,source,data,manifest=self.inputs()
        class Changed:
            def bytes_at(self,address,size):
                raw=data.bytes_at(address,size)
                if address==0xdb09e0:
                    raw=bytearray(raw)
                    raw[0xdb1b8a-address]=0
                    return bytes(raw)
                return raw
        result,evidence=recover_affair_man_affection_setup(fn,source,Changed(),manifest)
        self.assertEqual(result,source)
        self.assertEqual(evidence[0]['status'],'rejected')

    def test_cower_facing_uses_wife_before_retail_random_gate(self):
        fn,source,data,manifest=self.inputs()
        setup=read_call_window(data,0xdb09e0,5058,0xdb1656,argument_count=3)
        self.assertEqual(setup.stack_arguments,(('register','ebp'),('stack',44),('constant',0)))
        result,evidence=recover_affair_man_affection_setup(fn,source,data,manifest)
        self.assertEqual(evidence[0]['status'],'recovered')
        snippet=evidence[0]['edits'][1]['new']
        self.assertIn(snippet,result)
        lifter=Lifter(manifest,{},'quest',True,'',data)
        body='\n'.join(lifter.lift('Cower','{\nuVar9 = GSI->GetHero();\n'+snippet+'\nreturn iVar10;\n}',parameters={'local_native_cached_wife':'thing'}))
        self.assertEqual(lifter.todo,[])
        lua,events=LuaRuntime(),[]
        run=lua.execute('return function(quest,me,local_native_cached_wife)\n'+body+'\nend')
        for remainder in (0,1,49):
            events.clear()
            quest=lua.table_from({'GetHero':lambda q:'unrelated hero',
                'EntitySetFacingAngleTowardsThing':lambda q,actor,target,snap:events.append(('facing',actor,target,snap)),
                'RetailRandModulo':lambda q,modulus:events.append(('random',modulus)) or remainder})
            self.assertEqual(run(quest,'man','wife'),remainder)
            self.assertEqual(events,[('facing','man','wife',False),('random',50)])

    def test_conversation_faces_cached_woman_toward_fresh_hero(self):
        fn,source,data,manifest=self.inputs()
        result,evidence=recover_affair_man_affection_setup(fn,source,data,manifest)
        self.assertEqual(evidence[0]['status'],'recovered')
        call=evidence[0]['edits'][2]['new']
        self.assertIn(call,result)
        self.assertIn('uVar9 = GSI->GetHero();',result[:result.index(call)])
        lifter=Lifter(manifest,{},'quest',True,'',data)
        body='\n'.join(lifter.lift('FaceHero','{\nuVar9 = GSI->GetHero();\n'+call+'\n}',parameters={'local_native_cached_woman':'thing'}))
        self.assertEqual(lifter.todo,[])
        lua,events=LuaRuntime(),[]
        quest=lua.table_from({'GetHero':lambda q:events.append('hero') or 'fresh hero',
            'EntitySetFacingAngleTowardsThing':lambda q,actor,target,snap:events.append((actor,target,snap))})
        lua.execute('return function(quest,me,local_native_cached_woman)\n'+body+'\nend')(quest,'man','woman')
        self.assertEqual(events,['hero',('woman','fresh hero',False)])
