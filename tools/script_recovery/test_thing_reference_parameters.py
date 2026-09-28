"""`CScriptThing const &` / `C3DVector const &` parameters (V_BeardyBaldy SetWanderPointAndDistance 0x00E53F60)."""
from tools.script_recovery.native_evidence_lowering import fold_thing_staged_from_reference
from tools.script_recovery.native_function_parameters import function_parameters

HEADER = '''/* [bsim sim=0.7480887365766618 <- ego_r]
   public: void __thiscall NScript::CV_BeardyBaldyScript::SetWanderPointAndDistance(class
   CScriptThing const &,class C3DVector const &) */

void __thiscall
NScript::CV_BeardyBaldyScript::SetWanderPointAndDistance
          (CV_BeardyBaldyScript *this,int param_1,int param_2)

{
}
'''

STAGED = '''  uVar1 = *(undefined4 *)(p + 4);
  piVar2 = *(int **)(p + 8);
  if (piVar2 != (int *)0x0) {
    *piVar2 = *piVar2 + 1;
  }
  thing._4_4_ = uVar1;
  thing._0_4_ = &PTR__scalar_deleting_destructor__01238c8c;
  thing._8_4_ = piVar2;
  (**(code **)(**(int **)(this + 0x40) + 0xbec))(*(int **)(this + 0x40),thing,center);
  uVar1 = *(undefined4 *)(p + 4);
'''


def test_reviewed_reference_types_outrank_ghidra_int():
    sig = function_parameters(HEADER, member=True)
    assert [p['type'] for p in sig['parameters']] == ['CScriptThing *', 'C3DVector *']
    assert sig['bsimVoid']


def test_reviewed_types_need_a_matching_count():
    short = HEADER.replace('class\n   CScriptThing const &,class C3DVector const &', 'class CScriptThing const &')
    assert [p['type'] for p in function_parameters(short, member=True)['parameters']] == ['int', 'int']


def test_by_value_copy_from_a_reference_parameter_is_the_parameter():
    out = fold_thing_staged_from_reference(STAGED)
    assert out.startswith('  thing = (CScriptThing *)p;\n  (**(code **)')


def test_a_temporary_read_later_keeps_the_staging():
    assert fold_thing_staged_from_reference(STAGED + '  Use(piVar2);\n') == STAGED + '  Use(piVar2);\n'


def test_discarded_squared_distance_is_the_bare_call():
    from tools.script_recovery.native_evidence_lowering import finish_lua
    out = finish_lua('function Main(quest, me)\n    local d = ENGINE_SquaredDistance(a, b)\n    ENGINE_SquaredDistance(a, c)\nend\n')
    assert 'local d = (quest:GetDistanceBetweenThings(a, b) ^ 2)' in out
    assert '\n    quest:GetDistanceBetweenThings(a, c)\n' in out


def test_string_getter_through_a_hidden_slot_assigns_it():
    from tools.script_recovery.native_evidence_lowering import lower_after_annotate
    out = lower_after_annotate('    CScriptThing::GetDataString(xStack_30, &native_arg_speaker);\n')
    assert 'native_arg_speaker = CScriptThing::GetDataString(xStack_30);' in out
