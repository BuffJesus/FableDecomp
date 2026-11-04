from types import SimpleNamespace
import pytest

from tools.script_recovery.native_member_string_refs import lower_member_string_refs as lower
def metadata(entity=True):
 return SimpleNamespace(entity=entity,parent_arrays=[dict(name=n,base=b,element='CCharString',stride=4,count=5,members={0:('','String')}) for n,b in [('Male',316),('Female',336)]],self_arrays=[])
SOURCE='''CCharString *pOther;
if (sex == 1) {
  pOther = (CCharString *)(*(int *)(this + 0x14) + 0x13c + (choice % 5) * 4);
  goto join;
}
pOther = (CCharString *)(*(int *)(this + 0x14) + 0x150 + (choice % 5) * 4);
join:
MUTATE_PARENT_FIELD();
CCharString::operator=((CCharString *)&destination,pOther);
'''
def test_join_reads_selected_key_after_mutation():
 out=lower(SOURCE,metadata())
 assert 'pOther = ENGINE_Concat("Male_",ENGINE_IntToString((choice % 5)));' in out
 assert 'pOther = ENGINE_Concat("Female_",ENGINE_IntToString((choice % 5)));' in out
 assert out.count('QUESTSTATE_GetString(pOther)')==1
 assert out.index('MUTATE_PARENT_FIELD')<out.index('QUESTSTATE_GetString')<out.index('CCharString::operator=')
 assert 'this + 0x14' not in out

@pytest.mark.parametrize('extra',[
 'ESCAPE(pOther);','pOther = other;','other = pOther;','*pOther = 0;',
 'CCharString::CCharString(&other,pOther);','return pOther;',
 'pOther++;','pOther[0] = value;','if (pOther == other) {}',
 'CCharString::operator=(pOther,other);',
])
def test_unknown_pointer_uses_refuse_whole_alias(extra):
 source=SOURCE+extra+'\n';assert lower(source,metadata())==source

@pytest.mark.parametrize('change',[
 {'stride':8},{'count':0},{'element':'int'},{'members':{0:('','Int')}},
 {'members':{0:('text','String')}},{'members':{4:('','String')}},
])
def test_incompatible_metadata_stays_diagnostic(change):
 meta=metadata()
 for item in meta.parent_arrays:item.update(change)
 assert lower(SOURCE,meta)==SOURCE

def test_ambiguous_metadata_refuses_alias():
 meta=metadata();meta.parent_arrays.append(dict(meta.parent_arrays[0],name='Ambiguous'))
 assert lower(SOURCE,meta)==SOURCE

def test_mixed_ownership_refuses_alias():
 meta=metadata();meta.self_arrays=[dict(meta.parent_arrays[1])]
 source=SOURCE.replace('*(int *)(this + 0x14) + 0x150','this + 0x150')
 assert lower(source,meta)==source

@pytest.mark.parametrize('text',['// pOther is an address\n','/* pOther retained here */\n','LOG("pOther");\n'])
def test_comments_and_string_mentions_are_not_pointer_escapes(text):
 assert 'QUESTSTATE_GetString(pOther)' in lower(SOURCE+text,metadata())

@pytest.mark.parametrize('expression',[
 '*(int *)(this + 0x14) + 0x13c + index * 4',
 'index * 4 + 316 + *(int *)(this + 0x14)',
 '*(int *)(this + 0x14) + index * 4 + 316',
])
def test_equivalent_address_shapes(expression):
 source=f'p = (CCharString *)({expression});\nCCharString::operator=(&out,p);\n'
 assert 'QUESTSTATE_GetString(p)' in lower(source,metadata())

def test_self_array_uses_entity_state():
 meta=metadata();meta.self_arrays=meta.parent_arrays;meta.parent_arrays=[]
 source=SOURCE.replace('*(int *)(this + 0x14)','this')
 assert 'ENTITYSTATE_GetString(pOther)' in lower(source,meta)

def test_quest_array_uses_quest_state():
 meta=metadata(False);meta.self_arrays=meta.parent_arrays;meta.parent_arrays=[]
 source=SOURCE.replace('*(int *)(this + 0x14)','this')
 assert 'QUESTSTATE_GetString(pOther)' in lower(source,meta)

@pytest.mark.parametrize('index',['next_index()','(index + 1)','(index % 0)','(index % -5)'])
def test_unrecognized_index_keeps_original(index):
 source=SOURCE.replace('(choice % 5)',index);assert lower(source,metadata())==source

def test_generated_temporary_cannot_collide():
 out=lower(SOURCE+'int native_arg_member_string_value_1;\n',metadata())
 assert 'native_arg_member_string_value_2 = QUESTSTATE_GetString' in out

def test_no_consumer_does_not_rewrite_address():
 source=SOURCE.split('CCharString::operator=')[0];assert lower(source,metadata())==source

