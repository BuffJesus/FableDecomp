import math,struct
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_float32 import narrow_arithmetic
@pytest.mark.parametrize('text',[
 '(float)(a - b)', '(float)(((float10)a - b) - ((float10)c - (float10)(float)d))',
 '(float)(a + b)', '(float)((a - b) + (c - d))',
])
def test_explicit_arithmetic_narrows(text):
 result=narrow_arithmetic(text)
 assert 'string.pack("<f"' in result
 assert narrow_arithmetic(result)==result
@pytest.mark.parametrize('text',[
 '(float)a','(float)(a)','(float)(GetHealth(a) - b)',
 '(float)(*a - b)','(float)((int)a - b)','(float)(a / b)',
 '(float)(a * b)','(float)(a <= b)','(float)(a - b',
 '(float)(a & b)','(float)(a[0] - b)',
])
def test_unproven_expression_unchanged(text):assert narrow_arithmetic(text)==text
@pytest.mark.parametrize('hero,whisper',[(20,2.000000238418579),(20,5.000000476837158),(15,5.000000476837158),(8,3.000000238418579),(5,20)])
def test_lua_standard_float_pack_matches_native_score(hero,whisper):
 lua=LuaRuntime()
 actual=lua.eval('function(a,b,c,d)return (string.unpack("<f",string.pack("<f",(a-b)-(c-d))))end')(20,whisper,20,hero)
 expected=struct.unpack('<f',struct.pack('<f',(20-whisper)-(20-hero)))[0]
 assert actual==expected
