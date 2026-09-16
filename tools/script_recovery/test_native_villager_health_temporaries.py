import copy
import struct
import unittest
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_villager_health_temporaries import verify
from tools.script_recovery.test_native_barrel_man_health_branches import native


class VillagerHealthTemporariesTests(unittest.TestCase):
    def test_coverage_and_output_identity(self):
        data=RData();rows=verify(data)['temporaries']
        for index in range(2):
            changed=copy.deepcopy(rows);changed.pop(index)
            with self.assertRaisesRegex(ValueError,'coverage changed'):verify(data,changed)
        changed=copy.deepcopy(rows);changed[0]['output']=['stack',80]
        with self.assertRaisesRegex(ValueError,'output changed'):verify(data,changed)

    def test_native_positive_health_survives_destructor(self):
        data=RData();verify(data)
        self.assertEqual(data.bytes_at(0x122DEDC,4),bytes(4))
        for address,end,slot in ((0xDAE247,0xDAE263,68),(0xDAE318,0xDAE334,80)):
            region=dict(address=address,size=end-address,output=slot,result='bl')
            for bits in (0,0x80000000,0x3F800000,0xBF800000,1,0x80000001,0x7F800000,0xFF800000,0x7FC12345):
                value=struct.unpack('<f',struct.pack('<I',bits))[0]
                self.assertEqual(native(data,region,bits),value>0)
