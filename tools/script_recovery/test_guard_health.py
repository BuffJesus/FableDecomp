import itertools
import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.guard_health import SOURCE, DRAFT, lower, prove
from tools.script_recovery.guard_health_native import execute
from tools.script_recovery.lift_native_lua import RData


class GuardHealthTests(unittest.TestCase):
    def test_all_native_getter_float_and_destructor_paths(self):
        lua=LuaRuntime();lua.execute(SOURCE)
        for site,value,empty in itertools.product(range(19),(1.0,0.0,-1.0,float('nan'),float('inf'),-float('inf')),(False,True)):
            with self.subTest(site=site,value=value,empty=empty):
                events=[];r=lua.table()
                def new(_,control):
                    self.assertEqual(control,17);events.append(('thing.new',empty));return 29
                def health(_,actor):
                    self.assertEqual(actor,29);events.append(('health',empty));return value
                def destroy(_,actor):
                    self.assertEqual(actor,29);events.append(('thing.destroy',))
                r.NewThingFromResource=new;r.ThingHealth=health;r.DestroyThing=destroy
                result=lua.globals().GuardControlledHealthPositive(r,17)
                self.assertEqual(execute(site,value,empty),(result,events))

    def test_source_order_is_distinct_from_native_order(self):
        source,w=lower(DRAFT.read_text())
        self.assertNotIn('GetHealth(nil',source)
        self.assertEqual(w['sourceCorrespondence'][0]['nativeIndex'],11)
        self.assertEqual(w['sourceCorrespondence'][17]['nativeIndex'],6)
        self.assertEqual(len(w['sourceCorrespondence']),19)
        self.assertIn('full Main control flow',w['limits'][0])

    def test_changed_native_operands_and_draft_rejected(self):
        original=RData()
        class Changed:
            def __init__(self,at):self.at=at
            def bytes_at(self,at,size):
                raw=original.bytes_at(at,size)
                if at<=self.at<at+size:
                    raw=bytearray(raw);raw[self.at-at]^=1;raw=bytes(raw)
                return raw
        for at in (0xdacb5d,0xdacb70,0xdadc7d,0x122dedc):
            with self.subTest(at=at),self.assertRaises(ValueError):prove(Changed(at))
        with self.assertRaises(ValueError):lower(DRAFT.read_text().replace('GUARD_ON_HIT','GUARD_ON_TALK_BAD'))


if __name__=='__main__':unittest.main()
