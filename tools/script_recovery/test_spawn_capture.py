import unittest

from tools.script_recovery.lift_native_lua import RE_FLAG_GUARD, RE_THREAD_CAPTURE, _captured_entity_self

# DarkwoodTrader Init 0x00E04BD0, as the lifter sees it (field-wise copy of the entity's own thing)
INIT = '''  this_00 = operator_new(0x48);
  if (this_00 == (CSpawnedFunc<NScript::CExpression_FollowScript> *)0x0) {
    this_00 = (CSpawnedFunc<NScript::CExpression_FollowScript> *)0x0;
    auVar5 = v_stk_20;
  }
  else {
    xStack_c._4_4_ = *(undefined4 *)(this + 0xc);
    xStack_c._8_4_ = *(undefined4 *)(this + 0x10);
    xStack_c = QUESTTHING_Empty();
    if (xStack_c._8_4_ != (int *)0x0) {
      *xStack_c._8_4_ = *xStack_c._8_4_ + 1;
    }
    uVar1 = *(undefined4 *)(this + 0x14);
    CCharString::CCharString(xStack_10,"WatchForPickpocketing",-1);
    pCVar3 = extraout_EAX;
    CCharString::CCharString(xStack_14,"ParentClass.",-1);
    pCVar3 = ENGINE_Concat(a, pCVar3);
    CSpawnedFunc<NScript::CExpression_FollowScript>::CSpawnedFunc<NScript::CExpression_FollowScript> (this_00,pCVar3,0);
    *(undefined ***)this_00 = &PTR__scalar_deleting_destructor__012df764;
    *(code **)(this_00 + 0x34) = NScript::CQ_TraderEscortScript::WatchForPickpocketing;
    *(undefined4 *)(this_00 + 0x38) = uVar1;
    CScriptThing::CScriptThing((CScriptThing *)(this_00 + 0x3c),(CScriptThing *)xStack_c,p1);
    auVar5 = 7;
  }
  CCharString::CCharString(&xStack_20,"",-1);
  CGuiVarTransferStruct::Add(*(CGuiVarTransferStruct **)(this + 0x14),this_00,sectionName);
  if ((auVar5 & 4) != 0) {
    auVar5 = ((uint)auVar5 & 0xfffffffb);
  }
  return;
'''

# FUN_00e07640's TurnToBalv spawn (copy-constructed from p0 = this + 8)
BALV = '''  p0 = (CScriptThing_bv *)(this + 8);
  this_01 = operator_new(0x48);
  if (this_01 == (CSpawnedFunc<NScript::CExpression_FollowScript> *)0x0) {
    this_01 = (CSpawnedFunc<NScript::CExpression_FollowScript> *)0x0;
  }
  else {
    CScriptThing::CScriptThing((CScriptThing *)xStack_c0,p0,unaff_EDI);
    uVar15 = *(undefined4 *)(this + 0x14);
    CCharString::CCharString(xStack_b0,"TurnToBalv",-1);
    pCVar11 = extraout_EAX;
    CCharString::CCharString(&xStack_c8,"ParentClass.",-1);
    pCVar11 = ENGINE_Concat(a, pCVar11);
    CSpawnedFunc<NScript::CExpression_FollowScript>::CSpawnedFunc<NScript::CExpression_FollowScript>(this_01,pCVar11,0);
    *(undefined ***)this_01 = &PTR__scalar_deleting_destructor__012df764;
    *(code **)(this_01 + 0x34) = Script_Darkwood_Balverine_Trader;
    *(undefined4 *)(this_01 + 0x38) = uVar15;
    CScriptThing::CScriptThing((CScriptThing *)(this_01 + 0x3c),(CScriptThing *)xStack_c0,unaff_EDI);
  }
  CCharString::CCharString(&xStack_c4,"",-1);
  CGuiVarTransferStruct::Add(*(CGuiVarTransferStruct **)(this + 0x14),this_01,sectionName);
  if ((xStack_170 & 0x200) != 0) {
    CVar14 = xStack_170 & 0xfffffdff;
  }
'''


class SpawnCaptureTests(unittest.TestCase):
    def test_fieldwise_entity_copy(self):
        m = RE_THREAD_CAPTURE.search(INIT)
        self.assertIsNotNone(m)
        self.assertEqual(m.group('name'), 'WatchForPickpocketing')
        self.assertTrue(_captured_entity_self(m.group('body'), m.group('src'), INIT))
        self.assertEqual(RE_FLAG_GUARD.match(INIT, m.end()).group('flag'), 'auVar5')

    def test_copy_constructed_from_p0_uses_the_name_literal(self):
        m = RE_THREAD_CAPTURE.search(BALV)
        self.assertIsNotNone(m)
        self.assertEqual(m.group('name'), 'TurnToBalv')
        self.assertTrue(_captured_entity_self(m.group('body'), m.group('src'), BALV))

    def test_foreign_capture_is_not_the_entity(self):
        text = BALV.replace('p0 = (CScriptThing_bv *)(this + 8);', 'p0 = pOther;')
        m = RE_THREAD_CAPTURE.search(text)
        self.assertFalse(_captured_entity_self(m.group('body'), m.group('src'), text))
        text = INIT.replace('(this + 0x10)', '(pOther + 0x10)')
        m = RE_THREAD_CAPTURE.search(text)
        self.assertFalse(_captured_entity_self(m.group('body'), m.group('src'), text))


if __name__ == '__main__':
    unittest.main()
