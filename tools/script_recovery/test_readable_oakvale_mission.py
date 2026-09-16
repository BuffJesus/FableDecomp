import itertools
import tempfile
import unittest
from pathlib import Path
from tools.script_recovery.build_readable_new_oakvale import RAW,build
from tools.script_recovery.readable_oakvale_mission import lower
from tools.script_recovery.test_oakvale_mission_dispatcher import run
from tools.script_recovery.native_oakvale_mission_dispatcher import execute


class ReadableOakvaleMissionTests(unittest.TestCase):
    def test_emitted_mission_matches_original_control_flow(self):
        with tempfile.TemporaryDirectory() as directory:
            report=build(Path(directory));source=(Path(directory)/'FSE/NewOakValeIntro/NewOakValeIntro.lua').read_text()
            self.assertTrue(report['syntax']['ok']);self.assertIsNotNone(report['mission'])
            body=source[source.index('function DoMission('):source.index('function AttackStuff(')]
            self.assertNotIn('uVar8',body);self.assertNotIn('GetThingWithScriptName',body)
            for delay,initial,cancel,post in itertools.product((0,2),(False,True),(1,2,3,5,8,99),(False,True)):
                args=dict(region_delay=delay,attack_initial=initial,attack_after=4,cancel=cancel,post_cancel=post)
                self.assertEqual(run(source=source,**args),execute(**args),args)

    def test_changed_raw_mission_rejected(self):
        source=(RAW/'FSE/NewOakValeIntro/NewOakValeIntro.lua').read_text()
        with self.assertRaisesRegex(ValueError,'DoMission draft changed'):
            lower(source.replace('CacheMusicSet(46)','CacheMusicSet(45)'))


if __name__=='__main__':unittest.main()
