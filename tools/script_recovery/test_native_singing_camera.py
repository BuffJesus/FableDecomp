import json
import unittest
from pathlib import Path

from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_reviewed_singing_camera import (
    DIAGNOSTICS, NEW, OLD, restore,
)

ROOT = Path(__file__).resolve().parents[2]
W = ROOT / 'work/codex_lua_singing_camera_20261004'
BEFORE = W / 'readable_comparison/before/singing_stones/FSE/V_SingingStones/V_SingingStones.lua'
AFTER = W / 'readable_comparison/after/singing_stones/FSE/V_SingingStones/V_SingingStones.lua'


def play(path, personal):
    lua = LuaRuntime(unpack_returned_tuples=True)
    values = {'CurrentPlayListIndex': 4}
    for prefix, notes in (('PlayList', (0, 1, 2, 3)),
                          ('RudePlayList', (3, 2, 1, 0)),
                          ('YourPersonalPlayList', personal)):
        values.update({f'{prefix}_{i}': value for i, value in enumerate(notes)})
    lua.globals().VALUES = lua.table_from(values)
    lua.execute('''
        events={}
        resources=setmetatable({}, {__index=function(_,k) return function() return 0 end end})
        quest=setmetatable({}, {__index=function(_,k)
            return function(self,...)
                local args={...}
                if k=='IsActiveThreadTerminating' then return false end
                if k=='GetStateInt' then return VALUES[args[1]] or 0 end
                if k=='GetHero' or k=='GetNearestWithScriptName' then return {} end
                if k=='RetailResources' then return resources end
                if k=='GetMasterGameState' then return true end
                if k=='NewScriptFrame' then return false end
                if k=='AddNewConversation' then return 1 end
                if k=='GetAllThingsWithScriptName' then return {} end
                if k=='GetStateListCopy' then return {} end
                if k=='CameraUseCameraPoint' then
                    table.insert(events,{'camera',select('#',...),
                        args[1] or '<nil>',args[2] or '<nil>',args[3] or '<nil>',
                        args[4],args[5],args[6]})
                    return
                end
                return 0
            end
        end})
    ''')
    lua.execute(path.read_text())
    lua.globals().WatchForCompleteTune(lua.globals().quest)
    return [tuple(row.values()) for row in lua.globals().events.values()]


class SingingCameraReview(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        unit = json.loads((W / 'candidate_unit.json').read_text())
        cls.row = unit['reviewedSingingCamera']['0x00ed2bb0']
        cls.sidecar = unit['reviewedSingingCamera']['sidecar']
        typed = json.loads((ROOT / 'refs/script_recovery/singing_stones/translation_unit_typed.json').read_text())
        cls.fn = next(f for f in typed['functions'] if f['address'].lower() == '0x00ed2bb0')
        cls.rdata = RData()

    def test_native_sidecar_and_lifted_gates(self):
        source = (W / 'baseline_draft.lua').read_text()
        report = json.loads((W / 'baseline_report.json').read_text())
        todo = next(f['todo'] for unit in report['units'] for f in unit['functions']
                    if f['address'].lower() == '0x00ed2bb0')
        result, remaining, calls = restore(source, todo[:], [], self.fn, self.row,
                                           self.rdata, self.sidecar)
        self.assertEqual(result.count(NEW), 2)
        self.assertEqual(len(remaining), len(todo)-len(DIAGNOSTICS))
        self.assertEqual(calls, [])
        with self.assertRaisesRegex(ValueError, 'native body changed'):
            restore(source, todo[:], [], self.fn,
                    {**self.row, 'nativeSha256': '0'*64}, self.rdata, self.sidecar)
        with self.assertRaisesRegex(ValueError, 'lifted camera call shape changed'):
            restore(source.replace(OLD[0], 'quest:OtherCamera()'), todo[:], [],
                    self.fn, self.row, self.rdata, self.sidecar)

    def test_normal_and_rude_camera_calls(self):
        expected = ('camera', 6, 'CAM_Stones', '<nil>', '<nil>', -1.0, 0, -1)
        for personal in ((3, 2, 1, 0), (1, 1, 2, 3)):
            with self.subTest(personal=personal):
                after = play(AFTER, personal)
                before = play(BEFORE, personal)
                self.assertEqual([event for event in after if event[0] == 'camera'], [expected])
                self.assertEqual([event for event in before if event[0] == 'camera'],
                                 [('camera', 5, '<nil>', '<nil>', 0, 0, -1.0)])
        self.assertEqual([event for event in play(AFTER, (0, 1, 2, 3))
                          if event[0] == 'camera'], [])


if __name__ == '__main__':
    unittest.main()
