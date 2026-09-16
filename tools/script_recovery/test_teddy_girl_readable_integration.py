import itertools,tempfile,unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.build_readable_new_oakvale import build
from tools.script_recovery.teddy_girl_candidate import generate
from tools.script_recovery.test_teddy_girl_candidate import FIXTURE

class TeddyGirlReadableTests(unittest.TestCase):
    def test_entry_lifecycle_ledger_and_readable_body_behavior(self):
        with tempfile.TemporaryDirectory() as directory:
            report=build(Path(directory));relative='FSE/NewOakValeIntro/Entities/NOVI_TeddyGirl.lua'
            output=(Path(directory)/relative).read_text();source,candidate=generate()
            self.assertTrue(report['syntax']['ok']);self.assertFalse(report['teddyGirlCandidate']['enabled'])
            self.assertIn('Quests = {}',(Path(directory)/'FSE/quests.lua').read_text())
            self.assertEqual(output.count('quest:RegisterBoundConsciousCondition()'),1)
            self.assertNotIn('goto ',output);self.assertNotIn('[[missing]]',output);self.assertNotIn('_DAT_',output)
            rows={row['function']:row for row in report['functions'] if row['owner']=='NOVI_TeddyGirl'}
            for name,implementation in (('Main','resourceBody'),('Init','TeddyGirlInitialize'),('GivenTeddy','TeddyGirlGiven')):
                self.assertEqual(rows[name]['implementationFunction'],implementation)
            self.assertIsNotNone(rows['Main']['entryCondition']);self.assertIsNone(rows['Init']['entryCondition'])
            helpers=report['sourceMap'][relative]['structure']['nativeMainHelpers']
            self.assertIn('TeddyGirlDeparture',helpers);self.assertNotIn('TeddyGirlInitialize',helpers);self.assertNotIn('TeddyGirlGiven',helpers)
            def run(text,*args):
                lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(text+FIXTURE);return lua.globals().fixture(*args)
            for question,talk,hit,cancel,fault in itertools.product((False,True),(False,True),(False,True),(1,2,3,999),(None,'departure')):
                args=(question,talk,hit,None,fault,cancel)
                self.assertEqual(run(source,*args),run(output,*args))

if __name__=='__main__':unittest.main()
