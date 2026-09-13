"""Reject incomplete or misidentified movie cleanup in the archived native CFG."""
import json
from pathlib import Path
import unittest
import capstone
from tools.script_recovery.audit_guardian_resource_lifetimes import check_movie_identity
ROOT=Path(__file__).resolve().parents[2]


class GuardianMovieIdentityTests(unittest.TestCase):
    def test_archived_cfg_and_invalid_event_maps(self):
        data=json.loads((ROOT/'refs/script_recovery/guardian_trophy_dealer_info/runtime_evidence/native-resource-lifetimes-20260912.json').read_text())
        decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);decoder.detail=True
        instructions=list(decoder.disasm(bytes.fromhex(data['native_bytes']),int(data['function'],16)))
        events={int(a,16):tuple(v) for a,v in data['movies'].items()}
        self.assertEqual(check_movie_identity(instructions,events),data['identity_check'])
        for address,event in [(0xe28c85,('end',104)),(0xe28c85,('end',120)),
                              (0xe283f5,('end',104)),(0xe28933,('start',120))]:
            with self.subTest(address=hex(address),event=event):
                self.assertIsNone(check_movie_identity(instructions,{**events,address:event}))
        missing=dict(events);del missing[0xe28659]
        self.assertIsNone(check_movie_identity(instructions,missing))


if __name__=='__main__':unittest.main()
