import unittest

from tools.script_recovery.analyze_cutscene_task_capture import analyze_text


class TaskCaptureTests(unittest.TestCase):
    def test_skip_query_and_command_order_are_preserved(self):
        report = analyze_text("""[CutsceneSkipDiag] begin macro=RUN manager=0x1
[CutsceneCommandDiag] macro=RUN tick=10 command=BULLY.DialogadSpeak BULLY,TEXT_GOING
[CutsceneCommandDiag] skipQueryTrue macro=RUN caller=0xcc0130 tick=20
[CutsceneCommandDiag] macro=RUN tick=21 command=FadeOut
[CutsceneCommandDiag] macro=OTHER tick=22 command=BULLY.RunTo MARKER,0
[CutsceneSkipDiag] end macro=RUN manager=0x1
""")
        run = report['runs'][0]
        self.assertEqual([c['command'] for c in run['commands']], ['BULLY.DialogadSpeak BULLY,TEXT_GOING', 'FadeOut'])
        self.assertEqual(run['skipQueries'][0]['tick'], 20)
        self.assertEqual(run['skipQueries'][0]['caller'], '0xcc0130')

    def test_foreign_polls_do_not_prove_bully_moved(self):
        report = analyze_text("""Actor identity 'BULLY': thing=0x1 resourceData=0xABC resourceInfo=0x2
[CutsceneSkipDiag] begin macro=RUN manager=0x1
[CutsceneMoveDiag] poll macro=RUN controller=0xdef active=1
[CutsceneMoveDiag] poll macro=RUN controller=0xdef active=0
[CutsceneSkipDiag] end macro=RUN manager=0x1
""")
        run = report['runs'][0]
        self.assertEqual(run['bullyBoundary']['controller'], '0xabc')
        self.assertEqual(run['bullyBoundary']['pollCount'], 0)
        self.assertEqual(run['controllers']['0xdef']['actorNames'], [])
        self.assertTrue(run['ended'])

    def test_repeated_runs_and_incomplete_capture_keep_distinct_identities(self):
        report = analyze_text("""Actor identity 'BULLY': resourceData=0xabc
[CutsceneSkipDiag] begin macro=RUN manager=0x1
[CutsceneMoveDiag] issueThing macro=RUN controller=0xabc target=0x2 radius=0
[CutsceneMoveDiag] poll macro=RUN controller=0xabc active=1
[CutsceneMoveDiag] poll macro=RUN controller=0xabc active=1
[CutsceneMoveDiag] poll macro=RUN controller=0xabc active=0
[CutsceneSkipDiag] end macro=RUN manager=0x1
Actor identity 'BULLY': resourceData=0xdef
[CutsceneSkipDiag] begin macro=RUN manager=0x1
[CutsceneMoveDiag] poll macro=RUN controller=0xdef active=1
""")
        first, second = report['runs']
        self.assertEqual(first['bullyBoundary']['issueCount'], 1)
        self.assertEqual(first['bullyBoundary']['pollCount'], 3)
        self.assertEqual([p['active'] for p in first['controllers']['0xabc']['transitions']], [1, 0])
        self.assertEqual(second['bullyBoundary']['controller'], '0xdef')
        self.assertFalse(second['ended'])


if __name__ == '__main__':
    unittest.main()
