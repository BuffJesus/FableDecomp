import copy
import json
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch

from tools.script_recovery.lift_native_lua import ROOT, RData
from tools.script_recovery.native_book_trader_lifetime import map_book_trader_lifetime


class BookTraderLifetimeTests(unittest.TestCase):
    def inputs(self):
        unit = json.loads((ROOT / 'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
        fn = next(f for f in unit['functions'] if f['address'].lower() == '0x00db3fa0')
        return fn, RData()

    def test_four_resource_exits_and_eight_temporary_lifetimes(self):
        fn, data = self.inputs()
        evidence, = map_book_trader_lifetime(fn, data)
        self.assertEqual(evidence['status'], 'mapped')
        self.assertEqual(len(evidence['resourceEvents']), 54)
        self.assertEqual(len(evidence['thingEvents']), 24)
        self.assertEqual({e['site'] for e in evidence['resourceEvents'] if e['operation'] == 'end'},
                         {0xDB41C7, 0xDB4EB6, 0xDB4EFF, 0xDB4F5A})
        self.assertEqual([e['site'] for e in evidence['resourceEvents'] if e['operation'] == 'start'], [0xDB3FFC])
        self.assertIn('not yet generated', evidence['loweringStatus'])

    def test_omitted_use_exit_or_wrong_branch_receiver_is_not_accepted(self):
        fn, data = self.inputs()
        witness_path = Path(__file__).with_name('native_book_trader_lifetime_witness.json')
        original = json.loads(witness_path.read_text())
        variants = []
        for site in (0xDB4475, 0xDB4EB6, 0xDB4EFF):
            witness = copy.deepcopy(original)
            witness['resourceEvents'] = [e for e in witness['resourceEvents'] if e['site'] != site]
            variants.append(witness)
        witness = copy.deepcopy(original)
        witness['selections'][str(0xDB41A1)] = ['stack', 24]
        variants.append(witness)
        witness = copy.deepcopy(original)
        next(e for e in witness['thingEvents'] if e['site'] == 0xDB40F2)['identity'] = ['stack', 140]
        variants.append(witness)
        witness = copy.deepcopy(original)
        witness['thingEvents'] = [e for e in witness['thingEvents'] if e['site'] != 0xDB4147]
        variants.append(witness)
        read_text = Path.read_text
        for index, witness in enumerate(variants):
            def modified(path, *args, **kwargs):
                return json.dumps(witness) if path.name == witness_path.name else read_text(path, *args, **kwargs)
            with self.subTest(index=index), patch.object(Path, 'read_text', modified):
                evidence, = map_book_trader_lifetime(fn, data)
                self.assertEqual(evidence['status'], 'rejected')

    def test_changed_cleanup_or_abi_bytes_reject(self):
        fn, data = self.inputs()
        for site in (0xDB3FFC, 0xDB41A1, 0xDB411F, 0xDB4E92, 0xDB4EDB, 0x891D4C, 0x99A430):
            def changed(address, size):
                raw = data.bytes_at(address, size)
                if raw is not None and address <= site < address + size:
                    raw = bytearray(raw)
                    raw[site - address] ^= 1
                    return bytes(raw)
                return raw
            with self.subTest(site=hex(site)):
                reader = SimpleNamespace(bytes_at=changed, string_at=data.string_at)
                evidence, = map_book_trader_lifetime(fn, reader)
                self.assertEqual(evidence['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()
