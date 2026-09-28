"""V_BookCollecting BS_Teacher AskForBook 0x00E55CE0: lost parameter, member-resource Speak, and its callers."""
import json
from pathlib import Path

from tools.script_recovery.native_evidence_lowering import LoweringSpec, lower, return_misattached_getter_operands
from tools.script_recovery.native_function_parameters import function_parameters

ROOT = Path(__file__).resolve().parents[2]
UNIT = ROOT / 'refs/script_recovery/book_collecting'


def typed(address):
    d = json.loads((UNIT / 'translation_unit_typed.json').read_text(encoding='utf-8-sig'))
    return next(f for f in d['functions'] if f['address'].lower() == address)


def test_reviewed_prototype_restores_the_dropped_parameter():
    sig = function_parameters(typed('0x00e55ce0')['decompile'], member=True)
    assert [(p['native'], p['lua'], p['type']) for p in sig['parameters']] == [
        ('param_1', 'native_arg_param_1', 'long'), ('stack0x00000008', 'native_arg_param_2', 'CCharString')]


def test_member_resource_alias_calls_are_resource_methods():
    unit = json.loads((UNIT / 'units/V_BookCollecting.json').read_text(encoding='utf-8'))
    spec = LoweringSpec(unit, 'BS_Teacher', entity=True, thing_slots={})
    spec.call_labels = {}
    out = lower('{\n  pOther = this + 0x34;\n  pCVar17 = pOther;\n'
                '  pCVar7 = (CScriptThing *)(**(code **)(*(int *)((int)this + 0x34) + 0x30))(aCStack_1c);\n'
                '  iVar6 = *(int *)pCVar17;\n  (**(code **)(iVar6 + 0x34))(pCVar7,key,2,0,1,0);\n'
                '  cVar4 = (**(code **)(*(int *)pCVar17 + 0x68))();\n}', spec)[0]
    assert '_stk_1c = RESOURCE_ScriptThing(RESOURCE_MemberResource("seh_me", (CScriptThing *)(this + 8)));' in out
    assert '_Speak_CScriptGameResourceObjectScriptedThingBase((CScriptGameResourceObjectScriptedThingBase *)pCVar17, pCVar7,key,2,0,1,0);' in out
    assert '_IsPerformingScriptTask_' in out and 'iVar6' not in out


def test_getter_operands_move_to_the_call_that_consumes_it():
    text = ('  uVar12 = 2;\n  uVar12 = GSI->GetHero(pvVar9,uVar12,uVar13,uVar14,uVar16);\n'
            '  Res::_Speak_((Res *)pCVar17, uVar12);\n')
    out = return_misattached_getter_operands(text)
    assert 'uVar12_pushed = uVar12;\n  uVar12 = GSI->GetHero();' in out
    assert 'Res::_Speak_((Res *)pCVar17, uVar12,pvVar9,uVar12_pushed,uVar13,uVar14,uVar16);' in out


def test_callers_get_the_pushed_operands_back():
    from tools.script_recovery.lift_native_lua import RData
    from tools.script_recovery.native_local_helper_operands import pushed_operands
    r = RData()
    assert pushed_operands(r, 0x00E54E90, 0x00E555EB, 2) == ['*(int *)(*(int *)(this + 0x14) + 0x8c)',
                                                           '"TEXT_QST_B16_BOOK_REFUSE_AGAIN"']
    assert pushed_operands(r, 0x00E57530, 0x00E5791F, 2) == [None, '"TEXT_QST_B16_BOOK_REFUSED"']
