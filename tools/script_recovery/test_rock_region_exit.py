import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.rock_region_exit import recover
from tools.script_recovery.rock_region_exit_native import execute
from tools.script_recovery.lift_native_lua import RData,ROOT
from tools.script_recovery.rock_state_candidate import generate


def run(loaded_frames=0,stop=100):
    body,_=recover();lua=LuaRuntime();lua.execute('function WatchForRegionExit(quest)\n'+body+'end')
    q=lua.table();events=[];state={'frames':0,'queries':0}
    def frame(_):state['frames']+=1;events.append(('frame',))
    def term(_):state['queries']+=1;value=state['queries']>=stop;events.append(('term',value));return value
    def region(_,name):
        value=state['frames']<loaded_frames;events.extend([('string.new',name),('region',name,value),('string.destroy',name)]);return value
    def generators(_,name,enabled):events.extend([('string.new',name),('generators',name,enabled),('string.destroy',name)])
    q.NewScriptFrame=frame;q.IsActiveThreadTerminating=term;q.IsRegionLoaded=region;q.SetCreatureGeneratorsEnabled=generators
    lua.globals().WatchForRegionExit(q);return events


class RegionExitTests(unittest.TestCase):
    def test_native_trace_all_wait_and_exit_cancellation_boundaries(self):
        for loaded_frames in (0,1,3):
            for stop in (1,2,3,4,100):
                with self.subTest(loaded_frames=loaded_frames,stop=stop):
                    self.assertEqual(run(loaded_frames,stop),execute(loaded_frames,stop))

    def test_cancellation_does_not_reenable_generators_or_repoll(self):
        events=run(3,1)
        self.assertEqual(events[-2:],[('frame',),('term',True)])
        self.assertEqual(sum(e[0]=='region' for e in events),1)
        self.assertFalse(any(e[0]=='generators' for e in events))
        self.assertEqual(run(0,1)[-1],('term',True))

    def test_changed_region_flag_and_cancellation_operands_rejected(self):
        class Changed(RData):
            def bytes_at(self,address,size):
                raw=super().bytes_at(address,size)
                if address<=self.changed<address+size:
                    raw=bytearray(raw);raw[self.changed-address]^=1;return bytes(raw)
                return raw
        for address in (0xec4138,0xec416e,0xec41a8,0xec41c7):
            data=Changed();data.changed=address
            with self.subTest(address=address),self.assertRaises(ValueError):recover(data)

    def test_composed_thread_name_resolves_without_adding_actor_stubs(self):
        report=generate();self.assertNotIn('WatchForRegionExit',report['missingFunctions'])
        path=ROOT/'work/rock_trigger_converter/state_candidate/RockTrollFirstEncounter/RockTrollFirstEncounter.lua'
        text=path.read_text();self.assertIn('function WatchForRegionExit(quest)',text)
        self.assertIn('CreateThread("WatchForRegionExit", {region = ""})',text)
        self.assertFalse(report['enabled']);self.assertIn('RTFE_RockTroll.Main',report['missingFunctions'])


if __name__=='__main__':unittest.main()
