import itertools
import unittest
from types import SimpleNamespace
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_affair_man_health as fixtures
from tools.script_recovery.native_book_trader_health import recover_affair_man_health
from tools.script_recovery.native_book_trader_hits import recover_affair_man_hits
from tools.script_recovery.lift_native_lua import Lifter,load_slots,load_thing_tables,thing_signatures


class AffairManHitTests(unittest.TestCase):
    def inputs(self):
        fn,source,data,manifest=fixtures.AffairManHealthTests().inputs()
        source,_=recover_affair_man_health(fn,source,data,manifest)
        return fn,source,data,manifest

    def test_truth_table_query_order_and_mask(self):
        fn,source,data,manifest=self.inputs()
        result,evidence=recover_affair_man_hits(fn,source,data,manifest)
        self.assertEqual(evidence[0]['status'],'recovered')
        self.assertNotIn('LAB_00db0bc9',result)
        snippet=evidence[0]['new'].replace('cStack_11d','native_arg_man_hit')
        things,_=load_thing_tables(manifest,load_slots())
        lifter=Lifter(manifest,{},'quest',True,'',data,thing_sigs=thing_signatures(things))
        body='\n'.join(lifter.lift('Hit','{\n'+snippet+'\nreturn native_arg_man_hit;\n}',parameters={'uVar14':'number'}))
        self.assertEqual(lifter.todo,[])
        lua=LuaRuntime()
        run=lua.execute('return function(me,uVar14)\n'+body.replace('return native_arg_man_hit','return native_arg_man_hit,uVar17,cVar5')+'\nend')
        for hit,any_special,special in itertools.product((False,True),repeat=3):
            events=[]
            me=lua.table_from({'MsgIsHitByHero':lambda actor:events.append('hit') or hit,
                'MsgIsHitByAnySpecialAbilityFromHero':lambda actor:events.append('any') or any_special,
                'MsgIsHitByHeroSpecialAbility':lambda actor,value:events.append(('special',value)) or special})
            actual=run(me,0x80)
            expected=['hit']+([] if hit else ['any']+([('special',14)] if any_special else []))
            self.assertEqual(events,expected)
            self.assertEqual(actual,(hit or (any_special and not special),0x80 | (1 if hit else 7 if any_special else 3),hit if hit else special if any_special else any_special))

    def test_changed_ability_rejects(self):
        fn,source,data,manifest=self.inputs()
        def changed(address,size):
            raw=data.bytes_at(address,size)
            if address==0xdb09e0:
                raw=bytearray(raw)
                raw[0xdb0bb1-address]=13
                return bytes(raw)
            return raw
        result,evidence=recover_affair_man_hits(fn,source,SimpleNamespace(bytes_at=changed,string_at=data.string_at),manifest)
        self.assertEqual(result,source)
        self.assertEqual(evidence[0]['status'],'rejected')
