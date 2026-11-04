import pytest
from tools.script_recovery.native_evidence_lowering import fold_name_compare

@pytest.mark.parametrize('spelling',['*(void **)value','(void *)*value'])
@pytest.mark.parametrize('literal',['EXPRESSION_BELCH','EXPRESSION_FART',''])
def test_equivalent_pointer_casts_keep_string_comparison(spelling,literal):
    source=f'  p = {spelling};\n  result = CBasicString<char>::Compare(p,"{literal}");\n'
    assert fold_name_compare(source)==f'  result = ENGINE_StrCmp(value, "{literal}");\n'

@pytest.mark.parametrize('middle',['label:\n','value = other;\n','Escape(p);\n','p++;\n'])
def test_alternate_pointer_spelling_requires_unambiguous_use(middle):
    source='  p = (void *)*value;\n'+middle+'  result = CBasicString<char>::Compare(p,"EXPRESSION_BELCH");\n'
    assert fold_name_compare(source)==source


def test_reused_pointer_keeps_each_string_lifetime():
    source='  p = (void *)*first;\n  a = CBasicString<char>::Compare(p,"a");\n  p = (void *)*second;\n  b = CBasicString<char>::Compare(p,"b");\n'
    assert fold_name_compare(source)=='  a = ENGINE_StrCmp(first, "a");\n  b = ENGINE_StrCmp(second, "b");\n'
