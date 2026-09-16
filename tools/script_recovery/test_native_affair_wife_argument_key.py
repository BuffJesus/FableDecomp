import unittest
from types import SimpleNamespace

from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_affair_wife_argument_key import verify


class WifeArgumentKeyTests(unittest.TestCase):
    def test_key_survives_optional_reply_on_every_native_path(self):
        result=verify(RData())
        self.assertEqual(result['keyEvents']['end'],[0xDB3C82,0xDB3D8B])
        self.assertIn(0xDB3C6F,result['keyEvents']['use'])

    def test_changed_helper_query_branch_or_cleanup_rejects(self):
        data=RData()
        for site in (0x99F570,0x99F830,0x99EFE0,0x892070,0xDB3BE2,0xDB3C82,0xDB3D8B):
            def read(address,size):
                raw=data.bytes_at(address,size)
                if raw is not None and address<=site<address+size:
                    raw=bytearray(raw);raw[site-address]^=1;return bytes(raw)
                return raw
            with self.subTest(site=hex(site)),self.assertRaises(ValueError):
                verify(SimpleNamespace(bytes_at=read,string_at=data.string_at))
