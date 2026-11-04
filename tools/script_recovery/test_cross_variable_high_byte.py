"""One byte flag printed under two dwords (Arena Roth Main 0x00F21880, `mov byte ptr [esp+0x13], 1`)."""
from tools.script_recovery.native_evidence_lowering import fold_cross_variable_high_byte_flags

ROTH = ('    pCVar22 = (CCharString *)CONCAT13(bVar3,(int3)in_stack_fffffeb4);\r\n'
        '    if ((char)((uint)pCVar22 >> 0x18) == \'\\0\') {\r\n'
        '      if (bVar3) {\r\n'
        '        in_stack_fffffeb4 = (CCharString *)CONCAT13(1,(int3)pCVar22);\r\n'
        '      }\r\n'
        '      else {\r\n'
        '        in_stack_fffffeb4 = (CCharString *)((uint)pCVar22 & 0xffffff);\r\n'
        '      }\r\n'
        '      Cleanup();\r\n'
        '      if ((char)((uint)in_stack_fffffeb4 >> 0x18) != \'\\0\') {\r\n')


def test_store_alternative_and_test_become_one_boolean():
    out = fold_cross_variable_high_byte_flags(ROTH)
    assert 'hb_stk_f001 = bVar3;' in out and "if ((char)hb_stk_f001 == '\\0')" not in out
    assert 'hb_stk_f002 = true;' in out and 'hb_stk_f002 = false;' in out
    assert 'CONCAT13' not in out and '>> 0x18' not in out


def test_another_use_of_either_name_in_the_window_keeps_the_source():
    text = ROTH.replace('      Cleanup();', '      Use(pCVar22);')
    out = fold_cross_variable_high_byte_flags(text)
    assert 'in_stack_fffffeb4 = (CCharString *)CONCAT13(1,(int3)pCVar22);' in out



def test_bully_pointer_casts_and_mask_spellings():
    # E5C199/E5C1A5/E5C1B1/E5C1C2: independent byte at esp+13.
    for cast in ('', '(undefined ***)'):
        for mask in ('(uint)low & 0xffffff', '((uint)low & 0xffffff)', '(low & 0xffffff)'):
            text = (f'  word = {cast}CONCAT13(1,(int3)low);\n'
                    f'  if (health <= 0.0) {{\n    word = {cast}{mask};\n  }}\n'
                    "  Cleanup();\n  if ((char)((uint)word >> 0x18) != '\\0') {\n")
            out = fold_cross_variable_high_byte_flags(text)
            assert 'hb_stk_f001 = true;' in out
            assert 'hb_stk_f001 = false;' in out
            assert 'CONCAT13' not in out and '>> 0x18' not in out
            assert 'Cleanup();' in out


def test_pointer_mask_other_uses_and_nonzero_high_byte_remain_unresolved():
    original = ("  word = (undefined ***)CONCAT13(1,(int3)low);\n"
                "  if (health <= 0.0) {\n    word = (undefined ***)(low & 0xffffff);\n  }\n"
                "  Cleanup();\n  if ((char)((uint)word >> 0x18) != '\\0') {\n")
    for text in (original.replace('Cleanup();', 'Use(low);'),
                 original.replace('Cleanup();', 'Use(word);'),
                 original.replace('0xffffff', '0x1ffffff'),
                 original.replace('(low &', '(other &')):
        assert fold_cross_variable_high_byte_flags(text) == text
