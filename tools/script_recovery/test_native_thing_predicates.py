import copy
import hashlib
import json
import unittest

from tools.script_recovery.native_thing_predicates import WITNESSES, recover_reviewed_thing_predicates
from tools.script_recovery.lift_native_lua import (
    ROOT, RData, RETAIL_EXE, annotate, load_slots, load_manifest, load_thing_tables, lift_cluster,
)


def digest(text):
    return hashlib.sha256(text.encode()).hexdigest()


class PredicateWitnessTests(unittest.TestCase):
    def setUp(self):
        self.source = ("{\nchar cVar4;\nGSI->GetThingWithScriptName();\n"
                       "while ((piStack_10 != (int *)0x0 && "
                       "(cVar4 = (**(code **)(*piStack_10 + 300))(), cVar4 != '\\0'))) {\n}\n}")
        self.function = {"address": "0x1000", "decompile": self.source}
        self.memory = {(0x1000, 4): b"body", (0x2000, 4): (0x3000).to_bytes(4, "little")}
        self.witness = {
            "id": "test", "functionAddress": "0x1000", "decompileSha256": digest(self.source),
            "annotatedSha256": digest(self.source), "association": {"wrapperOffset": 8},
            "nativeRegions": [{"name": "caller", "address": "0x1000", "size": 4,
                               "sha256": hashlib.sha256(b"body").hexdigest()}],
            "vtableSlots": [{"name": "lookup", "address": "0x2000", "target": "0x3000"}],
        }

    def recover(self, source=None):
        return recover_reviewed_thing_predicates(
            self.function, self.source if source is None else source,
            lambda address, size: self.memory.get((address, size)), [self.witness])

    def test_records_proven_receiver_and_null_safe_predicate(self):
        output, evidence = self.recover()
        self.assertIn("__native_thing_predicate_1 = GSI->GetThingWithScriptName();", output)
        self.assertIn("while (__native_thing_predicate_1 && CScriptThing::IsAlive(__native_thing_predicate_1))", output)
        self.assertEqual(evidence[0]["status"], "recovered")
        self.assertEqual(evidence[0]["nativeRegions"], self.witness["nativeRegions"])

    def test_native_mismatch_or_absence_never_rewrites(self):
        for key in self.memory:
            for value in (None, b"bad", b"xxxx"):
                with self.subTest(key=key, value=value):
                    original = self.memory[key]
                    self.memory[key] = value
                    output, evidence = self.recover()
                    self.memory[key] = original
                    self.assertEqual(output, self.source)
                    self.assertEqual(evidence[0]["status"], "rejected")

    def test_source_or_annotation_drift_never_rewrites(self):
        output, evidence = self.recover(self.source.replace("300", "304"))
        self.assertEqual(evidence[0]["status"], "rejected")
        self.assertIn("304", output)
        self.function["decompile"] += "\n"
        output, evidence = self.recover()
        self.assertEqual(output, self.source)
        self.assertEqual(evidence[0]["status"], "rejected")

    def test_even_reviewed_source_requires_dead_result_and_unique_shape(self):
        for altered in (
            self.source + "\nconsume(cVar4);",
            self.source + "\nGSI->GetThingWithScriptName();",
            self.source.replace("300", "304"),
            self.source.replace("piStack_10 +", "piStack_14 +"),
        ):
            with self.subTest(source=altered):
                self.witness["annotatedSha256"] = digest(altered)
                output, evidence = self.recover(altered)
                self.assertEqual(output, altered)
                self.assertEqual(evidence[0]["status"], "rejected")

    def test_receiver_name_avoids_source_collision(self):
        source = self.source + "\nint __native_thing_predicate_1;"
        self.witness["annotatedSha256"] = digest(source)
        _, evidence = self.recover(source)
        self.assertEqual(evidence[0]["receiver"], "__native_thing_predicate_1_")

    def test_negative_shape_retains_live_assignment_and_lookup_operands(self):
        source = ("{\nGSI->GetNearestWithScriptName(buffer,hero);\n"
                  "if ((piStack_14 == (int *)0x0) || "
                  "(cVar1 = (**(code **)(*piStack_14 + 300))(), cVar1 == '\\0')) {\n}\n"
                  "consume(cVar1);\n}")
        self.witness.update(shape='if_not_alive', lookupMethod='GetNearestWithScriptName',
                            annotatedSha256=digest(source))
        output, evidence = self.recover(source)
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertIn('= GSI->GetNearestWithScriptName(buffer,hero);', output)
        self.assertIn("(cVar1 = CScriptThing::IsAlive(__native_thing_predicate_1), cVar1 == '\\0')", output)
        self.assertIn('consume(cVar1);', output)

    def test_unknown_shape_or_lookup_is_rejected(self):
        for key, value in (('shape', 'if_alive'), ('lookupMethod', 'GetUnknownThing')):
            with self.subTest(key=key):
                self.witness[key] = value
                output, evidence = self.recover()
                self.assertEqual(output, self.source)
                self.assertEqual(evidence[0]['status'], 'rejected')
                del self.witness[key]

    def test_unknown_or_ambiguous_function_has_no_guessed_recovery(self):
        for address in ("", "n/a", "0x1001"):
            self.function["address"] = address
            self.assertEqual(self.recover(), (self.source, []))
        self.function["address"] = "0x1000"
        output, evidence = recover_reviewed_thing_predicates(
            self.function, self.source, lambda *_: None, [self.witness, copy.deepcopy(self.witness)])
        self.assertEqual(output, self.source)
        self.assertEqual(evidence[0]["status"], "rejected")

    def test_byte_reader_rejects_virtual_padding_crossing_and_truncation(self):
        reader = object.__new__(RData)
        reader.ok, reader.base = True, 0x400000
        reader.raw_secs, reader.data = [(0x1000, 4, 0)], b"abcd"
        self.assertEqual(reader.bytes_at(0x401001, 2), b"bc")
        for address, size in ((0x401003, 2), (0x401004, 1), (0x400fff, 1), (0x401000, 0)):
            self.assertIsNone(reader.bytes_at(address, size))
        reader.data = b"ab"
        self.assertIsNone(reader.bytes_at(0x401000, 4))


@unittest.skipUnless(RETAIL_EXE.is_file(), "installed retail executable required for native witness")
class GuardianPredicateIntegrationTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.report = lift_cluster("QS_GuardianTrophyDealerInfo")

    def test_actual_export_and_installed_bytes_match_reviewed_witness(self):
        proof = self.report["functions"]["Thread:WaitForPieceOver"]["thingPredicateEvidence"]
        self.assertEqual(proof[0]["status"], "recovered")
        self.assertEqual(proof[0]["association"]["lookupCall"], "0x00E27B1C")
        fn = json.loads((ROOT / "refs/script_recovery/native_threads/QS_GuardianTrophyDealerInfo.json")
                        .read_text(encoding="utf-8-sig"))["functions"][0]
        slots = load_slots()
        things, returning = load_thing_tables(load_manifest(), slots)
        source = annotate(fn["decompile"], slots, things, returning, entity=False)
        unchanged, evidence = recover_reviewed_thing_predicates(fn, source, lambda *_: None)
        self.assertEqual(unchanged, source)
        self.assertEqual(evidence[0]["status"], "rejected")

    def test_actual_thread_waits_for_piece_and_actor_and_honors_termination(self):
        from lupa.lua54 import LuaRuntime

        # (frames before PieceOver, live actor checks, termination frame, absent actor)
        for piece_frames, live_checks, terminate, absent in (
            (0, 0, None, False), (0, 0, None, True), (2, 2, None, False),
            (2, 2, 1, False), (0, 2, 1, False), (2, 2, 3, False),
        ):
            with self.subTest(piece_frames=piece_frames, live_checks=live_checks,
                              terminate=terminate, absent=absent):
                lua = LuaRuntime()
                events, frames, checks = [], [0], [0]

                def frame(_quest):
                    frames[0] += 1
                    self.assertLess(frames[0], 10, "loop failed to recheck its predicate")
                    events.append("frame")
                    return frames[0] != terminate

                def is_alive(_thing):
                    checks[0] += 1
                    events.append("IsAlive")
                    return checks[0] <= live_checks

                actor = None if absent else lua.table_from({"IsAlive": is_alive})

                def lookup(_quest, name):
                    self.assertEqual(name, "GTDI_Maze")
                    self.assertGreaterEqual(frames[0], piece_frames)
                    events.append("lookup")
                    return actor

                quest = lua.table_from({
                    "GetStateBool": lambda _q, name: frames[0] >= piece_frames,
                    "NewScriptFrame": frame, "GetThingWithScriptName": lookup,
                    "GetActiveQuestName": lambda _q: "GuardianTrophyDealerInfo",
                    "DeactivateQuestLater": lambda _q, name, delay: events.append((name, delay)),
                })
                lua.execute(self.report["lua"])
                lua.globals().WaitForPieceOver(quest)
                if terminate is None:
                    self.assertEqual(events[-1], ("GuardianTrophyDealerInfo", 0))
                    self.assertEqual(frames[0], piece_frames + (0 if absent else live_checks))
                    self.assertEqual(checks[0], 0 if absent else live_checks + 1)
                else:
                    self.assertEqual(frames[0], terminate)
                    self.assertNotIn(("GuardianTrophyDealerInfo", 0), events)
                self.assertEqual(events.count("lookup"), int(terminate is None or terminate > piece_frames))


@unittest.skipUnless(RETAIL_EXE.is_file(), "installed retail executable required for native witness")
class BeardyPredicateIntegrationTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        from tools.script_recovery.lift_native_lua import CLUSTERS

        cluster = json.loads((CLUSTERS / 'V_BeardyBaldy.json').read_text(encoding='utf-8-sig'))
        cls.function = next(f for f in cluster['lifecycle'] if f['role'] == 'Main')
        slots = load_slots()
        cls.things, returning = load_thing_tables(load_manifest(), slots)
        cls.annotated = annotate(cls.function['decompile'], slots, cls.things, returning, entity=False)

    def test_main_pipeline_emits_verified_negative_condition(self):
        report = lift_cluster('V_BeardyBaldy')
        proof = report['functions']['Main']['thingPredicateEvidence'][0]
        self.assertEqual(proof['status'], 'recovered')
        self.assertEqual(proof['association']['lookupCall'], '0x00E4FFDA')
        self.assertNotIn('(*piStack_14 + 300)', report['lua'])

    def test_changed_barber_call_window_is_rejected(self):
        reader = RData()

        def changed(address, size):
            data = reader.bytes_at(address, size)
            if address <= 0xE4FFDA < address + size and data is not None:
                data = bytearray(data)
                data[0xE4FFDA - address] ^= 1
                return bytes(data)
            return data

        output, evidence = recover_reviewed_thing_predicates(self.function, self.annotated, changed)
        self.assertEqual(output, self.annotated)
        self.assertEqual(evidence[0]['status'], 'rejected')

    def test_actual_lookup_and_condition_use_barber_and_preserve_skipped_flag(self):
        from lupa.lua54 import LuaRuntime
        from tools.script_recovery.lift_native_lua import Lifter, thing_signatures

        recovered, evidence = recover_reviewed_thing_predicates(self.function, self.annotated, RData().bytes_at)
        self.assertEqual(evidence[0]['status'], 'recovered')
        end = recovered.index('{', recovered.index('if ((!__native_thing_predicate_1)')) + 1
        start = recovered.rindex('CCharString::CCharString(local_20,"BB_BeardyBaldyMan",-1);', 0, end)
        # Execute the actual recovered lookup/guard slice; the rest of Main still has unsupported
        # native control flow. Instrument only the guard body and subsequent flag observation.
        source = ('{\ncVar1 = true;\n' + recovered[start:end] + '\nGSI->SetTimer(42,10);\n}\n'
                  'GSI->SetTimer(42,cVar1);\n}')
        lifter = Lifter(load_manifest(), {}, 'Quest', False, 'BeardyBaldy', RData(),
                        thing_sigs=thing_signatures(self.things))
        body = '\n'.join(lifter.lift('Main', source))
        for barber_alive in (None, False, True):
            with self.subTest(barber_alive=barber_alive):
                lua = LuaRuntime()
                events, results = [], []
                hero = object()
                beardy = object()  # No IsAlive method: selecting Beardy would fail this test.

                def alive(_actor):
                    events.append('barber:IsAlive')
                    return barber_alive

                barber = None if barber_alive is None else lua.table_from({'IsAlive': alive})

                def lookup(_quest, name):
                    self.assertEqual(name, 'BB_BeardyBaldyMan')
                    events.append('Beardy')
                    return beardy

                def nearest(_quest, origin, name):
                    self.assertIs(origin, hero)
                    self.assertEqual(name, 'CREATURE_BOWERSTONE_SLUMS_VILLAGER_FEMALE_BARBER')
                    events.append('barber')
                    return barber

                quest = lua.table_from({'GetThingWithScriptName': lookup,
                    'GetHero': lambda _q: hero, 'GetNearestWithScriptName': nearest,
                    'SetTimer': lambda _q, _timer, value: results.append(value)})
                lua.execute('return function(Quest)\n' + body + '\nend')(quest)
                self.assertEqual(events, ['Beardy', 'barber'] + ([] if barber_alive is None else ['barber:IsAlive']))
                self.assertEqual(results[:-1], [] if barber_alive else [10])
                self.assertIs(results[-1], True if barber_alive is None else barber_alive)


if __name__ == "__main__":
    unittest.main()
