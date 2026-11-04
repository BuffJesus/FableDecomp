"""Immutable book definitions: typed reads, CFG joins and generated inventory operands."""
import json
from pathlib import Path
from types import SimpleNamespace
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_evidence_lowering import LoweringSpec, lower_string_snapshots
from tools.script_recovery.native_member_resource_calls import recover_definition_vector_loads
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.test_book_specific_args import generated

ROOT=Path(__file__).resolve().parents[2]

def spec(address=0xe57530):
    unit=json.loads((ROOT/'refs/script_recovery/book_collecting/units/V_BookCollecting.json').read_text())
    s=LoweringSpec(unit,'BS_Teacher',entity=True,thing_slots={});s.native_address=address
    return s

ELEMENT='(CCharString_bv *)(*(int *)(*(int *)(this + 0x14) + 0x94) + (int)index * 4)'

@pytest.mark.parametrize('address,expected',[(0xe57530,True),(0xe55ce0,True),(0xe54e90,True),(0xe54990,False),(0,False)])
def test_reader_evidence_excludes_init_writes_and_unknown_functions(address,expected):
    out=lower_string_snapshots('consume('+ELEMENT+');',spec(address))
    assert ('ENGINE_StateStringVectorAt("BookDef", index)' in out)==expected

@pytest.mark.parametrize('change',[('0x94','0xa0'),('0x14','0x18'),('* 4','* 12'),('CCharString_bv','CScriptThing')])
def test_wrong_owner_member_stride_and_element_are_not_lowered(change):
    source='consume('+ELEMENT.replace(*change)+');'
    assert lower_string_snapshots(source,spec())==source

@pytest.mark.parametrize('middle,expected',[
    ('other = GetHero();',True),('if (flag) {',False),('}',False),('LAB_1234:',False),
    ('goto done;',False),('overwrite(&ptr);',False),('ptr = other;',False),('ptr++;',False),
    ('if (flag) ptr = other;',False),('++ptr;',False),('ptr |= other;',False),('ptr <<= 1;',False)])
def test_alias_read_stays_in_uninterrupted_block(middle,expected):
    source='ptr = *(int *)(*(int *)(this + 0x14) + 0x94);\n'+middle+'\nconsume((CCharString *)(ptr + index * 4));'
    out=lower_string_snapshots(source,spec())
    assert ('ENGINE_StateStringVectorAt' in out)==expected

@pytest.fixture(scope='module')
def native():
    functions=json.loads((ROOT/'refs/script_recovery/book_collecting/translation_unit_typed.json').read_text())['functions']
    return next(f for f in functions if f['address'].lower()=='0x00e57530'),RData()

@pytest.mark.parametrize('site',[None,0xe5753a,0xe5764e,0xe57574])
def test_actual_loop_requires_entry_backedge_and_member_load_agreement(native,site):
    fn,r=native
    def read(address,count):
        b=r.bytes_at(address,count)
        if site is not None and address<=site<address+count:
            b=bytearray(b);b[site-address]^=4;return bytes(b)
        return b
    out=recover_definition_vector_loads(fn['decompile'],fn,spec(),SimpleNamespace(bytes_at=read))
    assert ('iVar7 = *(int *)(*(int *)(this + 0x14) + 0x94);' in out)==(site is None)

@pytest.mark.parametrize('count',[0,1,3,9])
def test_generated_lookup_reads_each_definition_index_and_hero(generated,count):
    for _,source in generated:
        lua=LuaRuntime(unpack_returned_tuples=True)
        lua.execute('package.preload["V_BookCollecting.native_quest_helpers"] = function() return {} end')
        lua.execute(source)
        events=[]
        def read(_,offset,index):
            events.append(('read',offset,index));return 'BOOK_'+str(index)
        def owned(_,book,hero):
            events.append(('owned',book,hero));return False
        q=lua.table_from({'RetailResources':lambda _:lua.table(),
            'ReadGlobalGameData':lambda _,offset:{1248:10,1252:20,1256:30}.get(offset,0),
            'ReadGlobalGameDataFloat':lambda *_:0,
            'GetStateInt':lambda _,key:count if key=='BooksInGame' else -1,
            'GetHero':lambda _:'hero', 'GetStateStringVectorAt':read,
            'IsObjectInThingsPossession':owned, 'IsActiveThreadTerminating':lambda _:True})
        lua.globals().Init(q,'teacher')
        f=lua.globals().LookForBook or lua.globals().helper_E57530
        f(q,'teacher')
        assert events==[event for i in range(count) for event in [('read','BookDef',i),('owned','BOOK_'+str(i),'hero')]]


@pytest.mark.parametrize('index',[0,15,23])
@pytest.mark.parametrize('terminate',[False,True])
def test_generated_offer_passes_selected_definition_to_removal(generated,index,terminate):
    # Stop at the removal boundary: donated-byte writes and later completion
    # remain unresolved and this test must not pretend to validate them.
    for _,source in generated:
        lua=LuaRuntime(unpack_returned_tuples=True)
        lua.execute('package.preload["V_BookCollecting.native_quest_helpers"] = function() return {} end')
        lua.execute(source)
        events=[]
        class RemovalReached(Exception): pass
        def read(_,offset,value):
            events.append(('read',offset,value));return 'BOOK_'+str(value)
        def take(_,book):
            events.append(('take',book));raise RemovalReached()
        q=lua.table_from({'RetailResources':lambda _:lua.table(), 'GetHero':lambda _:'hero',
            'GiveHeroYesNoQuestion':lambda _,key,*a:events.append(('question',key)),
            'MsgIsQuestionAnsweredYesOrNo':lambda _:1,
            'IsActiveThreadTerminating':lambda _:terminate,
            'SetStateBool':lambda _,key,value:events.append(('state',key,value)),
            'GetStateStringVectorAt':read,'TakeObjectFromHero':take})
        fn=lua.globals().AskForBook or lua.globals().helper_E55CE0
        if terminate:
            fn(q,'teacher',index,'decline')
            assert events==[('question','TEXT_QST_B16_OFFER_BOOK_'+str(index))]
        else:
            with pytest.raises(RemovalReached):fn(q,'teacher',index,'decline')
            assert events==[('question','TEXT_QST_B16_OFFER_BOOK_'+str(index)),
                            ('state','ReadingBook',True),('read','BookDef',index),('take','BOOK_'+str(index))]
