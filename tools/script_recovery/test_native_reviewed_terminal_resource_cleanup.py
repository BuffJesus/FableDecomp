"""Focused acceptance/rejection checks for the scratch terminal recovery."""
from copy import deepcopy
import hashlib,json,sys
from pathlib import Path
import pytest
R=Path(__file__).resolve().parents[2]
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_terminal_resource_cleanup import restore

@pytest.fixture(scope='module')
def evidence():
    c=UnitConverter(R/'refs/script_recovery/bordello/translation_unit_typed.json')
    return (R/'tools/script_recovery/testdata/bordello_terminal_resource_cleanup.lua').read_text(),c.native('0x00e3eb10'),json.loads((R/'refs/script_recovery/bordello/units/V_Bordello.json').read_text())['reviewedTerminalResourceCleanup']['0x00e3eb10'],c.rdata

def test_exact_single_cleanup(evidence):
    source,fn,row,rd=evidence
    expected=source.replace('if bVar5 then\n                return', 'if bVar5 then\n                resources:ReleaseResource(ladyResource)\n                return',1)
    for indent in ['                    ','                ']:
        expected=expected.replace(indent+'resources:DestroyMovie(xStack_190)\n'+indent+'resources:DestroyMovie(xStack_190)',
            indent+'resources:DestroyMovie(xStack_190)\n'+indent+'resources:ReleaseResource(ladyResource)',1)
    assert restore(source,fn,row,rd)==expected
    assert restore(source,fn,None,rd)==source

@pytest.mark.parametrize('key,value',[
    ('nativeSha256','0'*64),('sourceSha256','0'*64),('luaSha256','0'*64),
    ('terminationBranch','0xe3ec78'),('cleanupEntry','0xe40335'),('inlineCleanupStart','0xe40335'),
    ('constructorReceiver','0xe40335'),('cleanupReceiver','0xe3ec71'),('balancedStackSlot',28),
    ('alias','wrongResource'),('constructorHandle','wrongHandle'),('pollLabel','missingLabel'),
    ('terminationFlag','wrongFlag'),('alias','bad-name')])
def test_changed_review_rejected(evidence,key,value):
    source,fn,row,rd=evidence;row=deepcopy(row);row[key]=value
    # Constructor and cleanup use the same slot; swapping their witnesses is
    # still equivalent, so require a truly wrong instruction for that mutation.
    if key=='constructorReceiver':row[key]='0xe3ec71'
    with pytest.raises(ValueError):restore(source,fn,row,rd)

@pytest.mark.parametrize('kind',['bytes','target','global_hash','global_address','global_size','alias_store','poll_shape','constructor'])
def test_changed_semantics_rejected(evidence,kind):
    source,fn,row,rd=evidence;row=deepcopy(row)
    if kind=='bytes':row['nativeInstructions']['0xe40318']='9090'
    elif kind=='target':row['nativeCalls']['0xe3ec73']='0x99a380'
    elif kind.startswith('global_'):
        item=row['equivalentDestructorBodies'][1]
        item[{'global_hash':'sha256','global_address':'address','global_size':'size'}[kind]]={'global_hash':'0'*64,'global_address':'0x99a430','global_size':65}[kind]
    else:
        source=source.replace({'alias_store':'::LADY_POLL::','poll_shape':'if bVar5 then\n                return','constructor':'local ladyResource = x_stk_188'}[kind],
                              {'alias_store':'ladyResource = nil\n        ::LADY_POLL::','poll_shape':'if bVar5 then\n                resources:ReleaseResource(ladyResource)\n                return','constructor':'local ladyResource = wrongHandle'}[kind],1)
        row['luaSha256']=hashlib.sha256(source.encode()).hexdigest()
    with pytest.raises(ValueError):restore(source,fn,row,rd)

def test_reapplication_rejected(evidence):
    source,fn,row,rd=evidence
    with pytest.raises(ValueError,match='source phase'):restore(restore(source,fn,row,rd),fn,row,rd)

@pytest.mark.parametrize('key,value',[('pairCount',1),('movieAlias','otherMovie'),('entry','0xe40368')])
def test_changed_split_metadata_rejected(evidence,key,value):
    source,fn,row,rd=evidence;row=deepcopy(row);row['splitCleanup'][key]=value
    with pytest.raises(ValueError):restore(source,fn,row,rd)

def test_changed_split_pair_rejected(evidence):
    source,fn,row,rd=evidence;row=deepcopy(row)
    source=source.replace('resources:DestroyMovie(xStack_190)\n                    resources:DestroyMovie(xStack_190)',
        'resources:DestroyMovie(xStack_190)\n                    resources:ReleaseResource(ladyResource)',1)
    row['luaSha256']=hashlib.sha256(source.encode()).hexdigest()
    with pytest.raises(ValueError,match='cleanup shape'):restore(source,fn,row,rd)
