import unittest
from unittest.mock import patch
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import ROOT,RData
from tools.script_recovery.scythe_runtime_candidate import build,wire
from tools.script_recovery.scythe_control import recover as control
from tools.script_recovery.scythe_condition import recover as condition
from tools.script_recovery.scythe_cleanup import recover as cleanup
from tools.script_recovery.scythe_returned_things import recover


class ScytheReturnedThingTests(unittest.TestCase):
    def test_candidate_uses_scoped_capabilities_and_exact_facing_flags(self):
        report=build();root=ROOT/'work/scythe_converter/runtime_proposal/draft/Entities'
        lua=LuaRuntime();events=[];me=lua.table()
        lua.execute((root/'ScytheMarker.lua').read_text())
        def spawn(_q,actor):
            self.assertTrue(lua.eval('function(a,b)return a==b end')(actor,me));events.append('spawn')
        lua.globals().Main(lua.table_from({'SpawnScytheAndRemoveMarker':spawn}),me)
        self.assertEqual(events,['spawn'])
        near=(root/'ScytheNearOracle.lua').read_text()
        self.assertIn('quest:FaceThingByScriptName(me, "MK_OW_SCYTHE3", false)',near)
        self.assertNotIn('quest:GetThingWithScriptName(',near)
        self.assertEqual(report['entities']['ScytheMarker']['Main']['returnedThingEvidence']['status'],'pending runtime integration')

    def test_changed_native_targets_names_and_source_reject(self):
        source=(ROOT/'work/scythe_converter/draft/Entities/ScytheNearOracle.lua').read_text()
        source=cleanup(condition(control(wire(source))[0])[0])[0]
        data=RData();original=data.bytes_at
        for site in (0xE29F9D,0xE29FA7,0xE29FBF,0xE2A446,0xE2A44F,0x4AA850):
            def changed(address,size):
                raw=original(address,size)
                if address<=site<address+size:
                    raw=bytearray(raw);raw[site-address]^=1;return bytes(raw)
                return raw
            with patch.object(data,'bytes_at',side_effect=changed):
                with self.assertRaises(ValueError):recover('ScytheNearOracle',source,data)
        with patch.object(data,'string_at',return_value='changed'):
            with self.assertRaisesRegex(ValueError,'name changed'):recover('ScytheNearOracle',source,data)
        with self.assertRaises(ValueError):recover('ScytheNearOracle',source+'-- changed')


if __name__=='__main__':unittest.main()
