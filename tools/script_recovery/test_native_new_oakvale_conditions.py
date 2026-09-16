import json
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime

from tools.script_recovery.lift_native_lua import ROOT, RData
from tools.script_recovery.native_new_oakvale_conditions import recover, verify


class NewOakvaleConditionTests(unittest.TestCase):
    def test_all_fifteen_register_once_before_first_frame_and_cancellation(self):
        witness = json.loads(Path(__file__).with_name('native_new_oakvale_conditions_witness.json').read_text())
        self.assertEqual(len(witness['entries']), 15)
        methods = []
        for entry in witness['entries']:
            with self.subTest(owner=entry['owner']):
                path = ROOT / 'refs/script_recovery/lifted/NewOakValeIntro/FSE/NewOakValeIntro/Entities' / (entry['owner']+'.lua')
                source, evidence = recover(entry['owner'], path.read_text())
                methods.append(evidence['method'])
                lua = LuaRuntime()
                lua.execute('''events = {}
                    quest = {}
                    function quest:RegisterBoundAliveCondition() events[#events+1] = 'alive' end
                    function quest:RegisterBoundConsciousCondition() events[#events+1] = 'conscious' end
                    function quest:NewScriptFrame(me) events[#events+1] = 'frame'; return true end
                    function quest:IsActiveThreadTerminating() return true end
                ''')
                lua.execute(source)
                lua.globals().Main(lua.globals().quest, lua.table())
                self.assertEqual(list(lua.globals().events.values()),
                    ['alive' if entry['owner']=='OVI_DeadFather' else 'conscious', 'frame'])
        self.assertEqual(methods.count('RegisterBoundConsciousCondition'), 14)

    def test_predicate_clone_and_registration_bytes_are_required(self):
        real = RData()
        for changed in (0xCDDAB0, 0xCDDAE0, 0xF35B10, 0xDB3FA0):
            class Changed:
                def bytes_at(self, address, size):
                    data = real.bytes_at(address, size)
                    return bytes([data[0]^1])+data[1:] if address==changed else data
            with self.subTest(address=hex(changed)), self.assertRaisesRegex(ValueError, 'condition bytes changed'):
                verify('NOVI_BookTrader', Changed())

    def test_unknown_owner_is_unchanged_and_changed_entry_rejects(self):
        self.assertEqual(recover('Unreviewed', 'anything'), ('anything', None))
        path = ROOT / 'refs/script_recovery/lifted/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_BookTrader.lua'
        source = path.read_text()
        changed = source.replace('    local alive = true', '    quest:UnexpectedCall()\n    local alive = true')
        with self.assertRaisesRegex(ValueError, 'prefix correspondence changed'):
            recover('NOVI_BookTrader', changed)
        output, _ = recover('NOVI_BookTrader', source)
        with self.assertRaisesRegex(ValueError, 'already present'):
            recover('NOVI_BookTrader', output)

    def test_resource_candidates_register_before_any_resource_construction(self):
        for owner in ('NOVI_AffairMan','NOVI_BookTrader','NOVI_AffairWoman'):
            if owner == 'NOVI_AffairWoman':
                from tools.script_recovery.generate_affair_woman_resource_candidate import generate
                source, _ = generate()
            else:
                source=(ROOT/'refs/script_recovery/lifted/NewOakValeIntro/candidates'/(owner+'.resource_candidate.lua')).read_text()
            source, _=recover(owner,source)
            lua=LuaRuntime()
            lua.execute('''events={}; quest={}
                function quest:WithRetailResources(body) body({}) end
                function quest:RegisterBoundConsciousCondition() events[#events+1]='condition' end
                function quest:NewScriptFrame(me) events[#events+1]='frame';return true end
                function quest:IsActiveThreadTerminating() return true end
            ''')
            lua.execute(source)
            lua.globals().Main(lua.globals().quest,lua.table())
            self.assertEqual(list(lua.globals().events.values()),['condition','frame'])


if __name__ == '__main__':
    unittest.main()
