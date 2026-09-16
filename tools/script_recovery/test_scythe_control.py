import unittest
from unittest.mock import patch
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import ROOT, RData
from tools.script_recovery.scythe_runtime_candidate import build, wire
from tools.script_recovery.scythe_control import recover


class ScytheControlTests(unittest.TestCase):
    def test_cancellation_has_no_extra_frame_and_all_entered_bodies_release(self):
        report = build()
        self.assertEqual(report['entities']['ScytheNearOracle']['Main']['todo'],
                         ['pending runtime integration: RunScytheOracleCutscene','pending runtime integration: FaceThingByScriptName'])
        source = (ROOT / 'work/scythe_converter/runtime_proposal/draft/Entities/ScytheNearOracle.lua').read_text()
        for stop, acquired, fail_marker in [(1,True,False),(99,False,False),(2,True,False),
                                            (3,True,False),(99,True,True)]:
            with self.subTest(stop=stop, acquired=acquired, fail_marker=fail_marker):
                lua, events = LuaRuntime(), []
                checks = [0]
                def terminating(_quest):
                    checks[0] += 1
                    return checks[0] == stop
                def marker(_q, actor, key):
                    if fail_marker: raise RuntimeError('marker double failed')
                quest = lua.table_from({'NewScriptFrame': lambda _q, actor: events.append('frame'),
                    'RegisterBoundAliveCondition': lambda _q: events.append('condition'),
                    'IsActiveThreadTerminating': terminating, 'MiniMapAddMarker': marker,
                    'GetThingWithScriptName': lambda _q, key: 'marker',
                    'EntitySetFacingAngleTowardsThing': lambda _q, *args: None,
                    'FaceThingByScriptName': lambda _q, *args: None,
                    'RegisterTimer': lambda _q: events.append('timer.start') or 81,
                    'DeregisterTimer': lambda _q, timer: events.append(('timer.end',timer))})
                me = lua.table_from({'AcquireControl': lambda _me, priority: events.append(('acquire',priority)) or acquired,
                                    'ReleaseControl': lambda _me: events.append('release')})
                lua.execute(source)
                if fail_marker:
                    with self.assertRaisesRegex(Exception, 'marker double failed'):
                        lua.globals().Main(quest, me)
                else:
                    lua.globals().Main(quest, me)
                expected = ['condition','frame']
                if stop != 1:
                    expected.append(('acquire',4))
                    if acquired:
                        if stop == 3: expected += ['timer.start',('timer.end',81)]
                        expected.append('release')
                self.assertEqual(events, expected)

    def test_native_operand_exit_and_cfg_changes_reject(self):
        source = wire((ROOT / 'work/scythe_converter/draft/Entities/ScytheNearOracle.lua').read_text())
        data = RData()
        original = data.bytes_at
        for site in (0xE2A3A4,0xE2A3D1,0xE2A3C4,0xE2A86F,0xE2A88F,0xE2A8B3,0xE2A8F2,0x7E74DA):
            def changed(address, size):
                raw = original(address,size)
                if address <= site < address + size:
                    raw = bytearray(raw); raw[site-address] ^= 1; return bytes(raw)
                return raw
            with patch.object(data,'bytes_at',side_effect=changed):
                with self.assertRaises(ValueError): recover(source,data)
        with patch('tools.script_recovery.scythe_control.check_single_resource_lifetime',return_value=False):
            with self.assertRaisesRegex(ValueError,'lifetime changed'): recover(source)
        with self.assertRaisesRegex(ValueError,'correspondence changed'): recover(source+'-- changed')


if __name__ == '__main__':
    unittest.main()
