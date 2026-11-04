"""Ordered x87 health gates, including decompiler-lost unordered results."""
import copy
import pytest
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.native_ordered_health_flags import restore

@pytest.fixture(scope='module')
def evidence():
    converter=UnitConverter(ROOT/'refs/script_recovery/beggar_and_child/translation_unit_typed.json')
    return converter.native('0x00e5b3b0'),converter.rdata

@pytest.fixture
def source():
    parts=[]
    for i in range(9):
        parts.append(f'    value{i} = quest:GetHealth(actor)\n')
        if i<6:
            parts.append(f'    flag{i} = true\n    if value{i} <= 0.0 then\n        flag{i} = false\n    end\n')
        else:parts.append(f'    flag{i} = not (value{i} <= 0.0)\n')
    return ''.join(parts)

def test_all_native_health_results_require_ordered_positive(source,evidence):
    fn,data=evidence
    result=restore(source,fn,data)
    assert result.count(' > 0.0)')==9
    assert '<=' not in result
    assert result.count('quest:GetHealth(actor)')==9

@pytest.mark.parametrize('before,after',[
    ('value0 <= 0.0','value0 <= 1.0'),
    ('quest:GetHealth(actor)','quest:GetHealth(nil --[[missing]])'),
    ('flag0 = true','flag0 = true\n    Observe(value0)'),
    ('flag0 = true','value0 = true'),
])
def test_ambiguous_source_remains_unchanged(source,evidence,before,after):
    fn,data=evidence;source=source.replace(before,after,1)
    assert restore(source,fn,data)==source

class ChangedBytes:
    def __init__(self,data,patch):self.data,self.patch=data,patch
    def bytes_at(self,address,size):
        raw=self.data.bytes_at(address,size)
        if raw is None:return None
        value=bytearray(raw)
        for where,byte in self.patch.items():
            if address<=where<address+size:value[where-address]=byte
        return bytes(value)

@pytest.mark.parametrize('patch',[
    {0xE5C1AC+2:0x40},  # status test no longer rejects equality/unordered
    {0xE5C1B1+4:1},     # false store changed
    {0xE5C1AF:0x75},    # branch polarity changed
    {0x122DEDC:1},      # threshold changed
    {0xE5C199:0x90},    # native call missing
])
def test_unproven_native_gate_is_not_recovered(source,evidence,patch):
    fn,data=evidence
    assert restore(source,fn,ChangedBytes(data,patch))==source

def test_call_inventory_must_agree(source,evidence):
    fn,data=evidence;fn=copy.deepcopy(fn)
    fn['indirectCalls']=[c for c in fn['indirectCalls'] if c.get('site','').lower()!='0x00e5c199']
    assert restore(source,fn,data)==source


@pytest.mark.parametrize('health',[float('-inf'),-1.0,-0.0,0.0,1.0,float('inf'),float('nan')])
def test_recovered_byte_results_match_native_ordered_predicate(source,evidence,health):
    from lupa.lua54 import LuaRuntime
    fn,data=evidence;lua=LuaRuntime();lua.globals().health=health
    lua.execute('quest={};calls=0;function quest:GetHealth(actor) calls=calls+1;return health end')
    result=lua.execute(restore(source,fn,data)+'return '+','.join(f'flag{i}' for i in range(9)))
    assert result==(health>0,)*9
    assert lua.globals().calls==9
