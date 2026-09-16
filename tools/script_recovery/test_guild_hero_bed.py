import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.benchmark_lifter import LuaSyntaxChecker

SOURCE = open('refs/script_recovery/lifted/GuildTraining/readable/FSE/GuildTraining/Entities/HeroBed.lua', encoding='utf-8').read()

class GuildHeroBedTests(unittest.TestCase):
    def test_all_bed_definitions_are_disabled_and_persistent(self):
        lua=LuaRuntime();lua.execute(SOURCE);quest=lua.table();me=lua.table();events=[]
        beds={
            'OBJECT_GUILD_BED_FLOOR_PALLET_01':['floor1','floor2'],
            'OBJECT_GUILD_BED_APPRENTICE_01':['apprentice'],
            'OBJECT_BS_SLUM_BED_BROWN_01':['slum1','slum2','slum3'],
        }
        quest.SetThingAsUsable=lambda _,thing,value: events.append(('usable',value))
        quest.SetThingPersistent=lambda _,thing,value: events.append(('persistent',value))
        quest.GetAllThingsWithDefName=lambda _,name: lua.table(*beds[name])
        quest.IsActiveThreadTerminating=lambda _: False
        lua.globals().Main(quest,me)
        self.assertEqual(events.count(('usable',False)),7)
        self.assertEqual(events.count(('persistent',True)),7)
        self.assertTrue(LuaSyntaxChecker().check({'HeroBed.lua':SOURCE})['ok'])

    def test_termination_stops_before_later_bed_groups(self):
        lua=LuaRuntime();lua.execute(SOURCE);quest=lua.table();me=lua.table();queries=0;events=[]
        quest.SetThingAsUsable=lambda _,thing,value: events.append('usable')
        quest.SetThingPersistent=lambda _,thing,value: events.append('persistent')
        quest.GetAllThingsWithDefName=lambda _,name: lua.table('first')
        def terminating(_):
            nonlocal queries
            queries+=1
            return queries > 0
        quest.IsActiveThreadTerminating=terminating
        lua.globals().Main(quest,me)
        self.assertEqual(events[:2],['usable','persistent'])
        self.assertEqual(len(events),2)

if __name__ == '__main__': unittest.main()
