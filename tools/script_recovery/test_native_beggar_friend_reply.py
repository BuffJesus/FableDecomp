import copy,itertools,json,re
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.native_reviewed_stack_aliases import restore

@pytest.fixture(scope='module')
def evidence(tmp_path_factory):
    converter=UnitConverter(ROOT/'refs/script_recovery/beggar_and_child/translation_unit_typed.json')
    unit=json.loads((ROOT/'refs/script_recovery/beggar_and_child/units/V_BeggarAndChild.json').read_text())
    fn=converter.native('0x00e5b3b0')
    row=unit['reviewedStackScalarAliases'][fn['address'].lower()]
    out=tmp_path_factory.mktemp('beggar_friend_reply')
    converter.convert(unit,out)
    source=(out/'FSE/V_BeggarAndChild/Entities/BeggarBully.lua').read_text()
    return converter,fn,row,source

def make_probe(source,flag):
    init=re.search(r'^\s*'+flag+r' = false\n',source,re.M)[0]
    match=re.search(r'^\s*if not '+flag+r' then\n',source,re.M)
    guard=source.rfind('quest:IsActiveThreadTerminating()',0,match.start())
    start=source.rfind('\n',0,guard)+1
    stop=source.index('resources:ScriptThing(',match.end());stop=source.rfind('\n',0,stop)+1
    body=source[start:stop]
    lua=LuaRuntime()
    return lua.execute(f'''local {flag},alive,bVar6
local inputCancel,calls
local quest={{}}
function quest:IsActiveThreadTerminating() calls=calls+1;return inputCancel end
{init}
return function(cancel)
inputCancel=cancel;calls=0
{body}
 do return {flag},true,false,calls end
end
do return {flag},false,false,calls end
::LAB_00e5ce6e::
::LAB_00e5bab8::
::LAB_00e5c30a::
return {flag},false,true,calls
end''')

@pytest.mark.parametrize('cancels',list(itertools.product((False,True),repeat=3)))
def test_generated_first_reply_flag_survives_attempts_and_cancellation(evidence,cancels):
    # Probe stops before health/speech/resource cleanup; no whole-quest claim.
    _,_,row,source=evidence;probe=make_probe(source,row['alias']);selected=False
    for cancel in cancels:
        first=not selected;selected=selected or (first and not cancel)
        expected=(selected,first and not cancel,cancel,1+int(first and not cancel))
        assert probe(cancel)==expected

@pytest.mark.parametrize('kind',['native','exported_source','printed_anchor'])
def test_changed_evidence_rejects_the_reviewed_flag(evidence,kind):
    converter,fn,row,_=evidence;fn=copy.deepcopy(fn);source=fn['decompile'];data=converter.rdata
    if kind=='exported_source':fn['decompile']+='\n'
    elif kind=='printed_anchor':source=source.replace(row['edits'][0][0],'Unproven();')
    else:
        original=data
        class Changed:
            def bytes_at(self,address,size):
                raw=original.bytes_at(address,size)
                if raw is not None and address<=0xE5BD7B<address+size:
                    result=bytearray(raw);result[0xE5BD7B-address]=0x90;return bytes(result)
                return raw
        data=Changed()
    with pytest.raises(ValueError):restore(source,fn,row,data)
