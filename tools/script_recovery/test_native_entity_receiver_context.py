import unittest

from tools.script_recovery import test_lift_native_lua as fixtures
from tools.script_recovery import test_native_post_attack_cleanup as post_attack
from tools.script_recovery.lift_native_lua import Lifter, RData


class EntityReceiverContextTests(unittest.TestCase):
    def test_quest_control_call_does_not_invent_me_or_consume_recent_hero(self):
        lifter = fixtures.make()
        lifter.locals.add('hero')
        lifter.kinds['hero'] = 'thing'
        lifter.push_temp('hero', 'hero')
        body = '\n'.join(lifter.lift('QuestControl', '{\nGSI->StartScriptingEntity();\n}'))
        self.assertNotIn('me:', body)
        self.assertNotIn('AcquireControl', lifter.calls)
        self.assertIn('unresolved entity receiver/resource', body)
        self.assertTrue(lifter.todo)

    def test_entity_only_api_without_receiver_marks_result_unresolved(self):
        lifter = fixtures.make()
        body = '\n'.join(lifter.lift('QuestPredicate', '{\nanswer = GSI->IsAlive();\n}'))
        self.assertNotIn('me:', body)
        self.assertIn('answer = nil --[[unresolved native result]]', body)
        self.assertNotIn('IsAlive', lifter.calls)
        self.assertTrue(lifter.todo)

    def test_shared_helper_with_explicit_execution_entity_keeps_me(self):
        lifter = Lifter(fixtures.MANIFEST, {}, 'quest', False, '', RData(), execution_entity=True)
        body = '\n'.join(lifter.lift('EntityHelper', '{\nanswer = GSI->IsAlive();\n}'))
        self.assertIn('me:IsAlive()', body)
        self.assertEqual(lifter.todo, [])

    def test_actual_post_attack_control_is_reported_without_phantom_actor(self):
        fn, source, manifest, _ = post_attack.PostAttackCleanupTests().inputs()
        lifter = Lifter(manifest, {}, 'quest', False, '', RData(), live_termination=True, native_gotos=True)
        body = '\n'.join(lifter.lift('PostAttackStuff', source, native_function=fn))
        self.assertNotIn('me:AcquireControl', body)
        self.assertIn('StartScriptingEntity: unresolved entity receiver/resource in quest context', body)
        self.assertNotIn('AcquireControl', lifter.calls)


if __name__ == '__main__':
    unittest.main()
