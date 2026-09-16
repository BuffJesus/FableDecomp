import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import ROOT,RData
from tools.script_recovery.villager_quest_speech_lists import recover
from tools.script_recovery.native_speech_vectors import recover_vectors


class VillagerQuestSpeechListsTests(unittest.TestCase):
    def test_init_appends_exact_native_order_on_every_call(self):
        data=RData();path=ROOT/'refs/script_recovery/lifted/NewOakValeIntro/FSE/NewOakValeIntro/NewOakValeIntro.lua'
        raw=path.read_text();source,report=recover(raw,data);lua=LuaRuntime()
        lua.execute(source)
        events=lua.execute('''
            local events={}
            local lists={Append=function(_,category,male,key) events[#events+1]=key end}
            local q={SetStateBool=function() end,SetStateInt=function() end,
                GetStateInt=function() return 7 end,
                WithRetailResources=function(_,callback) callback({SetVillagerAmbientTimer=function(_,id,value) assert(id==7 and value==0) end}) end,
                GetVillagerSpeechLists=function() return lists end}
            Init(q);Init(q);return events
        ''')
        expected=[key for keys in recover_vectors(data.bytes_at).values() for key in keys]*2
        self.assertEqual(list(events.values()),expected)
        self.assertEqual(report['appends'],42)
        with self.assertRaises(ValueError):recover(raw.replace('.. (4), false)','.. (5), false)'),data)
