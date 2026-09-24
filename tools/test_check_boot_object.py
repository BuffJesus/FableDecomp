"""The functional allowance must never become a general parity bypass."""
import unittest

from check_boot_object import CGAME_PLAY_RELOCATIONS, is_cgame_play_register_residue


class CGamePlayResidueTests(unittest.TestCase):
    def setUp(self):
        self.retail = bytearray(394)
        self.retail[0xEB:0xF0] = bytes.fromhex("8d4c240c51")
        self.built = bytearray(self.retail)
        self.built[0xEB:0xF0] = bytes.fromhex("8d54240c52")

    def accepted(self, address="00412f90", relocations=None):
        return is_cgame_play_register_residue(
            address, bytes(self.retail), bytes(self.built),
            CGAME_PLAY_RELOCATIONS if relocations is None else relocations)

    def test_known_temporary_register_change(self):
        self.assertTrue(self.accepted())

    def test_other_function_is_rejected(self):
        self.assertFalse(self.accepted("00412f91"))

    def test_any_additional_instruction_change_is_rejected(self):
        for offset in range(394):
            self.built[offset] ^= 1
            self.assertFalse(self.accepted(), hex(offset))
            self.built[offset] ^= 1

    def test_different_lengths_are_rejected(self):
        self.built.append(0)
        self.assertFalse(self.accepted())

    def test_relocation_drift_is_rejected(self):
        self.assertFalse(self.accepted(relocations=CGAME_PLAY_RELOCATIONS[:-1]))
        self.assertFalse(self.accepted(relocations=CGAME_PLAY_RELOCATIONS + [0xEC]))

    def test_changed_oracle_is_rejected(self):
        self.retail[0xEE] = 0x10
        self.built[0xEE] = 0x10
        self.assertFalse(self.accepted())


if __name__ == "__main__":
    unittest.main()
