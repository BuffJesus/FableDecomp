import tempfile
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.build_readable_new_oakvale import build
from tools.script_recovery.guard_candidate import generate
from tools.script_recovery.test_guard_candidate import HARNESS

class GuardReadableIntegrationTests(unittest.TestCase):
    def test_disabled_package_ledger_and_whole_body_trace_preservation(self):
        with tempfile.TemporaryDirectory() as directory:
            report=build(Path(directory));relative='FSE/NewOakValeIntro/Entities/NOVI_Guard.lua'
            output=(Path(directory)/relative).read_text();source,candidate=generate()
            self.assertTrue(report['syntax']['ok']);self.assertFalse(report['guardCandidate']['enabled'])
            self.assertIn('Quests = {}',(Path(directory)/'FSE/quests.lua').read_text())
            row=next(r for r in report['functions'] if r['owner']=='NOVI_Guard' and r['function']=='Main')
            self.assertEqual(row['implementationFunction'],'resourceBody');self.assertIsNone(row['entryCondition'])
            self.assertNotIn('RegisterBound',output);self.assertNotIn('goto ',output)
            self.assertFalse(report['sourceMap'][relative]['structure']['unstructuredJoinsRetained'])
            self.assertEqual(report['guardCandidate']['limits'],candidate['limits'])
            def run(text,mode,cancel,fault):
                lua=LuaRuntime(unpack_returned_tuples=True)
                events,ok,error,fields=lua.execute(HARNESS)(text,mode,cancel,fault)
                return list(events.values()),ok,'PRIMARY_CALLBACK_FAULT' in error,dict(fields.items())
            for mode in ('lecture','talk','hit'):
                for cancel in (1,3,6,10,20,80):
                    self.assertEqual(run(source,mode,cancel,''),run(output,mode,cancel,''))
            for mode,fault in (('lecture','health'),('hit','speak:TEXT_QST_048_GUARD_ON_HIT'),('talk','line')):
                self.assertEqual(run(source,mode,80,fault),run(output,mode,80,fault))

if __name__=='__main__':unittest.main()
