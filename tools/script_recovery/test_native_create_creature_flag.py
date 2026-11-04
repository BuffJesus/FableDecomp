import copy,json
from pathlib import Path
import pytest
from tools.script_recovery.convert_quest_unit import _text_order_sites
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_create_creature_flag import restore
ROOT=Path(__file__).resolve().parents[2]

@pytest.fixture(scope='module')
def evidence():
    fn=next(f for f in json.loads((ROOT/'refs/script_recovery/arena/translation_unit_typed.json').read_text(encoding='utf-8'))['functions'] if f['address']=='0x00F1EED0')
    data=RData();spans={(0xf1eed0,9262):data.bytes_at(0xf1eed0,9262),(0x4aa980,18):data.bytes_at(0x4aa980,18),(0x8a9100,532):data.bytes_at(0x8a9100,532)}
    return fn,spans

class Bytes:
    def __init__(self,spans):self.spans=spans;self.backing=RData()
    def bytes_at(self,address,size):return self.spans[(address,size)] if (address,size) in self.spans else self.backing.bytes_at(address,size)

@pytest.mark.parametrize('flag',[0,1])
def test_native_staged_immediate_survives_zero_argument_getpos(evidence,flag):
    fn,spans=evidence;spans=dict(spans);raw=bytearray(spans[(0xf1eed0,9262)]);raw[0xf1f848-0xf1eed0]=flag;spans[(0xf1eed0,9262)]=bytes(raw)
    source=fn['decompile'];out=restore(source,fn,_text_order_sites(source,fn),Bytes(spans))
    assert out!=source and 'SUB41(pCVar19,0)' not in out
    sites=_text_order_sites(out,fn);target=next(args for a,e,args,site,key,vtable in sites if site.get('site')=='0x00F1F865')
    assert target[-1]==('true' if flag else 'false')

@pytest.mark.parametrize('change',['flag','getpos_slot','getpos_abi','creature_abi','effectful','receiver','metadata','extent','extra_push','branch'])
def test_ambiguous_or_changed_contract_preserves_unresolved_source(evidence,change):
    fn,spans=evidence;fn=copy.deepcopy(fn);spans=dict(spans);source=fn['decompile'];raw=bytearray(spans[(0xf1eed0,9262)])
    if change=='flag':raw[0xf1f848-0xf1eed0]=2
    elif change=='getpos_slot':raw[0xf1f853-0xf1eed0]=0x1c
    elif change=='extra_push':raw[0xf1f855-0xf1eed0]=0x50
    elif change=='branch':raw[0xf1f855-0xf1eed0:0xf1f857-0xf1eed0]=b'\xeb\x00'
    elif change=='getpos_abi':spans[(0x4aa980,18)]=b'\0'*18
    elif change=='creature_abi':spans[(0x8a9100,532)]=b'\0'*532
    elif change=='effectful':source=source.replace('SUB41(pCVar19,0)','SUB41(side_effect(),0)')
    elif change=='receiver':source=source.replace('*(void **)(this + 0x40)','*(void **)(other + 0x40)')
    elif change=='extent':fn.pop('bodyEndExclusive')
    spans[(0xf1eed0,9262)]=bytes(raw)
    entries=_text_order_sites(source,fn)
    if change=='metadata':entries=[(a,e,args,{**site,'slot':'0x168'},key,vtable) for a,e,args,site,key,vtable in entries]
    assert restore(source,fn,entries,Bytes(spans))==source
