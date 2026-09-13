import struct
import unittest
from tools.script_recovery.observe_retail_cutscene import snapshot_audio_backend, snapshot_audio_update_gate

class BackendObserverTests(unittest.TestCase):
    def fixture(self, delta=0):
        memory = bytearray(0x10000)
        def put(address, fmt, *args):
            struct.pack_into(fmt, memory, address, *args)
        put(0x1000, '<I', 0x12B2748+delta)
        put(0x1228, '<I', 1)
        put(0x1234, '<I', 0x2000)
        put(0x2004, '<I', 0x3000)
        put(0x200C, '<I', 0x4000)
        memory[0x2025] = 1
        put(0x3038, '<fff', 0, .5, 0)
        memory[0x3044] = 1
        put(0x3048, '<I', 1)
        put(0x4158, '<fff', 10, 30, 20.5)
        put(0x412C, '<fff', .5, 1, .001)
        put(0x4190, '<IIf', 30, 90, .001)
        reads = []
        def read(address, size):
            reads.append((address,size))
            if address+size > len(memory):
                raise OSError('unreadable')
            return bytes(memory[address:address+size])
        return memory, read, reads

    def test_fields_and_relocation(self):
        for delta in [0,0x120000]:
            _,read,_ = self.fixture(delta)
            result = snapshot_audio_backend(read,0x1000,delta)
            self.assertTrue(result['complete'])
            voice = result['voices'][0]
            self.assertEqual(voice['submitted_position'], (10,30,20.5))
            self.assertEqual(voice['distance'], .5)
            self.assertAlmostEqual(voice['directional_factor'], .001)
            self.assertEqual(voice['relative'], 1)

    def test_proxy_gate_and_class_rejection(self):
        for delta in [0,0x120000]:
            for gate in [0,1]:
                memory,read,reads=self.fixture(delta)
                struct.pack_into('<I',memory,0x17BC,0x5000)
                struct.pack_into('<I',memory,0x5000,0x12B3290+delta)
                memory[0x5019]=gate
                result=snapshot_audio_update_gate(read,0x1000,delta)
                self.assertTrue(result['complete'])
                self.assertEqual(result['proxy_gate_0x19'],gate)
                struct.pack_into('<I',memory,0x5000,0)
                self.assertEqual(snapshot_audio_update_gate(read,0x1000,delta)['partial_reason'],'unknown_proxy_vtable')

    def test_gates_do_not_follow_inactive_backend(self):
        for address in [0x2004,0x200C,0x2025,0x3048]:
            memory,read,reads = self.fixture()
            struct.pack_into('<I',memory,address,0)
            self.assertTrue(snapshot_audio_backend(read,0x1000)['complete'])
            self.assertNotIn((0x4000,0x1DC),reads)

    def test_native_mode_input_and_absent_mode_object(self):
        for mode in [None,0,1,2,3,4,7]:
            memory,read,_=self.fixture()
            struct.pack_into('<I',memory,0x17BC,0x5000)
            struct.pack_into('<I',memory,0x5000,0x12B3290)
            struct.pack_into('<I',memory,0x6188,0 if mode is None else 0x7000)
            struct.pack_into('<I',memory,0x7004,mode or 0)
            struct.pack_into('<I',memory,0x6000,0x12B5BC0)
            struct.pack_into('<II',memory,0x61B8,64,6)
            def with_global(address,size):
                if address==0x143BB5C:return struct.pack('<I',0x6000)
                return read(address,size)
            result=snapshot_audio_update_gate(with_global,0x1000)
            self.assertTrue(result['mode_complete'])
            self.assertEqual(result['native_mode_code'],0x80000000 if mode is None else mode)
            self.assertTrue(result['output_complete'])
            self.assertEqual(result['output_selected_mode'],64)
            self.assertEqual(result['output_channel_count'],6)
            struct.pack_into('<I',memory,0x6000,0)
            result=snapshot_audio_update_gate(with_global,0x1000)
            self.assertTrue(result['mode_complete'])
            self.assertFalse(result['output_complete'])
            self.assertEqual(result['output_partial_reason'],'unknown_output_backend_vtable')

    def test_unknown_class_and_bounds(self):
        memory,read,reads = self.fixture()
        struct.pack_into('<I',memory,0x1000,0)
        self.assertEqual(snapshot_audio_backend(read,0x1000)['partial_reason'],'unknown_mixer_vtable')
        self.assertEqual(reads,[(0x1000,4)])
        memory,read,reads = self.fixture()
        struct.pack_into('<I',memory,0x1228,129)
        self.assertEqual(snapshot_audio_backend(read,0x1000)['partial_reason'],'voice_limit')
        self.assertEqual(len(reads),2)
        memory,read,reads = self.fixture()
        struct.pack_into('<I',memory,0x200C,0xFFFF)
        self.assertFalse(snapshot_audio_backend(read,0x1000)['complete'])

if __name__ == '__main__':
    unittest.main()
