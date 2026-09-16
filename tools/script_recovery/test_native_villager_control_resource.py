import copy
import unittest
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_villager_control_resource import verify


class VillagerControlResourceTests(unittest.TestCase):
    def test_whole_lifetime_and_event_omissions(self):
        data=RData();witness=verify(data)
        self.assertEqual(len(witness['events']),23)
        self.assertEqual(sum(e['name']=='acquire' for e in witness['events']),4)
        for index in range(len(witness['events'])):
            changed=copy.deepcopy(witness);changed['events'].pop(index)
            with self.assertRaisesRegex(ValueError,'event coverage'):verify(data,changed)
        changed=copy.deepcopy(witness)
        event=next(e for e in changed['events'] if e['name']=='acquire')
        event['setup']['stack_arguments'][0]=['register','esi']
        with self.assertRaisesRegex(ValueError,'operands changed'):verify(data,changed)
