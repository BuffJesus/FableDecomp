import json
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_reviewed_renown_message import OLD, EXPECTED, restore

ROOT=Path(__file__).resolve().parents[2]
AFTER=ROOT/'refs/script_recovery/lifted/GameflowAssistance/readable/FSE/GameflowAssistance/GameflowAssistance.lua'

def run(path,level,saved=0):
    lua=LuaRuntime(unpack_returned_tuples=True)
    lua.globals().LEVEL=level
    lua.globals().SAVED=saved
    lua.execute('''
        events={}
        checks=0
        quest=setmetatable({}, {__index=function(_,k)
            return function(self,...)
                local args={...}
                if k=='IsActiveThreadTerminating' then return false end
                if k=='GetMasterGameState' then checks=checks+1;return checks>1 end
                if k=='GetHeroRenownLevel' then return LEVEL end
                if k=='IsHeroControlledByPlayer' then return true end
                if k=='GetStateInt' then return SAVED end
                if k=='SetStateInt' then table.insert(events,{'state',args[1],args[2]});return end
                if k=='HeroReceiveMessageFromGuildMaster' then table.insert(events,{'message',args[2]});return end
                if k=='RemoveQuestCardFromGuild' then table.insert(events,{'remove',args[1]});return end
                return 0
            end
        end})
    ''')
    lua.execute(path.read_text())
    lua.globals().Tutorial_RenownLevelIncrease(lua.globals().quest)
    return [tuple(x.values()) for x in lua.globals().events.values()]


class RenownMessageReview(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        unit=json.loads((ROOT/'refs/script_recovery/gameflow_assistance/units/GameflowAssistance.json').read_text())
        cls.row=unit['reviewedRenownMessages']['0x00cf02a0']
        typed=json.loads((ROOT/'refs/script_recovery/gameflow_assistance/translation_unit_typed.json').read_text())
        cls.fn=next(f for f in typed['functions'] if f['address'].lower()=='0x00cf02a0')
        cls.rdata=RData()
        cls.source='function Tutorial_RenownLevelIncrease(quest)\n    iVar2 = quest:GetHeroRenownLevel()\n    '+OLD+'\nend\n'

    def test_native_gated_message_table(self):
        value=restore(self.source,self.fn,self.row,self.rdata)
        self.assertEqual(value.count('local renownMessages'),1)
        self.assertEqual(value.count('renownMessages[iVar2]'),1)
        for key in EXPECTED:self.assertIn(key,value)
        with self.assertRaisesRegex(ValueError,'lifted message source changed'):
            restore(self.source.replace('GetHeroRenownLevel','GetHeroLevel'),self.fn,self.row,self.rdata)

    def test_unknown_native_refused(self):
        with self.assertRaisesRegex(ValueError,'native body changed'):
            restore(self.source,self.fn,{**self.row,'nativeSha256':'0'*64},self.rdata)
        with self.assertRaisesRegex(ValueError,'native message literals changed'):
            restore(self.source,self.fn,{**self.row,'messages':[]},self.rdata)

    def test_level_specific_messages_and_other_effects(self):
        for level in range(0,10):
            with self.subTest(level=level):
                events=run(AFTER,level)
                messages=[x[1] for x in events if x[0]=='message']
                self.assertEqual(messages,[EXPECTED[level-1]] if 1<=level<=7 else [])
                self.assertEqual([x for x in events if x[0]=='state'],[('state','SavedRenownLevel',level)] if level>0 else [])
                self.assertEqual(len([x for x in events if x[0]=='remove']),3 if level>1 else 0)

if __name__=='__main__':
    unittest.main()
