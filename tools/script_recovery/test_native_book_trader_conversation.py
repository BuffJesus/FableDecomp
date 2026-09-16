import unittest
from types import SimpleNamespace

from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_book_trader_conversation import verify


class BookTraderConversationTests(unittest.TestCase):
    def test_borrowed_hero_and_call_operands(self):
        result = verify(RData())
        self.assertEqual(result['heroOwnership'], 'borrowed interface+0x30')
        self.assertEqual(result['status'], 'verified')

    def test_changed_native_order_ownership_binding_or_literal_rejects(self):
        data = RData()
        for site in (0xDB4D6B, 0xDB4E38, 0xDB4E42, 0xDB4E5F, 0x891CB8,
                     0x891D46, 0x8906CC, 0x89071C, 0x89076C, 0x1260F0C + 0x118):
            def read(address, size):
                raw = data.bytes_at(address, size)
                if raw is not None and address <= site < address + size:
                    raw = bytearray(raw); raw[site-address] ^= 1; return bytes(raw)
                return raw
            with self.subTest(site=hex(site)), self.assertRaises(ValueError):
                verify(SimpleNamespace(bytes_at=read, string_at=data.string_at))
        with self.assertRaisesRegex(ValueError, 'literal'):
            verify(SimpleNamespace(bytes_at=data.bytes_at, string_at=lambda address: 'WRONG'))
