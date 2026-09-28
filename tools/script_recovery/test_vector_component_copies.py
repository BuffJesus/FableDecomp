"""Positions copied member-wise into stack slots (V_ChickenKicking KickedChicken Main 0x00E64210, foul-line test)."""
from tools.script_recovery.native_vector_component_copies import fold_vector_component_copies

SOURCE = '''{
  C3DVector_bv *pCVar4;
  pCVar4 = (**(code **)(*(int *)pCVar3 + 0x18))(pCVar3);
  piStack_3c = *(int **)pCVar4;
  piStack_38 = *(int **)&pCVar4->field_0x4;
  uStack_34 = *(undefined4 *)&pCVar4->field_0x8;
  pCVar4 = (**(code **)(*(int *)pCVar3 + 0x18))(pCVar3);
  auStack_30 = *(undefined1 (*) [4])pCVar4;
  fStack_2c = *(float *)&pCVar4->field_0x4;
  piStack_28 = *(int **)&pCVar4->field_0x8;
  if ((fStack_2c - (float)piStack_38) * ((float)piStack_3c - (float)auStack_30) <= 0.0) {
  }
  F(x,(CScriptThing_bv *)auStack_30,&name);
  G((float)auStack_30);
  piStack_3c = (int *)0x0;
  H((float)piStack_3c);
}
'''


def test_slots_read_as_vector_components():
    out = fold_vector_component_copies(SOURCE)
    assert 'vec_3c = ENGINE_VectorCopy(pCVar4);' in out and 'vec_30 = ENGINE_VectorCopy(pCVar4);' in out
    assert 'if ((vec_30.y - vec_3c.y) * (vec_3c.x - vec_30.x) <= 0.0)' in out


def test_substitution_stops_at_reuse_and_restore():
    out = fold_vector_component_copies(SOURCE)
    # the slot reused as a hidden thing result ends the component's life; a later store likewise
    assert 'F(x,(CScriptThing_bv *)auStack_30,&name);' in out and 'G((float)auStack_30);' in out
    assert 'H((float)piStack_3c);' in out


def test_a_triple_that_is_not_offsets_0_4_8_is_left_alone():
    text = SOURCE.replace('&pCVar4->field_0x8', '&pCVar4->field_0xc')
    assert 'ENGINE_VectorCopy' not in fold_vector_component_copies(text)


def test_dword_stored_as_four_bytes_is_one_store():
    from tools.script_recovery.native_vector_component_copies import fold_split_dword_stores
    text = ('  thing._8_1_ = (char)piVar2;\n  thing._9_1_ = (char)((uint)piVar2 >> 8);\n'
            '  thing._10_1_ = (char)((uint)piVar2 >> 0x10);\n  thing._11_1_ = (char)((uint)piVar2 >> 0x18);\n')
    assert fold_split_dword_stores(text) == '  thing._8_4_ = piVar2;\n'
    assert fold_split_dword_stores(text.replace('>> 0x10);', '>> 0x10);\n  x = 1;')) != '  thing._8_4_ = piVar2;\n'


def test_counter_split_from_a_string_register_is_assigned():
    from tools.script_recovery import test_lift_native_lua as fx
    body = '\n'.join(fx.make().lift('Main', '{\n  CCharString ctr_CVar19;\n  ctr_CVar19 = 0;\n  ctr_CVar19 = ctr_CVar19 + 0x64;\n}'))
    assert 'ctr_CVar19 = ctr_CVar19 + 0x64' in body and 'TODO' not in body
