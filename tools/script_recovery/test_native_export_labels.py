import pytest
from tools.script_recovery.native_export_labels import normalize

NAME='override_prt_d07c09_8df7e253'
SOURCE=f'''if (cancel) goto {NAME};
live();
{NAME}:
cleanup();
return;
'''

@pytest.mark.parametrize('newline',['\n','\r\n'])
def test_definition_and_reference_renamed_together(newline):
    source=SOURCE.replace('\n',newline)
    assert normalize(source)==source.replace(NAME,'LAB_00d07c09')

@pytest.mark.parametrize('occupied', ['LAB_00d07c09','LAB_00d07c09 LAB_00d07c09_c1'])
def test_existing_identifiers_are_never_merged(occupied):
    source=occupied+';\n'+SOURCE
    new='LAB_00d07c09_c2' if '_c1' in occupied else 'LAB_00d07c09_c1'
    assert normalize(source)==source.replace(NAME,new)

@pytest.mark.parametrize('opaque',[f'"goto {NAME};"',f"'goto {NAME};'",f'// goto {NAME};',f'/*\n{NAME}:\ngoto {NAME};\n*/'])
def test_literals_and_comments_remain_unchanged(opaque):
    assert normalize(opaque+'\n'+SOURCE)==opaque+'\n'+SOURCE.replace(NAME,'LAB_00d07c09')

@pytest.mark.parametrize('source',[
    f'goto {NAME};',
    SOURCE+f'{NAME}:\nreturn;',
    SOURCE+f'ptr = &&{NAME};',
    SOURCE.replace(NAME,'unknown_label'),
    SOURCE.replace(NAME,'override_prt_d07c09_bad_hash'),
    f'{NAME}::call();',
])
def test_ambiguous_or_unsupported_names_are_not_changed(source):
    assert normalize(source)==source

def test_distinct_exported_labels_at_same_address_remain_distinct():
    source=SOURCE+SOURCE.replace('8df7e253','abcdef12')
    assert normalize(source)==SOURCE.replace(NAME,'LAB_00d07c09')+SOURCE.replace(NAME,'LAB_00d07c09_c1')
    assert normalize(normalize(source))==normalize(source)
