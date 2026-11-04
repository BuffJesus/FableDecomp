import json
import unittest
from pathlib import Path

from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_reviewed_wasp_offset import (
    DIAGNOSTIC, NEW, OLD, restore,
)

ROOT = Path(__file__).resolve().parents[2]
W = ROOT / 'work/codex_lua_wasp_offset_20261004'
BEFORE = W / 'readable_comparison/before/wasp_boss/FSE/WaspBoss/WaspBoss.lua'
AFTER = W / 'readable_comparison/after/wasp_boss/FSE/WaspBoss/WaspBoss.lua'


def play(path):
    lua = LuaRuntime(unpack_returned_tuples=True)
    lua.execute('''
        events={}
        quest=setmetatable({}, {__index=function(_,k)
            return function(self,...)
                local args={...}
                if k=='RegisterTimer' then return 12 end
                if k=='SetQuestWorldMapOffset' then
                    table.insert(events,{'offset',select('#',...),args[1],args[2],args[3] or '<nil>'})
                elseif k=='AddQuestRegion' then
                    table.insert(events,{'region',args[1],args[2]})
                elseif k=='SetStateInt' then
                    table.insert(events,{'state-int',args[1],args[2]})
                elseif k=='SetStateBool' then
                    table.insert(events,{'state-bool',args[1],args[2]})
                elseif k=='SetTimer' then
                    table.insert(events,{'timer',args[1],args[2]})
                end
                if k=='GetStateInt' then return 12 end
                return 0
            end
        end})
    ''')
    lua.execute(path.read_text())
    lua.globals().Init(lua.globals().quest)
    return [tuple(row.values()) for row in lua.globals().events.values()]


class WaspOffsetReview(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        unit = json.loads((W / 'candidate_unit.json').read_text())
        cls.row = unit['reviewedWaspOffset']['0x00e0e820']
        typed = json.loads((ROOT / 'refs/script_recovery/wasp_boss/translation_unit_typed.json').read_text())
        cls.fn = next(f for f in typed['functions'] if f['address'].lower() == '0x00e0e820')
        cls.rdata = RData()

    def test_native_and_lifted_gates(self):
        source = (W / 'baseline_draft.lua').read_text()
        report = json.loads((W / 'baseline_report.json').read_text())
        todo = next(f['todo'] for unit in report['units'] for f in unit['functions']
                    if f['address'].lower() == '0x00e0e820')
        result, remaining, calls = restore(source, todo[:], [], self.fn, self.row, self.rdata)
        self.assertEqual(result.count(NEW), 1)
        self.assertEqual(remaining, [])
        self.assertEqual(calls, [])
        with self.assertRaisesRegex(ValueError, 'native body changed'):
            restore(source, todo[:], [], self.fn,
                    {**self.row, 'nativeSha256': '0'*64}, self.rdata)
        with self.assertRaisesRegex(ValueError, 'lifted map offset changed'):
            restore(source.replace(OLD, 'quest:OtherOffset()'), todo[:], [],
                    self.fn, self.row, self.rdata)
        self.assertEqual(todo, [DIAGNOSTIC])

    def test_full_init_map_offset_and_other_calls(self):
        before, after = play(BEFORE), play(AFTER)
        self.assertIn(('offset', 3, 'Q_WaspBoss', 10, '<nil>'), before)
        self.assertIn(('offset', 3, 'Q_WaspBoss', 10, 10), after)
        self.assertEqual([row for row in before if row[0] != 'offset'],
                         [row for row in after if row[0] != 'offset'])


if __name__ == '__main__':
    unittest.main()
