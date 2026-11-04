"""Native hidden-return argument ownership for direct quest cards."""
import pytest
from tools.script_recovery.convert_quest_unit import UnitConverter, _text_order_sites
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.native_quest_card_operands import restore

@pytest.fixture(scope="module")
def converter():
    return UnitConverter(ROOT/"refs/script_recovery/beggar_and_child/translation_unit_typed.json")

@pytest.mark.parametrize("address",["0x00e58d40","0x00e5b3b0"])
def test_native_pair_restores_card_quest_and_false(converter,address):
    fn=converter.native(address);source=fn["decompile"]
    result=restore(source,fn,converter.rdata,_text_order_sites(source,fn,include_unslotted=True))
    before={key:args for _,_,args,_,key,_ in _text_order_sites(source,fn,include_unslotted=True) if key in ("0xa3c","0x49c")}
    after={key:args for _,_,args,_,key,_ in _text_order_sites(result,fn,include_unslotted=True) if key in ("0xa3c","0x49c")}
    assert after["0xa3c"]==before["0xa3c"][:1]
    assert after["0x49c"][1:]==[before["0x49c"][1],"0"]
    assert after["0x49c"][0]!=before["0x49c"][0]
    assert restore(result,fn,converter.rdata,_text_order_sites(result,fn,include_unslotted=True))==result

class ChangedBytes:
    def __init__(self,data,patch):self.data,self.patch=data,patch
    def string_at(self,address):return self.data.string_at(address)
    def bytes_at(self,address,size):
        raw=self.data.bytes_at(address,size)
        if raw is None:return None
        value=bytearray(raw)
        for where,byte in self.patch.items():
            if address<=where<address+size:value[where-address]=byte
        return bytes(value)

@pytest.mark.parametrize("patch",[
    {0xE5BC4F:2}, # non-boolean argument
    {0xE5BC63:0x8c}, # card address goes to ECX instead of EAX
    {0xE5BC61:0x51}, # returned quest name is not pushed
    {0xE5BC47:8}, # different getter receiver
    {0xE5BC60:8}, # different outer receiver
    {0xE5BC35:0x45}, # constructor literal differs
    {0xE5BC3C:0xe8}, # constructor storage differs
    {0x8918A3:8}, # getter purges too much
    {0x896A1C:8}, # outer purges too little
    {0x1260F0C+0x49c:0}, # API target differs
])
def test_native_ambiguity_is_unchanged(converter,patch):
    fn=converter.native("0x00e5b3b0");source=fn["decompile"]
    assert restore(source,fn,ChangedBytes(converter.rdata,patch),_text_order_sites(source,fn,include_unslotted=True))==source

@pytest.mark.parametrize("change",["flag","literal","result","missing_site"])
def test_source_ambiguity_is_unchanged(converter,change):
    fn=converter.native("0x00e5b3b0");source=fn["decompile"]
    sites=_text_order_sites(source,fn,include_unslotted=True)
    if change=="literal":source=source.replace('"OBJECT_QUEST_CARD_BEGGAR_AND_CHILD"','"WRONG_CARD"')
    else:
        a,e,args,_,_,_=next(s for s in sites if s[4]==("0xa3c" if change=="flag" else "0x49c"))
        if change=="flag":args[-1]="1"
        if change=="result":args[-1]="wrong_result"
        source=source[:a]+",".join(args)+source[e:]
    sites=_text_order_sites(source,fn,include_unslotted=True)
    if change=="missing_site":sites=[s for s in sites if s[4]!="0xa3c"]
    assert restore(source,fn,converter.rdata,sites)==source


def test_true_native_boolean_is_preserved(converter):
    fn=converter.native("0x00e5b3b0");source=fn["decompile"]
    a,e,args,_,_,_=next(s for s in _text_order_sites(source,fn,include_unslotted=True) if s[4]=="0xa3c")
    args[-1]="1";source=source[:a]+",".join(args)+source[e:]
    data=ChangedBytes(converter.rdata,{0xE5BC4F:1})
    result=restore(source,fn,data,_text_order_sites(source,fn,include_unslotted=True))
    assert next(s[2] for s in _text_order_sites(result,fn,include_unslotted=True) if s[4]=="0x49c")[-1]=="1"
