import itertools,unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.teddy_girl_health import SOURCE,DRAFT,lower,prove
from tools.script_recovery.teddy_girl_health_native import execute
from tools.script_recovery.lift_native_lua import RData

class TeddyGirlHealthTests(unittest.TestCase):
    def test_twelve_native_scopes_with_empty_outputs_unordered_and_far_join(self):
        lua=LuaRuntime();lua.execute(SOURCE)
        for site,value,empty in itertools.product(range(12),(1.0,0.0,-1.0,float('nan'),float('inf'),-float('inf')),(False,True)):
            with self.subTest(site=site,value=value,empty=empty):
                events=[];r=lua.table()
                def new(_,control):self.assertEqual(control,17);events.append(('thing.new',empty));return 29
                def health(_,actor):self.assertEqual(actor,29);events.append(('health',empty));return value
                def destroy(_,actor):self.assertEqual(actor,29);events.append(('thing.destroy',))
                r.NewThingFromResource=new;r.ThingHealth=health;r.DestroyThing=destroy
                result=lua.globals().TeddyGirlControlledHealthPositive(r,17)
                self.assertEqual(execute(site,value,empty),(result,events))

    def test_distinct_branch_roles_and_resource_operand(self):
        source,w=lower(DRAFT.read_text())
        self.assertNotIn('quest:GetHealth(',source)
        self.assertEqual([x['nativeIndex'] for x in w['sourceCorrespondence']],[1,0,2,3,4,5,6,9,10,8,7,11])
        self.assertEqual(w['healthSites'][4]['end'],0xdafa18)
        self.assertTrue(w['limits'])

    def test_changed_native_getter_comparison_destructor_and_source_rejected(self):
        original=RData()
        class Changed:
            def __init__(self,at):self.at=at
            def bytes_at(self,at,size):
                raw=original.bytes_at(at,size)
                if at<=self.at<at+size:
                    raw=bytearray(raw);raw[self.at-at]^=1;raw=bytes(raw)
                return raw
        for at in (0xdaf2f5,0xdaf851,0xdafa13,0x122dedc):
            with self.subTest(at=at),self.assertRaises(ValueError):prove(Changed(at))
        with self.assertRaises(ValueError):lower(DRAFT.read_text().replace('TEDDYGIRL_DONT_HIT','TEDDYGIRL_DONT_WANT'))

if __name__=='__main__':unittest.main()
