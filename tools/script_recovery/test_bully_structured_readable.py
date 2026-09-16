import tempfile
import unittest
from pathlib import Path
from tools.script_recovery.build_readable_new_oakvale import build
from tools.script_recovery.bully_main_structure import generate
from tools.script_recovery.test_bully_readable_integration import run


class BullyStructuredReadableTests(unittest.TestCase):
    def test_emitted_structure_and_behavior(self):
        with tempfile.TemporaryDirectory() as directory:
            report=build(Path(directory))
            output=(Path(directory)/'FSE/NewOakValeIntro/Entities/NOVI_Bully.lua').read_text()
            self.assertTrue(report['syntax']['ok'])
            for token in ('goto ','LAB_','TODO(native)','pCVar','fVar'):self.assertNotIn(token,output)
            self.assertIn('Quests = {}',(Path(directory)/'FSE/quests.lua').read_text())
            self.assertIn('BullyAcquirePrepared',output)
            self.assertIn('BullyRunoffControls',output)
            source,_=generate()
            for mode in ('normal','macro_error'):
                for cancel in (1,2,5,10,20,40,60,80,99,999):
                    with self.subTest(mode=mode,cancel=cancel):
                        self.assertEqual(run(source,mode,1.0,cancel,True),run(output,mode,1.0,cancel,True))
