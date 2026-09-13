"""Compare supported generated quest Main sequences with Aeon's unchanged sample scripts."""
import hashlib
import json
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

from lupa.lua54 import LuaRuntime
from tools.script_recovery import native_call_operands as recovery
from tools.script_recovery.lift_native_lua import ROOT, RETAIL_EXE, lift_cluster


AEON = ROOT / 'work/aeon_lua_ports'


class DeferredOperandEvidenceTests(unittest.TestCase):
    def test_verified_pair_and_rejection_paths(self):
        source = 'ppVar5 = GSI->GetActiveQuestName(&stack0xfffffff4,0);\nGSI->DeactivateQuestLater();'
        sha = hashlib.sha256(source.encode()).hexdigest()
        witness = {'id': 'test', 'functionAddress': '0x1000', 'decompileSha256': sha,
            'annotatedSha256': sha, 'nativeRegions': [{'name': 'caller', 'address': '0x1000',
                'size': 4, 'sha256': hashlib.sha256(b'body').hexdigest()}],
            'vtableSlots': [{'name': 'getter', 'address': '0x2000', 'target': '0x3000'}]}
        function = {'address': '0x1000', 'decompile': source}
        memory = {(0x1000, 4): b'body', (0x2000, 4): (0x3000).to_bytes(4, 'little')}
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / 'witness.json'
            path.write_text(json.dumps({'witnesses': [witness]}))
            with patch.object(recovery, 'WITNESSES', path):
                output, evidence = recovery.recover_reviewed_call_operands(function, source, lambda a, n: memory.get((a, n)))
                self.assertEqual(evidence[0]['status'], 'recovered')
                self.assertIn('GetActiveQuestName(&stack0xfffffff4);', output)
                self.assertIn('DeactivateQuestLater(ppVar5,0);', output)
                for key in memory:
                    for bad in (None, b'bad', b'xxxx'):
                        with self.subTest(key=key, bad=bad):
                            result, proof = recovery.recover_reviewed_call_operands(function, source,
                                lambda a, n: bad if (a, n) == key else memory.get((a, n)))
                            self.assertEqual(result, source)
                            self.assertEqual(proof[0]['status'], 'rejected')
                for raw, annotated in ((source + ' ', source), (source, source + ' ')):
                    result, proof = recovery.recover_reviewed_call_operands(
                        dict(function, decompile=raw), annotated, lambda a, n: memory.get((a, n)))
                    self.assertEqual(result, annotated)
                    self.assertEqual(proof[0]['status'], 'rejected')
                self.assertEqual(recovery.recover_reviewed_call_operands(
                    dict(function, address='0x9999'), source, lambda *_: None), (source, []))


@unittest.skipUnless(RETAIL_EXE.is_file() and (AEON / 'Fisherman').is_dir(), 'retail image and Aeon samples required')
class AeonMainParityTests(unittest.TestCase):
    def run_main(self, source, package, saved_at=0, unloaded_at=0, cancel=None):
        lua, events, frames = LuaRuntime(), [], [0]

        def frame(_quest):
            frames[0] += 1
            self.assertLess(frames[0], 12, 'quest did not advance')
            events.append(('frame',))
            return frames[0] != cancel

        def state(_quest, name):
            self.assertEqual(name, 'HeroHasSavedFishermanFromWasps')
            return frames[0] >= saved_at

        def region(_quest, name):
            self.assertEqual(name, 'Fisherman')
            return frames[0] < unloaded_at

        def deactivate(_quest, name, delay):
            # The current host requires an unsigned delay; nil is not a default argument.
            self.assertIsInstance(delay, int)
            events.append(('DeactivateQuestLater', name, delay))

        def active_name(_quest):
            events.append(('GetActiveQuestName',))
            return package

        quest = lua.table_from({'Log': lambda *_: None, 'NewScriptFrame': frame,
            'GetStateBool': state, 'IsRegionLoaded': region,
            'AddEntityBinding': lambda _q, *args: events.append(('AddEntityBinding', *args)),
            'FinalizeEntityBindings': lambda _q: events.append(('FinalizeEntityBindings',)),
            'ActivateQuest': lambda _q, name: events.append(('ActivateQuest', name)),
            'GetActiveQuestName': active_name, 'DeactivateQuestLater': deactivate,
            'SetQuestCardObjective': lambda _q, *args: events.append(('SetQuestCardObjective', *args))})
        lua.execute(source)
        lua.globals().Main(quest)
        return events

    def test_fisherman_transition_and_cancellation_match_sample(self):
        report = lift_cluster('V_Fisherman')
        self.assertEqual(report['functions']['Main']['operandEvidence'][0]['status'], 'recovered')
        port = (AEON / 'Fisherman/FSE/Fisherman/Fisherman.lua').read_text()
        for saved, unloaded, cancel in ((0, 0, None), (2, 4, None), (0, 3, None), (2, 4, 1), (2, 4, 3)):
            with self.subTest(saved=saved, unloaded=unloaded, cancel=cancel):
                actual = self.run_main(report['lua'], 'Fisherman', saved, unloaded, cancel)
                self.assertEqual(actual, self.run_main(port, 'Fisherman', saved, unloaded, cancel))
                if cancel is None:
                    self.assertEqual(actual[-3:], [('ActivateQuest', 'V_FishingCompetition'),
                        ('GetActiveQuestName',), ('DeactivateQuestLater', 'Fisherman', 0)])
                else:
                    self.assertFalse(any(e[0] in ('ActivateQuest', 'DeactivateQuestLater') for e in actual))

    def test_old_missing_delay_is_detected(self):
        source = lift_cluster('V_Fisherman')['lua']
        broken = source.replace('DeactivateQuestLater(ppVar5, 0)', 'DeactivateQuestLater(ppVar5, nil)')
        self.assertNotEqual(source, broken)
        with self.assertRaises(AssertionError):
            self.run_main(broken, 'Fisherman')

    def test_meet_sister_bindings_and_objective_match_sample(self):
        generated = lift_cluster('QS_MeetSister')['lua']
        port = (AEON / 'MeetSister/FSE/MeetSister/MeetSister.lua').read_text()
        actual = self.run_main(generated, 'MeetSister')
        self.assertEqual(actual, self.run_main(port, 'MeetSister'))
        self.assertEqual(actual[-1], ('SetQuestCardObjective', 'MeetSister',
            'TEXT_QUEST_THERESA_MOTHER_INFO_2_SUMMARY', 'HauntedHouse', 'BarrowFields'))


if __name__ == '__main__':
    unittest.main()
