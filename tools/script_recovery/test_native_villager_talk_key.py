import unittest
from types import SimpleNamespace
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_villager_talk_key import verify


class VillagerTalkKeyTests(unittest.TestCase):
    def test_suffix_scope_and_literal_identity(self):
        data=RData();report=verify(data)
        self.assertEqual(report['identity'],('stack',24))
        self.assertEqual(len(report['events']),6)
        for address in (0x12D86E4,0x12D86DC,0x12D86B4,0x12D8694):
            changed=SimpleNamespace(bytes_at=data.bytes_at,string_at=lambda site:'CHANGED' if site==address else data.string_at(site))
            with self.assertRaisesRegex(ValueError,'literal changed'):verify(changed)
