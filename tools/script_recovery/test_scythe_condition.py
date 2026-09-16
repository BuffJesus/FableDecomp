import unittest
from unittest.mock import patch
from tools.script_recovery.lift_native_lua import ROOT, RData
from tools.script_recovery.scythe_condition import recover
from tools.script_recovery.scythe_control import recover as control
from tools.script_recovery.scythe_runtime_candidate import wire, build


class ScytheConditionTests(unittest.TestCase):
    def test_condition_precedes_first_frame_and_control(self):
        report=build()
        source=(ROOT/'work/scythe_converter/runtime_proposal/draft/Entities/ScytheNearOracle.lua').read_text()
        self.assertEqual(source.count('quest:RegisterBoundAliveCondition()'),1)
        self.assertLess(source.index('quest:RegisterBoundAliveCondition()'),source.index('quest:NewScriptFrame(me)'))
        self.assertLess(source.index('quest:NewScriptFrame(me)'),source.index('me:AcquireControl(4)'))
        self.assertEqual(report['entities']['ScytheNearOracle']['Main']['conditionEvidence']['status'],'recovered')
        self.assertFalse(report['registrationEnabled'])

    def test_changed_condition_target_clone_predicate_destructor_or_thread_rejects(self):
        source=control(wire((ROOT/'work/scythe_converter/draft/Entities/ScytheNearOracle.lua').read_text()))[0]
        data=RData(); original=data.bytes_at
        for site in (0xE2A337,0xE2A34B,0xCDF125,0xCDF151,0xE241CA,0xF35A19,0xCB7920,0xCB794A):
            def changed(address,size):
                raw=original(address,size)
                if address<=site<address+size:
                    raw=bytearray(raw);raw[site-address]^=1;return bytes(raw)
                return raw
            with patch.object(data,'bytes_at',side_effect=changed):
                with self.assertRaises(ValueError):recover(source,data)
        with self.assertRaisesRegex(ValueError,'correspondence changed'):recover(source+'-- changed')


if __name__=='__main__':unittest.main()
