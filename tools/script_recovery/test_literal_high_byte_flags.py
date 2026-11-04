import sys
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
HERE=Path(__file__).resolve().parent
ROOT=next(p for p in HERE.parents if (p/'tools/script_recovery/script_units.py').is_file())
sys.path.insert(0,str(ROOT))
if HERE.parent.name=='work':
 sys.path.insert(0,str(HERE));from candidate_bootstrap import install
 install()
from tools.script_recovery.native_evidence_lowering import fold_literal_high_byte_flags
@pytest.mark.parametrize('value',[0,1,0xffffff,0x1000000,0x1000001,0x7fffffff,0x80000000,0xff000000,0xffffffff])
@pytest.mark.parametrize('spelling',['decimal','hex'])
def test_full_word_write_preserves_word_and_updates_boolean(value,spelling):
 literal=str(value) if spelling=='decimal' else hex(value)
 source="  word = "+literal+";\n  if ((char)((uint)word >> 0x18) == '\\0') {}"
 result=fold_literal_high_byte_flags(source)
 assert 'word = '+literal+';' in result
 boolean='true' if value>>24 else 'false'
 assert 'word_b3 = '+boolean+';' in result
 lua=LuaRuntime(unpack_returned_tuples=True)
 assert lua.execute('local word,word_b3;'+result.split('  if ')[0]+';return word,word_b3')==(value,bool(value>>24))
@pytest.mark.parametrize('extra',['word = GetValue();','word += 1;','++word;','word++;','Call(&word);','word_b3 = true;','word = other;'])
def test_unsupported_writes_or_aliases_remain_unchanged(extra):
 source="  word = 0;\n  word = 0x1000000;\n  if ((char)((uint)word >> 0x18) == '\\0') {}\n  "+extra
 assert fold_literal_high_byte_flags(source)==source
@pytest.mark.parametrize('test',["Use(word);","if ((word >> 24) == 0) {}","if ((char)((uint)other >> 0x18) == '\\0') {}"])
def test_no_matching_high_byte_test_remains_unchanged(test):
 source='  word = 0;\n  word = 0x1000000;\n  '+test
 assert fold_literal_high_byte_flags(source)==source
