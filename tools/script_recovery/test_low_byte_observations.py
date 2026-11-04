import pytest

from tools.script_recovery.native_evidence_lowering import fold_low_byte_flags


@pytest.mark.parametrize('tail', [
    'Observe(value);', 'copy = value;', 'return value;',
    'Observe(&value);', 'value = value + 1;', 'if (value == 0) { Done(); }',
    'value = CONCAT31(value._1_3_,value + 1);',
])
def test_full_word_observation_refuses_split(tail):
    source = 'value = 0x12345678;\nvalue = CONCAT31(value._1_3_,1);\n' + tail + '\n'
    assert fold_low_byte_flags(source) == source


@pytest.mark.parametrize('source', [
    'do {\nObserve(value);\nvalue = CONCAT31(value._1_3_,1);\n} while (again);\n',
    'while (value) {\nvalue = CONCAT31(value._1_3_,1);\n}\n',
    'for (; value; ) {\nvalue = CONCAT31(value._1_3_,1);\n}\n',
    'while (value)\nvalue = CONCAT31(value._1_3_,1);\n',
    'LAB_1:\nObserve(value);\nvalue = CONCAT31(value._1_3_,1);\ngoto LAB_1;\n',
    'p = &value;\nvalue = CONCAT31(value._1_3_,1);\nObserve(*p);\n',
    'Observe((char)value);\nvalue = CONCAT31(value._1_3_,1);\n',
])
def test_prefix_read_or_escape_can_observe_a_later_store(source):
    assert fold_low_byte_flags(source) == source


def test_entry_input_reads_and_separate_colour_lifetime_still_fold():
    source = '''int Handle(int value)
{
copy = value;
if (value == 2) {
value = CONCAT31(value._1_3_,bVar1);
if ((char)value == '\\0') { Use(); }
value = -0x10000;
Bar((CRGBColour_bv *)&value);
}
}
'''
    output = fold_low_byte_flags(source)
    assert 'value_flag = bVar1;' in output
    assert 'copy = value;' in output
    assert 'Bar((CRGBColour_bv *)&value);' in output


def test_closed_earlier_loop_does_not_make_entry_read_live():
    source = '''copy = value;
do {
copy = copy - 1;
} while (copy);
value = CONCAT31(value._1_3_,bVar1);
if ((char)value != '\\0') { Done(); }
'''
    assert 'value_flag = bVar1;' in fold_low_byte_flags(source)


def test_byte_only_loop_keeps_recovery():
    source = '''value = CONCAT31(value._1_3_,bVar1);
while (again) {
if ((char)value != '\\0') { Done(); }
value = CONCAT31(value._1_3_,bVar1);
}
'''
    assert 'value_flag = bVar1;' in fold_low_byte_flags(source)
