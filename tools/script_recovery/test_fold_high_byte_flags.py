"""A flag kept in a dword slot's high byte becomes its own boolean local."""
from tools.script_recovery.native_evidence_lowering import fold_high_byte_flags

# Arena ArenaCellDoorGuard2 Main 0x00F19BB0
C = '''    in_stack_ffffff34 = (CCharString *)CONCAT13(1,(int3)in_stack_ffffff34);
    in_stack_ffffff34 = (CCharString *)((uint)in_stack_ffffff34 & 0xffffff);
    if ((char)((uint)in_stack_ffffff34 >> 0x18) == '\\0') {
    Call(pThing,in_stack_ffffff34,0);'''


def test_high_byte_stores_and_tests_become_a_boolean():
    out = fold_high_byte_flags(C)
    assert 'hb_stk_ffffff34 = true;' in out
    assert 'hb_stk_ffffff34 = false;' in out
    assert "if (hb_stk_ffffff34 == '\\0') {" in out
    assert 'Call(pThing,in_stack_ffffff34,0);' in out        # other uses of the slot are untouched


def test_slots_without_a_high_byte_set_are_left_alone():
    text = "    x = (uint)x & 0xffffff;\n    if ((char)((uint)x >> 0x18) == '\\0') {"
    assert fold_high_byte_flags(text) == text
