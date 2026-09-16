"""Readable package integration and whole-body trace preservation for Bully."""
import itertools
import tempfile
import unittest
from pathlib import Path
from collections import Counter
from lupa.lua54 import LuaRuntime
from tools.script_recovery.build_readable_new_oakvale import build
from tools.script_recovery.bully_full_resource_candidate import generate
from tools.script_recovery.test_bully_full_resource_candidate import HARNESS
from tools.script_recovery.test_bully_full_completion import EXTRA


def run(source, mode, health, cancel=999, completion=False):
    harness=HARNESS.replace('function quest:IsActiveThreadTerminating() return false end',
        'local queries=0; function quest:IsActiveThreadTerminating() queries=queries+1; rec("query:"..queries); return queries>='+str(cancel)+' end')
    if completion:
        harness=harness.replace('    source=source..', EXTRA+'\n    source=source..',1)
        harness=harness.replace('fn();fixture()', 'fn();Init(quest,me)')
    lua=LuaRuntime(unpack_returned_tuples=True)
    lua.execute('package={preload={}}; function require(n) return package.preload[n]() end')
    events,ok,error,polls=lua.execute(harness)(source,mode,health)
    # Lua embeds source line numbers in failures; compare the actual fault.
    fault=next((x for x in ('AFTER_ITEM','QUESTION','NATIVE_MACRO_FAULT','FRAME_GUARD') if x in error),error if ok else error.split(':')[-1])
    return list(events.values()),ok,fault,polls


class BullyReadableIntegrationTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.directory=tempfile.TemporaryDirectory()
        cls.report=build(Path(cls.directory.name))
        cls.relative='FSE/NewOakValeIntro/Entities/NOVI_Bully.lua'
        cls.output=(Path(cls.directory.name)/cls.relative).read_text()
        cls.source,cls.candidate=generate()

    @classmethod
    def tearDownClass(cls):
        cls.directory.cleanup()

    def test_disabled_package_entry_and_main_ledger(self):
        self.assertEqual(self.output.count('quest:RegisterBoundConsciousCondition()'),1)
        self.assertNotIn('WithBullyInitialControl',self.output)
        self.assertTrue(self.report['syntax']['ok'])
        self.assertFalse(self.report['bullyCandidate']['enabled'])
        self.assertFalse(self.report['bullyCandidate']['gameplayComplete'])
        self.assertIn('Quests = {}',(Path(self.directory.name)/'FSE/quests.lua').read_text())
        mapping=self.report['sourceMap'][self.relative]
        expected=Counter()
        for f in mapping['functions']:
            if f['function']=='resourceBody' or f['function'].startswith('Bully'):
                expected.update(v['basis'] for v in f['locals'].values())
        row=next(r for r in self.report['functions'] if r['owner']=='NOVI_Bully' and r['function']=='Main')
        self.assertEqual(row['implementationFunction'],'resourceBody')
        self.assertEqual(row['renamedLocals'],sum(expected.values()))
        # Structured phase generators already supply semantic local names;
        # generic rename counts do not measure implementation coverage.
        self.assertIn('resourceBody', {f['function'] for f in mapping['functions']})
        self.assertIn('BullyRunoffControls', mapping['structure']['nativeMainHelpers'])
        self.assertIn('BullyReturnHomePhase', mapping['structure']['nativeMainHelpers'])
        self.assertEqual(row['entryCondition'],self.candidate['entryCondition'])
        self.assertFalse(mapping['structure']['unstructuredJoinsRetained'])
        self.assertNotRegex(self.output, r'(?m)^\s*(?:goto\s|::LAB_)')

    def test_item_errors_health_and_query_cancellation_preserve_trace(self):
        for mode,health,cancel in itertools.product(('teddy','other','question'),(1.0,0.0,float('nan')),(1,2,3,4,5,6,7,8,12,999)):
            with self.subTest(mode=mode,health=health,cancel=cancel):
                before=run(self.source,mode,health,cancel)
                self.assertEqual(before,run(self.output,mode,health,cancel))
                self.assertEqual(before[0].count('condition'),1)

    def test_init_hits_and_runoff_error_preserve_entire_body_trace(self):
        for mode,cancel in itertools.product(('normal','macro_error'),(1,5,12,20,999)):
            with self.subTest(mode=mode,cancel=cancel):
                self.assertEqual(run(self.source,mode,1.0,cancel,True),run(self.output,mode,1.0,cancel,True))


if __name__=='__main__':unittest.main()
