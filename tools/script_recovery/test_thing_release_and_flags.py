import json
import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.convert_quest_unit import out_thing_messages
from tools.script_recovery.native_evidence_lowering import fold_by_value_thing_release, fold_char_flags

# WatchForPickpocketing 0x00E04F10 after annotate: a by-value CScriptThing parameter destroyed inline on two exits
RELEASE = '''    if (bVar1) {
      native_arg_Trader = QUESTTHING_Empty();
      if ((native_arg_Trader._8_4_ != 0) && (*native_arg_Trader._8_4_ = *native_arg_Trader._8_4_ + -1, *native_arg_Trader._8_4_ == 0)) {
        (**(code **)(native_arg_Trader._8_4_ + 4))();
LAB_00e05100:
        operator_delete((void *)native_arg_Trader._8_4_);
      }
LAB_00e05108:
      return;
    }
    cVar2 = CScriptThing::IsEqualTo(xStack_c, native_arg_Trader._4_4_);
LAB_00e050a5:
      native_arg_Trader = QUESTTHING_Empty();
      if ((native_arg_Trader._8_4_ == 0) || (*native_arg_Trader._8_4_ = *native_arg_Trader._8_4_ + -1, *native_arg_Trader._8_4_ != 0))
      goto LAB_00e05108;
      (**(code **)(native_arg_Trader._8_4_ + 4))();
      goto LAB_00e05100;
    if (native_arg_Trader._4_4_ == 0) {
'''


class ByValueThingRelease(unittest.TestCase):
    def setUp(self):
        self.out = fold_by_value_thing_release(RELEASE)

    def test_forward_release_keeps_only_its_label(self):
        self.assertNotIn('operator_delete', self.out)
        self.assertIn('LAB_00e05100:\nLAB_00e05108:', self.out)

    def test_inverted_release_becomes_the_goto(self):
        self.assertIn('native_arg_Trader = QUESTTHING_Empty();\n      goto LAB_00e05108;\n    if (', self.out)
        self.assertNotIn('+ 4))();', self.out)

    def test_data_word_is_validity_and_is_equal_to_takes_the_thing(self):
        self.assertIn('if (!__thing_valid(native_arg_Trader)) {', self.out)
        self.assertIn('CScriptThing::IsEqualTo(xStack_c, native_arg_Trader);', self.out)

    def test_untouched_without_the_release_idiom(self):
        text = 'if (other._4_4_ == 0) {\n'
        self.assertEqual(fold_by_value_thing_release(text), text)


class CharFlags(unittest.TestCase):
    def test_flag_literals_become_booleans(self):
        text = ("    if (x) { cVar2 = '\\0'; } else { cVar2 = MsgIsKilledBy(t); }\n"
                "    if (cVar2 != '\\0') { f(); }\n    if (cVar2 == '\\0') { g(); }\n    cVar2 = '\\x01';\n")
        out = fold_char_flags(text)
        self.assertIn('cVar2 = false;', out)
        self.assertIn('if (cVar2) { f(); }', out)
        self.assertIn('if (!cVar2) { g(); }', out)
        self.assertIn('cVar2 = true;', out)

    def test_byte_local_with_numeric_stores(self):
        # WaspHelper Main: `undefined1 uVar2;` stored 0/1 and tested `(bool)uVar2`
        text = ("  undefined1 uVar2;\n    uVar2 = 0;\n    if (!(bool)uVar2) {\n      say();\n      uVar2 = 1;\n    }\n"
                "    if ((bool)uVar2) { uVar2 = 0; }\n")
        out = fold_char_flags(text)
        self.assertIn('uVar2 = false;', out)
        self.assertIn('if (!uVar2) {', out)
        self.assertIn('uVar2 = true;', out)
        self.assertIn('if (uVar2) { uVar2 = false; }', out)

    def test_byte_local_storing_a_value_is_left_alone(self):
        text = "  undefined1 uVar4;\n    uVar4 = GetCount();\n    if ((bool)uVar4) { f(); }\n    uVar4 = 0;\n"
        self.assertIn('uVar4 = 0;', fold_char_flags(text))

    def test_arithmetic_use_keeps_numbers(self):
        text = "    cVar3 = '\\0';\n    if (cVar3 != '\\0') { n = cVar3 + 1; }\n"
        self.assertEqual(fold_char_flags(text), text)

    def test_field_is_not_a_local_flag(self):
        text = "    if (this->done != '\\0') { f(); }\n"
        self.assertEqual(fold_char_flags(text), text)


class LowByteFlagsAndDwordColours(unittest.TestCase):
    def test_low_byte_flag_gets_its_own_local(self):
        from tools.script_recovery.native_evidence_lowering import fold_low_byte_flags
        text = ("  brain_state = CONCAT31(brain_state._1_3_,bVar1);\n  if ((char)brain_state == '\\0') {\n"
                "  iVar4 = CONCAT31((int3)((uint)extraout_EAX >> 8),(char)brain_state);\n")
        out = fold_low_byte_flags(text)
        self.assertIn('brain_state_flag = bVar1;', out)
        self.assertIn("if (brain_state_flag == '\\0') {", out)
        self.assertIn('iVar4 = brain_state_flag;', out)
        self.assertNotIn('extraout_EAX', out)

    def test_dword_colour_literal_used_as_colour(self):
        from tools.script_recovery.native_evidence_lowering import fold_dword_colours
        text = "  brain_state = -0x10000;\n  iVar4 = GSI->AddQuestInfoBarHealth(pFollower,(CRGBColour_bv *)&brain_state,&s,1.0);\n"
        self.assertIn('ENGINE_Colour(255, 0, 0, 255)', fold_dword_colours(text))

    def test_dword_used_otherwise_is_left_alone(self):
        from tools.script_recovery.native_evidence_lowering import fold_dword_colours
        text = "  x = -0x10000;\n  f((CRGBColour_bv *)&x);\n  g(x + 1);\n"
        self.assertEqual(fold_dword_colours(text), text)


class OutThingMessages(unittest.TestCase):
    def test_bool_slot_with_thing_out_and_object_binding(self):
        spec = {'slots': {
            '0xd4': {'name': 'MsgOnHeroPickedPocket', 'ret': 'bool', 'params': [{'name': 'p', 'type': 'CScriptThing *'}]},
            '0x10': {'name': 'MsgIsSomething', 'ret': 'bool', 'params': [{'name': 'p', 'type': 'CScriptThing *'}]},
            '0x20': {'name': 'GetThing', 'ret': 'CScriptThing *', 'params': [{'name': 'p', 'type': 'CScriptThing *'}]}}}
        manifest = {
            'MsgOnHeroPickedPocket': {'returnType': 'sol::object', 'parameters': [{'name': 's', 'type': 'sol::this_state'}]},
            'MsgIsSomething': {'returnType': 'bool', 'parameters': [{'name': 't', 'type': 'CScriptThing*'}]},
            'GetThing': {'returnType': 'sol::object', 'parameters': [{'name': 's', 'type': 'sol::this_state'}]}}
        with tempfile.TemporaryDirectory() as d:
            path = Path(d) / 'typing_spec.json'
            path.write_text(json.dumps(spec), encoding='utf-8')
            self.assertEqual(out_thing_messages(path, manifest), frozenset({'MsgOnHeroPickedPocket'}))


class FlagRelay(unittest.TestCase):
    # DarkwoodTrader Main 0x00E07640: the slot-shared destruction flags reloaded through a register
    TEXT = '''  CStack_170 = 0;
  CStack_170 = CStack_170 | 2;
  if ((CStack_170 & 2) != 0) {
    CStack_170 = CStack_170 & 0xfffffffd;
  }
  CVar16 = CStack_170;
  if ((CStack_170 & 0x200) != 0) {
    CVar16 = CStack_170 & 0xfffffdff;
  }
  if (((CVar16 & 0x80) != 0)) {
  }
  CVar16 = CStack_170;
  CStack_170 = CStack_170 | 0x2000;
  CStack_170 = CVar16 | 0x6000;
  if ((CStack_170 & 0x4000) != 0) {
    CStack_170 = CStack_170 & 0xffffbfff;
  }
  CVar16 = *(int **)(this + 4);
  CCharString::CCharString((CCharString *)&CStack_170,"DarkwoodTrader",-1);
'''

    def test_relay_lines_move_onto_the_slot(self):
        from tools.script_recovery.native_evidence_lowering import fold_flag_relays
        out = fold_flag_relays(self.TEXT, {'CStack_170'})
        self.assertNotIn('CVar16 = CStack_170', out)
        self.assertIn('CStack_170 = CStack_170 | 0x6000;', out)
        self.assertIn('if ((CStack_170 & 0x80) != 0) {', out)
        self.assertIn('CVar16 = *(int **)(this + 4);', out)     # the register's own later life is untouched

    def test_register_used_otherwise_keeps_the_copy(self):
        from tools.script_recovery.native_evidence_lowering import fold_flag_relays
        text = self.TEXT.replace('  CStack_170 = CVar16 | 0x6000;\n', '  f(CVar16);\n')
        self.assertIn('  CVar16 = CStack_170;\n  CStack_170 = CStack_170 | 0x2000;', fold_flag_relays(text, {'CStack_170'}))

    def test_whole_flag_word_is_dropped(self):
        from tools.script_recovery.native_evidence_lowering import drop_eh_state_flags
        text = self.TEXT.replace('CStack_170 = CStack_170 | 2;', 'CStack_170 = (CCharString)((uint)CStack_170 | 2);')
        out = drop_eh_state_flags(text)
        self.assertNotRegex(out, r'CStack_170 [|&]|ehflag_')
        self.assertIn('CCharString::CCharString((CCharString *)&CStack_170', out)


class StackThingOwnVtable(unittest.TestCase):
    SLOTS = {0x12c: ('IsAlive', '?IsAlive@CScriptThing@@UBE_NXZ')}

    def test_filled_stack_thing_resolves(self):
        from tools.script_recovery.native_evidence_lowering import lower_after_annotate
        text = ('  CScriptThing xStack_24;\n  GSI->CreateCreature(&xStack_24,&xStack_44,pCVar7,pScriptName,bVar5);\n'
                '  cVar6 = (**(code **)(xStack_24._0_4_ + 0x12c))();\n  if (cVar6 == 0) { f(); }\n')
        self.assertIn('cVar6 = CScriptThing::IsAlive(xStack_24);', lower_after_annotate(text, self.SLOTS))

    def test_never_filled_stack_thing_stays(self):
        from tools.script_recovery.native_evidence_lowering import lower_after_annotate
        text = '  CScriptThing CStack_d8;\n  cVar4 = (**(code **)(CStack_d8._0_4_ + 0x12c))();\n  if (cVar4 == 0) { f(); }\n'
        self.assertIn('(**(code **)(CStack_d8._0_4_ + 0x12c))()', lower_after_annotate(text, self.SLOTS))


class InlineStringEquality(unittest.TestCase):
    # DarkwoodTrader Main's TraderToTalk test, in the spelling the typed export prints (CCharString reps)
    TYPED = '''            pCVar13 = CScriptThing::GetDataString((CScriptThing *)(this + 8), &xStack_34);
            CVar15 = *pCVar13;
            CVar1 = QUESTSTATE_GetString("TraderToTalk");
            if (CVar1 == CVar15) {
              c_stk_171 = '\\x01';
            }
            else if ((CVar1 == (CCharString)0x0) || (CVar15 == (CCharString)0x0)) {
              c_stk_171 = '\\0';
            }
            else if (*(int *)((int)CVar1 + 4) == *(int *)((int)CVar15 + 4)) {
              iVar8 = CBasicString<char>::Compare(*(void **)CVar1,*(void **)CVar15);
              c_stk_171 = !(iVar8 != 0);
            }
            else {
              c_stk_171 = '\\0';
            }
            f();
            if (c_stk_171 == '\\0') {
'''

    def test_typed_spelling_folds(self):
        from tools.script_recovery.native_evidence_lowering import fold_inline_string_equality
        out = fold_inline_string_equality(self.TYPED)
        self.assertIn('c_stk_171 = ENGINE_StrEq(pCVar13, QUESTSTATE_GetString("TraderToTalk"));', out)
        self.assertIn('if (!c_stk_171) {', out)
        self.assertNotIn('CBasicString<char>::Compare', out)


class MovieStartsOnce(unittest.TestCase):
    # WatchForMissionRules 0x00E06440: the movie object operand is printed with a pointer cast
    def test_cast_movie_operand_drops_the_second_start(self):
        from tools.script_recovery.native_evidence_lowering import lower_after_annotate
        text = ('    xStack_50 = RESOURCE_StartMovie("");\n'
                '    CCharString::CCharString(&xStack_f0,"");\n'
                '    GSI->StartMovieSequence(&xStack_f0,(CScriptThing *)xStack_50);\n'
                '    GSI->PauseAllNonScriptedEntities(true);\n')
        out = lower_after_annotate(text)
        self.assertIn('RESOURCE_StartMovie', out)
        self.assertNotIn('StartMovieSequence', out)


if __name__ == '__main__':
    unittest.main()
