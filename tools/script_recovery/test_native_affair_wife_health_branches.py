import math
import struct
import unittest

from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import UC_X86_REG_EBX
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_affair_wife_resource_scope import verify
from tools.script_recovery import test_generate_affair_wife_resource_candidate as candidate_tests


class WifeHealthBranchesTests(unittest.TestCase):
    def test_original_fpu_branches_require_ordered_positive_health(self):
        data=RData()
        import json
        from tools.script_recovery.lift_native_lua import ROOT
        unit=json.loads((ROOT/'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
        function=next(f for f in unit['functions'] if int(f['address'],16)==0xDB2B10)
        verify(function,data)
        sites=[(0xDB2DFC,0xDB2E0F),(0xDB2FA7,0xDB2FBA),(0xDB314F,0xDB3162),
               (0xDB320D,0xDB3220),(0xDB38C2,0xDB38D5),(0xDB3A8B,0xDB3A9E)]
        for start,end in sites:
            for value in (float('nan'),float('inf'),float('-inf'),0.0,-0.0,1.0,-1.0,1.401298464e-45):
                with self.subTest(site=hex(start),value=value):
                    machine=Uc(UC_ARCH_X86,UC_MODE_32)
                    machine.mem_map(0xDB2000,0x2000);machine.mem_map(0x122D000,0x1000);machine.mem_map(0x200000,0x1000)
                    machine.mem_write(start,data.bytes_at(start,end-start))
                    machine.mem_write(0x122DEDC,b'\0'*4)
                    machine.mem_write(0x200100,struct.pack('<f',value))
                    machine.mem_write(0x200000,b'\xd9\x05'+struct.pack('<I',0x200100))
                    machine.emu_start(0x200000,0x200006,count=1)
                    machine.emu_start(start,end,count=30)
                    self.assertEqual(bool(machine.reg_read(UC_X86_REG_EBX)&255),value>0)

    def test_nan_never_reaches_hit_speech_in_candidate(self):
        runner=candidate_tests.WifeCandidateTests()
        runner.source,_=candidate_tests.generate()
        events,error=runner.run_case(hit=True,health=float('nan'))
        self.assertIsNone(error)
        self.assertFalse(any(e[0]=='speak' for e in events))
