import copy,json,struct
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import definition_conversation_fixture as c
from tools.script_recovery.definition_vector_fixture import stock_fixture,to_lua,validate
from tools.script_recovery.script_def_offsets import load
from tools.script_recovery.smoke_run_unit import HARNESS
R=Path(__file__).resolve().parents[2]

@pytest.fixture(scope='module')
def stock():
 metadata=load();fixture=stock_fixture();fixture['conversations']=c.decode_conversations(metadata);fixture['schema']='script-definition-vector-fixture/2'
 return fixture,metadata

def execute(source,stock):
 fixture,metadata=stock;lua=LuaRuntime(unpack_returned_tuples=True)
 runner=lua.execute('return function(SOURCE,NAME,FUNCTIONS,FRAME_BUDGET,INSTRUCTION_LIMIT,PARAM_KINDS,ENTITY_FILE,DEFINITION_FIXTURE) '+HARNESS+' end')
 return runner(source,'conversation_fixture.lua',lua.table('Main'),100,1000,lua.table(),False,to_lua(lua,fixture,metadata))['Main']

def test_stock_values_and_independent_vector_counts(stock):
 fixture,metadata=stock;groups=fixture['conversations'];assert len(groups)==1
 group=groups[0];assert (group['offset'],group['name'])==(0x4c8,'BookReactions')
 assert len(group['records'])==25
 assert [(i,row['Lines']) for i,row in enumerate(group['records']) if row['Lines']]==[(2,7),(12,9),(17,6),(20,10),(24,7)]
 assert group['records'][2]['Speaker']==['boy0','girl0','boy0','girl0','boy0','girl0','boy0']
 assert group['records'][2]['Dialogue'][1]=='TEXT_QST_B16_BOOK02_REACTION_10'
 assert group['records'][24]['Dialogue'][:6]==['NULL']*6
 assert {name:sum(len(row[name]) for row in group['records']) for name in c.STRINGS}==dict.fromkeys(c.STRINGS,39)

def test_all_stock_truncations_and_trailing_bytes_are_rejected(stock):
 raw=bytes.fromhex(stock[1]['0x4c8']['value'])
 for end in range(len(raw)):
  with pytest.raises(ValueError):c.decode_records(raw[:end])
 with pytest.raises(ValueError):c.decode_records(raw+b'\0')

@pytest.mark.parametrize('position',[4,12,20,28,36])
def test_changed_record_tags_are_rejected(stock,position):
 raw=bytearray.fromhex(stock[1]['0x4c8']['value']);raw[position]^=1
 with pytest.raises(ValueError,match='tag/order'):c.decode_records(bytes(raw))

@pytest.mark.parametrize('position,value',[(0,4097),(8,0xffffffff),(8,4097),(16,4097)])
def test_invalid_counts_and_line_bounds_are_rejected(stock,position,value):
 raw=bytearray.fromhex(stock[1]['0x4c8']['value']);struct.pack_into('<I',raw,position,value)
 with pytest.raises(ValueError):c.decode_records(bytes(raw))

def serialized(row):
 out=struct.pack('<I',1)
 for name in c.FIELDS:
  out+=struct.pack('<I',c.field_tag(name))
  if name=='Lines':out+=struct.pack('<i',row[name])
  else:
   out+=struct.pack('<I',len(row[name]))
   out+=b''.join(value+b'\0' for value in row[name])
 return out

def test_independent_vector_sizes_are_preserved():
 row=dict(Lines=2,Speaker=[b'boy0'],Dialogue=[b'NULL',b'X',b'Y'],Animation=[],AnimLoop=[b'',b'NULL'])
 decoded=c.decode_records(serialized(row))[0]
 assert decoded==dict(Lines=2,Speaker=['boy0'],Dialogue=['NULL','X','Y'],Animation=[],AnimLoop=['','NULL'])

@pytest.mark.parametrize('fault',['missing_terminator','invalid_utf8','long_string'])
def test_invalid_string_data_is_rejected(fault):
 row=dict(Lines=1,Speaker=[b'X'],Dialogue=[],Animation=[],AnimLoop=[])
 if fault=='invalid_utf8':row['Speaker']=[b'\xff']
 if fault=='long_string':row['Speaker']=[b'X'*16385]
 raw=serialized(row)
 if fault=='missing_terminator':raw=raw[:21]
 with pytest.raises(ValueError):c.decode_records(raw)

@pytest.mark.parametrize('fault',['duplicate','wrong_name','scalar','unaligned','bool_offset','unanchored','negative_lines','bool_lines','missing_field','extra_field','nonstring','nul','huge_lines','overlap'])
def test_fixture_shapes_are_checked(stock,fault):
 fixture,metadata=copy.deepcopy(stock);group=fixture['conversations'][0];row=group['records'][2]
 if fault=='duplicate':fixture['conversations'].append(copy.deepcopy(group))
 if fault=='wrong_name':group['name']='Invented'
 if fault=='scalar':group['offset']=0x168
 if fault=='unaligned':group['offset']+=1
 if fault=='bool_offset':group['offset']=True
 if fault=='unanchored':metadata['0x4c8'].pop('retailTransferCall')
 if fault=='negative_lines':row['Lines']=-1
 if fault=='bool_lines':row['Lines']=True
 if fault=='missing_field':row.pop('Speaker')
 if fault=='extra_field':row['Invented']=[]
 if fault=='nonstring':row['Speaker'][0]=1
 if fault=='nul':row['Speaker'][0]='A\0B'
 if fault=='huge_lines':row['Lines']=4097
 if fault=='overlap':metadata['0x4cc']={'name':'Collision','defType':'int32'}
 with pytest.raises(ValueError):validate(fixture,metadata)

def test_integration_global_values_and_native_stride(stock):
 result=execute('''function Main(q)
 local rows=q:GlobalConversations(0x4c8)
 assert(#rows==25 and rows[3].Lines==7)
 assert(rows[3].Speaker[1]=='boy0' and rows[3].Dialogue[2]=='TEXT_QST_B16_BOOK02_REACTION_10')
 assert(q:ReadGlobalGameData(0x4cc)-q:ReadGlobalGameData(0x4c8)==25*0x5c)
 assert(q:ReadGlobalGameData(0x4d0)==q:ReadGlobalGameData(0x4cc))
 rows[3].Speaker[1]='CHANGED';assert(q:GlobalConversations(0x4c8)[3].Speaker[1]=='boy0')
 end''',stock)
 assert result['ok'],result['error']

def test_integration_owned_snapshot_and_returned_copy(stock):
 result=execute('''function Main(q)
 assert(#q:GetStateConversations('BookReactions')==0)
 q:SnapshotGlobalConversations('BookReactions',0x4c8)
 local rows=q:GetStateConversations('BookReactions');assert(#rows==25 and rows[3].Lines==7)
 rows[3].Lines=-1;rows[3].Dialogue[2]='CHANGED'
 local next=q:GetStateConversations('BookReactions');assert(next[3].Lines==7 and next[3].Dialogue[2]=='TEXT_QST_B16_BOOK02_REACTION_10')
 q:SnapshotGlobalConversations('Other',0x4c8);assert(#q:GetStateConversations('Other')==25)
 assert(#q:GetStateConversations('Missing')==0)
 end''',stock)
 assert result['ok'],result['error']

@pytest.mark.parametrize('book,lines',[(2,7),(12,9),(17,6),(20,10),(24,7)])
def test_integration_actual_book_reaction_calls_all_lines(stock,book,lines):
 source=(R/'refs/script_recovery/lifted/BookCollecting/readable/FSE/V_BookCollecting/V_BookCollecting.lua').read_text(encoding='utf-8')
 source+=f'''
 function Main(q)
 Init(q)
 local old=DoConversation;local n=0
 DoConversation=function(quest,book,line) n=n+1;assert(book=={book} and line==n-1);return old(quest,book,line) end
 BookReaction(q,{book});assert(n=={lines})
 end
 '''
 result=execute(source,stock);assert result['ok'],result['error']
 assert result['frames']==lines

@pytest.mark.parametrize('offset',['0x4cc','-1','nil'])
def test_integration_unknown_conversation_offset_is_explicit_error(stock,offset):
 result=execute('function Main(q) q:GlobalConversations('+offset+') end',stock)
 assert not result['ok'] and 'unknown conversation offset' in result['error']

def test_snapshot_store_is_per_mock_quest(stock):
 first=execute("function Main(q) q:SnapshotGlobalConversations('x',0x4c8);assert(#q:GetStateConversations('x')==25) end",stock)
 second=execute("function Main(q) assert(#q:GetStateConversations('x')==0) end",stock)
 assert first['ok'] and second['ok']


def test_empty_conversation_vector_stays_empty(stock):
 fixture,metadata=copy.deepcopy(stock);fixture['conversations'][0]['records']=[]
 result=execute("function Main(q) assert(#q:GlobalConversations(0x4c8)==0);assert(q:ReadGlobalGameData(0x4c8)==q:ReadGlobalGameData(0x4cc));q:SnapshotGlobalConversations('empty',0x4c8);assert(#q:GetStateConversations('empty')==0) end",(fixture,metadata))
 assert result['ok'],result['error']

def test_conversation_index_is_not_padded_or_clamped(stock):
 result=execute("function Main(q) q:SnapshotGlobalConversations('x',0x4c8);local row=q:GetStateConversations('x')[26];local n=row.Lines end",stock)
 assert not result['ok'] and 'nil value' in result['error']


@pytest.mark.parametrize('fault',['old_with_conversations','new_without_conversations'])
def test_extended_fixture_schema_is_explicit(stock,fault):
 fixture,metadata=copy.deepcopy(stock)
 if fault=='old_with_conversations':fixture['schema']='script-definition-vector-fixture/1'
 else:fixture.pop('conversations')
 with pytest.raises(ValueError,match='schema version 2'):validate(fixture,metadata)
