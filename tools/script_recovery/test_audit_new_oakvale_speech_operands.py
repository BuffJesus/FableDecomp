"""Discriminating checks for conservative native register-operand analysis."""
import unittest
import capstone
from tools.script_recovery.audit_new_oakvale_speech_operands import recover, transfer, BASE

class Image:
    def __init__(self,code):self.code=bytes.fromhex(code)
    def get_data(self,offset,count):return self.code[offset:offset+count]

class SpeechOperandAnalysisTests(unittest.TestCase):
    def setUp(self):
        self.cs=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32)
        self.cs.detail=True

    def decode(self,code):return next(self.cs.disasm(bytes.fromhex(code),BASE))

    def test_calls_preserve_only_callee_saved_constants(self):
        out=transfer(self.decode('e800000000'),{'eax':1,'ecx':2,'edx':3,'ebx':4,'ebp':0})
        self.assertEqual(out,{'ebx':4,'ebp':0})

    def test_partial_write_does_not_preserve_false_zero(self):
        self.assertEqual(transfer(self.decode('b301'),{'ebx':0})['ebx'],1)
        self.assertNotIn('ebx',transfer(self.decode('b301'),{}))

    def test_conflicting_branch_values_are_unknown(self):
        # xor ebx,ebx; jne join; mov ebx,1; join: push ebx; ret.
        _,_,states,_=recover(Image('31db7505bb0100000053c3'),self.cs,BASE)
        self.assertNotIn('ebx',states[BASE+9])

    def test_zero_definition_after_join_is_known(self):
        _,_,states,_=recover(Image('31db7505bb0100000031db53c3'),self.cs,BASE)
        self.assertEqual(states[BASE+11]['ebx'],0)

    def test_loop_with_preserved_register_converges(self):
        # xor ebp,ebp; call placeholder; test eax,eax; jne call; push ebp; ret.
        _,_,states,_=recover(Image('31ede80000000085c075f755c3'),self.cs,BASE)
        self.assertEqual(states[BASE+11]['ebp'],0)

    def test_unreviewed_indirect_jump_is_rejected(self):
        with self.assertRaisesRegex(ValueError,'Unreviewed indirect branch'):
            recover(Image('ffe0'),self.cs,BASE)

if __name__=='__main__':unittest.main()
