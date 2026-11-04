import pytest
from tools.script_recovery.native_evidence_lowering import fold_counted_map_stores

LABEL='CCountedPointer<T>::operator=__at8ab1e0'
SOURCE='''resource = RESOURCE_NewResource();
actors = ACTORMAP_New();
CCharString::CCharString(&key,"HERO",-1);
node = std::map<CCharString,CCountedPointer<T>>::operator[](actors,&key);
CCountedPointer<T>::operator=__at8ab1e0((CCountedPointer<T> *)(node + 8), (int)&((CScriptThing *)resource)->field_0x8);
'''

@pytest.mark.parametrize('newline',['\n','\r\n'])
@pytest.mark.parametrize('space',['',' '])
def test_resource_field_copy(newline,space):
    source=SOURCE.replace(', (int)',','+space+'(int)').replace('\n',newline)
    result=fold_counted_map_stores(source,{LABEL:0x8ab1e0})
    assert 'ACTORMAP_Set(actors, "HERO", resource);' in result
    assert 'operator=' not in result

@pytest.mark.parametrize('old,new',[
    ('RESOURCE_NewResource()', 'QUESTTHING_Empty()'),
    ('ACTORMAP_New()', 'RESOURCE_NewResource()'),
    ('field_0x8','field_0xc'),
    ('(node + 8)','(node + 4)'),
    ('(node + 8)','(other + 8)'),
    ('*)resource','*)other'),
    ('actors,&key','actors,&other'),
    ('&((CScriptThing *)resource)','((CScriptThing *)resource)'),
    ('CCharString::CCharString','resource = unknown;\nCCharString::CCharString'),
    ('CCharString::CCharString','actors = unknown;\nCCharString::CCharString'),
    ('CCharString::CCharString','RESOURCE_ReleaseResource(resource);\nCCharString::CCharString'),
    ('CCharString::CCharString','ACTORMAP_Destroy(actors);\nCCharString::CCharString'),
    ('CCharString::CCharString','if (condition) {\nCCharString::CCharString'),
    ('CCharString::CCharString','join:\nCCharString::CCharString'),
])
def test_ambiguous_field_copy_remains_unresolved(old,new):
    source=SOURCE.replace(old,new)
    assert fold_counted_map_stores(source,{LABEL:0x8ab1e0})==source

def test_wrong_native_target_remains_unresolved():
    assert fold_counted_map_stores(SOURCE,{LABEL:0x8ab1e1})==SOURCE

def test_repeated_store_after_namespaced_call():
    source=SOURCE+SOURCE[SOURCE.index('CCharString::'):]
    result=fold_counted_map_stores(source,{LABEL:0x8ab1e0})
    assert result.count('ACTORMAP_Set(actors, "HERO", resource);')==2

def test_preexisting_plain_resource_spelling():
    source=SOURCE.replace('&((CScriptThing *)resource)->field_0x8','resource')
    assert 'ACTORMAP_Set(actors, "HERO", resource);' in fold_counted_map_stores(source,{LABEL:0x8ab1e0})
