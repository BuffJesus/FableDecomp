import unittest
from tools.decomp_pipeline.near_match import compare, normalized_instructions


class SimilarityTests(unittest.TestCase):
    def test_register_and_stack_allocation(self):
        # mov eax,[esp+4]; add eax,1; ret vs ecx and a different stack slot
        score = compare(bytes.fromhex('8b4c240883c101c3'), bytes.fromhex('8b44240483c001c3'))
        self.assertTrue(score.near_match)
        self.assertEqual(score.opcode_similarity, 1)

    def test_constants_member_offsets_and_branch_conditions_preserved(self):
        for retail, built in [('83c001c3', '83c002c3'), ('8b4104c3', '8b4110c3'), ('7400c3', '7500c3')]:
            self.assertFalse(compare(bytes.fromhex(built), bytes.fromhex(retail)).near_match)

    def test_omitted_frame_ebp_is_an_object_register(self):
        # mov ecx,[ebp+4] vs mov eax,[esi+4]: the same member access.
        self.assertEqual(compare(bytes.fromhex('8b4d04c3'), bytes.fromhex('8b4604c3')).opcode_similarity, 1)
        # Object member offsets must remain significant, including EBP.
        self.assertFalse(compare(bytes.fromhex('8b4d04c3'), bytes.fromhex('8b4d10c3')).near_match)

    def test_real_ebp_frame_masks_stack_slots(self):
        # push ebp; mov ebp,esp; mov eax,[ebp+slot]; pop ebp; ret
        self.assertEqual(compare(bytes.fromhex('558bec8b45085dc3'), bytes.fromhex('558bec8b45105dc3')).opcode_similarity, 1)

    def test_ebp_object_before_setup_or_after_reassignment_is_not_masked(self):
        for prefix in ['8b4504', '558bec8b29']:
            # Retain member offsets when EBP's frame meaning is ambiguous.
            if prefix == '8b4504':
                a='8b4504558becc3';b='8b4510558becc3'
            else:
                a=prefix+'8b4504c3';b=prefix+'8b4510c3'
            self.assertFalse(compare(bytes.fromhex(a), bytes.fromhex(b)).near_match)

    def test_relocations_and_call_destinations(self):
        self.assertTrue(compare(bytes.fromhex('b800000000c3'), bytes.fromhex('b834124000c3'), [1], [1]).near_match)
        self.assertTrue(compare(bytes.fromhex('e800000000c3'), bytes.fromhex('e8ff000000c3')).near_match)

    def test_incomplete_and_empty_decode_fail_closed(self):
        for code in [b'', bytes.fromhex('c30f')]:
            self.assertFalse(compare(code, code).near_match)

    def test_count_threshold_inclusive(self):
        self.assertTrue(compare(b'\x90'*11, b'\x90'*10).near_match)
        self.assertFalse(compare(b'\x90'*12, b'\x90'*10).near_match)

    def test_width_and_order_are_not_erased(self):
        self.assertNotEqual(normalized_instructions(bytes.fromhex('8b01'))[0], normalized_instructions(bytes.fromhex('8a01'))[0])
        self.assertFalse(compare(bytes.fromhex('5bc350'), bytes.fromhex('505bc3')).near_match)


if __name__ == '__main__':
    unittest.main()
