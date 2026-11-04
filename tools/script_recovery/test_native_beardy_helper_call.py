import json
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_reviewed_beardy_helper_call import CALL, restore

ROOT=Path(__file__).resolve().parents[2]
AFTER=ROOT/'refs/script_recovery/lifted/BeardyBaldy/readable/FSE/V_BeardyBaldy/Entities/BB_BeardyBaldyMan.lua'

def run(path, beard, phase=2):
    lua=LuaRuntime(unpack_returned_tuples=True)
    lua.globals().BEARD=beard
    lua.globals().PHASE=phase
    lua.execute('''
        events={}
        hero={}
        package.preload['V_BeardyBaldy.native_quest_helpers']=function()
            return setmetatable({}, {__index=function(_,k) return function() return k=='IsHeroWearingAnyTash' or k=='IsHeroWearingAnyOddHairdo' end end})
        end
        resources=setmetatable({}, {__index=function(_,k)
            return function(self,...)
                if k=='NewResource' then return {} end
                if k=='TryAcquire' then return true end
                if k=='ScriptThing' then return {} end
                return 0
            end
        end})
        frames=0
        quest=setmetatable({}, {__index=function(_,k)
            return function(self,...)
                local args={...}
                if k=='GetHero' then return hero end
                if k=='RetailResources' then return resources end
                if k=='IsActiveThreadTerminating' then return false end
                if k=='GetStateBool' then return PHASE==3 and args[1]=='AllHairChanged' end
                if k=='GetStateInt' then if args[1]=='QuestPhase' then return PHASE end;return 1 end
                if k=='GetTimer' then return 1 end
                if k=='GetHealth' then return 100 end
                if k=='IsWearingHairstyle' then return BEARD and args[2]=='OBJECT_HERO_BEARD_01' end
                if k=='NewScriptFrame' then frames=frames+1;return frames<5 end
                return 0
            end
        end})
        me=setmetatable({}, {__index=function(_,k)
            return function(self,...)
                if k=='IsTalkedToByHero' then return true end
                if k=='Speak' then local a={...};table.insert(events,a[2]);return false end
                if k=='IsPerformingScriptTask' then return false end
                return 0
            end
        end})
    ''')
    lua.execute(path.read_text())
    lua.globals().Main(lua.globals().quest,lua.globals().me)
    return list(lua.globals().events.values())


class BeardyHelperCallReview(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        unit=json.loads((ROOT/'refs/script_recovery/beardy_baldy/units/V_BeardyBaldy.json').read_text())
        cls.row=unit['reviewedBeardyHelperCall']['0x00e50fd0']
        cls.binding=unit['entities']['BB_BeardyBaldyMan']['helpers']['helper_E53670']
        typed=json.loads((ROOT/'refs/script_recovery/beardy_baldy/translation_unit_typed.json').read_text())
        cls.fn=next(f for f in typed['functions'] if f['address'].lower()=='0x00e50fd0')
        cls.helper=next(f for f in typed['functions'] if f['address'].lower()=='0x00e53670')
        cls.rdata=RData()
        block='    -- TODO(native): bVar6 = helper_E53670(quest, me, *(this + 0x14))\n    bVar6 = nil --[[unresolved native value]]\n'
        cls.source='function Main(quest, me)\n'+block+block+'end\n'
        cls.todo=['helper_E53670: 1 helper operands for 0 parameters']*2+[
            'unlifted: bVar6 = helper_E53670(quest, me, *(this + 0x14))']*2

    def test_native_gated_local_helper_calls(self):
        source,todo,calls=restore(self.source,self.todo,[],self.fn,self.helper,self.row,self.rdata,self.binding)
        self.assertEqual(source,'function Main(quest, me)\n'+'    bVar6 = helper_E53670(quest, me)\n'*2+'end\n')
        self.assertEqual(todo,[])
        self.assertEqual(calls,['helper_E53670'])
        with self.assertRaisesRegex(ValueError,'lifted helper calls changed'):
            restore(self.source.replace('0x14','0x18'),self.todo,[],self.fn,self.helper,self.row,self.rdata,self.binding)

    def test_unknown_native_or_binding_refused(self):
        with self.assertRaisesRegex(ValueError,'native body changed'):
            restore(self.source,self.todo,[],self.fn,self.helper,{**self.row,'helper':{**self.row['helper'],'nativeSha256':'0'*64}},self.rdata,self.binding)
        with self.assertRaisesRegex(ValueError,'helper registration changed'):
            restore(self.source,self.todo,[],self.fn,self.helper,self.row,self.rdata,{'address':'0x0'})

    def test_phase_three_dialogue_and_finale(self):
        expected={
            (2,False):'TEXT_QST_014_PHASE3_NO_BEARD',
            (2,True):'TEXT_QST_014_PHASE3_WRONG_HAIR_AND_BEARD',
            (3,False):'TEXT_QST_014_END_WRONG',
            (3,True):'TEXT_QST_014_END_FINALE1',
        }
        for (phase,beard),line in expected.items():
            with self.subTest(phase=phase,beard=beard):
                self.assertEqual(run(AFTER,beard,phase),[line])

if __name__=='__main__':
    unittest.main()
