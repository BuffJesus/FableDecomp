import copy
import unittest
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_villager_conversation_key import verify


class VillagerConversationKeyTests(unittest.TestCase):
    def test_retained_lifetime_and_call_identity(self):
        data=RData();w=verify(data)
        for index in range(len(w['events'])):
            changed=copy.deepcopy(w);changed['events'].pop(index)
            with self.assertRaisesRegex(ValueError,'coverage changed'):verify(data,changed)
        for index in (1,3,4):
            changed=copy.deepcopy(w);line=next(e for e in changed['events'] if e['name']=='line')
            line['setup']['stack_arguments'][index]=['constant',0]
            with self.assertRaisesRegex(ValueError,'operands changed'):verify(data,changed)
