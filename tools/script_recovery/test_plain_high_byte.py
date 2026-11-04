import pytest
from tools.script_recovery.native_evidence_lowering import fold_high_byte_flags

FLAG="  uStack_220 = uStack_220 & 0xffffff;\n  if (uStack_220._3_1_ == '\\0') {\n    uStack_220 = CONCAT13(1,(undefined3)uStack_220);\n  }\n  uStack_220 = 0;\n"
def test_plain_clear_preserves_independent_high_byte():
 out=fold_high_byte_flags(FLAG)
 assert 'hb_stk_220 = false;' in out and 'if (!hb_stk_220)' in out
 assert 'hb_stk_220 = true;' in out
 assert 'uStack_220 = 0;\n  hb_stk_220 = false;' in out
 assert 'CONCAT13' not in out and '._3_1_' not in out

@pytest.mark.parametrize('use',[
 'uVar4 = uStack_220;',
 'uVar4 = (uint)uStack_220;',
 'uVar4 = uStack_220 | 2;',
 'uStack_220 = uStack_220 & 0xffffff | 2;',
 'uStack_220 = uStack_220 + 1;',
 'uStack_220 = 0x41800000;',
 'uStack_220 = 4;',
 'Use(&uStack_220);',
 'uStack_220 = CONCAT13(1,CONCAT12(1,(short)uStack_220));',
])
def test_live_word_or_multiple_flag_bytes_remain_unchanged(use):
 source=FLAG+use+'\n'
 assert fold_high_byte_flags(source)==source
