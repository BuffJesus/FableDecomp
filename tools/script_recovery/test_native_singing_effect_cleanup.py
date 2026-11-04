import json
import unittest
from pathlib import Path

from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
try:
    from tools.script_recovery.native_reviewed_singing_effect_cleanup import KILL, ERASE, KILL_DIAG, ERASE_DIAG, restore
except ImportError:
    from work.codex_lua_singing_effect_cleanup_20261004.native_reviewed_singing_effect_cleanup import KILL, ERASE, KILL_DIAG, ERASE_DIAG, restore


ROOT = Path(__file__).resolve().parents[2]
AFTER = ROOT / 'refs/script_recovery/lifted/SingingStones/readable/FSE/V_SingingStones/V_SingingStones.lua'
if not (ROOT / 'tools/script_recovery/native_reviewed_singing_effect_cleanup.py').exists():
    AFTER = ROOT / 'work/codex_lua_singing_effect_cleanup_20261004/readable_comparison/after/singing_stones/FSE/V_SingingStones/V_SingingStones.lua'


def region_events(path, alive):
    lua = LuaRuntime(unpack_returned_tuples=True)
    lua.globals().ALIVE = lua.table_from(alive)
    lua.execute('''
        events={}
        regionChecks=0
        local effect={}
        for i=1,#ALIVE do
            effect[i]={id=i,IsAlive=function(self) return ALIVE[self.id] end}
        end
        quest=setmetatable({}, {__index=function(_,k)
            return function(self,...)
                local args={...}
                if k=='IsActiveThreadTerminating' then return false end
                if k=='IsRegionLoaded' then regionChecks=regionChecks+1; return regionChecks==1 end
                if k=='NewScriptFrame' then return false end
                if k=='GetStateListCopy' then
                    table.insert(events,{'copy',args[1]})
                    return effect
                end
                if k=='RemoveThing' then
                    table.insert(events,{'remove',args[1].id,args[2],args[3]})
                    return
                end
                if k=='StateListClear' then table.insert(events,{'clear',args[1]}); return end
                error('unexpected quest method '..k)
            end
        end})
    ''')
    lua.execute(path.read_text())
    lua.globals().WatchForRegionLeaving(lua.globals().quest)
    return [tuple(row.values()) for row in lua.globals().events.values()]


class SingingEffectCleanupReview(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        unit_path = ROOT / 'refs/script_recovery/singing_stones/units/V_SingingStones.json'
        if not (ROOT / 'tools/script_recovery/native_reviewed_singing_effect_cleanup.py').exists():
            unit_path = ROOT / 'work/codex_lua_singing_effect_cleanup_20261004/candidate_unit.json'
        unit = json.loads(unit_path.read_text())
        cls.meta = unit['reviewedSingingEffectCleanup']
        typed = json.loads((ROOT / 'refs/script_recovery/singing_stones/translation_unit_typed.json').read_text())
        cls.fn = {f['address'].lower(): f for f in typed['functions']}
        cls.rdata = RData()

    def test_native_gated_cleanup(self):
        for address, count in (('0x00ed2bb0', 3), ('0x00ed2a50', 1)):
            with self.subTest(address=address):
                row = self.meta[address]
                name = 'WatchForCompleteTune' if count == 3 else 'WatchForRegionLeaving'
                source = ('function ' + name + '(quest)\n' + ('    ' + KILL + '\n') * count +
                          ('    quest:StateListClear("EffectList")\n' * 3 if count == 3 else '    ' + ERASE + '\n') + 'end\n')
                todo = [KILL_DIAG] * count + ([] if count == 3 else [ERASE_DIAG])
                result, diagnostics, calls = restore(source, todo, [], self.fn[address], row, self.rdata, self.meta)
                self.assertEqual(result.count('RemoveLiveEffects(quest)'), count + (count == 3))
                self.assertEqual(result.count('quest:StateListClear("EffectList")'), 3 if count == 3 else 1)
                self.assertEqual(diagnostics, [])
                self.assertIn('RemoveThing', calls)
                with self.assertRaisesRegex(ValueError, 'native body changed'):
                    restore(source, todo, [], self.fn[address], {**row, 'nativeSha256': '0' * 64}, self.rdata, self.meta)

    def test_region_leaving_removes_only_live_effects_then_clears(self):
        for alive in ([], [True], [False], [True, False, True]):
            with self.subTest(alive=alive):
                events = region_events(AFTER, alive)
                self.assertEqual(events, [('copy', 'EffectList'),
                                          *[('remove', i, False, True) for i, live in enumerate(alive, 1) if live],
                                          ('clear', 'EffectList')])

if __name__ == '__main__':
    unittest.main()
