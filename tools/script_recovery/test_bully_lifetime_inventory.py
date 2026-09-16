import unittest
from collections import Counter
from tools.script_recovery.bully_lifetime_inventory import inventory
from tools.script_recovery.native_resource_lifetime import check_resource_lifetimes
from tools.script_recovery.lift_native_lua import RData

class BullyLifetimeInventoryTests(unittest.TestCase):
    def test_all_native_resource_movie_and_retained_victim_paths_balance(self):
        report,ins,events,selections=inventory()
        self.assertTrue(report['cfgLifetimeProved']);self.assertEqual(len(events),123)
        starts=Counter(resource for operation,resource in events.values() if operation=='start')
        self.assertEqual(starts[('resource',16)],1);self.assertEqual(starts[('resource',60)],1)
        self.assertEqual(starts[('resource',116)],1);self.assertEqual(starts[('victim',44)],1)
        self.assertEqual(sum(n for (kind,slot),n in starts.items() if kind=='movie'),5)
        self.assertEqual(starts[('pause',0)],5)
        self.assertEqual(sum(op=='use' and resource==('resource',16) for op,resource in events.values()),62)

    def test_wrong_resource_omitted_cleanup_and_shared_movie_selection_reject(self):
        _,ins,events,selections=inventory()
        bad=dict(events);bad[0xdbc92f]=('use',('resource',116))
        self.assertFalse(check_resource_lifetimes(ins,bad,receiver_register='ecx',selections=selections))
        for address in (0xdbccef,0xdbcce6,0xdbcc37):
            bad=dict(events);del bad[address]
            self.assertFalse(check_resource_lifetimes(ins,bad,receiver_register='ecx',selections=selections))
        bad=dict(selections);bad[0xdbb9dd]=('movie',240)
        self.assertFalse(check_resource_lifetimes(ins,events,receiver_register='ecx',selections=bad))

    def test_native_constructor_target_or_lookup_mutation_rejects(self):
        class Changed(RData):
            def bytes_at(self,address,size):
                raw=super().bytes_at(address,size)
                if address<=self.changed<address+size:
                    raw=bytearray(raw);raw[self.changed-address]^=1;return bytes(raw)
                return raw
        for changed in (0xdbb56a,0xdbc8fe,0xdbc92f,0xdbc9a9,0xdbbd7e,0xdbcce6):
            data=Changed();data.changed=changed
            with self.assertRaises(ValueError):inventory(data)
