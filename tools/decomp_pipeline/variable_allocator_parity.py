"""Shared complete-body parity check for behavior-gated allocator providers."""
import struct
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE,pe_sections,va_to_off

def check(obj,symbol,address,length,count,targets):
    c=Coff(obj);section,offset=c.symbols[symbol];body,relocs=c.sections[section]
    assert offset==0 and len(relocs)==count,(symbol,offset,len(relocs))
    patched=bytearray(body)
    for p,(target,kind)in relocs.items():
        assert target in targets and kind in (6,20),(symbol,target,kind)
        v=targets[target]+struct.unpack_from('<I',body,p)[0]
        if kind==20:v-=address+p+4
        struct.pack_into('<I',patched,p,v&0xffffffff)
    raw=EXE.read_bytes();at=va_to_off(pe_sections(raw),address);retail=raw[at:at+length]
    score=compare(body,retail,tuple(relocs),retail_relocations(f'{address:08x}',length))
    # Fixed-image retail spans can lack relocation metadata for embedded
    # vtable addresses. An audited, fully relocated exact match is stronger
    # evidence than that incomplete normalized comparison (e.g. the 7-byte
    # CBaseClass destructor, whose only non-return instruction stores a vptr).
    if patched==retail:
        score=compare(patched,retail,(),())
    assert score.near_match,(symbol,score)
    return dict(address=f'{address:08x}',symbol=symbol,built_bytes=len(body),retail_bytes=length,relocations=count,
                grade='MATCH'if patched==retail else'NEAR_MATCH',**score.to_dict())
