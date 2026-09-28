"""A stack thing's Data word printed as its own local (V_BookCollecting BS_Teacher 0x00E56D10)."""
from tools.script_recovery.native_split_thing_words import merge_split_thing_words

SOURCE = ('{\n  undefined1 auStack_18 [4];\n  undefined4 uStack_14;\n  int *piStack_10;\n\n'
          '  GSI->Find((CScriptThing_bv *)auStack_18,&name);\n  while (uStack_14 != (int *)0x0) {\n'
          '    cVar3 = (**(code **)(*uStack_14 + 300))();\n    uStack_14 = piVar2;\n  }\n}\n')


def test_data_word_becomes_the_things_field():
    out = merge_split_thing_words(SOURCE)
    assert 'undefined4 uStack_14;' not in out and 'uStack_14' not in out
    assert 'while (auStack_18._4_4_ != (int *)0x0)' in out
    assert '(**(code **)(*(int *)auStack_18._4_4_ + 300))()' in out
    assert '  int *piStack_10;\n\n' in out          # the declaration block stays contiguous


def test_a_word_with_other_uses_is_left_alone():
    text = SOURCE.replace('uStack_14 = piVar2;', 'Use(uStack_14);')
    assert merge_split_thing_words(text) == text
