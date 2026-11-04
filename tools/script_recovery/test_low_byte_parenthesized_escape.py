import pytest
from tools.script_recovery.native_evidence_lowering import fold_low_byte_flags

@pytest.mark.parametrize('escape',[
    'p = &(value);',
    'p = &((value));',
    'p = & /* storage */ ( value );',
    'KeepPointer((void *)&(value));',
    'p = &(( /* owner */ value ));',
])
def test_parenthesized_address_stays_attached_to_modified_storage(escape):
    source='value = 0x12345678;\n'+escape+'\nvalue = CONCAT31(value._1_3_,1);\nObserve(*p);\n'
    assert fold_low_byte_flags(source)==source

@pytest.mark.parametrize('prefix',[
    'text = "&(value)";',
    '// p = &(value);',
    '/* p = &((value)); */',
    'p = &(value_other);',
    'snapshot = value;',
])
def test_opaque_or_unrelated_prefix_does_not_block_byte_lifetime(prefix):
    source=prefix+'\nvalue = CONCAT31(value._1_3_,1);\nif ((char)value == 0) { Done(); }\n'
    output=fold_low_byte_flags(source)
    assert 'value_flag = 1;' in output
    assert prefix in output
