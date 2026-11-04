import json
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_reviewed_renown_cleanup import LOOP, TODO, DIAGNOSTIC, restore

ROOT=Path(__file__).resolve().parents[2]
AFTER=ROOT/'refs/script_recovery/lifted/GameflowAssistance/readable/FSE/GameflowAssistance/GameflowAssistance.lua'

def run(path,level,saved=0,termination_at=None):
    lua=LuaRuntime(unpack_returned_tuples=True)
    lua.globals().LEVEL=level
    lua.globals().SAVED=saved
    lua.globals().TERMINATION_AT=termination_at or 999
    lua.execute('''
        events={}
        checks=0
        terminationChecks=0
        quest=setmetatable({}, {__index=function(_,k)
            return function(self,...)
                local args={...}
                if k=='IsActiveThreadTerminating' then terminationChecks=terminationChecks+1;return terminationChecks>=TERMINATION_AT end
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


class RenownCleanupReview(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        unit=json.loads((ROOT/'refs/script_recovery/gameflow_assistance/units/GameflowAssistance.json').read_text())
        cls.row=unit['reviewedRenownCleanup']['0x00cf02a0']
        typed=json.loads((ROOT/'refs/script_recovery/gameflow_assistance/translation_unit_typed.json').read_text())
        cls.fn=next(f for f in typed['functions'] if f['address'].lower()=='0x00cf02a0')
        cls.rdata=RData()
        loop='    repeat\n        register0x00000010 = (register0x00000010 + -4)\n        iVar2 = iVar2 + -1\n    until not (iVar2 ~= 0)\n'
        cls.source=('function Tutorial_RenownLevelIncrease(quest)\n    local iVar2, register0x00000010\n'+
                    '    iVar2 = 8\n'+loop+'    iVar2 = 8\n'+loop+
                    '    iVar2 = 8\n'+loop+loop+
                    ('    '+TODO+'\n')*3+'end\n')
        cls.todo=[DIAGNOSTIC]*3

    def test_native_gated_teardown_removal(self):
        source,todo=restore(self.source,self.todo,self.fn,self.row,self.rdata)
        self.assertNotIn('register0x00000010',source)
        self.assertNotIn('iVar2 = 8',source)
        self.assertNotIn(TODO,source)
        self.assertEqual(todo,[])
        with self.assertRaisesRegex(ValueError,'lifted cleanup shape changed'):
            restore(self.source.replace('iVar2 = 8','iVar2 = 7'),self.todo,self.fn,self.row,self.rdata)

    def test_unknown_native_refused(self):
        with self.assertRaisesRegex(ValueError,'native body changed'):
            restore(self.source,self.todo,self.fn,{**self.row,'nativeSha256':'0'*64},self.rdata)
        with self.assertRaisesRegex(ValueError,'native temporary teardown changed'):
            restore(self.source,self.todo,self.fn,{**self.row,'temporaryCount':7},self.rdata)

    def test_termination_paths(self):
        for level in (1,3,8):
            for stop in (None,1,2,3,4,5,6,7,8,9):
                with self.subTest(level=level,stop=stop):
                    events=run(AFTER,level,termination_at=stop)
                    if stop is not None and stop<=3:
                        self.assertEqual(events,[])
                    else:
                        state=[x for x in events if x[0]=='state']
                        self.assertEqual(state,[('state','SavedRenownLevel',level)])
                        if stop is None or stop>=5:
                            self.assertEqual([x[1] for x in events if x[0]=='message'],
                                             ['TEXT_QST_080_RENOWN_LEVEL_'+{1:'ONE',3:'THREE'}[level]] if level<8 else [])

if __name__=='__main__':
    unittest.main()
