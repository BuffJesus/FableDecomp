"""Fail closed on changed operands, native paths, bindings or lifted statements."""
import copy
import hashlib
import json
from pathlib import Path
from types import SimpleNamespace

import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_lifted_interface_calls import restore

ROOT=Path(__file__).resolve().parents[2]
FIXTURE=ROOT/'tools/script_recovery/testdata/bordello_lifted_interface_cleanup.lua'


@pytest.fixture(scope='module')
def inputs():
    converter=UnitConverter(ROOT/'refs/script_recovery/bordello/translation_unit_typed.json')
    fn=converter.native('0x00e3eb10')
    unit=json.loads((ROOT/'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    row=unit['reviewedLiftedInterfaceCalls']['0x00e3eb10']
    source=FIXTURE.read_text(encoding='utf-8')
    todo=['unlifted: ppuVar13 = *piVar1','unlifted: ppuVar13 = *pppuVar28']
    for call in row['calls']:
        if 'statement' in call:
            todo.extend(['unlifted: '+call['statement']]*call['diagnosticCount'])
    return source,todo,fn,row,converter.rdata


def test_exact_recovery_preserves_other_diagnostics(inputs):
    source,todo,fn,row,rd=inputs
    after,remaining=restore(source,todo,fn,row,rd)
    expected=source
    for call in row['calls']:
        old=call.get('printedCall') or '-- TODO(native): '+call['statement']
        expected=expected.replace(old,'quest:PauseAllNonScriptedEntities(false)',1)
    assert after==expected
    assert len(remaining)==len(todo)-3
    assert 'unlifted: ppuVar13 = *piVar1' in remaining
    assert 'unlifted: ppuVar13 = *pppuVar28' in remaining
    assert after.count('quest:PauseAllNonScriptedEntities(false)')==source.count('quest:PauseAllNonScriptedEntities(false)')+4
    LuaRuntime().compile(after)
    assert restore('unrelated',['retained'],fn,None,rd)==('unrelated',['retained'])


@pytest.mark.parametrize('fault',[
    'native_hash','source_hash','lua_hash','slot','target','method','callee',
    'bind','load','save','stack','instruction','duplicate_call','missing_predecessor',
    'duplicate_predecessor','wrong_site','statement','diagnostic_count',
])
def test_changed_review_fails_closed(inputs,fault):
    source,todo,fn,row,rd=inputs;row=copy.deepcopy(row)
    if fault.endswith('_hash'):row[{'native_hash':'nativeSha256','source_hash':'sourceSha256','lua_hash':'luaSha256'}[fault]]='0'*64
    elif fault=='slot':row['slot']='0x5e8'
    elif fault=='target':row['target']='0x890ab1'
    elif fault=='method':row['method']='FixMovieSequenceCamera'
    elif fault=='callee':row['calleeBytes']='00'*32
    elif fault in ['bind','load','save']:row['receiverOrigin'][fault]='0xe3eb1f'
    elif fault=='stack':row['receiverOrigin']['balancedStackOffset']+=4
    elif fault=='instruction':row['nativeInstructions']['0xe3eb1f']='00'
    elif fault=='duplicate_call':row['calls'].append(copy.deepcopy(row['calls'][0]))
    elif fault=='missing_predecessor':row['calls'][1]['setups'].pop()
    elif fault=='duplicate_predecessor':row['calls'][1]['setups'].append(row['calls'][1]['setups'][0])
    elif fault=='wrong_site':row['calls'][0]['site']='0xe40074'
    elif fault=='statement':row['calls'][0]['statement']='different();'
    elif fault=='diagnostic_count':row['calls'][0]['diagnosticCount']=1
    with pytest.raises((ValueError,IndexError,KeyError)):
        restore(source,todo,fn,row,rd)


@pytest.mark.parametrize('fault',['source','missing_diagnostic','duplicate_statement'])
def test_changed_lifted_artifact_fails_closed(inputs,fault):
    source,todo,fn,row,rd=inputs;row=copy.deepcopy(row);todo=list(todo)
    if fault=='source':source+='\n'
    elif fault=='missing_diagnostic':todo.remove('unlifted: '+row['calls'][2]['statement'])
    else:
        source+='-- TODO(native): '+row['calls'][2]['statement']+'\n'
        row['luaSha256']=hashlib.sha256(source.encode()).hexdigest()
    with pytest.raises(ValueError):restore(source,todo,fn,row,rd)


@pytest.mark.parametrize('site,replacement,reason',[
    (0xe3eb1f,'0bff','false argument provenance'),
    (0xe3f488,'0bff','false argument provenance'),
    (0xe3eeee,'6a01','false argument provenance'),
    (0xe3ed64,'8b5d08','entity interface load'),
    (0xe3eb19,'8bea','script receiver bind'),
])
def test_rewitnessed_wrong_native_semantics_are_still_rejected(inputs,site,replacement,reason):
    source,todo,fn,row,rd=inputs;row=copy.deepcopy(row)
    start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
    raw=bytearray(rd.bytes_at(start,end-start));changed=bytes.fromhex(replacement)
    raw[site-start:site-start+len(changed)]=changed
    row['nativeSha256']=hashlib.sha256(raw).hexdigest()
    row['nativeInstructions'][hex(site)]=replacement
    def read(address,count):
        if start<=address and address+count<=end:return bytes(raw[address-start:address-start+count])
        return rd.bytes_at(address,count)
    with pytest.raises(ValueError,match=reason):restore(source,todo,fn,row,SimpleNamespace(bytes_at=read))
