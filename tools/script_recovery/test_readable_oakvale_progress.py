import itertools
import tempfile
import unittest
from pathlib import Path
from tools.script_recovery.build_readable_new_oakvale import RAW,build
from tools.script_recovery.readable_oakvale_progress import lower
from tools.script_recovery.test_oakvale_progress import run
from tools.script_recovery.native_oakvale_progress import execute


class ReadableOakvaleProgressTests(unittest.TestCase):
    def test_emitted_objective_callers_match_native(self):
        with tempfile.TemporaryDirectory() as directory:
            report=build(Path(directory));source=(Path(directory)/'FSE/NewOakValeIntro/NewOakValeIntro.lua').read_text()
            self.assertTrue(report['syntax']['ok']);self.assertIsNotNone(report['progressHelpers'])
            for kind,initial,delay,cancel in itertools.product(('gold','attack'),(-1,2,3),(0,2),(1,3,99)):
                args=dict(initial=initial,delay=delay,cancel=cancel)
                self.assertEqual(run(kind,source=source,**args),execute(kind,**args),(kind,args))

    def test_changed_raw_helpers_rejected(self):
        source=(RAW/'FSE/NewOakValeIntro/NewOakValeIntro.lua').read_text()
        for before,after in (('SetTimeOfDay(23.0)','SetTimeOfDay(12.0)'),('if 2 < iVar2 then','if 3 < iVar2 then')):
            with self.assertRaisesRegex(ValueError,'Oakvale progress draft changed'):lower(source.replace(before,after))


if __name__=='__main__':unittest.main()
