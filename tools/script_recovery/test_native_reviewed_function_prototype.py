import copy,json
from pathlib import Path
import pytest
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_reviewed_function_prototype import reviewed_function_parameters
ROOT=Path(__file__).resolve().parents[2]
@pytest.fixture(scope='module')
def evidence():
    unit=json.loads((ROOT/'refs/script_recovery/trader_escort/units/Q_TraderEscort.json').read_text(encoding='utf-8'))
    fn=next(f for f in json.loads((ROOT/'refs/script_recovery/trader_escort/translation_unit_typed.json').read_text(encoding='utf-8'))['functions'] if f['address']=='0x00E0A510')
    return fn,unit,RData()
def test_pdb_void_prototype_preserves_parameter_and_input(evidence):
    fn,unit,data=evidence;before=copy.deepcopy(unit);source=fn['decompile']
    result=reviewed_function_parameters(fn,unit,data)
    assert result['returnKind'] is None and result['reviewedVoid'] is True
    assert result['parameters']==[{'native':'brain_state','lua':'native_arg_brain_state','type':'int'}]
    assert unit==before and fn['decompile']==source

def test_unreviewed_signature_keeps_existing_behavior(evidence):
    fn,unit,data=evidence;result=reviewed_function_parameters(fn,{},data)
    assert result['returnKind']=='number' and 'reviewedVoid' not in result

@pytest.mark.parametrize('change',['source','native','extent','address','oracle','missing','outside','prototype','parameters','return'])
def test_changed_or_missing_evidence_is_refused(evidence,change):
    fn,unit,data=evidence;fn=copy.deepcopy(fn);unit=copy.deepcopy(unit);row=unit['reviewedFunctionPrototypes']['0x00e0a510']
    if change=='source':fn['decompile']+='\n'
    elif change=='native':row['nativeSha256']='0'*64
    elif change=='extent':row['bodyExtent']+=1
    elif change=='address':row['address']='0x00e0a514'
    elif change=='oracle':row['oracleSha256']='0'*64
    elif change=='missing':row['oraclePath']='refs/script_recovery/trader_escort/nonexistent-oracle.txt'
    elif change=='outside':row['oraclePath']='../outside-oracle.txt'
    elif change=='prototype':row['returnType']='bool'
    elif change=='parameters':row['expectedParameters']=[]
    elif change=='return':row['expectedReturnKind']='bool'
    with pytest.raises(ValueError,match='Reviewed function prototype:'):reviewed_function_parameters(fn,unit,data)


def test_effectful_return_is_not_erased(evidence):
    import hashlib
    fn,unit,data=evidence;fn=copy.deepcopy(fn);unit=copy.deepcopy(unit)
    fn['decompile']=fn['decompile'].replace('return iVar4;','return side_effect();',1)
    unit['reviewedFunctionPrototypes']['0x00e0a510']['sourceSha256']=hashlib.sha256(fn['decompile'].encode('utf-8')).hexdigest()
    with pytest.raises(ValueError,match='effectful return expression unsupported'):reviewed_function_parameters(fn,unit,data)
