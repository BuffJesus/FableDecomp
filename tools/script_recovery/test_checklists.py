"""Every autopilot checklist is well formed (2026-09-22).

A checklist is only read once the game is up and a run is minutes in, so a typo in a regex, a verb the input
driver does not know, or a duplicate step id costs a whole launch. These checks are static and cheap:

* the file parses and is a list of step objects;
* `expect` / `forbid` compile as regexes (`autopilot.run` searches the log with them);
* every `input:` line starts with a verb `send_input` implements, with the arity it expects;
* every channel line is `<QuestName>: <lua>` or one of the bare console commands;
* step ids are unique inside a file, and `repeat` / `interval` / `timeout` are numbers.
"""
import json
import re
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
CHECKLISTS = sorted((ROOT / 'tools/script_recovery/checklists').glob('*.json'))

# send_input's verbs (autopilot.py) and how many words each takes after the verb
INPUT_VERBS = {'key': None, 'capture': 1, 'wait': 1, 'hold': 2, 'chord': 2, 'click': 2, 'move': 2, 'clear': 0, 'clear_all': 0, 'skip': 0, 'focus': 0, 'lmb': (0, 1)}
# console commands the sidecar's exec channel takes without a quest prefix (docs/scripts/AUTOPILOT_DESIGN.md)
BARE_COMMANDS = {'list'}
BARE_PREFIXES = ('hero ', 'dump ')
CHANNEL_LINE = re.compile(r'^(?:eval )?[A-Za-z_]\w*: .+', re.S)


def _is_console_line(line):
    return line.strip() in BARE_COMMANDS or line.startswith(BARE_PREFIXES)


class ChecklistTests(unittest.TestCase):
    def test_sister_replay_preserves_natural_progression(self):
        steps = json.loads((ROOT / 'tools/script_recovery/checklists/guardian_sister_info.json').read_text())
        for step in steps:
            for line in step.get('do', []):
                self.assertNotRegex(line, r'ActivateQuest\(|SetQuestAsCompleted\(|SetMasterGameState\(|SetStateBool\(')
                if 'GoToMapSlotRetailTransition(' in line:
                    self.assertEqual(step.get('repeat', 1), 1)
                    self.assertIn('assert(not quest:IsInCutscene() and not quest:IsInMovieSequence()', line)

    def test_sister_handoff_is_read_only(self):
        steps = json.loads((ROOT / 'tools/script_recovery/checklists/guardian_sister_handoff.json').read_text())
        for step in steps:
            for line in step['do']:
                self.assertTrue(line.startswith('eval LUAGameflow: '))
                self.assertNotRegex(line, r'MsgOnQuestCompleted|quest:Set|quest:Activate')

    def test_orchard_replay_does_not_force_success_or_activation(self):
        steps = json.loads((ROOT / 'tools/script_recovery/checklists/orchard_farm_raid.json').read_text())
        for step in steps:
            for line in step.get('do', []):
                self.assertNotRegex(line, r'ActivateQuest\(|SetQuestAsCompleted\(|SetStateBool\(')
                if 'GoToMapSlotRetailTransition(' in line:
                    self.assertEqual(step.get('repeat', 1), 1)
                    self.assertIn('assert(not quest:IsInCutscene() and not quest:IsInMovieSequence()', line)

    def test_orchard_state_queries_use_shared_quest(self):
        steps = json.loads((ROOT / 'tools/script_recovery/checklists/orchard_farm_raid.json').read_text())
        queries = [line for step in steps for line in step.get('do', []) if ':GetState' in line]
        self.assertTrue(queries)
        for line in queries:
            self.assertTrue(line.startswith('eval OrchardFarmRaid: '), line)

    def test_orchard_handoff_cannot_force_or_consume_completion(self):
        steps = json.loads((ROOT / 'tools/script_recovery/checklists/orchard_farm_handoff.json').read_text())
        for step in steps:
            for line in step['do']:
                self.assertTrue(line.startswith('eval LUAGameflow: '))
                self.assertNotRegex(line, r'MsgOnQuestCompleted|quest:Set|quest:Activate')

    def test_orchard_combat_checks_do_not_click_unexpected_ui(self):
        steps = json.loads((ROOT / 'tools/script_recovery/checklists/orchard_farm_raid.json').read_text())
        for step in steps:
            if step['id'] in ('orchard_crates', 'orchard_outcome'):
                self.assertFalse(any(line.startswith('input:') for line in step['do']))

    def test_wasp_entry_requires_the_card_instead_of_activating_the_script(self):
        steps = json.loads((ROOT / 'tools/script_recovery/checklists/wasp_boss.json').read_text())
        actions = [line for step in steps for line in step.get('do', [])]
        self.assertFalse(any('ActivateQuest(' in line for line in actions))
        self.assertIn('wasp_card_prerequisite', [step['id'] for step in steps])
        self.assertTrue(any("IsQuestActive('Q_WaspBoss')" in line for line in actions))
        self.assertIn('manually', steps[0]['note'])

    def test_wasp_checklist_does_not_force_combat_outcomes(self):
        steps = json.loads((ROOT / 'tools/script_recovery/checklists/wasp_boss.json').read_text())
        for step in steps:
            for line in step.get('do', []):
                self.assertNotRegex(line, r"SetStateBool\(['\"](?:QueenHornetAttacks|MissionSucceeded)['\"]")

    def test_there_are_checklists(self):
        self.assertTrue(CHECKLISTS, 'no checklists found')

    def test_each_checklist_is_well_formed(self):
        for path in CHECKLISTS:
            with self.subTest(checklist=path.name):
                steps = json.loads(path.read_text(encoding='utf-8'))
                self.assertIsInstance(steps, list)
                ids = [s.get('id') for s in steps]
                self.assertEqual(len(ids), len(set(ids)), f'duplicate step id in {path.name}: {ids}')
                for step in steps:
                    self.assertIsInstance(step, dict)
                    self.assertIn('id', step)
                    for key in ('expect', 'forbid'):
                        if step.get(key) is not None:
                            re.compile(step[key])          # raises on a bad pattern
                    for key in ('repeat', 'interval', 'timeout'):
                        if key in step:
                            self.assertIsInstance(step[key], (int, float), (path.name, step['id'], key))
                    for line in step.get('do', []):
                        self.assertIsInstance(line, str)
                        if line.startswith('input:'):
                            parts = line[len('input:'):].split()
                            self.assertTrue(parts, f'{path.name}/{step["id"]}: empty input line')
                            verb, args = parts[0], parts[1:]
                            self.assertIn(verb, INPUT_VERBS, f'{path.name}/{step["id"]}: unknown input verb {verb!r}')
                            arity = INPUT_VERBS[verb]
                            if isinstance(arity, tuple):
                                self.assertIn(len(args), arity, f'{path.name}/{step["id"]}: {line!r}')
                            elif arity is not None:
                                self.assertEqual(len(args), arity, f'{path.name}/{step["id"]}: {line!r}')
                        else:
                            self.assertTrue(_is_console_line(line) or CHANNEL_LINE.match(line),
                                            f'{path.name}/{step["id"]}: not a channel line: {line!r}')
                            if line.startswith('eval '):
                                # Autopilot::RunIn wraps this in return tostring((...)).
                                # Supplying a return statement fails before a scene gate can run.
                                expression = line.split(':', 1)[1].strip()
                                self.assertFalse(re.match(r'return\b', expression),
                                                 f'{path.name}/{step["id"]}: eval takes an expression')

    def test_runtime_errors_are_forbidden_where_lua_runs(self):
        """A step that drives script code and does not forbid `LUA RUNTIME ERROR` can pass through a broken
        script: the log carries the error and the checklist never looks."""
        for path in CHECKLISTS:
            steps = json.loads(path.read_text(encoding='utf-8'))
            for step in steps:
                drives_lua = any(not l.startswith('input:') and not _is_console_line(l)
                                 for l in step.get('do', []))
                if step.get('expect') and drives_lua:
                    with self.subTest(checklist=path.name, step=step['id']):
                        self.assertIsNotNone(step.get('forbid'), 'no forbid pattern')
                        self.assertIn('LUA RUNTIME ERROR', step['forbid'])


if __name__ == '__main__':
    unittest.main()
