import copy
import json
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_movie_members import restore

ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def evidence():
    c=UnitConverter(ROOT/'refs/script_recovery/beggar_and_child/translation_unit_typed.json')
    fn=c.native('0x00e58d40')
    row=json.loads((ROOT/'refs/script_recovery/beggar_and_child/units/V_BeggarAndChild.json').read_text())['reviewedMovieMembers']['0x00e58d40']
    source=[]
    for alias in row['stackReceiverAliases']:
        cast='(CScriptGameResourceObjectMovieBase *)'+alias['old']
        source.extend([f'this_00 = {cast};']*alias.get('phiCount',0))
        source.extend([f'destroy({cast});']*(alias['count']-alias.get('phiCount',0)))
        source.append(f'use_other_type((CScriptThing *){alias["old"]});')
    return '\n'.join(source),fn,row,c.rdata


def test_native_movie_receivers_and_shared_phi_preserved(evidence):
    source,fn,row,data=evidence
    result=restore(source,fn,row,data)
    assert result.count('this_00 = xStack_1b0;')==2
    assert result.count('this_00 = xStack_1c0;')==1
    for alias in row['stackReceiverAliases']:
        assert '(CScriptGameResourceObjectMovieBase *)'+alias['old'] not in result
        assert f'use_other_type((CScriptThing *){alias["old"]});' in result
    assert restore(source,fn,None,data)==source


@pytest.mark.parametrize('index',range(4))
@pytest.mark.parametrize('kind',['slot','frame','lea','constructor','count','source'])
def test_changed_movie_receiver_evidence_rejected(evidence,index,kind):
    source,fn,row,data=evidence;row=copy.deepcopy(row);alias=row['stackReceiverAliases'][index]
    if kind=='slot':alias['entrySlot']+=4
    elif kind=='frame':alias['frameBytes']+=4
    elif kind=='lea':row['nativeInstructions'][alias['receiverLeas'][0]]='00'
    elif kind=='constructor':alias['constructorTarget']='0x006e7b80'
    elif kind=='count':alias['count']+=1
    elif kind=='source':source=source.replace('(CScriptGameResourceObjectMovieBase *)'+alias['old'],'missing',1)
    with pytest.raises(ValueError,match='reviewed movie members'):
        restore(source,fn,row,data)


@pytest.mark.parametrize('kind',['native','export','phi','ctor_receiver'])
def test_changed_body_phi_or_receiver_join_rejected(evidence,kind):
    source,fn,row,data=evidence;row=copy.deepcopy(row)
    if kind=='native':row['nativeSha256']='0'*64
    elif kind=='export':row['sourceSha256']='0'*64
    elif kind=='phi':source=source.replace('this_00 =','lost_phi =',1)
    elif kind=='ctor_receiver':row['stackReceiverAliases'][0]['constructorSite']='0x00E590AC'
    with pytest.raises(ValueError,match='reviewed movie members'):
        restore(source,fn,row,data)


@pytest.fixture(scope='module')
def scoped_evidence():
    c=UnitConverter(ROOT/'refs/script_recovery/beggar_and_child/translation_unit_typed.json')
    fn=c.native('0x00e5b3b0')
    row=json.loads((ROOT/'refs/script_recovery/beggar_and_child/units/V_BeggarAndChild.json').read_text())['reviewedMovieMembers']['0x00e5b3b0']
    aliases=row['stackReceiverAliases']
    source='outside((CScriptGameResourceObjectMovieBase *)xStack_110);\n'
    for group in (aliases[:2],aliases[2:]):
        source+=group[0]['scopeStart']+'\n'
        for a in group:
            source+=(f'destroy((CScriptGameResourceObjectMovieBase *){a["old"]});\n')*a['count']
    return source,fn,row,c.rdata


def test_same_exported_receiver_names_resolve_to_different_native_objects(scoped_evidence):
    source,fn,row,data=scoped_evidence
    result=restore(source,fn,row,data)
    assert result.count('destroy((CScriptGameResourceObjectMovieBase *)xStack_138);')==3
    assert result.count('destroy((CScriptGameResourceObjectMovieBase *)xStack_124);')==3
    assert 'outside((CScriptGameResourceObjectMovieBase *)xStack_110);' in result


@pytest.mark.parametrize('kind',['missing_start','duplicate_start','missing_end','reverse','scope_widened','overlap','unpinned_flag_store'])
def test_scoped_movie_alias_guards(scoped_evidence,kind):
    source,fn,row,data=scoped_evidence;row=copy.deepcopy(row);a=row['stackReceiverAliases'][0]
    if kind=='missing_start':source=source.replace(a['scopeStart'],'missing')
    elif kind=='duplicate_start':source+=a['scopeStart']
    elif kind=='missing_end':a['scopeEnd']='missing'
    elif kind=='reverse':a['scopeStart'],a['scopeEnd']=a['scopeEnd'],a['scopeStart']
    elif kind=='scope_widened':row['stackReceiverAliases'][1].pop('scopeEnd')
    elif kind=='overlap':row['stackReceiverAliases'].append(copy.deepcopy(a))
    elif kind=='unpinned_flag_store':row['nativeInstructions'].pop('0x00E5B717')
    with pytest.raises(ValueError,match='reviewed movie members'):
        restore(source,fn,row,data)
