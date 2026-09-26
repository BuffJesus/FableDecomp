"""Regressions from Bandit Camp's last three syntax failures (BanditKing, BCGameMaster, AssassinMarker)."""
import unittest

from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_lift_native_lua as fixtures
from tools.script_recovery.native_evidence_lowering import bind_st0_results, fold_signed_pow2_division
from tools.script_recovery.native_structured_switch import lower_nonfallthrough_switches


class St0BindingTests(unittest.TestCase):
    def test_unassigned_ftol2_operand_binds_the_preceding_call(self):
        # BanditKing Main 0x00D0A830: GetHealth (0x420) leaves the float in ST0 for `fistp`
        native = '\n'.join([
            '{', '  float10 value;', '  float10 value_00;',
            '  (**(code **)(**(int **)(this + 4) + 0x420))(*(int **)(this + 4),p0);',
            '  CVar8 = (CCharString_bv)__ftol2(value);',
            '  (**(code **)(**(int **)(this + 4) + 0x1c))(*(int **)(this + 4));',
            '  (**(code **)(**(int **)(this + 4) + 0x420))(*(int **)(this + 4),p0);',
            '  iVar10 = __ftol2(value_00);', '}'])
        bound = bind_st0_results(native)
        self.assertIn('fret_v0 = (**(code **)(**(int **)(this + 4) + 0x420))', bound)
        self.assertIn('__ftol2(fret_v0)', bound)
        self.assertIn('__ftol2(fret_v00)', bound)
        self.assertEqual(bound.count('= (**(code **)(**(int **)(this + 4) + 0x420))'), 2)
        self.assertNotIn('fret_v0 = (**(code **)(**(int **)(this + 4) + 0x1c))', bound)

    def test_assigned_value_local_is_not_an_st0_operand(self):
        native = '{\n  float10 value;\n  GSI->GetHealth(p0);\n  value = 3.0;\n  i = __ftol2(value);\n}'
        self.assertEqual(bind_st0_results(native), native)

    def test_st0_read_in_a_loop_head_binds_the_call_in_that_expression(self):
        # BCGameMaster Main 0x00D067A0: the preceding statement (SetQuitTavernGame) returns nothing
        native = '\n'.join([
            '{', '  float10 extraout_ST0_01;',
            '  (**(code **)(**(int **)((int)this + 4) + 0xd80))(*(int **)((int)this + 4),true);',
            '  while (((**(code **)(**(int **)((int)this + 4) + 0xb80))(*(int **)((int)this + 4)),',
            '         (float10)_DAT_0122dedc == extraout_ST0_01 ||',
            '         (bVar3 = (**(code **)(**(int **)((int)this + 4) + 0xb84))(*(int **)((int)this + 4)), bVar3)',
            '         )) {', '  }', '}'])
        bound = bind_st0_results(native)
        self.assertIn('  (**(code **)(**(int **)((int)this + 4) + 0xd80))', bound)
        self.assertIn('while ((fret_01 = (**(code **)(**(int **)((int)this + 4) + 0xb80))', bound)
        self.assertIn('== fret_01 ||', bound)


class DivisionTests(unittest.TestCase):
    def run_lua(self, expression, x):
        return LuaRuntime().execute(f'local CVar8 = {x}\nreturn ' + expression)

    def test_signed_power_of_two_idiom_truncates_toward_zero(self):
        native = '(int)((int)CVar8 * 3 + ((int)CVar8 * 3 >> 0x1f & 3U)) >> 2'
        folded = fold_signed_pow2_division(native)
        self.assertEqual(folded, 'ENGINE_Trunc(((int)CVar8 * 3) / 4)')
        lua = folded.replace('(int)', '').replace('ENGINE_Trunc(', 'math.tointeger(math.modf(') + ')'
        for x in (0, 1, 5, 7, 100, -5, -7):
            c = int(x * 3 / 4)            # C: truncation toward zero
            self.assertEqual(self.run_lua(lua, x), c, x)

    def test_mismatched_mask_is_left_alone(self):
        native = '(int)(x + (x >> 0x1f & 7U)) >> 2'
        self.assertEqual(fold_signed_pow2_division(native), native)

    def test_int_cast_quotient_truncates(self):
        folded = fold_signed_pow2_division('KingHealth < (int)CVar8 / 2')
        self.assertEqual(folded, 'KingHealth < ENGINE_Trunc(CVar8 / 2)')
        self.assertEqual(fold_signed_pow2_division('x[(iVar7) / 0xc]'), 'x[(iVar7) / 0xc]')


class LoopHeadCallTests(unittest.TestCase):
    def test_calls_in_a_short_circuit_loop_head_run_on_every_test(self):
        source = '''{
GSI->SetTimer(1, 0);
while ((fret_01 = GSI->GetBestTimeGuessTheAddition(), 0.0 == fret_01) || (bVar3 = GSI->IsHeroInTavernGame(), bVar3)) {
GSI->SetTimer(4, 1);
}
return;
}'''
        manifest = dict(fixtures.MANIFEST,
                        GetBestTimeGuessTheAddition={'scope': 'Quest', 'returnType': 'float', 'parameters': []},
                        IsHeroInTavernGame={'scope': 'Quest', 'returnType': 'bool', 'parameters': []})
        lifter = fixtures.Lifter(manifest, {}, 'Quest', False, 'Pkg', fixtures.NoRData())
        lifter.accessor_kinds = True
        body = '\n'.join(lifter.lift('Main', source))
        self.assertNotIn('while (', body)
        self.assertEqual(lifter.todo, [])
        # best time: 0, 0, 7 ; in game: (not asked while best is 0), true, false
        best, in_game, calls = [0.0, 0.0, 7.0, 7.0], [True, False], []
        def get_timer(_):
            calls.append('best')
            return best.pop(0)
        def done(_):
            calls.append('game')
            return in_game.pop(0)
        lua = LuaRuntime()
        quest = lua.table_from({'SetTimer': lambda _, t, v: calls.append(('set', t)),
                                'GetBestTimeGuessTheAddition': get_timer, 'IsHeroInTavernGame': done})
        lua.execute('return function(Quest)\n' + body + '\nend')(quest)
        self.assertEqual(calls, [('set', 1), 'best', ('set', 4), 'best', ('set', 4), 'best', 'game',
                                 ('set', 4), 'best', 'game'], body)


RELAY_FLAG = '''{
  CCharString CVar12;
  CCharString CVar13;
  CCharString xStack_124;
  CVar12 = (CCharString)0x0;
  CCharString::CCharString((CCharString *)&xStack_124,"Gate1GuardInner",-1);
  GSI->LookupThing(&xStack_124);
  if (a) {
    CVar13 = (CCharString)((uint)CVar12 | 1);
    xStack_124 = CVar13;
    if (((uint)CVar13 & 1) != 0) {
      xStack_124 = (CCharString)((uint)CVar13 & 0xfffffffe);
      ~CCharString(&xStack_f0);
    }
    CVar13 = xStack_124;
  }
  CVar12 = (CCharString)((uint)CVar13 | 0x20);
  if (((uint)CVar12 & 0x20) != 0) {
    CVar12 = (CCharString)((uint)CVar12 & 0xffffffdf);
    ~CCharString(&xStack_f8);
  }
  GSI->SetTimer(1, 2);
}'''


class RelaySlotFlagTests(unittest.TestCase):
    """Gate1GuardOuter Main 0x00D01630: a temp-destruction flag word alternating between two registers and
    parked in a stack slot that is also a real string."""

    def test_flag_word_parked_in_a_reused_slot_is_removed(self):
        from tools.script_recovery.native_evidence_lowering import drop_eh_state_flags
        out = drop_eh_state_flags(RELAY_FLAG)
        body = '\n'.join(l for l in out.split('\n') if not l.strip().startswith('CCharString CVar'))
        self.assertNotRegex(body, r'\bCVar1[23]\b', out)
        self.assertIn('CCharString::CCharString((CCharString *)&xStack_124,"Gate1GuardInner",-1);', out)
        self.assertIn('GSI->LookupThing(&xStack_124);', out)
        self.assertNotIn('xStack_124 = ', out)
        self.assertIn('GSI->SetTimer(1, 2);', out)

    def test_slot_read_as_a_real_value_keeps_the_flag(self):
        from tools.script_recovery.native_evidence_lowering import drop_eh_state_flags
        leaked = RELAY_FLAG.replace('    CVar13 = xStack_124;\n', '    GSI->SetTimer(xStack_124, 1);\n    CVar13 = xStack_124;\n')
        out = drop_eh_state_flags(leaked)
        self.assertIn('xStack_124 = CVar13', out)


BANDIT_KILLS = '''{
  int iStack_c;
  int i_stk_8;
  iStack_c = 0;
  i_stk_8 = 0;
  this_00 = GSI->GetHero();
  bVar2 = CScriptThing::MsgGetThingsKilled(this_00, &iStack_c);
  if (bVar2) {
    uVar6 = 0;
    iVar3 = iStack_c;
    if (i_stk_8 - iStack_c >> 2 != 0) {
      do {
        if ((*(byte *)(iVar3 + uVar6 * 4) & 4) != 0) {
          v_stk_10 = v_stk_10 + 1;
          iVar3 = iStack_c;
          iVar4 = i_stk_8;
        }
        uVar6 = uVar6 + 1;
      } while (uVar6 < (uint)(iVar4 - iVar3 >> 2));
    }
    std_vector_push_copy_element(&iStack_c,iVar3,iVar4);
  }
  CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)&iStack_c);
}'''


class ThingsKilledVectorTests(unittest.TestCase):
    """Q_BanditCamp CheckAnyBanditsKilled 0x00D032F0 and the area-massacre checks read the creature-group words
    retail MsgGetThingsKilled (0x008D3DD0) pushes; the sidecar's MsgGetThingsKilledGroups returns them as a list."""

    def test_group_words_become_a_list(self):
        from tools.script_recovery.native_evidence_lowering import fold_things_killed_vectors
        out = fold_things_killed_vectors(BANDIT_KILLS)
        self.assertIn('iStack_c = CScriptThing::MsgGetThingsKilledGroups(this_00);', out)
        self.assertIn('bVar2 = ENGINE_ListLen(iStack_c) != 0;', out)
        self.assertIn('if (ENGINE_ListLen(iStack_c) != 0) {', out)
        self.assertIn('if ((ENGINE_ListWord(iStack_c, uVar6) & 4) != 0) {', out)
        self.assertIn('while (uVar6 < (uint)(ENGINE_ListLen(iStack_c)));', out)
        self.assertNotRegex(out, r'\bi_stk_8 = |\biVar[34] = |OnReadFinished|std_vector_push_copy_element', out)

    def test_counted_kills_match_the_bandit_bit(self):
        from tools.script_recovery.native_evidence_lowering import finish_lua
        lua = LuaRuntime()
        count = lua.execute(finish_lua('''return function(groups)
  local n, i = 0, 0
  if ENGINE_ListLen(groups) ~= 0 then
    repeat
      if (ENGINE_ListWord(groups, i) & 4) ~= 0 then n = n + 1 end
      i = i + 1
    until not (i < ENGINE_ListLen(groups))
  end
  return n
end'''))
        self.assertEqual(count(lua.table_from([4, 1, 6, 8, 0x14])), 3)
        self.assertEqual(count(lua.table_from([])), 0)

    def test_other_use_of_a_copy_leaves_the_function(self):
        from tools.script_recovery.native_evidence_lowering import fold_things_killed_vectors
        leaked = BANDIT_KILLS.replace('v_stk_10 = v_stk_10 + 1;', 'v_stk_10 = v_stk_10 + iVar3;')
        self.assertEqual(fold_things_killed_vectors(leaked), leaked)

    def test_massacre_total_in_a_string_typed_slot(self):
        from tools.script_recovery.native_evidence_lowering import fold_things_killed_vectors
        native = '''{
  xStack_74 = (CCharString)0x0;
  CIndexBuffer::CIndexBuffer((CIndexBuffer *)&xStack_5c,(int)&xStack_75);
  bVar4 = CScriptThing::MsgGetThingsKilled(pCVar6, &xStack_5c);
  if (bVar4) {
    xStack_74 = (CCharString)((int)xStack_74 + (i_stk_58 - (int)xStack_5c >> 2));
    std_vector_push_copy_element(&xStack_5c,(int)xStack_5c,i_stk_58);
  }
  cVar5 = CScriptThing::MsgGetThingsKilled(elem_1, &xStack_5c);
  if (cVar5 != '\\0') {
    xStack_74 = (CCharString)((int)xStack_74 + (i_stk_58 - (int)xStack_5c >> 2));
  }
  if (ENGINE_GlobalGameDataFloat(0xe78) <= (float)(int)xStack_74) {
    CCharString::CCharString(&xStack_74,"",-1);
  }
}'''
        out = fold_things_killed_vectors(native)
        self.assertIn('native_arg_kills_xStack_74 = 0;', out)
        self.assertEqual(out.count('native_arg_kills_xStack_74 = native_arg_kills_xStack_74 + ENGINE_ListLen(xStack_5c);'), 2)
        self.assertIn('<= (float)native_arg_kills_xStack_74) {', out)
        self.assertIn('CCharString::CCharString(&xStack_74,"",-1);', out)      # the slot's string life keeps its name
        self.assertIn('xStack_5c = CScriptThing::MsgGetThingsKilledGroups(elem_1);', out)
        self.assertIn('if (cVar5) {', out)


class InterleavedColourTests(unittest.TestCase):
    def test_two_colours_built_together_both_fold(self):
        # BanditKing Main 0x00D0A830: the Twinblade health bar's two colours
        from tools.script_recovery.native_evidence_lowering import fold_stack_colours
        native = '\n'.join([
            '{', '  fVar13 = 1.0;',
            '  xStack_98._2_1_ = 0xff;', '  xStack_98._3_1_ = 0xff;', '  xStack_94._2_1_ = 0xff;', '  xStack_94._3_1_ = 0x80;',
            '  pColour2 = &xStack_98;', '  pColour1 = &xStack_94;', '  fVar8 = 0.0;',
            '  xStack_98._1_1_ = 0;', '  xStack_98._0_1_ = 0;', '  xStack_94._1_1_ = 0x10;', '  xStack_94._0_1_ = 0;',
            '  iVar6 = GSI->AddQuestInfoBar(fret_00,fVar8,(CRGBColour *)pColour1,(CRGBColour *)pColour2,pTexture,pCVar9,fVar13);',
            '}'])
        out = fold_stack_colours(native)
        self.assertIn('ENGINE_Colour(255, 0, 0, 255)', out)
        self.assertIn('ENGINE_Colour(255, 16, 0, 128)', out)
        self.assertNotRegex(out, r'xStack_9[48]\._', out)
        self.assertIn('fVar8 = 0.0;', out)

    def test_slot_read_between_its_stores_is_left_alone(self):
        from tools.script_recovery.native_evidence_lowering import gather_colour_byte_stores
        native = '\n'.join(['  xStack_98._2_1_ = 0xff;', '  xStack_98._3_1_ = 0xff;', '  iVar1 = xStack_98;',
                            '  xStack_98._1_1_ = 0;', '  xStack_98._0_1_ = 0;'])
        self.assertEqual(gather_colour_byte_stores(native), native)


GATE_SPAWN = '''            this_01 = operator_new(0x44);
            if (this_01 == (CSpawnedFunc<NScript::CExpression_FollowScript> *)0x0) {
              this_01 = (CSpawnedFunc<NScript::CExpression_FollowScript> *)0x0;
            }
            else {
              u_stk_f4 = u_stk_f4 | 0x38;
              CCharString::CCharString(&xStack_7c,"Gate2Outer",-1);
              uVar1 = *(undefined4 *)(this + 0x14);
              CCharString::CCharString(xStack_5c,"OpenGate",-1);
              pCVar9 = extraout_EAX;
              CCharString::CCharString(xStack_64,"ParentClass.",-1);
              pCVar9 = ENGINE_Concat(a, pCVar9);
              CSpawnedFunc<NScript::CExpression_FollowScript>::CSpawnedFunc<NScript::CExpression_FollowScript>(this_01,pCVar9,0);
              *(undefined ***)this_01 = &PTR__scalar_deleting_destructor__012c9918;
              *(code **)(this_01 + 0x34) = FUN_00d0ebc0;
              *(undefined4 *)(this_01 + 0x38) = uVar1;
              *(undefined4 *)(this_01 + 0x3c) = 2.0;
              CCharString::CCharString((CCharString *)(this_01 + 0x40),&xStack_7c);
              std::_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&> (&xStack_7c);
            }
            CCharString::CCharString(xStack_58,"",-1);
            CGuiVarTransferStruct::Add(*(CGuiVarTransferStruct **)(this + 0x14),this_01,sectionName);
'''


class BoundValueSpawnTests(unittest.TestCase):
    """Gate2Guard1 Main 0x00D0D910 spawns ParentClass.OpenGate(2.0, "Gate2Outer")."""

    def test_bound_values_become_thread_arguments(self):
        from tools.script_recovery.lift_native_lua import RE_THREAD_VALUES, _bound_thread_values
        m = RE_THREAD_VALUES.search(GATE_SPAWN)
        self.assertIsNotNone(m)
        self.assertEqual(m.group('name'), 'OpenGate')
        self.assertEqual(_bound_thread_values(m), ['2.0', '"Gate2Outer"'])

    def test_a_non_literal_field_is_not_bound(self):
        from tools.script_recovery.lift_native_lua import RE_THREAD_VALUES, _bound_thread_values
        m = RE_THREAD_VALUES.search(GATE_SPAWN.replace('+ 0x3c) = 2.0;', '+ 0x3c) = fVar2;'))
        self.assertIsNone(_bound_thread_values(m) if m else None)

    def test_parameter_string_passed_by_address_is_the_operand(self):
        manifest = dict(fixtures.MANIFEST, GetThingWithScriptName={
            'scope': 'Quest', 'returnType': 'CScriptThing*',
            'parameters': [{'name': 'scriptName', 'type': 'const std::string&', 'optional': False}]})
        lifter = fixtures.Lifter(manifest, {}, 'Quest', False, 'Pkg', fixtures.NoRData())
        lifter.accessor_kinds = True
        body = '\n'.join(lifter.lift('Main', '{\npDoor = GSI->GetThingWithScriptName((CScriptThing *)xStack_c,'
                                             '(CCharString *)&native_arg_door_name);\nreturn;\n}'))
        self.assertIn('GetThingWithScriptName(native_arg_door_name)', body)
        self.assertEqual(lifter.todo, [])


class CodeRefLabelTests(unittest.TestCase):
    def test_code_r_labels_become_ordinary_labels(self):
        # Gate1GuardOuter Main 0x00D01630: the guard keeps waiting unless AttackedOuterGateGuards is set
        from tools.script_recovery.native_evidence_lowering import normalise_typed_decompile
        native = ("{\n  if (*(char *)(*(int *)((int)this + 0x14) + 0x48) == '\\0') goto code_r0x00d0283a;\n"
                  "  GSI->GiveThingBestEnemyTarget(me, hero);\n  return;\ncode_r0x00d0283a:\n  GSI->NewScriptFrame();\n}")
        out = normalise_typed_decompile(native)
        self.assertIn('goto LAB_00d0283a;', out)
        self.assertIn('\nLAB_00d0283a:\n', out)
        self.assertNotIn('code_r0x', out)

    def test_existing_lab_of_the_same_address_keeps_the_code_r_name(self):
        from tools.script_recovery.native_evidence_lowering import normalise_typed_decompile
        native = '{\n  goto code_r0x00d0283a;\nLAB_00d0283a:\n  return;\ncode_r0x00d0283a:\n  return;\n}'
        self.assertIn('code_r0x00d0283a', normalise_typed_decompile(native))


class BossKingTests(unittest.TestCase):
    """BanditKingMissionProcess 0x00D109F0: the created Twinblade and the fight's quarter-health threshold."""

    def test_counted_pair_assignment_keeps_the_created_thing(self):
        from tools.script_recovery.native_evidence_lowering import lower_after_annotate
        native = ('{\n  pCVar3 = GSI->CreateCreature(&xStack_2c,&xStack_94,pCVar4,pCVar9,bVar2);\n'
                  '  CCountedPointer<CDiskFileWin32>::operator=__at704580((CCountedPointer<CDiskFileWin32> *)xStack_90,'
                  '(int)&*(int *)(pCVar3 + 0x4));\n  fret_0 = GSI->GetHealth((CScriptThing *)xStack_90);\n}')
        out = lower_after_annotate(native)
        self.assertIn('xStack_90 = (CScriptThing *)pCVar3;', out)
        self.assertNotIn('__at704580', out)

    def test_float_copy_back_judged_over_the_targets_life(self):
        from tools.script_recovery.native_evidence_lowering import normalise_typed_decompile
        native = '\n'.join([
            '{', '  CCharString::CCharString(&xStack_a4,"",-1);',
            '  xStack_a4 = (CCharString)(float)(int)C_stk_4c;', '  C_stk_4c = xStack_a4;',
            '  if ((float)xStack_a4 < fret_01) {', '  }',
            '  CCharString::CCharString(&xStack_a4,"Q_BanditCamp_Barriers",-1);', '  GSI->DeactivateQuest(&xStack_a4,0);',
            '  GSI->ModifyThingHealth(king,(float)C_stk_4c - fret_04,false);', '}'])
        out = normalise_typed_decompile(native)
        self.assertIn('C_stk_4c = f_stk_a4;', out)
        self.assertIn('CCharString::CCharString(&xStack_a4,"Q_BanditCamp_Barriers",-1);', out)   # the string life keeps its name

    def test_copy_whose_target_is_never_read_as_float_keeps_the_slot(self):
        from tools.script_recovery.native_evidence_lowering import normalise_typed_decompile
        native = '\n'.join(['{', '  xStack_a4 = (CCharString)(float)(int)C_stk_4c;', '  pThing = xStack_a4;',
                            '  CCharString::CCharString(&xStack_a4,"x",-1);', '  GSI->Use(pThing);', '}'])
        self.assertIn('pThing = xStack_a4;', normalise_typed_decompile(native))


class CastCaseLabelTests(unittest.TestCase):
    def test_string_typed_integer_labels_compare_as_integers(self):
        statements = ['switch(ctr_c8) {', 'case (CCharString)0x0:', 'GSI->SetTimer(1, 0);', 'break;',
                      'case (CCharString)0x1:', 'GSI->SetTimer(1, 1);', 'break;', '}', 'ctr_c8 = ctr_c8 + 1;']
        lowered, evidence = lower_nonfallthrough_switches(statements)
        self.assertEqual([item['status'] for item in evidence if item.get('status') == 'rejected'], [])
        text = '\n'.join(lowered)
        self.assertNotIn('CCharString', text)
        self.assertNotIn('0x0', text)
        self.assertRegex(text, r'== 0\b')
        self.assertRegex(text, r'== 1\b')


if __name__ == '__main__':
    unittest.main()
