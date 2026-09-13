import unittest

from tools.script_recovery.test_lift_native_lua import make


class StaleValueTests(unittest.TestCase):
    def test_unresolved_stack_write_cannot_reuse_previous_string(self):
        lifter = make()
        lifter.statement('Main', 'uStack_1ec = "OBJECT_CHOCOLATE_BOX_UNGIVEABLE";')
        lifter.statement('Main', 'uStack_1ec = uStack_1ec & 0xfffffffb;')
        result = lifter.expr('uStack_1ec & 4')
        self.assertNotIn('OBJECT_CHOCOLATE', result)
        self.assertEqual(result, 'uStack_1ec & 4')
        self.assertTrue(any('uStack_1ec' in t for t in lifter.todo))

    def test_noise_and_unknown_local_writes_invalidate_all_operand_facts(self):
        for name, replacement in (('pCVar8', 'FUN_00123456()'),
                                  ('pCVar8', 'Unknown::Value()'),
                                  ('pCVar8', '&local_missing'),
                                  ('uStack_20', 'uStack_20 & 0xffffff00')):
            with self.subTest(name=name, replacement=replacement):
                lifter = make(entity=True)
                lifter.push_temp(name, '"STALE"')
                lifter.kinds[name] = 'string'
                lifter.me_aliases.add(name)
                lifter.statement('Main', name + ' = ' + replacement + ';')
                self.assertNotIn(name, lifter.temps)
                self.assertNotIn(name, lifter.order)
                self.assertNotIn(name, lifter.kinds)
                self.assertNotIn(name, lifter.me_aliases)

    def test_known_address_copy_still_preserves_its_source_operand(self):
        lifter = make()
        lifter.statement('Main', 'local_text = "CURRENT";')
        lifter.statement('Main', 'pCVar8 = "OLD";')
        lifter.statement('Main', 'pCVar8 = &local_text;')
        self.assertEqual(lifter.expr('pCVar8'), '"CURRENT"')


if __name__ == '__main__':
    unittest.main()
