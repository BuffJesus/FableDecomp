import unittest
from tools.script_recovery.native_entity_self_copies import fold
from tools.script_recovery.lift_native_lua import _captured_entity_self


SOURCE='''
    xStack_1c8._4_4_ = *(undefined4 *)(this + 0xc);
    piStack_1c0 = *(int **)(this + 0x10);
    bVar5 = true;
    bVar8 = true;
    bVar6 = true;
    xStack_1c8._0_4_ = &PTR__scalar_deleting_destructor__01238c8c;
    if (piStack_1c0 != (int *)0x0) {
      *piStack_1c0 = *piStack_1c0 + 1;
    }
    CScriptThing::CScriptThing((CScriptThing *)(worker + 0x3c),(CScriptThing *)xStack_1c8,ignored);
'''


class EntitySelfCopyTests(unittest.TestCase):
    def test_preserves_captured_self_and_flags(self):
        result=fold(SOURCE)
        self.assertIn('xStack_1c8 = (CScriptThing *)(this + 8);',result)
        self.assertNotIn('01238c8c',result)
        self.assertNotIn('piStack_1c0',result)
        for flag in ('bVar5','bVar8','bVar6'):self.assertIn(flag+' = true;',result)
        self.assertTrue(_captured_entity_self(result,'xStack_1c8'))

    def test_other_layout_or_incomplete_retain_stays_unresolved(self):
        changes=[('this + 0xc','this + 0x14'),('this + 0x10','this + 0x18'),
                 ('piStack_1c0','piStack_1bc'),('01238c8c','0127094c'),
                 ('+ 1;','+ 2;'),('bVar5 = true;','xStack_1c8 = true;'),
                 ('(CScriptThing *)xStack_1c8','(Other *)xStack_1c8')]
        for old,new in changes:
            with self.subTest(old=old):
                source=SOURCE.replace(old,new)
                self.assertEqual(fold(source),source)

    def test_live_info_word_is_not_removed(self):
        source=SOURCE+'    result = piStack_1c0;\n'
        self.assertEqual(fold(source),source)

    def test_new_info_lifetime_does_not_prevent_copy_recovery(self):
        source=SOURCE+'    piStack_1c0 = other;\n    result = piStack_1c0;\n'
        self.assertIn('xStack_1c8 = (CScriptThing *)(this + 8);',fold(source))

    def test_overwritten_or_other_thing_not_accepted_as_self_capture(self):
        for body in ('slot = (CScriptThing *)(this + 12);',
                     'slot = (CScriptThing *)(this + 8); slot = other;'):
            self.assertFalse(_captured_entity_self(body,'slot'))
