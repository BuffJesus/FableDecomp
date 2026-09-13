import itertools
import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_affair_man_hits as fixtures
from tools.script_recovery.native_book_trader_hits import recover_affair_man_hits
from tools.script_recovery.native_affair_wife_mask import recover_affair_man_mask
from tools.script_recovery.lift_native_lua import Lifter,load_slots,load_thing_tables,thing_signatures


class AffairManMaskTests(unittest.TestCase):
    def inputs(self):
        fn,source,data,manifest=fixtures.AffairManHitTests().inputs()
        source,e=recover_affair_man_hits(fn,source,data,manifest)
        return fn,source,data,manifest,e[0]['new'].splitlines()[0]

    def test_cleanup_mask_survives_intervening_random_value(self):
        fn,source,data,manifest,start_line=self.inputs()
        result,evidence=recover_affair_man_mask(fn,source,data)
        self.assertEqual(evidence[0]['status'],'recovered')
        self.assertIn('pCVar13 = (CCarriedReadableDef *)&uStack_118;',result)
        start=result.index(start_line)
        stop=result.index("if (native_arg_man_hit != '\\0')",start)
        snippet=result[start:stop]
        self.assertIn('uVar14 = native_arg_man_cleanup_mask;',result)
        things,_=load_thing_tables(manifest,load_slots())
        lifter=Lifter(manifest,{},'quest',True,'',data,thing_sigs=thing_signatures(things))
        body='\n'.join(lifter.lift('Mask','{\n'+snippet+'\nuVar14 = 1;\nuVar14 = native_arg_man_cleanup_mask;\nreturn uVar14;\n}',parameters={'uVar14':'number'}))
        self.assertEqual(lifter.todo,[])
        lua=LuaRuntime()
        run=lua.execute('return function(me,uVar14)\n'+body+'\nend')
        for mask in (0,8,0x38,0xa8,0xfffffff8):
            for hit,any_special,special in itertools.product((False,True),repeat=3):
                me=lua.table_from({'MsgIsHitByHero':lambda actor:hit,
                    'MsgIsHitByAnySpecialAbilityFromHero':lambda actor:any_special,
                    'MsgIsHitByHeroSpecialAbility':lambda actor,value:special})
                self.assertEqual(run(me,mask),mask)

    def test_changed_mask_source_rejects(self):
        fn,source,data,manifest,_=self.inputs()
        source=source.replace('uStack_118 = uVar17 & 0xfffffffe;','uStack_118 = uVar17 & 0xfffffffd;')
        result,evidence=recover_affair_man_mask(fn,source,data)
        self.assertEqual(result,source)
        self.assertEqual(evidence[0]['status'],'rejected')
