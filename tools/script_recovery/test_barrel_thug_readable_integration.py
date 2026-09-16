import tempfile
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.build_readable_new_oakvale import build


class BarrelThugReadableTests(unittest.TestCase):
    def test_emitted_entry_init_and_native_ledger(self):
        with tempfile.TemporaryDirectory() as directory:
            report=build(Path(directory))
            output=(Path(directory)/'FSE/NewOakValeIntro/Entities/NOVI_BarrelThug.lua').read_text()
            self.assertTrue(report['syntax']['ok'])
            self.assertFalse(report['barrelThugCandidate']['enabled'])
            self.assertEqual(output.count('quest:RegisterBoundConsciousCondition()'),1)
            self.assertIn('Quests = {}',(Path(directory)/'FSE/quests.lua').read_text())
            for token in ('LAB_','goto ','[[missing]]','pCVar','fVar','TODO'):self.assertNotIn(token,output)
            rows={r['function']:r for r in report['functions'] if r['owner']=='NOVI_BarrelThug'}
            self.assertEqual(rows['Main']['implementationFunction'],'runBarrelThugMainAfterCondition')
            self.assertEqual(rows['Init']['implementationFunction'],'initializeBarrelThug')
            lua=LuaRuntime();lua.execute(output)
            lua.execute('''
                local events={}
                local function event(name) events[#events+1]=name end
                local resources={InitializeBarrelThugActor=function(_,me) assert(me==17);event('init') end}
                local q={
                    WithRetailResources=function(_,body) body(resources) end,
                    RegisterBoundConsciousCondition=function(_,...) assert(select('#',...)==0);event('condition') end,
                    NewScriptFrame=function(_,me) assert(me==17);event('frame') end,
                    IsActiveThreadTerminating=function() return true end,
                }
                Init(q,17);Main(q,17)
                assert(table.concat(events,',')=='init,condition,frame')
                assert(q.DoneIntro==nil and q.LastTimeSpoken==nil)
            ''')
