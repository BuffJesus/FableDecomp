import json
import unittest
from pathlib import Path
from types import SimpleNamespace
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_barrel_conversations import recover


class BarrelConversationTests(unittest.TestCase):
    def test_missing_operands_replaced_and_evidence_changes_rejected(self):
        witness=json.loads(Path(__file__).with_name('native_barrel_conversations_witness.json').read_text())
        source=''.join(b['oldLua'] for b in witness['blocks']);data=RData()
        result,report=recover(source,data)
        self.assertEqual(result,''.join(b['newLua'] for b in witness['blocks']))
        self.assertEqual(report['status'],'recovered')
        for region in witness['blocks']+witness['helpers']:
            def changed(address,size):
                raw=data.bytes_at(address,size)
                if address==region['address']:return bytes(size)
                return raw
            with self.assertRaisesRegex(ValueError,'native instructions'):
                recover(source,SimpleNamespace(bytes_at=changed,string_at=data.string_at))
        with self.assertRaisesRegex(ValueError,'literal changed'):
            recover(source,SimpleNamespace(bytes_at=data.bytes_at,string_at=lambda _: 'CHANGED'))
        with self.assertRaisesRegex(ValueError,'source correspondence'):
            recover(source.replace('me, r4','me, r5'),data)
