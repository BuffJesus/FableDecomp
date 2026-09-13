import unittest
from types import SimpleNamespace

from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_entry_timer as entry
from tools.script_recovery import test_native_barrel_update as fixtures
from tools.script_recovery.native_barrel_update import recover_barrel_update
from tools.script_recovery.native_barrel_creation import recover_barrel_creation
from tools.script_recovery.lift_native_lua import ROOT, Lifter, RData


class BarrelCreationTests(unittest.TestCase):
    def generated(self):
        fn, source, manifest = entry.EntryTimerTests().inputs()
        lifter = Lifter(manifest, {'0x108': ('WatchTimer', 'Int'), '0x60': ('GUIBarrelCounter', 'Int'),
                                  '0x73': ('BarrelManSpokenToHeroOnReturn', 'Bool')},
                        'quest', False, '', RData(), live_termination=True, native_gotos=True)
        body = '\n'.join(lifter.lift('StartBarrelTimer', source, native_function=fn))
        self.assertEqual(lifter.barrel_creation_evidence[0]['status'], 'recovered')
        self.assertEqual(lifter.todo, [])
        return body

    def run_scenario(self, body, scenario, port=False):
        lua = LuaRuntime()
        if port:
            fse = ROOT / 'refs/script_recovery/reconstructed/NewOakValeIntro/FSE'
            lua.globals().root = fse.as_posix()
            lua.execute('package.path = root .. "/?.lua;" .. package.path')
            lua.execute((fse / 'NewOakValeIntro/NewOakValeIntro.lua').read_text())
            function = lua.globals().StartBarrelTimer
        else:
            function = lua.execute('return function(quest)\n' + body + '\nend')
        events, state = [], {'WatchTimer': 9, 'GUIBarrelCounter': 0, 'BarrelManSpokenToHeroOnReturn': False}
        frames, active_frames = [0], [0]
        terminating = [scenario == 'before_creation']
        def timer(_q, timer_id):
            self.assertEqual(timer_id, 9)
            if frames[0] == 0 and scenario in ('wait_then_complete', 'terminate_wait'):
                return 0
            return 45 - active_frames[0]
        def frame(_q):
            frames[0] += 1
            self.assertLess(frames[0], 6, 'generated wait failed to finish')
            events.append(('frame',))
            if state['GUIBarrelCounter']:
                active_frames[0] += 1
                if active_frames[0] == 2:
                    state['BarrelManSpokenToHeroOnReturn'] = True
            if scenario in ('terminate_wait', 'terminate_active'):
                terminating[0] = True
            return not terminating[0]
        def colour(table):
            return tuple(table[k] for k in ('r', 'g', 'b', 'a'))
        def add(_q, current, maximum, c1, c2, texture, text, scale):
            events.append(('add', current, maximum, colour(c1), colour(c2), texture, text, scale))
            return 71
        def set_int(_q, key, value):
            state[key] = value
            events.append(('state', key, value))
        def lookup(_q, key):
            events.append(('lookup', key))
            return 'guard-point'
        def distance(_q, first, second, threshold):
            self.assertEqual({first, second}, {'hero', 'guard-point'})
            self.assertEqual(threshold, 2.0)
            return active_frames[0] == 1
        quest = lua.table_from({
            'GetStateInt': lambda _q, key: state[key], 'GetStateBool': lambda _q, key: state[key],
            'SetStateInt': set_int, 'GetTimer': timer, 'NewScriptFrame': frame,
            'IsActiveThreadTerminating': lambda _q: terminating[0], 'AddQuestInfoBar': add,
            'GetThingWithScriptName': lookup, 'GetHero': lambda _q: 'hero',
            'IsDistanceBetweenThingsUnder': distance,
            'ChangeQuestInfoBarColour': lambda _q, bar, c1, c2: events.append(('colour', bar, colour(c1), colour(c2))),
            'UpdateQuestInfoBar': lambda _q, *args: events.append(('update', *args)),
            'RemoveQuestInfoElement': lambda _q, bar: events.append(('remove', bar)), 'Log': lambda *_: None,
        })
        function(quest)
        return events

    def test_whole_generated_timer_matches_port_effects_and_exit_paths(self):
        body = self.generated()
        for scenario in ('complete', 'wait_then_complete', 'terminate_wait', 'terminate_active'):
            with self.subTest(scenario=scenario):
                events = self.run_scenario(body, scenario)
                self.assertEqual(events, self.run_scenario(body, scenario, port=True))
                if scenario == 'terminate_wait':
                    self.assertEqual(events, [('frame',)])
                elif scenario == 'terminate_active':
                    self.assertEqual(events[-1], ('frame',))
                    self.assertNotIn(('remove', 71), events)
                else:
                    self.assertEqual(events[-1], ('remove', 71))
                    self.assertEqual([e for e in events if e[0] == 'update'],
                                     [('update', 71, 44, -1.0, -1.0), ('update', 71, 43, -1.0, -1.0)])
                    green, red = (0, 255, 0, 255), (255, 0, 0, 255)
                    self.assertIn(('add', 45.0, 0.0, green, green, 'HUD_CLOCK_ICON', '', 1.0), events)
                    self.assertEqual([e for e in events if e[0] == 'colour'],
                                     [('colour', 71, green, green), ('colour', 71, red, red)])
        # Retail checks termination after the initial positive timer query;
        # the reference port's predicate helper does not make that extra query.
        self.assertEqual(self.run_scenario(body, 'before_creation'), [])

    def test_changed_native_strings_body_or_contract_rejects(self):
        fn, source, manifest = fixtures.BarrelUpdateTests().inputs()
        source = recover_barrel_update(fn, source, RData(), manifest)[0]
        data = RData()
        for changed in (
                SimpleNamespace(bytes_at=lambda *_: None, string_at=data.string_at),
                SimpleNamespace(bytes_at=data.bytes_at, string_at=lambda *_: 'WRONG'),
                SimpleNamespace(bytes_at=lambda address, size: b'X' if address == 0x122d70e else data.bytes_at(address, size), string_at=data.string_at)):
            self.assertEqual(recover_barrel_creation(fn, source, changed, manifest)[1][0]['status'], 'rejected')
        self.assertEqual(recover_barrel_creation(fn, source, data, {})[1][0]['status'], 'rejected')
