import pytest
from tools.script_recovery.native_evidence_lowering import fold_char_flags

SOURCE="char cVar5;\nchar c_stk_115;\ncVar5 = '\\0';\nc_stk_115 = '\\0';\nwhile (cVar5 == '\\0') {\n c_stk_115 = '\\x01';\n cVar5 = c_stk_115;\n}\n"

def test_numeric_byte_snapshot_is_converted_at_boolean_copy():
 result=fold_char_flags(SOURCE)
 assert "cVar5 = (c_stk_115 != '\\0');" in result
 assert "c_stk_115 = '\\0';" in result
 assert 'while (!cVar5)' in result

@pytest.mark.parametrize('change',[
 "c_stk_115 = GetValue();",
 "c_stk_115 = other;",
 "c_stk_115 = 2;",
 "c_stk_115 += 1;",
 "c_stk_115++;",
 "Use(&c_stk_115);",
 "if (ready) c_stk_115 = other;",
 "++c_stk_115;",
])
def test_unclassified_or_mutable_byte_keeps_copy(change):
 result=fold_char_flags(SOURCE+change+'\n')
 assert 'cVar5 = c_stk_115;' in result

@pytest.mark.parametrize('decl',['bool','int','float','char *'])
def test_non_numeric_byte_storage_keeps_copy(decl):
 result=fold_char_flags(SOURCE.replace('char c_stk_115;',decl+' c_stk_115;'))
 assert 'cVar5 = c_stk_115;' in result

def test_numeric_destination_keeps_numeric_copy():
 result=fold_char_flags(SOURCE.replace("while (cVar5 == '\\0')","while (cVar5 < 3)"))
 assert 'cVar5 = c_stk_115;' in result
