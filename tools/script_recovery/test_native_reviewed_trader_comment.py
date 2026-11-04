import copy
import hashlib
import json
from pathlib import Path
import pytest
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_reviewed_trader_comment import restore

ROOT=Path(__file__).resolve().parents[2]
@pytest.fixture(scope='module')
def evidence():
    unit=json.loads((ROOT/'refs/script_recovery/trader_escort/units/Q_TraderEscort.json').read_text(encoding='utf-8-sig'))
    fn=next(f for f in json.loads((ROOT/'refs/script_recovery/trader_escort/translation_unit_typed.json').read_text(encoding='utf-8-sig'))['functions'] if int(f['address'],16)==0xe01900)
    row=unit['reviewedTraderComments'][str(fn['address']).lower()]
    text=(ROOT/'tools/script_recovery/fixtures/trader_comment/parameter_lowering_input.c').read_bytes().decode('utf-8')
    return fn,row,text,RData()

def test_reviewed_source_preserves_actor_and_byte_lifetimes(evidence):
    fn,row,text,data=evidence
    result=restore(text,fn,data,row)
    assert hashlib.sha256(result.encode()).hexdigest()==row['recoveredSha256']
    assert 'native_reply_available = 0;' in result and 'native_reply_available = 1;' in result
    assert 'local_nearest_handle = (CScriptThing_bv *)local_furthest_handle;' in result
    assert 'local_furthest_handle = (CScriptThing_bv *)native_swap_handle;' in result
    assert 'local_furthest_handle = QUESTTHING_Empty();' in result
    assert 'native_conversation_speaker,false,false' in result
    assert 'CONCAT31' not in result

def test_unreviewed_source_is_untouched_without_native_reads(evidence):
    fn,row,text,data=evidence
    class NoReads:
        def bytes_at(self,*args):raise AssertionError('Unreviewed source must not read native bytes')
    assert restore(text,fn,NoReads(),None)==text

@pytest.mark.parametrize('change',['native','source','intermediate','address','end','anchor','outside','output',
                                   'copy','assign','destroy','empty','get_string','is_alive'])
def test_changed_review_or_input_fails_closed(evidence,change):
    fn,row,text,data=evidence
    fn=copy.deepcopy(fn);row=copy.deepcopy(row)
    if change=='native':row['nativeSha256']='0'*64
    if change=='source':fn['decompile']+='\n'
    if change=='intermediate':text+='\n'
    if change=='address':fn['address']='0xe01901'
    if change=='end':fn['bodyEndExclusive']='0xe02388'
    if change=='anchor':row['anchors']['0xe01c24']='00'
    if change=='outside':row['anchors']['0x400000']='00'
    if change=='output':row['recoveredSha256']='0'*64
    dependency={'copy':'0x4abe90','assign':'0x8ab980','destroy':'0x4aa840','empty':'0x6e7b40','get_string':'0x4aa900','is_alive':'0x4ab130'}.get(change)
    if dependency:row['dependencies'][dependency]['sha256']='0'*64
    with pytest.raises(ValueError):restore(text,fn,data,row)
