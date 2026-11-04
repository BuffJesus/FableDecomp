from pathlib import Path
from copy import deepcopy
import json,sys
import pytest
W=Path(__file__).resolve().parent/'fixtures/stack_byte_liveness/book_specific_args'
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_stack_byte_helper_access import summarize
read=lambda p:json.loads(p.read_text(encoding='utf-8'))

@pytest.fixture(scope='module')
def baseline():
    return RData(),read(W/'native_function.json'),read(W/'metadata.json')

class Mutant:
    def __init__(self,parent,patches):self.parent=parent;self.patches=patches
    def __getattr__(self,name):return getattr(self.parent,name)
    def bytes_at(self,address,size):
        original=self.parent.bytes_at(address,size)
        if original is None:return None
        data=bytearray(original)
        for site,raw in self.patches:
            for offset,byte in enumerate(raw):
                if address<=site+offset<address+size:data[site+offset-address]=byte
        return bytes(data)

def test_actual_helpers_derive_contracts_and_owner(baseline):
    summary=summarize(*baseline)
    assert len(summary['nativeProofs'])==6
    assert summary['contracts'][0x99ebf0]['kind']=='constructor-overwrite'
    assert summary['contracts'][0x99eae0]['receiverBytes']==4
    assert summary['receiverOrigin']==dict(instruction='0xe5702b',register='ebx',origin='entry thiscall ECX')

@pytest.mark.parametrize('address,raw',[
    (0x99ebf3,'c60600909090'),
    (0x99ebf3,'8b0690909090'),
    (0x99ebf3,'eb0490909090'),
    (0xe54622,'18'),
    (0xe54623,'c2'),
    (0xf35b34,'10'),
    (0xcb7942,'30'),
    (0xcb794c,'04'),
    (0xcb7946,'01'),
    (0x99e9c3,'0c'),
    (0x99e9bb,'0c'),
    (0x99e9d5,'00'),
    (0x99ea01,'01'),
    (0x99e9b3,'8b04'),
    (0x99e9bc,'90'),
    (0x99eae0,'8b44241890'),
    (0xe5702c,'d8'),
    (0xe57324,'b3009090'),
    (0xe57020,'31c99090'),
    (0x12e5f4c,'3054e500'),
])
def test_changed_access_width_field_branch_callee_or_receiver_rejects(baseline,address,raw):
    image,caller,metadata=baseline
    with pytest.raises(ValueError):summarize(Mutant(image,[(address,bytes.fromhex(raw))]),caller,metadata)

@pytest.mark.parametrize('field,value',[
    ('parentOffset',0x18),('stateOffset',0x30),('flagOffset',4),('parentVtableSlot',16),
    ('parentVtableSlot',13),('instanceCounter','0x013bd804'),('freeText','0x00bfe9bc'),
    ('freeControl','0x00bfeb1c'),('entityVtable','0x012e5efc'),
])
def test_metadata_must_match_original_operands(baseline,field,value):
    image,caller,metadata=baseline;metadata=deepcopy(metadata);metadata[field]=value
    with pytest.raises(ValueError):summarize(image,caller,metadata)

def test_bss_counter_uses_virtual_section_not_fabricated_bytes(baseline):
    image,caller,metadata=baseline
    assert image.bytes_at(int(metadata['instanceCounter'],16),4) is None
    assert summarize(image,caller,metadata)['nativeProofs']['destructor']['size']==12
