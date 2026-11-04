"""Float snapshots must not keep the old unassigned string-register spelling."""
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_evidence_lowering import normalise_typed_decompile
from tools.script_recovery.test_lift_native_lua import make

@pytest.mark.parametrize('chain',[False,True])
@pytest.mark.parametrize('later',['CVar2 = 99;','CVar2 = "later";'])
@pytest.mark.parametrize('value',[0,7,30,16777216,2147483648])
def test_immediate_spill_preserves_snapshot_after_source_reuse(chain,later,value):
    source='''{
  CVar2 = (CCharString_bv)(float)raw;
CHAIN  xStack_34 = CVar2;
  LATER
  GSI->SetTimer(1,xStack_34);
}'''
    source=source.replace('CHAIN','  CVar6 = CVar2;\n' if chain else '').replace('LATER',later)
    lowered=normalise_typed_decompile(source)
    assert 'xStack_34 = f_CVar2;' in lowered
    lifter=make();lifter.accessor_kinds=True
    body='\n'.join(lifter.lift('Main',lowered,parameters={'raw':'float'}))
    assert not lifter.todo,body
    lua=LuaRuntime();writes=[]
    run=lua.execute('return function(Quest,raw)\n'+body+'\nend')
    run(lua.table_from({'SetTimer':lambda _,key,result:writes.append((key,result))}),value)
    assert writes==[(1,value)]

@pytest.mark.parametrize('boundary',[
    'CVar2 = Pointer();','Consume();','Mutate(&CVar2);','if (flag) {',
    'LAB_other:','goto LAB_other;','return;','CVar2 = CVar2;',
])
def test_later_or_ambiguous_copy_keeps_original_operand(boundary):
    source='  CVar2 = (CCharString_bv)(float)raw;\n  '+boundary+'\n  xStack_34 = CVar2;\n'
    lowered=normalise_typed_decompile(source)
    assert 'xStack_34 = CVar2;' in lowered
    assert 'xStack_34 = f_CVar2;' not in lowered

@pytest.mark.parametrize('initial',['CVar2 = Pointer();','CVar2 = (CCharString_bv)raw;'])
def test_unproven_source_is_not_classified_as_float(initial):
    source='  '+initial+'\n  xStack_34 = CVar2;\n'
    assert 'xStack_34 = CVar2;' in normalise_typed_decompile(source)
