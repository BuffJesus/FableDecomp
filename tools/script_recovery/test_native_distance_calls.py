"""Euclidean distance call identity, return stores, and refusal of ambiguous evidence."""
import copy
import struct
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_distance_calls import restore, TARGET
from tools.script_recovery.native_evidence_lowering import finish_lua, fold_engine_helpers
from tools.script_recovery.lift_native_lua import RData

SITE=0xE64271
LABEL='GetSquaredDistanceBetweenThings'
SOURCE=f'  fStack_58 = {LABEL}(first,second);\n'
FUNCTION={'calls':[{'site':hex(SITE),'target':hex(TARGET),'currentName':LABEL}], 'callOrder':[hex(SITE)]}

class Evidence:
    def __init__(self,following=b'\xd9\x5c\x24\x14',bad_target=False,bad_body=False):
        self.following=following;self.bad_target=bad_target;self.bad_body=bad_body
    def bytes_at(self,address,size):
        if address==TARGET:
            return b'\0'*size if self.bad_body else RData().bytes_at(address,size)
        if address==SITE:
            code=b'\xe8'+struct.pack('<i',TARGET+int(self.bad_target)-SITE-5)+self.following
            return (code+b'\x90'*size)[:size]
        return None

@pytest.mark.parametrize('following,kind',[(b'\xd9\x5c\x24\x14','Float32'),(b'\xd9\x9c\x24\x64\x01\0\0','Float32'),(b'\xd8\x5c\x24\x30',''),(b'\xd8\x9c\x24\x64\x01\0\0',''),(b'\xd8\x0d\xdc\xb7\x26\x01',''),(b'\xdd\xd8','')])
def test_verified_return_consumer(following,kind):
    out=restore(SOURCE,FUNCTION,Evidence(following))
    assert out==f'  fStack_58 = ENGINE_RetailDistance{kind}(first, second);\n'

@pytest.mark.parametrize('mutation',['body','target','consumer','order','count','args','identity'])
def test_ambiguous_or_changed_evidence_stays_unresolved(mutation):
    fn=copy.deepcopy(FUNCTION);source=SOURCE;evidence=Evidence()
    if mutation=='body':evidence.bad_body=True
    if mutation=='target':evidence.bad_target=True
    if mutation=='consumer':evidence.following=b'\xd9\x44\x24\x10'
    if mutation=='order':fn['callOrder']=[]
    if mutation=='count':source+=SOURCE
    if mutation=='args':source=source.replace('first,second','first')
    if mutation=='identity':fn['calls'][0]['target']='0xcbe583'
    assert restore(source,fn,evidence)==source

def test_name_alone_does_not_assert_squared_distance():
    assert fold_engine_helpers(SOURCE,{LABEL:TARGET})==SOURCE

@pytest.mark.parametrize('value',[5.0,1+2**-24,1+3*2**-24,float('inf'),float('nan')])
def test_emitted_raw_stored_and_discarded_calls(value):
    lua=LuaRuntime(unpack_returned_tuples=True)
    lua.globals().distance=value
    lua.execute('resources={calls=0}; function resources:DistanceBetweenThings(a,b) self.calls=self.calls+1; assert(a=="first" and b=="second");return distance end')
    source=finish_lua('function Exercise()\nlocal raw=ENGINE_RetailDistance("first", "second")\nlocal stored=ENGINE_RetailDistanceFloat32("first", "second")\nENGINE_RetailDistance("first", "second")\nreturn raw,stored\nend')
    lua.execute(source);raw,stored=lua.globals().Exercise()
    expected=struct.unpack('<f',struct.pack('<f',value))[0]
    assert (raw==value and stored==expected) or (value!=value and raw!=raw and stored!=stored)
    assert lua.globals().resources.calls==3

def test_first_retained_comparison_can_differ_from_stored_result():
    lua=LuaRuntime(unpack_returned_tuples=True)
    lua.execute('resources={};function resources:DistanceBetweenThings() return 1+2^-24 end')
    lua.execute(finish_lua('function Exercise()\nlocal raw=ENGINE_RetailDistance(nil,nil)\nlocal stored=ENGINE_RetailDistanceFloat32(nil,nil)\nreturn raw>1,stored>1\nend'))
    assert lua.globals().Exercise()==(True,False)

def test_float_conversion_survives_a_native_local_named_string():
    lua=LuaRuntime(unpack_returned_tuples=True)
    lua.execute('resources={};function resources:DistanceBetweenThings() return 1+2^-24 end')
    source=finish_lua('function Exercise()\nlocal string="native local"\nlocal value=ENGINE_RetailDistanceFloat32(nil,nil)\nreturn value,string\nend')
    lua.execute(source)
    assert lua.globals().Exercise()==(1,'native local')

def test_nearest_store_is_after_raw_comparison():
    source=f'  fVar5 = {LABEL}(first,second);\n  if (fVar5 < fStack_18) {{\n    fStack_18 = fVar5;\n  }}'
    fn=copy.deepcopy(FUNCTION);fn['calls'][0]['depth']=40
    following=bytes.fromhex('d85424108b742424dfe0f6c4057a588b4b14d95c2410')
    result=restore(source,fn,Evidence(following))
    assert 'ENGINE_RetailDistance(first, second)' in result
    assert 'if (fVar5 < fStack_18)' in result
    assert 'fStack_18 = ENGINE_DistanceFloatStore(fVar5)' in result
    assert restore(source.replace('fStack_18','fStack_20'),fn,Evidence(following))==source.replace('fStack_18','fStack_20')
    local_source=source.replace('fStack_18','local_18')
    assert 'local_18 = ENGINE_DistanceFloatStore(fVar5)' in restore(local_source,fn,Evidence(following))
