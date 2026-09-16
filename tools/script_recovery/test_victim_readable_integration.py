import tempfile
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.build_readable_new_oakvale import build


class VictimReadableTests(unittest.TestCase):
    def test_entry_state_callback_and_ledger(self):
        with tempfile.TemporaryDirectory() as directory:
            report=build(Path(directory));relative='FSE/NewOakValeIntro/Entities/NOVI_Victim.lua'
            output=(Path(directory)/relative).read_text()
            self.assertTrue(report['syntax']['ok']);self.assertFalse(report['victimCandidate']['enabled'])
            self.assertIn('Quests = {}',(Path(directory)/'FSE/quests.lua').read_text())
            self.assertEqual(output.count('quest:RegisterBoundConsciousCondition()'),1)
            for token in ('LAB_','goto ','[[missing]]','pCVar','fVar'):self.assertNotIn(token,output)
            rows={r['function']:r for r in report['functions'] if r['owner']=='NOVI_Victim'}
            self.assertEqual(rows['Main']['implementationFunction'],'VictimMain')
            self.assertEqual(rows['Init']['implementationFunction'],'VictimInit')
            lua=LuaRuntime();lua.execute(output)
            lua.execute('''
local events={}
local resources={InitializeVictimActor=function(_,me) assert(me==17);events[#events+1]='init' end}
local q={
    WithRetailResources=function(_,body) body(resources) end,
    RegisterBoundConsciousCondition=function(_,...) assert(select('#',...)==0);events[#events+1]='condition' end,
    NewScriptFrame=function() events[#events+1]='frame' end,
    IsActiveThreadTerminating=function() return true end,
}
Init(q,17);Main(q,17)
assert(table.concat(events,',')=='init,condition,frame')
local savedState
package.loaded['NewOakValeIntro.native_quest_helpers']={
    AddBadDeed=function(quest,me,deed) assert(quest==q and me==17 and deed==2);events[#events+1]='deed' end,
}
VictimMain=function(quest,me,state,addBadDeed)
    assert(quest==q and me==17 and state:GetStateBool('DoneThanks')==false)
    savedState=state;state:SetStateBool('DoneThanks',true);addBadDeed(2)
end
Main(q,17)
assert(savedState:GetStateBool('DoneThanks')==true and q.DoneThanks==nil and events[#events]=='deed')
Init(q,17);assert(savedState:GetStateBool('DoneThanks')==false and savedState:GetStateBool('DisplayedGameInfo')==false)
''')
