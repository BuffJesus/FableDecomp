import struct
import json
import unittest
from tools.script_recovery.native_concat_literal_operands import restore


class ConcatLiteralTests(unittest.TestCase):
    def fixture(self, *, ctor=0x99EBF0, forward=b'\x8b\xd0', length=b'\x6a\xff', receiver=b'\x8d\x4c\x24\x04', destination=b'\x8d\x4c\x24\x08', value='TEXT_PREFIX'):
        raw = length + b'\x68\x00\x20\x00\x00' + receiver
        ctor_site = 0x1000 + len(raw)
        raw += b'\xe8' + struct.pack('<i', ctor - (0x1000 + len(raw) + 5))
        raw += forward + destination
        site = 0x1000 + len(raw)
        raw += b'\xe8' + struct.pack('<i', 0x99F570 - (site + 5))
        class Data:
            def bytes_at(self, address, size):
                return raw if address == 0x1000 and size == len(raw) else None
            def string_at(self, address):
                return value if address == 0x2000 else None
        text = 'result = Concat(out,a,suffix);'
        a, e = text.index('out'), text.index(')')
        ordered = [(a, e, ['out', 'a', 'suffix'], {'target':'0x0099f570','site':hex(site)}, 'Concat', False)]
        ordered.append((0,0,['(CCharString *)&prefix',json.dumps(value),'-1'],{'target':hex(ctor),'site':hex(ctor_site)},'Ctor',False))
        fn = {'address':'0x1000','bodyEndExclusive':hex(0x1000 + len(raw))}
        return text, fn, ordered, Data()

    def test_recovers_literal_without_changing_suffix_or_destination(self):
        self.assertEqual(restore(*self.fixture()), 'result = Concat(out,prefix,suffix);')

    def test_escapes_literal(self):
        self.assertEqual(restore(*self.fixture(value='A"B\\C')), 'result = Concat(out,prefix,suffix);')

    def test_thread_prefix_retains_temporary_for_spawn_recognizer(self):
        self.assertEqual(restore(*self.fixture(value='ParentClass.')), 'result = Concat(out,prefix,suffix);')

    def test_missing_or_mismatched_constructor_pairing_remains_unchanged(self):
        for mutation in ('missing','literal','slot'):
            with self.subTest(mutation=mutation):
                text,fn,ordered,data=self.fixture()
                if mutation=='missing': ordered.pop()
                elif mutation=='literal': ordered[1][2][1]='"DIFFERENT"'
                else: ordered[1][2][0]='unknown + 4'
                self.assertEqual(restore(text,fn,ordered,data),text)

    def test_uncertain_native_sequences_remain_unchanged(self):
        cases = [dict(ctor=0x99EBF1), dict(forward=b'\x8b\xd1'),
                 dict(forward=b'\x8b\xd0\x90'), dict(length=b'\x6a\x01'),
                 dict(receiver=b'\x8d\x54\x24\x04'),
                 dict(destination=b'\x8d\x4b\x08'), dict(value=None)]
        for case in cases:
            with self.subTest(case=case):
                args = self.fixture(**case)
                self.assertEqual(restore(*args), args[0])

    def test_missing_pairing_remains_unchanged(self):
        text, fn, _, data = self.fixture()
        self.assertEqual(restore(text, fn, None, data), text)

    def test_other_overload_remains_unchanged(self):
        text, fn, ordered, data = self.fixture()
        ordered[0][3]['target'] = '0x0099f600'
        self.assertEqual(restore(text, fn, ordered, data), text)

    def test_indirect_call_remains_unchanged(self):
        text, fn, ordered, data = self.fixture()
        ordered[0] = (*ordered[0][:-1], True)
        self.assertEqual(restore(text, fn, ordered, data), text)
