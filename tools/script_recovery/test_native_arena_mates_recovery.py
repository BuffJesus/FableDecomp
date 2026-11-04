import capstone
from capstone import x86

from tools.script_recovery.lift_native_lua import Lifter, load_manifest
from tools.script_recovery.native_receiver_reaching import _cfg, _is_padding, _reaching
from tools.script_recovery.test_lift_native_lua import NoRData


def _decode(code, base=0x1000):
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    return list(md.disasm(bytes.fromhex(code), base))


def _edi_defs_at_push(code):
    ins = _decode(code)
    push = next(k for k, i in enumerate(ins) if i.mnemonic == 'push')
    return _reaching(ins, _cfg(ins, x86, capstone), push, x86.X86_REG_EDI, x86)


def test_alignment_padding_after_jmp_is_not_an_entry_path():
    # xor edi,edi; jmp L; lea ecx,[ecx] (filler, never executed); L: push edi; ret  (Needle F15B8B..F15B90)
    assert _edi_defs_at_push('33ff' 'eb02' '8d09' '57' 'c3') == {0}
    # a predecessor-less instruction that is not filler still reports the unknown entry
    assert -1 in _edi_defs_at_push('33ff' 'eb02' '8bc8' '57' 'c3')


def test_padding_idioms():
    assert all(_is_padding(i) for i in _decode('8d09' '8d642400' '90' 'cc' '8bff'))
    assert not any(_is_padding(i) for i in _decode('8d4c2410' '8bc8'))


def _lift(source):
    lifter = Lifter(load_manifest(), {}, 'quest', False, 'Pkg', NoRData())
    lifter.accessor_kinds = True
    return '\n'.join(lifter.lift('Main', source))


def test_else_arm_sees_kinds_at_the_if():
    # Flick 0x00F14A90: iVar9 is HintNumber at the switch, an IsPerformingScriptTask result inside case 0
    out = _lift('''{
  iVar9 = QUESTSTATE_GetInt("HintNumber");
  if (iVar9 == 0) {
    iVar9 = GSI->IsActiveThreadTerminating();
    cVar6 = (char)iVar9;
  }
  else {
    if (iVar9 != 1) {
      GSI->SetStateInt("Seen", 2);
    }
  }
}''')
    assert 'if iVar9 ~= 1 then' in out and 'not iVar9' not in out


def test_char_flag_stores_stay_numeric():
    # DoorGuard 0x00F17C70: c_stk_1f5 starts as a bool result, then is toggled with char literals
    out = _lift('''{
  char c_stk_1f5;
  bVar3 = GSI->IsActiveThreadTerminating();
  c_stk_1f5 = bVar3;
  if (c_stk_1f5 == '\\0') {
    c_stk_1f5 = '\\x01';
  }
}''')
    assert 'c_stk_1f5 = (bVar3) and 1 or 0' in out and 'if c_stk_1f5 == 0 then' in out


def test_char_slot_also_stored_from_a_call_keeps_boolean_lifting():
    # DarkwoodTrader 0x00DFBB60 reuses c_stk_169 for call results tested as booleans
    out = _lift('''{
  char c_stk_169;
  c_stk_169 = GSI->IsActiveThreadTerminating();
  if (c_stk_169 == '\\0') {
    c_stk_169 = '\\x01';
  }
  bVar3 = GSI->IsActiveThreadTerminating();
  c_stk_169 = bVar3;
}''')
    assert 'and 1 or 0' not in out


def test_literal_assigned_string_slot_is_a_variable_with_its_start_value():
    # Arena GivePrizeFund 0x00F21300: built "", then one operator= per round in a switch
    out = _lift('''{
  CCharString::CCharString(&xStack_4,"",-1);
  iVar1 = QUESTSTATE_GetInt("GoldMultiplier");
  if (iVar1 == 0) {
    CCharString::operator=((CCharString *)&xStack_4,"PRIZE_1");
  }
  else {
    CCharString::operator=((CCharString *)&xStack_4,"PRIZE_2");
  }
  GSI->AddLineToConversation(iVar2,&xStack_4,false,pCVar5,pCVar5);
}''')
    assert 'xStack_4 = ""' in out and 'xStack_4 = "PRIZE_1"' in out and 'xStack_4 = "PRIZE_2"' in out
    assert 'TODO' not in out and 'AddLineToConversation(iVar2, xStack_4,' in out
