import copy
import json
import unittest
from dataclasses import replace
from pathlib import Path
from unittest.mock import patch

from lupa.lua54 import LuaRuntime

from tools.script_recovery.lift_native_lua import (ROOT, CLUSTERS, RData, annotate,
    load_manifest, load_slots, load_thing_tables)
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_scythe_main import recover_scythe_main
from tools.script_recovery.scythe_converter import convert


class ScytheMainTests(unittest.TestCase):
    def inputs(self):
        cluster = json.loads((CLUSTERS / 'QS_ScytheInfo.json').read_text())
        fn = next(f for f in cluster['lifecycle'] if f['role'] == 'Main')
        manifest, slots, data = load_manifest(), load_slots(), RData()
        things, returning = load_thing_tables(manifest, slots)
        source = annotate(fn['decompile'], slots, things, returning, entity=False)
        return fn, source, data, manifest

    def test_main_behavior_and_every_termination_cleanup(self):
        report = convert(ROOT / 'work/scythe_converter/test-draft')
        self.assertFalse(report['registrationEnabled'])
        self.assertEqual(report['functions']['Main']['todo'], [])
        self.assertEqual(report['functions']['Init']['todo'], [])
        source = (ROOT / 'work/scythe_converter/test-draft/ScytheInfo.lua').read_text()
        for stop_at in (None, 1, 2, 3, 4, 5, 6):
            with self.subTest(stop_at=stop_at):
                lua, events = LuaRuntime(), []
                counts = {'frames': 0, 'termination': 0, 'quest': 0, 'timer': 0}
                state = {}
                def frame(_quest):
                    counts['frames'] += 1
                    if counts['frames'] == 3:
                        state['MissionSucceeded'] = True
                    self.assertLessEqual(counts['frames'], 3)
                    return True
                def terminating(_quest):
                    counts['termination'] += 1
                    return counts['termination'] == stop_at
                def quest_name(_quest):
                    counts['quest'] += 1
                    return 'active-' + str(counts['quest'])
                def get_timer(_quest, timer):
                    self.assertEqual(timer, 871)
                    counts['timer'] += 1
                    return 0 if counts['timer'] == 1 else 4
                def region(_quest, name):
                    self.assertEqual(name, 'NorthernWastes2')
                    return counts['frames'] < 2
                callbacks = {
                    'SetStateBool': lambda _q, name, value: state.__setitem__(name, value),
                    'GetStateBool': lambda _q, name: state[name],
                    'NewScriptFrame': frame, 'IsActiveThreadTerminating': terminating,
                    'GetActiveQuestName': quest_name, 'RegisterTimer': lambda _q: 871,
                    'GetTimer': get_timer, 'IsRegionLoaded': region,
                }
                for name in ('AddEntityBinding', 'FinalizeEntityBindings', 'SetQuestCardObjective',
                             'SetTimer', 'DeregisterTimer', 'HeroReceiveMessageFromGuildMaster',
                             'SetQuestAsCompleted', 'DeactivateQuestLater', 'FadeScreenIn'):
                    callbacks[name] = lambda _q, *args, name=name: events.append((name, *args))
                quest = lua.table_from(callbacks)
                lua.execute(source)
                lua.globals().Init(quest)
                self.assertEqual(state, {'MissionSucceeded': False})
                lua.globals().Main(quest)
                self.assertEqual(events[:4], [
                    ('AddEntityBinding', 'ScytheMarker', 'ScytheInfo/Entities/ScytheMarker'),
                    ('AddEntityBinding', 'ScytheNearOracle', 'ScytheInfo/Entities/ScytheNearOracle'),
                    ('FinalizeEntityBindings',), ('SetQuestCardObjective', 'active-1',
                     'TEXT_QUEST_AWAKEN_ORACLE_OBJECTIVE_03', '', 'NorthernWastes3')])
                self.assertEqual(events.count(('DeregisterTimer', 871)), 1)
                self.assertEqual(events[-1], ('DeregisterTimer', 871))
                reminders = [e for e in events if e[0] == 'HeroReceiveMessageFromGuildMaster']
                if stop_at not in (1, 2):
                    self.assertEqual(reminders, [('HeroReceiveMessageFromGuildMaster',
                        'TEXT_QST_B03_SCYTHE_RETURN_TO_SNOWSPIRE_REMINDER_10', '', True, True)])
                    self.assertEqual(events.count(('SetTimer', 871, 100)), 2)
                else:
                    self.assertEqual(reminders, [])
                completions = [e for e in events if e[0] in ('SetQuestAsCompleted', 'DeactivateQuestLater', 'FadeScreenIn')]
                self.assertEqual(completions, [] if stop_at else [
                    ('SetQuestAsCompleted', 'active-2', False, False, False),
                    ('DeactivateQuestLater', 'active-3', 0), ('FadeScreenIn',)])

    def test_changed_operand_target_or_receiver_rejects(self):
        fn, source, data, manifest = self.inputs()
        _, evidence = recover_scythe_main(fn, source, data, manifest)
        for call in evidence[0]['calls']:
            for changes in ({'ecx': ('constant', 0)}, {'target': ('constant', 0)},
                            {'stack_arguments': (('constant', 777),)}):
                def decode(*args, **kwargs):
                    setup = read_call_window(*args, **kwargs)
                    return replace(setup, **changes) if args[3] == call['site'] else setup
                with self.subTest(site=hex(call['site']), changes=changes), patch(
                        'tools.script_recovery.native_scythe_main.read_call_window', side_effect=decode):
                    result, status = recover_scythe_main(fn, source, data, manifest)
                    self.assertEqual(result, source)
                    self.assertEqual(status[0]['status'], 'rejected')

    def test_changed_native_cleanup_state_source_or_contract_rejects(self):
        fn, source, data, manifest = self.inputs()
        original = data.bytes_at
        for site in (0xE29CF9, 0xE29CFA, 0xE29DB9, 0xE29E61, 0xE29A72, 0xE2A5D9):
            def changed(address, size):
                raw = original(address, size)
                if raw is not None and address <= site < address + size:
                    raw = bytearray(raw)
                    raw[site - address] ^= 1
                    return bytes(raw)
                return raw
            with self.subTest(site=hex(site)), patch.object(data, 'bytes_at', side_effect=changed):
                self.assertEqual(recover_scythe_main(fn, source, data, manifest)[1][0]['status'], 'rejected')
        for altered_fn, altered_source in ((dict(fn, decompile=fn['decompile'] + '\n'), source),
                                           (fn, source + '\n')):
            self.assertEqual(recover_scythe_main(altered_fn, altered_source, data, manifest)[1][0]['status'], 'rejected')
        changed_manifest = copy.deepcopy(manifest)
        changed_manifest['DeactivateQuestLater']['parameters'].reverse()
        self.assertEqual(recover_scythe_main(fn, source, data, changed_manifest)[1][0]['status'], 'rejected')
        with patch('tools.script_recovery.native_scythe_main.check_single_resource_lifetime', return_value=False):
            self.assertEqual(recover_scythe_main(fn, source, data, manifest)[1][0]['status'], 'rejected')
        with patch.object(data, 'string_at', return_value='wrong'):
            self.assertEqual(recover_scythe_main(fn, source, data, manifest)[1][0]['status'], 'rejected')

    def test_output_cannot_overwrite_canonical_ports(self):
        with self.assertRaises(ValueError):
            convert(ROOT / 'refs/script_recovery/lifted/ScytheInfo')


if __name__ == '__main__':
    unittest.main()
