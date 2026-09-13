"""Compare the external decoder with archived execution of actual retail code."""
import json
from pathlib import Path
import unittest
import struct

from tools.script_recovery.observe_retail_cutscene import (
    eligibility, snapshot_audio_samples, snapshot_audio_listener)


class AudioListenerTests(unittest.TestCase):
    def test_native_offsets_and_relocated_classes(self):
        delta = 0x120000
        core = bytearray(0x3C)
        struct.pack_into("<I", core, 0, 0x12B1E24 + delta)
        struct.pack_into("<I", core, 0x38, 0x3000)
        mixer = bytearray(0x2C)
        struct.pack_into("<I", mixer, 0, 0x12B2748 + delta)
        struct.pack_into("<fffffffff", mixer, 8, 10, 20, 30, 1, 0, 0, 0, 0, 1)
        memory = {0x2000: core, 0x3000: mixer, 0x3885: b'\x01'}
        reads = []
        def read(address, size):
            reads.append(address)
            return bytes(memory[address][:size])
        result = snapshot_audio_listener(read, 0x2000, delta)
        self.assertTrue(result["complete"])
        self.assertEqual(result["position"], (10, 20, 30))
        self.assertEqual(result["forward"], (1, 0, 0))
        self.assertEqual(result["up"], (0, 0, 1))
        self.assertEqual(result["update_gate_0x885"], 1)
        # Unknown mixer class must not trigger a speculative +885 read.
        reads.clear()
        struct.pack_into("<I", mixer, 0, 0)
        self.assertEqual(snapshot_audio_listener(read, 0x2000, delta)["partial_reason"],
                         "unknown_mixer_vtable")
        self.assertEqual(reads, [0x2000, 0x3000])

    def test_failed_read_stays_local(self):
        def read(address, size):
            raise OSError("object deleted")
        result = snapshot_audio_listener(read, 0x2000)
        self.assertFalse(result["complete"])
        self.assertEqual(result["partial_reason"], "object deleted")


class AudioSnapshotTests(unittest.TestCase):
    def setUp(self):
        self.memory = {}
        def block(address, size, words):
            raw = bytearray(size)
            for offset, value in words.items():
                struct.pack_into("<I", raw, offset, value)
            self.memory[address] = raw
        block(0x1000, 0x80, {0: 0x129D2C4, 0x4C: 0x2000})
        block(0x2000, 12, {0: 0x2100})
        block(0x2100, 12, {0: 0x2000, 8: 0x3000})
        block(0x3000, 0x20, {0: 0x129E6EC, 0x14: 0x4000, 0x18: 1})
        block(0x4000, 16, {0: 0x4100})
        block(0x4100, 16, {0: 0x4200, 8: 0x5000})
        block(0x4200, 16, {0: 0x4000, 8: 0x5100})
        block(0x5000, 0x24, {0: 0x129E67C, 8: 7, 0xC: 0x6000,
                             0x14: 0x101, 0x18: 123})
        block(0x5100, 0x24, {0: 0x129E67C, 8: 8, 0xC: 0x6000,
                             0x14: 1, 0x18: 456})

    def read(self, address, size):
        if address not in self.memory:
            raise OSError("unmapped")
        return bytes(self.memory[address][:size])

    def capture(self, **kwargs):
        return snapshot_audio_samples(self.read, 0x1000, clock=lambda: 0, **kwargs)

    def test_two_sample_flags_and_ids(self):
        result = self.capture()
        self.assertTrue(result["complete"])
        samples = result["emitters"][0]["samples"]
        self.assertEqual([(s["sample_id"], s["spatial"], s["sound_in_2d"])
                          for s in samples], [(123, 1, 1), (456, 1, 0)])
        self.assertEqual(result["sample_count"], 2)

    def test_cycle_is_partial(self):
        struct.pack_into("<I", self.memory[0x4200], 0, 0x4100)
        result = self.capture()
        self.assertFalse(result["complete"])
        self.assertEqual(result["partial_reason"], "list_cycle")
        self.assertEqual(result["sample_count"], 2)

    def test_deleted_payload_is_partial(self):
        del self.memory[0x5100]
        result = self.capture()
        self.assertEqual(result["partial_reason"], "unmapped")
        self.assertEqual(result["sample_count"], 1)

    def test_limits_and_deadline(self):
        for options, reason in [({"max_samples": 1}, "sample_limit"),
                                ({"max_emitters": 0}, "emitter_limit"),
                                ({"budget_seconds": 0}, "time_budget")]:
            self.assertEqual(self.capture(**options)["partial_reason"], reason)

    def test_unknown_class_is_not_followed(self):
        struct.pack_into("<I", self.memory[0x3000], 0, 0xDEADBEEF)
        self.assertEqual(self.capture()["partial_reason"], "unknown_emitter_vtable")


class NativeEligibilityTests(unittest.TestCase):
    def test_matches_native_execution(self):
        proof_path = Path(__file__).resolve().parents[2] / (
            "refs/script_recovery/new_oakvale_intro/runtime_evidence/"
            "retail-skip-gate-emulation-20260912.json")
        proof = json.loads(proof_path.read_text())
        self.assertEqual(proof["address"], "0x48de44")
        self.assertEqual(len(proof["results"]), 384)
        for case in proof["results"]:
            with self.subTest(case=case):
                gui = bytearray(0x298)
                for offset, key in [(0xF6, "inside"), (0xF8, "enabled"),
                                    (0xF7, "skipped"), (0x100, "timer"),
                                    (0x10D, "phase"), (0x94, "flags")]:
                    gui[offset] = case[key]
                result = eligibility(gui, case["busy"])
                self.assertEqual(result["predicate"], case["actual"])
                # Native EASkipCutScene has its own in-cutscene gate.
                self.assertEqual(result["event_eligible"],
                                 bool(case["inside"] and case["actual"]))


if __name__ == "__main__":
    unittest.main()
