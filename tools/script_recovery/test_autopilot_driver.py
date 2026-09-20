"""The autopilot driver's file protocol against a fake sidecar (no game).

A thread plays the DLL side of FableScriptExtender/Autopilot.h: it consumes `autopilot/commands.txt`, answers on
the log with `[Autopilot] ...` lines (`batch <id>`, per-line results, `processed N line(s)`), and can be told to
emit a gameplay marker after a command so the checklist runner's expect / forbid / repeat logic is exercised.
"""
import json
import re
import tempfile
import threading
import time
import unittest
from pathlib import Path

from tools.script_recovery import autopilot


class FakeSidecar(threading.Thread):
    def __init__(self, bundle: Path, markers: dict[str, list[str]]):
        super().__init__(daemon=True)
        self.commands = bundle / 'NoviCompatibility' / 'autopilot' / 'commands.txt'
        self.log = bundle / 'NoviCompatibility' / 'FableScriptExtender.log'
        self.markers = markers          # command substring -> log lines to emit after it (popped one list entry per send)
        self.stop = threading.Event()
        self.seen: list[str] = []

    def emit(self, line: str) -> None:
        with self.log.open('a', encoding='utf-8') as f:
            f.write(line + '\n')

    def run(self) -> None:
        while not self.stop.is_set():
            if self.commands.is_file():
                lines = self.commands.read_text(encoding='utf-8').splitlines()
                self.commands.unlink()
                n = 0
                for line in lines:
                    line = line.strip()
                    if not line:
                        continue
                    n += 1
                    self.seen.append(line)
                    if line.startswith('batch '):
                        self.emit('[Autopilot] batch ' + line[6:])
                    elif line == 'list':
                        self.emit('[Autopilot] host GuildTrainingPreMelee/GuildTrainingPreMelee')
                        self.emit('[Autopilot] list: 1 hosts')
                    elif line.startswith('dump '):
                        self.emit('[Autopilot] state GuildTrainingPreMelee/GuildTrainingPreMelee:DummyHits = 7')
                        self.emit('[Autopilot] dump GuildTrainingPreMelee/GuildTrainingPreMelee: 1 entries')
                    elif 'error(' in line:
                        self.emit('[Autopilot] error ' + line + ': boom')
                    else:
                        self.emit('[Autopilot] ok ' + line)
                        for key, queue in self.markers.items():
                            if key in line and queue:
                                self.emit(queue.pop(0))
                self.emit(f'[Autopilot] processed {n} line(s)')
            time.sleep(0.05)


class AutopilotDriverTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.bundle = Path(self.tmp.name) / 'local-candidate-fake'
        (self.bundle / 'NoviCompatibility' / 'autopilot').mkdir(parents=True)
        (self.bundle / 'NoviCompatibility' / 'FableScriptExtender.log').write_text('--- boot ---\n', encoding='utf-8')
        self._orig_work = autopilot.WORK
        autopilot.WORK = Path(self.tmp.name)

    def tearDown(self):
        autopilot.WORK = self._orig_work
        self.tmp.cleanup()

    def test_send_waits_for_the_processed_marker(self):
        side = FakeSidecar(self.bundle, {}); side.start()
        try:
            out = autopilot.Channel('fake').send(['list', "Q_GuildTrainingPreMelee: quest:SetStateInt('DummyHits', 7)"], timeout=5)
        finally:
            side.stop.set()
        self.assertEqual(out[0], '[Autopilot] batch 1')
        self.assertIn('[Autopilot] list: 1 hosts', out)
        self.assertTrue(out[-1].startswith('[Autopilot] processed 3 line'), out)
        self.assertFalse(side.commands.exists(), 'the sidecar consumes the file')

    def test_checklist_pass_fail_and_repeat(self):
        side = FakeSidecar(self.bundle, {
            'DummyHits': ["[CutsceneCommandDiag] macro=<none> command=TEACHER.Speak TEACHER,'TEXT_CS_028_PREMELEE_STICK_10'"],
            'GuildScorpions': ['', '', "    [C++] SetQuestAsCompleted: ENTER name='Q_GuildTrainingWoodsMelee' flags=1,0,0."],
            'MK_GTM_WD_GUARD': ["[CutsceneCommandDiag] command=TEACHER.Speak TEACHER,'TEXT_CS_028_PREMELEE_PUNCH_10'"],
        })
        side.start()
        steps = [
            {'id': 'channel', 'do': ['list'], 'expect': r'\[Autopilot\] list: \d+ hosts', 'timeout': 5},
            {'id': 'punch_7', 'do': ["Q_GuildTrainingPreMelee: quest:SetStateInt('DummyHits', 7)"], 'expect': 'PREMELEE_STICK', 'forbid': 'LUA RUNTIME ERROR', 'timeout': 5},
            {'id': 'beetles', 'repeat': 5, 'interval': 0.2, 'do': ["Q_GuildTrainingWoodsMelee: for _, t in ipairs(quest:GetAllThingsWithScriptName('GuildScorpions')) do quest:SetThingAsKilled(t) end"],
             'expect': "SetQuestAsCompleted: ENTER name='Q_GuildTrainingWoodsMelee'", 'timeout': 5},
            {'id': 'split_yes', 'do': ['input: Enter'], 'expect': 'PlayAVIMovie', 'timeout': 1},
            {'id': 'woods_return', 'do': ['hero teleport MK_GTM_WD_GUARD'], 'expect': 'CS_GUILD_MELEE_WOODSWON', 'forbid': 'PREMELEE_PUNCH_10', 'timeout': 5},
            {'id': 'never', 'do': ['list'], 'expect': 'nope', 'timeout': 1},
        ]
        try:
            results = autopilot.run_checklist('fake', steps)
        finally:
            side.stop.set()
        status = {r['id']: r['status'] for r in results}
        self.assertEqual(status, {'channel': 'PASS', 'punch_7': 'PASS', 'beetles': 'PASS', 'split_yes': 'SKIPPED', 'woods_return': 'FAIL'})
        self.assertIn('after 3 send(s)', next(r['detail'] for r in results if r['id'] == 'beetles'))
        self.assertIn('forbidden marker', next(r['detail'] for r in results if r['id'] == 'woods_return'))
        # the failure dumped every live host before stopping; the step after the failure never ran
        self.assertIn('dump GuildTrainingPreMelee/GuildTrainingPreMelee', side.seen)
        self.assertNotIn('never', status)

    def test_channel_error_fails_the_step(self):
        side = FakeSidecar(self.bundle, {}); side.start()
        try:
            results = autopilot.run_checklist('fake', [{'id': 'bad', 'do': ["Q_X: error('x')"], 'expect': 'anything', 'timeout': 2}])
        finally:
            side.stop.set()
        self.assertEqual(results[0]['status'], 'FAIL')
        self.assertIn('channel:', results[0]['detail'])

    def test_shipped_checklist_parses(self):
        steps = json.loads((Path(__file__).parent / 'checklists' / 'guild_woods_return.json').read_text(encoding='utf-8'))
        for s in steps:
            re.compile(s['expect'])
            if s.get('forbid'):
                re.compile(s['forbid'])


if __name__ == '__main__':
    unittest.main()
