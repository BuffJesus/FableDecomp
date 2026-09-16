"""Composed candidate files versus independent native traces and host gates."""
import unittest
from unittest.mock import patch
from tools.script_recovery import test_maze_research_reference as oracle
from tools.script_recovery.test_maze_converter import candidate_bridge
from tools.script_recovery.maze_runtime_candidate import generate
from tools.script_recovery.lift_native_lua import ROOT


def runtime_bridge(lua,h,entry):
    path={'history':'Entities/HistoryBookcase.lua','grave':'Entities/EmptyGrave.lua','unlimbo':'MazeResearch.lua'}[entry]
    source=(ROOT/'work/maze_converter/runtime_candidate'/path).read_text()
    def setup(quest,me):
        def initialize(_):
            sword=h.lookup(h,'GoodSword');h.retain_sword(h,sword);h.limbo(h,sword,True,True)
        quest.InitializeMazeSword=initialize
        def unlimbo(_):
            h.limbo(h,'sword',False,True);h.alpha(h,'sword',0.0,True)
        quest.UnlimboMazeSword=unlimbo
        quest.CreateMazeUnlimboThread=lambda _:h.spawn_unlimbo(h)
    candidate_bridge(lua,h,entry,source_override=source,quest_setup=setup)


class MazeRuntimeNativeTraces(oracle.MazeResearchReferenceTests):
    staged=True
    @classmethod
    def setUpClass(cls):generate()
    def setUp(self):
        change=patch.object(oracle,'staged_bridge',runtime_bridge);change.start();self.addCleanup(change.stop)


class MazeRuntimeCandidateGate(unittest.TestCase):
    def test_disabled_candidate_and_concrete_requirements(self):
        report=generate()
        self.assertFalse(report['registrationEnabled'])
        self.assertEqual(len(report['requiredHostCapabilities']),4)
        self.assertEqual(len(report['unresolvedIntegrationGate']),4)
        self.assertEqual(len(report['syntax']),3)
        with self.assertRaises(ValueError):generate(ROOT/'refs/script_recovery/lifted/MazeResearch')

    def test_composed_root_persistence_native_cases(self):
        import json
        from lupa.lua54 import LuaRuntime
        generate()
        cases=json.loads((ROOT/'refs/script_recovery/maze_research/runtime_evidence/native-port-persistence-20260912.json').read_text())['cases']
        source=(ROOT/'work/maze_converter/runtime_candidate/MazeResearch.lua').read_text()
        for case in cases:
            if case['entry']!='0xea80a0':continue
            with self.subTest(case=case):
                lua=LuaRuntime();lua.execute(source);quest=lua.table()
                state=dict(zip(('SwordTaken','BookRead'),case['initial']));storage=dict(case['stored']);calls=[]
                context=object()
                quest.GetStateBool=lambda _,name:state[name]
                quest.SetStateBool=lambda _,name,value:state.__setitem__(name,value)
                def transfer(_,passed,name,value,default):
                    self.assertIs(passed,context)
                    after=storage.get(name,default) if case['mode']=='read' else value
                    if case['mode']=='write':storage[name]=value
                    calls.append(dict(name=name,offset='0x48' if name=='SwordTaken' else '0x49',before=value,default=default,after=after))
                    return after
                quest.PersistTransferBool=transfer
                lua.globals().OnPersist(quest,context)
                self.assertEqual(calls,case['calls']);self.assertEqual(list(state.values()),case['final'])
                self.assertEqual(storage,case['storage'])

    def test_all_composed_host_capabilities(self):
        from tools.script_recovery.maze_prepare_runtime_proposal import prepare as prepare_marker
        from tools.script_recovery.maze_prepare_entry_proposal import prepare as prepare_entry
        marker=prepare_marker();entry=prepare_entry()
        self.assertTrue(marker['compiledTest'].startswith('PASS:'))
        self.assertTrue(entry['compiledTest'].startswith('PASS:'))
        self.assertTrue(entry['swordGate'].startswith('PASS:'))


if __name__=='__main__':unittest.main()
