"""Owned conversation initialization, reader arguments and strict native lowering."""
import copy
from types import SimpleNamespace
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_conversation_snapshots import lower_owned_conversation_snapshots
from tools.script_recovery.test_book_reaction_lowering import generated


def spec(address=0xe54990):
    return SimpleNamespace(entity=False,native_address=address,call_labels={'assign':0xe54aa0},
        parent_conversation_snapshots={0x4c:{'name':'BookReactions','global':'0x4c8',
            'element':'CConversation','stride':92,'storage':'questConversationVector',
            'initialization':{'functions':['0x00e54990'],'assign':'0x00e54aa0'},
            'readFunctions':['0x00e566f0','0x00e569d0']}})


COPY='assign((int)this + 0x4c, DAT_0143e90c + 0x4c8);'
LINES='*(int *)(book * 0x5c + 0x28 + *(int *)(this + 0x4c))'
STRING='(CCharString *)(*(int *)(record + 0x2c + *(int *)(this + 0x4c)) + line)'


def test_exact_copy_and_reader_operands():
    assert lower_owned_conversation_snapshots(COPY,spec())=='GSI->SnapshotGlobalConversations("BookReactions", 0x4c8);'
    assert lower_owned_conversation_snapshots(LINES,spec(0xe566f0))=='ENGINE_StateConversationLines("BookReactions", book)'
    source='record = book * 0x5c;\nline = number * 4;\nconsume('+STRING+');'
    assert 'ENGINE_StateConversationString("BookReactions", book, "Speaker", number)' in lower_owned_conversation_snapshots(source,spec(0xe569d0))


@pytest.mark.parametrize('source',[COPY.replace('0x4c,','0x50,'),COPY.replace('0x4c8','0x4d4'),COPY.replace('assign','unknown'),COPY.replace(');',', extra);')])
def test_unproven_copy_remains_native(source):
    assert lower_owned_conversation_snapshots(source,spec())==source


@pytest.mark.parametrize('source',[LINES,STRING,COPY])
def test_unlisted_function_remains_native(source):
    assert lower_owned_conversation_snapshots(source,spec(0xe54e90))==source


@pytest.mark.parametrize('middle',['record = other;','if (flag) {','escape(&record);','record++;','++record;','record <<= 1;','LAB_001234:','escape(& (record));'])
def test_read_never_substitutes_stale_scaled_index(middle):
    source='record = book * 0x5c;\n'+middle+'\nconsume('+STRING+');'
    out=lower_owned_conversation_snapshots(source,spec(0xe569d0))
    assert 'ENGINE_StateConversationString("BookReactions", (record) / 0x5c,' in out


@pytest.mark.parametrize('change',[('0x4c','0x50'),('0x5c','0x60'),('0x28','0x24')])
def test_wrong_record_layout_not_lowered(change):
    source=LINES.replace(*change)
    assert lower_owned_conversation_snapshots(source,spec(0xe566f0))==source


def test_generated_init_retains_all_conversation_fields_after_source_mutation(generated):
    _,roots,_=generated
    for source in roots:
        lua=LuaRuntime();lua.execute(source)
        original={'Lines':1,'Speaker':['boy0'],'Dialogue':['OLD_TEXT'],'Animation':['OLD_ANIM'],'AnimLoop':['OLD_LOOP']}
        globals_data=[copy.deepcopy(original)];stored={};state={};events=[]
        def snapshot(_,name,offset):
            assert (name,offset)==('BookReactions',0x4c8)
            stored[name]=copy.deepcopy(globals_data)
        def read(_,name):
            assert name=='BookReactions'
            return lua.table_from(stored[name],recursive=True)
        def record(name):return lambda _,*args:events.append((name,*args))
        resources=lua.table_from({'MemberResource':lambda _,name:name,
            'PlayAnimation':record('animation'),'PlayLoopingAnimation':record('loop'),
            'IsPerformingScriptTask':lambda *_:False})
        q=lua.table_from({'RetailResources':lambda _:resources,
            'SnapshotGlobalConversations':snapshot,'GetStateConversations':read,
            'SetStateBool':lambda _,k,v:state.__setitem__(k,v),'SetStateInt':lambda _,k,v:state.__setitem__(k,v),
            'GetStateInt':lambda _,k:state[k],'GetStateBool':lambda _,k:state[k],
            'ReadGlobalGameData':lambda *_:0,'StateStringVectorResize':lambda *_:None,
            'StateFlagVectorResize':lambda *_:None,'GetHero':lambda _:'hero',
            'GetThingWithScriptName':lambda _,name:name,'IsActiveThreadTerminating':lambda _:False,
            'AddNewConversation':lambda *_:73,'AddPersonToConversation':lambda *_:None,
            'AddLineToConversation':record('line'),'RemoveConversation':record('remove'),
            'IsConversationActive':lambda *_:False,'NewScriptFrame':lambda *_:True})
        lua.globals().Init(q)
        globals_data[0]={'Lines':1,'Speaker':['girl0'],'Dialogue':['NEW_TEXT'],'Animation':['NEW_ANIM'],'AnimLoop':['NEW_LOOP']}
        fn=lua.globals().DoConversation or lua.globals().helper_E569D0
        for expected in [original,globals_data[0]]:
            events.clear();fn(q,0,0)
            assert ('line',73,expected['Dialogue'][0],expected['Speaker'][0],'hero',False) in events
            assert [e[2] for e in events if e[0]=='animation']==expected['Animation']
            assert [e[2] for e in events if e[0]=='loop']==expected['AnimLoop']
            lua.globals().Init(q)
        globals_data.clear();lua.globals().Init(q)
        assert stored['BookReactions']==[]
