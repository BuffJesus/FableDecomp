"""High-byte flag slots Ghidra prints with `(undefined3)` and `._3_1_` (V_SickChild WomanToAttract Main, V_BeggarAndChild
BeggarBully Main), and a string pointer loaded once per stretch for a run of compares (WomanToAttract's expression
reactions)."""
from tools.script_recovery.native_evidence_lowering import fold_high_byte_flags, fold_name_compare


def test_undefined3_flag_slot_is_its_own_boolean():
    text = ('  xStack_9c = pCVar9;\n'
            '  CCharString::CCharString(xStack_9c,"SCRIPT_NAME_HERO",-1);\n'
            "  xStack_9c = (CScriptThing_bv *)CONCAT13(cVar4,(undefined3)xStack_9c);\n"
            '  xStack_9c = (CScriptThing_bv *)((uint)xStack_9c & 0xffffff);\n'
            "  if (xStack_9c._3_1_ != '\\0') {\n")
    out = fold_high_byte_flags(text)
    assert "hb_stk_9c = cVar4 != '\\0';" in out
    assert 'hb_stk_9c = false;' in out
    assert 'if (hb_stk_9c) {' in out
    assert 'CONCAT13' not in out


def test_a_slot_whose_dword_is_read_is_not_a_flag():
    # WomanToAttract `uStack_a8`: a cleanup mask read back (`uVar4 = xStack_a8`, `| 2`)
    text = ('  uVar4 = xStack_a8;\n'
            '  uVar13 = xStack_a8 | 2;\n'
            '  xStack_a8 = CONCAT13(1,(undefined3)xStack_a8);\n'
            "  if (xStack_a8._3_1_ != '\\0') {\n")
    assert fold_high_byte_flags(text) == text


def test_a_clear_through_a_copy_of_the_slot_is_the_slots_own_clear():
    text = ('  uVar2 = (uint)xStack_a8;\n'
            '  xStack_a8 = (CScriptThing_bv *)CONCAT13(1,(undefined3)xStack_a8);\n'
            '  if (fVar14 <= 0.0) {\n'
            '    xStack_a8 = (CScriptThing_bv *)(uVar2 & 0xffffff);\n'
            '  }\n'
            "  if (xStack_a8._3_1_ != '\\0') {\n")
    out = fold_high_byte_flags(text)
    assert 'uVar2' not in out
    assert out.count('hb_stk_a8 = true;') == 1 and out.count('hb_stk_a8 = false;') == 1
    assert 'if (hb_stk_a8) {' in out


def test_a_constant_dword_store_sets_the_flag_and_a_compound_test_reads_it():
    text = ('  xStack_128 = 0;\n'
            "  if ((xStack_128._3_1_ != '\\0') || (bVar1 != '\\0')) {\n"
            '  xStack_128 = CONCAT13(1,(undefined3)xStack_128);\n')
    out = fold_high_byte_flags(text)
    assert 'xStack_128 = 0;\n  hb_stk_128 = false;' in out
    assert 'if ((hb_stk_128) || (bVar1' in out
    assert 'hb_stk_128 = true;' in out


def test_compares_fold_per_stretch_of_a_reassigned_pointer():
    text = ('  pvVar11 = *(void **)pCVar19;\n'
            '  iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_SHIT");\n'
            '  iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_FLIRT");\n'
            'LAB_1:\n'
            '  pvVar11 = CCharString::operator_char_const_((CCharString *)&uStack_f4);\n'
            '  F(pvVar11);\n'
            '  pvVar11 = *(void **)pCVar19;\n'
            '  iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_GIGGLE");\n')
    out = fold_name_compare(text)
    assert 'ENGINE_StrCmp(pCVar19, "EXPRESSION_SHIT")' in out and 'ENGINE_StrCmp(pCVar19, "EXPRESSION_FLIRT")' in out
    assert 'ENGINE_StrCmp(pCVar19, "EXPRESSION_GIGGLE")' in out
    assert 'F(pvVar11);' in out and 'Compare' not in out


def test_a_label_inside_the_stretch_keeps_a_reassigned_pointer():
    text = ('  pvVar11 = *(void **)pCVar19;\n'
            'LAB_1:\n'
            '  iVar5 = CBasicString<char>::Compare(pvVar11,"A");\n'
            '  pvVar11 = *(void **)pCVar20;\n'
            '  goto LAB_1;\n')
    assert fold_name_compare(text) == text
