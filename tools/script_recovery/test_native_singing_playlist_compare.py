import json
import unittest
from pathlib import Path

from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
try:
    from tools.script_recovery.native_reviewed_singing_playlist_compare import CHECKS, restore
except ImportError:
    from work.codex_lua_singing_playlist_compare_20261004.native_reviewed_singing_playlist_compare import CHECKS, restore

ROOT = Path(__file__).resolve().parents[2]
W = ROOT / 'work/codex_lua_singing_playlist_compare_20261004'
BEFORE = W / 'readable_comparison/before/singing_stones/FSE/V_SingingStones/V_SingingStones.lua'
AFTER = ROOT / 'refs/script_recovery/lifted/SingingStones/readable/FSE/V_SingingStones/V_SingingStones.lua'
if not (ROOT / 'tools/script_recovery/native_reviewed_singing_playlist_compare.py').exists():
    AFTER = W / 'readable_comparison/after/singing_stones/FSE/V_SingingStones/V_SingingStones.lua'


def run(path, normal, rude, personal, alive=(True, False, True)):
    lua = LuaRuntime(unpack_returned_tuples=True)
    values = {'CurrentPlayListIndex': 4}
    for prefix, notes in (('PlayList', normal), ('RudePlayList', rude), ('YourPersonalPlayList', personal)):
        values.update({f'{prefix}_{i}': note for i, note in enumerate(notes)})
    lua.globals().VALUES = lua.table_from(values)
    lua.globals().ALIVE = lua.table_from(alive)
    lua.execute('''
        events={}
        local effects={}
        for i=1,#ALIVE do effects[i]={id=i,IsAlive=function(self) return ALIVE[self.id] end} end
        resources=setmetatable({}, {__index=function(_,k) return function(self,...) return 0 end end})
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
                if k=='AddLineToConversation' then table.insert(events,{'line',args[2]});return end
                if k=='SetMasterGameState' then table.insert(events,{'master',args[1],args[2]});return end
                if k=='GetStateListCopy' then table.insert(events,{'copy',args[1]});return effects end
                if k=='RemoveThing' then table.insert(events,{'remove',args[1].id,args[2],args[3]});return end
                if k=='StateListClear' then table.insert(events,{'clear',args[1]});return end
                return 0
            end
        end})
    ''')
    lua.execute(path.read_text())
    lua.globals().WatchForCompleteTune(lua.globals().quest)
    return [tuple(row.values()) for row in lua.globals().events.values()]


class SingingPlaylistCompareReview(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        unit_path = ROOT / 'refs/script_recovery/singing_stones/units/V_SingingStones.json'
        if not (ROOT / 'tools/script_recovery/native_reviewed_singing_playlist_compare.py').exists():
            unit_path = W / 'candidate_unit.json'
        unit = json.loads(unit_path.read_text())
        cls.row = unit['reviewedSingingPlaylistCompare']['0x00ed2bb0']
        cls.fields = unit['quest']['fields']
        typed = json.loads((ROOT / 'refs/script_recovery/singing_stones/translation_unit_typed.json').read_text())
        cls.fn = next(f for f in typed['functions'] if f['address'].lower() == '0x00ed2bb0')
        cls.rdata = RData()
        cls.source = ('function WatchForCompleteTune(quest)\n'
                      '            c_stk_82 = 1\n            c_stk_81 = 1\n            iVar5 = 0\n'
                      '            piVar6 = (this + 0x6c)\n            repeat\n'
                      + ''.join('                ' + marker + '\n                if false then\n'
                                '                    c_stk_82 = 0\n                end\n'
                                for marker, _, _ in CHECKS)
                      + '                iVar5 = iVar5 + 1\n                piVar6 = piVar6 + 1\n'
                        '            until not (iVar5 < 4)\nend\n')
        cls.todo = [diagnostic for _, diagnostic, _ in CHECKS]

    def test_native_and_layout_gates(self):
        source, todo, calls = restore(self.source, self.todo[:], [], self.fn,
                                      self.row, self.rdata, self.fields)
        self.assertEqual(todo, [])
        self.assertEqual(source.count('GetStateInt("YourPersonalPlayList_" .. iVar5)'), 2)
        self.assertIn('GetStateInt', calls)
        with self.assertRaisesRegex(ValueError, 'native body changed'):
            restore(self.source, self.todo[:], [], self.fn,
                    {**self.row, 'nativeSha256': '0'*64}, self.rdata, self.fields)
        with self.assertRaisesRegex(ValueError, 'field layout changed'):
            restore(self.source, self.todo[:], [], self.fn,
                    self.row, self.rdata, {**self.fields, '0x4c': ['Wrong', 'Int']})

    def test_three_tune_outcomes(self):
        personal = (0, 1, 2, 3)
        normal = (0, 1, 2, 3)
        rude = (3, 2, 1, 0)
        cases = ((personal, rude, 'TEXT_QST_060_NAME_DBAC', False),
                 (rude, rude, 'TEXT_QST_060_NAME_CDBA', True),
                 ((1, 1, 2, 3), rude, 'TEXT_QST_060_NAME_BBCD', True))
        for chosen, expected_rude, line, cleanup in cases:
            with self.subTest(chosen=chosen):
                events = run(AFTER, normal, expected_rude, chosen)
                baseline = run(BEFORE, normal, expected_rude, chosen)
                self.assertIn(('line', 'TEXT_QST_060_NAME_DBAC'), baseline)
                self.assertIn(('line', line), events)
                self.assertEqual(('copy', 'EffectList') in events, cleanup)
                if cleanup:
                    self.assertEqual(events[-4:], [('copy', 'EffectList'),
                                                   ('remove', 1, False, True),
                                                   ('remove', 3, False, True),
                                                   ('clear', 'EffectList')])
                else:
                    self.assertIn(('master', 'SingingStonesInSync', True), events)


if __name__ == '__main__':
    unittest.main()
