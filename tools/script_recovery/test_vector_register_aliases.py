import unittest

from tools.script_recovery.native_evidence_lowering import fold_local_thing_vectors, fold_vector_register_aliases

# TraderComment Main 0x00E05360 (retail re-reads [esp+0x1c] / [esp+0x20] at 0x00E05410 and 0x00E054B4)
LOOP = ('  puVar1 = pu_stk_18;\n'
        '  while (pu_stk_18 = puVar1, puVar2 = xStack_1c, !bVar3) {\n'
        '    iVar5 = (int)puVar1 - (int)xStack_1c >> 0x1f;\n'
        '    if (((int)puVar1 - (int)xStack_1c) / 0xc + iVar5 != iVar5) {\n'
        '      puVar2 = pu_stk_18;\n'
        '      puVar1 = xStack_1c;\n'
        '    }\n'
        '    puVar2 = pu_stk_18;\n'
        '    puVar1 = xStack_1c;\n'
        '  }\n')


class VectorRegisterAliasTests(unittest.TestCase):
    def test_size_reads_the_end_slot_and_write_back_vanishes(self):
        out = fold_vector_register_aliases(LOOP, 'xStack_1c')
        self.assertIn('iVar5 = (int)pu_stk_18 - (int)xStack_1c >> 0x1f;', out)
        self.assertIn('((int)pu_stk_18 - (int)xStack_1c) / 0xc', out)
        self.assertIn('while (puVar2 = xStack_1c, !bVar3)', out)
        self.assertNotIn('pu_stk_18 = puVar1', out)

    def test_register_with_another_source_is_left_alone(self):
        text = LOOP + '  puVar1 = somethingElse;\n'
        self.assertEqual(fold_vector_register_aliases(text, 'xStack_1c'), text)

    def test_end_slot_written_from_a_foreign_value_is_left_alone(self):
        text = LOOP + '  pu_stk_18 = pOther;\n'
        self.assertEqual(fold_vector_register_aliases(text, 'xStack_1c'), text)

    def test_addressed_end_slot_is_left_alone(self):
        text = LOOP + '  Grow(&pu_stk_18);\n'
        self.assertEqual(fold_vector_register_aliases(text, 'xStack_1c'), text)


class MirroredElementOperandTests(unittest.TestCase):
    def test_bare_operand_is_the_element_but_a_data_pointer_keeps_its_field_offset(self):
        src = ('  GSI->GetAllThingsWithScriptName(&name,&xStack_1c);\n'
               '  bVar3 = ENGINE_IsDistanceBetweenThingsUnder(p0, (iVar5 + (int)xStack_1c), (float)d);\n'
               '  cVar4 = (**(code **)(**(int **)(iVar6 + (int)xStack_1c) + 0x138))(p0);\n')
        out = fold_local_thing_vectors(src, {0x138: ('IsEqualTo',)})
        self.assertIn('ENGINE_IsDistanceBetweenThingsUnder(p0, LOCALLIST_At(xStack_1c, (iVar5) / 0xc), (float)d)', out)
        self.assertIn('CScriptThing::IsEqualTo(LOCALLIST_At(xStack_1c, (iVar6 - 4) / 0xc), p0)', out)


class MergedBeginEndCountTests(unittest.TestCase):
    # WatchForMissionRules 0x00E06440 (Trader Escort's end without the end trader): the begin/end/capacity slots
    # iStack_b0 / iStack_ac / uStack_a8 all reach the fold as the slot's earlier actor-map name `xStack_b0`, so
    # `(end - begin) / 0xc` reads `(xStack_b0 - xStack_b0) / 0xc` -- lifted, a table minus itself
    TEXT = ('                xStack_b0 = 0;\n'
            '                xStack_b0 = 0;\n'
            '                xStack_b0 = 0;\n'
            '                GSI->GetAllThingsWithScriptName(&xStack_64,xStack_b0);\n'
            '                if ((xStack_b0 - xStack_b0) / 0xc == *(int *)(this + 0x50)) {\n'
            '                  GSI->SetStateBool("EndStarted", true);\n'
            '                }\n')

    def test_self_difference_of_a_vector_is_its_count(self):
        out = fold_local_thing_vectors(self.TEXT)
        self.assertIn('xStack_b0 = GSI->GetAllThingsWithScriptName(&xStack_64);', out)
        self.assertIn('if (LOCALLIST_Count(xStack_b0) == *(int *)(this + 0x50)) {', out)

    def test_self_difference_of_a_non_vector_is_left_alone(self):
        text = '  if ((iVar3 - iVar3) / 0xc == 1) {\n    f();\n  }\n'
        self.assertEqual(fold_local_thing_vectors(text), text)


if __name__ == '__main__':
    unittest.main()
